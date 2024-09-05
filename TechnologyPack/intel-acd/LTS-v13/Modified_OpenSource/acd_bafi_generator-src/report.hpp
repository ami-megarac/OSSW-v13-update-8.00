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
#include "generic_report.hpp"
#include "pcilookup.hpp"
#include "tor_defs.hpp"
#include "tor_eaglestream.hpp"
#include "tor_purley.hpp"
#include "tor_whitley.hpp"
#include "utils.hpp"
#include "retry_rd_spr.hpp"
#include "retry_rd_icx.hpp"
#include "retry_rd_cpx.hpp"
#include "retry_rd_skx.hpp"

#include <nlohmann/json.hpp>
#include <optional>
#include <regex>
#include <string>

using json = nlohmann::json;

class Report : public GenericReport
{
  public:
    Report() = delete;
        Report(Summary& summary, json deviceMapFile, json silkscreenMapFile, const json& input) :
        GenericReport(summary), summary(summary), deviceMap(deviceMapFile), silkscreenMap(silkscreenMapFile), input(input){}

    [[nodiscard]] bool chaNotPresentInMce(uint8_t chaNum, json& summaryEntry, json& errorsPerSocket,
                                          std::optional<std::string> die)
    {
        std::string firstError;

        json summarySection;
        if (die)
        {
            summarySection = summaryEntry[0]["MCE"][*die];
        }
        else
        {
            summarySection = summaryEntry[0]["MCE"];
        }

        for (const auto& [key, val] : summarySection.items())
        {
            std::string chaBank = "bank CHA" + std::to_string(chaNum);
            std::string firstMcerr = "FirstMCERR = CHA" + std::to_string(chaNum) + ",";
            if (std::string(key).find(chaBank) != std::string::npos &&
                std::string(errorsPerSocket).find(firstMcerr) == std::string::npos)
            {
                return true;
            }
        }
        return false;
    }

    void decodeTorEntry(std::shared_ptr<TORDataGen> torEntry, json& errorsPerSocket, json& socketEntry, json& summaryEntry,
                        SocketCtx ctxSocket, std::optional<std::string> die)
    {
        json entry;
        json firstMcerr;
        json& details = entry["Details"];
        uint8_t core_id = torEntry->getCoreId();
        uint8_t thread_id = torEntry->getThreadId();
        uint16_t request_opCode = torEntry->getRequestOpCode();
        uint8_t in_pipe = torEntry->getInPipe();
        uint8_t retry = torEntry->getRetry();
        uint8_t fsm = torEntry->getFsm();
        uint8_t lcs = torEntry->getLcs();
        uint8_t target = torEntry->getTarget();
        uint8_t sad = torEntry->getSad();
        uint64_t address = torEntry->getAddress();

        auto firstMcerrCha = getFirstErrorCha(summary.FirstErrorCha, ctxSocket.mcerr->getData());

        auto sideBandPortId = getFirstErrorCha(summary.FirstErrorCha, ctxSocket.ncEventsCrSmiSrcLog->getData());

        auto firstRmcaMcerrCha = getFirstErrorCha(summary.FirstErrorCha, ctxSocket.rMcerrErr->getData());

        auto firstIerrCha = getFirstErrorCha(summary.FirstErrorCha, ctxSocket.ierr->getData());

        if (summary.cpuType == "SPR" || summary.cpuType == "SPR-HBM" || summary.cpuType == "EMR")
        {
            auto entryCasted = std::dynamic_pointer_cast<TORDataSpr>(torEntry);

            if (summary.cpuId != "0x806f0" && summary.cpuId != "0x806f1" && summary.cpuId != "0x806f5")
            {
                address = entryCasted->getAddressSteppingSpecific();
            }
            if (summary.cpuId == "0x806f0" || summary.cpuId == "0x806f5")
            {
                core_id = entryCasted->getCoreIdSteppingA();
                thread_id = entryCasted->getThreadIdSteppingAB();
                request_opCode = entryCasted->getRequestOpCodeSteppingA();
                fsm = entryCasted->getFsmSteppingAB();
                lcs = entryCasted->getLcsSteppingA();
                target = entryCasted->getTargetSteppingA();
                sad = entryCasted->getSadSteppingA();
            }
            else if (summary.cpuId == "0x806f1")
            {
                core_id = entryCasted->getCoreIdSteppingB();
                thread_id = entryCasted->getThreadIdSteppingAB();
                request_opCode = entryCasted->getRequestOpCodeSteppingB();
                fsm = entryCasted->getFsmSteppingAB();
                target = entryCasted->getTargetSteppingB();
                sad = entryCasted->getSadSteppingB();
            }
        }
        details["CoreId"] = int_to_hex(static_cast<uint8_t>(core_id));
        details["ThreadId"] = int_to_hex(static_cast<uint8_t>(thread_id));
        details["CHA"] = int_to_hex(static_cast<uint8_t>(torEntry->cha));
        details["IDX"] = int_to_hex(static_cast<uint8_t>(torEntry->idx));
        details["Request"] = int_to_hex(static_cast<uint16_t>(request_opCode));
        auto requestDecoded = getDecoded(summary.OpCodeDecode, static_cast<uint32_t>(request_opCode));
        if (requestDecoded)
        {
            details["Request_decoded"] = *requestDecoded;
        }

        auto addressMapped = mapToMemory(address);
        if (addressMapped)
        {
            details["Address"] = int_to_hex(address) + " (" + *addressMapped + ")";
        }
        else
        {
            details["Address"] = int_to_hex(address);
        }

        details["InPipe"] = int_to_hex(static_cast<uint8_t>(in_pipe));
        details["Retry"] = int_to_hex(static_cast<uint8_t>(retry));
        details["TorFSMState"] = int_to_hex(static_cast<uint8_t>(fsm));
        details["LLC"] = int_to_hex(static_cast<uint8_t>(lcs));
        if (static_cast<uint8_t>(lcs) < summary.LLCS.size())
        {
            details["LLC_decoded"] = summary.LLCS[lcs];
        }
        details["Target"] = int_to_hex(static_cast<uint8_t>(target));
        if (static_cast<uint8_t>(target) < summary.PORT_ID.size())
        {
            details["Target_decoded"] = summary.PORT_ID[target];
        }

        details["SAD"] = int_to_hex(static_cast<uint8_t>(sad));
        if (static_cast<uint8_t>(sad) < SAD_RESULT.size())
        {
            details["SAD_decoded"] = SAD_RESULT[sad];
        }

        json& decoded = entry["Decoded"];

        if (sad == static_cast<uint8_t>(SadValues::CFG))
        {
            decoded["ErrorType"] = "TOR Timeout Error";
            decoded["ErrorSubType"] = "Type 1: PCIe* MMCFG access cause TOR Timeout";
            if (static_cast<uint8_t>(target) < summary.PORT_ID.size())
            {
                decoded["Port"] = summary.PORT_ID[target];
            }

            // Type 1 gets BDF from TOR address
            decoded["BDF"] = BDFtoString(getBDFFromAddress(address));
            auto bdfObj = nlohmann::ordered_json::object();
            getBdfFromFirstMcerrSection(decoded["BDF"], bdfObj);
            if (bdfObj.contains("Bus"))
                showBdfDescription(deviceMap, bdfObj);

            if (bdfObj.contains("Description"))
                decoded["Description"] = bdfObj["Description"];
        }
        else if (sad == static_cast<uint8_t>(SadValues::MMIOPartialRead))
        {
            decoded["ErrorType"] = "TOR Timeout Error";
            decoded["ErrorSubType"] = "Type 2: PCIe* MMIO access cause TOR timeout.";
            if (static_cast<uint8_t>(target) < summary.PORT_ID.size())
            {
                decoded["Port"] = summary.PORT_ID[target];
            }

            // Type 2 gets BDF from memory map
            decoded["BDF"] = BDFtoString(PciBdfLookup::lookup(address));
            auto bdfObj = nlohmann::ordered_json::object();
            getBdfFromFirstMcerrSection(decoded["BDF"], bdfObj);
            if (bdfObj.contains("Bus"))
                showBdfDescription(deviceMap, bdfObj);

            if (bdfObj.contains("Description"))
                decoded["Description"] = bdfObj["Description"];
        }
        else if (sad == static_cast<uint8_t>(SadValues::IO))
        {
            decoded["ErrorType"] = "TOR Timeout Error";
            decoded["ErrorSubType"] = "Type 3: I/O Port in access cause TOR timeout.";
            if (static_cast<uint8_t>(target) < summary.PORT_ID.size())
            {
                decoded["Port"] = summary.PORT_ID[target];
            }

            // Type 3 gets BDF from memory map
            decoded["BDF"] = BDFtoString(PciBdfLookup::lookup(address));
            auto bdfObj = nlohmann::ordered_json::object();
            getBdfFromFirstMcerrSection(decoded["BDF"], bdfObj);
            if (bdfObj.contains("Bus"))
                showBdfDescription(deviceMap, bdfObj);

            if (bdfObj.contains("Description"))
                decoded["Description"] = bdfObj["Description"];
        }

        if ((firstMcerrCha == static_cast<uint8_t>(torEntry->cha) && ctxSocket.mcerr->isValid()) ||
            (sideBandPortId == static_cast<uint8_t>(torEntry->cha) && ctxSocket.ncEventsCrSmiSrcLog->isValid()))
        {
            firstMcerr["First_MCERR"] = entry["Decoded"];
            firstMcerr["First_MCERR"]["Address"] = details["Address"];
            firstMcerr["First_MCERR"]["CHA"] = details["CHA"];
            if (!entry["Decoded"].empty())
            {
                summaryEntry.push_back(firstMcerr);
            }

            entry["First_MCERR"] = true;
        }
        else if (!entry["Decoded"].empty() &&
                 (sad == static_cast<uint8_t>(SadValues::CFG) || sad == static_cast<uint8_t>(SadValues::MMIOPartialRead) ||
                  sad == static_cast<uint8_t>(SadValues::IO)) &&
                 chaNotPresentInMce(torEntry->cha, summaryEntry, errorsPerSocket["Errors_Per_Socket"], die))
        {
            firstMcerr["First_MCERR_FALSE"] = entry["Decoded"];
            firstMcerr["First_MCERR_FALSE"]["Address"] = details["Address"];
            firstMcerr["First_MCERR_FALSE"]["CHA"] = details["CHA"];
            if (!entry["Decoded"].empty())
            {
                summaryEntry.push_back(firstMcerr);
            }
            entry["First_MCERR"] = false;
        }
        else if (summary.cpuType == "SPR" || summary.cpuType == "SPR-HBM" || summary.cpuType == "EMR" || summary.cpuType == "GNR"|| summary.cpuType == "SRF")
        {
            if (firstRmcaMcerrCha == static_cast<uint8_t>(torEntry->cha) && ctxSocket.rMcerrErr->isValid())
            {
                firstMcerr["First_MCERR"] = entry["Decoded"];
                firstMcerr["First_MCERR"]["Address"] = details["Address"];
                if (!entry["Decoded"].empty())
                {
                    summaryEntry.push_back(firstMcerr);
                }
                entry["First_MCERR"] = true;
            }
            else
            {
                entry["First_MCERR"] = false;
            }
        }
        else
        {
            entry["First_MCERR"] = false;
        }

        if (firstIerrCha == static_cast<uint8_t>(torEntry->cha) && ctxSocket.ierr->isValid())
        {
            entry["First_IERR"] = true;
        }
        else
        {
            entry["First_IERR"] = false;
        }
        socketEntry.push_back(entry);
    }

    std::optional<json> generateIoErrorsSection(json& summarySection)
    {
        std::regex uboxMCE("UBOX");
        std::regex iioMCE("IIO");

        json ioErrorsSeciton;
        for (auto& el : summarySection.items())
        {
            if (std::regex_search(std::string(el.key()), uboxMCE) || std::regex_search(std::string(el.key()), iioMCE))
            {
                std::size_t left = std::string(el.value()).find("Bus");
                if (left > std::string(el.value()).size())
                    continue;

                std::size_t right = std::string(el.value()).find(" from");
                ioErrorsSeciton["Device"] = std::string(el.value()).substr(left, right - left);

                auto bdfObj = nlohmann::ordered_json::object();
                getBdfFromIoErrorsSection(std::string(el.value()).substr(left, right - left), bdfObj);
                showBdfDescription(deviceMap, bdfObj);
                if (bdfObj.contains("Description"))
                {
                    ioErrorsSeciton["Description"] = std::string(bdfObj["Description"]);
                }

                return ioErrorsSeciton;
            }
        }
        return std::nullopt;
    }

    void processSocketTor(json& summaryEntry, json& errorsPerSocket, SocketCtx ctxSocket)
    {
        json uboxAdditionalInformation = nlohmann::ordered_json::object();

        json thermalStatus;
        errorsPerSocket["Errors_Per_Socket"] = createSummaryString(ctxSocket, summary.FirstError, summary.cpuType, summaryEntry);

        std::regex mcerr;
        if (summary.cpuType == "GNR"|| summary.cpuType == "SRF")
        {
            mcerr.assign("FirstMCERR = .*, bank 4");
        }
        else
        {
            mcerr.assign("FirstMCERR = .*, bank 6");
        }

        json& ioErrorsSection = uboxAdditionalInformation["IO_Errors"];
        std::optional<json> ioErrors;

        if (std::regex_search(std::string(errorsPerSocket["Errors_Per_Socket"]), mcerr))
        {
            if (summary.cpuType == "GNR" || summary.cpuType == "SRF")
            {
                for (const auto& [first, second] : summaryEntry[0]["MCE"].items())
                {
                    ioErrors = generateIoErrorsSection(second);
                    if (ioErrors)
                    {
                        ioErrorsSection[first] = *ioErrors;
                    }
                }
            }
            else
            {
                ioErrors = generateIoErrorsSection(summaryEntry[0]["MCE"]);
                if (ioErrors)
                {
                    ioErrorsSection = *ioErrors;
                }
            }
        }
        summaryEntry.push_back(errorsPerSocket);
        if (!ioErrorsSection.empty())
            summaryEntry.push_back(uboxAdditionalInformation);
    }

    [[nodiscard]] json createJSONReport()
    {
        json report = createGenericReport();
        json& torReport = report["TOR"];
        json& summaryReport = report["output_summary"];
        summaryReport["CPU_type"] = getFullVersionName(input);
        summaryReport["Crashdump_timestamp"] = getTimestamp(input);
        
        summary.tor->fillEntries(
            torReport, summaryReport,
            [&](std::shared_ptr<TORDataGen> torEntry, json& errorsPerSocket, json& socketEntry, json& summaryEntry,
                SocketCtx ctxSocket, std::optional<std::string> die) {
                decodeTorEntry(torEntry, errorsPerSocket, socketEntry, summaryEntry, ctxSocket, die);
            },
            [&](json& summaryEntry, json& errorsPerSocket, SocketCtx ctxSocket) {
                processSocketTor(summaryEntry, errorsPerSocket, ctxSocket);
            });
        
        // Get memory errors for specific CPUs
        processMemoryErrors(summaryReport);
        return report;
    }

    void processMemoryErrors(json& summaryReport)
    {
        if (summary.cpuType == "SKX")
        {
            auto sockets = findSocketsPurley(input);
            for (const auto& socket : sockets)
            {
                json& summaryEntry = summaryReport[socket];
                auto socketId = socket.substr(6, 1);
 
                auto memoryErrors = decodeRetryLogSkx(input, silkscreenMap, socketId);
                if (memoryErrors != nullptr)
                    summaryEntry.push_back(memoryErrors);
            }
        }
        else if (summary.cpuType == "ICX")
        {
            auto cpuSections = prepareJsonGeneric(input);
            for (auto const& [socketId, cpuSection] : cpuSections)
            {
                std::string socket = "socket" + std::string(socketId).substr(3, 1);
                json& summaryEntry = summaryReport[socket];
                
                auto memoryErrors = decodeRetryLogIcx(cpuSection, silkscreenMap, std::string(socketId).substr(3, 1));
                if (memoryErrors != nullptr)
                    summaryEntry.push_back(memoryErrors);
            }
        }
        else if (summary.cpuType == "CPX")
        {
            auto cpuSections = prepareJsonGeneric(input);
            for (auto const& [socketId, cpuSection] : cpuSections)
            {
                std::string socket = "socket" + std::string(socketId).substr(3, 1);
                json& summaryEntry = summaryReport[socket];

                auto memoryErrors = decodeRetryLogCpx(cpuSection, silkscreenMap, std::string(socketId).substr(3, 1));
                if (memoryErrors != nullptr)
                    summaryEntry.push_back(memoryErrors);
            }
        }
        else if (summary.cpuType == "SPR" || summary.cpuType == "SPR-HBM" || summary.cpuType == "EMR")
        {
            auto cpuSections = prepareJsonGeneric(input);
            for (auto const& [socketId, cpuSection] : cpuSections)
            {
                std::string socket = "socket" + std::string(socketId).substr(3, 1);
                json& summaryEntry = summaryReport[socket];

                auto memoryErrors = decodeRetryLogSpr(cpuSection, silkscreenMap, std::string(socketId).substr(3, 1));
                if (memoryErrors != nullptr)
                    summaryEntry.push_back(memoryErrors);

                auto counters = parseCounterRegisters(cpuSection);
                if (counters != nullptr)
                    summaryEntry.push_back(counters);
            }
        }

    }

  private:    
    Summary& summary;
    json deviceMap;
    json silkscreenMap;
    const json& input;
};
