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
#include "aer.hpp"
#include "mca_defs.hpp"
#include "tor_defs_cpx.hpp"
#include "tor_defs_icx.hpp"
#include "tor_defs_skx.hpp"
#include "tor_whitley_defs.hpp"
#include "utils.hpp"
#include "summary_field.hpp"
#include "tsc.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

[[nodiscard]] std::optional<std::reference_wrapper<const json>> getProperRootNode(const json& input)
{
    if (input.find("METADATA") != input.cend())
    {
        return input;
    }
    else if (input.find("sys_info") != input.cend())
    {
        return input;
    }
    else if (input.find("crash_data") != input.cend())
    {
        return input["crash_data"];
    }
    else if (input.find("crashdump") != input.cend())
    {
        return input["crashdump"]["cpu_crashdump"]["crash_data"];
    }
    else if (input.find("Oem") != input.cend())
    {
        return input["Oem"]["Intel"]["crash_data"];
    }
    return std::nullopt;
}

[[nodiscard]] std::string getCpuType(const json& input)
{
    Version decodedVersion;
    if (input.contains("metadata"))
    {
        if (!input["metadata"].contains("_version"))
        {
            return "SKX";
        }
    }

    if (!input["METADATA"].contains("_version") || !str2uint(input["METADATA"]["_version"], decodedVersion.version))
    {
        return {};
    }

    auto cpuId = getDecoded(decodedCpuId, static_cast<uint16_t>(decodedVersion.cpu_id));

    if (!cpuId)
    {
        return {};
    }
    return *cpuId;
}

std::string getTimestamp(const json& input)
{
    if (input.contains("metadata"))
    {
        if (input["metadata"].contains("timestamp"))
        {
            return input["metadata"]["timestamp"];
        }
    }
    else if (input.contains("METADATA"))
    {
        if (input["METADATA"].contains("timestamp"))
        {
            return input["METADATA"]["timestamp"];
        }
    }
    return "";
}

std::string getFullVersionName(const json& input)
{
    Version decodedVersion;
    std::string metadataKey = "";
    std::string versionKey = "";

    if (input.contains("metadata"))
    {
        metadataKey = "metadata";
        versionKey = "crashdump_version";
    }
    else if (input.contains("METADATA"))
    {
        metadataKey = "METADATA";
        versionKey = "_version";
    }

    if (!input[metadataKey].contains(versionKey) || !str2uint(input[metadataKey][versionKey], decodedVersion.version))
    {
        return "";
    }

    auto cpuId = getDecoded(decodedCpuFullName, static_cast<uint16_t>(decodedVersion.cpu_id));
    if (!cpuId)
    {
        return "";
    }

    return *cpuId;
}

[[nodiscard]] std::optional<std::string> getCpuId(const json& input)
{
    if (input.contains("metadata"))
    {
        if (input["metadata"].contains("cpu0"))
        {
            if (input["metadata"]["cpu0"].contains("cpuid"))
                return input["metadata"]["cpu0"]["cpuid"].get_ref<const std::string&>();
        }
    }
    else
    {
        if (input["METADATA"].contains("cpu0"))
        {
            if (input["METADATA"]["cpu0"].contains("cpuid"))
                return input["METADATA"]["cpu0"]["cpuid"].get_ref<const std::string&>();
        }
    }
    return {};
}

class WhitleyCpu
{
  public:
    static constexpr const char* uncoreSection = "uncore";
  
    std::vector<McBankProperties> McaBankPropertiesDecoding = {
        {"mc", McaTypeFlag::NO_TYPE},         {"cbo", McaTypeFlag::CBO},
        {"ifu_cr_mc", McaTypeFlag::NO_TYPE},  {"dcu_cr_mc", McaTypeFlag::NO_TYPE},
        {"dtlb_cr_mc", McaTypeFlag::NO_TYPE}, {"ml2_cr_mc", McaTypeFlag::NO_TYPE}};

    std::vector<std::string> bigCoreMcaNames{"ifu_cr_mc0", "dcu_cr_mc1", "dtlb_cr_mc2", "ml2_cr_mc3"};

    [[nodiscard]] std::map<std::string, std::array<uint64_t, 2>> getMemoryMap(const json& input)
    {
        // map is the same for all CPUs, so if once created other CPUs can be
        // skipped
        bool mapCreated = false;
        std::map<std::string, std::array<uint64_t, 2>> memoryMap;
        auto cpuSections = prepareJsonGeneric(input);
        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            if (mapCreated)
            {
                break;
            }

            if (cpuSection.get().find("address_map") == cpuSection.get().cend())
            {
                continue;
            }

            for (auto const& [name, value] : cpuSection.get()["address_map"].items())
            {
                if (name == "_record_enable" && value == false)
                {
                    break;
                }

                if (name == "_version")
                {
                    continue;
                }

                if (checkInproperValue(value))
                {
                    continue;
                }

                uint64_t uintValue;
                if (!str2uint(value, uintValue))
                {
                    continue;
                }

                if (name.find("_BASE") != std::string::npos)
                {
                    auto shortName = name.substr(0, name.find("_BASE"));
                    memoryMap[shortName][0] = uintValue;
                }
                else if (name.find("_LIMIT") != std::string::npos)
                {
                    auto shortName = name.substr(0, name.find("_LIMIT"));
                    memoryMap[shortName][1] = uintValue;
                }
            }

            mapCreated = true;
        }
        return memoryMap;
    }

    std::optional<MCAData> parseMca(const json& mcSection, const json& mcData, uint32_t coreId, uint32_t threadId)
    {
        if (mcData.is_string())
        {
            return std::nullopt;
        }

        MCAData mc;
        mc.core = coreId;
        mc.thread = threadId;
        std::string mcLower = mcSection;
        std::transform(mcLower.begin(), mcLower.end(), mcLower.begin(), ::tolower);
        std::string status = mcLower + "_status";
        std::string ctl = mcLower + "_ctl";
        std::string addr = mcLower + "_addr";
        std::string misc = mcLower + "_misc";

        if (!parseUintField(mcData, status, mc.mc_status) || !mc.valid)
        {
            return std::nullopt;
        }

        if (!SetMcaBanksProperties(McaBankPropertiesDecoding, mc, mcLower))
        {
            return std::nullopt;
        }

        if (!parseUintField<uint64_t>(mcData, ctl, mc.ctl, 0))
        {
            return std::nullopt;
        }

        if (!parseUintField<uint64_t>(mcData, addr, mc.address, 0))
        {
            return std::nullopt;
        }

        if (!parseUintField<uint64_t>(mcData, misc, mc.misc, 0))
        {
            return std::nullopt;
        }

        return mc;
    }

    [[nodiscard]] std::vector<MCAData> parseCoreMcas(std::reference_wrapper<const json> input)
    {
        std::vector<MCAData> allCoreMcas;
        if (input.get().find("MCA") == input.get().cend())
        {
            return allCoreMcas;
        }

        for (auto const& [core, threads] : input.get()["MCA"].items())
        {
            if (!startsWith(std::string(core), "core"))
            {
                continue;
            }

            uint32_t coreId;
            if (!str2uint(core.substr(4), coreId, decimal))
            {
                continue;
            }

            for (auto const& [thread, threadData] : threads.items())
            {
                if (!startsWith(thread, "thread"))
                {
                    continue;
                }

                uint32_t threadId;
                if (!str2uint(thread.substr(6), threadId, decimal))
                {
                    continue;
                }

                for (auto const& [mcSection, mcData] : threadData.items())
                {
                    auto coreMca = parseMca(mcSection, mcData, coreId, threadId);
                    if (coreMca)
                    {
                        allCoreMcas.push_back(*coreMca);
                    }
                }
            }
        }
        return allCoreMcas;
    }

    std::vector<MCAData> parseBigcoreMcas(std::reference_wrapper<const json> input)
    {
        std::vector<MCAData> allBigcoreMca;
        if (input.get().find("big_core") == input.get().cend())
        {
            return allBigcoreMca;
        }

        for (auto const& [core, threads] : input.get()["big_core"].items())
        {
            if (!startsWith(core, "core"))
            {
                continue;
            }

            uint32_t coreId;
            if (!str2uint(core.substr(4), coreId, decimal))
            {
                continue;
            }

            for (auto const& [thread, threadData] : threads.items())
            {
                if (!startsWith(thread, "thread") || threadData.is_string())
                {
                    continue;
                }

                uint32_t threadId;
                if (!str2uint(thread.substr(6), threadId, decimal))
                {
                    continue;
                }

                for (auto const& mcSection : bigCoreMcaNames)
                {
                    auto coreMca = parseMca(mcSection, threadData, coreId, threadId);
                    if (coreMca)
                    {
                        allBigcoreMca.push_back(*coreMca);
                    }
                }
            }
        }
        return allBigcoreMca;
    }

    [[nodiscard]] std::vector<MCAData> parseUncoreMcas(std::reference_wrapper<const json> input)
    {
        std::vector<MCAData> output;
        if (input.get().find("MCA") == input.get().cend())
        {
            return output;
        }

        if (input.get()["MCA"].find("uncore") == input.get()["MCA"].cend())
        {
            return output;
        }

        for (auto const& [mcSection, mcData] : input.get()["MCA"]["uncore"].items())
        {
            auto uncoreMc = parseMca(mcSection, mcData, 0, 0);
            if (uncoreMc)
            {
                output.push_back(*uncoreMc);
            }
        }
        return output;
    }

    [[nodiscard]] TSCPointer
        getTscDataForProcessorType(const std::map<std::string, std::reference_wrapper<const json>> cpuSections,
                                   TscVariablesNames tscVariablesNamesForProcessor)
    {

        TSCGenericPointer output = std::make_shared<TSCGeneric>();
        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            uint32_t socketId;
            TscData tsc;
            if (!str2uint(cpu.substr(3), socketId, decimal))
            {
                continue;
            }
            uint32_t defaultTscValue = 0;
            uint32_t pcu_first_ierr_tsc_lo_cfg =
                getJsonUintValue(defaultTscValue, cpuSection, uncoreSection, tscVariablesNamesForProcessor.pcu_first_ierr_tsc_lo_cfg_varname);
            uint32_t pcu_first_ierr_tsc_hi_cfg =
                getJsonUintValue(defaultTscValue, cpuSection, uncoreSection, tscVariablesNamesForProcessor.pcu_first_ierr_tsc_hi_cfg_varname);

            uint32_t pcu_first_mcerr_tsc_lo_cfg =
                getJsonUintValue(defaultTscValue, cpuSection, uncoreSection, tscVariablesNamesForProcessor.pcu_first_mcerr_tsc_lo_cfg_varname);
            uint32_t pcu_first_mcerr_tsc_hi_cfg =
                getJsonUintValue(defaultTscValue, cpuSection, uncoreSection, tscVariablesNamesForProcessor.pcu_first_mcerr_tsc_hi_cfg_varname);

            uint32_t pcu_first_rmca_tsc_lo_cfg =
                getJsonUintValue(defaultTscValue, cpuSection, uncoreSection, tscVariablesNamesForProcessor.pcu_first_rmca_tsc_lo_cfg_varname);
            uint32_t pcu_first_rmca_tsc_hi_cfg =
                getJsonUintValue(defaultTscValue, cpuSection, uncoreSection, tscVariablesNamesForProcessor.pcu_first_rmca_tsc_hi_cfg_varname);

            tsc.pcu_first_ierr_tsc_cfg =
                static_cast<uint64_t>(pcu_first_ierr_tsc_lo_cfg) | static_cast<uint64_t>(pcu_first_ierr_tsc_hi_cfg) << 32;
            tsc.pcu_first_mcerr_tsc_cfg =
                static_cast<uint64_t>(pcu_first_mcerr_tsc_lo_cfg) | static_cast<uint64_t>(pcu_first_mcerr_tsc_hi_cfg) << 32;
            tsc.pcu_first_rmca_tsc_cfg =
                static_cast<uint64_t>(pcu_first_rmca_tsc_lo_cfg) | static_cast<uint64_t>(pcu_first_rmca_tsc_hi_cfg) << 32;

            countTscCfg(tsc);

            output->insert(socketId, tsc);
        }

        return output;
    }

    ThermPointer
        getThermDataForProcessorType(const std::map<std::string, std::reference_wrapper<const json>> cpuSections,
                                     const char* thermStatusVariableNameForProcessor)
    {
        ThermGenericPointer thermStatus = std::make_shared<ThermGeneric>();
        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            uint32_t socketId;
            PackageThermStatus thermStatusSocket;
            if (!str2uint(cpu.substr(3), socketId, decimal))
            {
                continue;
            }

            auto thermStatusRaw = getJsonUintValue<uint32_t>(cpuSection, uncoreSection, thermStatusVariableNameForProcessor);
            if (!thermStatusRaw)
            {
                continue;
            }
            thermStatusSocket.package_therm_status = *thermStatusRaw;

            thermStatus->insert(socketId, thermStatusSocket);
        }
        return thermStatus;
    }
};
