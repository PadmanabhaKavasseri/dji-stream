/**
 ********************************************************************
 *
 * @copyright (c) 2023 DJI. All rights reserved.
 *
 * All information contained herein is, and remains, the property of DJI.
 * The intellectual and technical concepts contained herein are proprietary
 * to DJI and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of DJI.
 *
 * If you receive this source code without DJI’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify DJI of its removal. DJI reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */
#include "stream_processor_thread.h"

#include <cerrno>
#include <cstring>

#include "image_processor_thread.h"
#include "logger.h"
#include "stream_decoder.h"

#include <iostream>



using namespace edge_sdk;

namespace edge_app {

void StreamProcessorThread::SetupPipeline(){
    // Initialize GStreamer
    gst_init(nullptr, nullptr);

    // Create pipeline
    pipeline_ = gst_pipeline_new("video_pipeline");
    if (!pipeline_) {
        std::cerr << "Failed to create pipeline." << std::endl;
    }

    //create elements
    appsrc_ = gst_element_factory_make("appsrc", "app_src");
    app_queue_ = gst_element_factory_make("queue", "app_queue");
    waylandsink_ = gst_element_factory_make("waylandsink", "wayland_sink");

    // Check if elements were created successfully 
    if (!appsrc_ || !app_queue_ || !waylandsink_) { 
        std::cerr << "Failed to create one or more GStreamer elements." << std::endl;
         return; 
     }
    // Set appsrc properties 
    GstCaps *caps = gst_caps_new_simple( "video/x-raw", "format", G_TYPE_STRING, "I420", // YUV format 
        "width", G_TYPE_INT, 1920, // Set your width 
        "height", G_TYPE_INT, 1080, // Set your height 
        "framerate", GST_TYPE_FRACTION, 30, 1, // Example framerate 
        nullptr );
    g_object_set(appsrc_, "caps", caps, nullptr);
    gst_caps_unref(caps);
    g_object_set(appsrc_, "block", FALSE, nullptr);

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline_), appsrc_, app_queue_, waylandsink_, nullptr);

    // Link elements 
    if (!gst_element_link_many(appsrc_, app_queue_, waylandsink_, nullptr)) { 
        std::cerr << "Failed to link GStreamer elements: appsrc to waylandsink." << std::endl; 
        return;
    }

    // Set the pipeline to playing state
    gst_element_set_state(pipeline_, GST_STATE_PLAYING);

}

void StreamProcessorThread::WriteDataToFile(const std::vector<uint8_t>& data){
    // Write the data to the file
    outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    // Check if the write operation succeeded
    if (!outFile) {
        throw std::runtime_error("Error occurred while writing data to the file.");
    }
}

void StreamProcessorThread::PushFrameToAppsrc(AVFrame *frame){
    if (!appsrc_) {
        std::cerr << "Error: appsrc is not initialized." << std::endl;
        return;
    }
    // Calculate the buffer size needed for the frame
    int size = av_image_get_buffer_size((AVPixelFormat)frame->format, frame->width, frame->height, 1);

    // Create a new GStreamer buffer and fill it with the frame data
    GstBuffer *buffer = gst_buffer_new_allocate(nullptr, size, nullptr);

    // Map the buffer
    GstMapInfo map;
    if (!gst_buffer_map(buffer, &map, GST_MAP_WRITE)) { 
        std::cerr << "Failed to map GStreamer buffer." << std::endl; gst_buffer_unref(buffer); 
        return; 
    }
    // Copy the frame data to the buffer 
    av_image_copy_to_buffer(map.data, size, frame->data, frame->linesize, (AVPixelFormat)frame->format, frame->width, frame->height, 1);

    // Unmap the buffer 
    gst_buffer_unmap(buffer, &map);

    // Push the buffer into the appsrc element
    GstFlowReturn ret;
    g_signal_emit_by_name(appsrc_, "push-buffer", buffer, &ret);
    gst_buffer_unref(buffer);
    if (ret != GST_FLOW_OK) { 
        std::cerr << "Failed to push buffer to appsrc. GstFlowReturn: " << ret << std::endl; 
    }
}


StreamProcessorThread::StreamProcessorThread(const std::string& name)
    : processor_name_(name) {
    processor_start_ = false;

    SetupPipeline();

    // const std::string fileName = "output.bin";
    // outFile.open(fileName, std::ios::binary);
    // if (!outFile) {
    //     throw std::runtime_error("Failed to open the file for writing.");
    // }

}

StreamProcessorThread::~StreamProcessorThread() { Stop(); }

int32_t StreamProcessorThread::SetStreamDecoder(
    std::shared_ptr<StreamDecoder> decoder) {
    if (decoder == nullptr) {
        return -1;
    }
    stream_decoder_ = decoder;
    return 0;
}

int32_t StreamProcessorThread::SetImageProcessorThread(
    std::shared_ptr<ImageProcessorThread> image_processor) {
    if (!image_processor) {
        return -1;
    }
    image_processor_thread_ = image_processor;
    return 0;
}

void StreamProcessorThread::InputStream(const uint8_t* data, size_t length) {
    std::lock_guard<std::mutex> l(decode_vector_mutex_);
    decode_vector_.insert(decode_vector_.end(), data, data + length);
    decode_vector_cv_.notify_one();
}

void StreamProcessorThread::DecodeFrame(const uint8_t *data, size_t length){

    std::cout << "StreamProcessorThread::DecodeFrame: data size: " << length << std::endl;
    const uint8_t *pData = data;
    int remainingLen = length;
    int processedLen = 0;

    std::lock_guard<std::mutex> l(decode_mutex);
    AVPacket pkt;
    av_init_packet(&pkt);

    while (remainingLen > 0) {
        if (!pCodecParserCtx || !pCodecCtx) {
            break;
        }
        processedLen = av_parser_parse2(
            pCodecParserCtx, pCodecCtx, &pkt.data, &pkt.size, pData,
            remainingLen, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
        remainingLen -= processedLen;
        pData += processedLen;

        if (pkt.size > 0) {
            int ret = avcodec_send_packet(pCodecCtx, &pkt);
            if (ret < 0) { 
                std::cerr << "Error sending a packet for decoding: " << ret << std::endl; break; 
            }

            while (ret >= 0) { 
                ret = avcodec_receive_frame(pCodecCtx, pFrameYUV); 
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) { 
                    break; 
                } 
                else if (ret < 0) { 
                    std::cerr << "Error during decoding: " << ret << std::endl; 
                    break; 
                } 

                if (pFrameYUV->width != decode_width || pFrameYUV->height != decode_hight) { 
                    decode_width = pFrameYUV->width; 
                    decode_hight = pFrameYUV->height; 
                    if (nullptr != pSwsCtx) { 
                        sws_freeContext(pSwsCtx); 
                        pSwsCtx = nullptr; 
                    } 
                    INFO("New H264 Width: %d, Height: %d", decode_width, decode_hight); 
                } 
                // Push the frame data to appsrc 
                PushFrameToAppsrc(pFrameYUV);
            }
        }
        // Free the packet's data and reset it 
        av_packet_unref(&pkt);
    }
}

void StreamProcessorThread::InitDecoder(){
    pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    pCodecParserCtx = av_parser_init(pCodec->id);

    pCodecCtx = avcodec_alloc_context3(pCodec);
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = 1920;
    pCodecCtx->height = 1080;
    pCodecCtx->flags2 |= AV_CODEC_FLAG2_SHOW_ALL;
    pCodecCtx->thread_count = 4;
    auto ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
    pFrameYUV = av_frame_alloc();
    pSwsCtx = nullptr;

    std::cout << "Init Decoder!!!!!!!!!!!!" << std::endl;
}

int32_t StreamProcessorThread::Start() {
    if (processor_start_) {
        WARN("repeat start processor");
        return -1;
    }
    processor_start_ = true;
    InitDecoder();
    // if (stream_decoder_) {
    //     auto ret = stream_decoder_->Init();
    //     if (ret < 0) {
    //         ERROR("Failed to init stream decoder");
    //         processor_start_ = false;
    //         return -1;
    //     }
    // }
    //thread 1 start
    stream_processor_thread_ = std::thread(&StreamProcessorThread::ImageProcess, this);
    {
        sched_param sch;
        int policy;
        pthread_getschedparam(stream_processor_thread_.native_handle(), &policy, &sch);
        sch.sched_priority = 40;

        if (pthread_setschedparam(stream_processor_thread_.native_handle(), SCHED_FIFO, &sch)) {
            ERROR("Failed to setschedparam: %s", strerror(errno));
        }
    }

    // if (image_processor_thread_) image_processor_thread_->Start(); //thread 2 start
    return 0;
}

int32_t StreamProcessorThread::Stop() {
    processor_start_ = false;
    if (stream_processor_thread_.joinable()) {
        stream_processor_thread_.join();
    }

    return 0;
}

void StreamProcessorThread::ImageProcess() {
    INFO("start image processor: %s", processor_name_.c_str());
    pthread_setname_np(pthread_self(), "streamdecoder");
    std::vector<uint8_t> decode_data;
    std::vector<uint8_t> appsrc_data;
    while (processor_start_) {
        std::unique_lock<std::mutex> l(decode_vector_mutex_);

        decode_vector_cv_.wait(l, [&] { return decode_vector_.size() != 0; });
        decode_data = decode_vector_;
        appsrc_data = decode_vector_;
        decode_vector_.clear();
        l.unlock();
        std::cout << "StreamProcessorThread::ImageProcess: decode_data_ size: " << decode_data.size() << std::endl;

        //my code
        // Push the data into the GStreamer appsrc
        // PushDataToAppsrc(appsrc_data);
        // WriteDataToFile(appsrc_data);
        DecodeFrame(decode_data.data(), decode_data.size());

        //should call decoder here which calls push to appsrc inside
        //maybe in here?
        // stream_decoder_->Decode(
        //     decode_data.data(), decode_data.size(),
        //     [&](std::shared_ptr<Image>& result) -> void {
        //         if (result != nullptr && image_processor_thread_) {
        //             image_processor_thread_->InputImage(result);
        //         }
        //     }
        // );
    }
    INFO("stop image processor: %s", processor_name_.c_str());
}

}  // namespace edge_app
