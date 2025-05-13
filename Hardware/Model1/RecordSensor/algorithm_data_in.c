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

#include "algorithm_data_in.h"

#include "algorithm_config.h"

#include <stdio.h>
#include <string.h>

#include "cmsis_os2.h"
#include "cmsis_compiler.h"

#include "vstream_accelerometer.h"


// Configuration

#ifndef SENSOR_RAW_ITEMS_PER_SAMPLE
#define SENSOR_RAW_ITEMS_PER_SAMPLE     (3)     // For accelerometer there are 3 items (x, y, z) per sample
#endif

#ifndef SENSOR_RAW_BYTES_PER_SAMPLE
#define SENSOR_RAW_BYTES_PER_SAMPLE     (6)     // For accelerometer there are 6 bytes per sample
#endif

#ifndef SENSOR_RAW_SAMPLES_PER_SLICE
#define SENSOR_RAW_SAMPLES_PER_SLICE    (125)   // Number of samples per inference slice
#endif


// Raw sensor data sample structure
typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} accelerometer_sample_t;


// Pointer to vStream driver
static vStreamDriver_t *ptrDriver_vStreamAccelerometer = &Driver_vStreamAccelerometer;

// Event Flag for signaling a slice of sensor data is ready
static osEventFlagsId_t data_ready_evt_id = NULL;

// Raw sensor data (2 * ML slice)
static uint8_t vstream_buf[(SENSOR_RAW_SAMPLES_PER_SLICE * SENSOR_RAW_BYTES_PER_SAMPLE) * 2] __ALIGNED(4);

// Scaled input data (1 slice of items in float format for ML)
static float   scaled_sensor_data[SENSOR_RAW_SAMPLES_PER_SLICE * SENSOR_RAW_ITEMS_PER_SAMPLE];


// Local helper functions

// Initialize mechanism for signaling data ready event
static int32_t DataReadyEventInit (void) {
  data_ready_evt_id = osEventFlagsNew(NULL);

  if (data_ready_evt_id == NULL) {
    return -1;
  }

  return 0;
}

// Set data ready event
static void DataReadyEventSet (void) {
  (void)osEventFlagsSet(data_ready_evt_id, 1U);
}

// Wait for data ready event
static int32_t DataReadyEventWait (void) {
  uint32_t flags;
 
  if (osEventFlagsWait(data_ready_evt_id, 1U, osFlagsWaitAny, osWaitForever) != 1U) {
    return -1;
  }

  return 0;
}

// Function that sends event when data is available with vStream
static void vStreamSensorEvent (uint32_t event_flags) {

  if ((event_flags & VSTREAM_EVENT_DATA) != 0U) {
    // Signal to DataInputGet function that sensor data is ready
    DataReadyEventSet();
  }

  if ((event_flags & VSTREAM_EVENT_OVERFLOW) != 0U) {
    printf("Warning: Accelerometer overflow event!\r\n");
  }
}


// Public functions

/**
  \fn           int32_t DataInputInit (void)
  \brief        Initialize system for acquiring input data for ML.
  \return       0 on success; -1 on error
*/
int32_t DataInputInit (void) {

  // Initialize vStream driver for accelerometer
  if (ptrDriver_vStreamAccelerometer->Initialize(vStreamSensorEvent) != VSTREAM_OK) {
    return -1;
  }
  if (ptrDriver_vStreamAccelerometer->SetBuf(&vstream_buf, sizeof(vstream_buf), SENSOR_RAW_SAMPLES_PER_SLICE * SENSOR_RAW_BYTES_PER_SAMPLE) != VSTREAM_OK) {
    return -1;
  }
  if (ptrDriver_vStreamAccelerometer->Start(VSTREAM_MODE_CONTINUOUS) != VSTREAM_OK) {
    return -1;
  }

  // Initialize data ready signaling mechanism
  if (DataReadyEventInit() != 0) {
    return -1;
  }                 

  return 0;
}

/**
  \fn           int32_t DataInputGet (uint8_t *buf, uint32_t len)
  \brief        Get input data block as required for ML inference.
  \detail       Size of this block has to match size expected by ML model.
  \param[in]    buf             pointer to memory buffer for acquiring input data
  \param[in]    max_len         maximum number of bytes of input data to acquire
  \return       number of data bytes returned; -1 on error
*/
extern int32_t DataInputGet (uint8_t *buf, uint32_t max_len) {
  accelerometer_sample_t *ptr_acc_sample;
  float                  *ptr_scaled_sensor_data;

  // Wait for data available from sensor via vStream
  if (DataReadyEventWait() != 0) {
    return -1;
  }

  // Check if requested max_len is less than 1 slice of sensor data expected by ML
  if (max_len < DATA_IN_BLOCK_SIZE) {
    return -1;
  }

  // Scale sensor data -----------------
  // Convert raw sensor data from 16-bit signed values to scaled float values expected by ML
  // For example: 1 g = raw value 16384 => scaled value 10.0

  // Get pointer to raw sensor data
  ptr_acc_sample = (accelerometer_sample_t *)ptrDriver_vStreamAccelerometer->GetBlock();

  // Set pointer to buffer for scaled sensor data
  ptr_scaled_sensor_data = scaled_sensor_data;

  for (uint32_t i = 0U; i < SENSOR_RAW_SAMPLES_PER_SLICE; i++) {
    // Convert each raw sample value for x, y, z from int16 to float scaled to range used during model training
    ptr_scaled_sensor_data[0]  = ((float)ptr_acc_sample->x) / 1638.4f;
    ptr_scaled_sensor_data[1]  = ((float)ptr_acc_sample->y) / 1638.4f;
    ptr_scaled_sensor_data[2]  = ((float)ptr_acc_sample->z) / 1638.4f;

    // Used for debugging, to visually check that data is plausible
    // printf("Acc x=%i, y=%i, z=%i\r\n",         ptr_acc_sample->x           ,         ptr_acc_sample->y           ,         ptr_acc_sample->z           );
    // printf("Acc x=%f, y=%f, z=%f\r\n", ((float)ptr_acc_sample->x) / 1638.4f, ((float)ptr_acc_sample->y) / 1638.4f, ((float)ptr_acc_sample->z) / 1638.4f);

    ptr_acc_sample            += 1U;
    ptr_scaled_sensor_data    += SENSOR_RAW_ITEMS_PER_SAMPLE;
  }

  // Release raw sensor data block
  ptrDriver_vStreamAccelerometer->ReleaseBlock();

  // Copy scaled values to buffer
  memcpy(buf, scaled_sensor_data, DATA_IN_BLOCK_SIZE);

  return DATA_IN_BLOCK_SIZE;
}
