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

#include <filesystem>
#include <fstream>

extern "C" {
#include "engine/Crashlog.h"
#include "engine/CrashlogInternal.h"
#include "engine/base64Encode.h"
#include "engine/crashdump.h"
}

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using ::testing::Return;

TEST(CrashlogTest, Crashlog_TelemetrySupported)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);
    static cJSON* inRoot;
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_crashlog.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    bool supported = false;
    uint8_t returnValue[1] = {0x01}; // supported
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    supported = isTelemetrySupported(&cpuInfo);
    EXPECT_EQ(supported, true);
}

TEST(CrashlogTest, Crashlog_TelemetryNotSupported)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    bool supported = false;
    uint8_t returnValue[1] = {0x00}; // not supported
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    supported = isTelemetrySupported(&cpuInfo);
    EXPECT_EQ(supported, false);
}

TEST(CrashlogTest, Crashlog_TelemetryErrorReturn)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    bool supported = false;
    uint8_t returnValue[1] = {0x01}; // supported
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc),
                              Return(PECI_CC_DRIVER_ERR)));

    supported = isTelemetrySupported(&cpuInfo);
    EXPECT_EQ(supported, false);
}

TEST(CrashlogTest, Crashlog_TelemetryErrorCC)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    bool supported = false;
    uint8_t returnValue[1] = {0x01}; // supported
    uint8_t cc = 0x93;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    supported = isTelemetrySupported(&cpuInfo);
    EXPECT_EQ(supported, false);
}

TEST(CrashlogTest, Crashlog_getNumberOfCrashlogAgents_ErrorReturn)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t crashlogAgents = NO_CRASHLOG_AGENTS;
    uint8_t returnValue[1] = {0x01};
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc),
                              Return(PECI_CC_DRIVER_ERR)));

    crashlogAgents = getNumberOfCrashlogAgents(&cpuInfo);
    EXPECT_EQ(crashlogAgents, NO_CRASHLOG_AGENTS);
}

TEST(CrashlogTest, Crashlog_getNumberOfCrashlogAgents_ErrorCC)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t crashlogAgents = NO_CRASHLOG_AGENTS;
    uint8_t returnValue[1] = {0x01};
    uint8_t cc = 0x93;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    crashlogAgents = getNumberOfCrashlogAgents(&cpuInfo);
    EXPECT_EQ(crashlogAgents, NO_CRASHLOG_AGENTS);
}

TEST(CrashlogTest, Crashlog_getNumberOfCrashlogAgents)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t crashlogAgents = NO_CRASHLOG_AGENTS;
    uint8_t returnValue[1] = {0x03};
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 1),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    crashlogAgents = getNumberOfCrashlogAgents(&cpuInfo);
    EXPECT_EQ(crashlogAgents, 3);
}

TEST(CrashlogTest, Crashlog_getCrashlogSizeForPMCAgent_ErrorReturn)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 8),
                              SetArgPointee<7>(cc),
                              Return(PECI_CC_DRIVER_ERR)));

    crashlogAgentDetails agentDetails = getCrashlogDetailsForAgent(&cpuInfo, 0);
    EXPECT_EQ(agentDetails.crashSpace, 0);
}

TEST(CrashlogTest, Crashlog_getCrashlogSizeForPMCAgent_ErrorCC)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t cc = 0x93;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 8),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    crashlogAgentDetails agentDetails = getCrashlogDetailsForAgent(&cpuInfo, 0);
    EXPECT_EQ(agentDetails.crashSpace, 0);
}

TEST(CrashlogTest, Crashlog_getCrashlogSizeForPMCAgent)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[8] = {0x66, 0x55, 0x44, 0x33,
                              0x22, 0x11, 0xff, 0xee}; // 0xeeff112233445566
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnValue, returnValue + 8),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    crashlogAgentDetails agentDetails = getCrashlogDetailsForAgent(&cpuInfo, 0);
    EXPECT_EQ(agentDetails.crashSpace, 0xeeff);
}

TEST(CrashlogTest, Crashlog_collectCrashlogForAgent_PointerError)
{
    CPUInfo cpuInfo = {};
    crashlogAgentDetails agentDetails = {};
    uint8_t agent = 0;
    RunTimeInfo runTimeInfo;
    acdStatus status = collectCrashlogForAgent(&cpuInfo, agent, &agentDetails,
                                               NULL, runTimeInfo);

    EXPECT_EQ(status, ACD_INVALID_OBJECT);
}

TEST(CrashlogTest, Crashlog_collectCrashlogForAgent_PointerError2)
{
    crashlogAgentDetails agentDetails = {};
    uint64_t* rawCrashlog = (uint64_t*)(calloc(1, sizeof(uint32_t)));
    uint8_t agent = 0;
    RunTimeInfo runTimeInfo;
    acdStatus status = collectCrashlogForAgent(NULL, agent, &agentDetails,
                                               rawCrashlog, runTimeInfo);

    EXPECT_EQ(status, ACD_INVALID_OBJECT);
}

TEST(CrashlogTest, Crashlog_collectCrashlogForAgent_PointerError3)
{
    CPUInfo cpuInfo = {};
    uint64_t* rawCrashlog = (uint64_t*)(calloc(1, sizeof(uint32_t)));

    uint8_t agent = 0;
    RunTimeInfo runTimeInfo;
    acdStatus status = collectCrashlogForAgent(&cpuInfo, agent, NULL,
                                               rawCrashlog, runTimeInfo);

    EXPECT_EQ(status, ACD_INVALID_OBJECT);
}

TEST(CrashlogTest, Crashlog_collectCrashlogForAgent_ErrorReturn)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_GetCrashlogSample)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                              SetArgPointee<5>(cc),
                              Return(PECI_CC_DRIVER_ERR)));

    uint16_t crashlogSize = 2;
    uint64_t* rawCrashlog = (uint64_t*)(calloc(crashlogSize, sizeof(uint32_t)));
    crashlogAgentDetails agentDetails = {.entryType = 0x01,
                                         .crashType = 0x02,
                                         .uniqueId = 0x284b9c1f,
                                         .crashSpace = crashlogSize};

    uint8_t agent = 0;
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 10;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;

    acdStatus ret = collectCrashlogForAgent(&cpuInfo, agent, &agentDetails,
                                            rawCrashlog, runTimeInfo);
    EXPECT_EQ(ret, ACD_FAILURE);
}

TEST(CrashlogTest, Crashlog_collectCrashlogForAgent_ErrorCC)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    uint8_t cc = 0x93;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_GetCrashlogSample)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    uint16_t crashlogSize = 2;
    uint64_t* rawCrashlog = (uint64_t*)(calloc(crashlogSize, sizeof(uint32_t)));
    crashlogAgentDetails agentDetails = {.entryType = 0x01,
                                         .crashType = 0x02,
                                         .uniqueId = 0x284b9c1f,
                                         .crashSpace = crashlogSize};

    uint8_t agent = 0;
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 10;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    acdStatus ret = collectCrashlogForAgent(&cpuInfo, agent, &agentDetails,
                                            rawCrashlog, runTimeInfo);
    EXPECT_EQ(ret, ACD_FAILURE);
}

TEST(CrashlogTest, Crashlog_collectCrashlogForAgent)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[8] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    uint8_t returnValue1[8] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
    uint8_t returnValue2[8] = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33};

    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_GetCrashlogSample)
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue1, returnValue1 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue2, returnValue2 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    uint16_t crashlogSize = 0x6;
    uint64_t* rawCrashlog = (uint64_t*)(calloc(crashlogSize, sizeof(uint32_t)));
    crashlogAgentDetails agentDetails = {.entryType = 0x01,
                                         .crashType = 0x02,
                                         .uniqueId = 0x284b9c1f,
                                         .crashSpace = crashlogSize};

    uint8_t agent = 0;
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 10;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    collectCrashlogForAgent(&cpuInfo, agent, &agentDetails, rawCrashlog,
                            runTimeInfo);

    uint64_t* expected = (uint64_t*)returnValue;
    EXPECT_TRUE(std::is_permutation(rawCrashlog, rawCrashlog + 1, expected));

    uint64_t* expected1 = (uint64_t*)returnValue1;
    EXPECT_TRUE(
        std::is_permutation(rawCrashlog + 1, rawCrashlog + 2, expected1));

    uint64_t* expected2 = (uint64_t*)returnValue2;
    EXPECT_TRUE(
        std::is_permutation(rawCrashlog + 2, rawCrashlog + 3, expected2));
}

TEST(CrashlogTest, Crashlog_storeCrashlog)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnValue[24] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                               0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                               0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};

    uint16_t crashlogSize = 0x6;
    uint64_t* rawCrashlog = (uint64_t*)(returnValue);
    crashlogAgentDetails agentDetails = {.entryType = 0x01,
                                         .crashType = 0x02,
                                         .uniqueId = 0x284b9c1f,
                                         .crashSpace = crashlogSize};

    guidCrashlogSectionMapping agentMap = {0x284b9c1f, "PMC_TRACE_INDEX", true};
    agentsInfoInputFile agentsInfo = {&agentMap, 1};

    uint8_t agent = 5;
    acdStatus ret = storeCrashlog(crashdump.root, agent, &agentDetails,
                                  rawCrashlog, &agentsInfo);
    EXPECT_EQ(ret, ACD_SUCCESS);
    EXPECT_STREQ(crashdump.root->child->string, "agent_id_0x284b9c1f");
    EXPECT_STREQ(crashdump.root->child->child->string, "#data_PMC_TRACE_INDEX");
    EXPECT_STREQ(crashdump.root->child->child->valuestring,
                 "AAECAwQFBgcQERITFBUWFyAhIiMkJSYn");
}

TEST(CrashlogTest, base64Encode_base64Encode_1Byte)
{
    uint8_t src[1] = {0x41};
    char encodedString[4];
    uint64_t ret = base64Encode(src, sizeof(src), encodedString);
    EXPECT_EQ(ret, (uint64_t)0);
    EXPECT_STREQ(encodedString, "QQ==");
}

TEST(CrashlogTest, base64Encode_base64Encode_2Byte)
{
    uint8_t src[2] = {0x41, 0x7A};
    char encodedString[4] = {0, 0, 0, 0};
    uint64_t ret = base64Encode(src, sizeof(src), encodedString);
    EXPECT_EQ(ret, (uint64_t)0);
    EXPECT_STREQ(encodedString, "QXo=");
}

TEST(CrashlogTest, base64Encode_base64Encode_3Byte)
{
    uint8_t src[3] = {0x41, 0x7A, 0x35};
    char encodedString[4];
    uint64_t ret = base64Encode(src, sizeof(src), encodedString);
    EXPECT_EQ(ret, (uint64_t)0);
    EXPECT_STREQ(encodedString, "QXo1");
}

TEST(CrashlogTest, base64Encode_base64Encode_4Byte)
{
    uint8_t src[4] = {0x41, 0x7A, 0x7B, 0x2B};
    char encodedString[8];
    uint64_t ret = base64Encode(src, sizeof(src), encodedString);
    EXPECT_EQ(ret, (uint64_t)0);
    EXPECT_STREQ(encodedString, "QXp7Kw==");
}

TEST(CrashlogTest, base64Encode_base64Encode_CrashlogFile)
{
    std::ostringstream buf;
    std::filesystem::path path = std::filesystem::current_path() / ".." /
                                 "tests" / "UnitTestFiles" /
                                 "crashlog_binary_sample";
    std::ifstream input(path.string(), std::ios::binary);
    buf << input.rdbuf();
    std::string binary_file = buf.str();
    char encodedString[10000];

    uint64_t ret = base64Encode((uint8_t*)binary_file.c_str(),
                                binary_file.length(), encodedString);
    EXPECT_EQ(ret, (uint64_t)0);
    EXPECT_STREQ(
        encodedString,
        "AtMAAQADAADdCliuAgAAAAYAAAAIAAAA//9/gIAwAAAAAAAAAAAAAAAAACIAA/"
        "4RfgAAAAAAAAAAAQEAAAAAAAAAAAAAAAACACcAAQAgATMgAAAAQQAAAAAAAAAAAAAAAAIA"
        "AMcE7++"
        "A8f9zaAqfAOkBIGAAEQAAIAAAAAAAAAAAAAQAAACAKFAAGs8OAF0DBAMAAAgAAAAAAAAAE"
        "AEBAAERjgqY4QAAAAAMALUPNwIAAACwXwIB2BgAIAAAAAAAAAAnAAAAAAAADCCAAAAAAAA"
        "AAAAAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAg8Fo4CAYQP/wBKPb+uBcHygdev///"
        "KPB8EAIIB1CJAwDwWjgAAAAGAAAAAAAAAAAAAAAAAAAAACjwWrgIB1aJBwAAAAAACAAAAD"
        "AAAAwBAAEAAEAAADAABgAAAAAAAABCAwC4FAEKAAKACgAKiACAAAoQEAAUAQAAAAAAMnME"
        "gIuIAEAIAABAGAAAAAAKgQBkAwAJAQBgPT0AAAAPAAACAAZAAAAAAAAAAAAAAI8GAAEEAA"
        "AAAAAJAAAAgAAAAAAYElMAAHic4P///wCQAgAAAAAAAMMOODMAAABBIAABANg+AgD/"
        "CABOMQAwADkAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAA"
        "AAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAA"
        "AAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAA"
        "AAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAf"
        "AAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAACAAAAAAAAAAAA"
        "AAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAA"
        "AAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAA"
        "AAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAf"
        "AAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAfAAAAAAAAAAAAABAAAAAAAAAAAAAAAA"
        "AAAAAAAAAfAAAAAAAAAAAAAIARAAAAAIAABAD/"
        "YAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAA");
}

TEST(CrashlogTest, Crashlog_logError)
{
    cJSON* root = cJSON_CreateObject();
    logError(root, "this is error check");
    EXPECT_STREQ(root->child->string, "_error");
    EXPECT_STREQ(root->child->valuestring, "this is error check");
}

TEST(CrashlogTest, Crashlog_logCrashlogEBG)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    uint8_t returnCrashlogDetails0[8] = {
        0x66, 0x55, 0x36, 0x0b,
        0xba, 0x84, 0x02, 0x00}; // size = 0x02,agent_id = 0x84ba0b36,
    uint8_t returnCrashlogDetails1[8] = {
        0x66, 0x55, 0x3f, 0xf4,
        0x56, 0x99, 0x02, 0x0}; // size = 0x02,agent_id = 0x9956f43f,
    uint8_t returnCrashlogDetails2[8] = {
        0x66, 0x55, 0x09, 0x3b,
        0x32, 0xd5, 0x02, 0x00}; // size = 0x02,agent_id = 0xd5323b09,
    uint8_t returnCrashlogDetails3[8] = {
        0x66, 0x55, 0x78, 0x14,
        0x6d, 0xf2, 0x02, 0x00}; // size = 0x02,agent_id = 0xf26d1478,
    uint8_t returnCrashlogDetails4[8] = {
        0x66, 0x55, 0x90, 0x3c,
        0x8e, 0x25, 0x02, 0x00}; // size = 0x02,agent_id = 0x258e3c90,
    uint8_t returnCrashlogDetails5[8] = {
        0x66, 0x55, 0x9F, 0x06,
        0x99, 0x88, 0x02, 0x00}; // size = 0x02,agent_id = 0x8899069f,
    uint8_t returnCrashlogDetails6[8] = {
        0x66, 0x55, 0x1f, 0x9c,
        0x4b, 0x28, 0x02, 0x00}; // size = 0x02,agent_id = 0x284b9c1f,
    uint8_t returnCrashlogDetails7[8] = {
        0x66, 0x55, 0x20, 0xbf,
        0xc9, 0x57, 0xff, 0xff}; // size = 0x02,agent_id = 0x57c9bf20,
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails0,
                                            returnCrashlogDetails0 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails1,
                                            returnCrashlogDetails1 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails2,
                                            returnCrashlogDetails2 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails3,
                                            returnCrashlogDetails3 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails4,
                                            returnCrashlogDetails4 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails5,
                                            returnCrashlogDetails5 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails6,
                                            returnCrashlogDetails6 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails7,
                                            returnCrashlogDetails7 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    uint8_t returnValue[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    uint8_t returnValue1[8] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    uint8_t returnValue2[8] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
    cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_GetCrashlogSample)
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue1, returnValue1 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue2, returnValue2 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue1, returnValue1 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue2, returnValue2 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue1, returnValue1 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<4>(returnValue2, returnValue2 + 8),
                        SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 10;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    for (auto cpuinfo : crashdump.cpusInfo)
    {
        acdStatus ret =
            logCrashlogEBG(&cpuinfo, crashdump.root, 8, runTimeInfo);
        EXPECT_EQ(ret, ACD_SUCCESS);
    }
    EXPECT_STREQ(crashdump.root->child->next->string, "agent_id_0x84ba0b36");
    EXPECT_STREQ(crashdump.root->child->next->child->string,
                 "#data_PCODE_CRASH_INDEX");
    EXPECT_STREQ(crashdump.root->child->next->child->valuestring,
                 "AAECAwQFBgc=");

    EXPECT_STREQ(crashdump.root->child->next->next->string,
                 "agent_id_0x9956f43f");
    EXPECT_STREQ(crashdump.root->child->next->next->child->string,
                 "#data_PUNIT_CRASH_INDEX");
    EXPECT_STREQ(crashdump.root->child->next->next->child->valuestring,
                 "EBESExQVFhc=");

    EXPECT_STREQ(crashdump.root->child->next->next->next->string,
                 "agent_id_0xd5323b09");
    EXPECT_STREQ(crashdump.root->child->next->next->next->child->string,
                 "#data_OOBMSM_CRASH_INDEX");
    EXPECT_STREQ(crashdump.root->child->next->next->next->child->valuestring,
                 "ICEiIyQlJic=");
}

TEST(CrashlogTest, Crashlog_isValidPCHAgent_AllValidGUIDs)
{
    CPUInfo cpuInfo = {};
    crashlogAgentDetails agentDetails[] = {{
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0x284b9c1f,
                                               .crashSpace = 0,
                                           },
                                           {
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0x8899069f,
                                               .crashSpace = 0,
                                           },
                                           {
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0x258e3c90,
                                               .crashSpace = 0,
                                           }};

    guidCrashlogSectionMapping agentMap[3] = {
        {0x284b9c1f, "PMC_TRACE_INDEX", true},
        {0x8899069f, "PMC_RESET_INDEX", true},
        {0x258e3c90, "PMC_CRASH_INDEX", true}};
    agentsInfoInputFile agentsInfo = {agentMap, 3};

    for (uint8_t i = 0; i < sizeof(agentDetails) / sizeof(*agentDetails); i++)
    {
        acdStatus status =
            isValidAgent(&cpuInfo, &agentDetails[i], &agentsInfo);
        EXPECT_EQ(status, ACD_SUCCESS);
    }
}

TEST(CrashlogTest, Crashlog_isValidPCHAgent_SomeValidSomeInValidGUIDs)
{
    CPUInfo cpuInfo = {};
    crashlogAgentDetails agentDetails[] = {{
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0xaabbccdd,
                                               .crashSpace = 0,
                                           },
                                           {
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0xdeadbeef,
                                               .crashSpace = 0,
                                           },
                                           {
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0x8899069f,
                                               .crashSpace = 0,
                                           },
                                           {
                                               .entryType = 0,
                                               .crashType = 0,
                                               .uniqueId = 0x258e3c90,
                                               .crashSpace = 0,
                                           }};

    guidCrashlogSectionMapping agentMap[2] = {
        {0x8899069f, "PMC_TRACE_INDEX", true},
        {0x258e3c90, "PMC_CRASH_INDEX", true}};
    agentsInfoInputFile agentsInfo = {agentMap, 2};

    for (uint8_t i = 0; i < sizeof(agentDetails) / sizeof(*agentDetails); i++)
    {
        acdStatus status =
            isValidAgent(&cpuInfo, &agentDetails[i], &agentsInfo);
        EXPECT_EQ(status, ACD_SUCCESS);
    }
}

TEST(CrashlogTest, Crashlog_ReArmReadErrorReturn)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    acdStatus error = ACD_SUCCESS;
    uint8_t returnValue[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc),
                              Return(PECI_CC_DRIVER_ERR)));

    error = initiateCrashlogTriggerRearm(&cpuInfo);
    EXPECT_EQ(error, ACD_FAILURE);
}

TEST(CrashlogTest, Crashlog_ReArmReadErrorCC)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    acdStatus error = ACD_SUCCESS;
    uint8_t returnValue[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t cc = PECI_CC_HW_ERR;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    error = initiateCrashlogTriggerRearm(&cpuInfo);
    EXPECT_EQ(error, ACD_FAILURE);
}

TEST(CrashlogTest, Crashlog_ReArmWriteErrorReturn)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    acdStatus error = ACD_SUCCESS;
    uint8_t returnValue[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc),
                              Return(PECI_CC_DRIVER_ERR)));

    error = initiateCrashlogTriggerRearm(&cpuInfo);
    EXPECT_EQ(error, ACD_FAILURE);
}

TEST(CrashlogTest, Crashlog_ReArmWriteErrorCC)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    acdStatus error = ACD_SUCCESS;
    uint8_t returnValue[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    cc = PECI_CC_HW_ERR;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    error = initiateCrashlogTriggerRearm(&cpuInfo);
    EXPECT_EQ(error, ACD_FAILURE);
}

TEST(CrashlogTest, Crashlog_ReArmReadWriteSuccess)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);

    acdStatus error = ACD_SUCCESS;
    uint64_t returnValue = 0x0102030405060768;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArgPointee<4>(returnValue),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    error = initiateCrashlogTriggerRearm(&cpuInfo);

    EXPECT_EQ(error, ACD_SUCCESS);
}

TEST(CrashlogTest, Crashlog_logCrashlogEBG_Section_timeout)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);
    uint8_t returnCrashlogDetails1[8] = {
        0x66, 0x55, 0x90, 0x3c,
        0x8e, 0x25, 0x06, 0x00}; // size = 0x06,agent_id = 0x258e3c90,
                                 // 0x0006112233445566
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails1,
                                            returnCrashlogDetails1 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    uint8_t returnValue[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    cc = 0x40;

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 10;
    runTimeInfo.maxSectionTime = 0;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    CPUInfo cpuinfo = crashdump.cpusInfo[0];
    acdStatus ret = logCrashlogEBG(&cpuinfo, crashdump.root, 8, runTimeInfo);
    EXPECT_EQ(ret, ACD_SECTION_TIMEOUT);
}

TEST(CrashlogTest, Crashlog_logCrashlogEBG_Global_timeout)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);
    uint8_t returnCrashlogDetails1[8] = {
        0x66, 0x55, 0x90, 0x3c,
        0x8e, 0x25, 0x06, 0x00}; // size = 0x06,agent_id = 0x258e3c90,
                                 // 0x0006112233445566
    uint8_t cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails1,
                                            returnCrashlogDetails1 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    uint8_t returnValue[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    cc = 0x40;

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 0;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    CPUInfo cpuinfo = crashdump.cpusInfo[0];
    acdStatus ret = logCrashlogEBG(&cpuinfo, crashdump.root, 8, runTimeInfo);
    EXPECT_EQ(ret, ACD_GLOBAL_TIMEOUT);
}

TEST(CrashlogTest, Crashlog_logCrashlogEBG_max_size)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);
    uint8_t cc = 0x40;
    uint8_t returnCrashlogDetails0[8] = {
        0x66, 0x55, 0x36, 0x0b,
        0xba, 0x84, 0xff, 0xff}; // size = 0x02,agent_id = 0x84ba0b36,
    uint8_t returnCrashlogDetails1[8] = {
        0x66, 0x55, 0x3f, 0xf4,
        0x56, 0x99, 0xff, 0xff}; // size = 0x02,agent_id = 0x9956f43f,
    uint8_t returnCrashlogDetails2[8] = {
        0x66, 0x55, 0x09, 0x3b,
        0x32, 0xd5, 0xff, 0xff}; // size = 0x02,agent_id = 0xd5323b09,
    uint8_t returnCrashlogDetails3[8] = {
        0x66, 0x55, 0x78, 0x14,
        0x6d, 0xf2, 0xff, 0xff}; // size = 0x02,agent_id = 0xf26d1478,
    uint8_t returnCrashlogDetails4[8] = {
        0x66, 0x55, 0x90, 0x3c,
        0x8e, 0x25, 0xff, 0xff}; // size = 0x02,agent_id = 0x258e3c90,
    uint8_t returnCrashlogDetails5[8] = {
        0x66, 0x55, 0x9F, 0x06,
        0x99, 0x88, 0xff, 0xff}; // size = 0x02,agent_id = 0x8899069f,
    uint8_t returnCrashlogDetails6[8] = {
        0x66, 0x55, 0x1f, 0x9c,
        0x4b, 0x28, 0x02, 0x00}; // size = 0x02,agent_id = 0x284b9c1f,
    uint8_t returnCrashlogDetails7[8] = {
        0x66, 0x55, 0x20, 0xbf,
        0xc9, 0x57, 0xff, 0xff}; // size = 0x02,agent_id = 0x57c9bf20,
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails0,
                                            returnCrashlogDetails0 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails1,
                                            returnCrashlogDetails1 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails2,
                                            returnCrashlogDetails2 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails3,
                                            returnCrashlogDetails3 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails4,
                                            returnCrashlogDetails4 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails5,
                                            returnCrashlogDetails5 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails6,
                                            returnCrashlogDetails6 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<6>(returnCrashlogDetails7,
                                            returnCrashlogDetails7 + 8),
                        SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    uint8_t returnValue[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_GetCrashlogSample)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 10;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    CPUInfo cpuinfo = crashdump.cpusInfo[0];
    acdStatus ret = logCrashlogEBG(&cpuinfo, crashdump.root, 8, runTimeInfo);
    EXPECT_EQ(ret, ACD_SUCCESS);
}

TEST(CrashlogTest, Crashlog_logCrashlogEBG_partial_global_timeout)
{
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};
    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);
    uint8_t cc = 0x40;
    uint8_t returnCrashlogDetails0[8] = {
        0x66, 0x55, 0x36, 0x0b,
        0xba, 0x84, 0xff, 0xff}; // size = 0x02,agent_id = 0x84ba0b36,

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_Discovery)
        .WillRepeatedly(DoAll(SetArrayArgument<6>(returnCrashlogDetails0,
                                                  returnCrashlogDetails0 + 8),
                              SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

    uint8_t returnValue[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    cc = 0x40;
    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_GetCrashlogSample)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 8),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherRd)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_Telemetry_ConfigWatcherWr)
        .WillRepeatedly(DoAll(SetArrayArgument<4>(returnValue, returnValue + 1),
                              SetArgPointee<5>(cc), Return(PECI_CC_SUCCESS)));
    RunTimeInfo runTimeInfo;
    struct timespec sectionStart;
    struct timespec globalStart;
    runTimeInfo.maxGlobalTime = 1;
    runTimeInfo.maxSectionTime = 10;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    CPUInfo cpuinfo = crashdump.cpusInfo[0];
    acdStatus ret = logCrashlogEBG(&cpuinfo, crashdump.root, 8, runTimeInfo);
    EXPECT_EQ(ret, ACD_GLOBAL_TIMEOUT);
}
