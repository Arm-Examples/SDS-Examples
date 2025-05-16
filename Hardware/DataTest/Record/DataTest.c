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
#include "cmsis_os2.h"
#include "sds_control.h"
#include "DataTest.h"

// Buffer size configuration
#ifndef STREAM_IN_BUF_SIZE
#define STREAM_IN_BUF_SIZE              DATA_BUF_SIZE
#endif

#ifndef STREAM_OUT_BUF_SIZE
#define STREAM_OUT_BUF_SIZE             4096U
#endif

// IMU sensor buffer
static struct IMU {
  struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
  } accelerometer;
} imu_buf[DATA_BLOCK_SIZE/6];

// ML output buffer
static struct OUT {
  struct {
    uint16_t x;
    uint16_t y;
  } out;
} ml_buf[10];

// Stream buffers
static uint8_t stream_in_buf[STREAM_IN_BUF_SIZE];
static uint8_t stream_out_buf[STREAM_OUT_BUF_SIZE];

// Buffer identifiers
static sdsRecPlayId_t IdInData  = NULL;
static sdsRecPlayId_t IdOutData = NULL;

// Create dummy test data
static void CreateTestData (void) {
  static uint16_t index;
  uint16_t val;
  int32_t i;

  // Sensor input data
  for (i = 0; i < DATA_BLOCK_SIZE/6; i++) {
    val = (index + i) % 3000;
    imu_buf[i].accelerometer.x = val;
    val = (val + 250) % 3000;
    imu_buf[i].accelerometer.y = 2999 - val;
    val = (val + 300) % 3000;
    imu_buf[i].accelerometer.z = (val < 1500) ? val : (2999 - val);
  }
  index = (index + i) % 3000;
}

// Calculate dummy inference
static void sds_inference (void) {
  uint32_t sum_x, sum_y, sum_z;
  int32_t i;

  // Process input data
  sum_x = sum_y = sum_z = 0U;
  for (i = 0; i < DATA_BLOCK_SIZE/6; i++) {
    sum_x += imu_buf[i].accelerometer.x;
    sum_y += imu_buf[i].accelerometer.y;
    sum_z += imu_buf[i].accelerometer.z;
  }
  sum_x = (sum_x & 0xFFFF) + (sum_x >> 16);
  sum_y = (sum_y & 0xFFFF) + (sum_y >> 16);
  sum_z = (sum_z & 0xFFFF) + (sum_z >> 16);

  // Output data of Algorithm
  for (i = 0; i < 10; i++) {
    ml_buf[i].out.x = (sum_x ^ sum_z) & 0xFFFF;
    ml_buf[i].out.y = (sum_y ^ sum_z) & 0xFFFF;
    sum_z += 12345U;
  }
}

// Open SDS streams
void OpenStreams (void) {
  IdInData = sdsRecOpen("DataInput", stream_in_buf, sizeof(stream_in_buf));
  SDS_ASSERT(IdInData != NULL);

  IdOutData = sdsRecOpen("DataOutputRec", stream_out_buf, sizeof(stream_out_buf));
  SDS_ASSERT(IdOutData != NULL);

  printf("Algorithm Recording Started\n");
}

// Close SDS streams
void CloseStreams (void) {
  int32_t status;

  status = sdsRecClose(IdInData);
  SDS_ASSERT(status == SDS_REC_PLAY_OK);

  status = sdsRecClose(IdOutData);
  SDS_ASSERT(status == SDS_REC_PLAY_OK);

  printf("Algorithm Recording Stopped\n");

  if (sdsError.occurred) {
    printf("Error: Recording Failed\n");
  }
}

// Thread for generating simulated data
__NO_RETURN void AlgorithmThread (void *argument) {
  uint32_t timestamp, interval_time;
  int32_t  retv;
  (void)argument;

  interval_time = osKernelGetTickCount();

  for (;;) {
    if (sdsio_state == SDSIO_OPEN) {
      timestamp = osKernelGetTickCount();
 
      CreateTestData();
      retv = sdsRecWrite(IdInData, timestamp, &imu_buf, sizeof(imu_buf));
      SDS_ASSERT(retv == sizeof(imu_buf));

      // Execute Algorithm under test
      sds_inference();

      // Record output data of Algorithm
      retv = sdsRecWrite(IdOutData, timestamp, &ml_buf, sizeof(ml_buf));
      SDS_ASSERT(retv == sizeof(ml_buf));
    }

    if (sdsio_state == SDSIO_CLOSING) {
      // Algorithm execution stopped, transit to halted state
      sdsio_state = SDSIO_HALTED;
    }

    interval_time += TEST_INTERVAL;
    osDelayUntil(interval_time);
  }
}
