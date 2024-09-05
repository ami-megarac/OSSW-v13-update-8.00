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
#include "cpu.hpp"
#include "mca_defs.hpp"
#include "tor_defs_cpx.hpp"
#include "tor_whitley.hpp"
#include "utils.hpp"
#include "summary_field.hpp"

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

using json = nlohmann::json;

class CpxCpu final : public WhitleyCpu, public CpuGeneric
{
  public:
    // indexes for ierr and mcerr tsc information
    TscVariablesNames tscVariables = {
        "B01_D30_F4_0xF0", "B01_D30_F4_0xF4", "B01_D30_F4_0xF8", "B01_D30_F4_0xFC", "not implemented", "not implemented",
    };
    // indexes for IERR, MCERR and MCERR source
    static constexpr const char* ierr_varname = "B00_D08_F0_0xA4";
    static constexpr const char* mcerr_varname = "B00_D08_F0_0xA8";
    static constexpr const char* mca_err_src_varname = "mca_err_src_log";

    // masks are applied, so only meaningful bits are decoded
    static const uint32_t uncAerMask = 0x3FF030;
    static const uint32_t corAerMask = 0x31C1;

    const BdfRegisterSets UncAerRegisters{{
        {.B{{}}, .D{{std::make_tuple(1, 3, 1)}}, .F{{}}, .offset{{0x14C}}},
    }};

    const BdfRegisterSets CorAerRegisters{{
        {.B{{}}, .D{{std::make_tuple(1, 3, 1)}}, .F{{}}, .offset{{0x158}}},
    }};

    CpxCpu(std::string cpuName, std::string cpuId): CpuGeneric(cpuName, cpuId){}

    [[nodiscard]] TSCPointer getTscData(const json& input)
    {
        auto cpuSections = prepareJsonGeneric(input);
        return getTscDataForProcessorType(cpuSections, tscVariables);
    }

    virtual AerPointer analyzeAer(const json& input, BdfRegisterSets aerRegisters, uint32_t mask)
    {
        AerGenericPointer output = std::make_shared<AerGeneric>();
        auto cpuSections = prepareJsonGeneric(input);

        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            uint32_t socketId;
            if (!str2uint(cpu.substr(3), socketId, decimal))
            {
                continue;
            }

            std::map<std::string, uint32_t> registers = parseAer(cpuSection, aerRegisters);
            std::vector<AerData> allErrors;

            for (const auto& [addr, reg] : registers)
            {
                AerData aer;
                aer.error_status = reg & mask;
                if (aer.error_status != 0)
                {
                    aer.address = addr;
                    allErrors.push_back(aer);
                }
            }

            output->insert(socketId, allErrors);
        }

        return output;
    }

    [[nodiscard]] virtual MCAPointer analyzeMca(const json& input)
    {
        MCAGenericPointer output = std::make_shared<MCAGeneric>();
        auto cpuSections = prepareJsonGeneric(input);
        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            uint32_t socketId;
            if (!str2uint(cpu.substr(3), socketId, decimal))
            {
                continue;
            }

            std::vector<MCAData> uncoreMcas = parseUncoreMcas(cpuSection);
            std::vector<MCAData> coreMcas = parseCoreMcas(cpuSection);
            std::vector<MCAData> allMcas;

            allMcas.reserve(uncoreMcas.size() + coreMcas.size());
            allMcas.insert(allMcas.begin(), uncoreMcas.begin(), uncoreMcas.end());
            allMcas.insert(allMcas.begin(), coreMcas.begin(), coreMcas.end());
            output->insert(socketId, allMcas);
        }
        return output;
    }

    [[nodiscard]] std::optional<TORDataCpxSkx> parseTorData(const json& index)
    {
        if (index.find("subindex0") == index.cend())
        {
            return {};
        }

        if (checkInproperValue(index["subindex0"]))
        {
            return {};
        }

        TORDataCpxSkx tor;
        if (!str2uint(index["subindex0"], tor.subindex0) || !str2uint(index["subindex1"], tor.subindex1) ||
            !str2uint(index["subindex2"], tor.subindex2))
        {
            return {};
        }

        if (!tor.valid)
        {
            return {};
        }
        return tor;
    }

    [[nodiscard]] std::vector<std::shared_ptr<TORDataGen>> getTorsData(const json& input)
    {
        std::vector<std::shared_ptr<TORDataGen>> torsData;
        if (input.find("TOR") == input.cend())
        {
            return torsData;
        }

        for (const auto& [chaItemKey, chaItemValue] : input["TOR"].items())
        {
            if (!startsWith(chaItemKey, "cha"))
            {
                continue;
            }

            for (const auto& [indexDataKey, indexDataValue] : chaItemValue.items())
            {
                std::optional<TORDataCpxSkx> tor = parseTorData(indexDataValue);
                if (!tor)
                {
                    continue;
                }

                if (str2uint(chaItemKey.substr(3), tor->cha, decimal) && str2uint(indexDataKey.substr(5), tor->idx, decimal))
                {
                    torsData.push_back(std::make_shared<TORDataCpxSkx>(*tor));
                }
            }
        }
        return torsData;
    }

    TorPointer analyzeTor(const json& input)
    {
        TorGenericPointer output = std::make_shared<TorGeneric>();
        auto cpuSections = prepareJsonGeneric(input);
        for (auto const& [cpu, cpuSection] : cpuSections)
        {
            auto tors = getTorsData(cpuSection);
            uint32_t socketId;
            if (!str2uint(cpu.substr(3), socketId, decimal))
            {
                continue;
            }

            uint32_t ierrValue = getJsonUintValue(0, cpuSection, uncoreSection, ierr_varname);
            uint32_t mcerrValue = getJsonUintValue(0, cpuSection, uncoreSection, mcerr_varname);
            uint32_t mcerrErrSrcValue = getJsonUintValue(0, input, "METADATA", cpu, mca_err_src_varname);

            SocketCtx ctx{.ierr = std::make_shared<IerrLoggingRegGeneric>(ierrValue),
                          .mcerr = std::make_shared<McerrLoggingRegGeneric>(mcerrValue),
                          .rMcerrErr = std::make_shared<RMcerrLoggingRegGeneric>(0),
                          .ncEventsCrSmiSrcLog = std::make_shared<NcEventsCrSmiSrcLogGeneric>(),
                          .mcerrErrValue = mcerrErrSrcValue};

            output->insertSocketCtx(socketId, ctx);
            output->insert(socketId, tors);
        }
        return output;
    }

    Summary prepareSummary(const json& inputJson)
    {
        Summary summary(this->cpuName, this->cpuId);    

        summary.PORT_ID = CPX_PORT_ID;
        summary.LLCS = CPX_LLCS;
        summary.FirstErrorCha = CpxfirstErrorCha;
        summary.FirstError = CpxfirstError;
        summary.OpCodeDecode = CpxOpCodeDecode;

        summary.memoryMap = this->getMemoryMap(inputJson);
        summary.mca = analyzeMca(inputJson);
        summary.corAer = analyzeAer(inputJson, CorAerRegisters, corAerMask);
        summary.uncAer = analyzeAer(inputJson, UncAerRegisters, uncAerMask);
        summary.tsc = this->getTscData(inputJson);
        summary.tor = analyzeTor(inputJson);
        return summary;
    }
};
