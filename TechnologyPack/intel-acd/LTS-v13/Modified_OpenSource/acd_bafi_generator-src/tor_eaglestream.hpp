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
#include "tor_defs_spr.hpp"
#include "utils.hpp"
#include "summary_field.hpp"
#include "summary.hpp"
#include "tsc.hpp"


#include <algorithm>
#include <array>
#include <functional>
#include <locale>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class EaglestreamCpu
{
    std::vector<std::string> bigCoreMcaNames{"ifu_cr_mc0", "dcu_cr_mc1", "dtlb_cr_mc2", "ml2_cr_mc3"};

    std::vector<McBankProperties> McaBankPropertiesDecoding = {
        {"mc", McaTypeFlag::NO_TYPE},
        {"cbo", McaTypeFlag::CBO},
        {"m2mem", McaTypeFlag::M2M},
        {"upi", McaTypeFlag::UPI},
        {"mdf", McaTypeFlag::MDF},
        {"ifu_cr_mc", McaTypeFlag::NO_TYPE},
        {"dcu_cr_mc", McaTypeFlag::NO_TYPE},
        {"dtlb_cr_mc", McaTypeFlag::NO_TYPE},
        {"ml2_cr_mc", McaTypeFlag::NO_TYPE},
        {McaTypeFlag::HBM_M2M,
         [](std::string mcLower) -> std::optional<uint32_t> {
             uint32_t bankNumber;
             if (!str2uint(mcLower.substr(5, 10).substr(5), bankNumber, 10))
                 return std::nullopt;

             return bankNumber;
         },
         [](std::string mcLower) { return startsWith(mcLower, "hbm") && mcLower.find("m2mem") != std::string::npos; }},
        {McaTypeFlag::HBM_IMC,
         [](std::string mcLower) -> std::optional<uint32_t> {
             uint32_t bankNumber;
             if (!str2uint(mcLower.substr(5, 4).substr(3), bankNumber, 10))
                 return std::nullopt;

             return bankNumber == 0 ? 30 : 31;
         },
         [](std::string mcLower) { return startsWith(mcLower, "hbm") && mcLower.find("imc") != std::string::npos; }}
    };

  public:
    static constexpr const char* uncoreSection = "uncore";

    [[nodiscard]] std::map<std::string, std::array<uint64_t, 2>> getMemoryMap(const json& input)
    {
        std::map<std::string, std::array<uint64_t, 2>> memoryMap;
        auto cpuSections = prepareJsonGeneric(input);
        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            // For now only MMCFG base and limit registers are useful in BAFI
            std::string mmcfgBase = "B00_D00_F0_0x90";
            std::string mmcfgLimit = "B00_D00_F0_0x98";

            auto mmcfgBaseFound = getJsonUintValue<uint64_t>(cpuSection.get(), uncoreSection, mmcfgBase);
            auto mmcfgLimitFound = getJsonUintValue<uint64_t>(cpuSection.get(), uncoreSection, mmcfgLimit);
            if (!(mmcfgBaseFound && mmcfgLimitFound))
            {
                continue;
            }

            memoryMap["MMCFG"][0] = *mmcfgBaseFound;
            memoryMap["MMCFG"][1] = *mmcfgLimitFound;

            // map is the same for all CPUs, so if once created other CPUs can be skipped
            break;
        }
        return memoryMap;
    }
  
    std::optional<MCAData> parseMca(const std::string& mcSection, const json& mcData, uint32_t coreId, uint32_t threadId)
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

    std::vector<MCAData> parseCoreMcas(std::reference_wrapper<const json> input)
    {
        std::vector<MCAData> allCoreMcas;
        if (!input.get().contains("MCA"))
        {
            return allCoreMcas;
        }

        for (auto const& [core, threads] : input.get()["MCA"].items())
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

    std::vector<MCAData> parseUncoreMcas(std::reference_wrapper<const json> input)
    {
        std::vector<MCAData> output;
        if (!input.get().contains("MCA"))
        {
            return output;
        }

        if (!input.get()["MCA"].contains("uncore"))
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

    /* Reason for False Positive - That appears Coverity considers 128 bytes to be the rule-of-thumb threshold for pass-by-value.*/
    /* coverity[pass_by_value : FALSE] */
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

    [[nodiscard]] ThermPointer
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
