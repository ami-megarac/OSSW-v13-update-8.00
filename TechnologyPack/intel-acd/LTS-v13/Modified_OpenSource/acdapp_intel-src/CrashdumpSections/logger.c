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
//-----AMI-start-----
#define _GNU_SOURCE
//-----AMI-end-----
#include "logger.h"
#include <sys/errno.h>
#include <string.h>
acdStatus getPath(cJSON* section, LoggerStruct* loggerStruct)
{
    loggerStruct->nameProcessing.rootAtLevel = 255; // Default set to invalid
    if (section != NULL)
    {
        cJSON* RootAtLevelJson =
            cJSON_GetObjectItemCaseSensitive(section, "RootAtLevel");
        if (RootAtLevelJson != NULL)
        {
            loggerStruct->nameProcessing.rootAtLevel =
                RootAtLevelJson->valueint;
        }
        cJSON* OutputPath =
            cJSON_GetObjectItemCaseSensitive(section, "OutputPath");
        if (OutputPath == NULL)
        {
            return ACD_FAILURE;
        }
        // We need to save the OutputPath into a fix char array, because
        // strtok_s modifies the source, and we need to keep the input file
        // object intact, for the second cpu.
        memcpy(loggerStruct->pathParsing.pathString,
                  OutputPath->valuestring, strlen(OutputPath->valuestring) + 1);//AMI
        loggerStruct->pathParsing.pathStringToken =
            loggerStruct->pathParsing.pathString;
        return ACD_SUCCESS;
    }
    return ACD_FAILURE;
}

acdStatus ParsePath(LoggerStruct* loggerStruct)
{
    int j = 0;
    loggerStruct->pathParsing.numberOfTokens = 0;
    for (int i = 0; i < MAX_NUM_PATH_LEVELS; i++)
    {
        loggerStruct->pathParsing.pathLevelToken[i] = "\n";
    }
    size_t len = strnlen(loggerStruct->pathParsing.pathStringToken,
                           LOGGER_JSON_STRING_LEN);
    char* p2str;
    char* token =
        strtok(loggerStruct->pathParsing.pathStringToken, "/");
    while (token)
    {
        loggerStruct->pathParsing.pathLevelToken[j++] = token;
        token = strtok(NULL, "/");
    }
    loggerStruct->pathParsing.numberOfTokens = j;
    if (loggerStruct->pathParsing.numberOfTokens == 0)
    {
        CRASHDUMP_PRINT(ERR, stderr, "Number of Tokens on path are zero.\n");
        return ACD_FAILURE;
    }
    return ACD_SUCCESS;
}

acdStatus ParseNameSection(CmdInOut* cmdInOut, LoggerStruct* loggerStruct)
{
    cJSON* it = NULL;
    int position = 0;
    loggerStruct->nameProcessing.extraLevel = false;
    loggerStruct->nameProcessing.sizeFromOutput = false;
    loggerStruct->nameProcessing.zeroPaddedPrint = false;
    cJSON* nameJSON =
        cJSON_GetObjectItemCaseSensitive(cmdInOut->in.outputPath, "Name");
    if (nameJSON == NULL)
    {
        loggerStruct->nameProcessing.logRegister = false;
        // In case there is no Name section just exit and do not log the
        // register
        return ACD_SUCCESS;
    }
    cJSON* sizeJSON =
        cJSON_GetObjectItemCaseSensitive(cmdInOut->in.outputPath, "Size");
    if (sizeJSON != NULL)
    {
        loggerStruct->nameProcessing.size = sizeJSON->valueint;
        loggerStruct->nameProcessing.sizeFromOutput = true;
    }
    cJSON* zeroPaddedJSON =
        cJSON_GetObjectItemCaseSensitive(cmdInOut->in.outputPath, "ZeroPadded");
    if (zeroPaddedJSON != NULL)
    {
        loggerStruct->nameProcessing.zeroPaddedPrint = cJSON_IsTrue(zeroPaddedJSON);
    }

    cJSON_ArrayForEach(it, nameJSON)
    {
        switch (position)
        {
            case namePosition:
                loggerStruct->nameProcessing.registerName = it->valuestring;
                break;
            case sectionPosition:
                loggerStruct->nameProcessing.extraLevel = true;
                loggerStruct->nameProcessing.sectionName = it->valuestring;
                break;
            default:
                return ACD_FAILURE;
        }
        position++;
    }
    return ACD_SUCCESS;
}

acdStatus GenerateJsonPath(CmdInOut* cmdInOut, cJSON* root,
                           LoggerStruct* loggerStruct, bool useRootPath)
{
    char jsonItemString[LOGGER_JSON_STRING_LEN];
    cJSON* cjsonLevels[MAX_NUM_PATH_LEVELS];
    cJSON* lastLevelJSON = NULL;
    uint8_t itemsToProcess = 0;
    for (uint8_t i = 0; i < MAX_NUM_PATH_LEVELS; i++)
    {
        cjsonLevels[i] = NULL;
    }
    cjsonLevels[0] = root;

    if (!loggerStruct->nameProcessing.sizeFromOutput)
    {
        loggerStruct->nameProcessing.size = cmdInOut->out.size;
    }

    itemsToProcess = loggerStruct->pathParsing.numberOfTokens;
    if (useRootPath)
    {
        if (loggerStruct->nameProcessing.rootAtLevel <=
            loggerStruct->pathParsing.numberOfTokens)
        {
            itemsToProcess = loggerStruct->nameProcessing.rootAtLevel;
        }
    }
    for (int i = 0; i < itemsToProcess; i++)
    {
        cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, "%s",
                      loggerStruct->pathParsing.pathLevelToken[i]);
        int match = 1;
        match = memcmp(loggerStruct->pathParsing.pathLevelToken[i], LOGGER_CPU, strlen(LOGGER_CPU));//AMI
        if (match == 0)
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_CPU,
                          loggerStruct->contextLogger.cpu);
        }
        match = 1;
        match = memcmp(loggerStruct->pathParsing.pathLevelToken[i], LOGGER_CORE, strlen(LOGGER_CORE));//AMI
        if (match == 0)
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_CORE,
                          loggerStruct->contextLogger.core);
        }
        match = 1;
        match = memcmp(loggerStruct->pathParsing.pathLevelToken[i], LOGGER_THREAD, strlen(LOGGER_THREAD));//AMI
        if (match == 0)
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_THREAD,
                          loggerStruct->contextLogger.thread);
        }
        match = 1;
        match = memcmp(loggerStruct->pathParsing.pathLevelToken[i], LOGGER_CHA, strlen(LOGGER_CHA));//AMI
        if (match == 0)
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_CHA,
                          loggerStruct->contextLogger.cha);
        }
        match = 1;
        match =memcmp(loggerStruct->pathParsing.pathLevelToken[i], LOGGER_CBO, strlen(LOGGER_CBO));//AMI
        if (match == 0)
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_CBO,
                          loggerStruct->contextLogger.cha);
        }
        match = 1;
        match =memcmp(loggerStruct->pathParsing.pathLevelToken[i], LOGGER_CBO_UC, strlen(LOGGER_CBO_UC));//AMI
        if (match == 0)
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_CBO_UC,
                          loggerStruct->contextLogger.cha);
        }
        if ((cjsonLevels[i + 1] = cJSON_GetObjectItemCaseSensitive(
                 cjsonLevels[i], jsonItemString)) == NULL)
        {
            cJSON_AddItemToObject(cjsonLevels[i], jsonItemString,
                                  cjsonLevels[i + 1] = cJSON_CreateObject());
        }
    }
    loggerStruct->nameProcessing.jsonOutput = cjsonLevels[itemsToProcess];
    if (useRootPath)
    {
        loggerStruct->nameProcessing.extraLevel = false;
    }
    if (loggerStruct->nameProcessing.extraLevel)
    {
        cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, "%s",
                      loggerStruct->nameProcessing.sectionName);
        if ((lastLevelJSON = cJSON_GetObjectItemCaseSensitive(
                 cjsonLevels[itemsToProcess], jsonItemString)) == NULL)
        {
            cJSON_AddItemToObject(cjsonLevels[itemsToProcess], jsonItemString,
                                  lastLevelJSON = cJSON_CreateObject());
        }
        loggerStruct->nameProcessing.jsonOutput = lastLevelJSON;
    }
    if (loggerStruct->nameProcessing.jsonOutput == NULL)
    {
        return ACD_FAILURE;
    }
    return ACD_SUCCESS;
}

void GenerateRegisterName(char* registerName, LoggerStruct* loggerStruct)
{
    char* p2str;
//-------AMI start
    char *resultCha = NULL,
        *resultCbo = NULL ,
        *resultCpu = NULL,
        *resultCore = NULL,
        *resultThread = NULL,
        *resultSpecial = NULL;
        
    resultCha = memmem(loggerStruct->nameProcessing.registerName,
                                 LOGGER_JSON_STRING_LEN, LOGGER_CHA,
                                 strlen(LOGGER_CHA));
    resultCbo = memmem(loggerStruct->nameProcessing.registerName,
                                 LOGGER_JSON_STRING_LEN, LOGGER_CBO,
                                 strlen(LOGGER_CBO));
    resultCpu = memmem(loggerStruct->nameProcessing.registerName,
                                 LOGGER_JSON_STRING_LEN, LOGGER_CPU,
                                 strlen(LOGGER_CPU));
    resultCore = memmem(loggerStruct->nameProcessing.registerName,
                                  LOGGER_JSON_STRING_LEN, LOGGER_CORE,
                                  strlen(LOGGER_CORE));
    resultThread = memmem(loggerStruct->nameProcessing.registerName,
                                    LOGGER_JSON_STRING_LEN, LOGGER_THREAD,
                                    strlen(LOGGER_THREAD));
    resultSpecial = memmem(loggerStruct->nameProcessing.registerName,
                                     LOGGER_JSON_STRING_LEN, LOGGER_REPEAT,
                                     strlen(LOGGER_REPEAT));

    if (resultCha != NULL || resultCbo != NULL)
    {
        cd_snprintf_s(registerName, LOGGER_JSON_STRING_LEN,
                      loggerStruct->nameProcessing.registerName,
                      loggerStruct->contextLogger.cha);
    }
    else if (resultCore != NULL)
    {
        cd_snprintf_s(registerName, LOGGER_JSON_STRING_LEN,
                      loggerStruct->nameProcessing.registerName,
                      loggerStruct->contextLogger.core);
    }
    else if (resultCpu != NULL)
    {
        cd_snprintf_s(registerName, LOGGER_JSON_STRING_LEN,
                      loggerStruct->nameProcessing.registerName,
                      loggerStruct->contextLogger.cpu);
    }
    else if (resultThread != NULL)
    {
        cd_snprintf_s(registerName, LOGGER_JSON_STRING_LEN,
                      loggerStruct->nameProcessing.registerName,
                      loggerStruct->contextLogger.thread);
    }
    else if (resultSpecial != NULL)
    {
        cd_snprintf_s(registerName, LOGGER_JSON_STRING_LEN,
                      loggerStruct->nameProcessing.registerName,
                      loggerStruct->contextLogger.repeats);
    }
    else
    {
        memcpy(registerName,
                 loggerStruct->nameProcessing.registerName, 
                 strlen(loggerStruct->nameProcessing.registerName) + 1);//AMI
    }
//------AMI end---------
}

void LogValue(char* registerName, CmdInOut* cmdInOut,
              LoggerStruct* loggerStruct, cJSON* parent)
{
    char jsonItemString[LOGGER_JSON_STRING_LEN];
    char jsonNameString[LOGGER_JSON_STRING_LEN];
    if (cmdInOut->out.printString)
    {
        cJSON_AddStringToObject(parent, registerName, cmdInOut->out.stringVal);
        cmdInOut->out.printString = false;
        return;
    }
    switch (loggerStruct->nameProcessing.size)
    {
        case sizeof(uint8_t):
            cmdInOut->out.val.u64 &= 0xFF;
            break;
        case sizeof(uint16_t):
            cmdInOut->out.val.u64 &= 0xFFFF;
            break;
        case sizeof(uint32_t):
            cmdInOut->out.val.u64 &= 0xFFFFFFFF;
            break;
        default:
            break;
    }
    if (loggerStruct->contextLogger.skipFlag)
    {
        if (cmdInOut->runTimeInfo->maxGlobalRunTimeReached &&
            cmdInOut->runTimeInfo->globalMaxPrint)
        {
            cd_snprintf_s(jsonNameString, LOGGER_JSON_STRING_LEN,
                          LOGGER_GLOBAL_TIMEOUT,
                          loggerStruct->contextLogger.cpu,
                          loggerStruct->contextLogger.currentSectionName);
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN,
                          LOGGER_INFO_TIMEOUT,
                          cmdInOut->runTimeInfo->maxGlobalTime);
            cJSON_AddStringToObject(parent, jsonNameString, jsonItemString);
            cmdInOut->runTimeInfo->globalMaxPrint = false;
        }
        if (cmdInOut->runTimeInfo->maxSectionRunTimeReached &&
            cmdInOut->runTimeInfo->sectionMaxPrint)
        {
            cd_snprintf_s(jsonNameString, LOGGER_JSON_STRING_LEN,
                          LOGGER_SECTION_TIMEOUT,
                          loggerStruct->contextLogger.cpu,
                          loggerStruct->contextLogger.currentSectionName);
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN,
                          LOGGER_INFO_TIMEOUT,
                          cmdInOut->runTimeInfo->maxSectionTime);
            cJSON_AddStringToObject(parent, jsonNameString, jsonItemString);
            cmdInOut->runTimeInfo->sectionMaxPrint = false;
        }
        cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, LOGGER_NA);
        cJSON_AddStringToObject(parent, registerName, jsonItemString);
    }
    else if (cmdInOut->out.ret != PECI_CC_SUCCESS)
    {
        cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN,
                      LOGGER_FIXED_DATA_CC_RC, cmdInOut->out.cc,
                      cmdInOut->out.ret);
        cJSON_AddStringToObject(parent, registerName, jsonItemString);
    }
    else if (PECI_CC_UA(cmdInOut->out.cc))
    {
        cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN,
                      LOGGER_DATA_64_bits_CC_RC, cmdInOut->out.val.u64,
                      cmdInOut->out.cc, cmdInOut->out.ret);
        cJSON_AddStringToObject(parent, registerName, jsonItemString);
    }
    else
    {
        if (loggerStruct->nameProcessing.zeroPaddedPrint)
        {
            char* formatStr;
            switch (loggerStruct->nameProcessing.size)
            {
                case 1:
                    formatStr = "0x%02x";
                    break;
                case 2:
                    formatStr = "0x%04x";
                    break;
                case 4:
                    formatStr = "0x%08x";
                    break;
                case 8:
                    formatStr = "0x%016llx";
                    break;
                default:
                    formatStr = "0x%016llx";
                    break;
            }
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN, formatStr,
                          cmdInOut->out.val.u64);
        }
        else
        {
            cd_snprintf_s(jsonItemString, LOGGER_JSON_STRING_LEN,
                          LOGGER_DATA_64bits, cmdInOut->out.val.u64);
        }

        cJSON_AddStringToObject(parent, registerName, jsonItemString);
    }
}

void Logger(CmdInOut* cmdInOut, cJSON* root, LoggerStruct* loggerStruct)
{
    char registerName[64];
    if (GenerateJsonPath(cmdInOut, root, loggerStruct, false) == ACD_SUCCESS)
    {
        //-----AMI-start-----
        GenerateRegisterName(registerName, loggerStruct);
        LogValue(registerName, cmdInOut, loggerStruct,
                 loggerStruct->nameProcessing.jsonOutput);
        //-----AMI-end-----
    }
    else
    {
        CRASHDUMP_PRINT(ERR, stderr, "Could not log Value, path is Null.\n");
    }
}

void GenerateVersion(cJSON* Section, int* Version)
{
    int productType = 0;
    int recordType = 0;
    int revisionNum = 0;
    productType = cJSONToInt(cJSON_GetObjectItemCaseSensitive(
                                 Section, "ProductType"), 16);
    recordType = cJSONToInt(cJSON_GetObjectItemCaseSensitive(
                                Section, "RecordType"), 16);
    revisionNum = cJSONToInt(cJSON_GetObjectItemCaseSensitive(
                                 Section, "Revision"), 16);
    if (productType == 0 && recordType == 0 && revisionNum == 0)
    {
        *Version = 0;
    }
    else
    {
        *Version = (recordType << RECORD_TYPE_OFFSET) |
                   (productType << PRODUCT_TYPE_OFFSET) |
                   (revisionNum << REVISION_OFFSET);
    }
}

void logVersion(CmdInOut* cmdInOut, cJSON* root, LoggerStruct* loggerStruct)
{
    char* VersionStr = LOGGER_VERSION_STRING;
    if (loggerStruct->contextLogger.version != 0)
    {
        loggerStruct->nameProcessing.extraLevel = false;
        if (GenerateJsonPath(cmdInOut, root, loggerStruct, true) == ACD_SUCCESS)
        {
            loggerStruct->nameProcessing.zeroPaddedPrint = false;
            loggerStruct->contextLogger.skipFlag = false;
            cmdInOut->out.cc = PECI_DEV_CC_SUCCESS;
            cmdInOut->out.ret = PECI_CC_SUCCESS;
            cmdInOut->out.val.u64 = loggerStruct->contextLogger.version;
            loggerStruct->nameProcessing.size = 4;
            cmdInOut->out.printString = false;
            cJSON* previousVersion = cJSON_GetObjectItemCaseSensitive(
                loggerStruct->nameProcessing.jsonOutput, "_version");
            if (previousVersion == NULL)
            {
                LogValue(VersionStr, cmdInOut, loggerStruct,
                         loggerStruct->nameProcessing.jsonOutput);
            }
        }
        else
        {
            CRASHDUMP_PRINT(ERR, stderr,
                            "Could not log Version, path is Null.\n");
        }
    }
}

void logRecordDisabled(CmdInOut* cmdInOut, cJSON* root,
                       LoggerStruct* loggerStruct)
{
    loggerStruct->nameProcessing.extraLevel = false;
    if (GenerateJsonPath(cmdInOut, root, loggerStruct, true) == ACD_SUCCESS)
    {
        cJSON* previousVersion = cJSON_GetObjectItemCaseSensitive(
            loggerStruct->nameProcessing.jsonOutput, RECORD_ENABLE);
        if (previousVersion == NULL)
        {
            cJSON_AddBoolToObject(loggerStruct->nameProcessing.jsonOutput,
                                  RECORD_ENABLE, false);
        }
    }
    else
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "Could not log Record Disable, path is Null.\n");
    }
}

void logSectionRunTime(cJSON* parent, struct timespec* start, char* key)
{
    char timeString[64];
    struct timespec finish = {};
    uint64_t timeVal = 0;
    if (parent == NULL)
    {
        CRASHDUMP_PRINT(ERR, stderr, "Could not log time, parent is Null.\n");
        return;
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    uint64_t runTimeInNs = tsToNanosecond(&finish) - tsToNanosecond(start);

    timeVal = runTimeInNs;
    // Look for pevious time
    cJSON* previousTimeKey = cJSON_GetObjectItemCaseSensitive(parent, key);
    if (previousTimeKey != NULL)
    {
        //-------AMI start------
        double previousTime = 0;
        if(previousTimeKey->valuestring != NULL)
            previousTime = atof(previousTimeKey->valuestring);
        //-------AMI end--------
        double totalTime = (double)runTimeInNs / 1e9 + previousTime;

        cd_snprintf_s(timeString, sizeof(timeString), "%.2fs", totalTime);
        cJSON_DeleteItemFromObjectCaseSensitive(parent, key);
        cJSON_AddStringToObject(parent, key, timeString);
        clock_gettime(CLOCK_MONOTONIC, start);
        return;
    }

    cd_snprintf_s(timeString, sizeof(timeString), "%.2fs",
                  (double)timeVal / 1e9);
    cJSON_AddStringToObject(parent, key, timeString);

    clock_gettime(CLOCK_MONOTONIC, start);
}
