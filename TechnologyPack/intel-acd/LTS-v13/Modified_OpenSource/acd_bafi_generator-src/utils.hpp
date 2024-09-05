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

#pragma once
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_set>
#include <variant>

#include "tor_defs.hpp"
#include "mca_defs.hpp"

using json = nlohmann::json;

const static uint8_t decimal = 10;

[[nodiscard]] static inline bool startsWith(const std::string &input, std::string &&search)
{
    return (input.substr(0, search.length()) == search);
}

[[nodiscard]] bool checkInproperValue(const std::string &input)
{
    if (startsWith(input, "UA") || startsWith(input, "N/A") || input == "")
    {
        return true;
    }
    return false;
}

template <typename T>[[nodiscard]] bool str2uint(const std::string &stringValue, T &output, uint8_t base = 16)
{
    try
    {
        output = std::stoull(stringValue, nullptr, base);
    }
    catch (...)
    {
        if (!checkInproperValue(stringValue))
        {
            return false;
        }
    }
    return true;
}

template <typename T>[[nodiscard]] inline std::string int_to_hex(T val, size_t width = sizeof(T), bool prefix = true)
{
    std::stringstream ss;
    if (prefix)
        ss << "0x";

    ss << std::setfill('0') << std::setw(width) << std::hex << (val | 0);
    return ss.str();
}

template <typename T>[[nodiscard]] std::optional<std::string> getDecoded(std::map<T, const char *> decodingTable, T toDecode)
{
    const auto &item = decodingTable.find(toDecode);
    if (item != decodingTable.cend())
    {
        return item->second;
    }
    return {};
}

std::optional<std::reference_wrapper<const json>> getJsonNode(const json& input)
{
    // using (void)(input) to get rid of compilation error about unused parameter
    // TODO: consider removing -Werror=unused-parameter
    (void)(input);
    return std::nullopt;
}

template <typename T, typename... Args> std::optional<std::reference_wrapper<const json>> getJsonNode(const json& input, T first, Args... args)
{
    if (input.contains(first))
    {
        if (sizeof...(args) == 0)
        {
            return input[first];
        }

        return getJsonNode(input[first], args...);
    }

    return std::nullopt;
}

template <typename T, typename... Args> std::optional<std::string> getJsonStringValue(const json& input, T first, Args... args)
{
    auto foundNode = getJsonNode(input, first, args...);
    if (foundNode && foundNode->get().is_string() && (!checkInproperValue(foundNode->get())))
    {
        return foundNode->get();
    }

    return std::nullopt;
}

template <typename R, typename T, typename... Args> std::optional<R> getJsonUintValue(const json& input, T first, Args... args)
{
    auto foundString = getJsonStringValue(input, first, args...);
    R ret;
    if (foundString && str2uint(*foundString, ret))
    {
        return ret;
    }

    return std::nullopt;
}

template <typename R, typename T, typename... Args> R getJsonUintValue(R defaultVaule, const json& input, T first, Args... args)
{
    auto foundValue = getJsonUintValue<R>(input, first, args...);
    if (!foundValue)
    {
        return defaultVaule;
    }

    return *foundValue;
}

void showBdfDescription(nlohmann::json deviceMap, nlohmann::ordered_json &bdfObject)
{
    if (deviceMap != NULL)
    {
        for (const auto &[devKey, devVal] : deviceMap["devicesMap"].items())
        {
            if (devVal["bus"] == std::string(bdfObject["Bus"]) && devVal["device"] == std::string(bdfObject["Device"]) &&
                devVal["function"] == std::string(bdfObject["Function"]))
            {
                bdfObject["Description"] = devVal["description"];
            }
        }
    }
}

void getBdfFromFirstMcerrSection(std::string bdfString, nlohmann::ordered_json &bdfObj)
{
    if (bdfString != "Please refer to system address map")
    {
        std::size_t left = bdfString.find("Bus");
        std::size_t right = bdfString.find(",");
        bdfObj["Bus"] = bdfString.substr(left, right).substr(5);
        left = bdfString.find("Device");
        right = bdfString.substr(right + 2).find(",");
        bdfObj["Device"] = bdfString.substr(left, right).substr(8);
        left = bdfString.find("Function");
        bdfObj["Function"] = bdfString.substr(left, bdfString.size()).substr(10);
    }
}

void getBdfFromIoErrorsSection(std::string bdfString, nlohmann::ordered_json &bdfObj)
{
    if (bdfString != "Please refer to system address map")
    {
        std::size_t left = bdfString.find("Bus");
        std::size_t right = bdfString.find("] ");
        bdfObj["Bus"] = bdfString.substr(left, right).substr(4);
        left = bdfString.find("Device");
        right = bdfString.substr(right + 2).find("]");
        bdfObj["Device"] = bdfString.substr(left, right).substr(7);
        left = bdfString.find("Function");
        bdfObj["Function"] = bdfString.substr(left, bdfString.size() - left - 1).substr(9);
    }
}

void printHelp() {
  std::cerr << "\nUsage:\n";
  std::cerr << "  bafi [-d][-t][-v][-h][-m memory_map_file][-p device_map_file]";
  std::cerr << "[-s silkscreen_map_file] crashdump_file\n\n";
  std::cerr << "Options:\n";
  std::cerr << "  -d\t\t\tgenerate BAFI output from all crashdump files in current folder\n";
  std::cerr << "  -t\t\t\tprint triage information\n";
  std::cerr << "  --triage\t\tprint triage information\n";
  std::cerr << "  -v\t\t\tversion information\n";
  std::cerr << "  --version\t\tversion information\n";
  std::cerr << "  -h\t\t\tprint help\n";
  std::cerr << "  --help\t\tprint help\n";
  std::cerr << "  -m\t\t\timport memory map from json file\n";
  std::cerr << "  --memory-map\t\timport memory map from json file\n";
  std::cerr << "  -p\t\t\timport device map from json file\n";
  std::cerr << "  --pcie_names\t\timport device map from json file\n";
  std::cerr << "  -s\t\t\timport silkscreen map from json file\n";
  std::cerr << "  --silkscreen_names\timport silkscreen map from json file\n";
  std::cerr << "  memory_map_file\tJSON file containing memory map ";
  std::cerr << "information\n";
  std::cerr << "  device_map_file\tJSON file containing device map ";
  std::cerr << "information\n";
  std::cerr << "  silkscreen_map_file\tJSON file containing silkscreen map ";
  std::cerr << "information\n";
  std::cerr << "  crashdump_file\tJSON file containing Autonomous Crashdump\n";
  std::cerr << "\n\nInstructions:\n";
  std::cerr << "  Step 1: In Summary TSC section, locate which TSC register has";
  std::cerr << " \"Occurred first between all TSCs\" tag.\n";
  std::cerr << "  Step 2: From the first MCERR or first IERR information in step 1, locate the";
  std::cerr << "IP and MCE bank number that asserted FirstIERR, FirstMCERR and";
  std::cerr << "RMCA_FirstMCERR (i.e. FirstMCERR=CHA1 bank 10 M2MEM 0 bank 12, UPI 0 bank 5).\n";
  std::cerr << "  Step 3: Correlate above MCE bank number with error ";
  std::cerr << "information from MCE section or find device Bus/Device/";
  std::cerr << "Function number in 'First.MCERR' section if available.\n\n";
  std::cerr << "I have cscript memory map created using pci.resource_check() ";
  std::cerr << "command or iomem.txt from Linux /proc/iomem. Can I use it in BAFI?\n";
  std::cerr << "  Yes. You can use tools/cscript_map_converter.py like this:\n\n";
  std::cerr << "     > python3 csript_map_converter.py ";
  std::cerr << "<cscript_memory_map_file_format> or <iomem.txt> [-o output_path]\n\n";
  std::cerr << "  This command will create default_memory_map.json that can be ";
  std::cerr << "later used in BAFI using -m (--memory-map) option or by ";
  std::cerr << "copying this map to proper location described in README file.\n\n";
  std::cerr << "I have cscript PCIe map created using pci.devices() ";
  std::cerr << "command or lspci command output from Linux. Can I use it in BAFI?\n";
  std::cerr << "  Yes. You can use tools/devices_log_converter.py like this:\n\n";
  std::cerr << "     > python3 devices_log_converter.py ";
  std::cerr << "<cscript_device_map_file_format> or <lspci.txt> [-o output_path]\n\n";
  std::cerr << "  This command will create default_device_map.json that can be ";
  std::cerr << "later used in BAFI using -p (--pcie_names) option.\n\n";
}

std::string BDFtoString(const std::tuple<uint8_t, uint8_t, uint8_t> &bdf)
{
  auto &[bus, dev, func] = bdf;
  if (bus == 0 && dev == 0 && func == 0)
  {
      return "Please refer to system address map";
  }
  std::stringstream ss;
  ss << "Bus: " << int_to_hex(bus, false) << ", ";
  ss << "Device: " << int_to_hex(dev, false) << ", ";
  ss << "Function: " << int_to_hex(func, false);
  return ss.str();
}

std::tuple<uint8_t, uint8_t, uint8_t> getBDFFromAddress(uint32_t address)
{
    BDFFormatter bdf;
    bdf.address = address;
    return std::make_tuple(static_cast<uint8_t>(bdf.bus), static_cast<uint8_t>(bdf.dev), static_cast<uint8_t>(bdf.func));
}

template <typename T> std::string DecodeBits(const std::map<std::vector<uint8_t>, const char *> decode, T data)
{
    std::stringstream ss;
    T mask;

    for (auto const &[key, val] : decode)
    {
        mask = 0;
        for (auto const &position : key)
        {
            if (position > std::numeric_limits<T>::digits)
            {
                throw std::out_of_range("Bit position not in correct range");
            }
            /*Reason for False Positive - It has been confirmed whether the bit position is not within the correct range.*/
            /* coverity[large_shift : FALSE] */
            mask |= 1 << position;
        }

        if (data & mask)
        {
            ss << val << "|";
        }
    }

    return ss.str();
}

std::vector<std::string> findCpus(const json& input)
{
    std::vector<std::string> cpus;
    const auto& metaData = input.find("METADATA");
    if (metaData != input.cend())
    {
        for (const auto& metaDataItem : metaData.value().items())
        {
            if (startsWith(metaDataItem.key(), "cpu"))
            {
                cpus.push_back(metaDataItem.key());
            }
        }
    }
    return cpus;
}

std::map<std::string, std::reference_wrapper<const json>> getAllCpuSections(const json& processorsSection,
                                                                            std::vector<std::string> cpus)
{
    std::map<std::string, std::reference_wrapper<const json>> allCpuSections;
    for (const auto& cpu : cpus)
    {
        auto cpuSection = getJsonNode(processorsSection, cpu);
        if (!cpuSection)
        {
            continue;
        }

        allCpuSections.insert(std::pair<std::string, std::reference_wrapper<const json>>(cpu, *cpuSection));
    }
    return allCpuSections;
}

std::map<std::string, std::map<std::string, std::reference_wrapper<const json>>>
    getAllDieSections(const json& processorsSection, std::vector<std::string> cpus)
{
    std::map<std::string, std::map<std::string, std::reference_wrapper<const json>>> allDiesSections;
    for (const auto& cpu : cpus)
    {
        auto cpuSection = getJsonNode(processorsSection, cpu);
        if (!cpuSection)
        {
            continue;
        }

        for (const auto& [die, dieSection] : cpuSection->get().items())
        {
            if (!(startsWith(die, "soc") || startsWith(die, "io") || startsWith(die, "compute")))
            {
                continue;
            }
            allDiesSections[cpu].insert(std::pair<std::string, std::reference_wrapper<const json>>(die, dieSection));
        }
    }
    return allDiesSections;
}

std::optional<std::reference_wrapper<const json>> getProcessorsSection(const json& input)
{
    const auto& processorsSection = input.find("CPU");
    if (processorsSection != input.cend())
    {
        return *processorsSection;
    }

    const auto& processorsSection1 = input.find("PROCESSORS");
    if (processorsSection1 != input.cend())
    {
        return *processorsSection1;
    }
    return std::nullopt;
}

const std::map<std::string, std::reference_wrapper<const json>> prepareJsonGeneric(const json& input)
{
    std::vector<std::string> cpus = findCpus(input);
    std::optional processorsSection = getProcessorsSection(input);
    auto cpuSections = getAllCpuSections(*processorsSection, cpus);
    return cpuSections;
}

const std::map<std::string, std::map<std::string, std::reference_wrapper<const json>>> prepareJsonBhs(const json& input)
{
    std::vector<std::string> cpus = findCpus(input);
    std::optional processorsSection = getProcessorsSection(input);
    auto dieSections = getAllDieSections(*processorsSection, cpus);
    return dieSections;
}

template <typename T>
bool parseUintField(const json& data, const std::string& key, T& fieldValue, std::optional<T> defaultValue = std::nullopt)
{
    if (data.find(key) == data.cend() || checkInproperValue(data[key]))
    {
        if (defaultValue != std::nullopt)
        {
            fieldValue = *defaultValue;
            return true;
        }

        return false;
    }

    bool ret = str2uint(data[key], fieldValue);

    return ret;
}

struct McBankProperties
{
    McaTypeFlag typeFlag;
    std::function<std::optional<uint32_t>(std::string)> getBankNumber;
    std::function<bool(std::string)> compareBank;

    McBankProperties(std::string firstLetters, McaTypeFlag flag) : typeFlag(flag)
    {
        compareBank = [&, firstLetters](std::string mcLower) {
            std::string temp = firstLetters;
            return startsWith(mcLower, std::move(temp));
        };

        getBankNumber = [firstLetters](std::string mcLower) -> std::optional<uint32_t> {
            uint32_t bankNumber;
            if (!str2uint(mcLower.substr(firstLetters.length()), bankNumber, 10))
                return std::nullopt;

            return bankNumber;
        };
    }

    McBankProperties(McaTypeFlag flag, std::function<std::optional<uint32_t>(std::string)> getBankNumberIn,
                     std::function<bool(std::string)> compareBankIn) :
        typeFlag(flag),
        getBankNumber(getBankNumberIn), compareBank(compareBankIn)
    {
    }
};

bool SetMcaBanksProperties(std::vector<McBankProperties> bankProperties, MCAData& bankData, std::string bankString)
{
    for (const auto& property : bankProperties)
    {
        if (property.compareBank(bankString))
        {
            auto number = property.getBankNumber(bankString);
            if (!number)
            {
                continue;
            }
            bankData.bank = *number;
            bankData.type = property.typeFlag;

            return true;
        }
    }

    return false;
}

class NumbersRange
{
    std::vector<std::variant<int, std::tuple<uint32_t, uint32_t, uint32_t>>> numbers;
    uint32_t step = 0;

  public:
    NumbersRange(std::vector<std::variant<int, std::tuple<uint32_t, uint32_t, uint32_t>>> numbersIn) : numbers(numbersIn)
    {
    }

    std::vector<uint32_t> get() const
    {
        std::vector<uint32_t> ret;

        for (const auto& num : numbers)
        {
            if (std::holds_alternative<int>(num))
            {
                ret.push_back(static_cast<uint32_t>(std::get<int>(num)));
            }
            else if (std::holds_alternative<std::tuple<uint32_t, uint32_t, uint32_t>>(num))
            {
                auto& [lower, higher, step] = std::get<std::tuple<uint32_t, uint32_t, uint32_t>>(num);
                for (uint32_t i = lower; i <= higher; i += step)
                {
                    ret.push_back(i);
                }
            }
            else
            {
                throw std::bad_variant_access();
            }
        }
        return ret;
    }
};

struct RegisterSet
{
    NumbersRange B;
    NumbersRange D;
    NumbersRange F;
    NumbersRange offset;
};

class BdfRegisterSets
{
  private:
    std::vector<RegisterSet> registersWithEmptyFields;
    std::unordered_set<std::string> parsedRegisters;

    bool findInString(std::string data, std::vector<std::string> matches) const
    {
        if (matches.size() == 0)
        {
            return true;
        }

        for (const auto& match : matches)
        {
            if (data.find(match) != std::string::npos)
            {
                return true;
            }
        }

        return false;
    }

    bool containsAnyMatch(std::string data) const
    {
        std::stringstream ss;
        for (auto& reg : registersWithEmptyFields)
        {
            std::vector<std::string> busMatches;
            std::vector<std::string> deviceMatches;
            std::vector<std::string> functionMatches;
            std::vector<std::string> offsetMatches;
            for (const auto& b : reg.B.get())
            {
                ss.str(std::string());
                ss << std::dec << "B" << std::setw(2) << std::setfill('0') << b;
                busMatches.push_back(ss.str());
            }

            for (const auto& d : reg.D.get())
            {
                ss.str(std::string());
                ss << std::dec << "_D" << std::setw(2) << std::setfill('0') << d;
                deviceMatches.push_back(ss.str());
            }

            for (const auto& f : reg.F.get())
            {
                ss.str(std::string());
                ss << std::dec << "_F" << f;
                functionMatches.push_back(ss.str());
            }

            for (const auto& o : reg.offset.get())
            {
                ss.str(std::string());
                ss << "_0x" << std::hex << o;
                offsetMatches.push_back(ss.str());
            }

            if (findInString(data, busMatches) && findInString(data, deviceMatches) && findInString(data, functionMatches) &&
                findInString(data, offsetMatches))
            {
                return true;
            }
        }

        return false;
    }

  public:
    BdfRegisterSets(std::vector<RegisterSet> registers)
    {
        std::stringstream ss;

        for (auto& reg : registers)
        {
            auto bus = reg.B.get();
            auto device = reg.D.get();
            auto function = reg.F.get();
            auto offset = reg.offset.get();

            if (bus.empty() || device.empty() || function.empty() || offset.empty())
            {
                registersWithEmptyFields.push_back(reg);
                continue;
            }

            for (const auto& b : reg.B.get())
            {
                for (const auto& d : reg.D.get())
                {
                    for (const auto& f : reg.F.get())
                    {
                        for (const auto& o : reg.offset.get())
                        {
                            ss.str(std::string());
                            ss << std::dec;
                            ss << "B" << std::setw(2) << std::setfill('0') << b;
                            ss << "_D" << std::setw(2) << std::setfill('0') << d;
                            ss << "_F" << f;
                            ss << "_0x" << std::hex << o;
                            parsedRegisters.insert(ss.str());
                        }
                    }
                }
            }
        }
    }

    std::unordered_set<std::string> getRegisters() const
    {
        return parsedRegisters;
    }

    bool contains(std::string data) const
    {
        return parsedRegisters.count(data) > 0 || containsAnyMatch(data);
    }
};

std::optional<std::string> bankInMce(json& summaryEntry, std::string bankNumber, std::optional<std::string> die)
{
    json summarySection;
    if (die)
    {
        summarySection = summaryEntry[0]["MCE"][*die];
    }
    else
    {
        summarySection = summaryEntry[0]["MCE"];
    }
    for (auto const& bankError : summarySection.items())
    {
        if (std::string(bankError.key()).find(bankNumber) != std::string::npos)
        {
            return bankError.key();
        }
    }

    return std::nullopt;
}

std::optional<std::string> getFirstErrInformation(std::string firstMcerrSrc, std::string foundBank)
{
    // "|" separates bank name from MCACOD and MSCOD data in MCE
    return firstMcerrSrc + " " +  foundBank.substr(foundBank.find("|"));
}

std::optional<std::string> firstMcerrInMce(std::string firstMcerrSrc, json& summaryEntry)
{
    std::smatch matches;
    std::smatch dieMatches;
    std::regex coreRegex("bank ([0-9])-([0-9])");
    std::regex bankRegex("bank ([0-9]*)[, ]*([0-9]*)[, ]*([0-9]*)");
    std::regex dieRegex("(compute|io|soc)[0-9]*");

    std::optional<std::string> die;
    if (std::regex_search(firstMcerrSrc, dieMatches, dieRegex))
    {
        die = dieMatches[0];
    }

    if (summaryEntry != nullptr && summaryEntry[0].contains("MCE"))
    {
        if (std::regex_search(firstMcerrSrc, matches, coreRegex))
        {

            for (int i = std::stoi(matches[1].str()); i <= std::stoi(matches[2].str()); i++)
            {
                std::string bankDescription = "bank " + std::to_string(i) + " ";
                auto foundBank = bankInMce(summaryEntry, bankDescription, die);
                if (foundBank)
                {
                    return getFirstErrInformation(firstMcerrSrc, *foundBank);
                }
            }
        }
        else if (std::regex_search(firstMcerrSrc, matches, bankRegex))
        {
            std::size_t left = 0;
            std::size_t right = 0;
            if(die)
            {
                right = firstMcerrSrc.find(*die);
            }
            else
            {
                right = firstMcerrSrc.find(",");
            }

            if (startsWith(firstMcerrSrc, "0x"))
            {
                // skip hex number at beginning of firstMcerrSrc
                left = firstMcerrSrc.find(" ") + 1;
            }

            std::string bankDescription = "bank " + firstMcerrSrc.substr(left, right-left);
            auto foundBank = bankInMce(summaryEntry, bankDescription, die);
            if (foundBank)
            {
                return getFirstErrInformation(firstMcerrSrc, *foundBank);
            }

            uint32_t i = 1;
            while (matches[i].str() != "")
            {
                bankDescription = "bank " + matches[i].str() + " ";

                auto foundBank = bankInMce(summaryEntry, bankDescription, die);
                if (foundBank)
                {
                    return getFirstErrInformation(firstMcerrSrc, *foundBank);
                }
                i++;
            }
            
        }
    }
    return std::nullopt;
}
