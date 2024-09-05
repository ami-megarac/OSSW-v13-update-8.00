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

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <peci.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
//#include <systemd/sd-id128.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <future>
#include <regex>
//#include <sdbusplus/asio/object_server.hpp>
#include <sstream>
#include <vector>
#include "libpeci4.h"
extern "C" {
#include "cJSON.h"


#include "CrashdumpSections/BigCore.h"
#include "CrashdumpSections/TorDump.h"
#include "CrashdumpSections/flow.h"
#include "CrashdumpSections/crashdump.h"
#ifdef CONFIG_SPX_FEATURE_OEMDATA_SECTION
#include "CrashdumpSections/OemData.h"
#endif
#include "CrashdumpSections/utils.h"
//#include "safe_str_lib.h"
}

#include "crashdump.hpp"
#include "utils_triage.hpp"
#ifdef SPX_BMC_ACD
#define BAFI_APP "/usr/local/bin/acdbafigenerator -t "
#define CRASHDUMP_FILE "/var/crashdump/json/crashdump.json"
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI_DEV_MAP
#define BAFI_DEV_MAP "-p /home/crashdump/default_device_map.json "
#endif
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI_MEM_MAP
#define BAFI_MEM_MAP "-m /home/crashdump/default_memory_map.json "
#endif
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI_SILK_MAP
#define BAFI_SILK_MAP "-s /home/crashdump/default_silkscreen_map.json "
#endif
#endif

//#include <boost/asio/io_service.hpp>
namespace crashdump
{
#ifndef SPX_BMC_ACD
static std::vector<crashdump::CPUInfo> cpuInfo;
static boost::asio::io_service io;
static std::shared_ptr<sdbusplus::asio::connection> conn;
static std::shared_ptr<sdbusplus::asio::object_server> server;
static std::shared_ptr<sdbusplus::asio::dbus_interface> logIface;
static std::vector<
    std::pair<std::string, std::shared_ptr<sdbusplus::asio::dbus_interface>>>
    storedLogIfaces;

constexpr char const* crashdumpService = "com.intel.crashdump";
constexpr char const* crashdumpPath = "/com/intel/crashdump";
#else
static std::vector<CPUInfo> cpuInfo;
constexpr char const* crashdumpPath = "/var/crashdump/json";
#endif
#ifndef SPX_BMC_ACD
constexpr char const* crashdumpInterface = "com.intel.crashdump";
constexpr char const* crashdumpOnDemandPath = "/com/intel/crashdump/OnDemand";
constexpr char const* crashdumpTelemetryPath = "/com/intel/crashdump/Telemetry";
constexpr char const* crashdumpStoredInterface = "com.intel.crashdump.Stored";
constexpr char const* crashdumpDeleteAllInterface =
    "xyz.openbmc_project.Collection.DeleteAll";
constexpr char const* crashdumpOnDemandInterface =
    "com.intel.crashdump.OnDemand";
constexpr char const* crashdumpTelemetryInterface =
    "com.intel.crashdump.Telemetry";
constexpr char const* crashdumpRawPeciInterface =
    "com.intel.crashdump.SendRawPeci";
//static const std::filesystem::path crashdumpDir = "/tmp/crashdumps";
static const std::filesystem::path crashdumpDir = "/tmp/crashdump/output";
static const std::string crashdumpFileRoot{"crashdump_ondemand_"};
static const std::string crashdumpTelemetryFileRoot{"telemetry_"};
static const std::string crashdumpPrefix{"crashdump_"};
#else
static const std::string crashdumpPrefix{"crashdump"};
static const std::filesystem::path crashdumpDir = "/var/crashdump/json";
static const std::filesystem::path crashdumpLockPath = "/var";
#endif

constexpr char const* triggerTypeOnDemand = "On-Demand";
constexpr char const* triggerTypeIERR = "IERR";
constexpr int const vcuPeciWake = 5;

static PlatformState platformState = {false, DEFAULT_VALUE, DEFAULT_VALUE,
                                      DEFAULT_VALUE, DEFAULT_VALUE};

static void logRunTime(cJSON* parent, timespec* start, char* key)
{
    char timeString[64];
    timespec finish = {};
    uint64_t timeVal = 0;

    clock_gettime(CLOCK_MONOTONIC, &finish);
    uint64_t runTimeInNs = tsToNanosecond(&finish) - tsToNanosecond(start);

    timeVal = runTimeInNs;

    // only log the last metaData run
    cJSON_GetObjectItemCaseSensitive(parent, "_time");

    cd_snprintf_s(timeString, sizeof(timeString), "%.2fs",
                  (double)timeVal / 1e9);
    cJSON_AddStringToObject(parent, key, timeString);

    clock_gettime(CLOCK_MONOTONIC, start);
}
/******************************************************************************
 *
 *   fillBmcVersionJson
 *
 *   This function fills in the bmc_fw_ver JSON info
 *
 ******************************************************************************/
int fillBmcVersion(char* cSectionName, cJSON* pJsonChild)
{
    char bmcVersion[SI_BMC_VER_LEN] = {0};
    crashdump::getBMCVersionDBus(bmcVersion, sizeof(bmcVersion));
    // Fill in BMC Version string
    cJSON_AddStringToObject(pJsonChild, cSectionName, bmcVersion);
    return ACD_SUCCESS;
}
/******************************************************************************
 *
 *   fillBiosIdJson
 *
 *   This function fills in the bios_id JSON info
 *
 ******************************************************************************/
int fillBiosId(char* cSectionName, cJSON* pJsonChild)
{
    char biosVersion[SI_BIOS_ID_LEN] = {0};
    crashdump::getBIOSVersionDBus(biosVersion, sizeof(biosVersion));
    // Fill in BIOS Version string
    cJSON_AddStringToObject(pJsonChild, cSectionName, biosVersion);
    return ACD_SUCCESS;
}
static const std::string getUuid()
{
    std::string ret;
#ifndef SPX_BMC_ACD
    sd_id128_t appId = SD_ID128_MAKE(e0, e1, 73, 76, 64, 61, 47, da, a5, 0c, d0,
                                     cc, 64, 12, 45, 78);
    sd_id128_t machineId = SD_ID128_NULL;

    if (sd_id128_get_machine_app_specific(appId, &machineId) == 0)
    {
        std::array<char, SD_ID128_STRING_MAX> str;
        ret = sd_id128_to_string(machineId, str.data());
        ret.insert(8, 1, '-');
        ret.insert(13, 1, '-');
        ret.insert(18, 1, '-');
        ret.insert(23, 1, '-');
    }
#endif

    return ret;
}

static void getClientAddrs(std::vector<CPUInfo>& cpuInfo)
{
#ifndef SPX_BMC_ACD
    for (int cpu = 0, addr = MIN_CLIENT_ADDR; addr <= MAX_CLIENT_ADDR;
         cpu++, addr++)
#else
    for (int cpu = 0, addr = MIN_CLIENT_ADDR; cpu < MAX_CPUS && addr <= MAX_CLIENT_ADDR;
         cpu++, addr++)
#endif
    {
        if (peci_Ping(addr) == PECI_CC_SUCCESS)
        {
            cpuInfo.emplace_back();
            cpuInfo[cpu].clientAddr = addr;
        }
    }
}

static bool savePeciWake(std::vector<CPUInfo>& cpuInfo)
{
    uint8_t cc = 0;
    EPECIStatus retval = PECI_CC_SUCCESS;
    uint32_t peciRdValue = 0;
    for (CPUInfo& cpu : cpuInfo)
    {
        retval =
            peci_RdPkgConfig(cpu.clientAddr, vcuPeciWake, ON, sizeof(uint32_t),
                             (uint8_t*)&peciRdValue, &cc);
        if (retval != PECI_CC_SUCCESS)
        {
            CRASHDUMP_PRINT(
                ERR, stderr,
                "Cannot read Wake_on_PECI-> addr: (0x%x), ret: (0x%x), \
                        cc: (0x%x)\n",
                cpu.clientAddr, retval, cc);
            cpu.initialPeciWake = UNKNOWN;
            continue;
        }
        cpu.initialPeciWake = (peciRdValue & 0x1) ? ON : OFF;
    }
    return true;
}

static bool setPeciWake(std::vector<CPUInfo>& cpuInfo, pwState desiredState)
{
    uint8_t cc = 0;
    EPECIStatus retval = PECI_CC_SUCCESS;
    int writeValue = OFF;
    for (CPUInfo& cpu : cpuInfo)
    {
        if ((cpu.initialPeciWake == ON) || (cpu.initialPeciWake == UNKNOWN))
            continue;
        writeValue = static_cast<int>(desiredState);
        retval = peci_WrPkgConfig(cpu.clientAddr, vcuPeciWake, writeValue,
                                  writeValue, sizeof(uint32_t), &cc);
        if (retval != PECI_CC_SUCCESS)
        {
            CRASHDUMP_PRINT(
                ERR, stderr,
                "Cannot set Wake_on_PECI-> addr: (0x%x), ret: (0x%x), cc: "
                "(0x%x)\n",
                cpu.clientAddr, retval, cc);
        }
    }
    return true;
}

static bool checkPeciWake(std::vector<CPUInfo>& cpuInfo)
{
    uint8_t cc = 0;
    EPECIStatus retval = PECI_CC_SUCCESS;
    uint32_t peciRdValue = 0;
    for (CPUInfo& cpu : cpuInfo)
    {
        retval =
            peci_RdPkgConfig(cpu.clientAddr, vcuPeciWake, ON, sizeof(uint32_t),
                             (uint8_t*)&peciRdValue, &cc);
        if (retval != PECI_CC_SUCCESS)
        {
            CRASHDUMP_PRINT(
                ERR, stderr,
                "Cannot read Wake_on_PECI-> addr: (0x%x), ret: (0x%x), \
                        cc: (0x%x)\n",
                cpu.clientAddr, retval, cc);
            continue;
        }
        if (peciRdValue != 1)
        {
            CRASHDUMP_PRINT(ERR, stderr, "Wake_on_PECI in OFF state: (0x%x)\n",
                            cpu.clientAddr);
        }
    }
    return true;
}

void setResetDetected()
{
    if (!platformState.resetDetected)
    {
        platformState.resetDetected = true;
        platformState.resetCpu = platformState.currentCpu;
        platformState.resetSection = platformState.currentSection;
    }
}

inline void clearResetDetected()
{
    platformState.resetDetected = false;
    platformState.resetCpu = DEFAULT_VALUE;
    platformState.resetSection = DEFAULT_VALUE;
    platformState.currentSection = DEFAULT_VALUE;
    platformState.currentCpu = DEFAULT_VALUE;
}

inline void updateCurrentSection(const CrashdumpSection& sectionName,
                                 const CPUInfo* cpuInfo)
{
    platformState.currentSection = sectionName.section;
    platformState.currentCpu = cpuInfo->clientAddr & 0xF;
}

acdStatus loadInputFiles(std::vector<CPUInfo>& cpuInfo,
                         InputFileInfo* inputFileInfo, bool isTelemetry)
{
    int uniqueCount = 0;
    cJSON* defaultStateSection = NULL;
    acdStatus status = ACD_SUCCESS;

    for (CPUInfo& cpu : cpuInfo)
    {
        // read and allocate memory for crashdump input file
        // if it hasn't been read before
        if (inputFileInfo->buffers[cpu.model] == NULL)
        {
            inputFileInfo->buffers[cpu.model] = selectAndReadInputFile(
                cpu.model, &inputFileInfo->filenames[cpu.model], isTelemetry);
            if (inputFileInfo->buffers[cpu.model] != NULL)
            {
                uniqueCount++;
            }
        }

        inputFileInfo->unique = (uniqueCount <= 1);
        cpu.inputFile.filenamePtr = inputFileInfo->filenames[cpu.model];
        cpu.inputFile.bufferPtr = inputFileInfo->buffers[cpu.model];

        // Get and Check global enable/disable value from "DefaultState"
        defaultStateSection = cJSON_GetObjectItemCaseSensitive(
            inputFileInfo->buffers[cpu.model], "DefaultState");
        int defaultStateEnable = 1;
        if (defaultStateSection != NULL)
        {
            defaultStateEnable = strncmp(defaultStateSection->valuestring, "Enable", CRASHDUMP_VALUE_LEN);
            if (defaultStateEnable == 0)
            {
                status = ACD_SUCCESS;
	    }
	    else
            {
		defaultStateEnable = 1;
                defaultStateEnable = memcmp(defaultStateSection->valuestring,
                         "Disable", strlen("Disable"));
                if (defaultStateEnable == 0)
                {
                    status = ACD_INPUT_FILE_ERROR;
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Exiting, \"DefaultState\" set to Disable in: %s\n",
                        inputFileInfo->filenames[cpu.model]);
                }
                else
                {
                    status = ACD_SUCCESS;
                    CRASHDUMP_PRINT(ERR, stderr,
                                    "\"DefaultState\" (%s) is neither "
                                    "Enable/Disable in: %s\n",
                                    defaultStateSection->valuestring,
                                    inputFileInfo->filenames[cpu.model]);
                }
            }
        }
        else
        {
            status = ACD_SUCCESS;
        }
    }

    return status;
}

static bool getCoreMasks(std::vector<CPUInfo>& cpuInfo, cpuidState cpuState)
{
    uint8_t cc = 0;
    EPECIStatus retval = PECI_CC_SUCCESS;

    for (CPUInfo& cpu : cpuInfo)
    {
        if (cpu.coreMaskRead.coreMaskValid)
        {
            continue;
        }
        uint32_t coreMask0 = 0x0;
        uint32_t coreMask1 = 0x0;
        cpu.coreMask = 0;
        switch (cpu.model)
        {
            case cd_icx:
            case cd_icx2:
            case cd_icxd:
                // RESOLVED_CORES Local PCI B14:D30:F3 Reg 0xD0 and 0xD4
                retval = peci_RdPCIConfigLocal(cpu.clientAddr, 14, 30, 3, 0xD0,
                                               sizeof(coreMask0),
                                               (uint8_t*)&coreMask0, &cc);
                cpu.coreMaskRead.coreMaskCc = cc;
                cpu.coreMaskRead.coreMaskRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find coreMask0! ret: (0x%x), cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                retval = peci_RdPCIConfigLocal(cpu.clientAddr, 14, 30, 3, 0xD4,
                                               sizeof(coreMask1),
                                               (uint8_t*)&coreMask1, &cc);
                cpu.coreMaskRead.coreMaskCc = cc;
                cpu.coreMaskRead.coreMaskRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find coreMask1! ret: (0x%x), cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                cpu.coreMaskRead.coreMaskValid = true;
                break;
            case cd_emr:
            case cd_spr:
            case cd_sprhbm:
                // RESOLVED_CORES EP Local PCI B31:D30:F6 Reg 0x80 and 0x84
                retval = peci_RdEndPointConfigPciLocal(
                    cpu.clientAddr, 0, 31, 30, 6, 0x80, sizeof(coreMask0),
                    (uint8_t*)&coreMask0, &cc);
                cpu.coreMaskRead.coreMaskCc = cc;
                cpu.coreMaskRead.coreMaskRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find coreMask0! ret: (0x%x), cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                retval = peci_RdEndPointConfigPciLocal(
                    cpu.clientAddr, 0, 31, 30, 6, 0x84, sizeof(coreMask1),
                    (uint8_t*)&coreMask1, &cc);
                cpu.coreMaskRead.coreMaskCc = cc;
                cpu.coreMaskRead.coreMaskRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find coreMask1! ret: (0x%x), cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                cpu.coreMaskRead.coreMaskValid = true;
                break;
            default:
                return false;
        }
        if (cpu.coreMaskRead.coreMaskValid)
        {
            cpu.coreMask = coreMask1;
            cpu.coreMask <<= 32;
            cpu.coreMask |= coreMask0;
            cpu.coreMaskRead.source = cpuState;
        }
    }
    return true;
}

static bool getCHACounts(std::vector<CPUInfo>& cpuInfo, cpuidState cpuState)
{
    uint8_t cc = 0;
    EPECIStatus retval = PECI_CC_SUCCESS;

    for (CPUInfo& cpu : cpuInfo)
    {
        if (cpu.chaCountRead.chaCountValid)
        {
            continue;
        }
        uint32_t chaMask0 = 0x0;
        uint32_t chaMask1 = 0x0;
        cpu.chaCount = 0;
        switch (cpu.model)
        {
            case cd_icx:
            case cd_icx2:
            case cd_icxd:
                // LLC_SLICE_EN Local PCI B14:D30:F3 Reg 0x9C and 0xA0
                retval = peci_RdPCIConfigLocal(cpu.clientAddr, 14, 30, 3, 0x9C,
                                               sizeof(chaMask0),
                                               (uint8_t*)&chaMask0, &cc);
                cpu.chaCountRead.chaCountCc = cc;
                cpu.chaCountRead.chaCountRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find chaMask0! ret: (0x%x),  cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                retval = peci_RdPCIConfigLocal(cpu.clientAddr, 14, 30, 3, 0xA0,
                                               sizeof(chaMask1),
                                               (uint8_t*)&chaMask1, &cc);
                cpu.chaCountRead.chaCountCc = cc;
                cpu.chaCountRead.chaCountRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find chaMask1! ret: (0x%x), cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                cpu.chaCountRead.chaCountValid = true;
                break;
            case cd_emr:
            case cd_spr:
            case cd_sprhbm:
                // LLC_SLICE_EN EP Local PCI B31:D30:F3 Reg 0x9C and 0xA0
                retval = peci_RdEndPointConfigPciLocal(
                    cpu.clientAddr, 0, 31, 30, 3, 0x9c, sizeof(chaMask0),
                    (uint8_t*)&chaMask0, &cc);
                cpu.chaCountRead.chaCountCc = cc;
                cpu.chaCountRead.chaCountRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find chaMask0! ret: (0x%x),  cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                retval = peci_RdEndPointConfigPciLocal(
                    cpu.clientAddr, 0, 31, 30, 3, 0xa0, sizeof(chaMask1),
                    (uint8_t*)&chaMask1, &cc);
                cpu.chaCountRead.chaCountCc = cc;
                cpu.chaCountRead.chaCountRet = retval;
                if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "Cannot find chaMask1! ret: (0x%x), cc: (0x%x)\n",
                        retval, cc);
                    break;
                }
                cpu.chaCountRead.chaCountValid = true;
                break;
            default:
                return false;
        }
        if (cpu.chaCountRead.chaCountValid)
        {
            cpu.chaCount =
                __builtin_popcount(chaMask0) + __builtin_popcount(chaMask1);
            cpu.chaCountRead.source = cpuState;
        }
    }
    return true;
}

static void getCPUID(CPUInfo& cpuInfo)
{
    uint8_t cc = 0;
    CPUModel cpuModel{};
    uint8_t stepping = 0;
    EPECIStatus retval = PECI_CC_SUCCESS;

    retval = peci_GetCPUID(cpuInfo.clientAddr, &cpuModel, &stepping, &cc);
    cpuInfo.cpuidRead.cpuModel = cpuModel;
    cpuInfo.cpuidRead.stepping = stepping;
    cpuInfo.cpuidRead.cpuidCc = cc;
    cpuInfo.cpuidRead.cpuidRet = retval;
    if (retval != PECI_CC_SUCCESS || PECI_CC_UA(cc))
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "Cannot get CPUID! ret: (0x%x), cc: (0x%x)\n", retval,
                        cc);
    }
}

static void parseCPUInfo(CPUInfo& cpuInfo, cpuidState cpuState)
{
    switch ((int)cpuInfo.cpuidRead.cpuModel)
    {
        case icx:
            CRASHDUMP_PRINT(INFO, stderr, "ICX detected (CPUID 0x%x)\n",
                            cpuInfo.cpuidRead.cpuModel |
                                cpuInfo.cpuidRead.stepping);
            if (cpuInfo.cpuidRead.stepping >= STEPPING_ICX2)
            {
                cpuInfo.model = cd_icx2;
            }
            else
            {
                cpuInfo.model = cd_icx;
            }
            cpuInfo.cpuidRead.cpuidValid = true;
            cpuInfo.cpuidRead.source = cpuState;
            break;
        case EMR_MODEL:
            CRASHDUMP_PRINT(INFO, stderr, "EMR detected (CPUID 0x%x)\n",
                            cpuInfo.cpuidRead.cpuModel |
                                cpuInfo.cpuidRead.stepping);
            cpuInfo.model = cd_emr;
            cpuInfo.cpuidRead.cpuidValid = true;
            cpuInfo.cpuidRead.source = cpuState;
            break;
        case SPR_MODEL:
            if (isSprHbm(&cpuInfo))
            {
                CRASHDUMP_PRINT(INFO, stderr, "SPR-HBM detected (CPUID 0x%x)\n",
                                cpuInfo.cpuidRead.cpuModel |
                                    cpuInfo.cpuidRead.stepping);
                cpuInfo.model = cd_sprhbm;
            }
            else
            {
                CRASHDUMP_PRINT(INFO, stderr, "SPR detected (CPUID 0x%x)\n",
                                cpuInfo.cpuidRead.cpuModel |
                                    cpuInfo.cpuidRead.stepping);
                cpuInfo.model = cd_spr;
            }
            cpuInfo.cpuidRead.cpuidValid = true;
            cpuInfo.cpuidRead.source = cpuState;
            break;
        case ICXD_MODEL:
            CRASHDUMP_PRINT(INFO, stderr, "ICXD detected (CPUID 0x%x)\n",
                            cpuInfo.cpuidRead.cpuModel |
                                cpuInfo.cpuidRead.stepping);
            cpuInfo.model = cd_icxd;
            cpuInfo.cpuidRead.cpuidValid = true;
            cpuInfo.cpuidRead.source = cpuState;
            break;
        default:
            CRASHDUMP_PRINT(ERR, stderr, "Unsupported CPUID 0x%x\n",
                            cpuInfo.cpuidRead.cpuModel |
                                cpuInfo.cpuidRead.stepping);
            cpuInfo.cpuidRead.cpuidValid = false;
            break;
    }
}

static void overwriteCPUInfo(std::vector<CPUInfo>& cpuInfo)
{
    bool found = false;
    Model defaultModel = (Model)0x0;
    CPUModel defaultCpuModel = (CPUModel)0x0;
    uint8_t defaultStepping = 0x0;
    for (CPUInfo& cpu : cpuInfo)
    {
        if (cpu.cpuidRead.cpuidValid)
        {
            defaultModel = cpu.model;
            defaultCpuModel = cpu.cpuidRead.cpuModel;
            defaultStepping = cpu.cpuidRead.stepping;
            found = true;
            break;
        }
    }
    for (CPUInfo& cpu : cpuInfo)
    {
        if (!cpu.cpuidRead.cpuidValid)
        {
            if (found)
            {
                cpu.model = defaultModel;
                cpu.cpuidRead.cpuModel = defaultCpuModel;
                cpu.cpuidRead.stepping = defaultStepping;
                cpu.cpuidRead.source = OVERWRITTEN;
                cpu.cpuidRead.cpuidCc = PECI_DEV_CC_SUCCESS;
                cpu.cpuidRead.cpuidRet = PECI_CC_SUCCESS;
                cpu.cpuidRead.cpuidValid = true;
            }
            else
            {
                cpu.cpuidRead.source = INVALID;
            }
        }
    }
}

void initCPUInfo(std::vector<CPUInfo>& cpuInfo)
{
    if (cpuInfo.empty())
    {
        cpuInfo.reserve(MAX_CPUS);
        getClientAddrs(cpuInfo);
        for (CPUInfo& cpu : cpuInfo)
        {
            cpu.coreMaskRead.coreMaskValid = false;
            cpu.chaCountRead.chaCountValid = false;
            cpu.cpuidRead.cpuidValid = false;
            cpu.coreMaskRead.source = INVALID;
            cpu.chaCountRead.source = INVALID;
            cpu.cpuidRead.source = INVALID;
            cpu.model = (Model)0x0;
            cpu.cpuidRead.cpuModel = (CPUModel)0x0;
        }
    }
}

void getCPUData(std::vector<CPUInfo>& cpuInfo, cpuidState cpuState)
{
    if (cpuInfo.empty())
    {
        CRASHDUMP_PRINT(ERR, stderr,
                        "cpuInfo is empty, no PECI addresses are available\n");
    }
    for (CPUInfo& cpu : cpuInfo)
    {
        if (!cpu.cpuidRead.cpuidValid || (cpu.cpuidRead.source == OVERWRITTEN))
        {
            getCPUID(cpu);
            parseCPUInfo(cpu, cpuState);
        }
    }
    if (cpuState == EVENT)
    {
        overwriteCPUInfo(cpuInfo);
    }
    getCoreMasks(cpuInfo, cpuState);
    getCHACounts(cpuInfo, cpuState);
}

std::string newTimestamp(void)
{
    char logTime[64] = {0};
    /*Reason for False Positive - ARN A7 is 32 bits CPU, not 64 bits cpu*/
    /* coverity[declaration_with_small_time_t: FALSE] */
    time_t curtime;
    struct tm* loctime;

    // Add the timestamp
    curtime = time(NULL);
    loctime = localtime(&curtime);
    if (NULL != loctime)
    {
        strftime(logTime, sizeof(logTime), "%FT%TZ", loctime);
    }
    std::string tmp(logTime);
    return tmp;
}

static void logTimestamp(cJSON* parent, std::string& logTime)
{
    cJSON_AddStringToObject(parent, "timestamp", logTime.c_str());
}

static void logTriggerType(cJSON* parent, const std::string& triggerType)
{
    cJSON_AddStringToObject(parent, "trigger_type", triggerType.c_str());
}

static void logPlatformName(cJSON* parent)
{
    cJSON_AddStringToObject(parent, "platform_name", getUuid().c_str());
}

static void logCrashdumpVersion(cJSON* parent)
{
    cJSON_AddStringToObject(parent, "crashdump_ver", CRASHDUMP_VER);
}

static void logMEVersion(cJSON* parent)
{
    cJSON_AddStringToObject(parent, "me_fw_ver", MD_NA);
}

static void logMetaDataCommon(cJSON* parent, std::string& logTime,
                              const std::string& triggerType)
{
    logMEVersion(parent);
    logTimestamp(parent, logTime);
    logTriggerType(parent, triggerType);
    logPlatformName(parent);
    logCrashdumpVersion(parent);
}

static cJSON* addSectionLog(
    cJSON* parent, CPUInfo* cpuInfo, std::string sectionName,
    const std::function<int(CPUInfo* cpuInfo, cJSON*)> sectionLogFunc)
{
    CRASHDUMP_PRINT(INFO, stderr, "Logging %s on PECI address %d\n",
                    sectionName.c_str(), cpuInfo->clientAddr);

    // Create an empty JSON object for this section if it doesn't already
    // exist
    cJSON* logSectionJson;
    if ((logSectionJson = cJSON_GetObjectItemCaseSensitive(
             parent, sectionName.c_str())) == NULL)
    {
        cJSON_AddItemToObject(parent, sectionName.c_str(),
                              logSectionJson = cJSON_CreateObject());
    }

    // Get the log for this section
    int ret = 0;
    if ((ret = sectionLogFunc(cpuInfo, logSectionJson)) != ACD_SUCCESS)
    {
        CRASHDUMP_PRINT(ERR, stderr, "Error %d during %s log\n", ret,
                        sectionName.c_str());
    }

    // Check if child data is added to the JSON section
    if (logSectionJson->child == NULL)
    {
        // If there was supposed to be child data, add a failed status
        if (ret != 0)
        {
            cJSON_AddStringToObject(logSectionJson,
                                    crashdump::dbgStatusItemName,
                                    crashdump::dbgFailedStatus);
        }
    }
    return logSectionJson;
}
void logInputFileVersion(cJSON* root, CPUInfo cpuInfo,
                         InputFileInfo* inputFileInfo)
{
    cJSON* jsonVer = cJSON_GetObjectItemCaseSensitive(
        inputFileInfo->buffers[cpuInfo.model], "Version");

    if ((jsonVer != NULL) && cJSON_IsString(jsonVer))
    {
        cJSON_AddStringToObject(root, "_input_file_ver", jsonVer->valuestring);
    }
    else
    {
        cJSON_AddStringToObject(root, "_input_file_ver", MD_NA);
    }
}

static cJSON* addDisableSection(cJSON* parent, std::string sectionName)
{
    cJSON* logSection = NULL;
    cJSON* logSectionJson = NULL;
    logSection = cJSON_GetObjectItemCaseSensitive(parent, sectionName.c_str());
    if (logSection == NULL)
    {
        cJSON_AddItemToObject(parent, sectionName.c_str(),
                              logSectionJson = cJSON_CreateObject());
        cJSON_AddBoolToObject(logSectionJson, RECORD_ENABLE, false);
    }
    return logSectionJson;
}

void fillSection(
    cJSON* cpu, CPUInfo* cpuInfo, CrashdumpSection sectionName,
    const std::function<int(CPUInfo* cpuInfo, cJSON*)> sectionLogFunc,
    timespec* sectionStart, char* timeStr)
{
    cJSON* logSection = NULL;

    if (CHECK_BIT(cpuInfo->sectionMask, sectionName.section))
    {
        updateCurrentSection(sectionName, cpuInfo);

        logSection =
            addSectionLog(cpu, cpuInfo, sectionName.name, sectionLogFunc);

        // Do not log UNCORE & MCA_UNCORE version here
        if ((sectionName.section != Section::UNCORE) &&
            (sectionName.section != Section::MCA_UNCORE))
        {
            logCrashdumpVersion(logSection, cpuInfo, sectionName.record_type);
        }

        if (sectionName.section == Section::MCA_CORE)
        {
            updateMcaRunTime(logSection, sectionStart);
        }
        else
        {
            logRunTime(logSection, sectionStart, timeStr);
        }
    }
    else
    {
        addDisableSection(cpu, sectionName.name);
    }
}

void logMetaDataVersion(cJSON* metaData, CPUInfo cpuInfo,
                        InputFileInfo* inputFileInfo)
{
    int version = 0;
    cJSON* sections = cJSON_GetObjectItemCaseSensitive(
                                    inputFileInfo->buffers[cpuInfo.model],
                                    "Sections");
    if (sections != NULL)
    {
        cJSON* section = NULL;
        cJSON_ArrayForEach(section, sections)
        {
            if (cJSON_HasObjectItem(section, "Metadata_Cpu_Early"))
            {
                GenerateVersion(section->child, &version);
                char versionString[JSON_VAL_LEN];
                cd_snprintf_s(versionString, sizeof(versionString), "0x%x",
                                version);
                cJSON_AddStringToObject(metaData, "_version", versionString);
            }
        }
    }
    else {
        cJSON_AddStringToObject(metaData,"_version",
                                "ERROR inputFile is missing Sections region");
    }
}
void fillMetaDataCommon(cJSON* metaData, CPUInfo cpuInfo,
                        InputFileInfo* inputFileInfo,
                        const std::string& triggerType, std::string& timestamp,
                        timespec* crashdumpStart, timespec* sectionStart,
                        PlatformState platformState)
{
    if (metaData != NULL)
    {
        fillBmcVersion("bmc_fw_ver", metaData);
        fillBiosId("bios_id", metaData);
        logMetaDataCommon(metaData, timestamp, triggerType);
        logInputFileVersion(metaData, cpuInfo, inputFileInfo);

        if (inputFileInfo->unique)
        {
            fillInputFile(&cpuInfo, "_input_file", metaData, inputFileInfo);
        }
        logSectionRunTime(metaData, sectionStart, "_time_Metadata_Common");
        logSectionRunTime(metaData, crashdumpStart, GLOBAL_TIME_KEY);

        logMetaDataVersion(metaData, cpuInfo, inputFileInfo);
        logResetDetected(metaData, cpuInfo, platformState);
    }
}

#ifdef SPX_BMC_ACD
void fillSectionWithChild(
    cJSON* cpu, CPUInfo* cpuInfo, CrashdumpSection sectionName,
    const std::function<int(CPUInfo* cpuInfo, cJSON*)> sectionLogFunc1,
    const std::function<int(CPUInfo* cpuInfo, cJSON*)> sectionLogFunc2,
    timespec* sectionStart, char* timeStr)
{
    cJSON* logSection = NULL;

    if (CHECK_BIT(cpuInfo->sectionMask, sectionName.section))
    {
        updateCurrentSection(sectionName, cpuInfo);

        addSectionLog(cpu, cpuInfo, sectionName.name, sectionLogFunc1);
        logSection =
            addSectionLog(cpu, cpuInfo, sectionName.name, sectionLogFunc2);
        if (logSection)
        {
            logCrashdumpVersion(logSection, cpuInfo, sectionName.record_type);
            logRunTime(logSection, sectionStart, timeStr);
        }
    }
    else
    {
        addDisableSection(cpu, sectionName.name);
    }
}
#endif

/*void fillMetaDataCPU(cJSON* crashlogData, CPUInfo cpuInfo,
                     InputFileInfo* inputFileInfo)
{
    cJSON* logSection = NULL;
    if (CHECK_BIT(cpuInfo.sectionMask, METADATA))
    {
        updateCurrentSection(sectionNames[Section::METADATA], &cpuInfo);

        logSection =
            addSectionLog(crashlogData, &cpuInfo, "METADATA", logSysInfo);

        if (!inputFileInfo->unique)
        {
            // Fill in Input File Info in METADATA cpu section
            logSysInfoInputfile(&cpuInfo, logSection, inputFileInfo);
        }
    }
}
*/


static void cleanupInputFiles(InputFileInfo* inputFileInfo)
{
    for (int i = 0; i < cd_numberOfModels; i++)
    {
        FREE(inputFileInfo->filenames[i]);
        cJSON_Delete(inputFileInfo->buffers[i]);
    }
}

void updateTotalTime(std::string& storedLogContents, timespec* crashdumpStart)
{
    cJSON* content = cJSON_Parse(storedLogContents.c_str());
    if (content != NULL)
    {
        cJSON* crashdata = cJSON_GetObjectItem(content, "crash_data");
        if (crashdata != NULL)
        {
            cJSON* metadata = cJSON_GetObjectItem(crashdata, "METADATA");
            if (metadata != NULL)
            {
                cJSON* totaltime = cJSON_GetObjectItem(metadata, "_total_time");
                if (totaltime != NULL)
                {
                    logSectionRunTime(metadata, crashdumpStart,
                                      GLOBAL_TIME_KEY);
                    storedLogContents = cJSON_Print(content);
                    cJSON_Delete(content);
                    return;
                }
            }
        }
        cJSON_Delete(content);
    }
    CRASHDUMP_PRINT(INFO, stderr, "Could not update _total_time!\n");
}

void createCrashdump(std::vector<CPUInfo>& cpuInfo,
                     std::string& crashdumpContents,
                     const std::string& triggerType, std::string& timestamp,
                     bool isTelemetry)
{
    cJSON* root = NULL;
    cJSON* crashlogData = NULL;
    cJSON* metaData = NULL;
    cJSON* processors = NULL;
    cJSON* cpu = NULL;
    char* out = NULL;
    RunTimeInfo runTimeInfo;
    runTimeInfo.maxGlobalTime = 0xFFFFFFFF;
    InputFileInfo inputFileInfo = {
        .unique = true, .filenames = {NULL}, .buffers = {NULL}};
    static struct timespec sectionStart, crashdumpStart;
    // Clear any resets that happened before the crashdump collection started
    clearResetDetected();

    CRASHDUMP_PRINT(INFO, stderr, "Crashdump started...\n");
    crashdump::initCPUInfo(crashdump::cpuInfo);
    crashdump::getCPUData(cpuInfo, EVENT);
    crashdump::savePeciWake(cpuInfo);
    crashdump::setPeciWake(cpuInfo, ON);

    acdStatus inputFileLoadError =
        loadInputFiles(cpuInfo, &inputFileInfo, isTelemetry);

    if (inputFileLoadError == ACD_INPUT_FILE_ERROR)
    {
        cJSON_free(out);
        CRASHDUMP_PRINT(INFO, stderr, "Completed!\n");

        cleanupInputFiles(&inputFileInfo);
        crashdump::checkPeciWake(cpuInfo);
        crashdump::setPeciWake(cpuInfo, OFF);

        return;
    }

    // start the JSON tree for CPU dump
    root = cJSON_CreateObject();

    // Build the CPU Crashdump JSON file
    // Everything is logged under a "crash_data" section
    cJSON_AddItemToObject(root, "crash_data",
                          crashlogData = cJSON_CreateObject());

    // Create the METADATA section
    cJSON_AddItemToObject(crashlogData, "METADATA",
                          metaData = cJSON_CreateObject());

    // Create the processors section
    cJSON_AddItemToObject(crashlogData, "PROCESSORS",
                          processors = cJSON_CreateObject());
    if (inputFileLoadError)
    {
        cJSON_AddStringToObject(metaData, "_input_file_error",
                                INPUT_FILE_ERROR_STR);
    }

#ifdef NVD_SECTION
    cJSON* nvd = NULL;
    // Create the NVD section
    cJSON* nvdRecordEnable = cJSON_GetObjectItemCaseSensitive(
        cJSON_GetObjectItemCaseSensitive(cpuInfo[0].inputFile.bufferPtr, "NVD"),
        "RecordEnable");
    if (nvdRecordEnable != NULL)
    {
        // Create the NVD section
        cJSON_AddItemToObject(crashlogData, "NVD", nvd = cJSON_CreateObject());
        if (cJSON_IsFalse(nvdRecordEnable))
        {
            cJSON_AddFalseToObject(nvd, RECORD_ENABLE);
        }
    }
#endif

    // Include the version field
    logCrashdumpVersion(processors, &cpuInfo[0], RECORD_TYPE_BMCAUTONOMOUS);

    // Fill in the Crashdump data in the correct order (uncore to core) for
    // each CPU

    cJSON* maxCrashdumpTime = cJSON_GetObjectItemCaseSensitive(
        cpuInfo[0].inputFile.bufferPtr, "MaxTimeInSec");
    if (maxCrashdumpTime != NULL)
    {
        runTimeInfo.maxGlobalTime = (uint32_t)maxCrashdumpTime->valueint;
    }

    clock_gettime(CLOCK_MONOTONIC, &runTimeInfo.sectionRunTime);
    clock_gettime(CLOCK_MONOTONIC, &crashdumpStart);

    runTimeInfo.globalRunTime = crashdumpStart;
    char timeStr[] = "_time";
#ifdef SPX_BMC_ACD
    uint8_t numberOfSections = getNumberOfSections(&cpuInfo[0]);

    for (uint8_t section = 0; section < numberOfSections; section++)
    {
        for (size_t j = 0; j < cpuInfo.size(); j++)
        {
            
            char cpuString[8];
            //----AMI start-------------
            platformState.currentSection = section;
            platformState.currentCpu = j;
            //----AMI end---------------
            cd_snprintf_s(cpuString, sizeof(cpuString), "cpu%d", j);

            cpu = cJSON_GetObjectItemCaseSensitive(processors, cpuString);

            if (cpu == NULL)
            {
                cJSON_AddItemToObject(processors, cpuString,
                                      cpu = cJSON_CreateObject());
            }
            if (cpu != NULL)
            {
                if (cpuInfo[j].cpuidRead.cpuidValid)
                {
                    fillNewSection(root, &cpuInfo[j], j, &runTimeInfo, section);
                }
            }
        }
    }
#else
    for (size_t i = 0; i < cpuInfo.size(); i++)
    {
        // Create a section for this cpu
        char cpuString[8];
        cd_snprintf_s(cpuString, sizeof(cpuString), "cpu%d", i);
       /* cJSON_AddItemToObject(processors, cpuString,
                              cpu = cJSON_CreateObject());*/
        cpu = cJSON_GetObjectItemCaseSensitive(processors, cpuString);

        if (cpu == NULL)
        {
             cJSON_AddItemToObject(processors, cpuString,
                              cpu = cJSON_CreateObject());
        }
        // Fill in the Core Crashdump
        if ((cpu != NULL) && (cpuInfo[i].cpuidRead.cpuidValid))
        {
          /*  fillSection(cpu, &cpuInfo[i], sectionNames[Section::UNCORE],
                        logUncoreStatus, &sectionStart, timeStr);
            fillSection(cpu, &cpuInfo[i], sectionNames[Section::TOR],
                        logTorDump, &sectionStart, timeStr);
            fillSection(cpu, &cpuInfo[i], sectionNames[Section::PM_INFO],
                        logPowerManagement, &sectionStart, timeStr);
            fillSection(cpu, &cpuInfo[i], sectionNames[Section::ADDRESS_MAP],
                        logAddressMap, &sectionStart, timeStr);
*/
            addDelayToSection(cpu, &cpuInfo[i],
                              sectionNames[Section::BIG_CORE].name,
                              &crashdumpStart);

            fillSection(cpu, &cpuInfo[i], sectionNames[Section::BIG_CORE],
                        logCrashdump, &sectionStart, timeStr);
/*            fillSectionWithChild(cpu, &cpuInfo[i], sectionNames[Section::MCA_CORE],
                                 logCoreMca, logUncoreMca, &sectionStart,
                                 timeStr);
            fillMetaDataCPU(crashlogData, cpuInfo[i], &inputFileInfo);*/
        }
    }
#endif
#ifdef NVD_SECTION
    if ((nvdRecordEnable != NULL) && (cJSON_IsTrue(nvdRecordEnable)))
    {
        acdStatus nvdStatus = crashdump::getDIMMInventoryDBus(cpuInfo);
        if (nvdStatus == ACD_SUCCESS)
        {
            for (size_t j = 0; j < cpuInfo.size(); j++)
            {
                nvdStatus = fillNVDSection(&cpuInfo[j], j, nvd, &runTimeInfo);
            }
            if (nvdStatus != ACD_SECTION_DISABLE)
            {
                logRunTime(nvd, &runTimeInfo.sectionRunTime, TIME_KEY);
            }
        }
    }
#endif
    fillMetaDataCommon(metaData, cpuInfo[0], &inputFileInfo, triggerType,
                       timestamp, &crashdumpStart, &runTimeInfo.sectionRunTime,
                       platformState);
#ifdef CONFIG_SPX_FEATURE_OEMDATA_SECTION
    // OEM Customer Section
    clock_gettime(CLOCK_MONOTONIC, &sectionStart);
    crashdumpStart.tv_sec = sectionStart.tv_sec;
    crashdumpStart.tv_nsec = sectionStart.tv_nsec;
    cJSON* oemCrashlogData = NULL;
    cJSON* oemProcessors = NULL;
    cJSON* oemCpu = NULL;
    cJSON* oemLogSection = NULL;
    cJSON_AddItemToObject(root, "OEM",
                          oemCrashlogData = cJSON_CreateObject());
    cJSON_AddItemToObject(oemCrashlogData, "PROCESSORS",
                          oemProcessors = cJSON_CreateObject());
    logCrashdumpVersion(oemProcessors, &cpuInfo[0], RECORD_TYPE_BMCAUTONOMOUS);

    for (size_t i = 0; i < cpuInfo.size(); i++)
    {
        char cpuString[8];
        cd_snprintf_s(cpuString, sizeof(cpuString), "cpu%d", i);
        cJSON_AddItemToObject(oemProcessors, cpuString,
                              oemCpu = cJSON_CreateObject());
        oemLogSection =
            addSectionLog(oemCpu, &cpuInfo[i], "OEM", logOemData);
        if (oemLogSection)
        {
            logRunTime(oemLogSection, &sectionStart, timeStr);
        }
    }
    if (oemLogSection != NULL)
    {
        logTimestamp(oemCrashlogData, timestamp);
        /*Reason for False Positive -  Parse warnings */
        /* coverity[caretline : FALSE] */
        logRunTime(oemCrashlogData, &crashdumpStart, "_total_time");
    }
#endif
    
#ifdef CONFIG_SPX_FEATURE_CRASHDUMP_PRINT_UNFORMATTED
    out = cJSON_PrintUnformatted(root);
#else
    out = cJSON_Print(root);
#endif
    if (out != NULL)
    {
        crashdumpContents = out;
#ifdef TRIAGE_SECTION
        bool triageEnable = true;
        if (isTelemetry)
        {
            triageEnable = false;
        }
        if (readInputFileFlag(cpuInfo[0].inputFile.bufferPtr, triageEnable,
                              TRIAGE_ENABLE))
        {
            appendTriageSection(crashdumpContents);
        }
#endif
#ifdef BAFI_NDA_OUTPUT
        bool summaryEnable = true;
        if (isTelemetry)
        {
            summaryEnable = false;
        }
        if (readInputFileFlag(cpuInfo[0].inputFile.bufferPtr, summaryEnable,
                              SUMMARY_ENABLE))
        {
            appendSummarySection(crashdumpContents);
        }
#endif
        cJSON_free(out);
        CRASHDUMP_PRINT(INFO, stderr, "Completed!\n");
    }
    else
    {
        CRASHDUMP_PRINT(ERR, stderr, "cJSON_Print Failed\n");
    }
#ifdef CONFIG_SPX_FEATURE_ACD_SUMMARY
        bool summaryEnable = false;
        if (readInputFileFlag(cpuInfo[0].inputFile.bufferPtr, summaryEnable,
                              SUMMARY_ENABLE))
        {
            appendSummarySection(crashdumpContents);
        }

        updateTotalTime(crashdumpContents, &crashdumpStart);
#endif
    if (platformState.resetDetected)
    {
        clearResetDetected();
    }

    // Clear crashedCoreMask every time crashdump is run
    for (size_t i = 0; i < cpuInfo.size(); i++)
    {
        cpuInfo[i].crashedCoreMask = 0;
    }

    cleanupInputFiles(&inputFileInfo);
    cJSON_Delete(root);
    crashdump::checkPeciWake(cpuInfo);
    crashdump::setPeciWake(cpuInfo, OFF);
    if(cpuInfo.size() == 0)
    {
        printf("Crashdump - No Detected CPU\n");
    }
}

int scandir_filter(const struct dirent* dirEntry)
{
    // Filter for just the crashdump files
    return (strncmp(dirEntry->d_name, crashdumpPrefix.c_str(),
                    crashdumpPrefix.size()) == 0);
}
#ifndef SPX_BMC_ACD
void dbusRemoveOnDemandLog()
{
    // Always make sure the D-Bus properties are removed
    server->remove_interface(logIface);
    logIface.reset();

    std::error_code ec;
    if (!std::filesystem::exists(crashdumpDir))
    {
        // Can't delete something that doesn't exist
        return;
    }

    for (auto& fileList : std::filesystem::directory_iterator(crashdumpDir))
    {
        // always iterate through all files in the directory to clear
        // on-demand files. This is just a safeguard in the event more
        // than a single file is created.
        std::string fname = fileList.path();
        if (fname.substr(crashdumpDir.string().size() + 1,
                         crashdumpFileRoot.size()) == crashdumpFileRoot)
        {
            if (!(std::filesystem::remove(fname, ec)))
            {
                CRASHDUMP_PRINT(ERR, stderr, "failed to remove %s: %s\n",
                                fname.c_str(), ec.message().c_str());
                break;
            }
        }
    }
}

void dbusRemoveTelemetryLog()
{
    // Always make sure the D-Bus properties are removed
    server->remove_interface(logIface);
    logIface.reset();

    std::error_code ec;
    if (!std::filesystem::exists(crashdumpDir))
    {
        // Can't delete something that doesn't exist
        return;
    }

    for (auto& fileList : std::filesystem::directory_iterator(crashdumpDir))
    {
        // always iterate through all files in the directory to clear
        // on-demand files. This is just a safeguard in the event more
        // than a single file is created.
        std::string fname = fileList.path();
        if (fname.substr(crashdumpDir.string().size() + 1,
                         crashdumpTelemetryFileRoot.size()) ==
            crashdumpTelemetryFileRoot)
        {
            if (!(std::filesystem::remove(fname, ec)))
            {
                fprintf(stderr, "failed to remove %s: %s\n", fname.c_str(),
                        ec.message().c_str());
                break;
            }
        }
    }
}

void dbusAddLog(const std::string& logContents, const std::string& timestamp,
                const std::string& dbusPath, const std::string& filename)
{
    FILE* fpJson = NULL;
    std::error_code ec;
    std::filesystem::path out_file = crashdumpDir / filename;

    // create the crashdump/output directory if it doesn't exist
    if (!(std::filesystem::create_directories(crashdumpDir, ec)))
    {
        if (ec.value() != 0)
        {
            CRASHDUMP_PRINT(ERR, stderr, "failed to create %s: %s\n",
                            crashdumpDir.c_str(), ec.message().c_str());
            return;
        }
    }

    fpJson = fopen(out_file.c_str(), "w");
    if (fpJson)
    {
        fprintf(fpJson, "%s", logContents.c_str());
        fclose(fpJson);
    }
    logIface = server->add_interface(dbusPath, crashdumpInterface);
    logIface->register_property("Log", out_file.string());
    logIface->register_property("Timestamp", timestamp);
    logIface->register_property("Filename", filename);
    logIface->initialize();
}

void newOnDemandLog(std::vector<CPUInfo>& cpuInfo,
                    std::string& onDemandLogContents, std::string& timestamp)
{
    // Start the log to the on-demand location
    createCrashdump(cpuInfo, onDemandLogContents, triggerTypeOnDemand,
                    timestamp, false);
}

void newTelemetryLog(std::vector<CPUInfo>& cpuInfo,
                     std::string& telemetryLogContents, std::string& timestamp)
{
    // Start the log to the telemetry location
    createCrashdump(cpuInfo, telemetryLogContents, triggerTypeOnDemand,
                    timestamp, true);
}

void incrementCrashdumpCount()
{
    // Get the current count
    conn->async_method_call(
        [](boost::system::error_code ec,
           const std::variant<uint8_t>& property) {
            if (ec)
            {
                CRASHDUMP_PRINT(ERR, stderr, "Failed to get Crashdump count\n");
                return;
            }
            const uint8_t* crashdumpCountVariant =
                std::get_if<uint8_t>(&property);
            if (crashdumpCountVariant == nullptr)
            {
                CRASHDUMP_PRINT(ERR, stderr,
                                "Unable to read Crashdump count\n");
                return;
            }
            uint8_t crashdumpCount = *crashdumpCountVariant;
            if (crashdumpCount == std::numeric_limits<uint8_t>::max())
            {
                CRASHDUMP_PRINT(ERR, stderr,
                                "Maximum crashdump count reached\n");
                return;
            }
            // Increment the count
            crashdumpCount++;
            conn->async_method_call(
                [](boost::system::error_code ec) {
                    if (ec)
                    {
                        CRASHDUMP_PRINT(ERR, stderr,
                                        "Failed to set Crashdump count\n");
                    }
                },
                "xyz.openbmc_project.Settings",
                "/xyz/openbmc_project/control/processor_error_config",
                "org.freedesktop.DBus.Properties", "Set",
                "xyz.openbmc_project.Control.Processor.ErrConfig",
                "CrashdumpCount", std::variant<uint8_t>{crashdumpCount});
        },
        "xyz.openbmc_project.Settings",
        "/xyz/openbmc_project/control/processor_error_config",
        "org.freedesktop.DBus.Properties", "Get",
        "xyz.openbmc_project.Control.Processor.ErrConfig", "CrashdumpCount");
}
#endif
constexpr int numStoredLogs = 3;

void dbusAddStoredLog(const std::string& storedLogContents,
                      const std::string& timestamp)
{
    constexpr char const* crashdumpFile = "crashdump_%llu";
    uint64_t crashdump_num = 0;
    struct dirent** namelist = NULL;
    FILE* fpJson = NULL;
    std::error_code ec;
    char BAFICmd[256] = {0};
    // create the crashdump/output directory if it doesn't exist
    if (!(std::filesystem::create_directories(crashdumpDir, ec)))
    {
        if (ec.value() != 0)
        {
            CRASHDUMP_PRINT(ERR, stderr, "failed to create %s: %s\n",
                            crashdumpDir.c_str(), ec.message().c_str());
            return;
        }
    }
CRASHDUMP_PRINT(ERR, stderr, "succesfully create %s\n",crashdumpDir.c_str());
#ifndef SPX_BMC_ACD
    // Search the crashdump/output directory for existing log files
    int numLogFiles =
        scandir(crashdumpDir.c_str(), &namelist, scandir_filter, versionsort);
    if (numLogFiles < 0)
    {
        // scandir failed, so print the error
        perror("scandir");
        return;
    }

    // Get the number for this crashdump by finding the highest numbered
    // crashdump so far and incrementing by 1. The crashdumpNum is not kept as a
    // static in order to cover crashdump service restarts. In that case it is
    // undesirable to restart the number at zero.
    if (numLogFiles > 0)
    {
        // otherwise, get the number of the last log and increment it
        sscanf(namelist[numLogFiles - 1]->d_name, crashdumpFile,
                 &crashdump_num);
        crashdump_num++;
    }

    // In case multiple crashdumps are triggered for the same error, the policy
    // is to keep the first log until it is manually cleared and rotate through
    // additional logs.  This guarantees that we have the first and last log of
    // a failure.
    for (int i = 0; i < numLogFiles; i++)
    {
        // Except for log 0, if it's below the number of saved logs, delete it
        if ((i != 0) && (i <= (numLogFiles - (numStoredLogs - 1))))
        {
            std::error_code ec;
            if (!(std::filesystem::remove(crashdumpDir / namelist[i]->d_name,
                                          ec)))
            {
                CRASHDUMP_PRINT(ERR, stderr, "failed to remove %s: %s\n",
                                namelist[i]->d_name, ec.message().c_str());
            }
            // Now remove the interface for the deleted log
#ifndef SPX_BMC_ACD
            uint64_t currentNum = 0;
        int ret = sscanf_s(namelist[i]->d_name, crashdumpFile, &currentNum);
        if (ret > 0)
        {
            // This is a stored log, so track it for possible removal
            storedLogsToRemove.emplace_back(namelist[i]->d_name);
            if (currentNum >= crashdumpNum)
            {
                crashdumpNum = currentNum + 1;
            }
        }
        free(namelist[i]);
#endif
        }
        free(namelist[i]);
    }
    free(namelist);
#endif
#ifdef SPX_BMC_ACD
    std::string new_logfile_name = crashdumpPrefix + ".json";
#else
    // Create the new crashdump filename
    std::string new_logfile_name = crashdumpPrefix +
                                   std::to_string(crashdump_num) + "-" +
                                   timestamp + ".json";
#endif
    std::filesystem::path out_file = crashdumpDir / new_logfile_name;

    // open the JSON file to write the crashdump contents
    fpJson = fopen(out_file.c_str(), "w");
    if (fpJson != NULL)
    {
        fprintf(fpJson, "%s", storedLogContents.c_str());
        fclose(fpJson);
        memcpy(BAFICmd, BAFI_APP, strlen(BAFI_APP) );
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI_DEV_MAP
        strcat(BAFICmd, BAFI_DEV_MAP);
#endif
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI_MEM_MAP
        strcat(BAFICmd, BAFI_MEM_MAP);
#endif
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI_SILK_MAP
        strcat(BAFICmd, BAFI_SILK_MAP);
#endif
        strcat(BAFICmd, CRASHDUMP_FILE);

#ifdef CONFIG_SPX_FEATURE_ACD_BAFI
        // create BAFI.json using acdbafigenerator binary app
        /*Reason for False Positive - create BAFI.json*/
        /* coverity[check_return : FALSE] */
        system(BAFICmd);
#endif
    }  

    // Add the new interface for this log
    std::filesystem::path path =
        std::filesystem::path(crashdumpPath) / std::to_string(crashdump_num);
    if(system("/etc/init.d/crashdump_compress.sh &") == -1)
    {
        fprintf(stderr, "an error happens during producing a crashdump file\n");
    }
    else
    {
       fprintf(stderr, "Crashdump data is available in: /var/crashdump/sysdebug1.json\n");
#ifdef CONFIG_SPX_FEATURE_ACD_BAFI
	fprintf(stderr, "BAFI json is available in:/var/crashdump/json/bafi_decoded1.json\n");
#endif
    }
#ifndef SPX_BMC_ACD
    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceLog =
    server->add_interface(path.c_str(), crashdumpInterface);
    storedLogIfaces.emplace_back(new_logfile_name, ifaceLog);
    // Log Property

    ifaceLog->register_property("Log", out_file.string());
    ifaceLog->register_property("Timestamp", timestamp);
    ifaceLog->register_property("Filename", new_logfile_name);
    ifaceLog->register_signal<void>("CrashdumpComplete");
    ifaceLog->register_signal<void>("CrashdumpLogDeleted");
    ifaceLog->initialize();

    // Signaling that the Crashdump log has been stored
    sendSdBusSignal("CrashdumpComplete", ifaceLog);

    // Increment the count for this completed crashdump
    incrementCrashdumpCount();
#endif
}

void newStoredLog(std::vector<CPUInfo>& cpuInfo, std::string& storedLogContents,
                  const std::string& triggerType, std::string& timestamp)
{
    // Start the log
    createCrashdump(cpuInfo, storedLogContents, triggerType, timestamp, false);
}

bool isPECIAvailable()
{
    std::vector<CPUInfo> cpuInfo;
    getClientAddrs(cpuInfo);
    if (cpuInfo.empty())
    {
        CRASHDUMP_PRINT(ERR, stderr, "PECI is not available!\n");
        return false;
    }
    return true;
}
#ifndef SPX_BMC_ACD
/** Exception for when a log is attempted while power is off. */
struct PowerOffException final : public sdbusplus::exception_t
{
    const char* name() const noexcept override
    {
        return "org.freedesktop.DBus.Error.NotSupported";
    };
    const char* description() const noexcept override
    {
        return "Power off, cannot access peci";
    };
    const char* what() const noexcept override
    {
        return "org.freedesktop.DBus.Error.NotSupported: "
               "Power off, cannot access peci";
    };
    #ifdef BHS_EGS_BUILD
    int get_errno() const noexcept override
    {
        return EOPNOTSUPP;
    }
    #endif
};
/** Exception for when a log is attempted while another is in progress. */
struct LogInProgressException final : public sdbusplus::exception_t
{
    const char* name() const noexcept override
    {
        return "org.freedesktop.DBus.Error.ObjectPathInUse";
    };
    const char* description() const noexcept override
    {
        return "Log in progress";
    };
    const char* what() const noexcept override
    {
        return "org.freedesktop.DBus.Error.ObjectPathInUse: "
               "Log in progress";
    };
    #ifdef BHS_EGS_BUILD
    int get_errno() const noexcept override
    {
        return EBUSY;
    }
    #endif
};
#endif
} // namespace crashdump

#ifdef SPX_BMC_ACD
static int IsValidError()
{
    uint32_t u32PeciData;
    uint8_t u8Cpu;
    uint8_t cc = 0;
    int (*IsError)(uint32_t);
    void *dl_handle = NULL;
    dl_handle = dlopen((char *)ACDPDK_LIB,RTLD_NOW);
    if(!dl_handle)
    {
        printf("Error in loading ACDPDK_LIB library %s\n",dlerror());
        return 1;
    }
    fprintf(stderr, "Crashdump - Read MCA_ERR_SRC_LOG\n");

#if 0
    if (!crashdump::getCPUInfo(cpuInfo))
    {
        fprintf(stderr, "Failed to get CPU Info!\n");
        return 0;
    }
#endif

    // Get the list of CPU Info for this log
    if(crashdump::cpuInfo.size() == 0)
    {
        crashdump::initCPUInfo(crashdump::cpuInfo);
        crashdump::getCPUData(crashdump::cpuInfo, STARTUP);
    }

    for (u8Cpu = 0; u8Cpu < crashdump::cpuInfo.size(); u8Cpu++)
    {
        memset(&u32PeciData, 0x0, sizeof(uint32_t));
        if (peci_RdPkgConfig(crashdump::cpuInfo[u8Cpu].clientAddr, PECI_MBX_INDEX_CPU_ID,
      	                 PECI_PKG_ID_MACHINE_CHECK_STATUS, sizeof(uint32_t),
               	         (uint8_t*)&u32PeciData, &cc) != PECI_CC_SUCCESS)
        {
            fprintf(stderr, "[DBG] - CPU #%d MCA_ERR_SRC_LOG Failed\n", u8Cpu);
            dlclose(dl_handle);
            return 0;
        }
        printf("MCA_ERR_SRC_LOG regsiter = 0x%x, cpu = %d\n", u32PeciData, u8Cpu);

        IsError = (int (*)(uint32_t))dlsym(dl_handle,"ACDPDK_IsValidError");
        if (IsError != NULL)
        {
            if (IsError(u32PeciData))
            {
                dlclose(dl_handle);
                return 1;
            }
        }
        else
        {
            dlclose(dl_handle);
            return 1;
        }
    }
    dlclose(dl_handle);
    return 0;
}
#endif
int main()
{
#ifdef SPX_BMC_ACD
printf("----------------Crashdump is starting from SPX_BMC_ACD----------------\n");
	int tmp = 0;
	std::string ami_onDemandLogContents;
	std::string onDemandTimestamp = crashdump::newTimestamp();
	int (*GetTriggerType)();
        int (*IsACDTriggered)();
	int (*IsValidTrigger)();
	int (*PostDumpAction)();
        int (*DataAreaConfig)(void *);
        int (*RecordDataAreaConfig)(void *);
	int validTrigger = 0, checkErrCount = 0, gtt=0, ret = 0;
	void *dl_handle = NULL;
    std::string storedLogContents;
    if (!crashdump::isPECIAvailable())
    {
        CRASHDUMP_PRINT(ERR, stderr, "PECI not available\n");
    }
    crashdump::initCPUInfo(crashdump::cpuInfo);
    crashdump::getCPUData(crashdump::cpuInfo, STARTUP);
    std::string storedLogTime = crashdump::newTimestamp();

    dl_handle = dlopen((char *)ACDPDK_LIB,RTLD_NOW);
    if(!dl_handle)
    {
        printf("Error in loading ACDPDK_LIB library %s\n",dlerror());
        return 1;
    }

    only_one_instance((char *)crashdump::crashdumpLockPath.c_str());

    IsACDTriggered = (int (*)())dlsym(dl_handle,"ACDPDK_IsACDTriggered");
    if (IsACDTriggered == NULL)
    {
        dlclose(dl_handle);
        return 1;
    }
    DataAreaConfig =(int (*)(void *))dlsym(dl_handle,"ACDPDK_ChgDataAreaConfig");
    if (DataAreaConfig == NULL)
    {
        dlclose(dl_handle);
        return 1;
    }
    RecordDataAreaConfig = (int (*)(void *))dlsym(dl_handle, "ACDPDK_RecordDataAreaConfig");
    if (RecordDataAreaConfig == NULL)
    {
        dlclose(dl_handle);
        return 1;
    }
    GetTriggerType = (int (*)())dlsym(dl_handle,"ACDPDK_getTargetType");
    if (GetTriggerType == NULL)
    {
        dlclose(dl_handle);
        return 1;
    }
    RecordDataAreaConfig((void *)sectionNames);
	while(1) 
	{
		ret = IsACDTriggered();
        	if(ret == 4)//enable data area configuration
		{
		    tmp = DataAreaConfig((void *)sectionNames);
		    continue;
		}
		else if ( ret != 1)
		{
		    dlclose(dl_handle);
		    return 1;
		}
                gtt = 0;
		validTrigger = 1;
		checkErrCount = 0;
		while (!IsValidError())
		{
			//Check for error for about 30 seconds (delay of 100ms * 300 iterations)
			if (checkErrCount >= ERRCHECKMAXCOUNT)
			{
                /*Reason for False Positive - this is the debug message.*/
                /* coverity[dead_error_begin : FALSE] */
				fprintf(stderr, "CRASHDUMP error is not valid!\n");
				break;
			}

			IsValidTrigger = (int (*)())dlsym(dl_handle,"ACDPDK_IsValidTrigger");
			if (IsValidTrigger != NULL)
			{
				//If not a valid CATERR, continue to wait for next valid CATERR
				if(!IsValidTrigger())
				{
					validTrigger = 0;
					break;
				}
				break;
			}
			else
				break;
		}
		if ((validTrigger == 0))// || (checkErrCount >= ERRCHECKMAXCOUNT))
			continue;
                storedLogTime = crashdump::newTimestamp();

		gtt = GetTriggerType();
                if (gtt == 2){
                    crashdump::newStoredLog(crashdump::cpuInfo, storedLogContents, crashdump::triggerTypeIERR, storedLogTime);
                }
                else{
                    crashdump::newStoredLog(crashdump::cpuInfo, storedLogContents, crashdump::triggerTypeOnDemand, storedLogTime);
                }

		if (storedLogContents.empty())
		{
			CRASHDUMP_PRINT(ERR, stderr, "LOG is empty so exiting\n");
		    // Log is empty, so don't save it
			return ACD_FAILURE;
		}
		crashdump::dbusAddStoredLog(storedLogContents,storedLogTime);

		/* PDK for post crasdump actions */
		PostDumpAction = (int (*)())dlsym(dl_handle,"ACDPDK_PostDumpAction");
		if (PostDumpAction != NULL)
		{
			PostDumpAction();
		}
	}

#else
    // future to use for long-running tasks
    std::future<void> future;

    // setup connection to dbus
    crashdump::conn =
        std::make_shared<sdbusplus::asio::connection>(crashdump::io);

    // CPU Debug Log Object
    crashdump::conn->request_name(crashdump::crashdumpService);
    crashdump::server =
        std::make_shared<sdbusplus::asio::object_server>(crashdump::conn);

    // Reserve space for the stored log interfaces
    crashdump::storedLogIfaces.reserve(crashdump::numStoredLogs);

    // Stored Log Interface
    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceStored =
        crashdump::server->add_interface(crashdump::crashdumpPath,
                                         crashdump::crashdumpStoredInterface);
    ifaceStored->register_signal<void>("CrashdumpAllDeleted");

    if (!crashdump::isPECIAvailable())
    {
        CRASHDUMP_PRINT(ERR, stderr, "PECI not available\n");
    }
    crashdump::initCPUInfo(crashdump::cpuInfo);
    crashdump::getCPUData(crashdump::cpuInfo, STARTUP);

    // Generate a Stored Log
    ifaceStored->register_method(
        "GenerateStoredLog",
        [&future, ifaceStored](const std::string& triggerType) {
            if (!crashdump::isPECIAvailable())
            {
                throw crashdump::PowerOffException();
            }
            if (future.valid() && future.wait_for(std::chrono::seconds(0)) !=
                                      std::future_status::ready)
            {
                throw crashdump::LogInProgressException();
            }
            future = std::async(std::launch::async, [triggerType,
                                                     ifaceStored]() {
                std::string storedLogContents;
                std::string storedLogTime = crashdump::newTimestamp();
                crashdump::newStoredLog(crashdump::cpuInfo, storedLogContents,
                                        triggerType, storedLogTime);

                if (storedLogContents.empty())
                {
                    // Log is empty, so don't save it
                    return;
                }
                boost::asio::post(
                    crashdump::io,
                    [storedLogContents = std::move(storedLogContents),
                     storedLogTime = std::move(storedLogTime)]() mutable {
                        crashdump::dbusAddStoredLog(storedLogContents,
                                                    storedLogTime);
                    });
            });
            return std::string("Log Started");
        });
    ifaceStored->initialize();

    // DeleteAll Interface
    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceDeleteAll =
        crashdump::server->add_interface(
            crashdump::crashdumpPath, crashdump::crashdumpDeleteAllInterface);

    // Delete all stored logs
    ifaceDeleteAll->register_method("DeleteAll", [ifaceStored]() {
        std::error_code ec;
        bool emitSignal = !crashdump::storedLogIfaces.empty();
        for (auto& [file, interface] : crashdump::storedLogIfaces)
        {
            if (!(std::filesystem::remove(crashdump::crashdumpDir / file, ec)))
            {
                CRASHDUMP_PRINT(ERR, stderr, "failed to remove %s: %s\n",
                                file.c_str(), ec.message().c_str());
            }
            crashdump::server->remove_interface(interface);
        }
        crashdump::storedLogIfaces.clear();
        crashdump::dbusRemoveOnDemandLog();

        if (emitSignal)
        {
            // Signaling the deletion of All Crashdump logs found
            crashdump::sendSdBusSignal("CrashdumpAllDeleted", ifaceStored);
        }

        CRASHDUMP_PRINT(INFO, stderr, "Crashdump logs cleared\n");
        return std::string("Logs Cleared");
    });
    ifaceDeleteAll->initialize();

    // OnDemand Log Interface
    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceOnDemand =
        crashdump::server->add_interface(crashdump::crashdumpPath,
                                         crashdump::crashdumpOnDemandInterface);

    // Generate an OnDemand Log
    ifaceOnDemand->register_method("GenerateOnDemandLog", [&future]() {
        if (!crashdump::isPECIAvailable())
        {
            throw crashdump::PowerOffException();
        }
        // Check if a Log is in progress
        if (future.valid() && future.wait_for(std::chrono::seconds(0)) !=
                                  std::future_status::ready)
        {
            throw crashdump::LogInProgressException();
        }
        // Remove the old on-demand log
        crashdump::dbusRemoveOnDemandLog();

        // Start the log asynchronously since it can take a long time
        future = std::async(std::launch::async, []() {
            std::string onDemandLogContents;
            std::string onDemandTimestamp = crashdump::newTimestamp();
            std::string filename =
                crashdump::crashdumpFileRoot + onDemandTimestamp + ".json";
            crashdump::newOnDemandLog(crashdump::cpuInfo, onDemandLogContents,
                                      onDemandTimestamp);
            boost::asio::post(
                crashdump::io,
                [onDemandLogContents = std::move(onDemandLogContents),
                 onDemandTimestamp = std::move(onDemandTimestamp),
                 filename]() mutable {
                    crashdump::dbusAddLog(
                        onDemandLogContents, onDemandTimestamp,
                        crashdump::crashdumpOnDemandPath, filename);
                });
        });

        // Return success
        return std::string("Log Started");
    });

    ifaceOnDemand->initialize();

    // Telemetry Log Interface
    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceTelemetry =
        crashdump::server->add_interface(
            crashdump::crashdumpPath, crashdump::crashdumpTelemetryInterface);

    // Generate a Telemetry Log
    ifaceTelemetry->register_method("GenerateTelemetryLog", [&future]() {
        if (!crashdump::isPECIAvailable())
        {
            throw crashdump::PowerOffException();
        }
        // Check if a Log is in progress
        if (future.valid() && future.wait_for(std::chrono::seconds(0)) !=
                                  std::future_status::ready)
        {
            throw crashdump::LogInProgressException();
        }
        crashdump::dbusRemoveTelemetryLog();

        // Start the log asynchronously since it can take a long time
        future = std::async(std::launch::async, []() {
            std::string telemetryLogContents;
            std::string telemetryTimestamp = crashdump::newTimestamp();
            std::string filename = crashdump::crashdumpTelemetryFileRoot +
                                   telemetryTimestamp + ".json";
            crashdump::newTelemetryLog(crashdump::cpuInfo, telemetryLogContents,
                                       telemetryTimestamp);
            boost::asio::post(
                crashdump::io,
                [telemetryLogContents = std::move(telemetryLogContents),
                 telemetryTimestamp = std::move(telemetryTimestamp),
                 filename]() mutable {
                    crashdump::dbusAddLog(
                        telemetryLogContents, telemetryTimestamp,
                        crashdump::crashdumpTelemetryPath, filename);
                });
        });

        // Return success
        return std::string("Log Started");
    });

    ifaceTelemetry->initialize();
    try
    {
        // Build up paths for any existing stored logs
        if (std::filesystem::exists(crashdump::crashdumpDir))
        {
            std::regex search("crashdump_([[:digit:]]+)-([[:graph:]]+?).json");
            std::smatch match;
            for (auto& p :
                 std::filesystem::directory_iterator(crashdump::crashdumpDir))
            {
                std::string file = p.path().filename();
                if (std::regex_match(file, match, search))
                {
                    // Log Interface
                    std::filesystem::path path =
                        std::filesystem::path(crashdump::crashdumpPath) /
                        match.str(1);
                    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceLog =
                        crashdump::server->add_interface(
                            path.c_str(), crashdump::crashdumpInterface);
                    crashdump::storedLogIfaces.emplace_back(file, ifaceLog);
                    // Log Property
                    ifaceLog->register_property("Log", p.path().string());
                    ifaceLog->register_property("Timestamp", match.str(2));
                    ifaceLog->register_property("Filename", file);
                    ifaceLog->initialize();
                }
            }
            crashdump::dbusRemoveOnDemandLog();
        }
    }
    catch (const std::regex_error& e)
    {
        CRASHDUMP_PRINT(
            ERR, stderr,
            "A regex error ocurred while removing previous OnDemand logs\n");
    }
    catch (const sdbusplus::exception::SdBusError& e)
    {
        CRASHDUMP_PRINT(
            ERR, stderr,
            "A SdBusError ocurred while removing previous OnDemand logs\n");
    }

    // Send Raw PECI Interface
    std::shared_ptr<sdbusplus::asio::dbus_interface> ifaceRawPeci =
        crashdump::server->add_interface(crashdump::crashdumpPath,
                                         crashdump::crashdumpRawPeciInterface);

    // Send a Raw PECI command
    ifaceRawPeci->register_method(
        "SendRawPeci", [](const std::vector<std::vector<uint8_t>>& rawCmds) {
            // D-Bus will time out after too long, so set a deadline for when to
            // abort the PECI commands (at 25s, it mostly times out, at 24s it
            // doesn't, so use 23s to be safe)
            constexpr int peciTimeout = 23;
            std::chrono::steady_clock::time_point peciDeadline =
                std::chrono::steady_clock::now() +
                std::chrono::duration<int>(peciTimeout);
            std::vector<std::vector<uint8_t>> rawResp;
            rawResp.resize(rawCmds.size());
            for (size_t i = 0; i < rawCmds.size(); i++)
            {
                const std::vector<uint8_t>& rawCmd = rawCmds[i];
                // If the commands are taking too long, return early to avoid a
                // D-Bus timeout
                if (std::chrono::steady_clock::now() > peciDeadline)
                {
                    CRASHDUMP_PRINT(
                        ERR, stderr,
                        "%d second deadline reached.  Aborting PECI commands "
                        "to avoid a timeout\n",
                        peciTimeout);
                    break;
                }

                if (rawCmd.size() < 3)
                {
                    throw std::invalid_argument("Command Length too short");
                }
                rawResp[i].resize(rawCmd[2]);
                peci_raw(rawCmd[0], rawCmd[2], &rawCmd[3], rawCmd[1],
                         rawResp[i].data(), rawResp[i].size());
            }
            return rawResp;
        });
    ifaceRawPeci->initialize();

    // Start tracking host state
    std::shared_ptr<sdbusplus::bus::match::match> hostStateMonitor =
        crashdump::startHostStateMonitor(crashdump::conn);

    try
    {
        crashdump::io.run();
    }
    catch (const boost::system::system_error& e)
    {
        CRASHDUMP_PRINT(ERR, stderr, "Failed to run io\n");
    }

    return ACD_SUCCESS;
#endif // SPX_BMC_ACD
}

