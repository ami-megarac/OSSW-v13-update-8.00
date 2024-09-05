/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2022 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials, and
 * your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise, you may
 * not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in
 * the License.
 *
 ******************************************************************************/

#define MEMORY_MAP_FILENAME "default_memory_map.json"
#define DEVICE_MAP_FILENAME "default_device_map.json"
#define SILKSCREEN_MAP_FILENAME "default_silkscreen_map.json"
#define BAFI_VERSION "2.93"
//AMI define
#define AMI_RELEASED " - SPR-SP \n - EMR-SP"
#define AMI_UNRELEASED " - SPR-SP \n - EMR-SP"

#ifdef _WIN32
#define OS_SEP "\\"
#define BMC_PATH "C:\\temp\\bafi\\"
#else
#define OS_SEP "/"
#define BMC_PATH "/conf/crashdump/input/" //AMI
#endif

#include "cpu_factory.hpp"
#include "report.hpp"
#include "summary.hpp"
#include "utils.hpp"

#include <cstring>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <optional>

using json = nlohmann::json;

enum ReturnCode
{
  SUCCESS = 0,
  CRASHDUMP_NOT_PROVIDED = 1,
  FAILED_TO_PARSE_CRASHDUMP_TO_JSON = 2,
  INCORRECT_JSON_ROOT_NODE = 3,
  CPU_TYPE_NOT_RECOGNIZED = 4,
  FAILED_TO_PREPARE_REPORT = 5,
  SYMLINK_FILE = 6,
  HARDLINK_FILE = 7,
};

bool checkIfMapsAreSymlinkFiles()
{
    std::ifstream iDev(BMC_PATH + std::string(DEVICE_MAP_FILENAME));
    std::ifstream iSil(BMC_PATH + std::string(SILKSCREEN_MAP_FILENAME));
    std::ifstream iMem(BMC_PATH + std::string(MEMORY_MAP_FILENAME));
    if (iDev.good())
    {
        std::filesystem::path file_path(BMC_PATH + std::string(DEVICE_MAP_FILENAME));
        if (std::filesystem::is_symlink(file_path))
        {
            return true;
        }
    }
    if (iSil.good())
    {
        std::filesystem::path file_path(BMC_PATH + std::string(SILKSCREEN_MAP_FILENAME));
        if (std::filesystem::is_symlink(file_path))
        {
            return true;
        }
    }
    if (iMem.good())
    {
        std::filesystem::path file_path(BMC_PATH + std::string(MEMORY_MAP_FILENAME));
        if (std::filesystem::is_symlink(file_path))
        {
            return true;
        }
    }
    return false;
}

bool checkIfMapsAreHardlinkFiles()
{
    std::ifstream iDev(BMC_PATH + std::string(DEVICE_MAP_FILENAME));
    std::ifstream iSil(BMC_PATH + std::string(SILKSCREEN_MAP_FILENAME));
    std::ifstream iMem(BMC_PATH + std::string(MEMORY_MAP_FILENAME));
    if (iDev.good())
    {
        std::filesystem::path file_path(BMC_PATH + std::string(DEVICE_MAP_FILENAME));
        if (std::filesystem::hard_link_count(file_path) > 1)
        {
            return true;
        }
    }
    if (iSil.good())
    {
        std::filesystem::path file_path(BMC_PATH + std::string(SILKSCREEN_MAP_FILENAME));
        if (std::filesystem::hard_link_count(file_path) > 1)
        {
            return true;
        }
    }
    if (iMem.good())
    {
        std::filesystem::path file_path(BMC_PATH + std::string(MEMORY_MAP_FILENAME));
        if (std::filesystem::hard_link_count(file_path) > 1)
        {
            return true;
        }
    }
    return false;
}

Summary prepareSummary(const json& inputJson)
{
    std::string cpuType = getCpuType(inputJson);
    std::string cpuId = "";
    auto optionalCpuId = getCpuId(inputJson);
    if (optionalCpuId)
    {
        cpuId = optionalCpuId.value();
    }
    auto cpu = CpuFactory::createInstance(cpuType, cpuId);

    if (cpu == nullptr)
        return Summary("", "");

    return cpu->prepareSummary(inputJson);
}

json prepareReport(Summary summary, const json& inputJson, json deviceMap, json silkscreenMap)
{
    json reportJson = nullptr;
    Report report(summary, deviceMap, silkscreenMap, inputJson);

    reportJson = report.createJSONReport();

    return reportJson;
}

void findSocketsFromSummary(std::vector<std::string>& sockets, json& reportJson)
{
    for (const auto& metaDataItem : reportJson["output_summary"].items())
    {
        if (startsWith(metaDataItem.key(), "socket"))
        {
            sockets.push_back(metaDataItem.key());
        }
    }
}

std::string findFirstErrorSocket(json& reportJson, std::string& firstError)
{
    std::vector<std::string> sockets;
    bool ierr = false;
    bool mcerr = false;
    bool rmcaMcerr = false;
    std::string firstSocket;
    findSocketsFromSummary(sockets, reportJson);
    for (const auto& socket : sockets)
    {
        for (const auto& [pciKey, pciVal] : reportJson["output_summary"][socket].items())
        {
            if (pciVal.contains("TSC"))
            {
                for (const auto& [metaKey, metaVal] : pciVal.items())
                {
                    for (const auto& [tscKey, tscVal] : metaVal.items())
                    {
                        if (std::string(tscVal).find("(Occurred first between all TSCs)") != std::string::npos)
                        {
                            if (std::string(tscVal).find("pcu_first_ierr_tsc_cfg") != std::string::npos)
                            {
                                firstError = "FirstIERR";
                                ierr = true;
                            }
                            else if (std::string(tscVal).find("pcu_first_mcerr_tsc_cfg") != std::string::npos)
                            {
                                firstError = "FirstMCERR";
                                mcerr = true;
                            }
                            else if (std::string(tscVal).find("pcu_first_rmca_tsc_cfg") != std::string::npos)
                            {
                                firstError = "RMCA_FirstMCERR";
                                rmcaMcerr = true;
                            }
                            else
                            {
                                firstError = "FirstMCERR";
                                mcerr = true;
                            }

                            firstSocket = socket;
                        }
                    }
                }
            }
        }
    }

    if (ierr || mcerr || rmcaMcerr)
        return firstSocket;
    else
        return "";
}

std::string getImcStringGeneric(std::string firstError)
{
    std::size_t left = firstError.find("bank");
    std::size_t right = firstError.find(")");

    return firstError.substr(left, right - left + 1);
}

std::string getImcStringBhs(std::string firstError)
{
    std::size_t left = firstError.find("MCDDR");
    std::size_t right = firstError.find(", bank");
    return firstError.substr(left, right - left);
}

std::string findDieNumber(std::string srcId)
{
    std::size_t computeLeft = srcId.find(" compute");
    std::size_t ioLeft = srcId.find(" io");
    std::size_t socLeft = srcId.find(" soc");
    if (socLeft != std::string::npos)
    {
        return "soc";
    }

    if (computeLeft == std::string::npos && ioLeft == std::string::npos)
        return "";
    
    if(computeLeft == std::string::npos)
    {
        // it is io die
        return srcId.substr(ioLeft+1, 3);
    }
    // it is compute die
    return srcId.substr(ioLeft+1, 8);
}

void getErrorStrings(std::string entry, std::string& ierr, std::string& mcerr, std::string& rmcaMcerr,
                     std::string& sidebandPortId, std::string cpuType)
{
    std::size_t left = std::string(entry).find("FirstIERR");
    std::size_t right = std::string(entry).substr(left, std::string(entry).size()).find(", FirstMCERR");
    ierr = std::string(entry).substr(left, right).substr(12);
    left = std::string(entry).find("FirstMCERR");
    if (cpuType == "SPR" || cpuType == "SPR-HBM" || cpuType == "EMR" || cpuType == "GNR"|| cpuType == "SRF")
    {
        right = std::string(entry).substr(left, std::string(entry).size()).find(", RMCA_FirstMCERR");
        mcerr = std::string(entry).substr(left, right).substr(13);

        left = std::string(entry).find(", RMCA_FirstMCERR");
        if( left != -1)//AMI
        {
            right = std::string(entry).substr(left, std::string(entry).size()).find(", SIDEBANDPORTID");
            rmcaMcerr = std::string(entry).substr(left, right).substr(20);
        }
    }
    else
    {
        right = std::string(entry).substr(left, std::string(entry).size()).find(", SIDEBANDPORTID");
        mcerr = std::string(entry).substr(left, right).substr(13);
    }
    if (cpuType != "SKX" && cpuType != "CPX")
    {
        left = std::string(entry).find("SIDEBANDPORTID");
        right = std::string(entry).size();
        sidebandPortId = std::string(entry).substr(left, right).substr(17);
    }
}

std::string determineFirstErrorType(std::string& ierr, std::string& mcerr, std::string& rmcaMcerr)
{
    if (ierr != "0x0" && mcerr == "0x0")
        return "FirstIERR";
    else if (rmcaMcerr != "0x0" && rmcaMcerr != "")
        return "RMCA_FirstMCERR";
    else
        return "FirstMCERR";
}

void getPPIN(const json& input, nlohmann::ordered_json& triageReport, std::string socket)
{
    if (input.find("METADATA") != input.cend())
    {
        if (!checkInproperValue(input["METADATA"]["cpu" + socket.substr(6)]["ppin"]) &&
            input["METADATA"]["cpu" + socket.substr(6)]["ppin"] != "dummy")
        {
            triageReport[socket]["PPIN"] = input["METADATA"]["cpu" + socket.substr(6)]["ppin"];
        }
        else
        {
            triageReport[socket]["PPIN"] = "N/A";
        }
    }
    else
    {
        triageReport[socket]["PPIN"] = json::object();
    }
}

void getUcodePath(const json& input, nlohmann::ordered_json& triageReport, std::string socket)
{
    if (input.find("METADATA") != input.cend())
    {
        if (!checkInproperValue(input["METADATA"]["cpu" + socket.substr(6)]["ucode_patch_ver"]) &&
            input["METADATA"]["cpu" + socket.substr(6)]["ucode_patch_ver"] != "dummy")
        {
            triageReport[socket]["Ucode_Patch_Ver"] = input["METADATA"]["cpu" + socket.substr(6)]["ucode_patch_ver"];
        }
        else
        {
            triageReport[socket]["Ucode_Patch_Ver"] = "N/A";
        }
    }
    else
    {
        triageReport[socket]["Ucode_Patch_Ver"] = json::object();
    }
}
void parseCorrectableMemoryError(nlohmann::ordered_json &socketReport, std::string errorLocation, std::string errorData)
{
    std::string slotName;
    if (!startsWith(errorData, "Bank"))
    {
        slotName = errorData.substr(0, errorData.find(",")) + ", " + errorLocation;
    }
    else
    {
        slotName = "N/A, " + errorLocation;
    }
    socketReport["CorrectableMemoryErrors"].push_back(slotName);
}

void parseUncorrectableMemoryError(nlohmann::ordered_json &socketReport, nlohmann::ordered_json &socketDimmEntry,
                                   std::string errorLocation, std::string errorData)
{
    std::string slotName;
    if (!startsWith(errorData, "Bank"))
    {
        slotName = errorData.substr(0, errorData.find(",")) + ", " + errorLocation;
    }
    else
    {
        slotName = "N/A, " + errorLocation;
    }
    socketReport["UncorrectableMemoryErrors"] .push_back(slotName);
    socketDimmEntry["Uncorrectable"].push_back(slotName);
}

void getMemoryErrors(json& reportJson, std::string socket, nlohmann::ordered_json& triageReport, nlohmann::ordered_json& dimmEntry)
{
    bool corMemErr = false;
    bool uncorMemErr = false;
    for (const auto& [pciKey, pciVal] : reportJson["output_summary"][socket].items())
    {
        if (pciVal.contains("Memory_errors"))
        {
            for (const auto& [metaKey, metaVal] : pciVal.items())
            {
                if (metaVal.contains("Correctable"))
                {
                    for (const auto& [strKey, strVal] : metaVal.items())
                    {
                        for (const auto& [key, val] : strVal.items())
                        {
                            if(val.is_array())
                            {
                                for (const auto& error : val)
                                {
                                    parseCorrectableMemoryError(triageReport[socket], key, error);
                                }
                            }
                            else
                            {
                                parseCorrectableMemoryError(triageReport[socket], key, val);
                            }
                            corMemErr = true;
                        }
                    }
                }
                if (metaVal.contains("Uncorrectable"))
                {
                    for (const auto& [strKey, strVal] : metaVal.items())
                    {
                        for (const auto& [key, val] : strVal.items())
                        {
                            if(val.is_array())
                            {
                                for (const auto& error : val)
                                {
                                    parseUncorrectableMemoryError(triageReport[socket], dimmEntry[socket], key, error);
                                }
                            }
                            else
                            {
                                parseUncorrectableMemoryError(triageReport[socket], dimmEntry[socket], key, val);
                            }
                            uncorMemErr = true;
                        }
                    }
                }
            }
        }
    }

    if (!corMemErr)
    {
        triageReport[socket]["CorrectableMemoryErrors"] = json::object();
    }

    if (!uncorMemErr)
    {
        triageReport[socket]["UncorrectableMemoryErrors"] = json::object();
    }
}

void getThermalErrors(std::string socket, nlohmann::ordered_json& triageReport, nlohmann::ordered_json& cpuThermalEntry,
                      Summary& summary)
{
    if(summary.cpuType == "GNR" || summary.cpuType == "SRF")
    {
        uint32_t socketId = std::stoi(socket.substr(6));
        if (summary.thermStatus == nullptr || (!summary.thermStatus->contains(socketId)))
        {
            triageReport[socket]["PackageThermalStatus"] = json::object();
            return;
        }
        bool thermalEventOccurred = false;
        std::vector<std::string> possibleDieSections{"compute0", "compute1", "compute2", "io0", "io1", "soc"};
        for (const auto& die : possibleDieSections)
        {
            if (!summary.thermStatus->contains(socketId, die))
            {
                continue;
            }
            if (!bool(summary.thermStatus->at(socketId, die).prochot_log) && !bool(summary.thermStatus->at(socketId, die).prochot_status) &&
                !bool(summary.thermStatus->at(socketId, die).pmax_log) && !bool(summary.thermStatus->at(socketId, die).pmax_status) &&
                !bool(summary.thermStatus->at(socketId, die).out_of_spec_log) &&
                !bool(summary.thermStatus->at(socketId, die).out_of_spec_status) &&
                !bool(summary.thermStatus->at(socketId, die).thermal_monitor_log) &&
                !bool(summary.thermStatus->at(socketId, die).thermal_monitor_status))
            {
                 triageReport[socket]["PackageThermalStatus"][die] = "Ok";
            }
            else
            {
                triageReport[socket]["PackageThermalStatus"][die] = "Error";
                thermalEventOccurred = true;
            }
        }

        if (thermalEventOccurred)
        {            
            cpuThermalEntry["Thermal_Event_On_CPUs"].push_back(socket);
        }
        return;
    }
    uint32_t socketId = std::stoi(socket.substr(6));
    if (summary.thermStatus == nullptr || (!summary.thermStatus->contains(socketId)))
    {
        triageReport[socket]["PackageThermalStatus"] = json::object();
    }
    else
    {
        if (!bool(summary.thermStatus->at(socketId).prochot_log) && !bool(summary.thermStatus->at(socketId).prochot_status) &&
            !bool(summary.thermStatus->at(socketId).pmax_log) && !bool(summary.thermStatus->at(socketId).pmax_status) &&
            !bool(summary.thermStatus->at(socketId).out_of_spec_log) &&
            !bool(summary.thermStatus->at(socketId).out_of_spec_status) &&
            !bool(summary.thermStatus->at(socketId).thermal_monitor_log) &&
            !bool(summary.thermStatus->at(socketId).thermal_monitor_status))
        {
            triageReport[socket]["PackageThermalStatus"] = "Ok";
        }
        else
        {
            triageReport[socket]["PackageThermalStatus"] = "Error";
            cpuThermalEntry["Thermal_Event_On_CPUs"].push_back(socket);
        }
    }
}

uint32_t getFullOutput(const json& inputJson, char*& fullOutput, size_t* fullOutputSize,
                       json deviceMap = NULL, json silkscreenMap = NULL, bool standaloneApp = false)
{
    auto input = getProperRootNode(inputJson);
    if (!input)
        return INCORRECT_JSON_ROOT_NODE;

    auto summary = prepareSummary(*input);

    if (summary.cpuType == "")
    {
        return CPU_TYPE_NOT_RECOGNIZED;
    }

    if (!standaloneApp)
    {
        if (checkIfMapsAreSymlinkFiles())
        {
            return SYMLINK_FILE;
        }
        if (checkIfMapsAreHardlinkFiles())
        {
            return HARDLINK_FILE;
        }
        PciBdfLookup deviceLookup((char*)"", true, DEVICE_MAP_FILENAME);
        PciBdfLookup silkscreenLookup((char*)"", true, SILKSCREEN_MAP_FILENAME);
        PciBdfLookup PciBdfLookup((char*)"", true, MEMORY_MAP_FILENAME);
        deviceMap = deviceLookup.lookupMap;
        silkscreenMap = silkscreenLookup.lookupMap;
    }
    json reportJson = prepareReport(summary, *input, deviceMap, silkscreenMap);
    if (reportJson.is_discarded())
        return FAILED_TO_PREPARE_REPORT;

    json report;
    json& fullReport = report;
    fullReport["summary"] = reportJson;

    // Assign full output to char pointer
    std::string outputStr = fullReport.dump(4);
    *fullOutputSize = outputStr.size();
    fullOutput = new char[*fullOutputSize + 1];
    outputStr.copy(fullOutput, *fullOutputSize);
    fullOutput[*fullOutputSize] = '\0';

    return SUCCESS;
}

uint32_t getFullOutput(char* inputPointer, std::size_t inputSize, char*& fullOutput, size_t* fullOutputSize,
                       json deviceMap = NULL, json silkscreenMap = NULL, bool standaloneApp = false)
{
    if (inputPointer == nullptr || inputSize == 0)
        return CRASHDUMP_NOT_PROVIDED;

    std::string inputStr(inputPointer);
    inputStr.erase(std::remove_if(inputStr.begin(), inputStr.end(), ::isspace), inputStr.end());

    auto inputJson = json::parse(inputStr, nullptr, false);
    if (inputJson.is_discarded())
        return FAILED_TO_PARSE_CRASHDUMP_TO_JSON;

    return getFullOutput(inputJson, fullOutput, fullOutputSize, deviceMap, silkscreenMap, standaloneApp);
}

void getChaErrors(std::string socket, std::string mcerr, std::string sidebandPortId, nlohmann::json reportJson,
                  nlohmann::ordered_json& triageReport)
{
    for (const auto& [pciKey, pciVal] : reportJson["MCA"][socket].items())
    {
        for (const auto& [mcaKey, mcaVal] : pciVal.items())
        {
            if(!mcaVal.is_string())
            {
                continue;
            }
            // WA for finding CHA1 in CHA1. (CHA10, CHA11, etc.)
            std::string mcaValInFirstErr = mcaVal;
            mcaValInFirstErr += ",";

            if (mcaKey == "Bank" && std::string(mcaVal).find("CHA") != std::string::npos &&
                (mcerr.find(mcaValInFirstErr) != std::string::npos || sidebandPortId.find(mcaValInFirstErr) != std::string::npos))
            {
                std::string bdfStr = "";
                if (std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PRd") != std::string::npos &&
                    std::string(pciVal["Address"]).find("MMCFG") != std::string::npos)
                {
                    std::string addressStr = std::string(pciVal["Address"]).substr(0, std::string(pciVal["Address"]).find(" "));
                    uint64_t address = std::strtoull(addressStr.c_str(), NULL, 0);
                    bdfStr = BDFtoString(getBDFFromAddress(address));
                }

                if ((bdfStr == "" || bdfStr == "Please refer to system address map") &&
                    (std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PortIn") != std::string::npos ||
                     std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PortOut") != std::string::npos ||
                     std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PRd") != std::string::npos))
                {
                    uint64_t address = std::strtoull(std::string(pciVal["Address"]).c_str(), NULL, 0);
                    bdfStr = BDFtoString(PciBdfLookup::lookup(address));
                }

                if (bdfStr != "")
                {
                    triageReport[socket]["CHA_Error"]["Address"] = pciVal["Address"];
                    triageReport[socket]["CHA_Error"]["BDF"] = bdfStr;
                    triageReport[socket]["CHA_Error"]["CHA"] = pciVal["Bank"];
                }
            }
        }
    }

    if (!triageReport[socket].contains("CHA_Error"))
    {
        triageReport[socket]["CHA_Error"] = json::object();
    }
}

void getChaErrorsBhs(std::string socket, std::string mcerr, std::string sidebandPortId, nlohmann::json reportJson,
                  nlohmann::ordered_json& triageReport)
{
    nlohmann::json firstMcerrFalseChas = nlohmann::json::array();

    for (const auto& [dieId, dieVal] : reportJson["MCA"][socket].items())
    {
        for (const auto& [pciKey, pciVal] : dieVal.items())
        {
            for (const auto& [mcaKey, mcaVal] : pciVal.items())
            {
                if(!mcaVal.is_string())
                {
                    continue;
                }
                // WA for finding CHA1 in CHA1. (CHA10, CHA11, etc.)
                std::string mcaValInFirstErr = mcaVal;
                mcaValInFirstErr += " ";

                if (mcaKey == "Bank" && std::string(mcaVal).find("CHA") != std::string::npos)
                {
                    std::string bdfStr = "";
                    if (std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PRd") != std::string::npos &&
                        std::string(pciVal["Address"]).find("MMCFG") != std::string::npos)
                    {
                        std::string addressStr = std::string(pciVal["Address"]).substr(0, std::string(pciVal["Address"]).find(" "));
                        uint64_t address = std::strtoull(addressStr.c_str(), NULL, 0);
                        bdfStr = BDFtoString(getBDFFromAddress(address));
                    }

                    if ((bdfStr == "" || bdfStr == "Please refer to system address map") &&
                        (std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PortIn") != std::string::npos ||
                        std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PortOut") != std::string::npos ||
                        std::string(pciVal["Status_decoded"]["MSCOD_decoded"]).find("PRd") != std::string::npos))
                    {
                        uint64_t address = std::strtoull(std::string(pciVal["Address"]).c_str(), NULL, 0);
                        bdfStr = BDFtoString(PciBdfLookup::lookup(address));
                    }
                    
                    if (bdfStr == "")
                    {
                        continue;
                    }
                    if (mcerr.find(mcaValInFirstErr) != std::string::npos || sidebandPortId.find(mcaValInFirstErr) != std::string::npos)
                    {
                        triageReport[socket]["CHA_Error"]["Address"] = pciVal["Address"];
                        triageReport[socket]["CHA_Error"]["BDF"] = bdfStr;
                        triageReport[socket]["CHA_Error"]["CHA"] = pciVal["Bank"];
                        continue;
                    }

                    // First_MCERR_FALSE CHA errors - WA for CHA ID mapping issue
                    nlohmann::json chaError;
                    chaError["Address"] = pciVal["Address"];
                    chaError["BDF"] = bdfStr;
                    chaError["CHA"] = pciVal["Bank"];
                    firstMcerrFalseChas.push_back(chaError);
                }
            }
        }
    }

    if(triageReport[socket].contains("CHA_Error"))
    {
        return;
    }

    //else
    if (!firstMcerrFalseChas.empty())
    {
        triageReport[socket]["CHA_Error"]["First_MCERR_FALSE"] = firstMcerrFalseChas;
        return;
    }
    triageReport[socket]["CHA_Error"] = json::object();
}

bool isImcError(std::string val)
{
    return (val.find("IMC") != std::string::npos) || (val.find("MCDDR") != std::string::npos);
}

void addMcerrToFruSection(bool firstMcerr, std::string socket, nlohmann::json& pciVal, nlohmann::ordered_json &triageReport, nlohmann::ordered_json& pcieIioEntry, json& deviceMap)
{
    std::string mcerrSection = "First_MCERR";
    auto bdfObj = nlohmann::ordered_json::object();

    if(!firstMcerr)
    {
        mcerrSection += "_FALSE";
        bdfObj["First_MCERR"] = false;
    }

    bdfObj["Address"] = pciVal[mcerrSection]["Address"];
    std::string bdf_string = pciVal[mcerrSection]["BDF"];
    if (bdf_string == "Please refer to system address map")
    {
        bdfObj["Bus"] = bdf_string;
        bdfObj["Device"] = bdf_string;
        bdfObj["Function"] = bdf_string;
        triageReport[socket]["FRU"].push_back(bdfObj);
        pcieIioEntry["Failing_FRUs"].push_back(bdfObj);
    }
    else
    {
        getBdfFromFirstMcerrSection(bdf_string, bdfObj);
        showBdfDescription(deviceMap, bdfObj);
        triageReport[socket]["FRU"].push_back(bdfObj);
        pcieIioEntry["Failing_FRUs"].push_back(bdfObj);
    }
}

uint32_t getTriageInformation(const json& inputJson, char*& triageInfo, size_t* triageInfoSize,
                              json deviceMap = NULL, json silkscreenMap = NULL, bool standaloneApp = false)
{
    auto input = getProperRootNode(inputJson);
    if (!input)
        return INCORRECT_JSON_ROOT_NODE;

    auto summary = prepareSummary(*input);
    if (summary.cpuType == "")
        return CPU_TYPE_NOT_RECOGNIZED;
    if (!standaloneApp)
    {
        if (checkIfMapsAreSymlinkFiles())
        {
            return SYMLINK_FILE;
        }
        if (checkIfMapsAreHardlinkFiles())
        {
            return HARDLINK_FILE;
        }
        PciBdfLookup deviceLookup((char*)"", true, DEVICE_MAP_FILENAME);
        PciBdfLookup silkscreenLookup((char*)"", true, SILKSCREEN_MAP_FILENAME);
        PciBdfLookup PciBdfLookup((char*)"", true, MEMORY_MAP_FILENAME);
        deviceMap = deviceLookup.lookupMap;
        silkscreenMap = silkscreenLookup.lookupMap;
    }

    json reportJson = prepareReport(summary, *input, deviceMap, silkscreenMap);
    if (reportJson.is_discarded())
        return FAILED_TO_PREPARE_REPORT;
    nlohmann::ordered_json report;
    nlohmann::ordered_json priority;
    nlohmann::ordered_json& triageReport = report["triage"];
    nlohmann::ordered_json& replacementPriority = priority["FRU_Replacement_Priority"];
    nlohmann::ordered_json cpuThermalEntry;
    nlohmann::ordered_json imcEntry;
    nlohmann::ordered_json dimmEntry;
    nlohmann::ordered_json cpuUpiEntry;
    nlohmann::ordered_json pcieIioEntry;
    nlohmann::ordered_json cpuEntry;
    std::string firstError = "";
    std::stringstream ss;
    std::stringstream ss2;
    std::regex core_regex("([0-9])-([0-9])");
    std::smatch matches;
    bool imc_present = false;
    bool aerCorSection = false;
    bool aerUncorSection = false;
    bool fru_section = false;
    std::string imc_string;
    std::string ierr;
    std::string mcerr;
    std::string sidebandPortId;
    std::string rmcaMcerr;
    std::string die;
    std::vector<std::string> sockets;
    triageReport["BAFI_version"] = BAFI_VERSION;
    triageReport["CPU_type"] = getFullVersionName(*input);
    triageReport["Crashdump_timestamp"] = getTimestamp(*input);
    // Determine on which socket the error occurred first
    std::string firstErrorSocket = findFirstErrorSocket(reportJson, firstError);
    findSocketsFromSummary(sockets, reportJson);
    if (firstErrorSocket != "")
    {
        triageReport["First_Error_Occurred_On_Socket"] = firstErrorSocket.substr(6);
        cpuEntry["First_Error_Occurred_On_Socket"] = firstErrorSocket.substr(6);
    }
    else
    {
        if (sockets.size() > 1)
        {
            triageReport["First_Error_Occurred_On_Socket"] = "N/A";
        }
        else
        {
            triageReport["First_Error_Occurred_On_Socket"] = "0";
            cpuEntry["First_Error_Occurred_On_Socket"] = "0";
        }
    }
    // Get descriptive information for each error
    for (const auto& socket : sockets)
    {
        std::optional<std::string> bankDescription;
        ss.str("");
        ss << "S" << socket.substr(6) << ".";
        for (const auto& [summaryKey, summaryVal] : reportJson["output_summary"][socket].items())
        {
            if (!summaryVal.contains("Errors_Per_Socket"))
            {
                continue;
            }
            getErrorStrings(std::string(summaryVal["Errors_Per_Socket"]), ierr, mcerr, rmcaMcerr, sidebandPortId, summary.cpuType);
            firstError = determineFirstErrorType(ierr, mcerr, rmcaMcerr);
            if (firstErrorSocket == "")
            {
                if (sockets.size() == 1 && ierr == "0x0" && mcerr == "0x0" && rmcaMcerr == "0x0")
                {
                    triageReport["First_Error_Occurred_On_Socket"] = "N/A";
                    cpuEntry.erase("First_Error_Occurred_On_Socket");
                }
            }

            if (firstError == "FirstIERR")
            {
                ss << "IERR.";

                bankDescription = firstMcerrInMce(ierr, reportJson["output_summary"][socket]);

                if (!bankDescription)
                {
                    ss << "FirstIERR != MCE bank#, manual review required";
                    break;
                }

                if (isImcError(ierr))
                {
                    imc_present = true;
                    if(summary.cpuType == "GNR" || summary.cpuType == "SRF")
                    {
                        imc_string = getImcStringBhs(ierr);
                    }
                    else
                    {
                        imc_string = getImcStringGeneric(*bankDescription);
                    }
                    break;
                }
                // else
                break;
            }
            
            // else
            if (mcerr == "0x0")
            {
                if(firstError == "RMCA_FirstMCERR" && rmcaMcerr != "0x0")
                {
                    mcerr = rmcaMcerr;
                }
                else
                {
                    ss << "N/A";
                    break;
                }
            }

            bankDescription = firstMcerrInMce(mcerr, reportJson["output_summary"][socket]);

            if (!bankDescription)
            {
                ss << "FirstMCERR != MCE bank#, manual review required";
                break;
            }
            ss << "MCERR.";
            if (isImcError(mcerr))
            {
                imc_present = true;
                if(summary.cpuType == "GNR" || summary.cpuType == "SRF")
                {
                    imc_string = getImcStringBhs(mcerr);
                }
                else
                {
                    imc_string = getImcStringGeneric(*bankDescription);
                }
            }
            break;
        }
        if(summary.cpuType == "GNR" || summary.cpuType == "SRF")
        {
            for (const auto& [summaryKey, summaryVal] : reportJson["output_summary"][socket].items())
            {
                if (!summaryVal.contains("MCE"))
                {
                    continue;
                }
                for (const auto& [dieId, dieVal] : summaryVal["MCE"].items())
                {
                    for (const auto& [mceKey, mceVal] : dieVal.items())
                    {
                        if (std::string(mceKey).find("MCHN") != std::string::npos)
                        {
                            triageReport[socket]["IMC_Error"].push_back(mceKey);
                            imcEntry[socket].push_back(mceKey);
                        }
                        if (std::string(mceKey).find("UPI") != std::string::npos)
                        {
                            triageReport[socket]["UPI_Error"].push_back(mceKey);
                            cpuUpiEntry["UPI_Errors"][socket].push_back(mceKey);
                        }
                    }
                }
            }
        }
        else
        {
            for (const auto& [summaryKey, summaryVal] : reportJson["output_summary"][socket].items())
            {
                if (!summaryVal.contains("MCE"))
                {
                    continue;
                }
                for (const auto& [mceKey, mceVal] : summaryVal["MCE"].items())
                {

                    if (std::string(mceKey).find("IMC") != std::string::npos)
                    {
                        triageReport[socket]["IMC_Error"].push_back(mceKey);
                        imcEntry[socket].push_back(mceKey);
                    }
                    if (std::string(mceKey).find("HBM") != std::string::npos)
                    {
                        triageReport[socket]["HBM_Error"].push_back(mceKey);
                    }
                    if (std::string(mceKey).find("UPI") != std::string::npos)
                    {
                        triageReport[socket]["UPI_Error"].push_back(mceKey);
                        cpuUpiEntry["UPI_Errors"][socket].push_back(mceKey);
                    }
                }
            }
        }

        if (!triageReport[socket].contains("IMC_Error"))
        {
            triageReport[socket]["IMC_Error"] = json::object();
        }
        if (!triageReport[socket].contains("HBM_Error"))
        {
            triageReport[socket]["HBM_Error"] = json::object();
        }
        if (!triageReport[socket].contains("UPI_Error"))
        {
            triageReport[socket]["UPI_Error"] = json::object();
        }
        
        if(bankDescription)
        {
            ss << *bankDescription;
        }

        nlohmann::ordered_json error_info = ss.str();
        triageReport[socket]["Error_Information"].push_back(error_info);
        // Get FRU section in case IMC bank is present in errors
        std::stringstream ss_fru;
        if (imc_present)
        {
            nlohmann::ordered_json fru_info = imc_string;
            triageReport[socket]["FRU"].push_back(fru_info);
            pcieIioEntry["Failing_FRUs"].push_back(fru_info);
        }
        // Get FRU section for all other banks
        for (const auto& [pciKey, pciVal] : reportJson["output_summary"][socket].items())
        {
            if (pciVal.contains("First_MCERR"))
            {
                addMcerrToFruSection(true, socket, pciVal, triageReport, pcieIioEntry, deviceMap);
                fru_section = true;
            }
            if (pciVal.contains("First_MCERR_FALSE"))
            {
                addMcerrToFruSection(false, socket, pciVal, triageReport, pcieIioEntry, deviceMap);
                fru_section = true;
            }

            if (pciVal.contains("IO_Errors"))
            {
                fru_section = true;

                if (summary.cpuType == "GNR" || summary.cpuType == "SRF")
                {
                    for (const auto& [dieId, dieData] : pciVal["IO_Errors"].items())
                    {
                        auto bdfObj = nlohmann::ordered_json::object();
                        std::string bdf_string = dieData["Device"];
                        getBdfFromIoErrorsSection(bdf_string, bdfObj);
                        if (dieData.contains("Description"))
                        {
                            bdfObj["Description"] = dieData["Description"];
                        }
                        triageReport[socket]["FRU"].push_back(bdfObj);
                        pcieIioEntry["Failing_FRUs"].push_back(bdfObj);
                    }
                }
                else
                {
                    auto bdfObj = nlohmann::ordered_json::object();
                    std::string bdf_string = pciVal["IO_Errors"]["Device"];
                    getBdfFromIoErrorsSection(bdf_string, bdfObj);
                    if (pciVal["IO_Errors"].contains("Description"))
                    {
                        bdfObj["Description"] = pciVal["IO_Errors"]["Description"];
                    }
                    triageReport[socket]["FRU"].push_back(bdfObj);
                    pcieIioEntry["Failing_FRUs"].push_back(bdfObj);
                }
            }
        }
        if (!fru_section)
        {
            triageReport[socket]["FRU"] = json::object();
        }

        fru_section = false;
        if (summary.cpuType == "SPR" || summary.cpuType == "SPR-HBM" || summary.cpuType == "ICX" || summary.cpuType == "ICX" || summary.cpuType == "GNR" || summary.cpuType == "SRF")
        {
            bool chaFirstMcerr = true;
            // Get CHA errors based on First_MCERR or SIDEBANDPORTID
            if(summary.cpuType == "GNR" || summary.cpuType == "SRF")
            {
                getChaErrorsBhs(socket, mcerr, sidebandPortId, reportJson, triageReport);
                // WA for CHA ID mapping issue
                if(triageReport[socket]["CHA_Error"].contains("First_MCERR_FALSE"))
                {
                    for (const auto& entry : triageReport[socket]["CHA_Error"]["First_MCERR_FALSE"])
                    {
                        pcieIioEntry["Failing_FRUs"].push_back(entry);
                    }
                    chaFirstMcerr = false;
                }
            }
            else
            {
                getChaErrors(socket, mcerr, sidebandPortId, reportJson, triageReport);
            }
            if (chaFirstMcerr && (!triageReport[socket]["CHA_Error"].empty()))
            {
                pcieIioEntry["Failing_FRUs"].push_back(triageReport[socket]["CHA_Error"]);
            }
        }
        else
        {
            triageReport[socket]["CHA_Error"] = json::object();
        }

        getPPIN(*input, triageReport, socket);
        getUcodePath(*input, triageReport, socket);
    }
    // Get AER sections
    sockets.clear();
    findSocketsFromSummary(sockets, reportJson);
    for (const auto& socket : sockets)
    {
        getMemoryErrors(reportJson, socket, triageReport, dimmEntry);
        for (const auto& [pciKey, pciVal] : reportJson["output_summary"][socket].items())
        {
            if (pciVal.contains("PCIe_AER_Uncorrectable_errors"))
            {
                triageReport[socket]["PcieAerUncorrectable"] = pciVal["PCIe_AER_Uncorrectable_errors"];
                aerUncorSection = true;
            }
            if (pciVal.contains("PCIe_AER_Correctable_errors"))
            {
                triageReport[socket]["PcieAerCorrectable"] = pciVal["PCIe_AER_Correctable_errors"];
                aerCorSection = true;
            }
        }

        if (!aerCorSection)
        {
            triageReport[socket]["PcieAerCorrectable"] = json::object();
        }
        if (!aerUncorSection)
        {
            triageReport[socket]["PcieAerUncorrectable"] = json::object();
        }

        getThermalErrors(socket, triageReport, cpuThermalEntry, summary);
        aerUncorSection = false;
        aerCorSection = false;
    }
    // Create FRU replacement priority node entries
    if (!cpuThermalEntry.empty())
    {
        cpuThermalEntry["Suggested_actions"] = "Monitor CPU temperature / check CPU fans";
        replacementPriority["CPU_Thermal"] = cpuThermalEntry;
    }

    if (!imcEntry.empty())
    {
        replacementPriority["DIMM"]["IMC_Error"] = imcEntry;
    }

    if (!dimmEntry.empty())
    {
        replacementPriority["DIMM"]["Memory_errors"] = dimmEntry;
    }

    if (replacementPriority.contains("DIMM"))
    {
        replacementPriority["DIMM"]["Suggested_actions"] = "Replace DIMM";
    }

    if (!cpuUpiEntry.empty())
    {
        cpuUpiEntry["Suggested_actions"] = "Replace CPU or mainboard";
        replacementPriority["CPU_UPI"] = cpuUpiEntry;
    }

    if (!pcieIioEntry.empty())
    {
        pcieIioEntry["Suggested_actions"] = "Replace PCIe/IIO or the device behind the root port";
        replacementPriority["PCIe/IIO"] = pcieIioEntry;
    }
    if (!cpuEntry.empty())
    {
        cpuEntry["Suggested_actions"] = "Replace CPU";
        replacementPriority["CPU"] = cpuEntry;
    }

    triageReport.merge_patch(priority);

    // Assign ErrorTriage to char pointer
    std::string triageStr = report.dump(4);
    *triageInfoSize = triageStr.size();
    triageInfo = new char[*triageInfoSize + 1];
    triageStr.copy(triageInfo, *triageInfoSize);
    triageInfo[*triageInfoSize] = '\0';
    return SUCCESS;
}

uint32_t getTriageInformation(char* inputPointer, std::size_t inputSize, char*& triageInfo, size_t* triageInfoSize,
                              json deviceMap = NULL, json silkscreenMap = NULL, bool standaloneApp = false)
{
    if (inputPointer == nullptr || inputSize == 0)
        return CRASHDUMP_NOT_PROVIDED;

    std::string inputStr(inputPointer);
    inputStr.erase(std::remove_if(inputStr.begin(), inputStr.end(), ::isspace), inputStr.end());

    auto inputJson = json::parse(inputStr, nullptr, false);
    if (inputJson.is_discarded())
        return FAILED_TO_PARSE_CRASHDUMP_TO_JSON;

    return getTriageInformation(inputJson, triageInfo, triageInfoSize, deviceMap, silkscreenMap, standaloneApp);
}
