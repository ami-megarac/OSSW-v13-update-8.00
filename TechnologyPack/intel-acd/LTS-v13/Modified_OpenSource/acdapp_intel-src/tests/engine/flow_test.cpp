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

class FlowTestFixture : public ::testing::Test
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

fs::path FlowTestFixture::UTFile;
cJSON* FlowTestFixture::inRoot;

TEST_F(FlowTestFixture, fillNewSectionSingleSectionTest)
{
    TestCrashdump crashdump(cpusInfo);
    static cJSON* inRoot;
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_uncore.json";
    inRoot = readInputFile(UTFile.c_str());
    RunTimeInfo runTimeInfo;
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].inputFile.bufferPtr = inRoot;
    uint8_t busValid[4] = {0x3f, 0x0f, 0x00, 0xc0}; // 0xc0000f3f;
    uint8_t busno7[4] = {0x00, 0x00, 0x7e, 0x7f};   // 0x7f7e0000;
    uint8_t busno2[4] = {0x00, 0x00, 0x7e, 0x7f};   // 0x7f7e0000;

    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
        .WillRepeatedly(DoAll(SetArgPointee<3>(0x1122334455667788),
                              SetArgPointee<4>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigMmio)
        .WillRepeatedly(
            DoAll(SetArgPointee<10>(0x40), Return(PECI_CC_SUCCESS)));

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
    cJSON* uncore = cJSON_GetObjectItem(
        cJSON_GetObjectItem(
            cJSON_GetObjectItem(cJSON_GetObjectItem(outRoot, "crash_data"),
                                "PROCESSORS"),
            "cpu0"),
        "uncore");
    cJSON* version = cJSON_GetObjectItem(uncore, "_version");
    if (version != NULL)
    {
        EXPECT_STREQ(version->valuestring, "0x801c022");
    }
    cJSON* time = cJSON_GetObjectItem(uncore, "_time:");

    if (time != NULL)
    {
        double test_value = std::stod(time->valuestring);
        EXPECT_NEAR(1.0, test_value, 0.3);
    }
}

TEST_F(FlowTestFixture, fillNewSectionDoubleSectionTest)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_uncore.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    cpusInfo[0].chaCount = 2;
    cpusInfo[1].chaCount = 2;
    uint8_t busValid[4] = {0x3f, 0x0f, 0x00, 0xc0}; // 0xc0000f3f;
    uint8_t busno7[4] = {0x00, 0x00, 0x7e, 0x7f};   // 0x7f7e0000;
    uint8_t busno2[4] = {0x00, 0x00, 0x7e, 0x7f};   // 0x7f7e0000;

    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
        .WillRepeatedly(DoAll(SetArgPointee<3>(0x1122334455667788),
                              SetArgPointee<4>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigMmio)
        .WillRepeatedly(
            DoAll(SetArgPointee<10>(0x40), Return(PECI_CC_SUCCESS)));

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

    cJSON* uncore = cJSON_GetObjectItem(
        cJSON_GetObjectItem(
            cJSON_GetObjectItem(cJSON_GetObjectItem(outRoot, "crash_data"),
                                "PROCESSORS"),
            "cpu0"),
        "uncore");
    cJSON* time = cJSON_GetObjectItem(uncore, "_time:");
    if (time != NULL)
    {
        double test_value = std::stod(time->valuestring);
        EXPECT_NEAR(1.0, test_value, 0.5);
    }
}

TEST_F(FlowTestFixture, fillNewSectionBadCCNodeTest)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_rdiamsr.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    cpusInfo[0].chaCount = 1;
    cpusInfo[1].chaCount = 1;

    EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
        .WillOnce(DoAll(SetArgPointee<3>(0x1122334455667788),
                        SetArgPointee<4>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArgPointee<3>(0x1122334455667788),
                        SetArgPointee<4>(0x90),
                        Return(PECI_CC_SUCCESS))) // ERROR
        .WillOnce(DoAll(SetArgPointee<3>(0x1122334455667788),
                        SetArgPointee<4>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArgPointee<3>(0x1122334455667788),
                        SetArgPointee<4>(0x90),
                        Return(PECI_CC_SUCCESS))); // ERROR

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
            status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo,
                                    section);
        }
    }
    cJSON* uncore = cJSON_GetObjectItem(
        cJSON_GetObjectItem(
            cJSON_GetObjectItem(cJSON_GetObjectItem(outRoot, "crash_data"),
                                "PROCESSORS"),
            "cpu0"),
        "uncore");
    cJSON* goodTransaction = cJSON_GetObjectItem(uncore, "RDIAMSR_0x17");
    if (goodTransaction != NULL)
    {
        EXPECT_STREQ(goodTransaction->valuestring, "0x1122334455667788");
    }
    cJSON* errorTransaction = cJSON_GetObjectItem(uncore, "RDIAMSR_0x194");
    if (errorTransaction != NULL)
    {
        EXPECT_STREQ(errorTransaction->valuestring,
                     "0x1122334455667788,CC:0x90,RC:0x0");
    }
    cJSON* naTransaction = cJSON_GetObjectItem(uncore, "RDIAMSR_0x1a1");
    if (naTransaction != NULL)
    {
        EXPECT_STREQ(naTransaction->valuestring, "N/A");
    }
}

TEST_F(FlowTestFixture, checkMaxGlobalCondition)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_uncore.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    cpusInfo[0].chaCount = 1;
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].chaCount = 1;
    cpusInfo[1].inputFile.bufferPtr = inRoot;

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 2;

    uint8_t numberOfSections = getNumberOfSections(&cpusInfo[0]);
    for (uint8_t section = 0; section < numberOfSections; section++)
    {
        for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
        {
            sleep(2); // Idle so that Max Time condition is reached.
            status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo,
                                    section);
        }
    }
    cJSON* uncore = cJSON_GetObjectItem(
        cJSON_GetObjectItem(
            cJSON_GetObjectItem(cJSON_GetObjectItem(outRoot, "crash_data"),
                                "PROCESSORS"),
            "cpu0"),
        "uncore");
    cJSON* noDataTransaction = cJSON_GetObjectItem(uncore, "cpubusno7");
    if (noDataTransaction != NULL)
    {
        EXPECT_STREQ(noDataTransaction->valuestring, "N/A");
    }
    cJSON* timeoutMessage0 =
        cJSON_GetObjectItem(uncore, "_cpu0_Uncore_MMIO_global_timeout:");
    EXPECT_TRUE(timeoutMessage0 != NULL);
    if (timeoutMessage0 != NULL)
    {
        EXPECT_STREQ(timeoutMessage0->valuestring, "2s");
    }
    cJSON* timeoutMessage1 =
        cJSON_GetObjectItem(uncore, "_cpu0_Uncore_RdIAMSR_global_timeout:");
    EXPECT_TRUE(timeoutMessage1 != NULL);
    if (timeoutMessage1 != NULL)
    {
        EXPECT_STREQ(timeoutMessage1->valuestring, "2s");
    }
    cJSON* time = cJSON_GetObjectItem(uncore, "_time:");
    if (time != NULL)
    {
        double test_value = std::stod(time->valuestring);
        EXPECT_NEAR(2.0, test_value, 0.5);
    }
}

TEST_F(FlowTestFixture, checkMaxSectionCondition)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_multiple_sections.json";
    inRoot = readInputFile(UTFile.c_str());
    RunTimeInfo runTimeInfo;
    cpusInfo[0].chaCount = 1;
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].chaCount = 1;
    cpusInfo[1].inputFile.bufferPtr = inRoot;
    uint8_t busValid[4] = {0x3f, 0x0f, 0x00, 0xc0}; // 0xc0000f3f;
    uint8_t busno7[4] = {0x00, 0x00, 0x7e, 0x7f};   // 0x7f7e0000;
    uint8_t busno2[4] = {0x00, 0x00, 0x7e, 0x7f};   // 0x7f7e0000;

    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
        .WillRepeatedly(DoAll(SetArgPointee<3>(0x1122334455667788),
                              SetArgPointee<4>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigMmio)
        .WillRepeatedly(
            DoAll(SetArgPointee<10>(0x40), Return(PECI_CC_SUCCESS)));
    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 10;

    uint8_t numberOfSections = getNumberOfSections(&cpusInfo[0]);
    for (uint8_t section = 0; section < numberOfSections; section++)
    {
        for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
        {
            sleep(2); // Idle so that Max Time condition is reached.
            status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo,
                                    section);
        }
    }
    cJSON* uncore = cJSON_GetObjectItem(
        cJSON_GetObjectItem(
            cJSON_GetObjectItem(cJSON_GetObjectItem(outRoot, "crash_data"),
                                "PROCESSORS"),
            "cpu0"),
        "uncore");
    cJSON* timeoutMessage0 =
        cJSON_GetObjectItem(uncore, "_cpu0_Uncore_PCI_section_timeout:");
    EXPECT_TRUE(timeoutMessage0 != NULL);
    if (timeoutMessage0 != NULL)
    {
        EXPECT_STREQ(timeoutMessage0->valuestring, "2s");
    }
    cJSON* noDataTransaction = cJSON_GetObjectItem(uncore, "B00_D00_F0_0x0");
    if (noDataTransaction != NULL)
    {
        EXPECT_STREQ(noDataTransaction->valuestring, "N/A");
    }
    cJSON* time = cJSON_GetObjectItem(uncore, "_time:");
    if (time != NULL)
    {
        double test_value = std::stod(time->valuestring);
        EXPECT_NEAR(2.5, test_value, 0.5);
    }
}

TEST_F(FlowTestFixture, failPreReqTest)
{
    TestCrashdump crashdump(cpusInfo);
    static cJSON* inRoot;
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_mmio_fail_prereq.json";
    inRoot = readInputFile(UTFile.c_str());
    RunTimeInfo runTimeInfo;
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    cpusInfo[1].inputFile.bufferPtr = inRoot;
    uint8_t busValid[4] = {0x0, 0x00, 0x00, 0x00}; // 0xc0000f3f;
    uint8_t busno7[4] = {0x00, 0x00, 0x7e, 0x7f};  // 0x7f7e0000;
    uint8_t busno2[4] = {0x00, 0x00, 0x7e, 0x7f};  // 0x7f7e0000;

    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busValid, busValid + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno7, busno7 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)))
        .WillOnce(DoAll(SetArrayArgument<7>(busno2, busno2 + 4),
                        SetArgPointee<8>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
        .WillRepeatedly(DoAll(SetArgPointee<3>(0x1122334455667788),
                              SetArgPointee<4>(0x40), Return(PECI_CC_SUCCESS)));
    EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigMmio)
        .WillRepeatedly(
            DoAll(SetArgPointee<10>(0x40), Return(PECI_CC_SUCCESS)));

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
    cJSON* uncore = cJSON_GetObjectItem(
        cJSON_GetObjectItem(
            cJSON_GetObjectItem(cJSON_GetObjectItem(outRoot, "crash_data"),
                                "PROCESSORS"),
            "cpu0"),
        "uncore");
    cJSON* post_enum_bus30 = cJSON_GetObjectItem(uncore, "post_enum_bus30");
    EXPECT_TRUE(post_enum_bus30 != NULL);
    if (post_enum_bus30 != NULL)
    {
        EXPECT_STREQ(post_enum_bus30->valuestring, "0x0");
    }
}