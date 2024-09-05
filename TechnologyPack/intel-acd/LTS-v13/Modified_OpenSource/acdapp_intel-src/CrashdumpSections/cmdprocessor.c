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

#include "cmdprocessor.h"

#include <search.h>

/*#include "../engine/BigCore.h"
#include "../engine/Crashlog.h"
#include "../engine/TorDump.h"
#include "../engine/utils.h"*/
#include "../CrashdumpSections/BigCore.h"
#include "../CrashdumpSections/Crashlog.h"
#include "../CrashdumpSections/TorDump.h"
#include "../CrashdumpSections/utils.h"
#include "inputparser.h"
#include "validator.h"
#include "cJSON.h"

static void UpdateInternalVar(CmdInOut* cmdInOut)
{
    if (cmdInOut->internalVarName != NULL)
    {
        char jsonItemString[JSON_VAL_LEN];
        if (cmdInOut->out.size == sizeof(uint64_t))
        {
            cd_snprintf_s(jsonItemString, JSON_VAL_LEN, "0x%" PRIx64 "",
                          cmdInOut->out.val.u64);
        }
        else
        {
            cd_snprintf_s(jsonItemString, JSON_VAL_LEN, "0x%" PRIx32 "",
                          cmdInOut->out.val.u32[0]);
        }
        cJSON_AddItemToObject(cmdInOut->internalVarsTracker,
                              cmdInOut->internalVarName,
                              cJSON_CreateString(jsonItemString));
    }
}

static acdStatus CrashDump_Discovery(CmdInOut* cmdInOut)
{
    struct peci_crashdump_disc_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsCrashDump_DiscoveryParamsValid(cmdInOut->in.params,
                                          cmdInOut->validatorParams))
    {
        return ACD_INVALID_CRASHDUMP_DISCOVERY_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.subopcode = it->valueint;
                break;
            case 2:
                params.param0 = it->valueint;
                break;
            case 3:
                params.param1 = it->valueint;
                break;
            case 4:
                params.param2 = it->valueint;
                break;
            case 5:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_CRASHDUMP_DISCOVERY_PARAMS;
        }
        position++;
    }
    cmdInOut->out.ret = peci_CrashDump_Discovery(
        params.addr, params.subopcode, params.param0, params.param1,
        params.param2, params.rx_len, (uint8_t*)&cmdInOut->out.val.u64,
        &cmdInOut->out.cc);

    if (cmdInOut->out.ret != PECI_CC_SUCCESS)
    {
        if (cmdInOut->internalVarsTracker != NULL)
        {
            char jsonItemString[JSON_VAL_LEN];
            cd_snprintf_s(jsonItemString, JSON_VAL_LEN, CMD_ERROR_VALUE);
            cJSON_AddItemToObject(cmdInOut->internalVarsTracker,
                                  cmdInOut->internalVarName,
                                  cJSON_CreateString(jsonItemString));
        }
    }
    else
    {
        UpdateInternalVar(cmdInOut);
    }

    return ACD_SUCCESS;
}

static acdStatus CrashDump_GetFrame(CmdInOut* cmdInOut)
{
    struct peci_crashdump_disc_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsCrashDump_GetFrameParamsValid(cmdInOut->in.params,
                                         cmdInOut->validatorParams))
    {
        return ACD_INVALID_CRASHDUMP_GETFRAME_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.param0 = it->valueint;
                break;
            case 2:
                params.param1 = it->valueint;
                break;
            case 3:
                params.param2 = it->valueint;
                break;
            case 4:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_CRASHDUMP_GETFRAME_PARAMS;
        }
        position++;
    }

    cmdInOut->out.ret = peci_CrashDump_GetFrame(
        params.addr, params.param0, params.param1, params.param2, params.rx_len,
        (uint8_t*)&cmdInOut->out.val.u64, &cmdInOut->out.cc);
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus Ping(CmdInOut* cmdInOut)
{
    if (!IsPingParamsValid(cmdInOut->in.params, cmdInOut->validatorParams))
    {
        return ACD_INVALID_PING_PARAMS;
    }
    cmdInOut->out.ret = peci_Ping(cmdInOut->in.params->valueint);
    return ACD_SUCCESS;
}

static acdStatus GetCPUID(CmdInOut* cmdInOut)
{
    if (!IsGetCPUIDParamsValid(cmdInOut->in.params, cmdInOut->validatorParams))
    {
        return ACD_INVALID_GETCPUID_PARAMS;
    }
    cmdInOut->out.ret = peci_GetCPUID(
        cmdInOut->in.params->valueint, &cmdInOut->out.cpuID.cpuModel,
        &cmdInOut->out.cpuID.stepping, &cmdInOut->out.cc);
    return ACD_SUCCESS;
}

static acdStatus RdIAMSR(CmdInOut* cmdInOut)
{
    struct peci_rd_ia_msr_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsRdIAMSRParamsValid(cmdInOut->in.params, cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDIAMSR_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.thread_id = it->valueint;
                break;
            case 2:
                params.address = it->valueint;
                break;
            default:
                return ACD_INVALID_RDIAMSR_PARAMS;
        }
        position++;
    }
    cmdInOut->out.size = sizeof(uint64_t);
    cmdInOut->out.ret =
        peci_RdIAMSR(params.addr, params.thread_id, params.address,
                     (uint64_t*)&cmdInOut->out.val.u64, &cmdInOut->out.cc);
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus RdPkgConfig(CmdInOut* cmdInOut)
{
    struct peci_rd_pkg_cfg_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsRdPkgConfigParamsValid(cmdInOut->in.params,
                                  cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDPKGCONFIG_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.index = it->valueint;
                break;
            case 2:
                params.param = it->valueint;
                break;
            case 3:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_RDPKGCONFIG_PARAMS;
        }
        position++;
    }

    cmdInOut->out.ret =
        peci_RdPkgConfig(params.addr, params.index, params.param, params.rx_len,
                         (uint8_t*)&cmdInOut->out.val.u64, &cmdInOut->out.cc);
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus RdPkgConfigCore(CmdInOut* cmdInOut)
{
    struct peci_rd_pkg_cfg_msg params = {0};
    int position = 0;
    uint8_t core = 0;
    cJSON* it = NULL;

    if (!IsRdPkgConfigCoreParamsValid(cmdInOut->in.params,
                                      cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDPKGCONFIGCORE_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.index = it->valueint;
                break;
            case 2:
                core = it->valueint;
                break;
            case 3:
                params.param = it->valueint;
                break;
            case 4:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_RDPKGCONFIGCORE_PARAMS;
        }
        position++;
    }
    params.param = (core << 8 | params.param);

    cmdInOut->out.ret =
        peci_RdPkgConfig(params.addr, params.index, params.param, params.rx_len,
                         (uint8_t*)&cmdInOut->out.val.u64, &cmdInOut->out.cc);

    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus RdPCIConfigLocal(CmdInOut* cmdInOut)
{
    struct peci_rd_pci_cfg_local_msg params = {0};
    int position = 0;
    cJSON* it = NULL;
    const int wordFrameSize = (sizeof(uint64_t) / sizeof(uint16_t));

    if (!IsRdPciConfigLocalParamsValid(cmdInOut->in.params,
                                       cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDPCICONFIGLOCAL_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.bus = it->valueint;
                break;
            case 2:
                params.device = it->valueint;
                break;
            case 3:
                params.function = it->valueint;
                break;
            case 4:
                params.reg = it->valueint;
                break;
            case 5:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_RDPCICONFIGLOCAL_PARAMS;
        }
        position++;
    }
    switch (params.rx_len)
    {
        case sizeof(uint8_t):
        case sizeof(uint16_t):
        case sizeof(uint32_t):
            cmdInOut->out.ret = peci_RdPCIConfigLocal(
                params.addr, params.bus, params.device, params.function,
                params.reg, params.rx_len, (uint8_t*)&cmdInOut->out.val.u64,
                &cmdInOut->out.cc);
            break;
        case sizeof(uint64_t):
            for (uint8_t u8Dword = 0; u8Dword < 2; u8Dword++)
            {
                cmdInOut->out.ret = peci_RdPCIConfigLocal(
                    params.addr, params.bus, params.device, params.function,
                    params.reg + (u8Dword * wordFrameSize), sizeof(uint32_t),
                    (uint8_t*)&cmdInOut->out.val.u32[u8Dword],
                    &cmdInOut->out.cc);
            }
            break;
        default:
            break;
    }
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus RdEndPointConfigPciLocal(CmdInOut* cmdInOut)
{
    struct peci_rd_end_pt_cfg_msg params = {0};
    int position = 0;
    cJSON* it = NULL;
    const int wordFrameSize = (sizeof(uint64_t) / sizeof(uint16_t));

    if (!IsRdEndPointConfigPciLocalParamsValid(cmdInOut->in.params,
                                               cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDENDPOINTCONFIGPCILOCAL_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.params.pci_cfg.seg = it->valueint;
                break;
            case 2:
                params.params.pci_cfg.bus = it->valueint;
                break;
            case 3:
                params.params.pci_cfg.device = it->valueint;
                break;
            case 4:
                params.params.pci_cfg.function = it->valueint;
                break;
            case 5:
                params.params.pci_cfg.reg = it->valueint;
                break;
            case 6:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_RDENDPOINTCONFIGPCILOCAL_PARAMS;
        }
        position++;
    }
    switch (params.rx_len)
    {
        case sizeof(uint8_t):
        case sizeof(uint16_t):
        case sizeof(uint32_t):
            cmdInOut->out.ret = peci_RdEndPointConfigPciLocal(
                params.addr, params.params.pci_cfg.seg,
                params.params.pci_cfg.bus, params.params.pci_cfg.device,
                params.params.pci_cfg.function, params.params.pci_cfg.reg,
                params.rx_len, (uint8_t*)&cmdInOut->out.val.u64,
                &cmdInOut->out.cc);
            break;
        case sizeof(uint64_t):
            for (uint8_t u8Dword = 0; u8Dword < 2; u8Dword++)
            {
                cmdInOut->out.ret = peci_RdEndPointConfigPciLocal(
                    params.addr, params.params.pci_cfg.seg,
                    params.params.pci_cfg.bus, params.params.pci_cfg.device,
                    params.params.pci_cfg.function,
                    params.params.pci_cfg.reg + (u8Dword * wordFrameSize),
                    sizeof(uint32_t), (uint8_t*)&cmdInOut->out.val.u32[u8Dword],
                    &cmdInOut->out.cc);
            }
            break;
        default:    
            break;
    }
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus WrEndPointConfigPciLocal(CmdInOut* cmdInOut)
{
    struct peci_wr_end_pt_cfg_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsWrEndPointConfigPciLocalParamsValid(cmdInOut->in.params,
                                               cmdInOut->validatorParams))
    {
        return ACD_INVALID_WRENDPOINTCONFIGPCILOCAL_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.params.pci_cfg.seg = it->valueint;
                break;
            case 2:
                params.params.pci_cfg.bus = it->valueint;
                break;
            case 3:
                params.params.pci_cfg.device = it->valueint;
                break;
            case 4:
                params.params.pci_cfg.function = it->valueint;
                break;
            case 5:
                params.params.pci_cfg.reg = it->valueint;
                break;
            case 6:
                params.tx_len = it->valueint;
                cmdInOut->out.size = params.tx_len;
                break;
            case 7:
                params.value = it->valueint;
                break;
            default:
                return ACD_INVALID_WRENDPOINTCONFIGPCILOCAL_PARAMS;
        }
        position++;
    }
    cmdInOut->out.ret = peci_WrEndPointPCIConfigLocal(
        params.addr, params.params.pci_cfg.seg, params.params.pci_cfg.bus,
        params.params.pci_cfg.device, params.params.pci_cfg.function,
        params.params.pci_cfg.reg, params.tx_len, params.value,
        &cmdInOut->out.cc);
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus RdEndPointConfigMmio(CmdInOut* cmdInOut)
{
    struct peci_rd_end_pt_cfg_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsRdEndPointConfigMmioParamsValid(cmdInOut->in.params,
                                           cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDENDPOINTCONFIGMMIO_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.params.mmio.seg = it->valueint;
                break;
            case 2:
                params.params.mmio.bus = it->valueint;
                break;
            case 3:
                params.params.mmio.device = it->valueint;
                break;
            case 4:
                params.params.mmio.function = it->valueint;
                break;
            case 5:
                params.params.mmio.bar = it->valueint;
                break;
            case 6:
                params.params.mmio.addr_type = it->valueint;
                break;
            case 7:
                params.params.mmio.offset = (uint64_t)it->valueint;
                break;
            case 8:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_RDENDPOINTCONFIGMMIO_PARAMS;
        }
        position++;
    }
    cmdInOut->out.ret = peci_RdEndPointConfigMmio(
        params.addr, params.params.mmio.seg, params.params.mmio.bus,
        params.params.mmio.device, params.params.mmio.function,
        params.params.mmio.bar, params.params.mmio.addr_type,
        params.params.mmio.offset, params.rx_len,
        (uint8_t*)&cmdInOut->out.val.u64, &cmdInOut->out.cc);
    UpdateInternalVar(cmdInOut);
    return ACD_SUCCESS;
}

static acdStatus RdPostEnumBus(CmdInOut* cmdInOut)
{
    PostEnumBus params = {0};
    cJSON* it = NULL;
    int position = 0;

    if (!IsRdPostEnumBusParamsValid(cmdInOut->in.params,
                                    cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDPOSTENUMBUS_PARAMS;
    }
    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.cpubusno_valid = (uint32_t)it->valuedouble;
                break;
            case 1:
                params.cpubusno = (uint32_t)it->valuedouble;
                break;
            case 2:
                params.bitToCheck = it->valueint;
                break;
            case 3:
                params.shift = (uint8_t)it->valueint;
                break;
            default:
                return ACD_INVALID_RDPOSTENUMBUS_PARAMS;
        }
        position++;
    }
    cmdInOut->out.size = sizeof(uint8_t);
    if (0 == CHECK_BIT(params.cpubusno_valid, params.bitToCheck))
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "RdPostEnumBus: Bit %d not enable in post"
                        " enumerated bus 0x%x.\n",
                        params.bitToCheck, params.cpubusno_valid);
        cmdInOut->out.val.u32[0] = 0;
    }
    else
    {
        cmdInOut->out.val.u32[0] = ((params.cpubusno >> params.shift) & 0xff);
    }
    UpdateInternalVar(cmdInOut);

    // Set ret & cc to success for logger
    cmdInOut->out.ret = PECI_CC_SUCCESS;
    cmdInOut->out.cc = PECI_DEV_CC_SUCCESS;
    return ACD_SUCCESS;
}

static acdStatus RdChaCount(CmdInOut* cmdInOut)
{
    ChaCount params = {0};
    cJSON* it = NULL;
    int position = 0;
    uint8_t chaCountValue;

    if (!IsRdChaCountParamsValid(cmdInOut->in.params,
                                 cmdInOut->validatorParams))
    {
        return ACD_INVALID_RDCHACOUNT_PARAMS;
    }
    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.chaMask0 = (uint32_t)it->valuedouble;
                break;
            case 1:
                params.chaMask1 = (uint32_t)it->valuedouble;
                break;
            default:
                return ACD_INVALID_RDCHACOUNT_PARAMS;
        }
        position++;
    }

    cmdInOut->out.size = sizeof(uint8_t);
    cmdInOut->out.val.u32[0] = __builtin_popcount((int)params.chaMask0) +
                               __builtin_popcount((int)params.chaMask1);
    UpdateInternalVar(cmdInOut);

    // Set ret & cc to success for logger
    cmdInOut->out.ret = PECI_CC_SUCCESS;
    cmdInOut->out.cc = PECI_DEV_CC_SUCCESS;
    return ACD_SUCCESS;
}

static acdStatus Telemetry_Discovery(CmdInOut* cmdInOut)
{
    struct peci_telemetry_disc_msg params = {0};
    int position = 0;
    cJSON* it = NULL;

    if (!IsTelemetry_DiscoveryParamsValid(cmdInOut->in.params,
                                          cmdInOut->validatorParams))
    {
        return ACD_INVALID_TELEMETRY_DISCOVERY_PARAMS;
    }

    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                params.addr = it->valueint;
                break;
            case 1:
                params.subopcode = it->valueint;
                break;
            case 2:
                params.param0 = it->valueint;
                break;
            case 3:
                params.param1 = it->valueint;
                break;
            case 4:
                params.param2 = it->valueint;
                break;
            case 5:
                params.rx_len = it->valueint;
                cmdInOut->out.size = params.rx_len;
                break;
            default:
                return ACD_INVALID_TELEMETRY_DISCOVERY_PARAMS;
        }
        position++;
    }
    cmdInOut->out.ret = peci_Telemetry_Discovery(
        params.addr, params.subopcode, params.param0, params.param1,
        params.param2, params.rx_len, (uint8_t*)&cmdInOut->out.val,
        &cmdInOut->out.cc);

    if (cmdInOut->out.ret != PECI_CC_SUCCESS || PECI_CC_UA(cmdInOut->out.cc))
    {
        if (cmdInOut->internalVarsTracker != NULL)
        {
            char jsonItemString[JSON_VAL_LEN];
            cd_snprintf_s(jsonItemString, JSON_VAL_LEN, CMD_ERROR_VALUE);
            cJSON_AddItemToObject(cmdInOut->internalVarsTracker,
                                  cmdInOut->internalVarName,
                                  cJSON_CreateString(jsonItemString));
        }
    }
    else
    {
        UpdateInternalVar(cmdInOut);
    }

    return ACD_SUCCESS;
}

uint8_t getIsTelemetrySupportedResult(CmdInOut* cmdInOut)
{
    cJSON* isTelemetryEnabledNode = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "IsEnable");

    if (isTelemetryEnabledNode != NULL)
    {
	//-----AMI start-------
        if (memcmp(CMD_ERROR_VALUE,
                     isTelemetryEnabledNode->valuestring,
                     strlen(isTelemetryEnabledNode->valuestring)) == 0)
	//-----AMI end----------
        {
            return TELEMETRY_UNSUPPORTED;
        }

        return (uint8_t)strtoul(isTelemetryEnabledNode->valuestring, NULL, 16);
    }

    return TELEMETRY_UNSUPPORTED;
}

uint8_t getNumberofCrashlogAgents(CmdInOut* cmdInOut)
{
    cJSON* numberofCrashlogAgentsNode = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "NumAgents");

    if (numberofCrashlogAgentsNode != NULL)
    {
	//-----AMI start-------
        if (memcmp(CMD_ERROR_VALUE,
                     numberofCrashlogAgentsNode->valuestring, 
                     strlen(numberofCrashlogAgentsNode->valuestring)) == 0)
	//-----AMI end-------
        {
            return NO_CRASHLOG_AGENTS;
        }

        return (uint8_t)strtoul(numberofCrashlogAgentsNode->valuestring, NULL,
                                16);
    }

    return NO_CRASHLOG_AGENTS;
}

static acdStatus LogCrashlog(CmdInOut* cmdInOut)
{
    cmdInOut->out.size = sizeof(uint8_t);
    if (GenerateJsonPath(cmdInOut, cmdInOut->root, cmdInOut->logger, false) ==
        ACD_SUCCESS)
    {
        if (getIsTelemetrySupportedResult(cmdInOut))
        {
            return logCrashlogSection(
                cmdInOut->cpuInfo, cmdInOut->logger->nameProcessing.jsonOutput,
                getNumberofCrashlogAgents(cmdInOut), *cmdInOut->runTimeInfo);
        }
    }
    else
    {
        CRASHDUMP_PRINT(ERR, stderr, "Could not log Crashlog, path is Null.\n");
    }

    return ACD_FAILURE;
}

static bool getIsCrashdumpEnableResult(CmdInOut* cmdInOut)
{
    cJSON* isCrashdumpEnableNode = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "IsCrashdumpEnable");

    if (isCrashdumpEnableNode != NULL)
    {
	//-------AMI start----------
        if (memcmp(CMD_ERROR_VALUE,
                     isCrashdumpEnableNode->valuestring, 
                     strlen(isCrashdumpEnableNode->valuestring)) == 0)
	//-------AMI end-------------
        {
            return false;
        }

        uint8_t isEnabled =
            (uint8_t)strtoul(isCrashdumpEnableNode->valuestring, NULL, 16);

        if (isEnabled == ICX_A0_CRASHDUMP_DISABLED)
        {
            CRASHDUMP_PRINT(ERR, stderr,
                            "Crashdump is disabled (%d) during discovery "
                            "(disabled:%d)\n",
                            cmdInOut->out.ret, isEnabled);
            return false;
        }
        return true;
    }
    return false;
}

static bool getNumCrashdumpAgentsResult(CmdInOut* cmdInOut)
{
    cJSON* numCrashdumpAgentsNode = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "NumCrashdumpAgents");

    if (numCrashdumpAgentsNode != NULL)
    {
	//------AMI start-------
        if (memcmp(CMD_ERROR_VALUE,
                     numCrashdumpAgentsNode->valuestring, 
                     strlen(numCrashdumpAgentsNode->valuestring)) == 0)
	//-------AMI end--------
        {
            return false;
        }

        uint16_t numCrashdumpAgents =
            (uint16_t)strtoul(numCrashdumpAgentsNode->valuestring, NULL, 16);
        if (numCrashdumpAgents <= PECI_CRASHDUMP_CORE)
        {
            CRASHDUMP_PRINT(
                ERR, stderr,
                "Error occurred (%d) during discovery (num of agents:%d)\n",
                cmdInOut->out.ret, numCrashdumpAgents);
            return false;
        }
        return true;
    }
    return false;
}

static bool getCrashdumpGUIDResult(CmdInOut* cmdInOut)
{
    cJSON* crashdumpGUIDNode = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "CrashdumpGUID");

    if (crashdumpGUIDNode != NULL)
    {
	//------AMI start-------
        if (memcmp(CMD_ERROR_VALUE,
                     crashdumpGUIDNode->valuestring,
                     strlen(crashdumpGUIDNode->valuestring)) == 0)
	//------AMI end-------
        {
            return false;
        }

        return true;
    }
    return false;
}

static bool getCrashdumpPayloadSizeResult(CmdInOut* cmdInOut)
{
    cJSON* crashdumpPayloadSizeNode = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "CrashdumpPayloadSize");

    if (crashdumpPayloadSizeNode != NULL)
    {
	//------AMI start-------
        if (memcmp(CMD_ERROR_VALUE,
                     crashdumpPayloadSizeNode->valuestring, 
                     strlen(crashdumpPayloadSizeNode->valuestring)) == 0)
	//------AMI end-------
        {
            return false;
        }
        return true;
    }
    return false;
}

static acdStatus LogBigCore(CmdInOut* cmdInOut)
{
    cmdInOut->out.size = sizeof(uint8_t);
    if (GenerateJsonPath(cmdInOut, cmdInOut->root, cmdInOut->logger, false) ==
        ACD_SUCCESS)
    {
        if (getIsCrashdumpEnableResult(cmdInOut) &&
            getNumCrashdumpAgentsResult(cmdInOut) &&
            getCrashdumpGUIDResult(cmdInOut) &&
            getCrashdumpPayloadSizeResult(cmdInOut))
        {
            return logBigCoreSection(
                cmdInOut->cpuInfo, cmdInOut->logger->nameProcessing.jsonOutput,
                *cmdInOut->runTimeInfo);
        }
    }
    else
    {
        CRASHDUMP_PRINT(ERR, stderr, "Could not log BigCore, path is Null.\n");
    }

    return ACD_FAILURE;
}

static acdStatus RdAndConcatenate(CmdInOut* cmdInOut)
{
    cJSON* it = NULL;
    int position = 0;
    uint32_t high32BitValue = 0;
    uint32_t low32BitValue = 0;

    if (!IsRdAndConcatenateParamsValid(cmdInOut->in.params,
                                       cmdInOut->validatorParams))
    {
        return ACD_INVALID_RD_CONCATENATE_PARAMS;
    }
    cJSON_ArrayForEach(it, cmdInOut->in.params)
    {
        switch (position)
        {
            case 0:
                low32BitValue = (uint32_t)it->valuedouble;
                break;
            case 1:
                high32BitValue = (uint32_t)it->valuedouble;
                break;
            default:
                return ACD_INVALID_RD_CONCATENATE_PARAMS;
        }
        position++;
    }

    cmdInOut->out.size = sizeof(uint64_t);
    cmdInOut->out.val.u32[1] = high32BitValue;
    cmdInOut->out.val.u32[0] = low32BitValue;

    UpdateInternalVar(cmdInOut);

    // Set ret & cc to success for logger
    cmdInOut->out.ret = PECI_CC_SUCCESS;
    cmdInOut->out.cc = PECI_DEV_CC_SUCCESS;
    return ACD_SUCCESS;
}

static acdStatus RdGlobalVars(CmdInOut* cmdInOut)
{
    if (!IsRdGlobalVarsValid(cmdInOut->in.params, cmdInOut->validatorParams))
    {
        return ACD_INVALID_GLOBAL_VARS_PARAMS;
    }
    cmdInOut->out.ret = PECI_CC_SUCCESS;
    cmdInOut->out.cc = PECI_DEV_CC_SUCCESS;
    int mismatchclientaddr = 1;
    int mismatchcpuid = 1;
    int mismatchcpuidsource = 1;
    int mismatchcoremasksource = 1;
    int mismatchchacountsource = 1;
    int mismatchchcoremask = 1;
    int mismatchchacount = 1;
    int mismatchcorecount = 1;
    int mismatchcrashcorecount = 1;
    int mismatchcrashcoremask = 1;
    //------AMI start-------
    mismatchclientaddr = memcmp(cmdInOut->out.stringVal,  peci_addr,
             strlen(peci_addr));
    //------AMI end---------
    if (mismatchclientaddr == 0)
    {
        cmdInOut->out.size = sizeof(uint8_t);
        cmdInOut->out.val.u64 = (uint64_t)cmdInOut->cpuInfo->clientAddr;
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchcpuid = memcmp(cmdInOut->out.stringVal,  cpuid, strlen(cpuid));
    //------AMI end-------
    if (mismatchcpuid == 0)
    {
        cmdInOut->out.size = sizeof(uint32_t);
        cmdInOut->out.val.u64 =
            (uint64_t)(cmdInOut->cpuInfo->cpuidRead.cpuModel |
                       cmdInOut->cpuInfo->cpuidRead.stepping);
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchcpuidsource = memcmp(cmdInOut->out.stringVal,
             cpuid_source, strlen(cpuid_source));
    //------AMI end-------
    if (mismatchcpuidsource == 0)
    {
        cmdInOut->out.printString = true;
        cmdInOut->out.stringVal =
            decodeState((int)cmdInOut->cpuInfo->cpuidRead.source);
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchcoremasksource =memcmp(cmdInOut->out.stringVal,
             coremask_source, strlen(coremask_source));
    //------AMI end-------
    if (mismatchcoremasksource == 0)
    {
        cmdInOut->out.printString = true;
        cmdInOut->out.stringVal =
            decodeState((int)cmdInOut->cpuInfo->coreMaskRead.source);
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchchacountsource = memcmp(cmdInOut->out.stringVal, 
             chacount_source, strlen(chacount_source));
    //------AMI end-------
    if (mismatchchacountsource == 0)
    {
        cmdInOut->out.printString = true;
        cmdInOut->out.stringVal =
            decodeState((int)cmdInOut->cpuInfo->chaCountRead.source);
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchchcoremask = memcmp(cmdInOut->out.stringVal, 
             coremask, strlen(coremask));
    //------AMI end-------
    if (mismatchchcoremask == 0)
    {
        cmdInOut->out.size = sizeof(uint64_t);
        cmdInOut->out.val.u64 = (uint64_t)(cmdInOut->cpuInfo->coreMask);
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchchacount = memcmp(cmdInOut->out.stringVal, 
             chacount, strlen(chacount));
    //------AMI end-------
    if (mismatchchacount == 0)
    {
        cmdInOut->out.size = sizeof(uint8_t);
        cmdInOut->out.val.u64 = (uint64_t)(cmdInOut->cpuInfo->chaCount);
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchcorecount = memcmp(cmdInOut->out.stringVal, 
             corecount, strlen(corecount));
    //------AMI end-------
    if (mismatchcorecount == 0)
    {
        cmdInOut->out.size = sizeof(uint8_t);
        cmdInOut->out.val.u64 =
            (uint64_t)(__builtin_popcountll(cmdInOut->cpuInfo->coreMask));
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchcrashcorecount = memcmp(cmdInOut->out.stringVal, crashcorecount,
             strlen(crashcorecount));
    //------AMI end-------
    if (mismatchcrashcorecount == 0)
    {
        cmdInOut->out.size = sizeof(uint8_t);
        cmdInOut->out.val.u64 = (uint64_t)(
            __builtin_popcountll(cmdInOut->cpuInfo->crashedCoreMask));
        return ACD_SUCCESS;
    }
    //------AMI start-------
    mismatchcrashcoremask = memcmp(cmdInOut->out.stringVal,
             crashcoremask,
             strlen(crashcoremask));
    //------AMI end-------
    if (mismatchcrashcoremask == 0)
    {
        cmdInOut->out.size = sizeof(uint64_t);
        cmdInOut->out.val.u64 = (uint64_t)(cmdInOut->cpuInfo->crashedCoreMask);
        return ACD_SUCCESS;
    }
    cmdInOut->out.printString = false;
    cmdInOut->out.val.u64 = 0;
    CRASHDUMP_PRINT(ERR, stderr, "Unknown RdGlobalVar command.\n");
    return ACD_FAILURE;
}

static acdStatus SaveStrVars(CmdInOut* cmdInOut)
{
    if (!IsSaveStrVarsValid(cmdInOut->in.params, cmdInOut->validatorParams))
    {
        return ACD_INVALID_SAVE_STR_VARS_PARAMS;
    }
    cmdInOut->out.ret = PECI_CC_SUCCESS;
    cmdInOut->out.cc = PECI_DEV_CC_SUCCESS;
    cmdInOut->out.printString = true;
    return ACD_SUCCESS;
}

static uint64_t getPayloadExp(CmdInOut* cmdInOut)
{
    cJSON* payloadSize = cJSON_GetObjectItemCaseSensitive(
        cmdInOut->internalVarsTracker, "CrashdumpPayloadSize");
    if (payloadSize == NULL)
    {
        CRASHDUMP_PRINT(ERR, stderr, "Could not read CrashdumpPayloadSize\n");
        return 0;
    }
    char* pCrashdumpPayloadSize = payloadSize->valuestring;
    uint8_t u8PayloadExp = (uint8_t)strtol(pCrashdumpPayloadSize, NULL, 16);
    return u8PayloadExp;
}

static acdStatus LogTor(CmdInOut* cmdInOut)
{
    cmdInOut->out.size = sizeof(uint8_t);
    if (GenerateJsonPath(cmdInOut, cmdInOut->root, cmdInOut->logger, false) ==
        ACD_SUCCESS)
    {
        if (getIsCrashdumpEnableResult(cmdInOut) &&
            getNumCrashdumpAgentsResult(cmdInOut) &&
            getCrashdumpGUIDResult(cmdInOut) &&
            getCrashdumpPayloadSizeResult(cmdInOut))
        {
            uint8_t u8PayloadExp = getPayloadExp(cmdInOut);
            if (u8PayloadExp > TOR_MAX_PAYLOAD_EXP ||
                u8PayloadExp == TOR_INVALID_PAYLOAD_EXP)
            {
                CRASHDUMP_PRINT(
                    ERR, stderr,
                    "Error during discovery (Invalid exponent value: %d)\n",
                    u8PayloadExp);
                return ACD_FAILURE;
            }
            uint8_t u8PayloadBytes = (1 << u8PayloadExp);
            return logTorSection(cmdInOut->cpuInfo,
                                 cmdInOut->logger->nameProcessing.jsonOutput,
                                 u8PayloadBytes);
        }
    }
    else
    {
        CRASHDUMP_PRINT(ERR, stderr, "Could not log Tor, path is Null.\n");
    }
    return ACD_FAILURE;
}

static acdStatus (*cmds[CD_NUM_OF_PECI_CMDS])() = {
    (acdStatus(*)())CrashDump_Discovery,
    (acdStatus(*)())CrashDump_GetFrame,
    (acdStatus(*)())Ping,
    (acdStatus(*)())GetCPUID,
    (acdStatus(*)())RdEndPointConfigMmio,
    (acdStatus(*)())RdPCIConfigLocal,
    (acdStatus(*)())RdEndPointConfigPciLocal,
    (acdStatus(*)())WrEndPointConfigPciLocal,
    (acdStatus(*)())RdIAMSR,
    (acdStatus(*)())RdPkgConfig,
    (acdStatus(*)())RdPkgConfigCore,
    (acdStatus(*)())RdPostEnumBus,
    (acdStatus(*)())RdChaCount,
    (acdStatus(*)())Telemetry_Discovery,
    (acdStatus(*)())LogCrashlog,
    (acdStatus(*)())LogBigCore,
    (acdStatus(*)())RdAndConcatenate,
    (acdStatus(*)())RdGlobalVars,
    (acdStatus(*)())SaveStrVars,
    (acdStatus(*)())LogTor,
};

static char* inputCMDs[CD_NUM_OF_PECI_CMDS] = {
    "CrashDump_Discovery",
    "CrashDump_GetFrame",
    "Ping",
    "GetCPUID",
    "RdEndpointConfigMMIO",
    "RdPCIConfigLocal",
    "RdEndpointConfigPCILocal",
    "WrEndPointConfigPciLocal",
    "RdIAMSR",
    "RdPkgConfig",
    "RdPkgConfigCore",
    "RdPostEnumBus",
    "RdChaCount",
    "Telemetry_Discovery",
    "LogCrashlog",
    "LogBigCore",
    "RdAndConcatenate",
    "RdGlobalVars",
    "SaveStrVars",
    "LogTor",
};

acdStatus BuildCmdsTable(ENTRY* entry)
{
    ENTRY* ep;

    hcreate(CD_NUM_OF_PECI_CMDS);
    for (int i = 0; i < CD_NUM_OF_PECI_CMDS; i++)
    {
        entry->key = inputCMDs[i];
        entry->data = (void*)(size_t)i;
        ep = hsearch(*entry, ENTER);
        if (ep == NULL)
        {
            CRASHDUMP_PRINT(ERR, stderr, "Fail adding (%s) to commands table\n",
                            entry->key);
            return ACD_FAILURE_CMD_TABLE;
        }
    }
    return ACD_SUCCESS;
}

acdStatus Execute(ENTRY* entry, CmdInOut* cmdInOut)
{
    ENTRY* ep;

    ep = hsearch(*entry, FIND);
    if (ep == NULL)
    {
        CRASHDUMP_PRINT(ERR, stderr, "Invalid PECICmd:(%s)\n", entry->key);
        return ACD_INVALID_CMD;
    }
    cmds[(size_t)(ep->data)](cmdInOut);
    return ACD_SUCCESS;
}
