#pragma once
#include "mca_defs.hpp"
#include "summary_field.hpp"
#include "utils.hpp"

#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <variant>
#include <vector>

using json = nlohmann::json;

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

std::map<std::string, std::vector<std::string>> findDies(const json& input, std::vector<std::string> cpus)
{
    std::map<std::string, std::vector<std::string>> dies;

    if (!input.contains("METADATA"))
    {
        return dies;
    }

    for (const auto& cpu : cpus)
    {
        if (!input["METADATA"].contains(cpu))
        {
            throw std::out_of_range("Cpu not found in METADATA section.");
        }

        std::vector<std::string> cpuDies;

        for (const auto& cpuSectionItem : input["METADATA"][cpu].items())
        {
            if (startsWith(cpuSectionItem.key(), "compute") || startsWith(cpuSectionItem.key(), "io"))
            {
                cpuDies.push_back(cpuSectionItem.key());
            }
        }

        dies[cpu] = cpuDies;
    }

    return dies;
}

std::optional<std::reference_wrapper<const json>> getCpuSection(std::string cpu, const json& input)
{
    const auto& cpuSection = input.find(cpu);
    if (cpuSection != input.cend())
    {
        return *cpuSection;
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<const json>> getDieSection(std::string die, const json& input)
{
    const auto& dieSection = input.find(die);
    if (dieSection != input.cend())
    {
        return *dieSection;
    }
    return std::nullopt;
}

std::map<std::string, std::reference_wrapper<const json>> getAllCpuSections(const json& processorsSection,
                                                                            std::vector<std::string> cpus)
{
    std::map<std::string, std::reference_wrapper<const json>> allCpuSections;
    for (const auto& cpu : cpus)
    {
        auto cpuSection = getCpuSection(cpu, processorsSection);
        if (!cpuSection)
        {
            continue;
        }

        allCpuSections.insert(std::pair<std::string, std::reference_wrapper<const json>>(cpu, *cpuSection));
    }
    return allCpuSections;
}

std::map<std::string, std::map<std::string, std::reference_wrapper<const json>>>
    getAllDieSections(const json& processorsSection, std::map<std::string, std::vector<std::string>> diesMap)
{
    std::map<std::string, std::map<std::string, std::reference_wrapper<const json>>> allDiesSections;
    for (const auto& [cpu, cpuDies] : diesMap)
    {
        auto cpuSection = getCpuSection(cpu, processorsSection);
        if (!cpuSection)
        {
            continue;
        }

        for (const auto& die : cpuDies)
        {
            auto dieSection = getDieSection(die, *cpuSection);
            if (!dieSection)
            {
                throw std::out_of_range("Die section not found in Cpu section.");
            }
            allDiesSections[cpu].insert(std::pair<std::string, std::reference_wrapper<const json>>(die, *dieSection));
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

const std::map<std::string, std::map<std::string, std::reference_wrapper<const json>>> prepareJsonGnr(const json& input)
{
    std::vector<std::string> cpus = findCpus(input);
    std::map<std::string, std::vector<std::string>> diesMap = findDies(input, cpus);
    std::optional processorsSection = getProcessorsSection(input);
    auto dieSections = getAllDieSections(*processorsSection, diesMap);
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
                return false;
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
    uint32_t step;

  public:
    NumbersRange(std::vector<std::variant<int, std::tuple<uint32_t, uint32_t, uint32_t>>> numbersIn) : numbers(numbersIn)
    {
    }

    std::vector<uint32_t> get()
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

struct AerRegisterSets
{

    std::vector<RegisterSet> registers;

    AerRegisterSets(std::vector<RegisterSet> s) : registers(s)
    {
    }

    std::vector<std::string> getRegisters()
    {
        std::stringstream ss;
        std::vector<std::string> ret;

        for (auto& reg : registers)
        {
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
                            ret.push_back(ss.str());
                        }
                    }
                }
            }
        }

        return ret;
    }
};