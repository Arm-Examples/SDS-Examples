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

#ifndef DATA_TEST_H_
#define DATA_TEST_H_

// Recording bandwidth in bytes/sec
#ifndef TEST_BANDWIDTH
#define TEST_BANDWIDTH          100000U
#endif

// Recording interval in ms
#ifndef TEST_INTERVAL
#define TEST_INTERVAL           10U
#endif

// Data block size in bytes
#define DATA_BLOCK_SIZE         (TEST_BANDWIDTH * TEST_INTERVAL / 1000)

// Data buffer size in bytes
// The buffer size should be at least (2 x block size) + 2 KB.
// The minimum recommended buffer size is 0x1000 (4 KB).
#if (2 * DATA_BLOCK_SIZE + 2048U) > 4096U
#define DATA_BUF_SIZE           (2 * DATA_BLOCK_SIZE + 2048U)
#else
#define DATA_BUF_SIZE           4096U
#endif

#ifdef  __cplusplus
extern "C"
{
#endif

// Thread for generating simulated data
extern void AlgorithmThread (void *argument);

#ifdef  __cplusplus
}
#endif

#endif