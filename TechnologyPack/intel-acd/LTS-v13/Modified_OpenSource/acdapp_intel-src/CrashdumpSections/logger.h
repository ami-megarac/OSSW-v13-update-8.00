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

#ifndef LOGGER_H
#define LOGGER_H
#include "../CrashdumpSections/crashdump.h"
#include "../CrashdumpSections/utils.h"
#include "cmdprocessor.h"
#include "logger_internal.h"

void Logger(CmdInOut* cmdInOut, cJSON* root, LoggerStruct* loggerStruct);
acdStatus getPath(cJSON* section, LoggerStruct* loggerStruct);
acdStatus ParsePath(LoggerStruct* loggerStruct);
acdStatus ParseNameSection(CmdInOut* cmdInOut, LoggerStruct* loggerStruct);
void logVersion(CmdInOut* cmdInOut, cJSON* root, LoggerStruct* loggerStruct);
void GenerateVersion(cJSON* Section, int* Version);
void logSectionRunTime(cJSON* parent, struct timespec* start, char* key);
void logRecordDisabled(CmdInOut* cmdInOut, cJSON* root,
                       LoggerStruct* loggerStruct);
acdStatus GenerateJsonPath(CmdInOut* cmdInOut, cJSON* root,
                           LoggerStruct* loggerStruct, bool useRootPath);
void LogValue(char* registerName, CmdInOut* cmdInOut,
              LoggerStruct* loggerStruct, cJSON* parent);
#define LOGGER_JSON_STRING_LEN 64
#define LOGGER_DATA_64bits "0x%" PRIx64 ""
#define LOGGER_DATA_64_bits_CC_RC "0x%" PRIx64 ",CC:0x%x,RC:0x%x"
#define LOGGER_FIXED_DATA_CC_RC "0x0,CC:0x%x,RC:0x%x"
#define LOGGER_INFO_TIMEOUT "%ds"
#define LOGGER_GLOBAL_TIMEOUT "_cpu%d_%s_global_timeout:"
#define LOGGER_SECTION_TIMEOUT "_cpu%d_%s_section_timeout:"
#define LOGGER_VERSION_STRING "_version"
#define LOGGER_NA "N/A"
#define RECORD_ENABLE "_record_enable"
#define namePosition 0
#define sectionPosition 1

#define LOGGER_CPU "cpu%d"
#define LOGGER_CORE "core%d"
#define LOGGER_THREAD "thread%d"
#define LOGGER_CBO "cbo%d"
#define LOGGER_CBO_UC "CBO%d"
#define LOGGER_CHA "cha%d"
#define LOGGER_REPEAT "%d"

#endif // LOGGER_H
