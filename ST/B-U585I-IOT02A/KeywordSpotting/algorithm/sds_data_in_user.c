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
#include "vstream_audio_in.h"

// Flag for signaling block of (audio in) data was captured
#define DATA_BLOCK_READY_FLAG           1U

// Audio In raw data buffer, size = (input raw data block size in bytes * 2), for double buffering
static uint8_t vstream_buf[(EI_CLASSIFIER_SLICE_SIZE * sizeof(int16_t)) * 2] __ALIGNED(4);

// Event flag for signaling that block of (audio in) data was captured
static osEventFlagsId_t evt_id_EventFlags = NULL;

// Pointer to Audio In vStream driver
static vStreamDriver_t *ptrDriver_vStreamAudioIn = &Driver_vStreamAudioIn;

// Function that handles Audio In stream events
static void vStreamAudioInEvent (uint32_t event_flags) {

  if ((event_flags & VSTREAM_EVENT_DATA) != 0U) {
    // New block of data is ready
    osEventFlagsSet(evt_id_EventFlags, DATA_BLOCK_READY_FLAG);
  }
  if ((event_flags & VSTREAM_EVENT_OVERFLOW) != 0U) {
    printf("Warning: Audio In data overflow event!\r\n");
  }
}

// Function that removes DC offset in audio signal using simple high-pass filter
static int16_t remove_dc (int16_t sample) {
  static int16_t in_  = 0;      // Previous input sample
  static int32_t out_ = 0;      // Previous output sample

  // R ≈ 0.995 in Q15
  const int32_t R = 32604;

  int32_t out = sample - in_ + ((R * out_) >> 15);

  in_  = sample;
  out_ = out;

  // Crop to int16
  out &= 0x0000FFFF;

  return (int16_t)out;
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
  if (ptrDriver_vStreamAudioIn->Initialize(vStreamAudioInEvent) != 0) {
    return -1;
  }
  if (ptrDriver_vStreamAudioIn->SetBuf(&vstream_buf, sizeof(vstream_buf), sizeof(vstream_buf) / 2) != 0) {
    return -1;
  }
  if (ptrDriver_vStreamAudioIn->Start(VSTREAM_MODE_CONTINUOUS) != 0) {
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
  int16_t *ptr_audio_in_data_raw;
  int16_t *ptr_buf;

  // Input data used for SDS recording are audio in data with removed DC offset in int16 format

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

    // Get pointer to captured Audio In data
    ptr_audio_in_data_raw = (int16_t *)ptrDriver_vStreamAudioIn->GetBlock();
    if (ptr_audio_in_data_raw == NULL) {
      return -1;
    }

    // Remove DC offset from audio data and put that data into `buf` buffer
    ptr_buf = (int16_t *)buf;
    for (uint16_t i = 0U; i < EI_CLASSIFIER_SLICE_SIZE; i++) {
      ptr_buf[i] = remove_dc(ptr_audio_in_data_raw[i]);
    }

    // Release captured and proessed data block
    ptrDriver_vStreamAudioIn->ReleaseBlock();

    return (EI_CLASSIFIER_SLICE_SIZE * sizeof(int16_t));
  }

  return -1;
}
