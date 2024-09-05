/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2022 Intel Corporation.
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

#include "test_crashdump.hpp"
using namespace ::testing;

void initCrashdump()
{
    if (nullptr != crashdump::conn.get())
    {
        crashdump::conn =
            std::make_shared<sdbusplus::asio::connection>(crashdump::io);

        crashdump::conn->request_name(crashdump::crashdumpService);
        crashdump::server =
            std::make_shared<sdbusplus::asio::object_server>(crashdump::conn);

        crashdump::storedLogIfaces.reserve(crashdump::numStoredLogs);

        std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceStored =
            crashdump::server->add_interface(
                crashdump::crashdumpPath, crashdump::crashdumpStoredInterface);
    }
}

TestCrashdump::TestCrashdump(Model model)
{
    initializeModelMap();
    initCrashdump();

    libPeciMock = std::make_unique<NiceMock<LibPeciMock>>();

    CPUInfo cpuInfo;
    // Initialize cpuInfo
    cpuInfo.clientAddr = 48;
    cpuInfo.model = model;
    cpuInfo.coreMask = 0x0000db7e;
    cpuInfo.chaCount = 0;
    cpuInfo.crashedCoreMask = 0x0;
    cpusInfo.push_back(cpuInfo);

    root = cJSON_CreateObject();
    setupInputFiles();
}

TestCrashdump::TestCrashdump(Model model, int delay)
{
    initializeModelMap();
    initCrashdump();

    libPeciMock = std::make_unique<NiceMock<LibPeciMockWithDelay>>(delay);

    CPUInfo cpuInfo;
    // Initialize cpuInfo
    cpuInfo.clientAddr = 48;
    cpuInfo.model = model;
    cpuInfo.coreMask = 0x0000db7e;
    cpuInfo.chaCount = 0;
    cpuInfo.crashedCoreMask = 0x0;
    cpuInfo.chaCountRead = {};
    cpuInfo.cpuidRead = {};
    cpuInfo.coreMaskRead = {};
    cpusInfo.push_back(cpuInfo);

    root = cJSON_CreateObject();
    setupInputFiles();
}

TestCrashdump::TestCrashdump(Model model, int delay, int numberOfCpus)
{
    initializeModelMap();
    initCrashdump();

    libPeciMock = std::make_unique<NiceMock<LibPeciMockWithDelay>>(delay);

    for (int c = 0; c < numberOfCpus; c++)
    {
        CPUInfo cpuInfo;
        // Initialize cpuInfo
        cpuInfo.clientAddr = 48 + c;
        cpuInfo.model = model;
        cpuInfo.coreMask = 0x0000db7e;
        cpuInfo.chaCount = 0;
        cpuInfo.crashedCoreMask = 0x0;
        cpuInfo.chaCountRead = {};
        cpuInfo.cpuidRead = {};
        cpuInfo.coreMaskRead = {};
        cpusInfo.push_back(cpuInfo);
    }

    root = cJSON_CreateObject();
    setupInputFiles();
}

TestCrashdump::TestCrashdump(std::vector<CPUInfo>& cpuInfo_) :
    cpusInfo(cpuInfo_)
{
    initializeModelMap();
    initCrashdump();

    libPeciMock = std::make_unique<NiceMock<LibPeciMock>>();
    root = cJSON_CreateObject();
    setupInputFiles();
}

TestCrashdump::~TestCrashdump()
{
    libPeciMock.reset();
    cpusInfo.clear();
    removeInputFiles();
}

void TestCrashdump::initializeModelMap()
{
    cpuModelMap = {{cd_icx, "icx"},
                   {cd_icx2, "icx"},
                   {cd_spr, "spr"},
                   {cd_icxd, "icx"},
                   {cd_emr, "emr"}};
}

void TestCrashdump::removeInputFiles()
{
    std::filesystem::remove_all(targetPath);
}

void TestCrashdump::setupInputFiles()
{
    try
    {
        std::string fileName =
            "crashdump_input_" + cpuModelMap[cpusInfo[0].model] + ".json";
        std::filesystem::path file =
            std::filesystem::current_path() / ".." / fileName;
        copyInputFilesToDefaultLocation(file);
        crashdump::loadInputFiles(cpusInfo, &inputFileInfo, false);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TestCrashdump::copyInputFilesToDefaultLocation(
    std::filesystem::path sourceFile)
{
    try
    {
        bool status = std::filesystem::create_directories(targetPath);

        if (status)
        {
            std::filesystem::copy(sourceFile, targetPath,
                                  std::filesystem::copy_options::recursive);
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }
}

std::unique_ptr<LibPeciMock> TestCrashdump::libPeciMock;

EPECIStatus peci_CrashDump_Discovery(uint8_t target, uint8_t subopcode,
                                     uint8_t param0, uint16_t param1,
                                     uint8_t param2, uint8_t u8ReadLen,
                                     uint8_t* pData, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_CrashDump_Discovery(
        target, subopcode, param0, param1, param2, u8ReadLen, pData, cc);
}

EPECIStatus peci_CrashDump_GetFrame(uint8_t target, uint16_t param0,
                                    uint16_t param1, uint16_t param2,
                                    uint8_t u8ReadLen, uint8_t* pData,
                                    uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_CrashDump_GetFrame(
        target, param0, param1, param2, u8ReadLen, pData, cc);
}

EPECIStatus peci_Ping(uint8_t target)
{
    return TestCrashdump::libPeciMock->peci_Ping(target);
}

EPECIStatus peci_GetCPUID(const uint8_t clientAddr, CPUModel* cpuModel,
                          uint8_t* stepping, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_GetCPUID(clientAddr, cpuModel,
                                                     stepping, cc);
}

void peci_Unlock(int peci_fd)
{
    return TestCrashdump::libPeciMock->peci_Unlock(peci_fd);
}

EPECIStatus peci_Lock(int* peci_fd, int timeout_ms)
{
    return TestCrashdump::libPeciMock->peci_Lock(peci_fd, timeout_ms);
}

EPECIStatus peci_RdEndPointConfigPciLocal_seq(uint8_t target, uint8_t u8Seg,
                                              uint8_t u8Bus, uint8_t u8Device,
                                              uint8_t u8Fcn, uint16_t u16Reg,
                                              uint8_t u8ReadLen,
                                              uint8_t* pPCIData, int peci_fd,
                                              uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_RdEndPointConfigPciLocal_seq(
        target, u8Seg, u8Bus, u8Device, u8Fcn, u16Reg, u8ReadLen, pPCIData,
        peci_fd, cc);
}

EPECIStatus peci_RdEndPointConfigPciLocal(uint8_t target, uint8_t u8Seg,
                                          uint8_t u8Bus, uint8_t u8Device,
                                          uint8_t u8Fcn, uint16_t u16Reg,
                                          uint8_t u8ReadLen, uint8_t* pPCIData,
                                          uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_RdEndPointConfigPciLocal(
        target, u8Seg, u8Bus, u8Device, u8Fcn, u16Reg, u8ReadLen, pPCIData, cc);
}

EPECIStatus peci_RdPkgConfig(uint8_t target, uint8_t u8Index, uint16_t u16Value,
                             uint8_t u8ReadLen, uint8_t* pPkgConfig,
                             uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_RdPkgConfig(
        target, u8Index, u16Value, u8ReadLen, pPkgConfig, cc);
}

EPECIStatus peci_WrPkgConfig(uint8_t target, uint8_t u8Index, uint16_t u16Param,
                             uint32_t u32Value, uint8_t u8WriteLen, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_WrPkgConfig(
        target, u8Index, u16Param, u32Value, u8WriteLen, cc);
}

EPECIStatus peci_RdIAMSR(uint8_t target, uint8_t threadID, uint16_t MSRAddress,
                         uint64_t* u64MsrVal, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_RdIAMSR(target, threadID,
                                                    MSRAddress, u64MsrVal, cc);
}

EPECIStatus peci_RdEndPointConfigMmio(uint8_t target, uint8_t u8Seg,
                                      uint8_t u8Bus, uint8_t u8Device,
                                      uint8_t u8Fcn, uint8_t u8Bar,
                                      uint8_t u8AddrType, uint64_t u64Offset,
                                      uint8_t u8ReadLen, uint8_t* pMmioData,
                                      uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_RdEndPointConfigMmio(
        target, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType, u64Offset,
        u8ReadLen, pMmioData, cc);
}

EPECIStatus peci_RdEndPointConfigMmio_seq(
    uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
    uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType, uint64_t u64Offset,
    uint8_t u8ReadLen, uint8_t* pMmioData, int peci_fd, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_RdEndPointConfigMmio_seq(
        target, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType, u64Offset,
        u8ReadLen, pMmioData, peci_fd, cc);
}

EPECIStatus peci_Telemetry_Discovery(uint8_t target, uint8_t subopcode,
                                     uint8_t param0, uint16_t param1,
                                     uint8_t param2, uint8_t u8ReadLen,
                                     uint8_t* pData, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_Telemetry_Discovery(
        target, subopcode, param0, param1, param2, u8ReadLen, pData, cc);
}

EPECIStatus peci_Telemetry_GetCrashlogSample(uint8_t target, uint16_t index,
                                             uint16_t sampleId,
                                             uint8_t u8ReadLen, uint8_t* pData,
                                             uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_Telemetry_GetCrashlogSample(
        target, index, sampleId, u8ReadLen, pData, cc);
}

EPECIStatus peci_Telemetry_ConfigWatcherRd(uint8_t target, uint16_t watcher,
                                           uint16_t offset, uint8_t u8ReadLen,
                                           uint8_t* pData, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_Telemetry_ConfigWatcherRd(
        target, watcher, offset, u8ReadLen, pData, cc);
}

EPECIStatus peci_Telemetry_ConfigWatcherWr(uint8_t target, uint16_t watcher,
                                           uint16_t offset, uint8_t u8DataLen,
                                           uint8_t* pData, uint8_t* cc)
{
    return TestCrashdump::libPeciMock->peci_Telemetry_ConfigWatcherWr(
        target, watcher, offset, u8DataLen, pData, cc);
}