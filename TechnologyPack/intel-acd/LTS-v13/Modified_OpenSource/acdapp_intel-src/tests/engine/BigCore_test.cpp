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
#include "../mock/test_crashdump.hpp"

extern "C" {
#include <safe_mem_lib.h>

#include "engine/BigCore.h"
#include "engine/crashdump.h"
}

#include <chrono>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace std::chrono;
using namespace ::testing;
using ::testing::DoDefault;
using ::testing::Return;

void validateCrashlogVersionAndSize(cJSON* output, std::string expectedVersion,
                                    std::string expectedSize)
{
    cJSON* actual = cJSON_CreateObject();
    actual = cJSON_GetObjectItemCaseSensitive(output->child->child,
                                              "crashlog_version");
    ASSERT_NE(actual, nullptr);
    EXPECT_STREQ(actual->valuestring, expectedVersion.c_str());
    actual = cJSON_GetObjectItemCaseSensitive(output->child->child, "size");
    ASSERT_NE(actual, nullptr);
    EXPECT_STREQ(actual->valuestring, expectedSize.c_str());
}

TEST(BigCoreTest, logCrashdumpSPR_testRegDump)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;

    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
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

    uint8_t firstFrame[8] = {0x01, 0x90, 0x02, 0x04,
                             0xa8, 0x05, 0x00, 0x00}; // 0x05a804029001;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x04029001", "0x05a8");
}

TEST(BigCoreTest, logCrashdump_NULL_JsonPtr)
{
    int ret;
    RunTimeInfo runTimeInfo;
    TestCrashdump crashdump(cd_icx);
    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, NULL, runTimeInfo);
        EXPECT_EQ(ret, ACD_INVALID_OBJECT);
    }
}

TEST(BigCoreTest, logCrashdumpSPR_testRegDumpWithSqDump)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
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
    UTFile /= "tests/UnitTestFiles/ut_bigcore.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    // SQ size = 0x0300, Reg Dump size = 0x05a8
    uint8_t firstFrame[8] = {0x01, 0x90, 0x02, 0x04,
                             0xa8, 0x05, 0x00, 0x03}; // 0x030005a804029001;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x04029001", "0x030005a8");
}

TEST(BigCoreTest, logCrashdumpSPR_B0_testRegDump)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
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

    uint8_t firstFrame[8] = {0x02, 0x90, 0x02, 0x04,
                             0xa8, 0x05, 0x00, 0x00}; // 0x05a804029002;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x04029002", "0x05a8");
}

TEST(BigCoreTest, logCrashdumpSPR_B0_testRegDumpWithSqDump)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
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

    // SQ size = 0x0300, Reg Dump size = 0x05a8
    uint8_t firstFrame[8] = {0x02, 0x90, 0x02, 0x04,
                             0xa8, 0x05, 0x00, 0x03}; // 0x030005a804029002;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x04029002", "0x030005a8");
}

TEST(BigCoreTest, logCrashdumpSPR_C0_testRegDump)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
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

    uint8_t firstFrame[8] = {0x03, 0x90, 0x02, 0x04,
                             0xb0, 0x05, 0x00, 0x00}; // 0x05b004029003;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x04029003", "0x05b0");
}

TEST(BigCoreTest, logCrashdumpSPR_C0_testRegDumpWithSqDump)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
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

    // SQ size = 0x0300, Reg Dump size = 0x05b0
    uint8_t firstFrame[8] = {0x03, 0x90, 0x02, 0x04,
                             0xb0, 0x05, 0x00, 0x03}; // 0x030005b004029003;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x04029003", "0x030005b0");
}

TEST(BigCoreTest, logCrashdumpICX_testRegDump_ICX)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_icx,
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

    uint8_t firstFrame[8] = {0x02, 0xe0, 0x01, 0x04,
                             0xf8, 0x05, 0x00, 0x00}; // 0x05f80401e002;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x0401e002", "0x05f8");
}

TEST(BigCoreTest, logCrashdumpICX_testRegDumpWithSqDump_ICX)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_icx,
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

    // SQ size = 0x0200, Reg Dump size = 0x05a8
    uint8_t firstFrame[8] = {0x02, 0xe0, 0x01, 0x04,
                             0xf8, 0x05, 0x00, 0x02}; // 0x020005f80401e002;
    uint8_t allOtherFrames[8] = {0xab, 0xfa, 0xed, 0xfe,
                                 0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;

    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(
            DoAll(SetArrayArgument<5>(allOtherFrames, allOtherFrames + 8),
                  SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)));

    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    validateCrashlogVersionAndSize(crashdump.root, "0x0401e002", "0x020005f8");
}

TEST(BigCoreTest, getTotalInputRegsSize_incorrectVersion_SPR)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], 0xff);
    EXPECT_EQ(size, ACD_INVALID_OBJECT);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSize_SPR)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t version_spr = 0x4029001;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);

    EXPECT_EQ(size, (uint32_t)0x5A8);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSizeOptimizedReturn_SPR)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t version_spr = 0x4029001;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);

    EXPECT_EQ(size, (uint32_t)0x5A8);

    size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);
    EXPECT_EQ(size, (uint32_t)0x5A8);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSize_SPR_B0)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t version_spr = 0x4029002;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);

    EXPECT_EQ(size, (uint32_t)0x5A8);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSizeOptimizedReturn_SPR_B0)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t version_spr = 0x4029002;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);

    EXPECT_EQ(size, (uint32_t)0x5A8);

    size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);
    EXPECT_EQ(size, (uint32_t)0x5A8);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSize_SPR_C0)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t version_spr = 0x4029003;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);

    EXPECT_EQ(size, (uint32_t)0x5B0);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSizeOptimizedReturn_SPR_C0)
{
    TestCrashdump crashdump(cd_spr);
    uint32_t version_spr = 0x4029003;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);

    EXPECT_EQ(size, (uint32_t)0x5B0);

    size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_spr);
    EXPECT_EQ(size, (uint32_t)0x5B0);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSize_ICX)
{
    TestCrashdump crashdump(cd_icx);
    uint32_t version_icx = 0x401e002;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_icx);

    EXPECT_EQ(size, (uint32_t)0x5f8);
}

TEST(BigCoreTest, getTotalInputRegsSize_correctSizeOptimizedReturn_ICX)
{
    TestCrashdump crashdump(cd_icx);
    uint32_t version_icx = 0x401e002;
    uint32_t size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_icx);

    EXPECT_EQ(size, (uint32_t)0x5f8);

    size = getTotalInputRegsSize(&crashdump.cpusInfo[0], version_icx);
    EXPECT_EQ(size, (uint32_t)0x5f8);
}

TEST(BigCoreTest, logCrashdumpSPR_C0_testRegDumpWithSqDump_withDelay)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    TestCrashdump crashdump(cd_spr, 700);
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;

    crashdump.libPeciMock->DelegateForBigCore();

    // SQ size = 0x0300, Reg Dump size = 0x05b0
    uint8_t firstFrame[8] = {0x03, 0x90, 0x02, 0x04,
                             0xb0, 0x05, 0x00, 0x03}; // 0x030005b004029003;
    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(DoDefault());

    std::chrono::steady_clock::time_point startTime =
        std::chrono::steady_clock::now();
    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    std::chrono::steady_clock::time_point endTime =
        std::chrono::steady_clock::now();
    ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                    startTime)
                  .count(),
              8400); // 8400 ==> 11(getframe calls) * 700ms
    validateCrashlogVersionAndSize(crashdump.root, "0x04029003", "0x030005b0");
}

TEST(BigCoreTest, logCrashdumpICX_testRegDumpWithSqDump_withDelay)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
    TestCrashdump crashdump(cd_icx, 700);

    crashdump.libPeciMock->DelegateForBigCore();

    // SQ size = 0x0200, Reg Dump size = 0x05a8
    uint8_t firstFrame[8] = {0x02, 0xe0, 0x01, 0x04,
                             0xf8, 0x05, 0x00, 0x02}; // 0x020005f80401e002;
    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(DoDefault());

    std::chrono::steady_clock::time_point startTime =
        std::chrono::steady_clock::now();
    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    std::chrono::steady_clock::time_point endTime =
        std::chrono::steady_clock::now();
    ASSERT_GE(std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                    startTime)
                  .count(),
              8400); // 8400 ==> 11(getframe calls) * 700ms
    validateCrashlogVersionAndSize(crashdump.root, "0x0401e002", "0x020005f8");
}

TEST(BigCoreTest, logCrashdumpSPR_C0_testRegDumpWithSqDump_withDelay_PerfTest)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    TestCrashdump crashdump(cd_spr, 700);
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
    crashdump.libPeciMock->DelegateForBigCoreWithGoodReturnWithDelay();

    // SQ size = 0x0300, Reg Dump size = 0x05b0
    uint8_t firstFrame[8] = {0x03, 0x90, 0x02, 0x04,
                             0xb0, 0x05, 0x00, 0x03}; // 0x030005b004029003;
    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(DoDefault());

    std::chrono::steady_clock::time_point startTime =
        std::chrono::steady_clock::now();
    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    std::chrono::steady_clock::time_point endTime =
        std::chrono::steady_clock::now();

    ASSERT_LE(std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                    startTime)
                  .count(),
              151000); // 151000 = 151000ms(30sec input file max) + other
                       // processing time

    cJSON* actual = cJSON_CreateObject();
    actual = cJSON_GetObjectItemCaseSensitive(
        crashdump.root->child->next->child, "crashlog_version");
    ASSERT_NE(actual, nullptr);
    EXPECT_STREQ(actual->valuestring, "0x04029003");

    actual = cJSON_GetObjectItemCaseSensitive(
        crashdump.root->child->next->child, "size");
    ASSERT_NE(actual, nullptr);
    EXPECT_STREQ(actual->valuestring, "0x030005b0");

    actual = cJSON_GetObjectItemCaseSensitive(crashdump.root, CD_ABORT_MSG_KEY);
    ASSERT_NE(actual, nullptr);
    EXPECT_THAT(actual->valuestring, HasSubstr("Max time 150 sec exceeded"));
}

TEST(BigCoreTest, logCrashdumpICX_testRegDumpWithSqDump_withDelay_PerfTest)
{
    int ret;
    uint8_t cc = PECI_DEV_CC_SUCCESS;
    RunTimeInfo runTimeInfo;
    TestCrashdump crashdump(cd_icx, 700);
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.maxGlobalTime = 700;
    crashdump.libPeciMock->DelegateForBigCoreWithGoodReturnWithDelay();

    // SQ size = 0x0200, Reg Dump size = 0x05a8
    uint8_t firstFrame[8] = {0x02, 0xe0, 0x01, 0x04,
                             0xf8, 0x05, 0x00, 0x02}; // 0x020005f80401e002;
    EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
        .WillOnce(DoAll(SetArrayArgument<5>(firstFrame, firstFrame + 8),
                        SetArgPointee<6>(cc), Return(PECI_CC_SUCCESS)))
        .WillRepeatedly(DoDefault());

    std::chrono::steady_clock::time_point startTime =
        std::chrono::steady_clock::now();
    for (auto cpuinfo : crashdump.cpusInfo)
    {
        ret = logBigCoreSection(&cpuinfo, crashdump.root, runTimeInfo);
        EXPECT_EQ(ret, PECI_CC_SUCCESS);
    }

    std::chrono::steady_clock::time_point endTime =
        std::chrono::steady_clock::now();

    // 151000 = 150000ms(150sec input file max) + other processing time
    ASSERT_LE(std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                    startTime)
                  .count(),
              151000);

    cJSON* actual = cJSON_CreateObject();
    actual = cJSON_GetObjectItemCaseSensitive(
        crashdump.root->child->next->child, "crashlog_version");
    ASSERT_NE(actual, nullptr);
    EXPECT_STREQ(actual->valuestring, "0x0401e002");

    actual = cJSON_GetObjectItemCaseSensitive(
        crashdump.root->child->next->child, "size");
    ASSERT_NE(actual, nullptr);
    EXPECT_STREQ(actual->valuestring, "0x020005f8");

    actual = cJSON_GetObjectItemCaseSensitive(crashdump.root, CD_ABORT_MSG_KEY);
    ASSERT_NE(actual, nullptr);
    EXPECT_THAT(actual->valuestring, HasSubstr("Max time 150 sec exceeded"));
}
