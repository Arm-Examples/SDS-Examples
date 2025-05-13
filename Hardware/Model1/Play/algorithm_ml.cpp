/*
 * Copyright (c) 2025 Arm Limited. All rights reserved.
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

#include "algorithm_ml.h"

#include <string.h>

#include "ei_run_classifier.h"
#include "model-parameters/model_variables.h"


// Input data information
static const uint8_t  *ptr_data_in_buf = NULL;
static       uint32_t  data_in_num     = 0U;

// ML data retrieval function
static signal_t features_signal;

// ML get input data callback
static int raw_feature_get_data (size_t offset, size_t length, float *out_ptr) {

  if ((ptr_data_in_buf != NULL) && (data_in_num = (length * sizeof(float)))) {
    memcpy(out_ptr, ptr_data_in_buf + (offset * sizeof(float)), data_in_num);
    data_in_num = 0U;
  } else {
    return -1;
  }

  return 0;
}

/**
  \fn           int32_t ML_Init (void)
  \brief        Initialize Machine Learning algorithm.
  \return       0 on success; -1 on error
*/
int32_t ML_Init (void) {

  // Clear input data information
  ptr_data_in_buf  = NULL;
  data_in_num      = 0U;

#if 0
  // Summary of inferencing settings (from model_metadata.h)
  ei_printf("Inferencing settings:\n");
  ei_printf("\tClassifier interval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
  ei_printf("\tInput frame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
  ei_printf("\tRaw sample count: %d samples.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
  ei_printf("\tRaw samples per frame: %d samples.\n", EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME);
  ei_printf("\tNumber of output classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
#endif

  // Register data retrieval function
  features_signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
  features_signal.get_data = &raw_feature_get_data;

  return 0;
}

/**
  \fn           int32_t ML_Infer (const uint8_t *in_buf, uint32_t in_num, uint8_t *out_buf, uint32_t out_num)
  \brief        Execute Machine Learning inference.
  \param[in]    in_buf          pointer to memory buffer containing input data for inference
  \param[in]    in_num          number of data bytes in input data buffer (in bytes)
  \param[in]    out_buf         pointer to memory buffer for returning inference results
  \param[in]    out_num         number of data bytes expected to be returned as inference results (in bytes)
  \return       0 on success; -1 on error
*/
int32_t ML_Infer (const uint8_t *in_buf, uint32_t in_num, uint8_t *out_buf, uint32_t out_num) {
  ei_impulse_result_t result = {nullptr};

  // Set input data information
  ptr_data_in_buf  = in_buf;
  data_in_num      = in_num;

  // Do inference
  EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);

  if (res != EI_IMPULSE_OK) {
    ei_printf("ERR: Failed to run classifier\n");
    return -1;
  }

  display_results(&ei_default_impulse, &result);

  // Output, for example:
  // idle: 0.996094
  // snake: 0.000000
  // updown: 0.000000
  // wave: 0.000000

  // If output data pointer is valid and number of output data bytes is as expected by the ML model
  if ((out_buf != NULL) && (out_num == (EI_CLASSIFIER_NN_OUTPUT_COUNT * sizeof(float)))) {
    float *ptr_data_out_buf = (float *)out_buf;

    for (uint8_t i = 0U; i < EI_CLASSIFIER_NN_OUTPUT_COUNT; i++) {
      *ptr_data_out_buf = result.classification[i].value;
      ptr_data_out_buf++;
    }
  }

  return 0;
}
