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
#include "tor_defs_spr.hpp"
#include "tor_eaglestream.hpp"
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

class SprCpu final : public EaglestreamCpu, public CpuGeneric
{
  public:
    SprCpu(std::string cpuName, std::string cpuId): CpuGeneric(cpuName, cpuId){}
    // indexes for ierr and mcerr tsc information
    TscVariablesNames tscVariables = {
        "B31_D30_F4_0xf0", "B31_D30_F4_0xf4", "B31_D30_F4_0xf8", "B31_D30_F4_0xfc", "B31_D30_F4_0x88", "B31_D30_F4_0x8c",
    };
    // index for PACKAGE_THERM_STATUS
    static constexpr const char* package_therm_status_varname = "RDIAMSR_0x1b1";
    // indexes for IERR, MCERR, RMCA, SIDEBANDPORTID and MCERR source
    static constexpr const char* ierr_varname = "B30_D00_F0_0xa4";
    static constexpr const char* mcerr_varname = "B30_D00_F0_0xa8";
    static constexpr const char* rmca_varname = "B30_D00_F0_0x1e0";
    static constexpr const char* ncevents_cr_smisrclog_varname = "B30_D00_F0_0x110";
    static constexpr const char* mca_err_src_varname = "mca_err_src_log";
    // bigcore MCAs indexes
    static constexpr const char* bigcore_mc0 = "ifu_cr_mc0";
    static constexpr const char* bigcore_mc1 = "dcu_cr_mc1";
    static constexpr const char* bigcore_mc2 = "dtlb_cr_mc2";
    static constexpr const char* bigcore_mc3 = "ml2_cr_mc3";

    // masks are applied, so only meaningful bits are decoded
    static const uint32_t uncAerMask = 0x7FFF030;
    static const uint32_t corAerMask = 0xF1C1;

    const BdfRegisterSets UncAerRegisters{{
        {.B{{std::make_tuple(0, 5, 1)}}, .D{{0}}, .F{{4}}, .offset{{0x104}}},
        {.B{{std::make_tuple(0, 5, 1)}}, .D{{std::make_tuple(1, 8, 1)}}, .F{{0}}, .offset{{0x104}}},
        {.B{{8}}, .D{{3}}, .F{{0}}, .offset{{0x104, 0x170}}},
        {.B{{30}}, .D{{0}}, .F{{3}}, .offset{{std::make_tuple(0x284, 0x2a0, 4)}}},
    }};

    const BdfRegisterSets CorAerRegisters{{
        {.B{{std::make_tuple(0, 5, 1)}}, .D{{0}}, .F{{4}}, .offset{{0x110}}},
        {.B{{std::make_tuple(0, 5, 1)}}, .D{{std::make_tuple(1, 8, 1)}}, .F{{0}}, .offset{{0x110}}},
        {.B{{8}}, .D{{3}}, .F{{0}}, .offset{{0x110, 0x174}}},
    }};

    std::string bigcore_mcas[4] = {bigcore_mc0, bigcore_mc1, bigcore_mc2, bigcore_mc3};

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

    virtual MCAPointer analyzeMca(const json& input)
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

    [[nodiscard]] std::optional<TORDataSpr> parseTorData(const json& index)
    {
        if (index.find("subindex0") == index.cend())
        {
            return {};
        }

        if (checkInproperValue(index["subindex0"]))
        {
            return {};
        }

        TORDataSpr tor;
        std::tie(tor.tordump0_subindex0_spr, tor.tordump1_subindex0_spr, tor.tordump2_subindex0_spr) =
            divideTordumps(index["subindex0"]);
        if (!tor.valid)
        {
            return {};
        }

        std::tie(tor.tordump0_subindex1_spr, tor.tordump1_subindex1_spr, tor.tordump2_subindex1_spr) =
            divideTordumps(index["subindex1"]);
        std::tie(tor.tordump0_subindex2_spr, tor.tordump1_subindex2_spr, tor.tordump2_subindex2_spr) =
            divideTordumps(index["subindex2"]);
        std::tie(tor.tordump0_subindex3_spr, tor.tordump1_subindex3_spr, tor.tordump2_subindex3_spr) =
            divideTordumps(index["subindex3"]);
        std::tie(tor.tordump0_subindex4_spr, tor.tordump1_subindex4_spr, tor.tordump2_subindex4_spr) =
            divideTordumps(index["subindex4"]);
        std::tie(tor.tordump0_subindex5_spr, tor.tordump1_subindex5_spr, tor.tordump2_subindex5_spr) =
            divideTordumps(index["subindex5"]);
        std::tie(tor.tordump0_subindex6_spr, tor.tordump1_subindex6_spr, tor.tordump2_subindex6_spr) =
            divideTordumps(index["subindex6"]);
        std::tie(tor.tordump0_subindex7_spr, tor.tordump1_subindex7_spr, tor.tordump2_subindex7_spr) =
            divideTordumps(index["subindex7"]);

        if (this->cpuId == "0x806f0" || this->cpuId == "0x806f5")
        {
            return tor;
        }

        if (this->cpuId == "0x806f1")
        {
            tor.tordump0_subindex2_spr2 = tor.tordump0_subindex2_spr;
            tor.tordump0_subindex4_spr2 = tor.tordump0_subindex4_spr;
            tor.tordump0_subindex7_spr2 = tor.tordump0_subindex7_spr;
            tor.tordump0_subindex5_spr2 = tor.tordump0_subindex5_spr;
            tor.tordump1_subindex6_spr2 = tor.tordump1_subindex6_spr;
        }
        else
        {
            tor.tordump0_subindex1_spr3 = tor.tordump0_subindex1_spr;
            tor.tordump0_subindex2_spr3 = tor.tordump0_subindex2_spr;
            tor.tordump0_subindex4_spr3 = tor.tordump0_subindex4_spr;
            tor.tordump2_subindex0_spr3 = tor.tordump2_subindex0_spr;
            tor.tordump2_subindex1_spr3 = tor.tordump2_subindex1_spr;
            tor.tordump2_subindex2_spr3 = tor.tordump2_subindex2_spr;
            tor.tordump2_subindex3_spr3 = tor.tordump2_subindex3_spr;
            tor.tordump2_subindex4_spr3 = tor.tordump2_subindex4_spr;
            tor.tordump0_subindex7_spr2 = tor.tordump0_subindex7_spr;
            tor.tordump0_subindex5_spr3 = tor.tordump0_subindex5_spr;
            tor.tordump1_subindex6_spr3 = tor.tordump1_subindex6_spr;
            tor.tordump1_subindex7_spr3 = tor.tordump1_subindex7_spr;
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
                std::optional<TORDataSpr> tor = parseTorData(indexDataValue);
                if (!tor)
                {
                    continue;
                }

                if (str2uint(chaItemKey.substr(3), tor->cha, decimal) && str2uint(indexDataKey.substr(5), tor->idx, decimal))
                {
                    torsData.push_back(std::make_shared<TORDataSpr>(*tor));
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
            uint32_t rmcerrValue = getJsonUintValue(0, cpuSection, uncoreSection, rmca_varname);
            uint32_t smisrclogValue = getJsonUintValue(0, cpuSection, uncoreSection, ncevents_cr_smisrclog_varname);
            uint32_t mcerrErrSrcValue = getJsonUintValue(0, input, "METADATA", cpu, mca_err_src_varname);

            SocketCtx ctx{.ierr = std::make_shared<IerrLoggingRegSpr>(ierrValue),
                          .mcerr = std::make_shared<McerrLoggingRegSpr>(mcerrValue),
                          .rMcerrErr = std::make_shared<RMcerrLoggingRegGeneric>(rmcerrValue),
                          .ncEventsCrSmiSrcLog = std::make_shared<NcEventsCrSmiSrcLogSpr>(smisrclogValue),
                          .mcerrErrValue = mcerrErrSrcValue};

            output->insertSocketCtx(socketId, ctx);
            output->insert(socketId, tors);
        }
        return output;
    }

    Summary prepareSummary(const json& inputJson)
    {
        Summary summary(this->cpuName, this->cpuId);

        summary.PORT_ID = SPR_PORT_ID;
        summary.LLCS = SPR_LLCS;
        summary.FirstErrorCha = SprfirstErrorCha;
        summary.FirstError = SprfirstError;
        summary.OpCodeDecode = SprOpCodeDecode;

        if (this->cpuName == "SPR-HBM")
        {
            summary.FirstError = SprHbmfirstError;
        }
        if (this->cpuName == "EMR")
        {
            summary.FirstError = EmrfirstError;
        }

        if (this->cpuId == "0x806f5")
        {
            summary.PORT_ID = SPR_S0_PORT_ID;
        }

        summary.memoryMap = this->getMemoryMap(inputJson);
        summary.mca = analyzeMca(inputJson);
        summary.corAer = analyzeAer(inputJson, CorAerRegisters, corAerMask);
        summary.uncAer = analyzeAer(inputJson, UncAerRegisters, uncAerMask);
        summary.tsc = getTscData(inputJson);
        summary.thermStatus = getThermData(inputJson);
        summary.tor = analyzeTor(inputJson);
        return summary;
    }
};
