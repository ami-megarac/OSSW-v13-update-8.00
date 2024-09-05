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

#ifndef CRASHDUMP_FLOW_H
#define CRASHDUMP_FLOW_H
#define MAX_CORE_MASK 64
#define TIME_KEY "_time"
#define GLOBAL_TIME_KEY "_total_time"
#define VALIDATE_ENABLE_KEY "ValidateInput"
#define REPEAT_LOWER_BOUND 1
#define REPEAT_UPPER_BOUND 1000
#include "cmdprocessor.h"
#include "crashdump.h"
#include "inputparser.h"
#include "logger.h"
#include "validator.h"

void ProcessPECICmds(ENTRY* entry, CPUInfo* cpuInfo, cJSON* peciCmds,
                     CmdInOut* cmdInOut, InputParserErrInfo* errInfo,
                     LoggerStruct* loggerStruct, cJSON* outRoot,
                     RunTimeInfo* runTimeInfo);
acdStatus fillNewSection(cJSON* root, CPUInfo* cpuInfo, uint8_t cpu,
                         RunTimeInfo* runTimeInfo, uint8_t sectionIndex);

#endif // CRASHDUMP_FLOW_H
