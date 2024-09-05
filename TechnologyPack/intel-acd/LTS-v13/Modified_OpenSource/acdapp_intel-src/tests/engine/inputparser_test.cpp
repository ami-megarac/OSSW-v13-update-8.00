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

extern "C" {
#include "engine/crashdump.h"
#include "engine/inputparser.h"
#include "engine/utils.h"
}

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using ::testing::Return;

class InputParserTestFixture : public ::testing::Test
{
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
        cJSON_Delete(params);
    }

  public:
    cJSON* params = NULL;
    InputParserErrInfo errInfo = {};
    acdStatus status;
    std::vector<CPUInfo> cpusInfo;
};

TEST_F(InputParserTestFixture, InputParserUpdateParams)
{
    TestCrashdump crashdump(cpusInfo);

    CmdInOut cmdInOut = {};
    InputParserErrInfo errInfo = {};
    LoggerStruct loggerStruct;

    cmdInOut.internalVarsTracker = cJSON_Parse(R"({"PostEnumBus": "0x7e"})");
    cmdInOut.internalVarName = "PostEnumBus";
    cmdInOut.in.params = cJSON_Parse(
        R"(["Target", 0, 0, "PostEnumBus", 26, 0, "0x201A4", 6, 4])");

    status = UpdateParams(&cpusInfo[0], &cmdInOut, &loggerStruct, &errInfo);
    EXPECT_EQ(ACD_SUCCESS, status);

    cmdInOut.in.params = cJSON_Parse(
        R"(["Target", 0, 0, "PostEnumBusInvalid", 26, 0, "0x201A4", 6, 4])");
    status = UpdateParams(&cpusInfo[0], &cmdInOut, &loggerStruct, &errInfo);
    EXPECT_EQ(ACD_SUCCESS, status);
}

TEST_F(InputParserTestFixture, InputParserSingle)
{
    params = cJSON_Parse(R"(["Target", 0, 0, "0x10"])");
    CmdInOut cmdInOut = {};
    LoggerStruct loggerStruct;
    cmdInOut.paramsTracker = cJSON_CreateObject();
    status = UpdateParams(&cpusInfo[0], &cmdInOut, &loggerStruct, &errInfo);
    EXPECT_EQ(ACD_SUCCESS, status);

    // "Target" should be updated and matching clientAddr
    if (cJSON_GetArraySize(cmdInOut.paramsTracker) != 0)
    {
        int newVal = cJSON_GetArrayItem(params, 0)->valueint;
        EXPECT_EQ(newVal, cpusInfo[0].clientAddr);
    }
    cJSON_Delete(cmdInOut.paramsTracker);
}

TEST_F(InputParserTestFixture, InputParserMulti)
{
    params = cJSON_Parse(R"(["Target", 0, 0, "0x10"])");
    LoggerStruct loggerStruct;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        CmdInOut cmdInOut = {};
        cmdInOut.paramsTracker = cJSON_CreateObject();
        status = UpdateParams(&cpusInfo[0], &cmdInOut, &loggerStruct, &errInfo);
        EXPECT_EQ(ACD_SUCCESS, status);

        if (cJSON_GetArraySize(cmdInOut.paramsTracker) != 0)
        {
            int newVal = cJSON_GetArrayItem(params, 0)->valueint;
            EXPECT_EQ(newVal, cpusInfo[cpu].clientAddr);
        }

        // Reset params for next cpuInfo
        status = ResetParams(params, cmdInOut.paramsTracker);
        EXPECT_EQ(ACD_SUCCESS, status);
        cJSON_Delete(cmdInOut.paramsTracker);
    }
}

TEST_F(InputParserTestFixture, InputParserSingleInvalidTarget)
{
    CmdInOut cmdInOut = {};
    LoggerStruct loggerStruct;
    cmdInOut.in.params = cJSON_Parse(R"(["TargetInvalid", 0, 0, "0x10"])");
    status = UpdateParams(&cpusInfo[0], &cmdInOut, &loggerStruct, &errInfo);
    EXPECT_EQ(ACD_SUCCESS, status);
}

TEST_F(InputParserTestFixture, InputParserSingleInvalidHexString)
{
    CmdInOut cmdInOut = {};
    LoggerStruct loggerStruct;
    cmdInOut.in.params = cJSON_Parse(R"(["Target", 0, 0, "0x10Invalid"])");
    status = UpdateParams(&cpusInfo[0], &cmdInOut, &loggerStruct, &errInfo);
    EXPECT_EQ(ACD_SUCCESS, status);
}
