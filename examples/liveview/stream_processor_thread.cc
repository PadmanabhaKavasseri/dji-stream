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

/*
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

    g_object_set(waylandsink_, "fullscreen", TRUE, nullptr);
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
*/


void StreamProcessorThread::create_pipe (GstAppOptions * options){
  GstElement *queue[QUEUE_COUNT], *tee[TEE_COUNT];
  GstElement *qtimlvconverter = NULL, *qtimlelement = NULL;
  GstElement *qtimlvdetection = NULL, *detection_filter_metamux = NULL;
  GstElement *detection_filter_rtspbin = NULL;
  GstElement *waylandsink = NULL;
  GstElement *appsrc = NULL, *h264parse_appsrc = NULL, *h264parse_rtsp = NULL;
  GstElement *qtic2vdec = NULL, *qtic2venc = NULL;
  GstElement *metamux = NULL, *overlay = NULL;
  GstElement *rtspbin = NULL;
  GstCaps *pad_filter = NULL;
  GstStructure *delegate_options = NULL;
  gboolean ret = FALSE;
  gchar element_name[128];
  gchar temp_str[1000];
  GValue layers = G_VALUE_INIT;
  GValue value = G_VALUE_INIT;
  gint module_id;
  for (gint i = 0; i < QUEUE_COUNT; i++)
    queue[i] = NULL;
  for (gint i = 0; i < TEE_COUNT; i++)
    tee[i] = NULL;
  // 1. Create the elements or Plugins
  // Create file source element for file stream
  appsrc = gst_element_factory_make ("appsrc", "appsrc");
  if (!appsrc) {
    g_printerr ("Failed to create appsrc\n");
    goto error_clean_elements;
  }
  // Create h264parse elment for parsing the stream
  h264parse_appsrc = gst_element_factory_make ("h264parse", "h264parse_appsrc");
  if (!h264parse_appsrc) {
    g_printerr ("Failed to create h264parse_appsrc\n");
    goto error_clean_elements;
  }
  // Create qtic2vdec element for encoding the stream
  qtic2vdec = gst_element_factory_make ("qtic2vdec", "qtic2vdec");
  if (!qtic2vdec) {
    g_printerr ("Failed to create qtic2vdec\n");
    goto error_clean_elements;
  }
  // Create queue to decouple the processing on sink and source pad
  for (gint i = 0; i < QUEUE_COUNT; i++) {
    snprintf (element_name, 127, "queue-%d", i);
    queue[i] = gst_element_factory_make ("queue", element_name);
    if (!queue[i]) {
      g_printerr ("Failed to create queue %d\n", i);
      goto error_clean_elements;
    }
  }
  // Use tee to send same data buffer for file and rtsp use cases
  // one for AI inferencing, one for Display composition
  for (gint i = 0; i < TEE_COUNT; i++) {
    snprintf (element_name, 127, "split_%d", i);
    tee[i] = gst_element_factory_make ("tee", element_name);
    if (!tee[i]) {
      g_printerr ("Failed to create tee %d\n", i);
      goto error_clean_elements;
    }
  }
  // Create qtimlconverter for Input preprocessing
  qtimlvconverter = gst_element_factory_make ("qtimlvconverter",
      "qtimlvconverter");
  if (!qtimlvconverter) {
    g_printerr ("Failed to create qtimlvconverter\n");
    goto error_clean_elements;
  }
  // Create the ML inferencing plugin SNPE/TFLITE
  if (options->model_type == GST_MODEL_TYPE_SNPE) {
    qtimlelement = gst_element_factory_make ("qtimlsnpe", "qtimlsnpe");
  } else if (options->model_type == GST_MODEL_TYPE_TFLITE) {
    qtimlelement = gst_element_factory_make ("qtimltflite", "qtimlelement");
  } else {
    g_printerr ("Invalid model type\n");
    goto error_clean_elements;
  }
  if (!qtimlelement) {
    g_printerr ("Failed to create qtimlelement\n");
    goto error_clean_elements;
  }
  // Create plugin for ML postprocessing for object detection
  qtimlvdetection = gst_element_factory_make ("qtimlvdetection",
      "qtimlvdetection");
  if (!qtimlvdetection) {
    g_printerr ("Failed to create qtimlvdetection\n");
    goto error_clean_elements;
  }
  // Used to negotiate between ML post proc o/p and qtivcomposer
  detection_filter_metamux = gst_element_factory_make ("capsfilter",
      "detection_filter_metamux");
  if (!detection_filter_metamux) {
    g_printerr ("Failed to create detection_filter_metamux\n");
    goto error_clean_elements;
  }
  detection_filter_rtspbin = gst_element_factory_make ("capsfilter",
      "detection_filter_rtspbin");
  if (!detection_filter_rtspbin) {
    g_printerr ("Failed to create detection_filter_rtspbin\n");
    goto error_clean_elements;
  }
  metamux = gst_element_factory_make ("qtimetamux", "metamux");
  if (!metamux) {
    g_printerr ("Failed to create metamux\n");
    goto error_clean_elements;
  }
  overlay = gst_element_factory_make ("qtivoverlay", "overlay");
  if (!overlay) {
    g_printerr ("Failed to create overlay\n");
    goto error_clean_elements;
  }
  qtic2venc = gst_element_factory_make ("qtic2venc", "qtic2venc");
  if (!qtic2venc) {
    g_printerr ("Failed to create qtic2venc");
    goto error_clean_elements;
  }
  h264parse_rtsp = gst_element_factory_make ("h264parse", "h264parse_rtsp");
  if (!h264parse_rtsp) {
    g_printerr ("Failed to create h264parse_rtsp");
    goto error_clean_elements;
  }
  rtspbin = gst_element_factory_make ("qtirtspbin", "qtirtspbin");
  if (!rtspbin) {
    g_printerr ("Failed to create rtspbin");
    goto error_clean_elements;
  }
  // Create Wayland composer compositor to render output on Display
  waylandsink = gst_element_factory_make ("waylandsink", "waylandsink");
  if (!waylandsink) {
    g_printerr ("Failed to create waylandsink");
    goto error_clean_elements;
  }
  // 2. Set properties for all GST plugin elements
  // 2.2 Set appsrc properties
  snprintf (temp_str, sizeof (temp_str),
      "video/x-h264,stream-format=byte-stream,framerate=30/1");
  pad_filter = gst_caps_from_string (temp_str);
  g_object_set (G_OBJECT (appsrc), "caps", pad_filter, NULL);
  gst_caps_unref (pad_filter);
  g_signal_connect (appsrc, "need-data", G_CALLBACK (start_feed), appctx);
  g_signal_connect (appsrc, "enough-data", G_CALLBACK (stop_feed), appctx);
  g_object_set (G_OBJECT (appsrc), "max-buffers", 100, NULL);
  g_object_set (G_OBJECT (appsrc), "block", TRUE, NULL);
  g_object_set (G_OBJECT (qtic2venc), "control-rate", "3", NULL);
  g_object_set (G_OBJECT (qtic2venc), "target-bitrate", "1200000", NULL);
  g_object_set (G_OBJECT (h264parse_rtsp), "config-interval", 1, NULL);
  appctx->appsrc = appsrc;
  // 2.5 Select the HW to DSP/GPU/CPU for model inferencing using
  // delegate property
  if (options->model_type == GST_MODEL_TYPE_SNPE) {
    GstMLSnpeDelegate snpe_delegate;
    if (options->use_cpu) {
      snpe_delegate = GST_ML_SNPE_DELEGATE_NONE;
      g_print ("Using CPU delegate\n");
    } else if (options->use_gpu) {
      snpe_delegate = GST_ML_SNPE_DELEGATE_GPU;
      g_print ("Using GPU delegate\n");
    } else {
      snpe_delegate = GST_ML_SNPE_DELEGATE_DSP;
      g_print ("Using DSP delegate\n");
    }
    g_object_set (G_OBJECT (qtimlelement), "model", options->model_path,
        "delegate", snpe_delegate, NULL);
  } else if (options->model_type == GST_MODEL_TYPE_TFLITE) {
    GstMLTFLiteDelegate tflite_delegate;
    if (options->use_cpu) {
      tflite_delegate = GST_ML_TFLITE_DELEGATE_NONE;
      g_print ("Using CPU Delegate\n");
      g_object_set (G_OBJECT (qtimlelement), "model", options->model_path,
          "delegate", tflite_delegate, NULL);
    } else if (options->use_gpu) {
      g_print ("Using GPU delegate\n");
      tflite_delegate = GST_ML_TFLITE_DELEGATE_GPU;
      g_object_set (G_OBJECT (qtimlelement), "model", options->model_path,
          "delegate", tflite_delegate, NULL);
    } else if (options->use_dsp) {
      g_print ("Using DSP delegate\n");
      delegate_options = gst_structure_from_string (
          "QNNExternalDelegate,backend_type=htp", NULL);
      g_object_set (G_OBJECT (qtimlelement), "model", options->model_path,
          "delegate", GST_ML_TFLITE_DELEGATE_EXTERNAL, NULL);
      g_object_set (G_OBJECT (qtimlelement),
          "external_delegate_path", "libQnnTFLiteDelegate.so", NULL);
      g_object_set (G_OBJECT (qtimlelement),
          "external_delegate_options", delegate_options, NULL);
      gst_structure_free (delegate_options);
    } else {
      g_printerr ("Invalid Runtime Selected\n");
      goto error_clean_elements;
    }
  } else {
    g_printerr ("Invalid model type\n");
    goto error_clean_elements;
  }
  // 2.6 Set properties for ML postproc plugins - module, layers, threshold
  g_value_init (&layers, GST_TYPE_ARRAY);
  g_value_init (&value, G_TYPE_STRING);
  if (options->model_type == GST_MODEL_TYPE_SNPE) {
    switch (options->yolo_model_type) {
      // YOLO_V5 specific settings
      case GST_YOLO_TYPE_V5:
        g_object_set (G_OBJECT (qtimlelement), "model",
            options->model_path, NULL);
        g_value_set_string (&value, "Conv_198");
        gst_value_array_append_value (&layers, &value);
        g_value_set_string (&value, "Conv_232");
        gst_value_array_append_value (&layers, &value);
        g_value_set_string (&value, "Conv_266");
        gst_value_array_append_value (&layers, &value);
        g_object_set_property (G_OBJECT (qtimlelement), "layers", &layers);
        // get enum values of module properties from qtimlvdetection plugin
        module_id = get_enum_value (qtimlvdetection, "module", "yolov5");
        if (module_id != -1) {
          g_object_set (G_OBJECT (qtimlvdetection), "module", module_id, NULL);
        } else {
          g_printerr ("Module yolov5 is not available in qtimlvdetection\n");
          goto error_clean_elements;
        }
        // set qtimlvdetection properties
        g_object_set (G_OBJECT (qtimlvdetection), "labels",
            options->labels_path, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "threshold",
            options->threshold, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "results", 10, NULL);
        break;
      // YOLO_V8 specific settings
      case GST_YOLO_TYPE_V8:
        g_object_set (G_OBJECT (qtimlelement), "model",
            options->model_path, NULL);
        g_value_set_string (&value, "Mul_248");
        gst_value_array_append_value (&layers, &value);
        g_value_set_string (&value, "Sigmoid_249");
        gst_value_array_append_value (&layers, &value);
        g_object_set_property (G_OBJECT (qtimlelement), "layers", &layers);
        // get enum values of module property frrom qtimlvdetection plugin
        module_id = get_enum_value (qtimlvdetection, "module", "yolov8");
        if (module_id != -1){
          g_object_set (G_OBJECT (qtimlvdetection), "module", module_id, NULL);
        } else {
          g_printerr ("Module yolov8 is not available in qtimlvdetection\n");
          goto error_clean_elements;
        }
        // set qtimlvdetection properties
        g_object_set (G_OBJECT (qtimlvdetection), "labels",
            options->labels_path, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "threshold",
            options->threshold, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "results", 10, NULL);
        break;
      // YOLO_NAS specific settings
      case GST_YOLO_TYPE_NAS:
        g_object_set (G_OBJECT (qtimlelement), "model",
            options->model_path, NULL);
        g_value_set_string (&value, "/heads/Mul");
        gst_value_array_append_value (&layers, &value);
        g_value_set_string (&value, "/heads/Sigmoid");
        gst_value_array_append_value (&layers, &value);
        g_object_set_property (G_OBJECT (qtimlelement), "layers", &layers);
        // get enum values of module property frrom qtimlvdetection plugin
        module_id = get_enum_value (qtimlvdetection, "module", "yolo-nas");
        if (module_id != -1) {
          g_object_set (G_OBJECT (qtimlvdetection), "module", module_id, NULL);
        } else {
          g_printerr ("Module yolo-nas is not available in qtimlvdetection\n");
          goto error_clean_elements;
        }
        // set qtimlvdetection properties
        g_object_set (G_OBJECT (qtimlvdetection), "labels",
            options->labels_path, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "threshold",
            options->threshold, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "results", 10, NULL);
        break;
      default:
        g_printerr ("Invalid Yolo Model type\n");
        goto error_clean_elements;
    }
  } else if (options->model_type == GST_MODEL_TYPE_TFLITE) {
    switch (options->yolo_model_type) {
      // YOLO_V8 specific settings
      case GST_YOLO_TYPE_V8:
        // set qtimlvdetection properties
        g_object_set (G_OBJECT (qtimlvdetection), "labels",
            options->labels_path, NULL);
        module_id = get_enum_value (qtimlvdetection, "module", "yolov8");
        if (module_id != -1) {
            g_object_set (G_OBJECT (qtimlvdetection), "module", module_id, NULL);
        } else {
          g_printerr ("Module yolov8 is not available in qtimlvdetection\n");
          goto error_clean_elements;
        }
        g_object_set (G_OBJECT (qtimlvdetection), "threshold",
            options->threshold, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "results", 10, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "constants",
            options->constants, NULL);
        break;
      // YOLO_V5 specific settings
      case GST_YOLO_TYPE_V5:
        // set qtimlvdetection properties
        g_object_set (G_OBJECT (qtimlvdetection), "labels",
            options->labels_path, NULL);
        module_id = get_enum_value (qtimlvdetection, "module", "yolov5");
        if (module_id != -1) {
            g_object_set (G_OBJECT (qtimlvdetection), "module", module_id, NULL);
        } else {
          g_printerr ("Module yolov5 is not available in qtimlvdetection\n");
          goto error_clean_elements;
        }
        g_object_set (G_OBJECT (qtimlvdetection), "threshold",
            options->threshold, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "results", 10, NULL);
        g_object_set (G_OBJECT (qtimlvdetection), "constants",
            options->constants, NULL);
        break;
      default:
        g_printerr ("Unsupported TFLITE model, Use YoloV5 or "
            "YoloV8 TFLITE model\n");
        goto error_clean_elements;
    }
  } else {
    g_printerr ("Invalid model_type or yolo_model_type\n");
    goto error_clean_elements;
  }
  // 2.7 Set the properties for Wayland compositer
  // g_object_set (G_OBJECT (waylandsink), "sync", FALSE, NULL); // Set to true for offline stream *****
  g_object_set (G_OBJECT (waylandsink), "fullscreen", TRUE, NULL);
  // 2.9 Set the properties of pad_filter for negotiation with qtivcomposer
  pad_filter = gst_caps_new_simple ("text/x-raw", "format", G_TYPE_STRING,
        "utf8", NULL);
  g_object_set (G_OBJECT (detection_filter_metamux), "caps", pad_filter, NULL);
  gst_caps_unref (pad_filter);
  pad_filter = gst_caps_new_simple ("text/x-raw", "format", G_TYPE_STRING,
        "utf8", NULL);
  g_object_set (G_OBJECT (detection_filter_rtspbin), "caps", pad_filter, NULL);
  gst_caps_unref (pad_filter);
  // 3. Setup the pipeline
  g_print ("Adding all elements to the pipeline...\n");
  gst_bin_add_many (GST_BIN (appctx->pipeline), appsrc, h264parse_appsrc,
      qtic2vdec, qtimlvconverter, qtimlelement, qtimlvdetection,
      detection_filter_metamux, metamux, overlay, waylandsink,
      detection_filter_rtspbin, qtic2venc, h264parse_rtsp, rtspbin, NULL);
  for (gint i = 0; i < QUEUE_COUNT; i++)
    gst_bin_add_many (GST_BIN (appctx->pipeline), queue[i], NULL);
  for (gint i = 0; i < TEE_COUNT; i++)
    gst_bin_add_many (GST_BIN (appctx->pipeline), tee[i], NULL);
  g_print ("Linking elements...\n");
  // Create pipeline for object detection
  ret = gst_element_link_many (appsrc, h264parse_appsrc, queue[0],
      qtic2vdec, queue[1], tee[0], queue[2], metamux, queue[3],
      overlay, queue[8], tee[1], NULL);
  if (!ret) {
    g_printerr ("Pipeline elements could not be linked for parse->tee\n");
    goto error_clean_pipeline;
  }
  ret = gst_element_link_many (tee[1], queue[9], waylandsink, NULL);
  if (!ret) {
    g_printerr ("Pipeline elements cannot be linked for"
        "metamux, overlay and waylandsink.\n");
    goto error_clean_pipeline;
  }
  ret = gst_element_link_many (tee[1], queue[10], qtic2venc, queue[11],
      h264parse_rtsp, queue[12], rtspbin, NULL);
  if (!ret) {
    g_printerr ("Pipeline elements cannot be linked for"
        "pre proc -> ml framework -> post proc.\n");
    goto error_clean_pipeline;
  }
  ret = gst_element_link_many (tee[0], queue[4], qtimlvconverter,
      queue[5], qtimlelement, queue[6], qtimlvdetection, tee[2],
      detection_filter_metamux, queue[7], metamux, NULL);
  if (!ret) {
    g_printerr ("Pipeline elements cannot be linked for"
        "pre proc -> ml framework -> post proc.\n");
    goto error_clean_pipeline;
  }
  ret = gst_element_link_many (tee[2], detection_filter_rtspbin,
      queue[13], rtspbin, NULL);
  if (!ret) {
    g_printerr ("Pipeline elements cannot be linked for"
        "detection filter -> rtspbin.\n");
    goto error_clean_pipeline;
  }
  return TRUE;
error_clean_pipeline:
  gst_object_unref (appctx->pipeline);
  appctx->pipeline = NULL;
  return FALSE;
error_clean_elements:
  cleanup_gst (&appsrc, &h264parse_appsrc, &qtic2vdec, &h264parse_rtsp,
      &qtimlvconverter, &qtimlelement, &qtic2venc,
      &qtimlvdetection, &detection_filter_metamux, &detection_filter_rtspbin,
      &metamux, &overlay, &waylandsink, &rtspbin, NULL);
  for (gint i = 0; i < QUEUE_COUNT; i++)
    gst_object_unref (queue[i]);
  for (gint i = 0; i < TEE_COUNT; i++)
    gst_object_unref (tee[i]);
  return FALSE;
}
  

void StreamProcessorThread::SetupPipeline() {
    // Initialize GStreamer
    gst_init(nullptr, nullptr);

    // Create pipeline
    pipeline_ = gst_pipeline_new("video_pipeline");
    if (!pipeline_) {
        std::cerr << "Failed to create pipeline." << std::endl;
        return;
    }

    // Create elements
    appsrc_ = gst_element_factory_make("appsrc", "app_src");
    videoconvert_ = gst_element_factory_make("videoconvert", "video_convert");
    tee_ = gst_element_factory_make("tee", "split");
    queue1_ = gst_element_factory_make("queue", "queue1");
    qtimetamux_ = gst_element_factory_make("qtimetamux", "metamux");
    queue2_ = gst_element_factory_make("queue", "queue2");
    qtioverlay_ = gst_element_factory_make("qtioverlay", "overlay");
    queue3_ = gst_element_factory_make("queue", "queue3");
    waylandsink_ = gst_element_factory_make("waylandsink", "wayland_sink");
    queue4_ = gst_element_factory_make("queue", "queue4");
    qtimlvconverter_ = gst_element_factory_make("qtimlvconverter", "mlv_converter");
    queue5_ = gst_element_factory_make("queue", "queue5");
    qtimltflite_ = gst_element_factory_make("qtimltflite", "tflite");
    queue6_ = gst_element_factory_make("queue", "queue6");
    qtimlvdetection_ = gst_element_factory_make("qtimlvdetection", "mlv_detection");

    // Check if elements were created successfully
    if (!appsrc_ || !videoconvert_ || !tee_ || !queue1_ || !qtimetamux_ || !queue2_ || !qtioverlay_ || !queue3_ || !waylandsink_ || !queue4_ || !qtimlvconverter_ || !queue5_ || !qtimltflite_ || !queue6_ || !qtimlvdetection_) {
        std::cerr << "Failed to create one or more GStreamer elements." << std::endl;
        return;
    }

    // Set appsrc properties
    GstCaps *caps = gst_caps_new_simple(
        "video/x-raw",
        "format", G_TYPE_STRING, "I420", // YUV format
        "width", G_TYPE_INT, 1920,       // Set your width
        "height", G_TYPE_INT, 1080,      // Set your height
        "framerate", GST_TYPE_FRACTION, 30, 1, // Example framerate
        nullptr
    );

    g_object_set(appsrc_, "caps", caps, nullptr);
    gst_caps_unref(caps);
    g_object_set(appsrc_, "block", FALSE, nullptr);

    g_object_set(waylandsink_, "fullscreen", TRUE, "sync", FALSE, nullptr);

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline_), appsrc_, videoconvert_, tee_, queue1_, qtimetamux_, queue2_, qtioverlay_, queue3_, waylandsink_, queue4_, qtimlvconverter_, queue5_, qtimltflite_, queue6_, qtimlvdetection_, nullptr);

    // Link elements
    if (!gst_element_link_many(appsrc_, videoconvert_, nullptr)) {
        std::cerr << "Failed to link GStreamer elements: appsrc to videoconvert." << std::endl;
        return;
    }

    GstCaps *video_caps = gst_caps_new_simple(
        "video/x-raw",
        "format", G_TYPE_STRING, "NV12",
        nullptr
    );

    if (!gst_element_link_filtered(videoconvert_, tee_, video_caps)) {
        std::cerr << "Failed to link videoconvert to tee with caps." << std::endl;
        return;
    }

    gst_caps_unref(video_caps);

    if (!gst_element_link_many(tee_, queue1_, qtimetamux_, queue2_, qtioverlay_, queue3_, waylandsink_, nullptr)) {
        std::cerr << "Failed to link elements: tee to waylandsink." << std::endl;
        return;
    }

    if (!gst_element_link_many(tee_, queue4_, qtimlvconverter_, queue5_, qtimltflite_, queue6_, qtimlvdetection_, qtimetamux_, nullptr)) {
        std::cerr << "Failed to link elements: tee to metamux." << std::endl;
        return;
    }

    // Set the pipeline to playing state
    gst_element_set_state(pipeline_, GST_STATE_PLAYING);
}

/*

gst-launch-1.0 \
appsrc name=app_src ! videoconvert ! "video/x-raw,format=NV12" ! tee name=split \
split. ! queue ! qtimetamux name=metamux ! queue ! qtioverlay ! queue ! waylandsink sync=false fullscreen=true \
split. ! queue ! qtimlvconverter ! queue ! qtimltflite delegate=gpu model=/data/yolov5m-320x320-int8.tflite ! queue ! qtimlvdetection threshold=50.0 results=5 module=yolov5 labels=/data/yolov5m.labels constants="YoloV8,q-offsets=<3.0>,q-scales=<0.005047998391091824>;" ! queue ! metamux.



*/


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
