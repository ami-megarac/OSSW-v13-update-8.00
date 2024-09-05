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

#define BAFI_VERSION "2.93"

#pragma once
#include "aer.hpp"
#include "mca.hpp"
#include "mca_defs.hpp"
#include "summary.hpp"
#include "tor_defs.hpp"
#include "utils.hpp"

#include <array>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using json = nlohmann::json;

class GenericReport
{
  public:
    GenericReport() = delete;
    GenericReport(Summary& summary) : summary(summary){};

    json generateUncAerEntry(std::vector<AerData> socketAerData)
    {
        json aerEntry;
        for (auto const& aerData : socketAerData)
        {
            aerEntry["PCIe_AER_Uncorrectable_errors"][aerData.address] = createUncAerString(aerData);
        }
        return aerEntry;
    }

    json generateCorAerEntry(std::vector<AerData> socketAerData)
    {
        json aerEntry;
        for (auto const& aerData : socketAerData)
        {
            aerEntry["PCIe_AER_Correctable_errors"][aerData.address] = createCorAerString(aerData);
        }
        return aerEntry;
    }

    json generateMcaEntry(std::vector<MCAData> socketMcasData)
    {
        json mcaEntry = json::array();
        for (auto const& mcaData : socketMcasData)
        {
            auto entry = createMcaEntry(mcaData);
            if (entry.empty())
            {
                continue;
            }
            mcaEntry.push_back(entry);
        }
        return mcaEntry;
    }

    std::optional<uint64_t> getSmallestTscFromSocket(TscData socketTscData)
    {
        if (socketTscData.pcu_first_mcerr_tsc_cfg != 0 || socketTscData.pcu_first_ierr_tsc_cfg != 0 ||
            socketTscData.pcu_first_rmca_tsc_cfg != 0)
        {
            std::vector<uint64_t> tempValues;
            if (socketTscData.pcu_first_mcerr_tsc_cfg != 0)
                tempValues.push_back(socketTscData.pcu_first_mcerr_tsc_cfg);
            if (socketTscData.pcu_first_ierr_tsc_cfg != 0)
                tempValues.push_back(socketTscData.pcu_first_ierr_tsc_cfg);
            if (socketTscData.pcu_first_rmca_tsc_cfg != 0)
                tempValues.push_back(socketTscData.pcu_first_rmca_tsc_cfg);

            return *std::min_element(tempValues.begin(), tempValues.end());
        }
        return std::nullopt;
    }

    std::optional<uint64_t> findSmallestTsc(TSCPointer field)
    {
        std::vector<uint64_t> smallestValuesFromSockets;
        auto allTscs = field->flatten();
        for (const auto& elem : allTscs)
        {
            auto smallestValueFromSocket = getSmallestTscFromSocket(elem);
            if (smallestValueFromSocket)
            {
                smallestValuesFromSockets.push_back(*smallestValueFromSocket);
            }
        }

        if(smallestValuesFromSockets.size() == 0)
        {
            // all tscs are 0
            return std::nullopt;
        }
        return *std::min_element(smallestValuesFromSockets.begin(), smallestValuesFromSockets.end());
    }

    json generateTscEntry(TscData socketTscData, uint64_t smallest)
    {
        json tscData = json::object();
        if (socketTscData.pcu_first_ierr_tsc_cfg != 0 || socketTscData.pcu_first_mcerr_tsc_cfg != 0 ||
            socketTscData.pcu_first_rmca_tsc_cfg != 0)
        {
            std::stringstream ss1;
            ss1 << int_to_hex(socketTscData.pcu_first_ierr_tsc_cfg);
            if (socketTscData.pcu_first_ierr_tsc_cfg == smallest && socketTscData.pcu_first_ierr_tsc_cfg != 0)
                ss1 << " (Occurred first between all TSCs)";
            tscData["TSC"]["pcu_first_ierr_tsc_cfg"] = ss1.str();

            std::stringstream ss2;
            ss2 << int_to_hex(socketTscData.pcu_first_mcerr_tsc_cfg);
            if (socketTscData.pcu_first_mcerr_tsc_cfg == smallest)
                ss2 << " (Occurred first between all TSCs)";
            tscData["TSC"]["pcu_first_mcerr_tsc_cfg"] = ss2.str();

            if (summary.cpuType == "SPR" || summary.cpuType == "SPR-HBM" || summary.cpuType == "EMR" || summary.cpuType == "GNR" || summary.cpuType == "SRF")
            {
                std::stringstream ss3;
                ss3 << int_to_hex(socketTscData.pcu_first_rmca_tsc_cfg);
                if (socketTscData.pcu_first_rmca_tsc_cfg == smallest)
                    ss3 << " (Occurred first between all TSCs)";
                tscData["TSC"]["pcu_first_rmca_tsc_cfg"] = ss3.str();
            }

            std::stringstream ss4;
            ss4 << std::boolalpha << socketTscData.ierr_on_socket;
            tscData["TSC"]["IERR occurred"] = ss4.str();

            std::stringstream ss5;
            ss5 << std::boolalpha << socketTscData.mcerr_on_socket;
            tscData["TSC"]["MCERR occurred"] = ss5.str();

            if (summary.cpuType == "SPR" || summary.cpuType == "SPR-HBM" || summary.cpuType == "EMR" || summary.cpuType == "GNR" || summary.cpuType == "SRF")
            {
                std::stringstream ss6;
                ss6 << std::boolalpha << socketTscData.rmca_on_socket;
                tscData["TSC"]["RMCA occurred"] = ss6.str();
            }
        }

        return tscData;
    }

    [[nodiscard]] json createGenericReport()
    {
        json report;
        json& summaryReport = report["output_summary"];
        json& mcaReport = report["MCA"];
        summaryReport["BAFI_version"] = BAFI_VERSION;

        summary.mca->fillEntries([&](std::vector<MCAData> mcaData) { return generateMcaEntry(mcaData); }, mcaReport,
                                 summaryReport);

        summary.uncAer->fillEntries([&](std::vector<AerData> socketAerData) { return generateUncAerEntry(socketAerData); },
                                    summaryReport);
        summary.corAer->fillEntries([&](std::vector<AerData> socketAerData) { return generateCorAerEntry(socketAerData); },
                                    summaryReport);

        if (summary.tsc != nullptr) // for earlier gnerations tsc data is not collected
        {
            auto smallestTsc = findSmallestTsc(summary.tsc);
            if (smallestTsc) // if there exists non-zero tsc
            {
                summary.tsc->fillEntries([&](TscData tscData) { return generateTscEntry(tscData, *smallestTsc); }, summaryReport);
            }
        }
        
        if (summary.thermStatus != nullptr) // for earlier gnerations therm data is not collected
        {
            summary.thermStatus->fillEntries([&](PackageThermStatus thermData) { return createThermStatusString(thermData); },
                                                summaryReport);
        }
        return report;
    }

    Summary& summary;

  protected:
    json createMcaEntry(MCAData mcaData)
    {
        auto bankDecoder = mcaDecoderFactory(mcaData, summary.cpuType);
        json entry;
        if (!bankDecoder)
        {
            return entry;
        }
        entry = bankDecoder->decode();
        // decode CHA bank address according to memory map
        if (mcaData.type == McaTypeFlag::CBO)
        {
            auto addressMapped = mapToMemory(mcaData.address);
            if (addressMapped)
            {
                std::stringstream ss;
                ss << entry["Address"].get<std::string>() << " (" << *addressMapped << ")";
                entry["Address"] = ss.str();
            }
        }
        return entry;
    }

    std::optional<std::string> mapToMemory(uint64_t address)
    {
        for (auto const& [name, limits] : summary.memoryMap)
        {
            if (address >= limits[0] && address <= limits[1])
            {
                return name;
            }
        }
        return {};
    }

    std::optional<uint8_t> getFirstErrorCha(std::map<uint8_t, uint8_t> decodingTable, uint32_t toDecode)
    {
        const auto& item = decodingTable.find(toDecode);
        if (item != decodingTable.cend())
        {
            return item->second;
        }
        return {};
    }

    json createThermStatusString(PackageThermStatus thermStatus)
    {
        std::stringstream ss;
        json ret;

        ss << "PROCHOT_LOG=";
        ss << int_to_hex(bool(thermStatus.prochot_log));
        ss << ", PROCHOT_STATUS=";
        ss << int_to_hex(bool(thermStatus.prochot_status));
        ss << ", PMAX_LOG=";
        ss << int_to_hex(bool(thermStatus.pmax_log));
        ss << ", PMAX_STATUS=";
        ss << int_to_hex(bool(thermStatus.pmax_status));
        ss << ", OUT_OF_SPEC_LOG=";
        ss << int_to_hex(bool(thermStatus.out_of_spec_log));
        ss << ", OUT_OF_SPEC_STATUS=";
        ss << int_to_hex(bool(thermStatus.out_of_spec_status));
        ss << ", THERMAL_MONITOR_LOG=";
        ss << int_to_hex(bool(thermStatus.thermal_monitor_log));
        ss << ", THERMAL_MONITOR_STATUS=";
        ss << int_to_hex(bool(thermStatus.thermal_monitor_status));
        ret["Package_Therm_Status"] = ss.str();
        return ret;
    }

    std::string createSummaryString(SocketCtx ctx, const std::map<uint16_t, const char*> firstErrorTable, std::string Cpu,
                                    json& summaryEntry)
    {
        std::stringstream ss;
        auto firstIerrSrc = getDecoded(firstErrorTable, static_cast<uint16_t>(ctx.ierr->getData()));
        auto firstMcerrSrc = getDecoded(firstErrorTable, static_cast<uint16_t>(ctx.mcerr->getData()));
        auto firstRmcaSrc = getDecoded(firstErrorTable, static_cast<uint16_t>(ctx.rMcerrErr->getData()));
        auto sideBandPortId = getDecoded(firstErrorTable, static_cast<uint16_t>(ctx.ncEventsCrSmiSrcLog->getData()));
        auto sideBandPortValid = ctx.ncEventsCrSmiSrcLog->isValid();

        std::map<std::vector<uint8_t>, const char*> decodeMcerrErrorSource;
        if (ctx.mcerrErrValue == 0)
        {
            ss << "MCA_ERR_SRC_LOG = N/A, ";
        }
        else if (Cpu == "SPR" || Cpu == "SPR-HBM" || Cpu == "EMR")
        {
            decodeMcerrErrorSource = decodeMcerrErrorSourceSpr;
        }
        else
        {
            decodeMcerrErrorSource = decodeMcerrErrorSourceGeneric;
        }

        ss << DecodeBits(decodeMcerrErrorSource, ctx.mcerrErrValue);

        if (firstIerrSrc && ctx.ierr->isValid())
        {
            ss << "FirstIERR = " << *firstIerrSrc << ", ";
        }
        else if (ctx.ierr->isValid())
        {
            ss << "FirstIERR = N/A, ";
        }
        else
        {
            ss << "FirstIERR = 0x0, ";
        }

        if (firstMcerrSrc && ctx.mcerr->isValid())
        {
            ss << "FirstMCERR = " << *firstMcerrSrc << ", ";
            if (!firstMcerrInMce(*firstMcerrSrc, summaryEntry))
            {
                ss << "FirstMCERR value doesn't match any MCE bank#, "
                      "manual review may be required, ";
            }
        }
        else if (ctx.mcerr->isValid())
        {
            ss << "FirstMCERR = N/A, ";
        }
        else
        {
            ss << "FirstMCERR = 0x0, ";
        }

        if (Cpu == "SPR" || Cpu == "SPR-HBM" || Cpu == "EMR")
        {
            if (firstRmcaSrc && ctx.rMcerrErr->isValid())
            {
                ss << "RMCA_FirstMCERR = " << *firstRmcaSrc << ", ";
                if (!firstMcerrInMce(*firstRmcaSrc, summaryEntry))
                {
                    ss << ", RMCA_FirstMCERR value doesn't match any MCE bank#, "
                          "manual review may be required, ";
                }
            }
            else if (ctx.rMcerrErr->isValid())
            {
                ss << "RMCA_FirstMCERR = N/A, ";
            }
            else
            {
                ss << "RMCA_FirstMCERR = 0x0, ";
            }
        }

        if (Cpu != "SKX" && Cpu != "CPX")
        {
            if (sideBandPortId && sideBandPortValid)
            {
                ss << "SIDEBANDPORTID = " << *sideBandPortId;
            }
            else if (sideBandPortValid)
            {
                ss << "SIDEBANDPORTID = N/A";
            }
            else
            {
                ss << "SIDEBANDPORTID = 0x0";
            }
        }

        std::string output = ss.str();
        if (Cpu == "SKX" || Cpu == "CPX")
        {
            output.erase(output.end() - 2, output.end());
        }

        return output;
    }

    std::string createUncAerString(AerData aerData)
    {
        return DecodeBits(decodeUncAer, aerData.error_status);
    }

    std::string createCorAerString(AerData aerData)
    {
        return DecodeBits(decodeCorAer, aerData.error_status);
    }
};
