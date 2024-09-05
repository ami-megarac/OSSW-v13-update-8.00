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

#include "inputparser.h"

#include "validator.h"

char* decodeState(int value)
{
    switch (value)
    {
        case STARTUP:
            return MD_STARTUP;
            break;
        case EVENT:
            return MD_EVENT;
            break;
        case OVERWRITTEN:
            return MD_OVERWRITTEN;
            break;
        case INVALID:
            return MD_INVALID;
            break;
        default:
            return MD_INVALID;
            break;
    }
}

void ReadLoops(cJSON* section, LoopOnFlags* loopOnFlags)
{
    loopOnFlags->loopOnCPU = false;
    loopOnFlags->loopOnCHA = false;
    loopOnFlags->loopOnCore = false;
    loopOnFlags->loopOnThread = false;
    cJSON* LoopOn = cJSON_GetObjectItemCaseSensitive(section, "LoopOnCPU");
    if (LoopOn != NULL)
    {
        loopOnFlags->loopOnCPU = true;
    }
    LoopOn = cJSON_GetObjectItemCaseSensitive(section, "LoopOnCHA");
    if (LoopOn != NULL)
    {
        loopOnFlags->loopOnCHA = true;
    }
    LoopOn = cJSON_GetObjectItemCaseSensitive(section, "LoopOnCore");
    if (LoopOn != NULL)
    {
        loopOnFlags->loopOnCore = true;
    }
    LoopOn = cJSON_GetObjectItemCaseSensitive(section, "LoopOnThread");
    if (LoopOn != NULL)
    {
        loopOnFlags->loopOnThread = true;
    }
}

acdStatus ResetParams(cJSON* params, cJSON* paramsTracker)
{
    cJSON* it = NULL;
    cJSON_ArrayForEach(it, paramsTracker)
    {
        cJSON_ReplaceItemInArray(params, it->valueint,
                                 cJSON_CreateString(it->string));
    }
    return ACD_SUCCESS;
}

acdStatus UpdateParams(CPUInfo* cpuInfo, CmdInOut* cmdInOut,
                       LoggerStruct* loggerStruct, InputParserErrInfo* errInfo)
{
    for (int pos = 0; pos < cJSON_GetArraySize(cmdInOut->in.params); pos++)
    {
        cJSON* param = cJSON_GetArrayItem(cmdInOut->in.params, pos);
        if (cJSON_IsString(param))
        {
            cJSON* internalVar = cJSON_GetObjectItemCaseSensitive(
                cmdInOut->internalVarsTracker, param->valuestring);
            if (internalVar != NULL)
            {
                cJSON_AddItemToObject(cmdInOut->paramsTracker,
                                      param->valuestring,
                                      cJSON_CreateNumber(pos));
                cJSON_ReplaceItemInArray(
                    cmdInOut->in.params, pos,
                    cJSON_CreateNumber(
                        (double)strtoull(internalVar->valuestring, NULL, 16)));
            }
            else
            {
                int mismatchtarget = 1;
                int mismatchcha = 1;
                int mismatchcorethread = 1;
                int mismatchcore = 1;
		//---------AMI start------
                mismatchtarget = memcmp(param->valuestring,
                         TARGET, strlen(TARGET));//AMI
                mismatchcha = memcmp(param->valuestring, CHA, strlen(CHA));
                mismatchcorethread = memcmp(param->valuestring,
                         CORETHREAD, strlen(CORETHREAD));//AMI
                mismatchcore = memcmp(param->valuestring, CORE, strlen(CORE));//AMI
		//---------AMI end--------                
		if (mismatchtarget == 0)
                {
                    cJSON_AddItemToObject(cmdInOut->paramsTracker, TARGET,
                                          cJSON_CreateNumber(pos));
                    cJSON_ReplaceItemInArray(
                        cmdInOut->in.params, pos,
                        cJSON_CreateNumber(cpuInfo->clientAddr));
                }
                else if (mismatchcha == 0)
                {
                    cJSON_AddItemToObject(cmdInOut->paramsTracker, CHA,
                                          cJSON_CreateNumber(pos));
                    cJSON_ReplaceItemInArray(
                        cmdInOut->in.params, pos,
                        cJSON_CreateNumber(loggerStruct->contextLogger.cha));
                }
                else if (mismatchcorethread == 0)
                {
                    cJSON_AddItemToObject(cmdInOut->paramsTracker, CORETHREAD,
                                          cJSON_CreateNumber(pos));
                    cJSON_ReplaceItemInArray(
                        cmdInOut->in.params, pos,
                        cJSON_CreateNumber(
                            ((loggerStruct->contextLogger.core * 2) +
                             loggerStruct->contextLogger.thread)));
                }
                else if (mismatchcore == 0)
                {
                    cJSON_AddItemToObject(cmdInOut->paramsTracker, CORE,
                                          cJSON_CreateNumber(pos));
                    cJSON_ReplaceItemInArray(
                        cmdInOut->in.params, pos,
                        cJSON_CreateNumber(loggerStruct->contextLogger.core));
                }
                else if (IsValidHexString(param->valuestring))
                {
                    cJSON_ReplaceItemInArray(
                        cmdInOut->in.params, pos,
                        cJSON_CreateNumber(
                            (double)strtoull(param->valuestring, NULL, 16)));
                }
                else
                {
                    cmdInOut->out.stringVal = param->valuestring;
                }
            }
        }
    }
    return ACD_SUCCESS;
}
