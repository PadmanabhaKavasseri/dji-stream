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
#ifndef __STREAM_PROCESSOR_THREAD_H__
#define __STREAM_PROCESSOR_THREAD_H__

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <iostream>

#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include "error_code.h"

#include <gst/gst.h>
#include <glib.h>
#include <gst/app/gstappsrc.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}



namespace cv {
class Mat;
}

namespace edge_app {

class StreamDecoder;
class ImageProcessorThread;

class StreamProcessorThread {
   public:
    explicit StreamProcessorThread(const std::string& name);

    virtual ~StreamProcessorThread();

    using Image = cv::Mat;

    const std::string Name() const { return processor_name_; }

    void InputStream(const uint8_t* data, size_t length);

    int32_t SetStreamDecoder(std::shared_ptr<StreamDecoder> decoder);

    int32_t SetImageProcessorThread(
        std::shared_ptr<ImageProcessorThread> image_processor);

    int32_t Start();

    int32_t Stop();

    void SetupPipeline();

    void PushDataToAppsrc(const std::vector<uint8_t>& data);

    void WriteDataToFile(const std::vector<uint8_t>& data);

    void InitDecoder();

    void DecodeFrame(const uint8_t *data, size_t length);

    // void SetupRTSPServer();

   protected:
    enum {
        kImageQueueSizeLimit = 10,
    };

    void ImageProcess();

    std::string processor_name_;

    std::vector<uint8_t> decode_vector_;
    std::mutex decode_vector_mutex_;
    std::condition_variable decode_vector_cv_;

    std::thread stream_processor_thread_;
    std::atomic<bool> processor_start_;
    std::shared_ptr<StreamDecoder> stream_decoder_;
    std::shared_ptr<ImageProcessorThread> image_processor_thread_;

    GstElement *appsrc_ = nullptr;
    GstElement *app_queue_ = nullptr;
    GstElement *app_queue2_ = nullptr;
    GstElement *h264parse_ = nullptr;
    GstElement *decoder_ = nullptr;
    GstElement *pipeline_ = nullptr;
    GstElement *waylandsink_ = nullptr;
    GstElement *videoconvert_ = nullptr;
    GstElement *qtic2venc_ = nullptr;
    GstElement *queue3_ = nullptr;
    GstElement *h264parse2_ = nullptr;
    GstElement *queue4_ = nullptr;
    GstElement *mp4mux_ = nullptr;
    GstElement *queue5_ = nullptr;
    GstElement *filesink_ = nullptr;
    
    std::ofstream outFile;

    AVCodecContext *pCodecCtx = nullptr;
    const AVCodec *pCodec = nullptr;
    AVCodecParserContext *pCodecParserCtx = nullptr;

};

}  // namespace edge_app

#endif
