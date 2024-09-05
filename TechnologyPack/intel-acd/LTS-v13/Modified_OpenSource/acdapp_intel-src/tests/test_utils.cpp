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

#include "test_utils.hpp"

#include <stdio.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "safe_str_lib.h"

bool enableDebug = false;
void Logging(cJSON* cjson, char* description)
{
    if (enableDebug)
    {
        char* jsonStr = NULL;
        if (cjson != NULL)
        {
            jsonStr = cJSON_Print(cjson);
            printf("%s%s\n", description, jsonStr);
        }
        else
        {
            printf("%s(NULL)\n", description);
        }
        free(jsonStr);
    }
}

char* readTestFile(char* filename)
{
    char* buffer = NULL;
    uint64_t length = 0;
    FILE* fp = fopen(filename, "r");
    size_t result = 0;
    if (fp != NULL)
    {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = (char*)calloc(length, sizeof(char));
        if (buffer)
        {
            result = fread(buffer, 1, length, fp);
            if (result != length)
            {
                printf("Size Error!\n");
            }
        }
        fclose(fp);
    }
    else
    {
        printf("Fail to open %s\n", filename);
    }
    return buffer;
}

char* removeQuotes(char* str)
{
    char* ptr = str;
    ptr++;
    ptr[strnlen_s(ptr, sizeof(ptr)) - 1] = 0;
    return ptr;
}
