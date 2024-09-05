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

#include "utils_triage.hpp"
#ifndef SPX_BMC_ACD
#include "bafi/triage.hpp"
#endif
extern "C" {
#ifndef SPX_BMC_ACD
#include <cjson/cJSON.h>
#else
#include "cJSON.h"
#endif
#include "CrashdumpSections/crashdump.h"
#include "CrashdumpSections/utils.h"
}
#ifdef TRIAGE_SECTION
void appendTriageSection(std::string& storedLogContents)
{
    char* triageInfo = NULL;
    size_t triageInfoSize = 0;

    CRASHDUMP_PRINT(INFO, stderr, "Logging triage section\n");
    uint32_t status = getTriageInformation((char*)storedLogContents.c_str(),
                                           storedLogContents.length() + 1,
                                           triageInfo, &triageInfoSize, NULL);
    cJSON* content = cJSON_Parse(storedLogContents.c_str());
    if (status == 0)
    {
        cJSON* triage =
            cJSON_GetObjectItem(cJSON_Parse(triageInfo), TRIAGE_KEY);
        if (triage == NULL)
        {
            cJSON_AddStringToObject(content, TRIAGE_KEY, TRIAGE_ERR);
            CRASHDUMP_PRINT(ERR, stderr, "Triage info not found!\n");
        }
        else
        {
            cJSON_AddItemToObject(content, TRIAGE_KEY, triage);
        }
    }
    else
    {
        char jsonStr[JSON_STR_LEN];
        cd_snprintf_s(jsonStr, JSON_STR_LEN, BAFI_RC, status);
        cJSON_AddStringToObject(content, TRIAGE_KEY, jsonStr);
        CRASHDUMP_PRINT(ERR, stderr, "Get triage info failed! (%d)\n", status);
    }

    char* out;
#ifdef CONFIG_SPX_FEATURE_CRASHDUMP_PRINT_UNFORMATTED
    out = cJSON_PrintUnformatted(content);
    storedLogContents.assign(out);
#else
    out = cJSON_Print(content);
    storedLogContents.assign(out);
#endif
    cJSON_free(out);
    cJSON_Delete(content);
    if (triageInfo != NULL)
    {
        free(triageInfo);
    }
}
#endif
#ifdef CONFIG_SPX_FEATURE_ACD_SUMMARY
void appendSummarySection(std::string& storedLogContents)
{
    char* summaryInfo = NULL;
    size_t summaryInfoSize = 0;

    CRASHDUMP_PRINT(INFO, stderr, "Logging summary section\n");
    uint32_t status = getFullOutput((char*)storedLogContents.c_str(),
                                    storedLogContents.length() + 1, summaryInfo,
                                    &summaryInfoSize, NULL);

    cJSON* content = cJSON_Parse(storedLogContents.c_str());
    if (status == 0)
    {
        cJSON* summary =
            cJSON_GetObjectItem(cJSON_Parse(summaryInfo), SUMMARY_KEY);
        if (summary == NULL)
        {
            cJSON_AddStringToObject(content, SUMMARY_KEY, SUMMARY_ERR);
            CRASHDUMP_PRINT(ERR, stderr, "Summary info not found!\n");
        }
        else
        {
            cJSON_AddItemToObject(content, SUMMARY_KEY, summary);
        }
    }
    else
    {
        char jsonStr[JSON_STR_LEN];
        cd_snprintf_s(jsonStr, JSON_STR_LEN, BAFI_RC, status);
        cJSON_AddStringToObject(content, SUMMARY_KEY, jsonStr);
        CRASHDUMP_PRINT(ERR, stderr, "Get summary info failed! (%d)\n", status);
    }
    char* out;
#ifdef CRASHDUMP_PRINT_UNFORMATTED
    out = cJSON_PrintUnformatted(content);
    storedLogContents.assign(out);
#else
    out = cJSON_Print(content);
    storedLogContents.assign(out);
#endif
    cJSON_free(out);
    cJSON_Delete(content);
    if (summaryInfo != NULL)
    {
        free(summaryInfo);
    }
}
#endif
