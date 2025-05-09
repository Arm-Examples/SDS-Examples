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

#include <string.h>
#include "cmsis_os2.h"
#include "sds_control.h"
#include "DataTest.h"

// IMU sensor buffer
struct IMU {
  struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
  } accelerometer;
} imu_buf[DATA_BLOCK_SIZE/6],
  imu_play[DATA_BLOCK_SIZE/6];

// ML output buffer
struct OUT {
  struct {
    uint16_t x;
    uint16_t y;
  } out;
} ml_buf[10],
  ml_play[10];

// Create test data indexes
static uint16_t index_in;
static uint16_t index_out;

// Create dummy test data
static void CreateTestData (void) {
  uint16_t val;
  int32_t i;

  // Sensor input data
  for (i = 0; i < DATA_BLOCK_SIZE/6; i++) {
    val = (index_in + i) % 3000;
    imu_buf[i].accelerometer.x = val;
    val = (val + 250) % 3000;
    imu_buf[i].accelerometer.y = 2999 - val;
    val = (val + 300) % 3000;
    imu_buf[i].accelerometer.z = (val < 1500) ? val : (2999 - val);
  }
  index_in = (index_in + i) % 3000;

  // ML output data
  for (i = 0; i < 10; i++) {
    val = (index_out + i) % 1000;
    ml_buf[i].out.x = val;
    ml_buf[i].out.y = val % 500;
  }
  index_out = (index_out + i) % 1000;
}

// Thread for verifying simulated data
__NO_RETURN void AlgorithmThread (void *argument) {
  uint32_t interval_time;
  int32_t  retv;
  (void)argument;

  interval_time = osKernelGetTickCount();

  for (;;) {
    if (sdsio_state == SDSIO_OPEN) {
      CreateTestData();

      retv = sdsPlayRead(playIdDataInput, NULL, &imu_play, sizeof(imu_play));
      if (retv >= 0) {
        SDS_ASSERT(retv == sizeof(imu_play));
        // Check if the received data matches the generated data
        retv = memcmp(imu_play, imu_buf, sizeof(imu_play));
        SDS_ASSERT(retv == 0);
      }
      else {
        sdsio_state = SDSIO_CLOSING;
      }

      retv = sdsPlayRead(playIdDataOutput, NULL, &ml_play, sizeof(ml_play));
      if (retv >= 0) {
        SDS_ASSERT(retv == sizeof(ml_play));
        // Check if the received data matches the generated data
        retv = memcmp(ml_play, ml_buf, sizeof(ml_play));
        SDS_ASSERT(retv == 0);
      }
      else {
        sdsio_state = SDSIO_CLOSING;
      }
    }

    if (sdsio_state == SDSIO_CLOSING) {
      // Algorithm execution stopped, transit to halted state
      sdsio_state = SDSIO_HALTED;
      index_in = index_out = 0U;
    }
 
    interval_time += TEST_INTERVAL;
    osDelayUntil(interval_time);
  }
}
