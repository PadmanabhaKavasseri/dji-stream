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
#ifndef __IMAGE_PROCESSOR_DIAPLAY_H__
#define __IMAGE_PROCESSOR_DIAPLAY_H__

#include <memory>
#include "opencv2/opencv.hpp"
#include "liveview/sample_liveview.h"

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

namespace edge_app {

class ImageDisplayProcessor : public ImageProcessor {
   public:
    ImageDisplayProcessor(const std::string& name, std::shared_ptr<void> userdata) : name_(name) {
        cv::namedWindow(name.c_str(), cv::WINDOW_NORMAL);
        cv::resizeWindow(name.c_str(), 960, 540);
        cv::moveWindow(name.c_str(), rand() & 0xFF, rand() & 0xFF);
        if (userdata) {
            liveview_sample_ = std::static_pointer_cast<LiveviewSample>(userdata);
        }
    }

    ~ImageDisplayProcessor() override {}

    void Process(const std::shared_ptr<Image> image) override {
        // std::cout << "image_processor_display.h" << std::endl;
        std::string h = std::to_string(image->size().width);
        std::string w = std::to_string(image->size().height);
        std::string osd = h + "x" + w;
        if (liveview_sample_) {
            // std::cout << "in liveview_sample_" << std::endl;
            auto kbps = liveview_sample_->GetStreamBitrate();
            osd += std::string(",") + std::to_string(kbps) + std::string("kbps");
        }
        putText(*image, osd, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1,
                cv::Scalar(0, 0, 255), 3);
        imshow(name_.c_str(), *image);
        cv::waitKey(1);

        // StreamToRTSP(*image);

    }

   private:

    // void StreamToRTSP(const cv::Mat& frame) {
    //     // Initialize FFmpeg
    //     AVFormatContext *formatContext = nullptr;
    //     AVCodecContext *codecContext = nullptr;
    //     AVCodec *codec = nullptr;
    //     AVStream *videoStream = nullptr;
    //     AVPacket packet;
    //     SwsContext *swsContext = nullptr;

    //     // Open the output stream
    //     std::string rtsp_url = "rtsp://192.168.200.55/live";
    //     if (avformat_alloc_output_context2(&formatContext, nullptr, "rtsp", rtsp_url.c_str()) < 0) {
    //         std::cerr << "Error creating output context." << std::endl;
    //         return;
    //     }

    //     // Find the encoder
    //     codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    //     if (!codec) {
    //         std::cerr << "H.264 encoder not found." << std::endl;
    //         return;
    //     }

    //     // Create a new video stream
    //     videoStream = avformat_new_stream(formatContext, codec);
    //     if (!videoStream) {
    //         std::cerr << "Error creating new stream." << std::endl;
    //         return;
    //     }

    //     codecContext = videoStream->codec;
    //     codecContext->width = frame.cols;
    //     codecContext->height = frame.rows;
    //     codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    //     codecContext->time_base = {1, 25};  // 25 FPS
    //     codecContext->bit_rate = 500000;

    //     // Open codec
    //     if (avcodec_open2(codecContext, codec, nullptr) < 0) {
    //         std::cerr << "Error opening codec." << std::endl;
    //         return;
    //     }

    //     // Open RTSP stream
    //     if (avio_open(&formatContext->pb, rtsp_url.c_str(), AVIO_FLAG_READ_WRITE) < 0) {
    //         std::cerr << "Error opening RTSP stream." << std::endl;
    //         return;
    //     }

    //     // Write the header of the RTSP stream
    //     if (avformat_write_header(formatContext, nullptr) < 0) {
    //         std::cerr << "Error writing header." << std::endl;
    //         return;
    //     }

    //     // Convert OpenCV Mat frame to AVFrame
    //     AVFrame *avFrame = av_frame_alloc();
    //     avFrame->format = AV_PIX_FMT_BGR24;
    //     avFrame->width = frame.cols;
    //     avFrame->height = frame.rows;
    //     av_frame_get_buffer(avFrame, 32);

    //     // Fill AVFrame with data from OpenCV Mat
    //     for (int y = 0; y < frame.rows; ++y) {
    //         memcpy(avFrame->data[0] + y * avFrame->linesize[0], frame.ptr(y), frame.cols * 3);
    //     }

    //     // Convert BGR to YUV420P (required for H264 encoding)
    //     swsContext = sws_getContext(frame.cols, frame.rows, AV_PIX_FMT_BGR24,
    //                                 frame.cols, frame.rows, AV_PIX_FMT_YUV420P,
    //                                 0, nullptr, nullptr, nullptr);
    //     AVFrame *yuvFrame = av_frame_alloc();
    //     sws_scale(swsContext, avFrame->data, avFrame->linesize, 0, frame.rows,
    //               yuvFrame->data, yuvFrame->linesize);

    //     // Encode the frame
    //     if (avcodec_send_frame(codecContext, yuvFrame) < 0) {
    //         std::cerr << "Error sending frame to encoder." << std::endl;
    //         return;
    //     }

    //     // Get the packet containing the encoded frame
    //     if (avcodec_receive_packet(codecContext, &packet) == 0) {
    //         // Write the packet to the stream
    //         av_interleaved_write_frame(formatContext, &packet);
    //         av_packet_unref(&packet);
    //     }

    //     // Clean up
    //     av_frame_free(&avFrame);
    //     av_frame_free(&yuvFrame);
    //     sws_freeContext(swsContext);

    //     // Close the output stream and cleanup
    //     av_write_trailer(formatContext);
    //     avcodec_close(codecContext);
    //     avformat_free_context(formatContext);
    // }
    std::string name_;
    std::shared_ptr<LiveviewSample> liveview_sample_;
};

}  // namespace edge_app

#endif

