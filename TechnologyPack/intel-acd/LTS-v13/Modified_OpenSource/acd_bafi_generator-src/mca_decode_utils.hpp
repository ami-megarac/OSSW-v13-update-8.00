#pragma once
#include <stdint.h>

#include <limits>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include "utils.hpp"
#include <vector>

namespace Generic
{
union MC_MISC_WITH_ADDRMODE
{
    struct
    {
        uint64_t reserved1 : 6, addrmode : 3, reserved2 : 55;
    };
    uint64_t mc_misc;
};
namespace
{ // anonymous namespace to make generic decodes accessible only from bank-specific namespaces
union McacodDataGeneric1
{
    struct
    {
        uint16_t mcacod_1_0 : 2, mcacod_3_2 : 2, mcacod_7_4 : 4, mcacod_8_8 : 1, reserved : 7;
    };
    uint16_t mcacod_15_0;
};

const std::map<uint8_t, const char *> decodeAddrmode = {
    {0x0, "Segment Offset"}, {0x1, "Linear Address"}, {0x2, "Physical Address"}, {0x3, "Memory Address"},
    {0x4, "Reserved"},       {0x5, "Reserved"},       {0x6, "Reserved"},         {0x7, "Generic"},
};

const std::map<uint8_t, const char *> decodeCorrErrStsInd = {
    {0x0, "No hardware status tracking is provided for the structure "
          "reporting this event"},
    {0x1, "Green - The current status is green (below threshold) for the "
          "structure posting the event"},
    {0x2, "Yellow - The current status is yellow (above threshold) for the "
          "structure posting the event"},
    {0x3, "Reserved"}};

const std::map<uint8_t, const char *> decodeErrorRequestType = {
    {0x0, "GENERIC_ERROR"}, {0x1, "GENERIC_READ"},      {0x2, "GENERIC_WRITE"}, {0x3, "DATA_READ"},
    {0x4, "DATA_WRITE"},    {0x5, "INSTRUCTION_FETCH"}, {0x6, "PREFETCH"},      {0x7, "EVICTION"},
    {0x8, "SNOOP"},         {0x9, "RESERVED"},          {0xa, "RESERVED"},      {0xb, "RESERVED"},
    {0xc, "RESERVED"},      {0xd, "RESERVED"},          {0xe, "RESERVED"},      {0xf, "RESERVED"},
};

const std::map<uint8_t, const char *> decodeErrorLevelEncoding = {
    {0x0, "LEVEL_0 (core L1 cache)"},
    {0x1, "LEVEL_1 (core L2 cache)"},
    {0x2, "LEVEL_2 (L3/LLC cache)"},
    {0x3, "GENERIC_ERROR_LEVEL_ENCODING"},
};

const std::map<uint8_t, const char *> decodeErrorTransactionType = {
    {0x0, "INSTRUCTION"},
    {0x1, "DATA"},
    {0x2, "GENERIC_ERROR_TRANSACTION_TYPE"},
    {0x3, "RESERVED"},
};

std::string DecodeErrorDetails(uint16_t mcacod, bool decode_only_cache_hierarchy_err = false)
{
    std::stringstream ss;
    McacodDataGeneric1 mcacodDecoded;
    mcacodDecoded.mcacod_15_0 = mcacod;

    if (decode_only_cache_hierarchy_err && mcacodDecoded.mcacod_8_8 != 1)
    {
        return "---";
    }

    auto errorLevelEncoding = getDecoded(decodeErrorLevelEncoding, static_cast<uint8_t>(mcacodDecoded.mcacod_1_0));
    auto errorTransactionType = getDecoded(decodeErrorTransactionType, static_cast<uint8_t>(mcacodDecoded.mcacod_3_2));
    auto errorRequestType = getDecoded(decodeErrorRequestType, static_cast<uint8_t>(mcacodDecoded.mcacod_7_4));

    if (mcacodDecoded.mcacod_8_8 == 1)
    {
        ss << "Cache_Hierarchy_Error|";
    }

    ss << *errorRequestType << "|" << *errorTransactionType << "|" << *errorLevelEncoding;

    return ss.str();
}

void DecodeMcacodGeneric(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod,
                         bool decode_only_cache_hierarchy_err, bool check_filtered = false, bool check_tlberr = false)
{
    auto mcacodDecoded = getDecoded(decodeMcacod, mcacod);
    if (mcacodDecoded)
    {
        entry["MCACOD_decoded"] = *mcacodDecoded;
    }
    else
    {
        std::string decoded = DecodeErrorDetails(mcacod, decode_only_cache_hierarchy_err);
        if (check_filtered && (mcacod & 0x1000) > 0)
        {
            decoded.append("|(FILTERED)");
        }
        /*Reason for False Positive - this is the debug code.*/
        /* coverity[result_independent_of_operands : FALSE] */
        if (check_tlberr && ((mcacod & 0x1F0) == 1))
        {
            decoded.append("|TLB_Error");
        };
        entry["MCACOD_decoded"] = decoded;
    }
}
} // namespace

template <typename T>
void DecodeField(nlohmann::json &entry, T field, const std::map<T, const char *> decodeField, const char *fieldName,
                 const char *defaultValue = nullptr)
{
    std::stringstream entryNodeName;
    entryNodeName << fieldName << "_decoded";
    auto fieldDecoded = getDecoded(decodeField, field);
    if (fieldDecoded)
    {
        entry[entryNodeName.str()] = *fieldDecoded;
    }
    else if (defaultValue != nullptr)
    {
        entry[entryNodeName.str()] = *defaultValue;
    }
}

template <typename T>
void DecodeFieldBits(nlohmann::json &entry, T field, const std::map<std::vector<uint8_t>, const char *> decodeField,
                     const char *fieldName)
{
    std::stringstream entryNodeName;
    entryNodeName << fieldName << "_decoded";
    std::string fieldDecoded = DecodeBits(decodeField, field);
    entry[entryNodeName.str()] = fieldDecoded;
}

void DecodeAddrmode(nlohmann::json &entry, uint8_t addrmode)
{
    auto addrmodeDecoded = getDecoded(decodeAddrmode, static_cast<uint8_t>(addrmode));

    if (addrmodeDecoded)
    {
        entry["ADDRMODE"] = *addrmodeDecoded;
    }
}

void DecodeCorrErrStsInd(nlohmann::json &entry, uint8_t status)
{
    DecodeField(entry, status, decodeCorrErrStsInd, "CORR_ERR_STATUS_IND");
}

namespace IFU
{
void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod)
{
    DecodeMcacodGeneric(entry, mcacod, decodeMcacod, false);
}
} // namespace IFU

namespace SrfBanks
{
//MEC, FEC, L2
void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod, bool decodeTlbErr = false)
{
    DecodeMcacodGeneric(entry, mcacod, decodeMcacod, false, false, decodeTlbErr);
}
} // namespace SrfBanks

namespace DCU = IFU;

namespace MLC
{
void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod)
{
    DecodeMcacodGeneric(entry, mcacod, decodeMcacod, true);
}

} // namespace MLC

namespace DTLB
{
union Mcacod
{
    struct
    {
        uint16_t tlb_err_ll : 2, tlb_err_tt : 2, tlb_err : 1, reserved : 11;
    };
    uint16_t mcacod_15_0;
};

union McacodSimpleErr
{
    struct
    {
        uint16_t simple_error : 3, reserved : 13;
    };
    uint16_t mcacod_15_0;
};

const std::map<uint8_t, const char *> decodeSimpleError = {
    {0x0, "No error"},       {0x1, "Unclassified error"},     {0x2, "Microcode ROM Parity Error"},
    {0x3, "External Error"}, {0x4, "FRC Error (Unexpected)"}, {0x5, "Internal Parity Error"},
    {0x6, "Simple Error"},   {0x7, "Simple Error"},
};

const std::map<uint8_t, const char *> decodeTlbErrTt = {
    {0x0, "Instruction"},
    {0x1, "Data"},
    {0x2, "Generic"},
    {0x3, "Reserved"},
};

const std::map<uint8_t, const char *> decodeTlbErrLl = {
    {0x0, "Level 0"},
    {0x1, "Level 1"},
    {0x2, "Level 2"},
    {0x3, "Generic"},
};

std::string DecodedMcacodGeneric(uint16_t mcacod)
{
    std::stringstream ss;
    Mcacod mcacodDecoded;
    mcacodDecoded.mcacod_15_0 = mcacod;

    if (mcacodDecoded.tlb_err == 1)
    {
        auto tlbErrLl = getDecoded<uint8_t>(decodeTlbErrLl, mcacodDecoded.tlb_err_ll);
        auto tlbErrTt = getDecoded<uint8_t>(decodeTlbErrTt, mcacodDecoded.tlb_err_tt);

        ss << "TLB_ERR|" << *tlbErrTt << "|" << *tlbErrLl;
    }
    else
    {
        McacodSimpleErr mcacodSimpleDecoded;
        mcacodSimpleDecoded.mcacod_15_0 = mcacod;
        auto simpleErr = getDecoded<uint8_t>(decodeSimpleError, mcacodSimpleDecoded.simple_error);
        ss << *simpleErr;
    }

    return ss.str();
}

void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod)
{
    auto mcacodDecoded = getDecoded(decodeMcacod, mcacod);
    if (mcacodDecoded)
    {
        entry["MCACOD_decoded"] = *mcacodDecoded;
    }
    else
    {
        entry["MCACOD_decoded"] = Generic::DTLB::DecodedMcacodGeneric(mcacod);
    }
}

} // namespace DTLB

namespace PCU
{
void DecodeMscod(nlohmann::json &entry, uint8_t mscod1916, const std::map<uint8_t, const char *> decodeMscod1916,
                 uint8_t mscod2320, const std::map<uint8_t, const char *> decodeMscod2320, uint8_t mscod3124,
                 const std::map<uint8_t, const char *> decodeMscod3124)
{
    std::stringstream ss;
    Generic::DecodeField<uint8_t>(entry, mscod1916, decodeMscod1916, "MSCOD_19_16");
    if (entry.contains("MSCOD_19_16_decoded"))
    {
        ss << entry["MSCOD_19_16_decoded"] << " | ";
    }

    Generic::DecodeField<uint8_t>(entry, mscod2320, decodeMscod2320, "MSCOD_23_20");
    if (entry.contains("MSCOD_23_20_decoded"))
    {
        ss << entry["MSCOD_23_20_decoded"] << " | ";
    }

    Generic::DecodeField<uint8_t>(entry, mscod3124, decodeMscod3124, "MSCOD_31_24");
    if (entry.contains("MSCOD_31_24_decoded"))
    {
        ss << entry["MSCOD_31_24_decoded"] << " | ";
    }
    else
    {
        entry["MSCOD_31_24_decoded"] = "Internal error";
        ss << "Internal error";
    }

    entry["MSCOD_decoded"] = ss.str();
}
} // namespace PCU

namespace UPI
{
union MC_MISC
{
    struct
    {
        uint64_t flit_hdr : 20, syndrome_or_l0cbadlanes : 16, illegal_flit_field : 1, phy_state : 3, rx_lane_reversal : 1,
            reserved : 1, nerr : 12, cor_err : 10;
    };
    uint64_t mc_misc;
};

std::map<std::vector<uint8_t>, const char *> decodeMscodSpare = {
    {{9}, "PHY Control Error"},
    {{8}, "Unexpected Retry.Ack flit"},
    {{7}, "Unexpected Retry.Req flit"},
    {{6}, "RF parity error"},
    {{5}, "Routeback Table Error"},
    {{4}, "Unexpected Tx Protocol Flit (EOP, Header or Data)"},
    {{3}, "Rx Header or Credit BGF credit overflow/underflow"},
    {{2}, "Link Layer Reset still in progress"},
    {{1}, "Link Layer reset initiated while protocol traffic not idle"},
    {{0}, "Link Layer Tx Parity Error"},
};

const std::map<uint8_t, const char *> decodePhystate = {
    {0x0, "20 lane flit mux phase 0"},
    {0x1, "20 lane flit mux phase 1"},
    {0x2, "20 lane flit mux phase 2"},
    {0x3, "20 lane flit mux phase 3"},
    {0x4, "20 lane flit mux phase 4"},
    {0x5, "Lower 8 logical lanes active in L0 (failover)"},
    {0x6, "Upper 8 logical lanes active in L0 (failover)"},
    {0x7, "Lower 8 logical lane active in L0p"},
};

std::map<std::vector<uint8_t>, const char *> decodeNerr = {
    {{11}, "UC Phy Initialization Failure"},
    {{10}, "UC Phy Detected Drift Buffer Alarm"},
    {{9}, "UC Phy Detected Latency Buffer Rollover"},
    {{8}, "Reserved"},
    {{7}, "Reserved"},
    {{6}, "UC LL Rx detected CRC error: unsuccessful LLR"},
    {{5}, "UC LL Rx Unsupported/Undefined packet"},
    {{4}, "UC LL or Phy Control Error"},
    {{3}, "UC LL Rx Parameter Exception"},
    {{2}, "Reserved"},
    {{1}, "Reserved"},
    {{0}, "UC LL Detected Control Error from M3UPI Correctable (COR)"},
};

std::map<std::vector<uint8_t>, const char *> decodeCorErr = {
    {{9}, "COR Phy Initialization Abort"},
    {{8}, "COR Phy Reset"},
    {{7}, "COR Phy Lane failure"},
    {{6}, "COR Phy L0c error corrected without Phy reset"},
    {{5}, "COR Phy L0c error triggering Phy Reset"},
    {{4}, "COR Phy L0p exit error triggering Phy Reset"},
    {{3}, "Reserved"},
    {{2}, "COR LL Rx detected CRC error: successful LLR without Phy Reinit"},
    {{1}, "COR LL Rx detected CRC error: successful LLR with Phy Reinit"},
    {{0}, "Reserved"},
};

std::map<std::vector<uint8_t>, const char *> decodeFithdr = {
    {{19}, "HIB"}, {{18}, "VN"}, {{17}, "VNA"}, {{16}, "ACK"}, {{15}, "CRD"},
};

std::map<std::vector<uint8_t>, const char *> decodeFithdrSlot = {
    {{14, 13, 12, 11}, "Slot 0 Opcode"}, {{10, 9, 8}, "Slot 0 MC"}, {{7, 6, 5, 4}, "Slot 1 Opcode"}, {{3}, "Slot 1 MC"},
    {{2, 1, 0}, "Slot 2 Opcode"},
};

void DecodeFithdr(nlohmann::json &entry, uint32_t flit_hdr)
{
    std::string flithdr = DecodeBits<uint32_t>(decodeFithdr, flit_hdr);
    if (flit_hdr & 1 << 19)
    {
        flithdr += DecodeBits<uint32_t>(decodeFithdrSlot, flit_hdr);
    }

    entry["FLIT_HDR_decoded"] = flithdr;
}

void DecodeMisc(nlohmann::json &entry, UPI::MC_MISC misc_decoded)
{
    entry["COR_ERR_decoded"] = DecodeBits<uint16_t>(decodeCorErr, misc_decoded.cor_err);

    entry["NERR_decoded"] = DecodeBits<uint16_t>(decodeNerr, misc_decoded.nerr);

    DecodeFithdr(entry, misc_decoded.flit_hdr);

    Generic::DecodeField<uint8_t>(entry, misc_decoded.phy_state, decodePhystate, "PHY_STATE");
}
} // namespace UPI

namespace IIO
{
void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod, uint8_t miscv,
                  uint8_t busLog, uint8_t deviceLog, uint8_t functionLog, uint8_t segmentLog)
{
    auto mcacodDecoded = getDecoded(decodeMcacod, static_cast<uint16_t>(mcacod));

    if (mcacodDecoded)
    {
        std::stringstream ss;
        ss << *mcacodDecoded;
        if (miscv)
        {
            ss << " on Bus[" << int_to_hex(busLog) << "] Device[" << int_to_hex(deviceLog) << "] Function["
               << int_to_hex(functionLog) << "] from Segment[" << int_to_hex(segmentLog) << "]";
        }
        entry["MCACOD_decoded"] = ss.str();
    }
}
} // namespace IIO

namespace M2M
{
const std::map<uint8_t, const char *> decodeSmiopcodeGeneric = {
    {0x0, "MemRd"},     {0x1, "MemSpecRd"}, {0x2, "MemRdData"},  {0x4, "MemRdXtoS"},  {0x5, "MemRdXtoI"},
    {0x6, "MemRdXtoA"}, {0x8, "MemInv"},    {0x9, "MemInvXtoI"}, {0xA, "MemInvXtoA"}, {0xC, "MemInvItoX"},
};

void DecodeTrkid(nlohmann::json &entry, uint16_t trkid)
{
    std::stringstream ss;
    uint16_t phyChn = ((trkid & 0xC0) >> 6);
    uint16_t htid = (((trkid & 0x100) >> 8) << 5) | (trkid & 0x3F);
    ss << "Physical Channel = " << phyChn << ", htid = " << htid;
    entry["TRKID_decoded"] = ss.str();
}

void DecodeMemreg(nlohmann::json &entry, uint8_t memreg, const std::map<uint8_t, const char *> decodeMemoregAccess)
{
    std::stringstream ss;

    if ((memreg & 0x1) == 1)
    {
        ss << "Block region|";
    }
    /*Reason for False Positive - this is the debug code.*/
    /* coverity[result_independent_of_operands : FALSE] */
    if ((memreg & 0x2) == 1)
    {
        ss << "PMEM region|";
    }
    uint8_t bits_3_2 = (memreg & 0xC) >> 2;

    auto access = getDecoded(decodeMemoregAccess, bits_3_2);
    ss << *access << "|";

    entry["MCCMDMEMREGION"] = ss.str();
}

void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, std::map<uint16_t, const char *> decodeMcacod150,
                  std::map<uint8_t, const char *> decodeMcacod157, std::map<uint8_t, const char *> decodeMcacod64,
                  std::map<uint8_t, const char *> decodeMcacod30)
{
    std::stringstream ss;

    union mcacodData
    {
        struct
        {
            uint16_t mcacod_3_0 : 4, mcacod_6_4 : 3, mcacod_15_7 : 9;
        };
        uint16_t mcacod_15_0;
    } mcacodDecoded;

    mcacodDecoded.mcacod_15_0 = mcacod;

    auto mcacod150 = getDecoded(decodeMcacod150, static_cast<uint16_t>(mcacodDecoded.mcacod_15_0));

    if (mcacod150)
    {
        ss << *mcacod150 << "|";
    }
    else
    {
        auto mcacod157 = getDecoded(decodeMcacod157, static_cast<uint8_t>(mcacodDecoded.mcacod_15_7));
        if (mcacod157)
        {
            ss << *mcacod157 << "|";
            auto mcacod64 = getDecoded(decodeMcacod64, static_cast<uint8_t>(mcacodDecoded.mcacod_6_4));
            auto mcacod30 = getDecoded(decodeMcacod30, static_cast<uint8_t>(mcacodDecoded.mcacod_3_0));
            if (mcacod64)
            {
                ss << *mcacod64 << "|";
            }
            if (mcacod30)
            {
                ss << *mcacod30;
            }
        }
    }

    entry["MCACOD_decoded"] = ss.str();
}
} // namespace M2M

namespace IMC
{

union McacodData
{
    struct
    {
        uint16_t channel : 4, err : 3, imc_err : 1, reserved0 : 8;
    };
    struct
    {
        uint16_t err_specific : 8, reserved1 : 8;
    };

    uint16_t mcacod_15_0;
};

union MC_MISC
{
    struct
    {
        uint64_t reserved : 6, addrmode : 3, extra_err_info : 55;
    };
    uint64_t mc_misc;
};

const std::map<uint8_t, const char *> decodeChannel = {
    {0x0, "Channel_0"},  {0x1, "Channel_1"},  {0x2, "Channel_2"},  {0x3, "Channel_3"},
    {0x4, "Channel_4"},  {0x5, "Channel_5"},  {0x6, "Channel_6"},  {0x7, "Channel_7"},
    {0x8, "Channel_8"},  {0x9, "Channel_9"},  {0xa, "Channel_10"}, {0xb, "Channel_11"},
    {0xc, "Channel_12"}, {0xd, "Channel_13"}, {0xe, "Channel_14"}, {0xf, "Channel_not_specified"},
};

const std::map<uint8_t, const char *> decodeError = {
    {0x0, "Generic undefined request"}, {0x1, "Memory Read Error"},      {0x2, "Memory Write Error"},
    {0x3, "Address/Command Error"},     {0x4, "Memory Scrubbing Error"},
};

void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, std::map<uint16_t, const char *> decodeErrSpecific, bool hbm,
                  std::string additionalInfo = "", std::string bankName = "IMC")
{
    std::stringstream ss;
    McacodData mcacodDecoded;
    mcacodDecoded.mcacod_15_0 = mcacod;

    auto errorSpecific = getDecoded<uint16_t>(decodeErrSpecific, mcacodDecoded.err_specific);

    if (errorSpecific)
    {
        entry["MCACOD_decoded"] = *errorSpecific;
        return;
    }

    if (mcacodDecoded.imc_err == 1)
    {
        ss << bankName << "_Error" << additionalInfo << "|";

        auto channel = getDecoded(decodeChannel, static_cast<uint8_t>(mcacodDecoded.channel));

        auto error = getDecoded(decodeError, static_cast<uint8_t>(mcacodDecoded.err));

        if (error)
        {
            ss << *error << "|";
        }
        if (channel)
        {
            if (*channel == "Channel_14" && hbm)
            {
                ss << "HBM";
            }
            else
            {
                ss << *channel;
            }
        }
    }
    else
    {
        ss << "Reserved";
    }

    entry["MCACOD_decoded"] = ss.str();
}
} // namespace IMC

namespace CHA
{
void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod)
{
    DecodeMcacodGeneric(entry, mcacod, decodeMcacod, true, true);
}

void DecodeMscod(nlohmann::json &entry, uint16_t mscod, const std::map<uint16_t, const char *> decodeMscod, uint32_t origreq,
                 const std::map<uint32_t, const char *> decodeOpCode, uint8_t torid)
{
    auto mscodDecoded = getDecoded<uint16_t>(decodeMscod, mscod);
    if (mscodDecoded)
    {
        std::stringstream ss;
        ss << *mscodDecoded;
        auto origreqDecoded = getDecoded(decodeOpCode, origreq);
        if (origreqDecoded)
        {
            ss << "_OriginalReq[" << *origreqDecoded << "]_TorID[" << static_cast<uint32_t>(torid) << "]";
        }

        entry["MSCOD_decoded"] = ss.str();
    }
}

} // namespace CHA

namespace LLC
{
void DecodeMcacod(nlohmann::json &entry, uint16_t mcacod, const std::map<uint16_t, const char *> decodeMcacod)
{
    DecodeMcacodGeneric(entry, mcacod, decodeMcacod, true, true);
}
} // namespace LLC
} // namespace Generic
