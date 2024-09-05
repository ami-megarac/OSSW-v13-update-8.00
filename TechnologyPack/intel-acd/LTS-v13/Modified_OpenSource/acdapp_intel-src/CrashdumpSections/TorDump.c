/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2019 Intel Corporation.
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

#include "TorDump.h"

#include "utils.h"

/******************************************************************************
 *
 *   torDumpJsonICX1
 *
 *   This function formats the TOR dump into a JSON object
 *
 ******************************************************************************/
static void torDumpJsonICXSPR(uint32_t u32Cha, uint32_t u32TorIndex,
                              uint32_t u32TorSubIndex, uint32_t u32PayloadBytes,
                              uint8_t* pu8TorCrashdumpData, cJSON* pJsonChild,
                              bool bInvalid, uint8_t cc, int ret, bool skipCha)
{
    cJSON* channel;
    cJSON* tor;
    char jsonItemName[TD_JSON_STRING_LEN];
    char jsonItemString[TD_JSON_STRING_LEN];
    char jsonErrorString[TD_JSON_STRING_LEN];

    // Add the channel number item to the TOR dump JSON structure only if it
    // doesn't already exist
    cd_snprintf_s(jsonItemName, TD_JSON_STRING_LEN, TD_JSON_CHA_NAME, u32Cha);
    if ((channel = cJSON_GetObjectItemCaseSensitive(pJsonChild,
                                                    jsonItemName)) == NULL)
    {
        cJSON_AddItemToObject(pJsonChild, jsonItemName,
                              channel = cJSON_CreateObject());
    }

    // Add the TOR Index item to the TOR dump JSON structure only if it
    // doesn't already exist
    cd_snprintf_s(jsonItemName, TD_JSON_STRING_LEN, TD_JSON_TOR_NAME,
                  u32TorIndex);
    if ((tor = cJSON_GetObjectItemCaseSensitive(channel, jsonItemName)) == NULL)
    {
        cJSON_AddItemToObject(channel, jsonItemName,
                              tor = cJSON_CreateObject());
    }
    // Add the SubIndex data to the TOR dump JSON structure
    cd_snprintf_s(jsonItemName, TD_JSON_STRING_LEN, TD_JSON_SUBINDEX_NAME,
                  u32TorSubIndex);
    if (skipCha)
    {
        cd_snprintf_s(jsonItemString, TD_JSON_STRING_LEN, TD_NA);
        cJSON_AddStringToObject(tor, jsonItemName, jsonItemString);
        return;
    }
    if (bInvalid)
    {
        cd_snprintf_s(jsonItemString, TD_JSON_STRING_LEN, TD_FIXED_DATA_CC_RC,
                      cc, ret);
        cJSON_AddStringToObject(tor, jsonItemName, jsonItemString);
        return;
    }
    else
    {
        cd_snprintf_s(jsonItemString, sizeof(jsonItemString), "0x0");
        bool leading = true;
        char* ptr = &jsonItemString[2];

        for (int i = u32PayloadBytes - 1; i >= 0; i--)
        {
            // exclude any leading zeros per schema
            if (leading && pu8TorCrashdumpData[i] == 0)
            {
                continue;
            }
            leading = false;

            ptr += cd_snprintf_s(ptr, u32PayloadBytes, "%02x",
                                 pu8TorCrashdumpData[i]);
        }
        if (PECI_CC_UA(cc))
        {
            cd_snprintf_s(jsonErrorString, TD_JSON_STRING_LEN, TD_DATA_CC_RC,
                          cc, ret);
            /*Reason for False Positive - the copy length is fixed and equal to the array length. no change to happen this issue */
            /*coverity[overrun-buffer-arg : FALSE]*/
            strcat(jsonItemString, jsonErrorString);
            cJSON_AddStringToObject(tor, jsonItemName, jsonItemString);
            return;
        }
    }
    cJSON_AddStringToObject(tor, jsonItemName, jsonItemString);
}

int logTorSectionICX1(CPUInfo* cpuInfo, cJSON* pJsonChild,
                      uint8_t u8PayloadBytes)
{
    (void)cpuInfo;
    (void)pJsonChild;
    (void)u8PayloadBytes;
    // Not supported in A0
    return ACD_SUCCESS;
}

int logTorSectionICXSPR(CPUInfo* cpuInfo, cJSON* outputNode,
                        uint8_t u8PayloadBytes)
{
    int ret = 0;
    uint8_t cc = 0;

    bool skipCha = false;
    bool skipFromInputFile = getSkipFromNewInputFile(cpuInfo, "TOR");

    // Crashdump Get Frames
    for (size_t cha = 0; cha < cpuInfo->chaCount; cha++)
    {
        for (uint32_t u32TorIndex = 0; u32TorIndex < TD_TORS_PER_CHA_ICX1;
             u32TorIndex++)
        {
            for (uint32_t u32TorSubIndex = 0;
                 u32TorSubIndex < TD_SUBINDEX_PER_TOR_ICX1; u32TorSubIndex++)
            {
                uint8_t* pu8TorCrashdumpData =
                    (uint8_t*)(calloc(u8PayloadBytes, sizeof(uint8_t)));
                bool bInvalid = false;
                if (pu8TorCrashdumpData == NULL)
                {
                    CRASHDUMP_PRINT(ERR, stderr,
                                    "Error allocating memory (size:%u)\n",
                                    u8PayloadBytes);
                    return ACD_ALLOCATE_FAILURE;
                }
                if (!skipCha)
                {
                    ret = peci_CrashDump_GetFrame(
                        cpuInfo->clientAddr, PECI_CRASHDUMP_TOR, cha,
                        (u32TorIndex | (u32TorSubIndex << 8)), u8PayloadBytes,
                        pu8TorCrashdumpData, &cc);

                    if (ret != PECI_CC_SUCCESS)
                    {
                        bInvalid = true;
                        CRASHDUMP_PRINT(ERR, stderr,
                                        "Error (%d) during GetFrame"
                                        "(cha:%d index:%d sub-index:%d)\n",
                                        ret, (int)cha, u32TorIndex,
                                        u32TorSubIndex);
                    }
                }
                torDumpJsonICXSPR(cha, u32TorIndex, u32TorSubIndex,
                                  u8PayloadBytes, pu8TorCrashdumpData,
                                  outputNode, bInvalid, cc, ret, skipCha);
                free(pu8TorCrashdumpData);
                if (PECI_CC_UA(cc) && !skipCha)
                {
                    if (skipFromInputFile)
                    {
                        skipCha = true;
                    }
                }
            }
        }
        skipCha = false;
    }
    return ACD_SUCCESS;
}

static const STorSectionVx sTorSectionVx[] = {
    {cd_icx, logTorSectionICX1},      {cd_icx2, logTorSectionICXSPR},
    {cd_icxd, logTorSectionICXSPR},   {cd_spr, logTorSectionICXSPR},
    {cd_sprhbm, logTorSectionICXSPR}, {cd_emr, logTorSectionICXSPR},
};

int logTorSection(CPUInfo* cpuInfo, cJSON* outputNode, uint8_t u8PayloadBytes)
{
    if (outputNode == NULL)
    {
        return ACD_INVALID_OBJECT;
    }

    for (uint32_t i = 0; i < (sizeof(sTorSectionVx) / sizeof(STorSectionVx));
         i++)
    {
        if (cpuInfo->model == sTorSectionVx[i].cpuModel)
        {
            return sTorSectionVx[i].logTorSectionVx(cpuInfo, outputNode,
                                                    u8PayloadBytes);
        }
    }

    CRASHDUMP_PRINT(ERR, stderr, "Cannot find version for %s\n", __FUNCTION__);
    return ACD_FAILURE;
}
