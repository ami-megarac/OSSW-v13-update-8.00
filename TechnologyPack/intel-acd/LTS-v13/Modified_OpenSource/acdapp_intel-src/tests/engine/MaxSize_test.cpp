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
#include "crashdump.hpp"

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

cJSON* outRoot = cJSON_CreateObject();
size_t nCha = 60;
int nCPUs = 2;
int globalCC = PECI_DEV_CC_SUCCESS;

std::filesystem::path out_file = "max_output.json";

static int getNumberOfRepetitions(cJSON* sections, char* section, char* command)
{
    cJSON* sectionObj =
        cJSON_GetObjectItemCaseSensitive(sections->child, section);
    cJSON* peciCmds = cJSON_GetObjectItemCaseSensitive(sectionObj, "PECICmds");
    cJSON* commandObjs =
        cJSON_GetObjectItemCaseSensitive(peciCmds->child, command);

    int nRegs = cJSON_GetArraySize(commandObjs);
    return nRegs;
}

static int getNumberOfRepetitionsUncorePCI(cJSON* sections)
{
    cJSON* sectionObj =
        cJSON_GetObjectItemCaseSensitive(sections->child, "Uncore_PCI");
    cJSON* peciCmds = cJSON_GetObjectItemCaseSensitive(sectionObj, "PECICmds");
    cJSON* commandObjs = cJSON_GetObjectItemCaseSensitive(
        peciCmds->child, "RdEndpointConfigPCILocal");

    int n_rx_4 = 0; // 1 peci call for each
    int n_rx_8 = 0; // 1 peci call for each
    cJSON* peci_call = NULL;
    cJSON_ArrayForEach(peci_call, commandObjs)
    {
        unsigned char rx_len =
            cJSON_GetArrayItem(peci_call->child, 6)->valueint;

        switch (rx_len)
        {
            case sizeof(uint8_t):
            case sizeof(uint16_t):
            case sizeof(uint32_t):
                n_rx_4++;
                break;
            case sizeof(uint64_t):
                n_rx_8++;
                break;
        }
    }
    int total_reps = n_rx_4 + (2 * n_rx_8);

    return total_reps;
}

static int getNumberOfRepetitionsPM_pll(cJSON* sections)
{
    cJSON* sectionObj = cJSON_GetObjectItemCaseSensitive(sections->child,
                                                         "PowerManagement_Pll");
    cJSON* peciCmds = cJSON_GetObjectItemCaseSensitive(sectionObj, "PECICmds");
    cJSON* commandObjs =
        cJSON_GetObjectItemCaseSensitive(peciCmds->child, "RdPkgConfig");

    int totalReps = 0;
    cJSON* peci_call = NULL;
    cJSON_ArrayForEach(peci_call, commandObjs)
    {
        if (cJSON_HasObjectItem(peci_call, "Repeat"))
        {
            cJSON* repeat =
                cJSON_GetObjectItemCaseSensitive(peci_call, "Repeat");
            totalReps += repeat->valueint;
        }
        else
        {
            totalReps++;
        }
    }

    return totalReps;
}

static int getNoActiveCores(CPUInfo& cpu)
{
    int coresActive = 0;

    for (uint8_t u8CoreNum = 0; u8CoreNum < MAX_CORE_MASK; u8CoreNum++)
    {
        if (!CHECK_BIT(cpu.coreMask, u8CoreNum))
        {
            continue;
        }
        if (CHECK_BIT(cpu.crashedCoreMask, u8CoreNum))
        {
            continue;
        }
        coresActive++;
    }
    return coresActive;
}

class MaxSizeTestFixture : public ::testing::Test
{
  protected:
    static cJSON* inRoot;
    static std::filesystem::path UTFile;
    static void readSampleFile()
    {
        UTFile = std::filesystem::current_path();
        UTFile = UTFile.parent_path();
        UTFile /= "crashdump_input_spr.json";
        inRoot = readInputFile(UTFile.c_str());
        if (inRoot == NULL)
        {
            EXPECT_TRUE(inRoot != NULL);
        }
    }
    static void SetUpTestSuite()
    {
        readSampleFile();
    }

    static void TearDownTestSuite()
    {
        cJSON_Delete(inRoot);
    }

    void SetUp() override
    {
        // Build a list of cpus
        CPUInfo cpuInfo = {.clientAddr = 48,
                           .model = cd_spr,
                           .coreMask = 0x1fffffffffffffff,
                           .crashedCoreMask = 0,
                           .chaCount = nCha,
                           .initialPeciWake = ON,
                           .inputFile = {},
                           .cpuidRead = {},
                           .chaCountRead = {},
                           .coreMaskRead = {},
                           .dimmMask = 0};

        for (int i = 0; i < nCPUs; i++)
        {
            cpusInfo.push_back(cpuInfo);
        }
    }

    void TearDown() override
    {
        FREE(jsonStr);
    }

  public:
    char* jsonStr = NULL;
    uint8_t cc = 0;
    bool enable = false;
    acdStatus status;
    std::vector<CPUInfo> cpusInfo;
};

std::filesystem::path MaxSizeTestFixture::UTFile;
cJSON* MaxSizeTestFixture::inRoot;

TEST_F(MaxSizeTestFixture, metadata)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_metadata.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    InSequence s;

    // METADATA
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        // PreReqs
        uint8_t data0[4] = {0xaa, 0xaa, 0xaa, 0xaa};
        uint8_t data1[4] = {0xbb, 0xbb, 0xbb, 0x00};
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdPkgConfig)
            .WillOnce(DoAll(
                            SetArrayArgument<4>(data0, data0 + 8),
                            SetArgPointee<5>(0x40),
                            Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(
                            SetArrayArgument<4>(data1, data1 + 8),
                            SetArgPointee<5>(0x40),
                            Return(PECI_CC_SUCCESS)));

        EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)));
        // Comands
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)));

        EXPECT_CALL(*crashdump.libPeciMock, peci_RdPkgConfig)
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(Return(PECI_CC_SUCCESS)));
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, MCA_UNCORE)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_mca_uncore.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");
    // MCA_UNCORE
    int mca_uncore_nRegs =
        getNumberOfRepetitions(sections, "MCA_UNCORE", "RdIAMSR");

    InSequence s;

    // MCA_UNCORE
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
            .Times(mca_uncore_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<3>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<4>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, MCA_CBO)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_mca_cbo.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // MCA_CBO
    int mca_cbo_nRegs = getNumberOfRepetitions(sections, "MCA_CBO", "RdIAMSR");

    InSequence s;

    // MCA_CBO
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
            .Times(nCha * mca_cbo_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<3>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<4>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, MCA_CORE)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_mca_core.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // MCA_CORE
    int mca_core_nRegs =
        getNumberOfRepetitions(sections, "MCA_CORE", "RdIAMSR");

    InSequence s;

    // MCA_CORE
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        int activeCores = getNoActiveCores(cpu);

        uint8_t threadsPerCore = 2;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
            .Times(activeCores * threadsPerCore * mca_core_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<3>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<4>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, Uncore_PCI)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_uncore_pci.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // Uncore_PCI
    int pci_nRegs = getNumberOfRepetitionsUncorePCI(sections);

    InSequence s;

    // Uncore_PCI
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
            .Times(pci_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<7>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<8>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }

    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, Uncore_MMIO)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_mmio.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // Uncore_MMIO
    int uncore_mmio_nRegs =
        getNumberOfRepetitions(sections, "Uncore_MMIO", "RdEndpointConfigMMIO");

    InSequence s;

    // Uncore_MMIO
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        uint8_t returnBusValidQuery[4] = {0x3f, 0x0f, 0x00,
                                          0xc0}; // 0xc0000f3f; bit 30 set
        uint8_t returnEnumeratedBus[4] = {0x00, 0x00, 0xab, 0x00}; // 0xab0000;
        uint8_t returnEnumeratedBus2[4] = {0x89, 0xab, 0xcd,
                                           0xef}; // 0xefcdab89;
        uint8_t cc = 0x40;
        // PreReqs
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigPciLocal)
            .WillOnce(DoAll(SetArrayArgument<7>(returnBusValidQuery,
                                                returnBusValidQuery + 4),
                            SetArgPointee<8>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<7>(returnEnumeratedBus,
                                                returnEnumeratedBus + 4),
                            SetArgPointee<8>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<7>(returnEnumeratedBus2,
                                                returnEnumeratedBus2 + 4),
                            SetArgPointee<8>(cc), Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
        // Commands
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdEndPointConfigMmio)
            .Times(uncore_mmio_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<9>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<10>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, Uncore_RdIAMSR)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_rdiamsr.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // Uncore_RdIAMSR
    int uncore_rdiamsr_nRegs =
        getNumberOfRepetitions(sections, "Uncore_RdIAMSR", "RdIAMSR");

    InSequence s;

    // Uncore_RdIAMSR
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
            .Times(uncore_rdiamsr_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<3>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<4>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }

    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, Uncore_RdIAMSR_CHA)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_rdiamsr_cha.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // Uncore_RdIAMSR_CHA
    int uncore_rdiamsr_cha_nRegs =
        getNumberOfRepetitions(sections, "Uncore_RdIAMSR_CHA", "RdIAMSR");

    InSequence s;
    // Uncore_RdIAMSR_CHA
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdIAMSR)
            .Times(nCha * uncore_rdiamsr_cha_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<3>(0xDEADBEEFDEADBEEF),
                                  SetArgPointee<4>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }

    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, Tor)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_tor.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    InSequence s;

    // TOR
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        uint8_t cc = 0x40;
        uint8_t u8CrashdumpEnabled = 0;
        uint16_t u16CrashdumpNumAgents = 2;
        uint8_t u64UniqueId[8] = {0xFF, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00};
        uint8_t PayloadExp[8] = {0x03, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00};
        uint8_t Data[8] = {0xef, 0xbe, 0xad, 0xde, 0xef, 0xbe, 0xad, 0xde};

        EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_Discovery)
            .WillOnce(DoAll(SetArgPointee<6>(u8CrashdumpEnabled),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArgPointee<6>(u16CrashdumpNumAgents),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<6>(u64UniqueId, u64UniqueId + 8),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<6>(PayloadExp, PayloadExp + 8),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

        EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
            .Times(nCha * 32 * 8)
            .WillRepeatedly(DoAll(SetArrayArgument<5>(Data, Data + 8),
                                  SetArgPointee<6>(globalCC),
                                  Return(PECI_CC_SUCCESS)));
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;

    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        sleep(1);
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }

    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, PM_PCI)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_pm_pci.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // PowerManagement_PCI
    int pm_pci_nRegs =
        getNumberOfRepetitions(sections, "PowerManagement_PCI", "RdPkgConfig");

    InSequence s;

    // PowerManagement_PCI
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        uint8_t Data[8] = {0xef, 0xbe, 0xad, 0xde, 0xef, 0xbe, 0xad, 0xde};
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdPkgConfig)
            .Times(pm_pci_nRegs)
            .WillRepeatedly(DoAll(SetArrayArgument<4>(Data, Data + 8),
                                  SetArgPointee<5>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, PM_PCI_core)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_pm_pci_core.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // PowerManagement_PCI_core
    int pm_pci_core_nRegs = getNumberOfRepetitions(
        sections, "PowerManagement_PCI_core", "RdPkgConfigCore");

    InSequence s;

    // PowerManagement_PCI_core
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        int activeCores = getNoActiveCores(cpu);
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdPkgConfig)
            .Times(activeCores * pm_pci_core_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<4>(0xDEADBEEFDEADBEAB),
                                  SetArgPointee<5>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, PM_PLL)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_pm_pll.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // PowerManagement_Pll
    int pm_pll_nRegs = getNumberOfRepetitionsPM_pll(sections);

    InSequence s;

    // PowerManagement_Pll
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdPkgConfig)
            .Times(pm_pll_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<4>(0xDEADBEEFDEADBEAB),
                                  SetArgPointee<5>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, PM_Dispatcher)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_pm_dispatcher.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    cJSON* sections = cJSON_GetObjectItemCaseSensitive(inRoot, "Sections");

    // PowerManagement_Dispatcher
    int pm_dispatcher_nRegs = getNumberOfRepetitions(
        sections, "PowerManagement_Dispatcher", "RdPkgConfig");

    InSequence s;

    // PowerManagement_Dispatcher
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        (void)cpu;
        EXPECT_CALL(*crashdump.libPeciMock, peci_RdPkgConfig)
            .Times(pm_dispatcher_nRegs)
            .WillRepeatedly(DoAll(SetArgPointee<4>(0xDEADBEEFDEADBEAB),
                                  SetArgPointee<5>(globalCC),
                                  Return(PECI_CC_SUCCESS)))
            .RetiresOnSaturation();
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }
    char* sOut = cJSON_Print(outRoot);

    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}

TEST_F(MaxSizeTestFixture, Big_core)
{
    TestCrashdump crashdump(cpusInfo);
    static std::filesystem::path UTFile;
    UTFile = std::filesystem::current_path();
    UTFile = UTFile.parent_path();
    UTFile /= "tests/UnitTestFiles/ut_bigcore.json";
    inRoot = readInputFile(UTFile.c_str());
    cpusInfo[0].inputFile.bufferPtr = inRoot;
    RunTimeInfo runTimeInfo;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        cpusInfo[cpu].inputFile.bufferPtr = inRoot;
    }

    InSequence s;

    // BigCore
    for (CPUInfo& cpu : crashdump.cpusInfo)
    {
        int coresActive = 0;

        for (uint8_t u8CoreNum = 0; u8CoreNum < MAX_CORE_MASK; u8CoreNum++)
        {
            if (!CHECK_BIT(cpu.coreMask, u8CoreNum))
            {
                continue;
            }
            if (CHECK_BIT(cpu.crashedCoreMask, u8CoreNum))
            {
                continue;
            }
            coresActive++;
        }

        uint8_t cc = 0x40;
        uint8_t u8CrashdumpEnabled = 0;
        uint16_t u16CrashdumpNumAgents = 2;
        uint8_t u64UniqueId[8] = {0xFF, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00};
        uint8_t PayloadExp[8] = {0x03, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00};
        uint8_t Data[8] = {0xef, 0xbe, 0xad, 0xde, 0xef, 0xbe, 0xad, 0xde};

        EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_Discovery)
            .WillOnce(DoAll(SetArgPointee<6>(u8CrashdumpEnabled),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArgPointee<6>(u16CrashdumpNumAgents),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<6>(u64UniqueId, u64UniqueId + 8),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<6>(PayloadExp, PayloadExp + 8),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));

        EXPECT_CALL(*crashdump.libPeciMock, peci_CrashDump_GetFrame)
            .Times(13235 * 2 * coresActive)
            .WillRepeatedly(DoAll(SetArrayArgument<5>(Data, Data + 8),
                                  SetArgPointee<6>(cc),
                                  Return(PECI_CC_SUCCESS)));
    }

    struct timespec sectionStart;
    struct timespec globalStart;
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    clock_gettime(CLOCK_MONOTONIC, &globalStart);
    runTimeInfo.globalRunTime = globalStart;
    runTimeInfo.sectionRunTime = sectionStart;
    runTimeInfo.maxGlobalTime = 700;
    for (int cpu = 0; cpu < (int)cpusInfo.size(); cpu++)
    {
        sleep(1);
        status = fillNewSection(outRoot, &cpusInfo[cpu], cpu, &runTimeInfo, 0);
    }

    char* sOut = cJSON_Print(outRoot);
    std::fstream fpJson;
    fpJson.open(out_file.c_str(), std::ios::out);
    if (fpJson)
    {
        fpJson << sOut;
        fpJson.close();
    }
}
