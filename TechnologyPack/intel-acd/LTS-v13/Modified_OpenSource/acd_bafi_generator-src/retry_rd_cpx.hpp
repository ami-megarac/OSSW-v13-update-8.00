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

namespace RetryRdCpx
{
const std::map<uint8_t, const char*> decodeMode = {{0x0, "sddc 2LM"},     {0x1, "sddc 1LM"},     {0x2, "sddc +1 2LM"},
                                                   {0x3, "sddc +1 1LM"},  {0x4, "adddc 2LM"},    {0x5, "adddc 1LM"},
                                                   {0x6, "adddc +1 2LM"}, {0x7, "adddc +1 1LM"}, {0x8, "read is from ddrt dimm"},
                                                   {0x9, "x8 sddc"},      {0xA, "x8 sddc +1"},   {0xB, "not a valid ecc mode"}};
union RetryRdErrLog
{
    struct
    {
        uint32_t valid : 1, uc : 1, over : 1, mode : 4, reserved : 25;
    };
    uint32_t data = 0;
};

union RetryRdErrLogAddress1
{
    struct
    {
        uint32_t reserved1 : 6, chip_select : 3, cbit : 3, bank : 4, column : 12, reserved2 : 4;
    };
    uint32_t data = 0;
};

union RetryRdErrLogAddress2
{
    struct
    {
        uint32_t row : 21, reserved : 11;
    };
    uint32_t data = 0;
};

struct RetryRdSet
{
    RetryRdErrLog errLog;
    RetryRdErrLogAddress1 errLogAddress1;
    RetryRdErrLogAddress2 errLogAddress2;
};

struct RetryRdOffsets
{
    std::string errLog;
    std::string errLogAddress1;
    std::string errLogAddress2;
};

// IMC 0 Channel 0 registers have BDF = B02_D0a_F3, IMC 0 Channel 1 registers have BDF = B02_D0A_F7, etc.
std::array<std::tuple<std::string, std::string, std::string>, 6> bdfs = {
    std::make_tuple("0", "0", "B02_D0a_F3_"), std::make_tuple("0", "1", "B02_D0a_F7_"), std::make_tuple("0", "2", "B02_D0b_F3_"),
    std::make_tuple("1", "0", "B02_D0c_F3_"), std::make_tuple("1", "1", "B02_D0c_F7_"), std::make_tuple("1", "2", "B02_D0d_F3_")};

// for each set of retry_rd registers' offsets are:
// "0x154" (retry_rd_err_log), "0x15C" (address1), "0x0x11422" (address2)
RetryRdOffsets setOffsets{"0x154", "0x15C", "0x114"};

bool decodeSingleRetryRd(std::stringstream& ssKey, std::stringstream& ssValue, json silkscreenMap, RetryRdSet decodedRetryRd,
                         std::string socketId, std::string imc, std::string channel)
{
    std::optional<std::string> mode = getDecoded(decodeMode, static_cast<uint8_t>(decodedRetryRd.errLog.mode));
    if (!mode)
    {
        return false;
    }

    std::string bankAddress = std::to_string((static_cast<uint8_t>(decodedRetryRd.errLogAddress1.bank) & 0xc) >> 2);
    std::string bankGroup = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.bank & 0x3));
    std::string rank = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.chip_select & 0x3));
    std::string dimm = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.chip_select >> 2));
    std::string subRank = std::to_string(static_cast<uint8_t>(decodedRetryRd.errLogAddress1.cbit));
    std::string column = std::to_string(static_cast<uint16_t>(decodedRetryRd.errLogAddress1.column & 0x3ff) << 2);
    std::string row = std::to_string(static_cast<uint32_t>(decodedRetryRd.errLogAddress2.row));

    if (silkscreenMap != NULL)
    {
        for (const auto& [slotKey, slotVal] : silkscreenMap["MemoryLocationPcb"].items())
        {
            if (slotVal["Socket"] != std::stoi(socketId))
            {
                continue;
            }

            if (slotVal["IMC"] != std::stoi(imc))
            {
                continue;
            }

            if (slotVal["Channel"] != std::stoi(channel))
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

    ssKey << "IMC " << imc << " Channel " << channel;
    ssKey << " SLOT: " << dimm;
    ssValue << "Bank: " << bankAddress << ", ";
    ssValue << "Bank group: " << bankGroup << ", ";
    ssValue << "Rank: " << rank << ", ";
    ssValue << "Sub-Rank: " << subRank << ", ";
    ssValue << "Mode: " << *mode << ", ";
    ssValue << "Column: " << column;
    ssValue << ", Row: " << row;

    return true;
}

bool readRetryRd(RetryRdSet& decodedRetryRd, std::string registerBdf, const json& input)
{
    std::string retryRdErrLogOffset = registerBdf + setOffsets.errLog;
    std::string retryRdErrLogAddress1offset = registerBdf + setOffsets.errLogAddress1;
    std::string retryRdErrLogAddress2offset = registerBdf + setOffsets.errLogAddress2;

    if (!(input["uncore"].contains(retryRdErrLogOffset) && input["uncore"].contains(retryRdErrLogAddress1offset) &&
          input["uncore"].contains(retryRdErrLogAddress2offset)))
    {
        return false;
    }

    if (!(str2uint(input["uncore"][retryRdErrLogOffset], decodedRetryRd.errLog.data) &&
          str2uint(input["uncore"][retryRdErrLogAddress1offset], decodedRetryRd.errLogAddress1.data) &&
          str2uint(input["uncore"][retryRdErrLogAddress2offset], decodedRetryRd.errLogAddress2.data)))
    {
        return false;
    }

    if (!decodedRetryRd.errLog.valid)
    {
        return false;
    }

    return true;
}
} // namespace RetryRdCpx

json decodeRetryLogCpx(const json& input, json silkscreenMap, std::string socketId)
{
    RetryRdCpx::RetryRdSet decodedRetryRd;

    json memoryErrors;
    bool uncorErr;

    if (!input.contains("uncore"))
    {
        return memoryErrors;
    }

    for (const auto& [imc, channel, registerBdf] : RetryRdCpx::bdfs)
    {
        if (!RetryRdCpx::readRetryRd(decodedRetryRd, registerBdf, input))
        {
            continue;
        }

        uncorErr = static_cast<bool>(decodedRetryRd.errLog.uc);

        std::stringstream ssKey;
        std::stringstream ssValue;

        if (!RetryRdCpx::decodeSingleRetryRd(ssKey, ssValue, silkscreenMap, decodedRetryRd, socketId, imc, channel))
        {
            continue;
        }

        if (uncorErr)
        {
            memoryErrors["Memory_errors"]["Uncorrectable"][ssKey.str()] = ssValue.str();
        }
        else
        {
            memoryErrors["Memory_errors"]["Correctable"][ssKey.str()] = ssValue.str();
        }
    }

    return memoryErrors;
}