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

#include <stddef.h>
#include <stdio.h>
#include "cmsis_os2.h"
#include "cmsis_compiler.h"
#include "sds_algorithm_config.h"
#include "sds_data_in.h"
#include "vstream_accelerometer.h"

// Flag for signaling block of (accelerometer) data was captured
#define DATA_BLOCK_READY_FLAG           1U

// Accelerometer sensor raw data buffer, size = (input raw data block size in bytes * 2), for double buffering
static uint8_t vstream_buf[(EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * sizeof(int16_t)) * 2] __ALIGNED(4);

// Event flag for signaling that block of (accelerometer) data was captured
static osEventFlagsId_t evt_id_EventFlags = NULL;

// Pointer to accelerometer vStream driver
static vStreamDriver_t *ptrDriver_vStreamAccel = &Driver_vStreamAccelerometer;

// Function that handles accelerometer stream events
static void vStreamAccelEvent (uint32_t event_flags) {

  if ((event_flags & VSTREAM_EVENT_DATA) != 0U) {
    // New block of data is ready
    osEventFlagsSet(evt_id_EventFlags, DATA_BLOCK_READY_FLAG);
  }
  if ((event_flags & VSTREAM_EVENT_OVERFLOW) != 0U) {
    printf("Warning: Accelerometer overflow event!\r\n");
  }
}

/**
  \fn           int32_t InitInputData (void)
  \brief        Initialize system for acquiring input data.
  \return       0 on success; -1 on error
*/
int32_t InitInputData (void) {

  evt_id_EventFlags = osEventFlagsNew(NULL);
  if (evt_id_EventFlags == NULL) {
    return -1;
  }
  if (ptrDriver_vStreamAccel->Initialize(vStreamAccelEvent) != 0) {
    return -1;
  }
  if (ptrDriver_vStreamAccel->SetBuf(&vstream_buf, sizeof(vstream_buf), sizeof(vstream_buf) / 2) != 0) {
    return -1;
  }
  if (ptrDriver_vStreamAccel->Start(VSTREAM_MODE_CONTINUOUS) != 0) {
    return -1;
  }

  return 0;
}

/**
  \fn           int32_t GetInputData (uint8_t *buf, uint32_t max_len)
  \brief        Get input data block as required for algorithm under test.
  \details      Size of this block has to match size expected by algorithm under test.
  \param[out]   buf             pointer to memory buffer for acquiring input data
  \param[in]    max_len         maximum number of bytes of input data to acquire
  \return       number of data bytes returned; -1 on error
*/
int32_t GetInputData (uint8_t *buf, uint32_t max_len) {
  int16_t *ptr_accel_data_raw;
  float   *ptr_buf;

  // Input data used for SDS recording are accelerometer data scaled and converted to float values

  // Check input parameters
  if ((buf == NULL) || (max_len == 0U)) {
    return -1;
  }

  // Check if buffer can fit expected data
  if (max_len < SDS_ALGO_DATA_IN_BLOCK_SIZE) {
    return -1;
  }

  uint32_t flags = osEventFlagsWait(evt_id_EventFlags, DATA_BLOCK_READY_FLAG, osFlagsWaitAny, osWaitForever);

  if (((flags & osFlagsError)          == 0U) &&        // If not an error and
      ((flags & DATA_BLOCK_READY_FLAG) != 0U)) {        // if flag is data block ready

    // Get pointer to captured accelerometer data
    ptr_accel_data_raw = (int16_t *)ptrDriver_vStreamAccel->GetBlock();
    if (ptr_accel_data_raw == NULL) {
      return -1;
    }

    // Convert each sample from int16 to float scaled to range used during model training and
    // put that data into `buf` buffer
    ptr_buf = (float *)buf;
    for (uint16_t i = 0U; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i++) {
      ptr_buf[i] = ((float)(ptr_accel_data_raw[i])) / 1638.4f;

      // Debug print, to check raw and converted data
      // printf("raw = %6i, cvt = %2.4f\n", ptr_accel_data_raw[i], ptr_buf[i]);
    }

    // Release captured and proessed data block
    ptrDriver_vStreamAccel->ReleaseBlock();

    return SDS_ALGO_DATA_IN_BLOCK_SIZE;
  }

  return -1;
}
