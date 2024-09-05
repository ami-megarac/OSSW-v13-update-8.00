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

#ifndef CRASHDUMP_INPUTPARSER_H
#define CRASHDUMP_INPUTPARSER_H

#include "../CrashdumpSections/crashdump.h"
#include "cmdprocessor.h"
#include "logger.h"

#define TARGET "Target"
#define CHA "CHA"
#define CORE "Core"
#define CORETHREAD "(CORE*2)+THREAD"

#define MD_STARTUP "STARTUP"
#define MD_EVENT "EVENT"
#define MD_OVERWRITTEN "OVERWRITTEN"
#define MD_INVALID "INVALID"

typedef struct
{
    char* sectionName;
    char* cmdGroup;
    uint8_t cmdGroupPos;
    uint8_t paramsPos;
    void* val;
} InputParserErrInfo;

typedef struct
{
    bool loopOnCPU;
    bool loopOnCHA;
    bool loopOnCore;
    bool loopOnThread;
} LoopOnFlags;

char* decodeState(int value);
acdStatus UpdateParams(CPUInfo* cpuInfo, CmdInOut* cmdInOut,
                       LoggerStruct* loggerStruct, InputParserErrInfo* errInfo);

acdStatus ResetParams(cJSON* params, cJSON* paramsTracker);
void ReadLoops(cJSON* sectionParams, LoopOnFlags* loopOnFlags);

#endif // CRASHDUMP_INPUTPARSER_H
