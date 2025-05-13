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

#include "algorithm_config.h"
#include "algorithm.h"

#include "cmsis_os2.h"
#include "sds_control.h"

#include "algorithm_ml.h"
#include "algorithm_data_in.h"

// Input data buffer
static uint8_t data_in_buf[DATA_IN_BLOCK_SIZE] __ALIGNED(4);

// Output data buffer
static uint8_t data_out_buf[DATA_OUT_BLOCK_SIZE] __ALIGNED(4);


// Algorithm Thread function
__NO_RETURN void AlgorithmThread (void *argument) {
  uint32_t timestamp;
  int32_t  retv;
  (void)argument;

  DataInputInit();                      // Initialize data acquisition
  ML_Init();                            // Initialize ML

  for (;;) {
    // Get data input block for ML inference
    if (DataInputGet(data_in_buf, sizeof(data_in_buf)) < 0) {
      // If there was an error retrieving data ML inference and try to get data again
      continue;
    }

    // Execute ML inference
    ML_Infer((const uint8_t *)data_in_buf, sizeof(data_in_buf), data_out_buf, sizeof(data_out_buf));

    if (sdsio_state == SDSIO_OPEN) {
      timestamp = osKernelGetTickCount();

      // Record ML input data
      retv = sdsRecWrite(recIdDataInput, timestamp, &data_in_buf, sizeof(data_in_buf));
      SDS_ASSERT(retv == sizeof(data_in_buf));

      // Record ML output data
      retv = sdsRecWrite(recIdDataOutput, timestamp, &data_out_buf, sizeof(data_out_buf));
      SDS_ASSERT(retv == sizeof(data_out_buf));
    }

    if (sdsio_state == SDSIO_CLOSING) {
      // Algorithm execution stopped, transit to halted state
      sdsio_state = SDSIO_HALTED;
    }
  }
}
