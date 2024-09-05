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

#ifndef CRASHDUMP_VALIDATOR_H
#define CRASHDUMP_VALIDATOR_H

#include "../CrashdumpSections/crashdump.h"

// libpeci commands
#define GETCPUID_PARAMS_LEN 1
#define PING_PARAMS_LEN 1
#define RDPKGCONFIG_PARAMS_LEN 4
#define RDPKGCONFIGCORE_PARAMS_LEN 5
#define RDIAMSR_PARAMS_LEN 3
#define RDPCICONFIGLOCAL_PARAMS_LEN 6
#define RDENDPOINTCONFIGPCILOCAL_PARAMS_LEN 7
#define RDENDPOINTCONFIGMMIO_PARAMS_LEN 9
#define CRASHDUMP_DISCOVERY_PARAMS_LEN 6
#define CRASHDUMP_GETFRAME_PARAMS_LEN 5
#define TELEMETRY_DISCOVERY_PARAMS_LEN 6
#define WRENDPOINTCONFIGPCILOCAL_PARAMS_LEN 8

// custom commands
#define RDPOSTENUMBUS_PARAMS_LEN 4
#define RDCHACOUNT_PARAMS_LEN 2
#define RDANDCONCATENATE_PARAMS_LEN 2
#define RDGLOBALVARS_PARAMS_LEN 1
#define SAVESTRVARS_PARAMS_LEN 1
#define LOGCRASHLOG_PARAMS_LEN 2
#define LOGBIGCORE_PARAMS_LEN 4
#define LOGTOR_PARAMS_LEN 4

typedef struct
{
    bool validateInput;
} ValidatorParams;

bool IsCrashDump_DiscoveryParamsValid(cJSON* params,
                                      const ValidatorParams* ValidatorParams);
bool IsCrashDump_GetFrameParamsValid(cJSON* params,
                                     const ValidatorParams* ValidatorParams);
bool IsGetCPUIDParamsValid(cJSON* params,
                           const ValidatorParams* ValidatorParams);
bool IsPingParamsValid(cJSON* params, const ValidatorParams* ValidatorParams);
bool IsRdPciConfigLocalParamsValid(cJSON* params,
                                   const ValidatorParams* ValidatorParams);
bool IsRdEndPointConfigMmioParamsValid(cJSON* params,
                                       const ValidatorParams* ValidatorParams);
bool IsRdEndPointConfigPciLocalParamsValid(
    cJSON* params, const ValidatorParams* ValidatorParams);
bool IsWrEndPointConfigPciLocalParamsValid(
    cJSON* params, const ValidatorParams* ValidatorParams);
bool IsRdIAMSRParamsValid(cJSON* params,
                          const ValidatorParams* ValidatorParams);
bool IsRdPkgConfigParamsValid(cJSON* params,
                              const ValidatorParams* ValidatorParams);
bool IsRdPkgConfigCoreParamsValid(cJSON* params,
                                  const ValidatorParams* ValidatorParams);
bool IsRdPostEnumBusParamsValid(cJSON* params,
                                const ValidatorParams* ValidatorParams);
bool IsRdChaCountParamsValid(cJSON* params,
                             const ValidatorParams* ValidatorParams);
bool IsValidHexString(char* str);
bool IsTelemetry_DiscoveryParamsValid(cJSON* params,
                                      const ValidatorParams* ValidatorParams);
bool IsRdAndConcatenateParamsValid(cJSON* params,
                                   const ValidatorParams* ValidatorParams);
bool IsRdGlobalVarsValid(cJSON* params, const ValidatorParams* ValidatorParams);
bool IsSaveStrVarsValid(cJSON* params, const ValidatorParams* ValidatorParams);

#endif // CRASHDUMP_VALIDATOR_H
