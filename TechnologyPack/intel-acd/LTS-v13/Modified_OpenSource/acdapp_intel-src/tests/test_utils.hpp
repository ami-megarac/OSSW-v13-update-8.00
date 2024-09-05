/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2019 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 *
 ******************************************************************************/

#pragma once
#include <cjson/cJSON.h>
#include <stdint.h>

#define NTIME(n) for (int i = 0; i < n; i++)

// uncomment to enable debug print out
//#define DEBUG_FLAG
#ifdef DEBUG_FLAG
inline void DEBUG_PRINT(cJSON* root, cJSON* expected)
{
    char* jsonStr = NULL;
    jsonStr = cJSON_Print(root);
    printf("%s\n", jsonStr);
    jsonStr = cJSON_Print(expected);
    printf("%s\n", jsonStr);
};

inline void DEBUG_CC_PRINT(uint8_t cc, uint64_t val)
{
    printf("cc:0x%x val:0x%" PRIx64 "\n", cc, val);
};
#endif
char* readTestFile(char* filename);
char* removeQuotes(char* str);
extern bool enableDebug;
void Logging(cJSON* cjson, char* description);
