/******************************************************************************
 * @file     vstream_accelerometer.h
 * @brief    CMSIS Virtual Streaming interface Driver header for
 *           Accelerometer sensor (ISM330DHCX) on the
 *           STMicroelectronics B-U585I-IOT02A board
 * @version  V1.0.0
 * @date     11. April 2025
 ******************************************************************************/
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

#ifndef VSTREAM_ACCELEROMETER_H_
#define VSTREAM_ACCELEROMETER_H_

#ifdef  __cplusplus
extern  "C"
{
#endif

#include "cmsis_vstream.h"

// External driver structure

extern vStreamDriver_t Driver_vStreamAccelerometer;

#ifdef  __cplusplus
}
#endif

#endif
