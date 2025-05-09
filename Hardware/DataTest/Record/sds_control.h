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

#ifndef SDS_CONTROL_H_
#define SDS_CONTROL_H_

#include <stdint.h>

#ifdef  __cplusplus
extern "C"
{
#endif

#include "sds_rec_play.h"

// SDSIO control states
#define SDSIO_CLOSED    0       // No SDS I/O pipes open
#define SDSIO_OPEN      1       // SDS I/O pipes open and ready for read/write operations
#define SDSIO_CLOSING   2       // Request to terminate the current algorithm execution
#define SDSIO_HALTED    3       // Algorithm execution completed after SDSIO_CLOSING state 

// Assert macro
#define SDS_ASSERT(cond)        if (!(cond) && (!sdsError.occurred)) { \
  sdsError.occurred = 1U; sdsError.file = __FILE__; sdsError.line = __LINE__; }

// Error information structure
typedef struct {
  uint8_t     occurred;
  uint8_t     reported;
  const char *file;
  uint32_t    line;
} sdsError_t;

// Error information
extern sdsError_t       sdsError;

// Recorder active status
extern volatile uint8_t sdsio_state;

// Recorder identifiers
extern sdsRecPlayId_t   recIdDataInput;
extern sdsRecPlayId_t   recIdDataOutput;

// SDS control thread function
extern void sdsControlThread (void *argument);

#ifdef  __cplusplus
}
#endif

#endif