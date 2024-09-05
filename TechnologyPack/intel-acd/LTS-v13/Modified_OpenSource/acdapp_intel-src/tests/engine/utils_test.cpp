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
#include "engine/utils.h"
}

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(UtilsTestFixture, snprintf_s_test)
{

    char jsonItemString[10];
    cd_snprintf_s(jsonItemString, 10, "%s", "testing");
    EXPECT_STREQ(jsonItemString, "testing");
}

TEST(UtilsTestFixture, readInputFileFlag_test)
{
    cJSON* root = NULL;
    bool status = readInputFileFlag(root, true, "ENABLE");
    EXPECT_TRUE(status);
    status = readInputFileFlag(root, false, "ENABLE");
    EXPECT_FALSE(status);
    static const char* rootstrtrue = R"({"ENABLE":true})";
    root = cJSON_Parse(rootstrtrue);
    status = readInputFileFlag(root, true, "ENABLE");
    EXPECT_TRUE(status);
    static const char* rootstrfalse = R"({"ENABLE":false})";
    root = cJSON_Parse(rootstrfalse);
    status = readInputFileFlag(root, true, "ENABLE");
    EXPECT_FALSE(status);
}

TEST(UtilsTestFixture, getNewCrashDataSection_test)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_mmio.json");
    cJSON* section = getNewCrashDataSection(inRoot, "Uncore_MMIO");
    EXPECT_TRUE(section != NULL);
}

TEST(UtilsTestFixture, getNewCrashDataSection_test_NULL)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_mmio.json");
    cJSON* section = getNewCrashDataSection(inRoot, "MMIO");
    EXPECT_TRUE(section == NULL);
}

TEST(UtilsTestFixture, getNewCrashDataSectionObjectOneLevel_test)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_mmio.json");
    cJSON* section =
        getNewCrashDataSectionObjectOneLevel(inRoot, "Uncore_MMIO", "PECICmds");
    EXPECT_TRUE(section != NULL);
}

TEST(UtilsTestFixture, getNewCrashDataSectionObjectOneLevel_test_NULL)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_mmio.json");
    cJSON* section = getNewCrashDataSectionObjectOneLevel(inRoot, "Uncore_MMIO",
                                                          "PECICmds2");
    EXPECT_TRUE(section == NULL);
}

TEST(UtilsTestFixture, isBigCoreRegVersionMatch_test)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_bigcore.json");
    bool status = isBigCoreRegVersionMatch(inRoot, 0x4029001);
    EXPECT_TRUE(status);
}

TEST(UtilsTestFixture, isBigCoreRegVersionMatch_test_NULL)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_bigcore.json");
    bool status = isBigCoreRegVersionMatch(inRoot, 0x4029004);
    EXPECT_FALSE(status);
}

TEST(UtilsTestFixture, getCrashDataSectionBigCoreRegList_test)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_bigcore.json");
    cJSON* output = getCrashDataSectionBigCoreRegList(inRoot, "0x4029001");
    EXPECT_TRUE(output != NULL);
}

TEST(UtilsTestFixture, getCrashDataSectionBigCoreRegList_test_NULL)
{
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_bigcore.json");
    cJSON* output = getCrashDataSectionBigCoreRegList(inRoot, "0x53");
    EXPECT_TRUE(output == NULL);
}

TEST(UtilsTestFixture, getCrashDataSectionBigCoreSize_test)
{
    uint32_t value = 0;
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_bigcore.json");
    uint32_t output = getCrashDataSectionBigCoreSize(inRoot, "0x4029001");
    EXPECT_EQ(output, value);
}

TEST(UtilsTestFixture, loadInputFiles_test)
{
    char* InputFile = "Testing";
    Model cpu = cd_sprhbm;
    cJSON* inRoot = selectAndReadInputFile(cpu, &InputFile, false);
    EXPECT_TRUE(inRoot == NULL);
}

TEST(UtilsTestFixture, loadInputFiles_telemetry_test)
{
    char* InputFile = "Testing";
    Model cpu = cd_sprhbm;
    cJSON* inRoot = selectAndReadInputFile(cpu, &InputFile, true);
    EXPECT_TRUE(inRoot == NULL);
}

TEST(UtilsTestFixture, getNVDSectionRegList_test)
{
    bool testing = true;
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_nvd.json");
    cJSON* outRoot = getNVDSectionRegList(inRoot, "csr", &testing);
    EXPECT_TRUE(outRoot != NULL);
}

TEST(UtilsTestFixture, getPMEMSectionErrLogList_test)
{
    bool testing = true;
    cJSON* inRoot = readInputFile("../tests/UnitTestFiles/ut_nvd.json");
    cJSON* outRoot = getPMEMSectionErrLogList(inRoot, "error_log", &testing);
    EXPECT_TRUE(outRoot != NULL);
}

TEST(UtilsTestFixture, logResetDetected_negative_test)
{
    static PlatformState platformState = {false, DEFAULT_VALUE, DEFAULT_VALUE,
                                          DEFAULT_VALUE, DEFAULT_VALUE};
    cJSON* metadata = cJSON_CreateObject();
    CPUInfo cpuInfo;
    logResetDetected(metadata, cpuInfo, platformState);
    EXPECT_STREQ(metadata->child->string, "_reset_detected");
    EXPECT_STREQ(metadata->child->valuestring, "NONE");
}

TEST(UtilsTestFixture, logResetDetected_postive_test)
{
    static PlatformState platformState = {false, DEFAULT_VALUE, DEFAULT_VALUE,
                                          DEFAULT_VALUE, DEFAULT_VALUE};
    cJSON* metadata = cJSON_CreateObject();
    CPUInfo cpuInfo;
    cJSON* inRoot = readInputFile("../crashdump_input_spr.json");
    cpuInfo.inputFile.bufferPtr = inRoot;
    platformState.resetDetected = true;
    platformState.resetCpu = 1;
    platformState.resetSection = 1;
    logResetDetected(metadata, cpuInfo, platformState);
    EXPECT_STREQ(metadata->child->string, "_reset_detected");
    EXPECT_STREQ(metadata->child->valuestring, "cpu1.MCA_UNCORE");
}

TEST(UtilsTestFixture, fillMeVersion_test)
{
    cJSON* metaData = cJSON_CreateObject();
    int status = fillMeVersion("me_fw_ver", metaData);
    EXPECT_EQ(status, ACD_SUCCESS);
}

TEST(UtilsTestFixture, fillCrashdumpVersion_test)
{
    cJSON* metaData = cJSON_CreateObject();
    int status = fillCrashdumpVersion("crashdump_ver", metaData);
    EXPECT_EQ(status, ACD_SUCCESS);
}

TEST(UtilsTestFixture, cJSONToInt_PASS_test)
{
    cJSON* object = cJSON_CreateObject();
    cJSON_AddStringToObject(object, "Register", "0x16");
    int value = cJSONToInt(object->child, 16);
    EXPECT_EQ(value, 22);
    value = cJSONToInt(object, 16);
    EXPECT_EQ(value, 0);
    cJSON* object2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(object, "Register", 2);
    value = cJSONToInt(object2->child, 16);
    EXPECT_EQ(value, 0);
    cJSON* object3 = NULL;
    value = cJSONToInt(object3, 16);
    EXPECT_EQ(value, 0);
}
