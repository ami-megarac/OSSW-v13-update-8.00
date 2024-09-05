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
#include "mca_decode_utils.hpp"
#include "mca_defs.hpp"
#include "mca_srf_defs.hpp"
#include "tor_defs_bhs.hpp"
#include "utils.hpp"

#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;

class SrfMcaDecoder : public McaDecoder
{
  public:
    SrfMcaDecoder(const MCAData& mca) : McaDecoder(mca){};

    json decode()
    {
        json entry;
        entry["Core"] = mca.core;
        entry["Module"] = mca.module;
        entry["Bank"] = decodeBankName(mca);
        entry["Status"] = int_to_hex(static_cast<uint64_t>(mca.mc_status));
        entry["Address"] = int_to_hex(static_cast<uint64_t>(mca.address));
        entry["Misc"] = int_to_hex(static_cast<uint64_t>(mca.misc));
        entry["CTL"] = int_to_hex(static_cast<uint64_t>(mca.ctl));
        entry["Status_decoded"] = decode_status();

        return entry;
    }

    virtual json decode_status() = 0;

  protected:
    const std::map<uint8_t, const char*> BankNames = {
        {0x0, "Core BUS"}, {0x1, "Core L2"}, {0x2, "Core FEC"}, {0x3, "Core MEC"}, {0x4, "UBOX"}, {0x5, "UPI"}, {0x6, "Merged Punit"},
        {0x7, "Merged CHA"}, {0x8, "Merged CHA"}, {0x9, "Merged LLC"}, {0xa, "Merged LLC"}, {0xb, "Merged MSE"}, {0xc, "Merged B2CMI"},
    };

    std::string decodeBankName(MCAData mca)
    {
        std::stringstream ss;
        if (mca.type == McaTypeFlag::MSE)
        {
            ss << "MSE" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::MCHN)
        {
            ss << "MCHN" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::B2CMI)
        {
            ss << "B2CMI" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::CBO)
        {
            ss << "CHA" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::LLC)
        {
            ss << "LLC" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::UPI)
        {
            ss << "UPI" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::PUNIT)
        {
            ss << "Punit";
        }
        else
        {
            ss << mca.bank;
            auto bankNameDecoded = getDecoded(BankNames, static_cast<uint8_t>(mca.bank));
            if (bankNameDecoded)
            {
                ss << " (" << *bankNameDecoded << ")";
            }
        }
        return ss.str();
    }
};

class SrfMcaBankFec final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_err_cnt : 15, threshold_based_status : 2, ar : 1,
                s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        struct
        {
            uint64_t reserved1 : 28, mscod_31_28: 4, reserved2 : 32;
        };
        uint64_t mc_status;
    };

    union CTL
    {
        struct
        {
            uint64_t ee : 6, reserved : 58;
        };
        uint64_t ctl_status;
    };

    MC_STATUS status_decoded;
    CTL ctl_decoded;
    json entry;

    void decodeMscod()
    {
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::FEC::decodeMsmcacod, "MSCOD");
        if (entry.contains("MSCOD_decoded"))
        {
            return;
        }
        if (SrfDefs::FEC::decodeMscodLl.count(static_cast<uint8_t>(status_decoded.mscod_31_28)) > 0)
        {
            if(status_decoded.mcacod == 0x151)
            {
                Generic::DecodeField<uint8_t>(entry, status_decoded.mscod_31_28, SrfDefs::FEC::decodeMscodLl, "MSCOD");
                return;
            }
            if(status_decoded.mcacod == 0x181)
            {
                entry["MSCOD_decoded"] = "L1I Corrected Tag Parity Error, Instruction Snoop";
                return;
            }
        }
        if(status_decoded.mcacod == 0x5)
        {
            Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::FEC::decodeMscodArr, "MSCOD");
            if (entry.contains("MSCOD_decoded"))
            {
                return;
            }
        }

        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod_31_28, SrfDefs::FEC::decodeMscod, "MSCOD");
    }

    void decodeKnownFields()
    {
        decodeMscod();
        Generic::SrfBanks::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::FEC::decodeMcacod, true);
        Generic::DecodeField<uint8_t>(entry, ctl_decoded.ee, SrfDefs::FEC::decodeCtl, "EE", "Not implemented");
    }

  public:
    SrfMcaBankFec(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        ctl_decoded.ctl_status = mca.ctl;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_based_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankL2 final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15,
                threshold_base_status : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };

    union CTL
    {
        struct
        {
            uint64_t ee : 5, reserved : 59;
        };
        uint64_t ctl_status;
    };

    MC_STATUS status_decoded;
    CTL ctl_decoded;
    Generic::MC_MISC_WITH_ADDRMODE misc_decoded;
    json entry;

    void decodeMscod()
    {
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::L2::decodeMsmcacod, "MSCOD");
        if (entry.contains("MSCOD_decoded"))
        {
            return;
        }
        if(status_decoded.mcacod == 0x010A)
        {
            Generic::DecodeField<uint8_t>(entry, status_decoded.other_info, SrfDefs::L2::decodeOtherInfo, "MSCOD");
        }
    }

    void decodeKnownFields()
    {
        decodeMscod();
        Generic::SrfBanks::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::L2::decodeMcacod);
        Generic::DecodeField<uint8_t>(entry, ctl_decoded.ee, SrfDefs::L2::decodeCtl, "EE", "Not implemented");
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SrfMcaBankL2(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        ctl_decoded.ctl_status = mca.ctl;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankMec final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15,
                threshold_based_status : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        struct
        {
            uint64_t reserved1 : 28, mscod_31_28: 4, reserved2 : 32;
        };
        uint64_t mc_status;
    };

    union CTL
    {
        struct
        {
            uint64_t ee : 6, reserved : 59;
        };
        uint64_t ctl_status;
    };

    MC_STATUS status_decoded;
    CTL ctl_decoded;
    Generic::MC_MISC_WITH_ADDRMODE misc_decoded;
    json entry;

    void decodeMscod()
    {
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::MEC::decodeMsmcacod, "MSCOD");
        if (entry.contains("MSCOD_decoded"))
        {
            return;
        }
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::MEC::decodeMsmcacodUn, "MSCOD");
        if (entry.contains("MSCOD_decoded"))
        {
            return;
        }

        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod_31_28, SrfDefs::MEC::decodeMscod, "MSCOD");
    }

    void decodeKnownFields()
    {
        decodeMscod();
        Generic::SrfBanks::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::MEC::decodeMcacod);
        Generic::DecodeField<uint8_t>(entry, ctl_decoded.ee, SrfDefs::MEC::decodeCtl, "EE", "Not implemented");
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SrfMcaBankMec(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        ctl_decoded.ctl_status = mca.ctl;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_based_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankBus final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15,
                threshold_base_status : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };

    union CTL
    {
        struct
        {
            uint64_t ee : 14, reserved : 50;
        };
        uint64_t ctl_status;
    };

    union MCACOD
    {
        struct
        {
            uint16_t level : 2, memory_io : 2, request : 4, timeout : 1, partici : 2, bus_interconnect_error : 1, reserved : 4;
        };
        uint16_t mcacod;
    };

    MC_STATUS status_decoded;
    CTL ctl_decoded;
    Generic::MC_MISC_WITH_ADDRMODE misc_decoded;
    json entry;

    void decodeMscod()
    {
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::BUS::decodeMsmcacod, "MSCOD");
        if (entry.contains("MSCOD_decoded"))
        {
            return;
        }
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, SrfDefs::BUS::decodeMscodMcacod, "MSCOD");
    }

    std::string DecodeErrorDetails()
    {
        std::stringstream ss;
        MCACOD mcacodDecoded;
        mcacodDecoded.mcacod = status_decoded.mcacod;

        auto participation = getDecoded(SrfDefs::BUS::decodeParticipation, static_cast<uint8_t>(mcacodDecoded.partici));
        auto request = getDecoded(SrfDefs::BUS::decodeRequest, static_cast<uint8_t>(mcacodDecoded.request));
        auto memoryIo = getDecoded(SrfDefs::BUS::decodeMemoryIo, static_cast<uint8_t>(mcacodDecoded.memory_io));
        auto level = getDecoded(SrfDefs::BUS::decodeLevel, static_cast<uint8_t>(mcacodDecoded.level));

        if (mcacodDecoded.bus_interconnect_error == 1)
        {
            ss << "Bus_Interconnect_Error|";
        }
        if (mcacodDecoded.timeout == 0)
        {
            ss << "Hardware_Error|";
        }

        ss << *participation << "|" << *request << "|" << *memoryIo << "|" << *level;

        return ss.str();
    }


    void decodeMcacod()
    {
        Generic::DecodeField<uint16_t>(entry, status_decoded.error_type, SrfDefs::BUS::decodeMcacod, "MCACOD");
        if (entry.contains("MSCOD_decoded"))
        {
            return;
        }
        entry["MCACOD_decoded"] = DecodeErrorDetails();
    }

    void decodeKnownFields()
    {
        decodeMcacod();
        decodeMscod();
        Generic::DecodeField<uint16_t>(entry, ctl_decoded.ee, SrfDefs::BUS::decodeCtl, "EE", "Not implemented");
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SrfMcaBankBus(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        ctl_decoded.ctl_status = mca.ctl;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankUbox final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15,
                threshold_base_status : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };
    union MC_MISC
    {
        struct
        {
            uint64_t reserved0 : 16, function_log : 3, device_log : 5, bus_log : 8, segment_log : 8, reserved1 : 24;
        };
        uint64_t mc_misc;
    };
    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;
    void decodeKnownFields()
    {
        Generic::IIO::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::UBOX::decodeMcacod, status_decoded.miscv,
                                   misc_decoded.bus_log, misc_decoded.device_log, misc_decoded.function_log,
                                   misc_decoded.segment_log);

        if (status_decoded.mcacod == 0x40c)
        {
            Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::PUNIT::decodeShutdown, "MSCOD");
        }
        else
        {
            Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::UBOX::decodeMscod, "MSCOD");
        }
    }

  public:
    SrfMcaBankUbox(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankUpi final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15,
                threshold_base_status : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t flit_hdr : 20, syndrome_or_l0cbadlanes : 16, illegal_flit_field : 1, phy_state : 3, rx_lane_reversal : 1,
                nerr : 13, reserved_54_55 : 2, cor_err : 8;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    json entry;
    MC_MISC misc_decoded;

    void decodeKnownFields()
    {
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::UPI::decodeMscod, "MSCOD");
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, SrfDefs::UPI::decodeMcacod, "MCACOD");

        Generic::DecodeField<uint8_t>(entry, misc_decoded.cor_err, SrfDefs::UPI::decodeCorrerr, "CORR_ERR", "Not implemented");
        Generic::DecodeField<uint16_t>(entry, misc_decoded.nerr, SrfDefs::UPI::decodeNerr, "NERR", "Not implemented");
        Generic::DecodeField<uint8_t>(entry, misc_decoded.phy_state, SrfDefs::UPI::decodePhystate, "PHY_STATE");
        Generic::UPI::DecodeFithdr(entry, misc_decoded.flit_hdr);
    }

  public:
    SrfMcaBankUpi(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankPunit final : public SrfMcaDecoder
{
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw_upd : 1, corrected_error_count : 15, thrs_err_st : 2, ar : 1,
                s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    Generic::MC_MISC_WITH_ADDRMODE misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, SrfDefs::PUNIT::decodeMcacod, "MCACOD");

        if (status_decoded.mcacod == 0x40c)
        {
            Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SrfDefs::PUNIT::decodeShutdown, "MSCOD");
        }
        else
        {
            Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::PUNIT::decodeMscod, "MSCOD");
        }

        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SrfMcaBankPunit(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW_UPD"] = int_to_hex(static_cast<bool>(status_decoded.fw_upd));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRS_ERR_ST"] = int_to_hex(static_cast<uint8_t>(status_decoded.thrs_err_st));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankMse final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15, threshold_base_status : 2,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    json entry;
    MC_STATUS status_decoded;
    Generic::MC_MISC_WITH_ADDRMODE misc_decoded;

    void decodeKnownFields()
    {
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SrfMcaBankMse(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankB2cmi final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15, threshold_based_status : 2,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t address_lsb : 6, addrmode : 3, hbmmemregion : 1, reserved_10 : 1, errortype : 10, trkid : 9,
                reserved_31_30 : 2, smiopcode : 4, bankindex : 8, mccmdvld : 1, mccmdopcode : 6, mccmdmemregion : 4,
                mccmdchnl : 2, reserved_58_57 : 2, mccmdmirrregion : 1, mccmdmirrsec : 1, mccmdmirrfo : 1, mirrorcorrer : 1,
                mirrorfailover : 1;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::M2M::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::B2CMI::decodeMcacod150, SrfDefs::B2CMI::decodeMcacod157,
                                   SrfDefs::B2CMI::decodeMcacod64, SrfDefs::B2CMI::decodeMcacod30);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::B2CMI::decodeMscod, "MSCOD");

        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeField<uint16_t>(entry, misc_decoded.errortype, SrfDefs::B2CMI::decodeErrortype, "ERRORTYPE", "Unexpected");
        Generic::DecodeField<uint8_t>(entry, misc_decoded.smiopcode, SrfDefs::B2CMI::decodeSmiopcode, "SMIOPCODE", "Unexpected");
        Generic::DecodeField<uint8_t>(entry, misc_decoded.bankindex, SrfDefs::B2CMI::decodeBankindex, "BANKINDEX",
                                      "Not implemented");
        Generic::DecodeField<uint8_t>(entry, misc_decoded.mccmdopcode, SrfDefs::B2CMI::decodeMemopcode, "MCCMDOPCODE",
                                      "Not implemented");
    }

  public:
    SrfMcaBankB2cmi(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_based_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankMcchan final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15, threshold_base_status : 2,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t recoverable_address_lsb : 6, addrmode : 3, column : 10, row : 18, bank_address : 2, bank_group : 3,
                failed_device : 9, subrank : 4, chip_select : 3, ecc_mode : 4, valid : 1, transient : 1;
        };
        uint64_t mc_misc;
    };

    union MCCHAN_MCACOD_DECODE
    {
        struct
        {
            uint16_t reserved1 : 7, data : 3, reserved2 : 6;
        };
        uint16_t mcacod;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void DecodeMcacod()
    {
        MCCHAN_MCACOD_DECODE decode;
        decode.mcacod = status_decoded.mcacod;
        auto mcacod157Decoded = getDecoded<uint8_t>(SrfDefs::MCCHAN::decodeMcacod157, decode.data);
        if (mcacod157Decoded)
        {
            Generic::IMC::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::MCCHAN::decodeErrSpecific, false, *mcacod157Decoded,
                                       "MCCHAN");
        }
        else
        {
            Generic::IMC::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::MCCHAN::decodeErrSpecific, false, "", "MCCHAN");
        }
    }

    void decodeKnownFields()
    {
        DecodeMcacod();
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::MCCHAN::decodeMscod, "MSCOD");
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeField<uint8_t>(entry, misc_decoded.ecc_mode, SrfDefs::MCCHAN::decodeEccmode, "ECC_MODE",
                                      "Invalid ECC mode");
    }

  public:
    SrfMcaBankMcchan(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankLlc final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15, threshold_base_status : 2,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    Generic::MC_MISC_WITH_ADDRMODE misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::LLC::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::LLC::decodeMcacod);

        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SrfDefs::LLC::decodeMscod, "MSCOD");
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SrfMcaBankLlc(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class SrfMcaBankCha final : public SrfMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw : 1, corrected_error_count : 15, threshold_base_status : 2,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved : 34, torid : 5, origreq : 11, reserved2 : 14;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::CHA::DecodeMcacod(entry, status_decoded.mcacod, SrfDefs::CHA::decodeMcacod);
        Generic::CHA::DecodeMscod(entry, status_decoded.mscod, SrfDefs::CHA::decodeMscod, misc_decoded.origreq, BhsOpCodeDecode,
                                  misc_decoded.torid);
    }

  public:
    SrfMcaBankCha(const MCAData& mca) : SrfMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW"] = int_to_hex(static_cast<bool>(status_decoded.fw));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["THRESHOLD_BASED_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.threshold_base_status));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["ADDRV"] = int_to_hex(static_cast<bool>(status_decoded.addrv));
        entry["MISCV"] = int_to_hex(static_cast<bool>(status_decoded.miscv));
        entry["EN"] = int_to_hex(static_cast<bool>(status_decoded.en));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["OVERFLOW"] = int_to_hex(static_cast<bool>(status_decoded.overflow));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};
