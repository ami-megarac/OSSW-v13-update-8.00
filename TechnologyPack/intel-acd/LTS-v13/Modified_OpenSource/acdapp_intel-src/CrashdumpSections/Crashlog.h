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

#ifndef CRASHLOG_H
#define CRASHLOG_H

#include "crashdump.h"

#define TELEMETRY_UNSUPPORTED 0
#define TELEMETRY_SUPPORTED 1
#define NO_CRASHLOG_AGENTS 0

#define CRASHLOG_ERROR_JSON_STRING_LEN 256

#define US_MMIO_SEG 0

#define CONFIG_WATCHER_ID 11
#define CONFIG_WATCHER_OFFSET 0
#define REARM_TRIGGER_BIT 28
#define REARM_TRIGGER_MASK (1 << REARM_TRIGGER_BIT)

enum CrashlogAgents
{
    AgentPCodeCrash,
    AgentPUnitCrash,
    AgentOOBMSMCrash,
    AgentTorCrash,
    AgentPMCCrash,
    AgentPMCReset,
    AgentPMCTrace,
    AgentSusram,
    AgentTotalSize
};

typedef struct
{
    uint32_t guid;
    char* crashlogSectionName;
    bool enable;
} guidCrashlogSectionMapping;

typedef struct
{
    uint8_t entryType;
    uint8_t crashType;
    uint32_t uniqueId;
    uint16_t crashSpace;
} crashlogAgentDetails;

typedef struct
{
    guidCrashlogSectionMapping* agentMap;
    uint8_t numberOfCrashlogAgents;
} agentsInfoInputFile;
//-----AMI-start-----
typedef struct
{
    Model pchModel;
    acdStatus (*logCrashlogVx)(const CPUInfo* const cpuInfo, cJSON* pJsonChild);
} CrashlogVx;

acdStatus logCrashlogSection(const CPUInfo* const cpuInfo,
                             cJSON* const outputNode, uint16_t totalAgents,
                             RunTimeInfo runTimeInfo);
//-----AMI-end-----
#endif // CRASHLOG_H
