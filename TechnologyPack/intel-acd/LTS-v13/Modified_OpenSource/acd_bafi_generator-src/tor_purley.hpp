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
#include "tor_defs_skx.hpp"
#include "utils.hpp"
#include "summary_field.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using json = nlohmann::json;

[[nodiscard]] std::optional<std::reference_wrapper<const json>> getSocketSection(std::string socket, const json& input)
{
    const auto& socketSection = input.find(socket);
    if (socketSection != input.cend())
    {
        return *socketSection;
    }
    return {};
}

[[nodiscard]] std::map<std::string, std::reference_wrapper<const json>> getAllSocketSections(const json& dumpSection,
                                                                                             std::vector<std::string> sockets)
{
    std::map<std::string, std::reference_wrapper<const json>> allSocketsSections;
    for (const auto& socket : sockets)
    {
        if (!dumpSection)
        {
            continue;
        }

        allSocketsSections.insert(std::pair<std::string, std::reference_wrapper<const json>>(socket, dumpSection));
    }
    return allSocketsSections;
}

[[nodiscard]] std::vector<std::string> findSocketsPurley(const json& input)
{
    // Socket info can be stored in various entries: sys_info, uncore_MCA,
    // uncore_status_regs or TOR_dump
    std::vector<std::string> cpus;
    std::array<std::string, 4> crashdumpKeys = {"sys_info", "uncore_MCA", "uncore_status_regs", "TOR_dump"};

    for (const auto& crashdumpKey : crashdumpKeys)
    {
        const auto& crashdumpEntry = input.find(crashdumpKey);
        if (crashdumpEntry != input.cend())
        {
            for (const auto& crashdumpEntryItem : crashdumpEntry.value().items())
            {
                if (startsWith(crashdumpEntryItem.key(), "socket") &&
                    std::find(cpus.begin(), cpus.end(), crashdumpEntryItem.key()) == cpus.end())
                {
                    cpus.push_back(crashdumpEntryItem.key());
                }
            }
        }
    }

    return cpus;
}

class PurleyCpu
{
  public:
    [[nodiscard]] std::map<std::string, std::array<uint64_t, 2>> getMemoryMap(const json& input)
    {
        // map is the same for all CPUs, so if once created other CPUs can be skipped
        std::map<std::string, std::array<uint64_t, 2>> memoryMap;
        if (input.find("addr_map") == input.cend())
        {
            return memoryMap;
        }

        if (!input["addr_map"].contains("sys_mem"))
        {
            return memoryMap;
        }

        if (!input["addr_map"]["sys_mem"].is_object())
        {
            return memoryMap;
        }

        for (auto const& [entry, entryVal] : input["addr_map"]["sys_mem"].items())
        {
            if (entry == "_record_enable" && entryVal == false)
            {
                break;
            }

            if (entry == "_version")
            {
                continue;
            }

            auto shortName = entryVal["region"];
            uint64_t uintValueBase;
            uint64_t uintValueLimit;
            if (entryVal.contains("range"))
            {
                if (checkInproperValue(entryVal["range"][0]))
                {
                    continue;
                }

                if (checkInproperValue(entryVal["range"][1]))
                {
                    continue;
                }

                if (!str2uint(entryVal["range"][0], uintValueBase))
                {
                    continue;
                }

                if (!str2uint(entryVal["range"][1], uintValueLimit))
                {
                    continue;
                }

                memoryMap[shortName][0] = uintValueBase;
                memoryMap[shortName][1] = uintValueLimit;
                continue;
            }
            for (auto const& [name, value] : entryVal.items())
            {
                if (name.find("_BASE") != std::string::npos)
                {
                    if (!str2uint(value, uintValueBase))
                    {
                        continue;
                    }

                    shortName = name.substr(0, name.find("_BASE"));
                    memoryMap[shortName][0] = uintValueBase;
                }
                if (name.find("_LIMIT") != std::string::npos)
                {
                    if (!str2uint(value, uintValueLimit))
                    {
                        continue;
                    }

                    shortName = name.substr(0, name.find("_LIMIT"));
                    memoryMap[shortName][1] = uintValueLimit;
                }
            }
        }
        return memoryMap;
    }

    std::optional<std::string> getUncoreData(const json& input, std::string socket, std::string varName)
    {
        if (input.contains("uncore_status_regs"))
        {
            return getJsonStringValue(input["uncore_status_regs"], socket, varName);
        }

        if (input.contains("crashdump"))
        {
            return getJsonStringValue(input["crashdump"], socket, "uncore_regs", varName);
        }

        return std::nullopt;
    }

    std::optional<MCAData> getCboData(const json& mcData, uint32_t coreId, uint32_t cboId)
    {
        if (mcData.is_string())
        {
            return {};
        }

        MCAData mc;
        mc.core = coreId;
        std::string ctl = std::string("cbo") + std::to_string(cboId) + std::string("_mc_ctl");
        std::string status = std::string("cbo") + std::to_string(cboId) + std::string("_mc_status");
        std::string addr = std::string("cbo") + std::to_string(cboId) + std::string("_mc_addr");
        std::string misc = std::string("cbo") + std::to_string(cboId) + std::string("_mc_misc");

        if (mcData.contains("status"))
        {
            if (checkInproperValue(mcData["status"]))
            {
                return {};
            }
        }

        if (!parseUintField(mcData, status, mc.mc_status) || !mc.valid)
        {
            return std::nullopt;
        }

        mc.type = McaTypeFlag::CBO;
        mc.bank = cboId;

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

    std::optional<MCAData> parseMca(const json& mcSection, const json& mcData, uint32_t coreId)
    {
        if (mcData.is_string())
        {
            return {};
        }

        MCAData mc;
        mc.core = coreId;
        std::string ctl = "CTL";
        std::string status = "STATUS";
        std::string addr = "ADDR";
        std::string misc = "MISC";

        if (!parseUintField(mcData, status, mc.mc_status) || !mc.valid)
        {
            return std::nullopt;
        }

        if (startsWith(mcSection, "MC"))
        {
            if (!str2uint(std::string(mcSection).substr(2), mc.bank, 10))
            {
                return {};
            }
        }
        else
        {
            return {};
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

    std::vector<MCAData> parseCoreMcas(const json& input, std::string socket)
    {
        std::vector<MCAData> allCoreMcas;
        if (!input.contains("core_MCA"))
        {
            return allCoreMcas;
        }

        if (!input["core_MCA"].contains(socket))
        {
            return allCoreMcas;
        }

        for (auto const& [core, coreData] : input["core_MCA"][socket].items())
        {
            if (!startsWith(core, "core"))
            {
                continue;
            }

            uint32_t coreId;
            if (!str2uint(core.substr(4), coreId, 10))
            {
                continue;
            }

            for (auto const& [mcSection, mcData] : coreData.items())
            {
                auto coreMca = parseMca(mcSection, mcData, coreId);
                if (coreMca)
                {
                    allCoreMcas.push_back(*coreMca);
                }
            }
        }
        return allCoreMcas;
    }

    std::vector<MCAData> parseUncoreMcas(const json& input, std::string socket)
    {
        std::vector<MCAData> output;
        if (!input.contains("uncore_MCA"))
        {
            return output;
        }

        if (!input["uncore_MCA"].contains(socket))
        {
            return output;
        }

        for (auto const& [mcSection, mcData] : input["uncore_MCA"][socket].items())
        {
            auto uncoreMc = parseMca(mcSection, mcData, 0);
            if (uncoreMc)
            {
                output.push_back(*uncoreMc);
            }
        }

        if (input.contains("uncore_status_regs"))
        {
            if (input["uncore_status_regs"].contains(socket))
            {
                for (uint32_t i = 0; i < 28; i++)
                {
                    auto cboData = getCboData(input["uncore_status_regs"][socket], 0, i);
                    if (cboData)
                    {
                        output.push_back(*cboData);
                    }
                }
            }
        }
        else if (input.contains("crashdump"))
        {
            if (input["crashdump"].contains(socket))
            {
                if (input["crashdump"][socket].contains("uncore_status_regs"))
                {
                    for (uint32_t i = 0; i < 28; i++)
                    {
                        auto cboData = getCboData(input["crashdump"][socket]["uncore_status_regs"], 0, i);
                        if (cboData)
                        {
                            output.push_back(*cboData);
                        }
                    }
                }
            }
        }
        return output;
    }
};
