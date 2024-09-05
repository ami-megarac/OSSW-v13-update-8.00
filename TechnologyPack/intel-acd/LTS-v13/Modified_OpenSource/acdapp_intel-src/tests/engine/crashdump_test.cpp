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
#include "crashdump.hpp"

#include <fstream>
#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

extern "C" {
}

using namespace ::testing;
using namespace crashdump;

TEST(CrashdumpTest, test_isPECIAvailable)
{
    TestCrashdump crashdump(cd_spr);
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));

    EXPECT_TRUE(crashdump::isPECIAvailable());
}

TEST(CrashdumpTest, test_isPECIAvailable_NO_CPUS)
{
    TestCrashdump crashdump(cd_spr);
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillRepeatedly(DoAll(Return(PECI_CC_TIMEOUT)));
    EXPECT_FALSE(crashdump::isPECIAvailable());
}

TEST(CrashdumpTest, test_initCPUInfo)
{
    TestCrashdump crashdump(cd_spr);
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));
    std::vector<CPUInfo> cpusInfo;
    initCPUInfo(cpusInfo);
    EXPECT_EQ(cpusInfo[0].cpuidRead.source, 2);
    EXPECT_EQ(cpusInfo[0].coreMaskRead.source, 2);
    EXPECT_EQ(cpusInfo[0].chaCountRead.source, 2);
    EXPECT_FALSE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[0].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[0].chaCountRead.chaCountValid);
}

TEST(CrashdumpTest, test_getCPUData_Startup)
{
    TestCrashdump crashdump(cd_spr);
    std::vector<CPUInfo> cpusInfo = {};
    uint8_t coreMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t coreMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t chaMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t chaMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_CALL(*crashdump.libPeciMock, peci_GetCPUID)
        .WillOnce(DoAll(
            SetArgPointee<1>((CPUModel)0x000806F0), SetArgPointee<2>(1),
            SetArgPointee<3>(PECI_DEV_CC_SUCCESS), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskHigh, coreMaskHigh + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskLow, coreMaskLow + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArrayArgument<7>(chaMaskHigh, chaMaskHigh + 4), // cha mask 0
            SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(
            DoAll(SetArrayArgument<7>(chaMaskLow, chaMaskLow + 4), // cha mask 1
                  SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    size_t chaExpectedValue = 13;
    uint64_t coreExpectedValue = 0x403020108070605;
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, STARTUP);
    EXPECT_TRUE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_TRUE(cpusInfo[0].chaCountRead.chaCountValid);
    EXPECT_TRUE(cpusInfo[0].coreMaskRead.coreMaskValid);
    EXPECT_EQ(cpusInfo[0].cpuidRead.source, 1);
    EXPECT_EQ(cpusInfo[0].chaCountRead.source, 1);
    EXPECT_EQ(cpusInfo[0].coreMaskRead.source, 1);
    EXPECT_EQ(cpusInfo[0].cpuidRead.cpuModel, spr);
    EXPECT_EQ(cpusInfo[0].chaCount, chaExpectedValue);
    EXPECT_EQ(cpusInfo[0].coreMask, coreExpectedValue);
}

TEST(CrashdumpTest, test_getCPUData_Event)
{
    TestCrashdump crashdump(cd_spr);
    std::vector<CPUInfo> cpusInfo = {};
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));
    size_t chaExpectedValue = 13;
    uint64_t coreExpectedValue = 0x403020108070605;
    initCPUInfo(cpusInfo);
    cpusInfo[0].cpuidRead.cpuidValid = true;
    cpusInfo[0].chaCountRead.chaCountValid = true;
    cpusInfo[0].coreMaskRead.coreMaskValid = true;
    cpusInfo[0].cpuidRead.source = STARTUP;
    cpusInfo[0].chaCountRead.source = STARTUP;
    cpusInfo[0].coreMaskRead.source = STARTUP;
    cpusInfo[0].cpuidRead.cpuModel = spr;
    cpusInfo[0].chaCount = chaExpectedValue;
    cpusInfo[0].coreMask = coreExpectedValue;
    getCPUData(cpusInfo, EVENT);
    EXPECT_TRUE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_TRUE(cpusInfo[0].chaCountRead.chaCountValid);
    EXPECT_TRUE(cpusInfo[0].coreMaskRead.coreMaskValid);
    EXPECT_EQ(cpusInfo[0].cpuidRead.source, 1);
    EXPECT_EQ(cpusInfo[0].chaCountRead.source, 1);
    EXPECT_EQ(cpusInfo[0].coreMaskRead.source, 1);
    EXPECT_EQ(cpusInfo[0].cpuidRead.cpuModel, spr);
    EXPECT_EQ(cpusInfo[0].chaCount, chaExpectedValue);
    EXPECT_EQ(cpusInfo[0].coreMask, coreExpectedValue);
}

TEST(CrashdumpTest, test_getCPUData_overwrite)
{
    TestCrashdump crashdump(cd_spr);
    CPUInfo cpuInfo1 = {.clientAddr = 48,
                        .model = (Model)0x0,
                        .coreMask = 0x0,
                        .crashedCoreMask = 0x0,
                        .chaCount = 0,
                        .initialPeciWake = ON,
                        .inputFile = {},
                        .cpuidRead = {},
                        .chaCountRead = {},
                        .coreMaskRead = {},
                        .dimmMask = 0};

    CPUInfo cpuInfo2 = {.clientAddr = 49,
                        .model = (Model)0x0,
                        .coreMask = 0x0,
                        .crashedCoreMask = 0x0,
                        .chaCount = 0,
                        .initialPeciWake = ON,
                        .inputFile = {},
                        .cpuidRead = {},
                        .chaCountRead = {},
                        .coreMaskRead = {},
                        .dimmMask = 0};

    std::vector<CPUInfo> cpusInfo;
    cpusInfo.push_back(cpuInfo1);
    cpusInfo.push_back(cpuInfo2);
    uint8_t coreMaskHigh[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    // uint8_t coreMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t chaMaskHigh[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    // uint8_t chaMaskLow[4] = {0x01, 0x02, 0x03, 0x04};

    EXPECT_CALL(*crashdump.libPeciMock, peci_GetCPUID)
        .WillOnce(DoAll(SetArgPointee<1>((CPUModel)0x0), SetArgPointee<2>(1),
                        SetArgPointee<3>(PECI_DEV_CC_NEED_RETRY),
                        Return(PECI_CC_SUCCESS)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskHigh, coreMaskHigh + 4),
                        SetArgPointee<8>(PECI_DEV_CC_NEED_RETRY),
                        Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArrayArgument<7>(chaMaskHigh, chaMaskHigh + 4), // cha mask 0
            SetArgPointee<8>(PECI_DEV_CC_NEED_RETRY), Return(PECI_CC_SUCCESS)));
    size_t chaExpectedValue0 = 13;
    uint64_t coreExpectedValue0 = 0x403020108070605;
    size_t chaExpectedValue1 = 0;
    uint64_t coreExpectedValue1 = 0x0;
    cpusInfo[0].cpuidRead.cpuidValid = true;
    cpusInfo[0].chaCountRead.chaCountValid = true;
    cpusInfo[0].coreMaskRead.coreMaskValid = true;
    cpusInfo[0].model = cd_spr;
    cpusInfo[0].cpuidRead.cpuModel = spr;
    cpusInfo[1].cpuidRead.cpuidValid = false;
    cpusInfo[1].chaCountRead.chaCountValid = false;
    cpusInfo[1].coreMaskRead.coreMaskValid = false;
    cpusInfo[0].chaCount = chaExpectedValue0;
    cpusInfo[0].coreMask = coreExpectedValue0;
    getCPUData(cpusInfo, EVENT);
    EXPECT_TRUE(cpusInfo[1].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[1].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[1].coreMaskRead.coreMaskValid);
    EXPECT_EQ(cpusInfo[1].cpuidRead.source, 3);
    EXPECT_EQ(cpusInfo[1].cpuidRead.cpuModel, spr);
    EXPECT_EQ(cpusInfo[1].model, cd_spr);
    EXPECT_EQ(cpusInfo[1].chaCount, chaExpectedValue1);
    EXPECT_EQ(cpusInfo[1].coreMask, coreExpectedValue1);
}

TEST(CrashdumpTest, test_getCPUData_invalid)
{
    TestCrashdump crashdump(cd_spr);
    std::vector<CPUInfo> cpusInfo;

    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));

    EXPECT_CALL(*crashdump.libPeciMock, peci_GetCPUID)
        .WillOnce(DoAll(SetArgPointee<1>((CPUModel)0x0), SetArgPointee<2>(1),
                        SetArgPointee<3>(PECI_DEV_CC_NEED_RETRY),
                        Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArgPointee<1>((CPUModel)0x0), SetArgPointee<2>(1),
                        SetArgPointee<3>(PECI_DEV_CC_NEED_RETRY),
                        Return(PECI_CC_SUCCESS)));

    size_t chaExpectedValue0 = 0;
    uint64_t coreExpectedValue0 = 0x0;
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, EVENT);
    EXPECT_FALSE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[1].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[0].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[0].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[1].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[1].coreMaskRead.coreMaskValid);
    EXPECT_EQ(cpusInfo[0].cpuidRead.source, 2);
    EXPECT_EQ(cpusInfo[1].cpuidRead.source, 2);
    EXPECT_EQ(cpusInfo[0].chaCount, chaExpectedValue0);
    EXPECT_EQ(cpusInfo[0].coreMask, coreExpectedValue0);
    EXPECT_EQ(cpusInfo[1].chaCount, chaExpectedValue0);
    EXPECT_EQ(cpusInfo[1].coreMask, coreExpectedValue0);
}

TEST(CrashdumpTest, test_platformTurnOff)
{
    TestCrashdump crashdump(cd_spr);
    std::vector<CPUInfo> cpusInfo = {};
    uint8_t coreMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t coreMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t chaMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t chaMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_CALL(*crashdump.libPeciMock, peci_GetCPUID)
        .WillOnce(DoAll(
            SetArgPointee<1>((CPUModel)0x000806F0), SetArgPointee<2>(1),
            SetArgPointee<3>(PECI_DEV_CC_SUCCESS), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArgPointee<1>((CPUModel)0x000806F0), SetArgPointee<2>(1),
            SetArgPointee<3>(PECI_DEV_CC_SUCCESS), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskHigh, coreMaskHigh + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskLow, coreMaskLow + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArrayArgument<7>(chaMaskHigh, chaMaskHigh + 4), // cha mask 0
            SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(
            DoAll(SetArrayArgument<7>(chaMaskLow, chaMaskLow + 4), // cha mask 1
                  SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskHigh, coreMaskHigh + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskLow, coreMaskLow + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArrayArgument<7>(chaMaskHigh, chaMaskHigh + 4), // cha mask 0
            SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(
            DoAll(SetArrayArgument<7>(chaMaskLow, chaMaskLow + 4), // cha mask 1
                  SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, STARTUP);
    // create_crashdump
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, EVENT);
    EXPECT_TRUE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_TRUE(cpusInfo[1].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[2].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[3].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[4].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[5].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[6].cpuidRead.cpuidValid);
    EXPECT_FALSE(cpusInfo[7].cpuidRead.cpuidValid);
    EXPECT_TRUE(cpusInfo[0].chaCountRead.chaCountValid);
    EXPECT_TRUE(cpusInfo[1].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[2].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[3].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[4].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[5].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[6].chaCountRead.chaCountValid);
    EXPECT_FALSE(cpusInfo[7].chaCountRead.chaCountValid);
    EXPECT_TRUE(cpusInfo[0].coreMaskRead.coreMaskValid);
    EXPECT_TRUE(cpusInfo[1].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[2].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[3].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[4].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[5].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[6].coreMaskRead.coreMaskValid);
    EXPECT_FALSE(cpusInfo[7].coreMaskRead.coreMaskValid);
}

TEST(CrashdumpTest, test_emr_startup)
{
    TestCrashdump crashdump(cd_spr);
    std::vector<CPUInfo> cpusInfo = {};
    uint8_t coreMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t coreMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t chaMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t chaMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_CALL(*crashdump.libPeciMock, peci_GetCPUID)
        .WillOnce(DoAll(
            SetArgPointee<1>((CPUModel)0x000C06f0), SetArgPointee<2>(0),
            SetArgPointee<3>(PECI_DEV_CC_SUCCESS), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskHigh, coreMaskHigh + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskLow, coreMaskLow + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArrayArgument<7>(chaMaskHigh, chaMaskHigh + 4), // cha mask 0
            SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(
            DoAll(SetArrayArgument<7>(chaMaskLow, chaMaskLow + 4), // cha mask 1
                  SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, STARTUP);
    // create_crashdump
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, EVENT);
    EXPECT_TRUE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_EQ(cpusInfo[0].cpuidRead.source, STARTUP);
    EXPECT_EQ(cpusInfo[0].cpuidRead.cpuModel, (CPUModel)0x000C06f0);
}

TEST(CrashdumpTest, test_emr_event)
{
    TestCrashdump crashdump(cd_spr);
    std::vector<CPUInfo> cpusInfo = {};
    uint8_t coreMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t coreMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t chaMaskHigh[4] = {0x05, 0x06, 0x07, 0x08};
    uint8_t chaMaskLow[4] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_CALL(*crashdump.libPeciMock, peci_GetCPUID)
        .WillOnce(DoAll(
            SetArgPointee<1>((CPUModel)0x000C06f0), SetArgPointee<2>(0),
            SetArgPointee<3>(PECI_DEV_CC_SUCCESS), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_Ping)
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)))
        .WillOnce(DoAll(Return(PECI_CC_TIMEOUT)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskHigh, coreMaskHigh + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(coreMaskLow, coreMaskLow + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(
            SetArrayArgument<7>(chaMaskHigh, chaMaskHigh + 4), // cha mask 0
            SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(
            DoAll(SetArrayArgument<7>(chaMaskLow, chaMaskLow + 4), // cha mask 1
                  SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, STARTUP);
    // create_crashdump
    initCPUInfo(cpusInfo);
    getCPUData(cpusInfo, EVENT);
    EXPECT_TRUE(cpusInfo[0].cpuidRead.cpuidValid);
    EXPECT_EQ(cpusInfo[0].cpuidRead.source, EVENT);
    EXPECT_EQ(cpusInfo[0].cpuidRead.cpuModel, (CPUModel)0x000C06f0);
}
