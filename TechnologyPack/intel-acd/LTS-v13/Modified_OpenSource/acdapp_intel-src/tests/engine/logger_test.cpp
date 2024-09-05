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

class ProcessLoggerTestFixture : public ::testing::Test
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

fs::path ProcessLoggerTestFixture::UTFile;
cJSON* ProcessLoggerTestFixture::inRoot;

TEST_F(ProcessLoggerTestFixture, OutputPathTest)
{
    LoggerStruct loggerStruct;
    static const char* OutputStr =
        R"({"OutputPath":"crash_data/PROCESSORS/cpu%d/uncore"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputStr);
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    EXPECT_EQ(loggerStruct.pathParsing.numberOfTokens, 4);
    EXPECT_STREQ(loggerStruct.pathParsing.pathLevelToken[0], "crash_data");
    EXPECT_STREQ(loggerStruct.pathParsing.pathLevelToken[1], "PROCESSORS");
    EXPECT_STREQ(loggerStruct.pathParsing.pathLevelToken[2], "cpu%d");
    EXPECT_STREQ(loggerStruct.pathParsing.pathLevelToken[3], "uncore");
}

TEST_F(ProcessLoggerTestFixture, LoggerCpuCycleTest)
{
    static const char* OutputPathStr = R"({"OutputPath":"cpu%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Size":"4", "Name":["RegisterName", "SectionName"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int cpu = 0; cpu < 2; cpu++)
    {
        loggerStruct.contextLogger.cpu = cpu;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu0");
    EXPECT_FALSE(Object1 == NULL);
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu1");
    EXPECT_FALSE(Object2 == NULL);
}

TEST_F(ProcessLoggerTestFixture, LoggerChaCycleTest)
{
    static const char* OutputPathStr = R"({"OutputPath":"cbo%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Size":"4", "Name":["RegisterName", "SectionName"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int cha = 0; cha < 2; cha++)
    {
        loggerStruct.contextLogger.cha = cha;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "cbo0");
    EXPECT_FALSE(Object1 == NULL);
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "cbo1");
    EXPECT_FALSE(Object2 == NULL);
}

TEST_F(ProcessLoggerTestFixture, LoggerCoreCycleTest)
{
    static const char* OutputPathStr = R"({"OutputPath":"core%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Size":"4", "Name":["RegisterName", "SectionName"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int core = 0; core < 2; core++)
    {
        loggerStruct.contextLogger.core = core;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "core0");
    EXPECT_FALSE(Object1 == NULL);
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "core1");
    EXPECT_FALSE(Object2 == NULL);
}

TEST_F(ProcessLoggerTestFixture, LoggerThreadCycleTest)
{
    static const char* OutputPathStr = R"({"OutputPath":"thread%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Size":"4", "Name":["RegisterName", "SectionName"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int thread = 0; thread < 2; thread++)
    {
        loggerStruct.contextLogger.thread = thread;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "thread0");
    EXPECT_FALSE(Object1 == NULL);
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "thread1");
    EXPECT_FALSE(Object2 == NULL);
}

TEST_F(ProcessLoggerTestFixture, LoggerPrintSkip)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    loggerStruct.nameProcessing.registerName = "RegisterName";
    cmdInOut.out.ret = PECI_CC_DRIVER_ERR;
    cmdInOut.out.cc = PECI_DEV_CC_MCA_ERROR;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.nameProcessing.sizeFromOutput = true;
    loggerStruct.contextLogger.skipFlag = true;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName");
    EXPECT_FALSE(Object1 == NULL);
    EXPECT_STREQ(Object1->valuestring, "N/A");
}

TEST_F(ProcessLoggerTestFixture, LoggerPrintBadRet)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    loggerStruct.nameProcessing.registerName = "RegisterName";
    cmdInOut.out.ret = PECI_CC_DRIVER_ERR;
    cmdInOut.out.cc = PECI_DEV_CC_MCA_ERROR;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.nameProcessing.sizeFromOutput = true;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName");
    EXPECT_FALSE(Object1 == NULL);
    EXPECT_STREQ(Object1->valuestring, "0x0,CC:0x91,RC:0x3");
}

TEST_F(ProcessLoggerTestFixture, LoggerBadCC)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    loggerStruct.nameProcessing.registerName = "RegisterName";
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_INVALID_REQ;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.nameProcessing.sizeFromOutput = true;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeefdeadbeef;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName");
    EXPECT_FALSE(Object1 == NULL);
    EXPECT_STREQ(Object1->valuestring, "0xdeadbeef,CC:0x90,RC:0x0");
}

TEST_F(ProcessLoggerTestFixture, LoggerGoodSize)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.sizeFromOutput = true;
    loggerStruct.contextLogger.skipFlag = false;
    loggerStruct.nameProcessing.registerName = "RegisterName32";
    loggerStruct.nameProcessing.size = 4;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object1 =
        cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName32");
    EXPECT_FALSE(Object1 == NULL);
    EXPECT_STREQ(Object1->valuestring, "0xdeadbeef");
    loggerStruct.nameProcessing.registerName = "RegisterName64";
    cmdInOut.out.val.u64 = 0xdeadbeefdeadbeef;
    loggerStruct.nameProcessing.size = 8;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object2 =
        cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName64");
    EXPECT_FALSE(Object2 == NULL);
    EXPECT_STREQ(Object2->valuestring, "0xdeadbeefdeadbeef");
    loggerStruct.nameProcessing.registerName = "RegisterName16";
    cmdInOut.out.val.u64 = 0xbeef;
    loggerStruct.nameProcessing.size = 2;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object3 =
        cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName16");
    EXPECT_FALSE(Object3 == NULL);
    EXPECT_STREQ(Object3->valuestring, "0xbeef");
    loggerStruct.nameProcessing.registerName = "RegisterName8";
    cmdInOut.out.val.u64 = 0xef;
    loggerStruct.nameProcessing.size = 1;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object4 = cJSON_GetObjectItemCaseSensitive(outRoot, "RegisterName8");
    EXPECT_FALSE(Object4 == NULL);
    EXPECT_STREQ(Object4->valuestring, "0xef");
}

TEST_F(ProcessLoggerTestFixture, LoggingTest)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.sizeFromOutput = true;
    loggerStruct.contextLogger.skipFlag = false;
    loggerStruct.nameProcessing.registerName = "String";
    loggerStruct.nameProcessing.size = 4;
    cmdInOut.out.printString = true;
    cmdInOut.out.stringVal = "Testing";
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "String");
    EXPECT_FALSE(Object1 == NULL);
    EXPECT_STREQ(Object1->valuestring, "Testing");
}

TEST_F(ProcessLoggerTestFixture, LoggerRepeatBadCC)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    loggerStruct.nameProcessing.registerName = "Pll_0x220c_r%d";
    loggerStruct.contextLogger.repeats = 0;
    cmdInOut.out.ret = PECI_CC_DRIVER_ERR;
    cmdInOut.out.cc = PECI_DEV_CC_MCA_ERROR;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.nameProcessing.sizeFromOutput = true;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    Logger(&cmdInOut, outRoot, &loggerStruct);
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "Pll_0x220c_r0");
    EXPECT_FALSE(Object1 == NULL);
    EXPECT_STREQ(Object1->valuestring, "0x0,CC:0x91,RC:0x3");
}

TEST_F(ProcessLoggerTestFixture, LoggerRecordDisable)
{
    static const char* OutputPathStr =
        R"({"OutputPath":"crash_data/PROCESSORS/cpu%d/MCA"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.rootAtLevel = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    logRecordDisabled(&cmdInOut, outRoot, &loggerStruct);
    cJSON* crash_data = cJSON_GetObjectItemCaseSensitive(outRoot, "crash_data");
    EXPECT_TRUE(crash_data != NULL);
    cJSON* processors =
        cJSON_GetObjectItemCaseSensitive(crash_data, "PROCESSORS");
    EXPECT_TRUE(processors != NULL);
    cJSON* cpu = cJSON_GetObjectItemCaseSensitive(processors, "cpu0");
    EXPECT_TRUE(cpu != NULL);
    cJSON* mca = cJSON_GetObjectItemCaseSensitive(cpu, "MCA");
    EXPECT_TRUE(mca != NULL);
    cJSON* record = cJSON_GetObjectItemCaseSensitive(mca, "_record_enable");
    EXPECT_TRUE(record != NULL);
    EXPECT_TRUE(cJSON_IsBool(record));
}

TEST_F(ProcessLoggerTestFixture, LoggerVersion)
{
    static const char* OutputPathStr =
        R"({"OutputPath":"crash_data/PROCESSORS/cpu%d/Uncore"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.rootAtLevel = 4;
    loggerStruct.contextLogger.version = 1;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    logVersion(&cmdInOut, outRoot, &loggerStruct);
    EXPECT_FALSE(loggerStruct.nameProcessing.zeroPaddedPrint);
    cJSON* crash_data = cJSON_GetObjectItemCaseSensitive(outRoot, "crash_data");
    EXPECT_TRUE(crash_data != NULL);
    cJSON* processors =
        cJSON_GetObjectItemCaseSensitive(crash_data, "PROCESSORS");
    EXPECT_TRUE(processors != NULL);
    cJSON* cpu = cJSON_GetObjectItemCaseSensitive(processors, "cpu0");
    EXPECT_TRUE(cpu != NULL);
    cJSON* uncore = cJSON_GetObjectItemCaseSensitive(cpu, "Uncore");
    EXPECT_TRUE(uncore != NULL);
    cJSON* version = cJSON_GetObjectItemCaseSensitive(uncore, "_version");
    EXPECT_TRUE(version != NULL);
    char* value = version->valuestring;
    EXPECT_STREQ("0x1", value);
}


TEST_F(ProcessLoggerTestFixture, GenerateVersion_Pass)
{
    static const char* strObject =
        R"({"RecordType":"0xb",
            "ProductType": "0x1c",
            "Revision": "0x01"})";
    int version=0;
    cJSON* jsonObject = cJSON_Parse(strObject);
    GenerateVersion(jsonObject, &version);
    EXPECT_EQ(0xb01C001, version);
}

TEST_F(ProcessLoggerTestFixture, GenerateVersion_Fail)
{
    static const char* strObject =
        R"({"RecordType":0,
            "ProductType": true,
            "Revision": 3.1416})";
    int version=0;
    cJSON* jsonObject = cJSON_Parse(strObject);
    GenerateVersion(jsonObject, &version);
    EXPECT_EQ(0x0, version);
}


TEST_F(ProcessLoggerTestFixture, LoggerRootLevelLessThanToken)
{
    static const char* OutputPathStr =
        R"({"OutputPath":"crash_data/PROCESSORS/cpu%d/Uncore"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.rootAtLevel = 2;
    loggerStruct.contextLogger.version = 1;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    logVersion(&cmdInOut, outRoot, &loggerStruct);
    cJSON* crash_data = cJSON_GetObjectItemCaseSensitive(outRoot, "crash_data");
    EXPECT_TRUE(crash_data != NULL);
    cJSON* processors =
        cJSON_GetObjectItemCaseSensitive(crash_data, "PROCESSORS");
    EXPECT_TRUE(processors != NULL);
    cJSON* version = cJSON_GetObjectItemCaseSensitive(processors, "_version");
    EXPECT_TRUE(version != NULL);
    char* value = version->valuestring;
    EXPECT_STREQ("0x1", value);
}

TEST_F(ProcessLoggerTestFixture, LoggerRootLevelBiggerThanToken)
{
    static const char* OutputPathStr =
        R"({"OutputPath":"crash_data/PROCESSORS/cpu%d/Uncore"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.rootAtLevel = 10;
    loggerStruct.contextLogger.version = 1;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    logVersion(&cmdInOut, outRoot, &loggerStruct);
    cJSON* crash_data = cJSON_GetObjectItemCaseSensitive(outRoot, "crash_data");
    EXPECT_TRUE(crash_data != NULL);
    cJSON* processors =
        cJSON_GetObjectItemCaseSensitive(crash_data, "PROCESSORS");
    EXPECT_TRUE(processors != NULL);
    cJSON* cpu = cJSON_GetObjectItemCaseSensitive(processors, "cpu0");
    EXPECT_TRUE(cpu != NULL);
    cJSON* uncore = cJSON_GetObjectItemCaseSensitive(cpu, "Uncore");
    EXPECT_TRUE(uncore != NULL);
    cJSON* version = cJSON_GetObjectItemCaseSensitive(uncore, "_version");
    EXPECT_TRUE(version != NULL);
    char* value = version->valuestring;
    EXPECT_STREQ("0x1", value);
}

TEST_F(ProcessLoggerTestFixture, LoggerNoRootAtLevelSpecified)
{
    static const char* OutputPathStr =
        R"({"OutputPath":"crash_data/PROCESSORS/cpu%d/Uncore"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    status = GetPath(OutputPathjson, &loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    status = ParsePath(&loggerStruct);
    EXPECT_EQ(ACD_SUCCESS, status);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.contextLogger.version = 1;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    logVersion(&cmdInOut, outRoot, &loggerStruct);
    cJSON* crash_data = cJSON_GetObjectItemCaseSensitive(outRoot, "crash_data");
    EXPECT_TRUE(crash_data != NULL);
    cJSON* processors =
        cJSON_GetObjectItemCaseSensitive(crash_data, "PROCESSORS");
    EXPECT_TRUE(processors != NULL);
    cJSON* cpu = cJSON_GetObjectItemCaseSensitive(processors, "cpu0");
    EXPECT_TRUE(cpu != NULL);
    cJSON* uncore = cJSON_GetObjectItemCaseSensitive(cpu, "Uncore");
    EXPECT_TRUE(uncore != NULL);
    cJSON* version = cJSON_GetObjectItemCaseSensitive(uncore, "_version");
    EXPECT_TRUE(version != NULL);
    char* value = version->valuestring;
    EXPECT_STREQ("0x1", value);
}

TEST_F(ProcessLoggerTestFixture, LoggerNoPathParseBeforeGenerationOfJson)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    status = GenerateJsonPath(&cmdInOut, outRoot, &loggerStruct, true);
    EXPECT_EQ(status, ACD_SUCCESS);
}

TEST_F(ProcessLoggerTestFixture, LoggerGenerateJsonPathFailure)
{
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    outRoot = NULL;
    status = GenerateJsonPath(&cmdInOut, outRoot, &loggerStruct, true);
    EXPECT_EQ(status, ACD_FAILURE);
}

TEST_F(ProcessLoggerTestFixture, ParseNameSectionNameNULL)
{
    static const char* OutputStr =
        R"({"Size": 8, "ZeroPadded": true})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = ParseNameSection(&cmdInOut, &loggerStruct);

    EXPECT_EQ(ACD_SUCCESS, status);

    EXPECT_EQ(loggerStruct.nameProcessing.extraLevel, false);
    EXPECT_EQ(loggerStruct.nameProcessing.sizeFromOutput, false);
    EXPECT_FALSE(loggerStruct.nameProcessing.zeroPaddedPrint);
    EXPECT_EQ(loggerStruct.nameProcessing.logRegister, false);
}

TEST_F(ProcessLoggerTestFixture, ParseNameSectionNamePosition)
{
    static const char* OutputStr =
        R"({"Size": 8,"Name": ["RegisterName"], "ZeroPadded": true})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = ParseNameSection(&cmdInOut, &loggerStruct);

    EXPECT_EQ(ACD_SUCCESS, status);

    EXPECT_EQ(loggerStruct.nameProcessing.extraLevel, false);
    EXPECT_STREQ(loggerStruct.nameProcessing.registerName, "RegisterName");
    EXPECT_EQ(loggerStruct.nameProcessing.sizeFromOutput, true);
    EXPECT_EQ(loggerStruct.nameProcessing.size, 8);
    EXPECT_TRUE(loggerStruct.nameProcessing.zeroPaddedPrint);
}

TEST_F(ProcessLoggerTestFixture, ParseNameSectionSectionPosition)
{
    static const char* OutputStr =
        R"({"Size": 8,"Name": ["RegisterName", "SectionName"], "ZeroPadded": true})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = ParseNameSection(&cmdInOut, &loggerStruct);

    EXPECT_EQ(ACD_SUCCESS, status);

    EXPECT_EQ(loggerStruct.nameProcessing.extraLevel, true);
    EXPECT_STREQ(loggerStruct.nameProcessing.registerName, "RegisterName");
    EXPECT_STREQ(loggerStruct.nameProcessing.sectionName, "SectionName");
    EXPECT_EQ(loggerStruct.nameProcessing.sizeFromOutput, true);
    EXPECT_EQ(loggerStruct.nameProcessing.size, 8);
    EXPECT_TRUE(loggerStruct.nameProcessing.zeroPaddedPrint);
}

TEST_F(ProcessLoggerTestFixture, LoggerLogValueWithoutZeroPadding)
{
    cJSON* root = cJSON_CreateObject();
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;

    // GenerateJsonPath
    loggerStruct.nameProcessing.sizeFromOutput = true;
    cmdInOut.out.size = 8;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.rootAtLevel = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.stringVal = "StringValue";

    // GenerateRegisterName
    loggerStruct.nameProcessing.registerName = "RegisterName";

    // LogValue
    loggerStruct.nameProcessing.size = 8;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = 0x40;
    loggerStruct.nameProcessing.zeroPaddedPrint = false;
    cmdInOut.out.val.u64 = 0xDEADBEEF;

    Logger(&cmdInOut, root, &loggerStruct);

    EXPECT_TRUE(cJSON_HasObjectItem(root, "RegisterName"));
    cJSON* regObj = cJSON_GetObjectItemCaseSensitive(
                            root, "RegisterName");
    EXPECT_STREQ(regObj->valuestring, "0xdeadbeef");
}

TEST_F(ProcessLoggerTestFixture, LoggerLogValueWithZeroPaddingAllSizes)
{
    cJSON* root = cJSON_CreateObject();
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;

    // GenerateJsonPath
    loggerStruct.nameProcessing.sizeFromOutput = true;
    cmdInOut.out.size = 8;
    loggerStruct.pathParsing.numberOfTokens = 0;
    loggerStruct.nameProcessing.rootAtLevel = 0;
    loggerStruct.nameProcessing.extraLevel = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.stringVal = "StringValue";

    // LogValue
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = 0x40;
    loggerStruct.nameProcessing.zeroPaddedPrint = true;
    cmdInOut.out.val.u64 = 0xA;

    // Size 8
    loggerStruct.nameProcessing.registerName = "RegisterNameSize8";
    loggerStruct.nameProcessing.size = 8;

    Logger(&cmdInOut, root, &loggerStruct);

    EXPECT_TRUE(cJSON_HasObjectItem(root, "RegisterNameSize8"));
    cJSON* regS8Obj = cJSON_GetObjectItemCaseSensitive(
                            root, "RegisterNameSize8");
    EXPECT_STREQ(regS8Obj->valuestring, "0x000000000000000a");

    // Size 4
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.nameProcessing.registerName = "RegisterNameSize4";

    Logger(&cmdInOut, root, &loggerStruct);

    EXPECT_TRUE(cJSON_HasObjectItem(root, "RegisterNameSize4"));
    cJSON* regS4Obj = cJSON_GetObjectItemCaseSensitive(
                            root, "RegisterNameSize4");
    EXPECT_STREQ(regS4Obj->valuestring, "0x0000000a");

    // Size 2
    loggerStruct.nameProcessing.size = 2;
    loggerStruct.nameProcessing.registerName = "RegisterNameSize2";

    Logger(&cmdInOut, root, &loggerStruct);

    EXPECT_TRUE(cJSON_HasObjectItem(root, "RegisterNameSize2"));
    cJSON* regS2Obj = cJSON_GetObjectItemCaseSensitive(
                            root, "RegisterNameSize2");
    EXPECT_STREQ(regS2Obj->valuestring, "0x000a");

    // Size 1
    loggerStruct.nameProcessing.size = 1;
    loggerStruct.nameProcessing.registerName = "RegisterNameSize1";

    Logger(&cmdInOut, root, &loggerStruct);

    EXPECT_TRUE(cJSON_HasObjectItem(root, "RegisterNameSize1"));
    cJSON* regS1Obj = cJSON_GetObjectItemCaseSensitive(
                            root, "RegisterNameSize1");
    EXPECT_STREQ(regS1Obj->valuestring, "0x0a");
}

TEST_F(ProcessLoggerTestFixture, LoggerGenerateRegisterName_Repeat)
{
    static const char* OutputPathStr = R"({"OutputPath":"cpu%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Name": ["Pll_0x210_r%d", "PLL"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    status = ParsePath(&loggerStruct);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int cpu = 0; cpu < 2; cpu++)
    {
        loggerStruct.contextLogger.cpu = cpu;
        loggerStruct.contextLogger.repeats = cpu;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu0");
    cJSON* PLL1 = cJSON_GetObjectItemCaseSensitive(Object1, "PLL");
    EXPECT_TRUE(PLL1->child != NULL);
    if (PLL1->child != NULL)
    {
        EXPECT_STREQ("Pll_0x210_r0", PLL1->child->string);
    }
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu1");
    cJSON* PLL2 = cJSON_GetObjectItemCaseSensitive(Object2, "PLL");
    EXPECT_TRUE(PLL2->child != NULL);
    if (PLL2->child != NULL)
    {
        EXPECT_STREQ("Pll_0x210_r1", PLL2->child->string);
    }
}

TEST_F(ProcessLoggerTestFixture, LoggerGenerateRegisterName_cpu)
{
    static const char* OutputPathStr = R"({"OutputPath":"cpu%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Name": ["cpu%d", "PLL"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    status = ParsePath(&loggerStruct);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int cpu = 0; cpu < 2; cpu++)
    {
        loggerStruct.contextLogger.cpu = cpu;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu0");
    cJSON* PLL1 = cJSON_GetObjectItemCaseSensitive(Object1, "PLL");
    EXPECT_TRUE(PLL1->child != NULL);
    if (PLL1->child != NULL)
    {
        EXPECT_STREQ("cpu0", PLL1->child->string);
    }
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu1");
    cJSON* PLL2 = cJSON_GetObjectItemCaseSensitive(Object2, "PLL");
    EXPECT_TRUE(PLL2->child != NULL);
    if (PLL2->child != NULL)
    {
        EXPECT_STREQ("cpu1", PLL2->child->string);
    }
}

TEST_F(ProcessLoggerTestFixture, LoggerGenerateRegisterName_core)
{
    static const char* OutputPathStr = R"({"OutputPath":"cpu%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Name": ["core%d", "PLL"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    status = ParsePath(&loggerStruct);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int cpu = 0; cpu < 2; cpu++)
    {
        loggerStruct.contextLogger.cpu = cpu;
        loggerStruct.contextLogger.core = cpu;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu0");
    cJSON* PLL1 = cJSON_GetObjectItemCaseSensitive(Object1, "PLL");
    EXPECT_TRUE(PLL1->child != NULL);
    if (PLL1->child != NULL)
    {
        EXPECT_STREQ("core0", PLL1->child->string);
    }
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu1");
    cJSON* PLL2 = cJSON_GetObjectItemCaseSensitive(Object2, "PLL");
    EXPECT_TRUE(PLL2->child != NULL);
    if (PLL2->child != NULL)
    {
        EXPECT_STREQ("core1", PLL2->child->string);
    }
}

TEST_F(ProcessLoggerTestFixture, LoggerGenerateRegisterName_thread)
{
    static const char* OutputPathStr = R"({"OutputPath":"cpu%d"})";
    cJSON* OutputPathjson = cJSON_Parse(OutputPathStr);
    static const char* OutputStr =
        R"({"Name": ["thread%d", "PLL"]})";
    cJSON* Outputjson = cJSON_Parse(OutputStr);
    LoggerStruct loggerStruct;
    CmdInOut cmdInOut;
    cmdInOut.in.outputPath = Outputjson;
    status = GetPath(OutputPathjson, &loggerStruct);
    status = ParsePath(&loggerStruct);
    status = ParseNameSection(&cmdInOut, &loggerStruct);
    cmdInOut.out.ret = PECI_CC_SUCCESS;
    cmdInOut.out.cc = PECI_DEV_CC_SUCCESS;
    loggerStruct.nameProcessing.size = 4;
    loggerStruct.contextLogger.skipFlag = false;
    cmdInOut.out.printString = false;
    cmdInOut.out.val.u64 = 0xdeadbeef;
    for (int cpu = 0; cpu < 2; cpu++)
    {
        loggerStruct.contextLogger.cpu = cpu;
        loggerStruct.contextLogger.thread = cpu;
        Logger(&cmdInOut, outRoot, &loggerStruct);
    }
    cJSON* Object1 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu0");
    cJSON* PLL1 = cJSON_GetObjectItemCaseSensitive(Object1, "PLL");
    EXPECT_TRUE(PLL1->child != NULL);
    if (PLL1->child != NULL)
    {
        EXPECT_STREQ("thread0", PLL1->child->string);
    }
    cJSON* Object2 = cJSON_GetObjectItemCaseSensitive(outRoot, "cpu1");
    cJSON* PLL2 = cJSON_GetObjectItemCaseSensitive(Object2, "PLL");
    EXPECT_TRUE(PLL2->child != NULL);
    if (PLL2->child != NULL)
    {
        EXPECT_STREQ("thread1", PLL2->child->string);
    }
}
