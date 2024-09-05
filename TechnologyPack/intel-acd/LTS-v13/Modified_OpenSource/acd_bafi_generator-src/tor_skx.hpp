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
#include "tor_purley.hpp"
#include "utils.hpp"
#include "summary_field.hpp"
#include "summary.hpp"
#include "tor_defs_skx.hpp"

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

class SkxCpu final : public PurleyCpu, public CpuGeneric
{
  public:
    // indexes for IERR, MCERR and MCERR source
    std::string ierr_varname = "ubox_ncevents_ierrloggingreg_b0d08f0";
    std::string mcerr_varname = "ubox_ncevents_mcerrloggingreg_b0d08f0";
    std::string mca_err_src_varname = "mca_err_src_log";
    std::string bdf_ierr_varname = "B00_D08_F0_0xA4";
    std::string bdf_mcerr_varname = "B00_D08_F0_0xA8";
    std::string bdf_mca_err_src_varname = "B01_D1e_F2_0xEC";
    // bitmasks of uncorrectable and correctable AER errors to decode
    static const uint32_t unc_spec_err_mask = 0x3FF030;
    static const uint32_t cor_spec_err_mask = 0x31C1;

    SkxCpu(std::string cpuName, std::string cpuId): CpuGeneric(cpuName, cpuId){}

    [[nodiscard]] virtual AerPointer analyzeUncAer(const json& input)
    {
        AerGenericPointer output = std::make_shared<AerGeneric>();
        if (!input.contains("uncore_status_regs"))
        {
            return output;
        }

        for (auto const& [socket, socketSection] : input["uncore_status_regs"].items())
        {
            if (!input["uncore_status_regs"].contains(socket))
            {
                return output;
            }

            if (!startsWith(socket, "socket"))
            {
                continue;
            }

            uint32_t socketId;
            if (!str2uint(socket.substr(6), socketId, decimal))
            {
                continue;
            }

            auto allUncErr = parseUncErrSts(socketSection);
            output->insert(socketId, allUncErr);
        }
        return output;
    }

    [[nodiscard]] std::vector<AerData> parseUncErrSts(const json& input)
    {
        // decode only pxp_b*d(0-3)f*_uncerrsts for SKX
        std::regex unc_decode_index_1("pxp_b.d00f._uncerrsts");
        std::regex unc_decode_index_2("pxp_b.d01f._uncerrsts");
        std::regex unc_decode_index_3("pxp_b.d02f._uncerrsts");
        std::regex unc_decode_index_4("pxp_b.d03f._uncerrsts");

        // decode only B*_D(0-3)_F* for SKX
        std::regex bdf_unc_decode_index_1("_D00_F._0x14C");
        std::regex bdf_unc_decode_index_2("_D01_F._0x14C");
        std::regex bdf_unc_decode_index_3("_D02_F._0x14C");
        std::regex bdf_unc_decode_index_4("_D03_F._0x14C");

        std::vector<AerData> allUncErrs;
        for (const auto& [pciKey, pciVal] : input.items())
        {
            uint32_t temp;
            if (pciVal == false || checkInproperValue(pciVal) || pciVal == "0x0")
            {
                continue;
            }

            // decode only D00 - D03 indexes
            if (!std::regex_search(std::string(pciKey), unc_decode_index_1) &&
                !std::regex_search(std::string(pciKey), unc_decode_index_2) &&
                !std::regex_search(std::string(pciKey), unc_decode_index_3) &&
                !std::regex_search(std::string(pciKey), unc_decode_index_4))
            {
                if (!std::regex_search(std::string(pciKey), bdf_unc_decode_index_1) &&
                    !std::regex_search(std::string(pciKey), bdf_unc_decode_index_2) &&
                    !std::regex_search(std::string(pciKey), bdf_unc_decode_index_3) &&
                    !std::regex_search(std::string(pciKey), bdf_unc_decode_index_4))
                {
                    continue;
                }
            }

            if (!str2uint(pciVal, temp))
            {
                continue;
            }

            temp = temp & unc_spec_err_mask;
            AerData uncErr;
            uncErr.error_status = temp;
            if (uncErr.error_status != 0)
            {
                uncErr.address = pciKey;
                allUncErrs.push_back(uncErr);
            }
        }
        return allUncErrs;
    }

    [[nodiscard]] virtual AerPointer analyzeCorAer(const json& input)
    {
        AerGenericPointer output = std::make_shared<AerGeneric>();
        if (!input.contains("uncore_status_regs"))
        {
            return output;
        }

        for (auto const& [socket, socketSection] : input["uncore_status_regs"].items())
        {
            if (!input["uncore_status_regs"].contains(socket))
            {
                return output;
            }

            if (!startsWith(socket, "socket"))
            {
                continue;
            }

            uint32_t socketId;
            if (!str2uint(socket.substr(6), socketId, decimal))
            {
                continue;
            }

            auto allCorErr = parseCorErrSts(socketSection);
            output->insert(socketId, allCorErr);
        }
        return output;
    }

    [[nodiscard]] std::vector<AerData> parseCorErrSts(const json& input)
    {
        // decode only pxp_b*d(0-3)f*_corerrsts for SKX
        std::regex cor_decode_index_1("pxp_b.d00f._corerrsts");
        std::regex cor_decode_index_2("pxp_b.d01f._corerrsts");
        std::regex cor_decode_index_3("pxp_b.d02f._corerrsts");
        std::regex cor_decode_index_4("pxp_b.d03f._corerrsts");

        // decode only B*_D(0-3)_F* for SKX
        std::regex bdf_cor_decode_index_1("_D00_F._0x158");
        std::regex bdf_cor_decode_index_2("_D01_F._0x158");
        std::regex bdf_cor_decode_index_3("_D02_F._0x158");
        std::regex bdf_cor_decode_index_4("_D03_F._0x158");

        std::vector<AerData> allCorErrs;
        for (const auto& [pciKey, pciVal] : input.items())
        {
            uint32_t temp;
            if (pciVal == false || checkInproperValue(pciVal) || pciVal == "0x0")
            {
                continue;
            }

            // decode only D00 - D03 indexes
            if (!std::regex_search(std::string(pciKey), cor_decode_index_1) &&
                !std::regex_search(std::string(pciKey), cor_decode_index_2) &&
                !std::regex_search(std::string(pciKey), cor_decode_index_3) &&
                !std::regex_search(std::string(pciKey), cor_decode_index_4))
            {
                if (!std::regex_search(std::string(pciKey), bdf_cor_decode_index_1) &&
                    !std::regex_search(std::string(pciKey), bdf_cor_decode_index_2) &&
                    !std::regex_search(std::string(pciKey), bdf_cor_decode_index_3) &&
                    !std::regex_search(std::string(pciKey), bdf_cor_decode_index_4))
                {
                    continue;
                }
            }

            if (!str2uint(pciVal, temp))
            {
                continue;
            }

            temp = temp & cor_spec_err_mask;
            AerData corErr;
            corErr.error_status = temp;
            if (corErr.error_status != 0)
            {
                corErr.address = pciKey;
                allCorErrs.push_back(corErr);
            }
        }
        return allCorErrs;
    }

    [[nodiscard]] virtual MCAPointer analyzeMca(const json& input)
    {
        MCAGenericPointer output = std::make_shared<MCAGeneric>();
        std::vector<std::string> sockets = findSocketsPurley(input);
        for (auto const& socket : sockets)
        {
            uint32_t socketId;
            if (!str2uint(socket.substr(6), socketId, decimal))
            {
                continue;
            }

            std::vector<MCAData> uncoreMcas = parseUncoreMcas(input, socket);
            std::vector<MCAData> coreMcas = parseCoreMcas(input, socket);
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
        if (index.find("DW0") == index.cend())
        {
            return {};
        }

        if (checkInproperValue(index["DW0"]))
        {
            return {};
        }

        TORDataCpxSkx tor;
        if (!str2uint(index["DW0"], tor.subindex0) || !str2uint(index["DW1"], tor.subindex1) ||
            !str2uint(index["DW2"], tor.subindex2))
        {
            return {};
        }

        if (!tor.valid)
        {
            return {};
        }
        return tor;
    }

    [[nodiscard]] std::vector<std::shared_ptr<TORDataGen>> getTorsData(const json& input, std::string socket)
    {
        std::vector<std::shared_ptr<TORDataGen>> torsData;
        if (input.find("TOR_dump") == input.cend())
        {
            return torsData;
        }

        if (input["TOR_dump"].find(socket) == input["TOR_dump"].cend())
        {
            return torsData;
        }

        for (const auto& [chaItemKey, chaItemValue] : input["TOR_dump"][socket].items())
        {
            if (!startsWith(chaItemKey, "CHA"))
            {
                continue;
            }

            for (const auto& [torDataKey, torDataValue] : chaItemValue.items())
            {
                std::optional<TORDataCpxSkx> tor = parseTorData(torDataValue);
                if (!tor)
                {
                    continue;
                }

                if (str2uint(chaItemKey.substr(3), tor->cha, decimal) && str2uint(torDataKey.substr(3), tor->idx, decimal))
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
        std::vector<std::string> sockets = findSocketsPurley(input);
        for (auto const& socket : sockets)
        {
            if (!input.contains("uncore_status_regs"))
            {
                ierr_varname = "IERRLOGGINGREG";
                mcerr_varname = "MCERRLOGGINGREG";
                mca_err_src_varname = "";
            }

            if (input.contains("crashdump") && input.contains("uncore_status_regs"))
            {
                if (input["crashdump"].contains(socket))
                {
                    if (input["crashdump"][socket].contains("uncore_regs") && input["uncore_status_regs"].contains(socket) &&
                        input["uncore_status_regs"].contains("status"))
                    {
                        ierr_varname = "IERRLOGGINGREG";
                        mcerr_varname = "MCERRLOGGINGREG";
                    }
                }
            }

            if (input.contains("uncore_status_regs"))
            {
                if (input["uncore_status_regs"].contains(socket))
                {
                    if (input["uncore_status_regs"][socket].contains("B01_D1e_F2_0xEC"))
                    {
                        ierr_varname = bdf_ierr_varname;
                        mcerr_varname = bdf_mcerr_varname;
                        mca_err_src_varname = bdf_mca_err_src_varname;
                    }
                }
            }
            // Look for mca_err_src_log in sys_info section
            std::optional mcerrErrSrc = getJsonStringValue(input, "sys_info", socket, mca_err_src_varname);
            // Look for mca_err_src_log in uncore_status_regs section
            if (mca_err_src_varname != "mca_err_src_log")
            {
                mcerrErrSrc = getUncoreData(input, socket, mca_err_src_varname);
            }

            std::optional ierr = getUncoreData(input, socket, ierr_varname);
            std::optional mcerr = getUncoreData(input, socket, mcerr_varname);

            auto tors = getTorsData(input, socket);
            uint32_t socketId;
            if (!str2uint(socket.substr(6), socketId, decimal))
            {
                continue;
            }

            uint32_t ierrValue, mcerrValue, mcerrErrSrcValue;

            if (!ierr || !str2uint(*ierr, ierrValue))
            {
                ierrValue = 0;
            }

            if (!mcerr || !str2uint(*mcerr, mcerrValue))
            {
                mcerrValue = 0;
            }

            if (!mcerrErrSrc || !str2uint(*mcerrErrSrc, mcerrErrSrcValue))
            {
                mcerrErrSrcValue = 0;
            }

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

        summary.PORT_ID = SKX_PORT_ID;
        summary.LLCS = SKX_LLCS;
        summary.FirstErrorCha = SkxfirstErrorCha;
        summary.FirstError = SkxfirstError;
        summary.OpCodeDecode = SkxOpCodeDecode;

        summary.memoryMap = this->getMemoryMap(inputJson);
        summary.mca = analyzeMca(inputJson);
        summary.corAer = analyzeCorAer(inputJson);
        summary.uncAer = analyzeUncAer(inputJson);
        summary.tor = analyzeTor(inputJson);
        return summary;
    }
};
