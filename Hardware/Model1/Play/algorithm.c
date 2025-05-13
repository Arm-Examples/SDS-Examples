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

#include <string.h>

#include "cmsis_os2.h"
#include "sds_control.h"

#include "algorithm_ml.h"

// Input data buffer
static uint8_t data_in_buf[DATA_IN_BLOCK_SIZE] __ALIGNED(4);

// Output data buffer
static uint8_t data_out_buf[DATA_OUT_BLOCK_SIZE] __ALIGNED(4);

// Play output data buffer
static uint8_t play_data_out_buf[DATA_OUT_BLOCK_SIZE] __ALIGNED(4);


// Algorithm Thread function
__NO_RETURN void AlgorithmThread (void *argument) {
  uint32_t play_data_in_timestamp_last = 0U;
  uint32_t play_data_in_timestamp;
  uint32_t play_data_out_timestamp;
  int32_t  retv;
  (void)argument;

  ML_Init();                            // Initialize ML

  for (;;) {
    if (sdsio_state == SDSIO_OPEN) {

      // Read input data from previously recorded .sds file
      retv = sdsPlayRead(playIdDataInput, &play_data_in_timestamp, &data_in_buf, sizeof(data_in_buf));
      if (retv >= 0) {
        SDS_ASSERT(retv == sizeof(data_in_buf));
      } else {
        sdsio_state = SDSIO_CLOSING;
        continue;
      }

      // Delay according to recorded timestamp
      if (play_data_in_timestamp_last != 0U) {
        osDelayUntil(osKernelGetTickCount() + (play_data_in_timestamp - play_data_in_timestamp_last));
      }
      play_data_in_timestamp_last = play_data_in_timestamp;

      // Execute ML inference
      ML_Infer((const uint8_t *)data_in_buf, sizeof(data_in_buf), data_out_buf, sizeof(data_out_buf));

      // Read output data from previously recorded .sds file
      // and check it matches results of ML inference on same data
      retv = sdsPlayRead(playIdDataOutput, &play_data_out_timestamp, &play_data_out_buf, sizeof(play_data_out_buf));
      if (retv >= 0) {
        SDS_ASSERT(retv == sizeof(play_data_out_buf));
        // Check that timestamp of recorded in and out data are the same
        SDS_ASSERT(play_data_in_timestamp == play_data_out_timestamp);
        // Check if the playback data matches the results of ML inference
        retv = memcmp(play_data_out_buf, data_out_buf, sizeof(data_out_buf));
        SDS_ASSERT(retv == 0);
      } else {
        sdsio_state = SDSIO_CLOSING;
      }
    } else {
      osDelay(100U);
    }

    if (sdsio_state == SDSIO_CLOSING) {
      // Algorithm execution stopped, transit to halted state
      sdsio_state = SDSIO_HALTED;
    }
  }
}
