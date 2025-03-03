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

#include "RTE_Components.h"

#include "cmsis_os2.h"
#include "cmsis_vio.h"

#include "main.h"
#include "os_tick.h"
#include "rec_management.h"
#include "sds_rec.h"

// IMU sensor buffer
struct IMU {
  struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
  } accelerometer;
  struct  {
    uint16_t x;
    uint16_t y;
    uint16_t z;
  } gyroscpe;
} imu_buf[30];

// ML output buffer
struct OUT {
  struct {
    uint16_t x;
    uint16_t y;
  } out;
} ml_buf[10];

// Idle tick counter in sixteenths of ms
static uint32_t cnt_idle = 0;

// Processor usage in percentage (%)
// Add this variable in debugger watch window
float cpu_usage;

// Create dummy test data
static void CreateTestData () {
  static uint16_t index_in, index_out;
  uint16_t val;
  int32_t i;

  // Sensor input data
  for (i = 0; i < 30; i++) {
    val = (index_in + i) % 3000;
    imu_buf[i].accelerometer.x = val;
    val = (val + 250) % 3000;
    imu_buf[i].accelerometer.y = 2999 - val;
    val = (val + 300) % 3000;
    imu_buf[i].accelerometer.z = (val < 1500) ? val : (2999 - val);

    val = (val + 150) % 1500;
    imu_buf[i].gyroscpe.x = val;
    val = (val + 70) % 1500;
    imu_buf[i].gyroscpe.y = 1499 - val;
    val = (val + 120) % 1500;
    imu_buf[i].gyroscpe.z = (val < 750) ? val : (1499 - val);
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

// Calculate CPU usage in percentage
static void calc_cpu_usage(void) {
  static uint32_t cnt;

  // Usage calculation interval is 3 seconds
  if (++cnt >= 300) {
    cpu_usage = (float)(48000 - cnt_idle) / 480.0;
    cnt_idle = cnt = 0;
  }
}

// Thread for generating simulated data
__NO_RETURN void threadDataTest(void *argument) {
  uint32_t n, timestamp;
  (void)argument;

  timestamp = osKernelGetTickCount();
  for (;;) {
    if (recActive != 0U) {
      CreateTestData();

      n = sdsRecWrite(recIdDataInput, timestamp, &imu_buf, sizeof(imu_buf));
      SDS_ASSERT(n == sizeof(imu_buf));

      n = sdsRecWrite(recIdDataOutput, timestamp, &ml_buf, sizeof(ml_buf));
      SDS_ASSERT(n == sizeof(ml_buf));
    }
    else {
      recDone = 1U;
    }
    calc_cpu_usage();

    timestamp += 10U;
    osDelayUntil(timestamp);
  }
}

// Measure system idle time
__NO_RETURN void osRtxIdleThread(void *argument) {
  uint32_t tick, next = 0xFFFFFFFF;
  (void)argument;

  for (;;) {
    __WFI();
    tick = osKernelGetTickCount();
    if (tick == next) {
      // Counts in sixteenths of an interval
      cnt_idle += (16 - 16*OS_Tick_GetCount()/OS_Tick_GetInterval());
    }
    next = tick + 1;
  }
}
