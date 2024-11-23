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

// Set up RTSP server with appsrc
void StreamProcessorThread::SetupRTSPServer() {
    // Create RTSP Server
    // server = gst_rtsp_server_new();
    // GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);
    // factory = gst_rtsp_media_factory_new();

    // // Set up the RTSP pipeline
    // gst_rtsp_media_factory_set_launch(factory,
    //     "( appsrc name=source is-live=true block=true format=3 ! "
    //     "h264parse ! rtph264pay name=pay0 pt=96 )");

    // // Add the factory to mount points
    // gst_rtsp_mount_points_add_factory(mounts, "/test", factory);
    // g_object_unref(mounts);

    // // Attach RTSP server
    // gst_rtsp_server_attach(server, nullptr);

    // // Print RTSP URL
    // g_print("Stream ready at rtsp://127.0.0.1:8554/test\n");
}

void StreamProcessorThread::SetupPipeline() {
    // Initialize GStreamer
    gst_init(nullptr, nullptr);

    // Create the GStreamer pipeline
    pipeline = GST_PIPELINE(gst_pipeline_new("rtsp-pipeline"));

    // Create appsrc element
    appsrc = gst_element_factory_make("appsrc", "source");
    if (!appsrc) {
        g_printerr("Failed to create appsrc element\n");
        return;
    }

    // Set appsrc properties
    g_object_set(appsrc, "is-live", TRUE, nullptr);

    // Set the caps for the H.264 stream
    GstCaps *caps = gst_caps_new_simple("video/x-h264",
                                        "stream-format", G_TYPE_STRING, "byte-stream",
                                        "alignment", G_TYPE_STRING, "au",
                                        nullptr);
    g_object_set(appsrc, "caps", caps, nullptr);
    gst_caps_unref(caps);

    // Create other necessary pipeline elements
    GstElement *h264parse = gst_element_factory_make("h264parse", "parser");
    GstElement *rtph264pay = gst_element_factory_make("rtph264pay", "payloader");
    GstElement *udpsink = gst_element_factory_make("udpsink", "udpsink");

    if (!h264parse || !rtph264pay || !udpsink) {
        g_printerr("Failed to create pipeline elements\n");
        return;
    }

    // Add all elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), appsrc, h264parse, rtph264pay, udpsink, nullptr);

    // Link the elements in the pipeline
    if (!gst_element_link(appsrc, h264parse) ||
        !gst_element_link(h264parse, rtph264pay) ||
        !gst_element_link(rtph264pay, udpsink)) {
        g_printerr("Failed to link elements in the pipeline\n");
        return;
    }

    // Set up properties for the UDP sink
    g_object_set(udpsink, "host", "127.0.0.1", "port", 5000, nullptr);

    // Start the pipeline
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);

    GstStateChangeReturn ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_SUCCESS) {
        g_print("Pipeline is now playing\n");
    } else if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Failed to change pipeline state to PLAYING\n");
    } else if (ret == GST_STATE_CHANGE_ASYNC) {
        g_print("State change is asynchronous, waiting...\n");
    } else {
        g_print("Unexpected state change result\n");
    }

    g_print("RTSP stream ready at rtsp://127.0.0.1:8554/test\n");
    g_print("GPRNTRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
}

void StreamProcessorThread::PushDataToAppsrc(const std::vector<uint8_t>& data) {
    if (!appsrc) {
        std::cerr << "Error: appsrc is not initialized." << std::endl;
        return;
    }
    std::cout << "StreamProcessorThread::PushDataToAppsrc: data: " << data.size() << std::endl;
    // Create a new GStreamer buffer and fill it with the data
    GstBuffer *buffer = gst_buffer_new_allocate(nullptr, data.size(), nullptr);
    gst_buffer_fill(buffer, 0, data.data(), data.size());

    // Push the buffer into the appsrc element
    GstFlowReturn ret;
    g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);
    gst_buffer_unref(buffer);

    if (ret != GST_FLOW_OK) {
        std::cerr << "Failed to push buffer to appsrc. GstFlowReturn: " << ret << std::endl;
    }
}

StreamProcessorThread::StreamProcessorThread(const std::string& name)
    : processor_name_(name) {
    processor_start_ = false;

    //init pipeline here?
    // std::cout << "init appsrc" << std::endl;
    // appsrc = nullptr;
    SetupRTSPServer();
    SetupPipeline();
    std::cout << "HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;

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

int32_t StreamProcessorThread::Start() {
    if (processor_start_) {
        WARN("repeat start processor");
        return -1;
    }
    processor_start_ = true;
    if (stream_decoder_) {
        auto ret = stream_decoder_->Init();
        if (ret < 0) {
            ERROR("Failed to init stream decoder");
            processor_start_ = false;
            return -1;
        }
    }
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

    if (image_processor_thread_) image_processor_thread_->Start(); //thread 2 start
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
        PushDataToAppsrc(appsrc_data);
        //maybe in here?
        stream_decoder_->Decode(
            decode_data.data(), decode_data.size(),
            [&](std::shared_ptr<Image>& result) -> void {
                if (result != nullptr && image_processor_thread_) {
                    image_processor_thread_->InputImage(result);
                }
            }
        );
    }
    INFO("stop image processor: %s", processor_name_.c_str());
}

}  // namespace edge_app
