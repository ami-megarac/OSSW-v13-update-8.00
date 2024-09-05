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
#include <map>
#include <string>

namespace RetryRdIcx
{
const std::map<uint8_t, const char*> decodeMode = {{0x0, "sddc 2LM"},     {0x1, "sddc 1LM"},     {0x2, "sddc +1 2LM"},
                                                   {0x3, "sddc +1 1LM"},  {0x4, "adddc 2LM"},    {0x5, "adddc 1LM"},
                                                   {0x6, "adddc +1 2LM"}, {0x7, "adddc +1 1LM"}, {0x8, "read is from ddrt dimm"},
                                                   {0x9, "x8 sddc"},      {0xA, "x8 sddc +1"},   {0xB, "not a valid ecc mode"}};
union RetryRdErrLog
{
    struct
    {
        uint32_t valid : 1, uc : 1, over : 1, mode : 4, reserved1 : 15, system_addr_valid : 1, reserved2 : 9;
    };
    uint32_t data = 0;
};

union RetryRdErrLogAddress1
{
    struct
    {
        uint32_t reserved : 6, chip_select : 3, cbit : 4, bank : 6, column : 10, reserved1 : 3;
    };
    uint32_t data = 0;
};

union RetryRdErrLogAddress2
{
    struct
    {
        uint32_t row : 18, reserved : 14;
    };
    uint32_t data = 0;
};

union RetryRdErrLogAddress3
{
    struct
    {
        uint64_t address : 52, reserved : 12;
    };
    uint64_t data = 0;
};

struct RetryRdSet
{
    RetryRdErrLog errLog;
    RetryRdErrLogAddress1 errLogAddress1;
    RetryRdErrLogAddress2 errLogAddress2;
    RetryRdErrLogAddress3 errLogAddress3;
};

struct RetryRdOffsets
{
    std::string errLog;
    std::string errLogAddress1;
    std::string errLogAddress2;
    std::string errLogAddress3;
};

// for each set of retry_rd IMC0 registers have BDF = B30_D26_F0, IMC1 registers have BDF = B30_D27_F0, etc.
std::array<std::pair<std::string, std::string>, 4> imcBdfs = {
    std::make_pair("IMC 0", "B30_D26_F0_"), std::make_pair("IMC 1", "B30_D27_F0_"), std::make_pair("IMC 2", "B30_D28_F0_"),
    std::make_pair("IMC 3", "B30_D29_F0_")};

// for each set of retry_rd CH0 registers' offsets start with:
// "0x22" (retry_rd_err_log), "0x22" (address1), "0x22" (address2), "0x20" (address3)
// and for CH1 registers offsets start with:
// "0x2A" (retry_rd_err_log), "0x2A" (address1), "0x2A" (address2), "0x28" (address3)
std::array<std::pair<std::string, RetryRdOffsets>, 2> channelOffsets = {
    std::make_pair("Channel 0", RetryRdOffsets{"0x22", "0x22", "0x22", "0x20"}),
    std::make_pair("Channel 1", RetryRdOffsets{"0x26", "0x26", "0x26", "0x24"})};

// for set1 retry_rd registers' offsets end with:
// "C60" (retry_rd_err_log), "C58" (address1), "C28" (address2), "ED8" (address3)
// for set2 retry_rd registers' offsets end with:
// "E54" (retry_rd_err_log), "E58" (address1), "E5C" (address2), "EE0" (address3)
// for set3 retry_rd registers' offsets end with:
// "C70" (retry_rd_err_log), "D58" (address1), "C64" (address2), "F10" (address3)
std::array<RetryRdOffsets, 2> setOffsets = {RetryRdOffsets{"C60", "C58", "C28", "ED8"},
                                            RetryRdOffsets{"E54", "E58", "E5C", "EE0"}};

void parseMemoryErrors(std::set<std::pair<std::string, std::string>>& correctableErrors,
                       std::set<std::pair<std::string, std::string>>& uncorrectableErrors, json& memoryErrors)
{
    for (const auto& [key, value] : correctableErrors)
    {
        if (!memoryErrors["Memory_errors"]["Correctable"].contains(key))
        {
            memoryErrors["Memory_errors"]["Correctable"][key] = json::array();
        }

        memoryErrors["Memory_errors"]["Correctable"][key].push_back(value);
    }

    for (const auto& [key, value] : uncorrectableErrors)
    {
        if (!memoryErrors["Memory_errors"]["Uncorrectable"].contains(key))
        {
            memoryErrors["Memory_errors"]["Uncorrectable"][key] = json::array();
        }

        memoryErrors["Memory_errors"]["Uncorrectable"][key].push_back(value);
    }
}

bool decodeSingleRetryRd(std::stringstream& ssKey, std::stringstream& ssValue, json silkscreenMap, RetryRdSet decodedRetryRd,
                         std::string socketId, std::string imc, std::string channel)
{
    std::optional<std::string> mode = getDecoded(decodeMode, static_cast<uint8_t>(decodedRetryRd.errLog.mode));
    if (!mode)
    {
        return false;
    }
    uint8_t bank = decodedRetryRd.errLogAddress1.bank;
    std::string bankAddress = std::to_string((bank & 0xc) >> 2);

    std::string bankGroup = std::to_string((bank & 0x3 || ((bank & 0x10) >> 2)));

    std::string rank = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.chip_select % 4));

    std::string dimm = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.chip_select >> 2 & 0x1));

    std::string subRank = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.cbit));

    std::string column = std::to_string(static_cast<uint16_t>(decodedRetryRd.errLogAddress1.column << 2));

    std::string row = std::to_string(static_cast<uint32_t>(decodedRetryRd.errLogAddress2.row));

    std::string address = int_to_hex(static_cast<uint64_t>(decodedRetryRd.errLogAddress3.address));

    if (silkscreenMap != NULL)
    {
        for (const auto& [slotKey, slotVal] : silkscreenMap["MemoryLocationPcb"].items())
        {
            if (slotVal["Socket"] != std::stoi(socketId))
            {
                continue;
            }
            // extract IMC number from "IMC X" string -|
            //                                         v
            if (slotVal["IMC"] != std::stoi(imc.substr(4)))
            {
                continue;
            }

            // extract channel number from "Channel X" string -|
            //                                                 v
            if (slotVal["Channel"] != std::stoi(channel.substr(8)))
            {
                continue;
            }

            if (slotVal["Slot"] != std::stoi(dimm))
            {
                continue;
            }

            ssValue << std::string(slotVal["SlotName"]) << ", ";
        }
    }

    ssKey << imc << " " << channel;
    ssKey << " SLOT: " << dimm;
    ssValue << "Bank: " << bankAddress << ", ";
    ssValue << "Bank group: " << bankGroup << ", ";
    ssValue << "Rank: " << rank << ", ";
    ssValue << "Sub-Rank: " << subRank << ", ";
    ssValue << "Mode: " << *mode << ", ";
    ssValue << "Column: " << column;
    ssValue << ", Row: " << row;
    if (decodedRetryRd.errLog.system_addr_valid)
    {
        ssValue << ", System address: " << address;
    }

    return true;
}

bool readRetryRd(RetryRdSet& decodedRetryRd, std::string registerBdf, RetryRdOffsets registersSetOffsetsBegin,
                 RetryRdOffsets registersSetOffsetsEnd, const json& input)
{
    std::string retryRdErrLogOffset = registerBdf + registersSetOffsetsBegin.errLog + registersSetOffsetsEnd.errLog;
    std::string retryRdErrLogAddress1offset =
        registerBdf + registersSetOffsetsBegin.errLogAddress1 + registersSetOffsetsEnd.errLogAddress1;
    std::string retryRdErrLogAddress2offset =
        registerBdf + registersSetOffsetsBegin.errLogAddress2 + registersSetOffsetsEnd.errLogAddress2;
    std::string retryRdErrLogAddress3offset =
        registerBdf + registersSetOffsetsBegin.errLogAddress3 + registersSetOffsetsEnd.errLogAddress3;

    if (!(input["uncore"].contains(retryRdErrLogOffset) && input["uncore"].contains(retryRdErrLogAddress1offset) &&
          input["uncore"].contains(retryRdErrLogAddress2offset) && input["uncore"].contains(retryRdErrLogAddress3offset)))
    {
        return false;
    }

    if (!(str2uint(input["uncore"][retryRdErrLogOffset], decodedRetryRd.errLog.data) &&
          str2uint(input["uncore"][retryRdErrLogAddress1offset], decodedRetryRd.errLogAddress1.data) &&
          str2uint(input["uncore"][retryRdErrLogAddress2offset], decodedRetryRd.errLogAddress2.data) &&
          str2uint(input["uncore"][retryRdErrLogAddress3offset], decodedRetryRd.errLogAddress3.data)))
    {
        return false;
    }

    if (!decodedRetryRd.errLog.valid)
    {
        return false;
    }

    return true;
}
} // namespace RetryRdIcx

json decodeRetryLogIcx(const json& input, json silkscreenMap, std::string socketId)
{
    RetryRdIcx::RetryRdSet decodedRetryRd;

    json memoryErrors;
    bool uncorErr;

    if (!input.contains("uncore"))
    {
        return memoryErrors;
    }

    std::set<std::pair<std::string, std::string>> correctableErrors;
    std::set<std::pair<std::string, std::string>> uncorrectableErrors;

    for (const auto& [imc, registerBdf] : RetryRdIcx::imcBdfs)
    {
        for (const auto& [channel, registersSetOffsetsBegin] : RetryRdIcx::channelOffsets)
        {
            for (const auto& registersSetOffsetsEnd : RetryRdIcx::setOffsets)
            {
                if (!RetryRdIcx::readRetryRd(decodedRetryRd, registerBdf, registersSetOffsetsBegin, registersSetOffsetsEnd,
                                             input))
                {
                    continue;
                }

                uncorErr = static_cast<bool>(decodedRetryRd.errLog.uc);

                std::stringstream ssKey;
                std::stringstream ssValue;

                if (!RetryRdIcx::decodeSingleRetryRd(ssKey, ssValue, silkscreenMap, decodedRetryRd, socketId, imc, channel))
                {
                    continue;
                }

                if (uncorErr)
                {
                    uncorrectableErrors.insert({ssKey.str(), ssValue.str()});
                }
                else
                {
                    correctableErrors.insert({ssKey.str(), ssValue.str()});
                }
            }
        }
    }

    RetryRdIcx::parseMemoryErrors(correctableErrors, uncorrectableErrors, memoryErrors);

    return memoryErrors;
}
