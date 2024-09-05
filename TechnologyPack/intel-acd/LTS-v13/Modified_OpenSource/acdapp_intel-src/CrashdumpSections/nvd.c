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

#include "nvd.h"

#include "device_mgmt.h"
#include "fis.h"
#include "smbus_peci.h"
#include "utils.h"

static void logSmartHealthInfoAndStatus(uint32_t status, const void* payload,
                                        size_t payloadSize, cJSON* jsonChild)
{
    char jsonItemString[NVD_JSON_STRING_LEN];
    const unsigned char* byte = payload;
    char buf[MAX_PAYLOAD_STR];

    // Add status object
    if (status == RETURN_SUCCESS)
    {
        cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT,
                      NVD_MB_SUCCESS);
    }
    else
    {
        cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT,
                      NVD_MB_FAIL);
    }
    cJSON_AddStringToObject(jsonChild, "status", jsonItemString);

    // Add payload object
    strcpy_s(buf, sizeof("0x"), "0x");
    for (int i = (int)payloadSize; i >= 0; i--)
    {
        cd_snprintf_s(jsonItemString, sizeof(jsonItemString), "%02x", byte[i]);
        strcat_s(buf, sizeof(buf), jsonItemString);
    }
    cJSON_AddStringToObject(jsonChild, "payload", buf);
}

static void logErrLogStatusAndEntries(uint32_t status, char* logName,
                                      uint8_t logType, error_log_info* errorLog,
                                      uint16_t numEntries, uint16_t seqNum,
                                      cJSON* jsonChild)
{
    char jsonItemString[NVD_JSON_STRING_LEN];
    cJSON* jsonLogType = cJSON_CreateObject();

    // Add status object
    if (status == RETURN_SUCCESS)
    {
        cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT,
                      NVD_MB_SUCCESS);
    }
    else
    {
        cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT,
                      NVD_MB_FAIL);
    }
    cJSON_AddStringToObject(jsonLogType, "status", jsonItemString);

    // Determine entry size
    uint8_t entrySize;
    switch (logType)
    {
        case error_log_type_media:
            entrySize = PMEM_MEDIA_LOG_ENTRY_SIZE;
            break;
        case error_log_type_thermal:
        default:
            entrySize = PMEM_THERMAL_LOG_ENTRY_SIZE;
            break;
    }

    // Build payloads
    char buf[MAX_PAYLOAD_STR];
    for (int k = 0; k < numEntries; k++)
    {
        strcpy_s(buf, sizeof("0x"), "0x");

        // Build single entry to buf, output_data start from the 9th byte of
        // the error log entry
        for (int i = entrySize - 9; i >= 0; i--)
        {
            cd_snprintf_s(jsonItemString, sizeof(jsonItemString), "%02x",
                          errorLog[k].output_data[i]);
            strcat_s(buf, sizeof(buf), jsonItemString);
        }

        // Add timestamp to buf
        cd_snprintf_s(jsonItemString, sizeof(jsonItemString), "%016" PRIx64 "",
                      errorLog[k].system_timestamp);
        strcat_s(buf, sizeof(buf), jsonItemString);

        // Add buf to payload# object
        char strPayload[NVD_JSON_STRING_LEN];
        cd_snprintf_s(strPayload, sizeof(strPayload), "payload%d", k);
        cJSON_AddStringToObject(jsonLogType, strPayload, buf);
    }

    // Add sequence number
    char strSeq[NVD_JSON_STRING_LEN];
    cd_snprintf_s(strSeq, sizeof(strSeq), "seq%d", seqNum);
    cJSON* jsonSeq = cJSON_CreateObject();
    cJSON_AddItemToObject(jsonSeq, strSeq, jsonLogType);

    // Add log name
    cJSON_AddItemToObject(jsonChild, logName, jsonSeq);
}

static acdStatus readErrLogUsingInputFile(cJSON** logList,
                                          const CPUInfo* const cpuInfo,
                                          const uint8_t dimm, cJSON* jsonChild,
                                          bool* const enable)
{
    *logList = getPMEMSectionErrLogList(cpuInfo->inputFile.bufferPtr,
                                        "error_log", enable);
    if (*logList == NULL)
    {
        if (cJSON_GetObjectItemCaseSensitive(jsonChild,
                                             PMEM_FILE_ERR_LOG_KEY) == NULL)
        {
            cJSON_AddStringToObject(jsonChild, PMEM_FILE_ERR_LOG_KEY,
                                    PMEM_FILE_ERR_LOG_ERR);
        }
        return ACD_INVALID_OBJECT;
    }

    if (*enable == false)
    {
        if (cJSON_GetObjectItemCaseSensitive(jsonChild, RECORD_ENABLE) == NULL)
        {
            cJSON_AddFalseToObject(jsonChild, RECORD_ENABLE);
        }
        return ACD_SECTION_DISABLE;
    }

    cJSON* itRegs = NULL;
    cJSON* itParams = NULL;
    inputErrLog errLog = {0};

    cJSON_ArrayForEach(itRegs, *logList)
    {
        int position = 0;
        cJSON_ArrayForEach(itParams, itRegs)
        {
            switch (position)
            {
                case PMEM_ERRLOG_NAME:
                    errLog.name = itParams->valuestring;
                    break;
                case PMEM_ERRLOG_TYPE:
                    errLog.type = (uint8_t)itParams->valueint;
                    break;
                case PMEM_ERRLOG_LEVEL:
                    errLog.level = (uint8_t)itParams->valueint;
                    break;
                case PMEM_ERRLOG_MAX_SEQ_NUM:
                    errLog.maxSeqNum = (uint16_t)itParams->valueint;
                    break;
                default:
                    break;
            }
            position++;
        }

        RETURN_STATUS returnCode;
        uint32_t numEntries;
        error_log_info errLogs[255] = {0};
        device_info_t device = {0};
        device.device_addr.peci_address.peci_addr = cpuInfo->clientAddr;
        device.device_addr.peci_address.slot = dimm;
        device.device_addr.interface.iface = interface_peci;

        for (int i = 0; i < errLog.maxSeqNum; i++)
        {
            returnCode = get_fw_error_logs(&device, errLog.level, errLog.type,
                                           i, &errLogs[0], &numEntries);
            logErrLogStatusAndEntries(returnCode, errLog.name, errLog.type,
                                      &errLogs[0], numEntries, i, jsonChild);
        }
    }

    return ACD_SUCCESS;
}

acdStatus logErrLogSection(const CPUInfo* const cpuInfo, const uint8_t dimm,
                           cJSON* jsonChild)
{
    cJSON* regList = NULL;
    cJSON* errLogSection = NULL;
    bool enable = true;

    CRASHDUMP_PRINT(INFO, stderr, "Logging %s on PECI address %d\n",
                    "NVD Error Log", cpuInfo->clientAddr);

    cJSON_AddItemToObject(jsonChild, "error_log",
                          errLogSection = cJSON_CreateObject());
    return readErrLogUsingInputFile(&regList, cpuInfo, dimm, errLogSection,
                                    &enable);
}

static void logStatusAndPayload(uint32_t status, const void* payload,
                                size_t payloadSize, cJSON* jsonChild)
{
    char jsonItemString[NVD_JSON_STRING_LEN];
    const unsigned char* byte;
    char buf[MAX_PAYLOAD_STR];

    // Add status object
    if (status == RETURN_SUCCESS)
    {
        cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT,
                      NVD_MB_SUCCESS);
    }
    else
    {
        cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, NVD_UINT32_FMT,
                      NVD_MB_FAIL);
    }
    cJSON_AddStringToObject(jsonChild, "status", jsonItemString);

    // Add payload object
    strcpy_s(buf, sizeof("0x"), "0x");
    for (byte = payload; payloadSize--; --byte)
    {
        cd_snprintf_s(jsonItemString, sizeof(jsonItemString), "%02x", *byte);
        strcat_s(buf, sizeof(buf), jsonItemString);
    }
    cJSON_AddStringToObject(jsonChild, "payload", buf);
}

static acdStatus logIdentifyDimm(const CPUInfo* const cpuInfo,
                                 const uint8_t dimm, cJSON* jsonChild)
{
    device_addr_t device = {0};
    fis_id_device_payload_t payload = {0};
    RETURN_STATUS fisRet;

    device.peci_address.peci_addr = cpuInfo->clientAddr;
    device.interface.iface = interface_peci;
    device.peci_address.slot = dimm;

    fisRet = fw_cmd_device_id(&device, &payload);
    if (fisRet == RETURN_SUCCESS)
    {
        cJSON* section = NULL;
        cJSON_AddItemToObject(jsonChild, "identify_dimm",
                              section = cJSON_CreateObject());

        // payload json output start from last byte first
        unsigned char* ptr = (unsigned char*)&payload.reserved4[48];
        logStatusAndPayload(fisRet, ptr, sizeof(payload), section);
    }

    return ACD_SUCCESS;
}

static acdStatus readCSRUsingInputFile(cJSON** regList,
                                       const CPUInfo* const cpuInfo,
                                       const uint8_t dimm, cJSON* jsonChild,
                                       bool* const enable)
{
    *regList =
        getNVDSectionRegList(cpuInfo->inputFile.bufferPtr, "csr", enable);
    if (*regList == NULL)
    {
        if (cJSON_GetObjectItemCaseSensitive(jsonChild, NVD_FILE_CSR_KEY) ==
            NULL)
        {
            cJSON_AddStringToObject(jsonChild, NVD_FILE_CSR_KEY,
                                    NVD_FILE_CSR_ERR);
        }
        return ACD_INVALID_OBJECT;
    }

    if (*enable == false)
    {
        if (cJSON_GetObjectItemCaseSensitive(jsonChild, RECORD_ENABLE) == NULL)
        {
            cJSON_AddFalseToObject(jsonChild, RECORD_ENABLE);
        }
        return ACD_SECTION_DISABLE;
    }

    cJSON* itRegs = NULL;
    cJSON* itParams = NULL;
    uint16_t count = 0;
    nvdCSR reg = {0};

    cJSON_ArrayForEach(itRegs, *regList)
    {
        int position = 0;
        count++;
        cJSON_ArrayForEach(itParams, itRegs)
        {
            switch (position)
            {
                case NVD_CSR_NAME:
                    reg.name = itParams->valuestring;
                    break;
                case NVD_CSR_DEV:
                    reg.dev = itParams->valueint;
                    break;
                case NVD_CSR_FUNC:
                    reg.func = itParams->valueint;
                    break;
                case NVD_CSR_OFFSET:
                    reg.offset = strtoull(itParams->valuestring, NULL, 16);
                    break;
                default:
                    break;
            }
            position++;
        }
        uint32_t val = 0;
        PECIStatus status;
        char jsonStr[NVD_JSON_STRING_LEN];
        SmBusCsrEntry csr = {
            .dev = reg.dev, .func = reg.func, .offset = reg.offset};
        status = csrRd(cpuInfo->clientAddr, dimm, csr, &val, NULL, false);

        if (status != PECI_SUCCESS)
        {
            cd_snprintf_s(jsonStr, NVD_JSON_STRING_LEN, NVD_RC, status);
        }
        else
        {
            cd_snprintf_s(jsonStr, NVD_JSON_STRING_LEN, NVD_UINT32_FMT, val);
        }
        cJSON_AddStringToObject(jsonChild, reg.name, jsonStr);
    }

    return ACD_SUCCESS;
}

acdStatus logCSRSection(const CPUInfo* const cpuInfo, const uint8_t dimm,
                        cJSON* jsonChild)
{
    cJSON* regList = NULL;
    cJSON* csrSection = NULL;
    bool enable = true;

    CRASHDUMP_PRINT(INFO, stderr, "Logging %s on PECI address %d\n", "NVD CSR",
                    cpuInfo->clientAddr);

    cJSON_AddItemToObject(jsonChild, "csr", csrSection = cJSON_CreateObject());

    return readCSRUsingInputFile(&regList, cpuInfo, dimm, csrSection, &enable);
}

static acdStatus logSmartAndHealthInfo(const CPUInfo* const cpuInfo,
                                       const uint8_t dimm, cJSON* jsonChild)
{
    device_info_t device = {0};
    fis_smart_and_health_payload_t payload = {0};
    RETURN_STATUS fisRet;

    device.device_addr.peci_address.peci_addr = cpuInfo->clientAddr;
    device.device_addr.peci_address.slot = dimm;
    device.device_addr.interface.iface = interface_peci;

    fisRet = fw_cmd_get_smart_and_health(&device.device_addr, &payload);
    if (fisRet == RETURN_SUCCESS)
    {
        logSmartHealthInfoAndStatus(fisRet, &payload, sizeof(payload),
                                    jsonChild);
    }

    return ACD_SUCCESS;
}

static acdStatus readSmartHealthInfoUsingInputFile(cJSON** smartHealthInfo,
                                                   const CPUInfo* const cpuInfo,
                                                   const uint8_t dimm,
                                                   cJSON* jsonChild,
                                                   bool* const enable)
{
    *smartHealthInfo = getNVDSection(cpuInfo->inputFile.bufferPtr,
                                     "smart_health_info", enable);
    if (*smartHealthInfo == NULL)
    {
        if (cJSON_GetObjectItemCaseSensitive(
                jsonChild, PMEM_FILE_SMART_HEALTH_LOG_KEY) == NULL)
        {
            cJSON_AddStringToObject(jsonChild, PMEM_FILE_SMART_HEALTH_LOG_KEY,
                                    PMEM_FILE_SMART_HEALTH_LOG_ERR);
        }
        return ACD_INVALID_OBJECT;
    }

    if (*enable == false)
    {
        if (cJSON_GetObjectItemCaseSensitive(jsonChild, RECORD_ENABLE) == NULL)
        {
            cJSON_AddFalseToObject(jsonChild, RECORD_ENABLE);
        }
        return ACD_SECTION_DISABLE;
    }

    logSmartAndHealthInfo(cpuInfo, dimm, jsonChild);
    return ACD_SUCCESS;
}

acdStatus logSmartHealthInfoSection(const CPUInfo* const cpuInfo,
                                    const uint8_t dimm, cJSON* jsonChild)
{
    cJSON* regList = NULL;
    cJSON* smartHealthInfo = NULL;
    bool enable = true;

    CRASHDUMP_PRINT(INFO, stderr, "Logging %s on PECI address %d\n",
                    "NVD Smart Health Info", cpuInfo->clientAddr);

    cJSON_AddItemToObject(jsonChild, "smart_health_info",
                          smartHealthInfo = cJSON_CreateObject());
    return readSmartHealthInfoUsingInputFile(&regList, cpuInfo, dimm,
                                             smartHealthInfo, &enable);
}

acdStatus fillNVDSection(CPUInfo* cpuInfo, const uint8_t cpuNum,
                         cJSON* jsonChild, RunTimeInfo* runTimeInfo)
{
    acdStatus status = ACD_SUCCESS;
    bool autoDiscovery = false;

    cJSON* jsonAutoDiscovery = cJSON_GetObjectItemCaseSensitive(
        cJSON_GetObjectItemCaseSensitive(cpuInfo->inputFile.bufferPtr, "NVD"),
        "dimm_masks_auto_discovery");
    if (jsonAutoDiscovery != NULL)
    {
        autoDiscovery = cJSON_IsTrue(jsonAutoDiscovery);
    }

    if (autoDiscovery)
    {
        uint16_t disStatus = discovery(cpuInfo->clientAddr, &cpuInfo->dimmMask);
        if ((disStatus != 0) || (cpuInfo->dimmMask == 0))
        {
            CRASHDUMP_PRINT(INFO, stderr,
                            "OptanePMem not found on PECI address %d\n",
                            cpuInfo->clientAddr);
            cpuInfo->dimmMask = 0;
        }
    }
    for (uint32_t dimm = 0; dimm < NVD_MAX_DIMM; dimm++)
    {
        if (!CHECK_BIT(cpuInfo->dimmMask, dimm))
        {
            continue;
        }

        cJSON* dimmSection = NULL;
        char jsonStr[NVD_JSON_STRING_LEN];
        cd_snprintf_s(jsonStr, sizeof(jsonStr), dimmMap[dimm], cpuNum);
        cJSON_AddItemToObject(jsonChild, jsonStr,
                              dimmSection = cJSON_CreateObject());

        uint32_t globalRemainingTime = (uint32_t)getTimeRemainingFromStart(
            runTimeInfo->maxGlobalTime, runTimeInfo->globalRunTime);
        if (globalRemainingTime == 0)
        {
            char jsonItemString[NVD_JSON_STRING_LEN];
            runTimeInfo->maxGlobalRunTimeReached = true;
            cd_snprintf_s(jsonItemString, NVD_JSON_STRING_LEN, "%ds",
                          runTimeInfo->maxGlobalTime);
            cJSON_AddStringToObject(dimmSection, "_nvd_global_timeout",
                                    jsonItemString);
            break;
        }

        logCSRSection(cpuInfo, dimm, dimmSection);
        logIdentifyDimm(cpuInfo, dimm, dimmSection);
        logErrLogSection(cpuInfo, dimm, dimmSection);
        logSmartHealthInfoSection(cpuInfo, dimm, dimmSection);
    }
    return status;
}
