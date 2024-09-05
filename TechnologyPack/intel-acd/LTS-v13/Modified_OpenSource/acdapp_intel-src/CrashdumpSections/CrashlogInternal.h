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

#ifndef CRASHLOG_INTERNAL_H
#define CRASHLOG_INTERNAL_H

acdStatus initiateCrashlogTriggerRearm(const CPUInfo* const cpuInfo);
acdStatus storeCrashlog(cJSON* const pJsonChild, const uint16_t agent,
                        const crashlogAgentDetails* const agentDetails,
                        const uint64_t* const rawCrashlog,
                        const agentsInfoInputFile* const agentsInfo);
acdStatus
    collectCrashlogForAgent(const CPUInfo* const cpuInfo, const uint16_t agent,
                            const crashlogAgentDetails* const agentDetails,
                            uint64_t* const rawCrashlog, RunTimeInfo runTimeInfo);
crashlogAgentDetails getCrashlogDetailsForAgent(const CPUInfo* const cpuInfo,
                                                const uint16_t agent);
uint16_t getNumberOfCrashlogAgents(const CPUInfo* const cpuInfo);
bool isTelemetrySupported(const CPUInfo* const cpuInfo);
bool isCpuCrashlogEnabled(const int clientAddr);
void logError(cJSON* const pJsonChild, const char* errorString);
acdStatus logCrashlogEBG(const CPUInfo* const cpuInfo, cJSON* const pJsonChild,
                         const uint16_t agentsNum, RunTimeInfo runTimeInfo);
acdStatus isValidAgent(const CPUInfo* const cpuInfo,
                       const crashlogAgentDetails* const agentDetails,
                       const agentsInfoInputFile* const agentsInfo);
acdStatus getAgentName(const crashlogAgentDetails* const agentDetails,
                       char* sectionNameString, const uint16_t length,
                       const agentsInfoInputFile* const agentsInfo);

#endif // CRASHLOG_INTERNAL_H