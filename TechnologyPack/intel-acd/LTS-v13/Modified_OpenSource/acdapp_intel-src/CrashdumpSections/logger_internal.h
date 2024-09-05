/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2021 Intel Corporation.
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

#ifndef LOGGER_INTERNAL_H
#define LOGGER_INTERNAL_H
#ifndef SPX_BMC_ACD
#include <cjson/cJSON.h>
#endif
#include <stdint.h>
#include <stdio.h>

#define LOGGER_JSON_PATH_STRING_LEN 128
#define MAX_NUM_PATH_LEVELS 11

typedef struct
{
    char* pathStringToken;
    char pathString[LOGGER_JSON_PATH_STRING_LEN];
    char* pathLevelToken[MAX_NUM_PATH_LEVELS];
    int numberOfTokens;
} PathParsing;

typedef struct
{
    char* registerName;
    char* sectionName;
    bool extraLevel;
    bool logRegister;
    uint8_t size;
    bool sizeFromOutput;
    int rootAtLevel;
    cJSON* jsonOutput;
    bool zeroPaddedPrint;
} NameProcessing;

typedef struct
{
    uint8_t cpu;
    uint8_t core;
    uint8_t thread;
    uint8_t cha;
    uint8_t repeats;
    bool skipFlag;
    bool skipCrashCores;
    bool skipOnFailFromInputFile;
    int version;
    char* currentSectionName;
} ContextLogger;

typedef struct
{
    PathParsing pathParsing;
    NameProcessing nameProcessing;
    ContextLogger contextLogger;
} LoggerStruct;

#endif // LOGGER_INTERNAL_H
