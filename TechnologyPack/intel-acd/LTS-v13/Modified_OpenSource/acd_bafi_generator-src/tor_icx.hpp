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
#include "tor_defs_icx.hpp"
#include "tor_whitley.hpp"
#include "utils.hpp"
#include "summary_field.hpp"
#include "summary.hpp"

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

class IcxCpu final : public WhitleyCpu, public CpuGeneric
{
  public:
    // indexes for ierr and mcerr tsc information
    TscVariablesNames tscVariables = {
        "B31_D30_F4_0xF0", "B31_D30_F4_0xF4", "B31_D30_F4_0xF8", "B31_D30_F4_0xFC", "not implemented", "not implemented",
    };
    // index for PACKAGE_THERM_STATUS
    static constexpr const char* package_therm_status_varname = "RDIAMSR_0x1B1";
    // indexes for IERR, MCERR and MCERR source
    static constexpr const char* ierr_varname = "B30_D00_F0_0xA4";
    static constexpr const char* mcerr_varname = "B30_D00_F0_0xA8";
    static constexpr const char* ncevents_cr_smisrclog_varname = "B30_D00_F0_0x110";
    static constexpr const char* mca_err_src_varname = "mca_err_src_log";
    // bigcore MCAs indexes
    static constexpr const char* bigcore_mc0 = "ifu_cr_mc0";
    static constexpr const char* bigcore_mc1 = "dcu_cr_mc1";
    static constexpr const char* bigcore_mc2 = "dtlb_cr_mc2";
    static constexpr const char* bigcore_mc3 = "ml2_cr_mc3";

    static const uint32_t uncAerMask = 0x7FFF030;
    static const uint32_t corAerMask = 0xF1C1;
    // index and bit mask of uncorrectable AER that requires different decoding
    // rules
    static constexpr const char* unc_spec_err_index = "B00_D03_F0_0x14C";
    static const uint32_t unc_spec_err_mask = 0x47FF030;
    // index and bit mask of correctable AER that requires different decoding
    // rule
    static constexpr const char* cor_spec_err_index = "B00_D03_F0_0x158";
    static const uint32_t cor_spec_err_mask = 0x31C1;

    const BdfRegisterSets UncAerRegisters{{
        {.B{{std::make_tuple(0, 29, 1)}}, .D{{}}, .F{{}}, .offset{{0x104}}},
        {.B{{0}}, .D{{3}}, .F{{0}}, .offset{{0x14C}}},
    }};

    const BdfRegisterSets CorAerRegisters{{
        {.B{{std::make_tuple(0, 29, 1)}}, .D{{}}, .F{{}}, .offset{{0x110}}},
        {.B{{0}}, .D{{3}}, .F{{0}}, .offset{{0x158}}},
    }};

    std::string bigcore_mcas[4] = {bigcore_mc0, bigcore_mc1, bigcore_mc2, bigcore_mc3};

    IcxCpu(std::string cpuName, std::string cpuId): CpuGeneric(cpuName, cpuId){}

    [[nodiscard]] virtual TSCPointer getTscData(const json& input)
    {
        auto cpuSections = prepareJsonGeneric(input);
        return getTscDataForProcessorType(cpuSections, tscVariables);
    }

    [[nodiscard]] virtual ThermPointer getThermData(const json& input)
    {
        auto cpuSections = prepareJsonGeneric(input);
        return getThermDataForProcessorType(cpuSections, package_therm_status_varname);
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
                if (addr == unc_spec_err_index)
                {
                    aer.error_status = reg & unc_spec_err_mask;
                }
                else if (addr == cor_spec_err_index)
                {
                    aer.error_status = reg & cor_spec_err_mask;
                }
                else
                {
                    aer.error_status = reg & mask;
                }
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

            std::vector<MCAData> bigCoreMcas = parseBigcoreMcas(cpuSection);
            std::vector<MCAData> uncoreMcas = parseUncoreMcas(cpuSection);
            std::vector<MCAData> coreMcas = parseCoreMcas(cpuSection);
            std::vector<MCAData> allMcas;
            allMcas.reserve(bigCoreMcas.size() + uncoreMcas.size() + coreMcas.size());
            allMcas.insert(allMcas.begin(), bigCoreMcas.begin(), bigCoreMcas.end());
            allMcas.insert(allMcas.begin(), uncoreMcas.begin(), uncoreMcas.end());
            allMcas.insert(allMcas.begin(), coreMcas.begin(), coreMcas.end());
            output->insert(socketId, allMcas);
        }
        return output;
    }

    [[nodiscard]] std::tuple<uint32_t, uint32_t, uint32_t> divideTordumps(const json& inputData)
    {
        uint32_t tordumpParsed0, tordumpParsed1, tordumpParsed2;
        std::string input = inputData;
        while (input.length() < 26)
        {
            input.insert(2, "0");
        }

        std::string tordump0 = input.substr(input.length() - 8, 8);
        std::string tordump1 = input.substr(input.length() - 16, 8);
        std::string tordump2 = input.substr(input.length() - 24, 8);
        if (!str2uint(tordump0, tordumpParsed0) || !str2uint(tordump1, tordumpParsed1) || !str2uint(tordump2, tordumpParsed2))
        {
            return std::make_tuple(0, 0, 0);
        }
        return std::make_tuple(tordumpParsed0, tordumpParsed1, tordumpParsed2);
    }

    [[nodiscard]] std::optional<TORDataIcx> parseTorData(const json& index)
    {
        if (index.find("subindex0") == index.cend())
        {
            return {};
        }

        if (checkInproperValue(index["subindex0"]))
        {
            return {};
        }

        TORDataIcx tor;
        std::tie(tor.tordump0_subindex0, tor.tordump1_subindex0, tor.tordump2_subindex0) = divideTordumps(index["subindex0"]);
        if (!tor.valid)
        {
            return {};
        }

        std::tie(tor.tordump0_subindex1, tor.tordump1_subindex1, tor.tordump2_subindex1) = divideTordumps(index["subindex1"]);
        std::tie(tor.tordump0_subindex2, tor.tordump1_subindex2, tor.tordump2_subindex2) = divideTordumps(index["subindex2"]);
        std::tie(tor.tordump0_subindex3, tor.tordump1_subindex3, tor.tordump2_subindex3) = divideTordumps(index["subindex3"]);
        std::tie(tor.tordump0_subindex4, tor.tordump1_subindex4, tor.tordump2_subindex4) = divideTordumps(index["subindex4"]);
        std::tie(tor.tordump0_subindex7, tor.tordump1_subindex7, tor.tordump2_subindex7) = divideTordumps(index["subindex7"]);
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
                std::optional<TORDataIcx> tor = parseTorData(indexDataValue);
                if (!tor)
                {
                    continue;
                }

                if (str2uint(chaItemKey.substr(3), tor->cha, decimal) && str2uint(indexDataKey.substr(5), tor->idx, decimal))
                {
                    torsData.push_back(std::make_shared<TORDataIcx>(*tor));
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
            uint32_t smisrclogValue = getJsonUintValue(0, cpuSection, uncoreSection, ncevents_cr_smisrclog_varname);
            uint32_t mcerrErrSrcValue = getJsonUintValue(0, input, "METADATA", cpu, mca_err_src_varname);

            SocketCtx ctx{.ierr = std::make_shared<IerrLoggingRegGeneric>(ierrValue),
                          .mcerr = std::make_shared<McerrLoggingRegGeneric>(mcerrValue),
                          .rMcerrErr = std::make_shared<RMcerrLoggingRegGeneric>(0),
                          .ncEventsCrSmiSrcLog = std::make_shared<NcEventsCrSmiSrcLogIcx>(smisrclogValue),
                          .mcerrErrValue = mcerrErrSrcValue};

            output->insertSocketCtx(socketId, ctx);
            output->insert(socketId, tors);
        }
        return output;
    }
    
    Summary prepareSummary(const json& inputJson)
    {
        Summary summary(this->cpuName, this->cpuId);

        summary.PORT_ID = ICX_PORT_ID;
        summary.LLCS = ICX_LLCS;
        summary.FirstErrorCha = IcxfirstErrorCha;
        summary.FirstError = IcxfirstError;
        summary.OpCodeDecode = IcxOpCodeDecode;

        summary.memoryMap = this->getMemoryMap(inputJson);
        summary.mca = analyzeMca(inputJson);
        summary.corAer = analyzeAer(inputJson, CorAerRegisters, corAerMask);
        summary.uncAer = analyzeAer(inputJson, UncAerRegisters, uncAerMask);
        summary.tsc = this->getTscData(inputJson);
        summary.thermStatus = getThermData(inputJson);
        summary.tor = analyzeTor(inputJson);
        return summary;
    }
};