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

#include <stdio.h>
#include <stdbool.h>
#include "RTE_Components.h"
#include "cmsis_os2.h"
#include "cmsis_vio.h"
#include "sds_control.h"
#include "sds_rec_play.h"
#include "DataTest.h"

// Configuration

// SDS Recorder data buffers
#ifndef REC_BUF_SIZE_DATA_IN
#define REC_BUF_SIZE_DATA_IN            8192U
#endif
#ifndef REC_BUF_SIZE_DATA_OUT
#define REC_BUF_SIZE_DATA_OUT           1536U
#endif

extern  int32_t  socket_startup (void);


// Recorder error information
sdsError_t       sdsError = { 0U, 0U, NULL, 0U };

// Recorder active status
volatile uint8_t sdsio_state = SDSIO_CLOSED;

// Recorder identifiers
sdsRecPlayId_t   recIdDataInput  = NULL;
sdsRecPlayId_t   recIdDataOutput = NULL;

// Idle time counter
static volatile  uint32_t idle_cnt;

// Recorder buffers
static uint8_t   rec_buf_data_in [REC_BUF_SIZE_DATA_IN];
static uint8_t   rec_buf_data_out[REC_BUF_SIZE_DATA_OUT];

// Recorder event callback
static void recorder_event_callback (sdsRecPlayId_t id, uint32_t event) {
  if ((event & SDS_REC_PLAY_EVENT_IO_ERROR) != 0U) {
    SDS_ASSERT(false);
  }
}

// Recording control thread function.
// Toggle recording via USER push-button.
// Toggle LED0 every 1 second to see that the thread is alive.
// Turn on LED1 when recording is started, turn it off when recording is stopped.
__NO_RETURN void sdsControlThread (void *argument) {
  uint8_t btn_val, keypress;
  uint8_t btn_prev = 0U;
  uint8_t led0_val = 0U;
  int32_t status;
  uint32_t no_load_cnt, prev_cnt;
  uint32_t interval_time, cnt = 0U;

  // Initialize idle counter
  idle_cnt = 0U;
  osDelay(10U);
  no_load_cnt = idle_cnt;

#ifdef RTE_SDS_IO_SOCKET
  // Initialize socket interface
  status = socket_startup();
  SDS_ASSERT(status == 0);
#endif

  // Initialize SDS recorder
  status = sdsRecPlayInit(recorder_event_callback);
  SDS_ASSERT(status == SDS_REC_PLAY_OK);

  osThreadNew(AlgorithmThread, NULL, NULL);

  interval_time = osKernelGetTickCount();
  prev_cnt      = idle_cnt;

  for (;;) {
    // Monitor user button
    btn_val  = vioGetSignal(vioBUTTON0);
    keypress = btn_val & ~btn_prev;
    btn_prev = btn_val;

    // Control SDS recorder
    switch (sdsio_state) {
      case SDSIO_CLOSED:
        if (!keypress) break;

        // Start recording the data
        recIdDataInput  = sdsRecOpen("DataInput",
                                      rec_buf_data_in,
                                      sizeof(rec_buf_data_in));
        SDS_ASSERT(recIdDataInput != NULL);

        recIdDataOutput = sdsRecOpen("DataOutput",
                                      rec_buf_data_out,
                                      sizeof(rec_buf_data_out));
        SDS_ASSERT(recIdDataOutput != NULL);

        printf("Start Recording\n");

        // Turn LED1 on
        vioSetSignal(vioLED1, vioLEDon);
        sdsio_state = SDSIO_OPEN;
        break;

      case SDSIO_OPEN:
        if (!keypress) break;

        // Request to stop algorithm execution
        sdsio_state = SDSIO_CLOSING;
        break;

      case SDSIO_HALTED:
        // Stop recording the data
        status = sdsRecClose(recIdDataInput);
        SDS_ASSERT(status == SDS_REC_PLAY_OK);

        status = sdsRecClose(recIdDataOutput);
        SDS_ASSERT(status == SDS_REC_PLAY_OK);

        printf("Stop Recording\n");

        // Turn LED1 off
        vioSetSignal(vioLED1, vioLEDoff);
        sdsio_state = SDSIO_CLOSED;
        break;
    }

    interval_time += 100U;
    osDelayUntil(interval_time);

    // Do 1 second interval
    if (++cnt == 10U) {
      cnt = 0U;

      // Print idle factor
      printf("%d%% idle\n",(idle_cnt - prev_cnt) / no_load_cnt);
      prev_cnt = idle_cnt;

      // Toggle LED0
      led0_val ^= 1U;
      vioSetSignal(vioLED0, led0_val);
    }
  }
}

// Measure system idle time
__NO_RETURN void osRtxIdleThread(void *argument) {
  (void)argument;

  for (;;) {
    idle_cnt++;
  }
}
