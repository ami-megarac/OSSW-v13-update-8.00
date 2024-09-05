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

#include "../mock/test_crashdump.hpp"
#include "../test_utils.hpp"

extern "C" {
#include <search.h>

#include "engine/cmdprocessor.h"
#include "engine/crashdump.h"
#include "engine/flow.h"
#include "engine/inputparser.h"
#include "engine/logger.h"
#include "engine/utils.h"
}

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using std::filesystem::current_path;
using ::testing::Return;
#include <fstream>
namespace fs = std::filesystem;

class ProcessCmdTestFixture : public ::testing::Test
{
  protected:
    static cJSON* inRoot;
    static fs::path UTFile;
    static void readUncoreSampleFile()
    {
        UTFile = fs::current_path();
        UTFile = UTFile.parent_path();
        UTFile /= "tests/UnitTestFiles/ut_uncore.json";
        inRoot = readInputFile(UTFile.c_str());
        if (inRoot == NULL)
        {
            EXPECT_TRUE(inRoot != NULL);
        }
    }
    static void SetUpTestSuite()
    {
        readUncoreSampleFile();
    }

    static void TearDownTestSuite()
    {
        cJSON_Delete(inRoot);
    }

    void SetUp() override
    {
        // Build a list of cpus
        CPUInfo cpuInfo = {};
        cpuInfo.model = cd_spr;
        cpuInfo.clientAddr = 0x30;
        cpusInfo.push_back(cpuInfo);
        cpuInfo.model = cd_spr;
        cpuInfo.clientAddr = 0x31;
        cpusInfo.push_back(cpuInfo);
    }

    void TearDown() override
    {
        FREE(jsonStr);
        cJSON_Delete(outRoot);
    }

  public:
    cJSON* outRoot = cJSON_CreateObject();
    char* jsonStr = NULL;
    uint8_t cc = 0;
    bool enable = false;
    acdStatus status;
    std::vector<CPUInfo> cpusInfo;
};

fs::path ProcessCmdTestFixture::UTFile;
cJSON* ProcessCmdTestFixture::inRoot;

TEST_F(ProcessCmdTestFixture, SimpleCmdPassFailTest)
{
    TestCrashdump crashdump(cpusInfo);

    // Build RdIAMSR params in cJSON form
    cJSON* params = cJSON_CreateObject();
    int paramsRdIAMSR[] = {0x30, 0x0, 0x435};
    cJSON_AddItemToObject(params, "Params",
                          cJSON_CreateIntArray(paramsRdIAMSR, 3));
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
        .Times(1)
        .WillRepeatedly(DoAll(SetArgPointee<3>(0x1122334455667788),
                              SetArgPointee<4>(cc), Return(PECI_CC_SUCCESS)));

    ENTRY entry;
    CmdInOut cmdInOut = {};
    cmdInOut.out.ret = PECI_CC_INVALID_REQ;
    cmdInOut.in.params = cJSON_GetObjectItem(params, "Params");
    cmdInOut.internalVarName = "UINT64Var";

    status = BuildCmdsTable(&entry);
    EXPECT_EQ(ACD_SUCCESS, status);

    entry.key = "RdIAMSR";
    cmdInOut.paramsTracker = cJSON_CreateObject();
    cmdInOut.internalVarsTracker = cJSON_CreateObject();
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_SUCCESS, status);
    EXPECT_EQ(cmdInOut.out.ret, PECI_CC_SUCCESS);
    EXPECT_EQ(cmdInOut.out.cc, cc);
    EXPECT_EQ((uint64_t)cmdInOut.out.val.u64, (uint64_t)0x1122334455667788);

    entry.key = "RdIAMSRInvalid";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_CMD, status);

    Logging(cmdInOut.internalVarsTracker, "internalVarsTracker:");
    cJSON* val = cJSON_GetObjectItem(cmdInOut.internalVarsTracker, "UINT64Var");
    EXPECT_STREQ(val->valuestring, "0x1122334455667788");
    hdestroy();
}

TEST_F(ProcessCmdTestFixture, cmdTestingPass)
{
    TestCrashdump crashdump(cpusInfo);
    static cJSON* inRoot;
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_cmdprocessor.json";
    inRoot = readInputFile(UTFile.c_str());
    RunTimeInfo runTimeInfo;
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].inputFile.bufferPtr = inRoot;
    struct timespec sectionStart;
    struct timespec globalStart;

    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;

    uint8_t numberOfSections = getNumberOfSections(&cpusInfo[0]);
    for (uint8_t section = 0; section < numberOfSections; section++)
    {
        for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
        {
            sleep(1);
            status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo,
                                    section);
        }
    }
    cJSON* testing = cJSON_GetObjectItem(outRoot, "testing");
    cJSON* CrashDump_Discovery =
        cJSON_GetObjectItem(testing, "CrashDump_Discovery");
    EXPECT_STREQ(CrashDump_Discovery->valuestring, "0x801c026");
    cJSON* CrashDump_GetFrame =
        cJSON_GetObjectItem(testing, "CrashDump_GetFrame");
    EXPECT_STREQ(CrashDump_GetFrame->valuestring, "0x801c026");
    cJSON* Ping = cJSON_GetObjectItem(testing, "Ping");
    EXPECT_STREQ(Ping->valuestring, "0x801c026");
    cJSON* GetCPUID = cJSON_GetObjectItem(testing, "GetCPUID");
    EXPECT_STREQ(GetCPUID->valuestring, "0x801c026");
    cJSON* RdIAMSR = cJSON_GetObjectItem(testing, "RdIAMSR");
    EXPECT_STREQ(RdIAMSR->valuestring, "0x801c026");
    cJSON* RdPkgConfig = cJSON_GetObjectItem(testing, "RdPkgConfig");
    EXPECT_STREQ(RdPkgConfig->valuestring, "0x801c026");
    cJSON* RdPkgConfigCore = cJSON_GetObjectItem(testing, "RdPkgConfigCore");
    EXPECT_STREQ(RdPkgConfigCore->valuestring, "0x801c026");
    cJSON* RdPostEnumBus = cJSON_GetObjectItem(testing, "RdPostEnumBus");
    EXPECT_STREQ(RdPostEnumBus->valuestring, "0x0");
    cJSON* RdChaCount = cJSON_GetObjectItem(testing, "RdChaCount");
    EXPECT_STREQ(RdChaCount->valuestring, "0x2");
    cJSON* Telemetry_Discovery =
        cJSON_GetObjectItem(testing, "Telemetry_Discovery");
    EXPECT_STREQ(Telemetry_Discovery->valuestring, "0x2");
    cJSON* RdAndConcatenate = cJSON_GetObjectItem(testing, "RdAndConcatenate");
    EXPECT_STREQ(RdAndConcatenate->valuestring, "0x0");
    cJSON* peci_id = cJSON_GetObjectItem(testing, "peci_id");
    EXPECT_STREQ(peci_id->valuestring, "0x30");
    cJSON* cpuidjson = cJSON_GetObjectItem(testing, "cpuid");
    EXPECT_STREQ(cpuidjson->valuestring, "0x0");
    cJSON* SaveStrVars = cJSON_GetObjectItem(testing, "SaveStrVars");
    EXPECT_STREQ(SaveStrVars->valuestring, "corecount");
}

TEST_F(ProcessCmdTestFixture, cmdBadParams)
{
    TestCrashdump crashdump(cpusInfo);

    // Build RdIAMSR params in cJSON form
    cJSON* params = cJSON_CreateObject();
    int paramsRdIAMSR[] = {0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    cJSON_AddItemToObject(params, "Params",
                          cJSON_CreateIntArray(paramsRdIAMSR, 10));
    ENTRY entry;
    CmdInOut cmdInOut = {};
    ValidatorParams val;
    val.validateInput = true;
    cmdInOut.validatorParams = &val;
    cmdInOut.in.params = cJSON_GetObjectItem(params, "Params");
    status = BuildCmdsTable(&entry);
    entry.key = "CrashDump_Discovery";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_CRASHDUMP_DISCOVERY_PARAMS, status);
    entry.key = "CrashDump_GetFrame";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_CRASHDUMP_GETFRAME_PARAMS, status);
    entry.key = "Ping";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_PING_PARAMS, status);
    entry.key = "GetCPUID";
    cmdInOut.validatorParams->validateInput = true;
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_GETCPUID_PARAMS, status);
    entry.key = "RdIAMSR";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDIAMSR_PARAMS, status);
    entry.key = "RdPkgConfig";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDPKGCONFIG_PARAMS, status);
    entry.key = "RdPkgConfigCore";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDPKGCONFIGCORE_PARAMS, status);
    entry.key = "RdPCIConfigLocal";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDPCICONFIGLOCAL_PARAMS, status);
    entry.key = "RdEndpointConfigPCILocal";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDENDPOINTCONFIGPCILOCAL_PARAMS, status);
    entry.key = "WrEndPointConfigPciLocal";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_WRENDPOINTCONFIGPCILOCAL_PARAMS, status);
    entry.key = "RdEndpointConfigMMIO";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDENDPOINTCONFIGMMIO_PARAMS, status);
    entry.key = "RdPostEnumBus";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDPOSTENUMBUS_PARAMS, status);
    entry.key = "RdChaCount";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RDCHACOUNT_PARAMS, status);
    entry.key = "Telemetry_Discovery";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_TELEMETRY_DISCOVERY_PARAMS, status);
    entry.key = "RdAndConcatenate";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_RD_CONCATENATE_PARAMS, status);
    entry.key = "UnknownCommand";
    status = Execute(&entry, &cmdInOut);
    EXPECT_EQ(ACD_INVALID_CMD, status);
    hdestroy();
}