/*
 * Copyright (c) 2025-2026 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstddef>  // For size_t
#include <cstdint>  // For uint8_t, int32_t, uint32_t
#include <cstdio>   // For printf
#include <stdio.h>

#include "sds_algorithm_config.h"
#include "sds_algorithm.h"

#include "DetectionResult.hpp"
#include "DetectorPostProcessing.hpp" /* Post Process */
#include "DetectorPreProcessing.hpp"  /* Pre Process  */
#include "YoloFastestModel.hpp"       /* Model API    */

#include "app_setup.h"
#include "cmsis_vstream.h"
#include "image_processing_func.h"

static void DrawBox(uint8_t *imageData, const uint32_t x0, const uint32_t y0, const uint32_t w, const uint32_t h);

using namespace arm::app;

/* Display frame buffer (RGB888) */
static uint8_t LCD_Frame[DISPLAY_IMAGE_SIZE] DISPLAY_FRAME_BUF_ATTRIBUTE;

/* Tensor arena buffer */
static uint8_t tensorArena[ACTIVATION_BUF_SZ] ACTIVATION_BUF_ATTRIBUTE;

/* Optional getter function for the model pointer and its size. */
namespace arm::app::object_detection {
  extern uint8_t* GetModelPointer();
  extern size_t GetModelLen();
}


/* Model object */
YoloFastestModel model;

/* Tensors and shapes */
TfLiteTensor   *inputTensor;
TfLiteTensor   *outputTensor0;
TfLiteTensor   *outputTensor1;
TfLiteIntArray *inputShape;

/* Input image dimensions */
int inputImgCols;
int inputImgRows;

/* Object to hold detection results */
std::vector<object_detection::DetectionResult> results;

/* Post-processing parameters */
object_detection::PostProcessParams postProcessParams;

/* Pre and Post processing objects */
DetectorPreProcess  *preProcess  = nullptr;
DetectorPostProcess *postProcess = nullptr;

/* Reference to the underlying CMSIS vStream VideoOut driver */
extern vStreamDriver_t          Driver_vStreamVideoOut;
#define vStream_VideoOut      (&Driver_vStreamVideoOut)


/* Video Out Stream Event Callback */
void VideoOut_Event_Callback (uint32_t event) {
    (void)event;
}

/**
  \fn           int32_t InitAlgorithm (void)
  \brief        Initialize algorithm under test.
  \return       0 on success; -1 on error
*/
int32_t InitAlgorithm (void) {
  vStreamStatus_t status;

  if (!model.Init(tensorArena, sizeof(tensorArena), object_detection::GetModelPointer(), object_detection::GetModelLen())) {
    printf("Failed to initialise model\n");
    return -1;
  }

  inputTensor   = model.GetInputTensor(0);
  outputTensor0 = model.GetOutputTensor(0);
  outputTensor1 = model.GetOutputTensor(1);

  /* Get input shape dimensions */
  inputShape = model.GetInputShape(0);
  inputImgCols = inputShape->data[YoloFastestModel::ms_inputColsIdx];
  inputImgRows = inputShape->data[YoloFastestModel::ms_inputRowsIdx];

  /* Set up pre and post-processing. */
  preProcess = new DetectorPreProcess(inputTensor, true, model.IsDataSigned());

  postProcessParams = {
      inputImgRows,
      inputImgCols,
      object_detection::originalImageSize,
      object_detection::anchor1,
      object_detection::anchor2};


  postProcess = new DetectorPostProcess(outputTensor0, outputTensor1, results, postProcessParams);

  /* Initialize Video Output Stream */
  if (vStream_VideoOut->Initialize(VideoOut_Event_Callback) != VSTREAM_OK) {
      printf("Failed to initialise video output driver\n");
      return -1;
  }

  /* Set Output Video buffer */
  if (vStream_VideoOut->SetBuf(LCD_Frame, sizeof(LCD_Frame), DISPLAY_IMAGE_SIZE) != VSTREAM_OK) {
      printf("Failed to set buffer for video output\n");
      return -1;
  }

  return 0;
}

/**
  \fn           int32_t ExecuteAlgorithm (uint8_t *in_buf, uint32_t in_num, uint8_t *out_buf, uint32_t out_num)
  \brief        Execute algorithm under test.
  \param[in]    in_buf          pointer to memory buffer containing input data for algorithm
  \param[in]    in_num          number of data bytes in input data buffer (in bytes)
  \param[out]   out_buf         pointer to memory buffer for returning algorithm output
  \param[in]    out_num         maximum number of data bytes returned as algorithm output (in bytes)
  \return       0 on success; -1 on error
*/
int32_t ExecuteAlgorithm (uint8_t *in_buf, uint32_t in_num, uint8_t *out_buf, uint32_t out_num) {
  vStreamStatus_t status;
  uint8_t *outFrame;
  size_t img_sz;
  uint8_t *img_buf;
  uint32_t n;

  results.clear();

  /* Input buffer is the image frame. 
     Input buffer is misused for output to LCD also to reduce memory consumption. */
  img_buf = in_buf;
  img_sz  = in_num;

  /* Run the pre-processing, inference and post-processing. */
  if (!preProcess->DoPreProcess(img_buf, img_sz)) {
    printf("Pre-processing failed.\n");
    return -1;
  }

  if (!model.RunInference()) {
    printf("Inference failed.\n");
    return -1;
  }

  if (!postProcess->DoPostProcess()) {
    printf("Post-processing failed.\n");
    return -1;
  }

  /* Clear output buffer */
  memset (out_buf, 0, out_num);

  /* Check the detection results */
  if (results.empty()) {
    printf("No object detected\n");
  }
  else {
    printf("Detected objects ");

    /* Display max 10 result objects */
    for (n = 0; n < 10 && n < results.size(); n++) {
      const auto& result = results[n];

      /* Copy result into output buffer */
      memcpy (out_buf + n * sizeof(object_detection::DetectionResult), &result, sizeof(object_detection::DetectionResult));

      /* Draw a box around detected object */
      DrawBox(img_buf, result.m_x0, result.m_y0, result.m_w, result.m_h);

      /* Sent detection coordinates to the console */
      printf(":: [x=%d, y=%d, w=%d, h=%d]", result.m_x0, result.m_y0, result.m_w, result.m_h);
    }
    printf("\n");
  }

  /* Wait for video output frame to be released */
  do {
      status = vStream_VideoOut->GetStatus();
  } while (status.active == 1U);

  /* Get output frame */
  outFrame = (uint8_t *)vStream_VideoOut->GetBlock();
  if (outFrame == NULL) {
      printf("Failed to get video output frame\n");
      return -1;
  }

  /* Copy ML image into the display frame buffer */
  image_copy_to_framebuffer(in_buf,
                            ML_IMAGE_WIDTH,
                            ML_IMAGE_HEIGHT,
                            outFrame,
                            DISPLAY_FRAME_WIDTH,
                            DISPLAY_FRAME_HEIGHT,
                            (DISPLAY_FRAME_WIDTH - ML_IMAGE_WIDTH) / 2,
                            (DISPLAY_FRAME_HEIGHT - ML_IMAGE_HEIGHT)/2,
                            IMAGE_FORMAT_RGB888);

  /* Release output frame */
  if (vStream_VideoOut->ReleaseBlock() != VSTREAM_OK) {
      printf("Failed to release video output frame\n");
  }

  /* Start video output */
  if (vStream_VideoOut->Start(VSTREAM_MODE_SINGLE) != VSTREAM_OK) {
      printf("Failed to start video output\n");
  }

  return 0;
}

/**
  Draws a box in the image.

  \param[out] imageData    Pointer to the start of the image.
  \param[in]  width        Image width.
  \param[in]  height       Image height.
  \param[in]  result       Object detection result.
 */
static void DrawBox(uint8_t *imageData, const uint32_t x0, const uint32_t y0, const uint32_t w, const uint32_t h) {
  const uint32_t step = ML_IMAGE_WIDTH * 3;
  uint8_t* const imStart = imageData + (y0 * step) + (x0 * 3);

  uint8_t* dst_0 = imStart;
  uint8_t* dst_1 = imStart + (h * step);

  for (uint32_t i = 0; i < w; ++i) {
    dst_0[1] = 255;
    dst_1[1] = 255;

    dst_0 += 3;
    dst_1 += 3;
  }

  dst_0 = imStart;
  dst_1 = imStart + (w * 3);

  for (uint32_t j = 0; j < h; ++j) {
    dst_0[1] = 255;
    dst_1[1] = 255;

    dst_0 += step;
    dst_1 += step;
  }
}
