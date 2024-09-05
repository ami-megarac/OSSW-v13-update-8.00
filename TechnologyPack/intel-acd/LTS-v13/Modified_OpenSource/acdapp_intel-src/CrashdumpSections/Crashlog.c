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

#include "Crashlog.h"

#include "CrashlogInternal.h"
#include "base64Encode.h"
#include "utils.h"

acdStatus getAgentName(const crashlogAgentDetails* const agentDetails,
                       char* sectionNameString, const uint16_t length,
                       const agentsInfoInputFile* const agentsInfo)
{
    for (uint8_t index = 0; index < agentsInfo->numberOfCrashlogAgents; index++)
    {
        if (agentDetails->uniqueId == agentsInfo->agentMap[index].guid)
        {
            cd_snprintf_s(sectionNameString, length, "%s",
                          agentsInfo->agentMap[index].crashlogSectionName);
            return ACD_SUCCESS;
        }
    }

    cd_snprintf_s(sectionNameString, length, "%x", agentDetails->uniqueId);
    return ACD_SUCCESS;
}

acdStatus initiateCrashlogTriggerRearm(const CPUInfo* const cpuInfo)
{
    uint64_t watcherData = 0;
    uint8_t cc = 0;

    EPECIStatus ret = peci_Telemetry_ConfigWatcherRd(
        cpuInfo->clientAddr, CONFIG_WATCHER_ID, CONFIG_WATCHER_OFFSET,
        sizeof(watcherData), (uint8_t *)&watcherData, &cc);

    if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)))
    {
        CRASHDUMP_PRINT(
            ERR, stderr,
            "Error while reading the Config Watcher for Trigger Rearm \n");
        return ACD_FAILURE;
    }

    watcherData = (watcherData | REARM_TRIGGER_MASK);

   ret = peci_Telemetry_ConfigWatcherWr(
        cpuInfo->clientAddr, CONFIG_WATCHER_ID, CONFIG_WATCHER_OFFSET,
        sizeof(watcherData),(uint8_t *) &watcherData, &cc);
    if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)))
    {
        CRASHDUMP_PRINT(
            ERR, stderr,
            "Error while writing the Config Watcher for Trigger Rearm \n");
        return ACD_FAILURE;
    }

    return ACD_SUCCESS;
}

acdStatus storeCrashlog(cJSON* const pJsonChild, const uint16_t agent,
                        const crashlogAgentDetails* const agentDetails,
                        const uint64_t* const rawCrashlog,
                        const agentsInfoInputFile* const agentsInfo)
{
    char guidString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
    cd_snprintf_s(guidString, CRASHLOG_ERROR_JSON_STRING_LEN, "agent_id_0x%x",
                  agentDetails->uniqueId);

    cJSON* guidItem = NULL;
    if ((guidItem = cJSON_GetObjectItemCaseSensitive(pJsonChild, guidString)) ==
        NULL)
    {
        cJSON_AddItemToObject(pJsonChild, guidString,
                              guidItem = cJSON_CreateObject());
    }

    // Each of 3 data bytes translates to 4 encoded bytes, so multiplying by
    // [ceiling(1.33) = 2]

    char* jsonItemString =
        (char*)(malloc(agentDetails->crashSpace * sizeof(uint32_t) * 2));

    if (NULL == jsonItemString)
    {
        char jsonErrorString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
        cd_snprintf_s(jsonErrorString, CRASHLOG_ERROR_JSON_STRING_LEN,
                      "Memory allocation for storing output string failed for "
                      "agent 0x%x, uniqueId=0x%x",
                      agent, agentDetails->uniqueId);
        logError(guidItem, jsonErrorString);
        return ACD_ALLOCATE_FAILURE;
    }

    uint64_t error = base64Encode((uint8_t*)rawCrashlog,
                                  (agentDetails->crashSpace * sizeof(uint32_t)),
                                  jsonItemString);

    if (error)
    {
        char jsonErrorString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
        cd_snprintf_s(jsonErrorString, CRASHLOG_ERROR_JSON_STRING_LEN,
                      "Could not encode full crashlog (error=0x%llu) for "
                      "agent=0x%x, uniqueId=0x%x",
                      error, agent, agentDetails->uniqueId);
        logError(guidItem, jsonErrorString);
    }

    char sectionNameString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
    //-----AMI-start----
    acdStatus status = getAgentName(agentDetails, sectionNameString,
                                    CRASHLOG_ERROR_JSON_STRING_LEN, agentsInfo);
    //-----AMI-end-----
    char recordNameString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
    cd_snprintf_s(recordNameString, CRASHLOG_ERROR_JSON_STRING_LEN, "#data_%s",
                  sectionNameString);

    cJSON_AddStringToObject(guidItem, recordNameString, jsonItemString);

    FREE(jsonItemString);
    return ACD_SUCCESS;
}

void logError(cJSON* const pJsonChild, const char* errorString)
{
    char jsonErrorString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
    cd_snprintf_s(jsonErrorString, CRASHLOG_ERROR_JSON_STRING_LEN, "%s",
                  errorString);
    cJSON_AddStringToObject(pJsonChild, "_error", jsonErrorString);
}

void logInOutput(cJSON* const pJsonChild, const char* key, const char* value)
{
    char jsonString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
    cd_snprintf_s(jsonString, CRASHLOG_ERROR_JSON_STRING_LEN, "%s", value);
    cJSON_AddStringToObject(pJsonChild, key, jsonString);
}

acdStatus
    collectCrashlogForAgent(const CPUInfo* const cpuInfo, const uint16_t agent,
                            const crashlogAgentDetails* const agentDetails,
                            uint64_t* const rawCrashlog,
                            RunTimeInfo runTimeInfo)
{
    uint8_t cc;
    uint64_t data = 0;
    acdStatus error = ACD_SUCCESS;

    if (NULL == agentDetails || NULL == cpuInfo || NULL == rawCrashlog)
    {
        return ACD_INVALID_OBJECT;
    }

    // agentDetails->crashSpace is the size in DWORDs and each GetCrashlogSample
    // returns 2 DWORDs, so we only need half that number of samples
    for (uint16_t sampleId = 0; sampleId < (agentDetails->crashSpace / 2);
         sampleId++)
    {
        double globalRemainingTime = getTimeRemainingFromStart(
            runTimeInfo.maxGlobalTime, runTimeInfo.globalRunTime);
        double sectionRemainingTime = getTimeRemainingFromStart(
            runTimeInfo.maxSectionTime, runTimeInfo.sectionRunTime);
        if (globalRemainingTime <= 0)
        {

            CRASHDUMP_PRINT(
                ERR, stderr,
                "Global Timeout while collecting crashlog for sampleID=0x%x\n",
                sampleId);

            return ACD_GLOBAL_TIMEOUT;
        }
        if (sectionRemainingTime <= 0)
        {

            CRASHDUMP_PRINT(
                ERR, stderr,
                "Section Timeout while collecting crashlog for sampleID=0x%x\n",
                sampleId);

            return ACD_SECTION_TIMEOUT;
        }
        EPECIStatus ret = peci_Telemetry_GetCrashlogSample(
            cpuInfo->clientAddr, agent, sampleId, sizeof(uint64_t),
            (uint8_t*)&data, &cc);
        if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)))
        {
            CRASHDUMP_PRINT(
                ERR, stderr,
                "Error while collecting crashlog for sampleID=0x%x\n",
                sampleId);

            return ACD_FAILURE;
        }
        rawCrashlog[sampleId] = data;
    }

    return error;
}

bool isAgentExcluded(const CPUInfo* const cpuInfo,
                     const crashlogAgentDetails* const agentDetails)
{
    cJSON* excludeAgentList =
        getCrashlogExcludeList(cpuInfo->inputFile.bufferPtr);

    if (NULL != excludeAgentList)
    {
        cJSON* entry = NULL;
        cJSON_ArrayForEach(entry, excludeAgentList)
        {
            uint32_t guid = strtoul(entry->valuestring, NULL, 16);
            if (agentDetails->uniqueId == guid)
            {
                CRASHDUMP_PRINT(
                    ERR, stderr,
                    "Excluding guid=0x%x from querying for crashlog\n",
                    agentDetails->uniqueId);
                return true;
            }
        }
    }

    return false;
}

bool isCrashlogAgentEnabled(const crashlogAgentDetails* const agentDetails,
                            const agentsInfoInputFile* const agentsInfo)
{
    if (NULL == agentDetails)
    {
        return false;
    }

    for (uint8_t i = 0; i < agentsInfo->numberOfCrashlogAgents; i++)
    {
        if ((agentDetails->uniqueId == agentsInfo->agentMap[i].guid) &&
            agentsInfo->agentMap[i].enable)
        {
            return true;
        }
    }

    return false;
}

acdStatus isValidAgent(const CPUInfo* const cpuInfo,
                       const crashlogAgentDetails* const agentDetails,
                       const agentsInfoInputFile* const agentsInfo)
{
    for (uint8_t index = 0; index < agentsInfo->numberOfCrashlogAgents; index++)
    {
        if (isCrashlogAgentEnabled(agentDetails, agentsInfo) ||
            !isAgentExcluded(cpuInfo, agentDetails))
        {
            return ACD_SUCCESS;
        }
    }

    return ACD_FAILURE;
}

crashlogAgentDetails getCrashlogDetailsForAgent(const CPUInfo* const cpuInfo,
                                                const uint16_t agent)
{
    uint64_t crashlogDetails = 0;
    uint8_t cc;

    EPECIStatus ret = peci_Telemetry_Discovery(
        cpuInfo->clientAddr, 2, 0x4, agent, 0, sizeof(crashlogDetails),
        (uint8_t*)&crashlogDetails, &cc);

    if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)))
    {
        CRASHDUMP_PRINT(
            ERR, stderr,
            "Error getting crashlog size for agent %d, ret=%d, cc=0x%x\n",
            agent, ret, cc);
        crashlogAgentDetails agentDetails = {
            .entryType = 0, .crashType = 0, .uniqueId = 0, .crashSpace = 0};
        return agentDetails;
    }

    crashlogAgentDetails agentDetails = {
        .entryType = crashlogDetails & 0xFF,
        .crashType = (crashlogDetails >> 8) & 0xFF,
        .uniqueId = (crashlogDetails >> 16) & 0xFFFFFFFF,
        .crashSpace = (crashlogDetails >> 48) & 0xFFFF};

    return agentDetails;
}

uint16_t getNumberOfCrashlogAgents(const CPUInfo* const cpuInfo)
{
    uint16_t crashlogAgents = NO_CRASHLOG_AGENTS;
    uint8_t cc;

    EPECIStatus ret = peci_Telemetry_Discovery(cpuInfo->clientAddr, 1, 0x4, 0,
                                               0, sizeof(crashlogAgents),
                                               (uint8_t*)&crashlogAgents, &cc);

    if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)))
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "Error getting number of crashlog agents\n");

        return NO_CRASHLOG_AGENTS;
    }

    return crashlogAgents;
}

bool isTelemetrySupported(const CPUInfo* const cpuInfo)
{
    uint8_t telemetrySupported = TELEMETRY_UNSUPPORTED;
    uint8_t cc;
    EPECIStatus ret = peci_Telemetry_Discovery(cpuInfo->clientAddr, 0, 0, 0, 0,
                                               sizeof(telemetrySupported),
                                               &telemetrySupported, &cc);

    if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)) ||
        TELEMETRY_UNSUPPORTED == telemetrySupported)
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "Telemetry is not supported, Returned (%d) during "
                        "discovery (disabled:%d)\n",
                        ret, telemetrySupported);

        return TELEMETRY_UNSUPPORTED;
    }

    return telemetrySupported;
}

bool isCpuCrashlogEnabled(const int clientAddr)
{
    //-----AMI-start-----
    uint64_t data = 0;
    //-----AMI-end-----
    uint8_t cc = 0;
    EPECIStatus ret = peci_Telemetry_ConfigWatcherRd(
        clientAddr, CONFIG_WATCHER_ID, CONFIG_WATCHER_OFFSET, sizeof(data),
        (uint8_t *) &data, &cc);
    if (ret != PECI_CC_SUCCESS || (PECI_CC_UA(cc)))
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "Telemetry_ConfigWatcherRd for crashlog enabled failed!"
                        "(ret=%d, cc=0x%x)\n",
                        ret, cc);
        return true;
    }
    bool crashlogDisabled = (data >> 31) & 0x1;
    return !crashlogDisabled;
}

acdStatus initCrashlogAgent(cJSON* agents, agentsInfoInputFile* agentsInfo)
{
    agentsInfo->numberOfCrashlogAgents = cJSON_GetArraySize(agents);
    agentsInfo->agentMap = calloc(agentsInfo->numberOfCrashlogAgents,
                                  sizeof(guidCrashlogSectionMapping));

    if (NULL == agentsInfo->agentMap)
    {
        return ACD_ALLOCATE_FAILURE;
    }

    cJSON* agent = NULL;
    int agentListIndex = 0;
    cJSON_ArrayForEach(agent, agents)
    {
        cJSON* name = cJSON_GetObjectItemCaseSensitive(agent, "Name");
        cJSON* enable = cJSON_GetObjectItemCaseSensitive(agent, "Enabled");

        agentsInfo->agentMap[agentListIndex].guid =
            strtoull(agent->string, NULL, 16);

        if (name)
        {
            agentsInfo->agentMap[agentListIndex].crashlogSectionName =
                name->valuestring;
        }
        if (enable)
        {
            agentsInfo->agentMap[agentListIndex].enable =
                cJSON_IsTrue(enable) ? true : false;
        }
        agentListIndex++;
    }

    return ACD_SUCCESS;
}

/******************************************************************************
 *  logCrashlogEBG
 *
 ******************************************************************************/
acdStatus logCrashlogEBG(const CPUInfo* const cpuInfo, cJSON* const pJsonChild,
                         const uint16_t agentsNum, RunTimeInfo runTimeInfo)
{
    cJSON* agents =
        getCrashlogAgentsFromInputFile(cpuInfo->inputFile.bufferPtr);
    if (NULL == agents)
    {
        return ACD_FAILURE;
    }

    if (isCpuCrashlogEnabled(cpuInfo->clientAddr))
    {
        logInOutput(pJsonChild, "cpu_crashlog_enabled", "true");
    }
    else
    {
        logInOutput(pJsonChild, "cpu_crashlog_enabled", "false");
    }

    agentsInfoInputFile agentsInfo;
    acdStatus error = initCrashlogAgent(agents, &agentsInfo);
    bool isCrashlogExtracted = false;

    for (uint16_t agent = AgentPCodeCrash; agent < agentsNum; agent++)
    {
        crashlogAgentDetails agentDetails =
            getCrashlogDetailsForAgent(cpuInfo, agent);
        uint16_t crashlogSize = agentDetails.crashSpace;

        if (0 == agentDetails.crashSpace)
        {
            continue;
        }

        if (ACD_SUCCESS != isValidAgent(cpuInfo, &agentDetails, &agentsInfo))
        {
            continue;
        }

        uint64_t* rawCrashlog =
            (uint64_t*)(calloc(agentDetails.crashSpace, sizeof(uint32_t)));

        if (rawCrashlog == NULL)
        {
            CRASHDUMP_PRINT(
                ERR, stderr,
                "Couldn't allocate memory for crashlog of size=0x%x\n",
                agentDetails.crashSpace);
            continue;
        }

        error = collectCrashlogForAgent(cpuInfo, agent, &agentDetails,
                                        rawCrashlog, runTimeInfo);
        if (error == ACD_GLOBAL_TIMEOUT)
        {
            char jsonErrorString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
            cd_snprintf_s(jsonErrorString, CRASHLOG_ERROR_JSON_STRING_LEN,
                          "Crashlog_global_timeout: %d sec. in agent %d",
                          runTimeInfo.maxGlobalTime, agent);
            logError(pJsonChild, jsonErrorString);
            FREE(rawCrashlog);
            break;
        }
        else if (error == ACD_SECTION_TIMEOUT)
        {
            char jsonErrorString[CRASHLOG_ERROR_JSON_STRING_LEN] = {0};
            cd_snprintf_s(jsonErrorString, CRASHLOG_ERROR_JSON_STRING_LEN,
                          "Crashlog_section_timeout: %d sec. in agent %d",
                          runTimeInfo.maxSectionTime, agent);
            logError(pJsonChild, jsonErrorString);
            FREE(rawCrashlog);
            break;
        }
        else
        {
            isCrashlogExtracted = true;
            error = storeCrashlog(pJsonChild, agent, &agentDetails, rawCrashlog,
                                  &agentsInfo);
            if (error)
            {
                CRASHDUMP_PRINT(ERR, stderr,
                                "Error storing crashlog for agent 0x%x in "
                                "output file\n",
                                agent);
            }
        }

        FREE(rawCrashlog);
    }

    if (isCrashlogExtracted)
    {
        // Processor patching not ready at time of release. Untested.
        if (initiateCrashlogTriggerRearm(cpuInfo) != ACD_SUCCESS)
        {
            logInOutput(pJsonChild, "crashlog_trigger_rearm_fail", "true");
        }
    }
    FREE(agentsInfo.agentMap);
    return error;
}
//-----AMI-start-----
acdStatus logCrashlogSection(const CPUInfo* const cpuInfo,
                             cJSON* const outputNode,
                             uint16_t totalAgents,
                             RunTimeInfo runTimeInfo)
//-----AMI-end-----
{
    return logCrashlogEBG(cpuInfo, outputNode, totalAgents, runTimeInfo);
}
/*
static CrashlogVx crashlogVx[] = {
    {cd_spr, logCrashlogEBG},
    {cd_sprhbm, logCrashlogEBG},
};
*/