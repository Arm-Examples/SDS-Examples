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

#include "sds_algorithm_config.h"
#include "sds_algorithm.h"

#include "ei_run_classifier.h"
#include "edge-impulse-sdk/porting/ei_classifier_porting.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "model-parameters/model_variables.h"
#include "numpy.hpp"


static const uint8_t *ptr_in_data = NULL;
static signal_t slice_signal;


// Get audio data for inference.
// Audio data is converted from int16 to float and then returned in `out_ptr`.
int get_data (size_t offset, size_t length, float *out_ptr) {

  // Debug print, to check data offsets and lengths
  // printf("ofs = %4i, len = %4i\n", offset, length);

  if (ptr_in_data == NULL) {
    return -1;
  }

  return numpy::int16_to_float((int16_t *)ptr_in_data + (offset * sizeof(int16_t)), out_ptr, length);
}

/**
  \fn           int32_t InitAlgorithm (void)
  \brief        Initialize algorithm under test.
  \return       0 on success; -1 on error
*/
extern "C" int32_t InitAlgorithm (void) {

  // Initialize the classifier (necessary for continuous classification)
  run_classifier_init();

  slice_signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
  slice_signal.get_data = &get_data;

  return 0;
}

/**
  \fn           int32_t ExecuteAlgorithm (const uint8_t *in_buf, uint32_t in_num, uint8_t *out_buf, uint32_t out_num)
  \brief        Execute algorithm under test.
  \param[in]    in_buf          pointer to memory buffer containing input data for algorithm
  \param[in]    in_num          number of data bytes in input data buffer (in bytes)
  \param[out]   out_buf         pointer to memory buffer for returning algorithm output
  \param[in]    out_num         maximum number of data bytes returned as algorithm output (in bytes)
  \return       0 on success; -1 on error
*/
extern "C" int32_t ExecuteAlgorithm (const uint8_t *in_buf, uint32_t in_num, uint8_t *out_buf, uint32_t out_num) {
  ei_impulse_result_t result = {nullptr};
  float *ptr_out_data;

  if ((in_buf == NULL) || (in_num == 0U)) {
    return -1;
  }

  // Register input data for `get_data` callback
  ptr_in_data = in_buf;

  // Run the classifier
  EI_IMPULSE_ERROR res = run_classifier_continuous(&slice_signal, &result, false);

  if (res != 0) {
    ei_printf("ERR: Failed to run classifier\n");
    return -1;
  }

  // If `helloworld` probability is higher then 60 % then
  // display classification results to the STDIO
  if (result.classification[0].value >= 0.6f) {
    display_results(&ei_default_impulse, &result);
  }

  // Output, for example:
  // Classification predictions:
  //   helloworld: 0.964844
  //   noise: 0.023438
  //   unknown: 0.011719
  // Timing: DSP 19 ms, inference 10 ms, anomaly 0 ms

  if ((out_buf != NULL) && (out_num >= SDS_ALGO_DATA_OUT_BLOCK_SIZE)) {
    ptr_out_data = (float *)out_buf;
    for (uint8_t i = 0U; i < EI_CLASSIFIER_NN_OUTPUT_COUNT; i++) {
      ptr_out_data[i] = result.classification[i].value;
    }
  }

  return 0;
}
