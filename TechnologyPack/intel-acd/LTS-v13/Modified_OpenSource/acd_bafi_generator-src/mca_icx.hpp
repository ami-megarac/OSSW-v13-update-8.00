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
#include "mca_icx_defs.hpp"
#include "tor_defs_icx.hpp"
#include "utils.hpp"

#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <sstream>
#include <string>

using json = nlohmann::json;

class IcxMcaDecoder : public McaDecoder
{
  public:
    IcxMcaDecoder(const MCAData& mca) : McaDecoder(mca){};

    json decode()
    {
        json entry;
        entry["Core"] = mca.core;
        entry["Thread"] = mca.thread;
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
        {0x0, "IFU"},
        {0x1, "DCU"},
        {0x2, "DTLB"},
        {0x3, "MLC"},
        {0x4, "PCU"},
        {0x5, "Intel UPI 0"},
        {0x6, "UBOX"},
        {0x7, "Intel UPI 1"},
        {0x8, "Intel UPI 2"},
        {0x9, "CHA_A"},
        {0xA, "CHA_B"},
        {0xB, "CHA_C"},
        {0xC, "M2MEM0"},
        {0xD, "IMC 0, channel 0"},
        {0xE, "IMC 0, channel 1"},
        {0xF, "IMC 0, channel 2"},
        {0x10, "M2MEM1"},
        {0x11, "IMC 1, channel 0"},
        {0x12, "IMC 1, channel 1"},
        {0x13, "IMC 1, channel 2"},
        {0x14, "M2MEM2"},
        {0x15, "IMC 2, channel 0"},
        {0x16, "IMC 2, channel 1"},
        {0x17, "IMC 2, channel 2"},
        {0x18, "M2MEM3"},
        {0x19, "IMC 3, channel 0"},
        {0x1A, "IMC 3, channel 1"},
        {0x1B, "IMC 3, channel 2"},
    };

    std::string decodeBankName(MCAData mca)
    {
        std::stringstream ss;
        if (mca.type == McaTypeFlag::CBO)
        {
            ss << "CHA" << mca.bank;
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

class IcxMcaBankIfu final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 11, enh_mca_avail0 : 5, mscod : 16, reserved_37_32 : 6, corrected_err_cnt : 14, sticky : 1,
                enh_mca_avail2 : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type & 0xFFFF07FF, IcxDefs::IFU::decodeMscod, "MSCOD");
        Generic::IFU::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::IFU::decodeMcacod);
    }

  public:
    IcxMcaBankIfu(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERR_CNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["STICKY"] = int_to_hex(static_cast<bool>(status_decoded.sticky));
        entry["ENH_MCA_AVAIL2"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail2));
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

class IcxMcaBankDcu final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 22, cecnt : 15, enh_mca_avail : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1,
                uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 38, reserved : 26;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeField<uint64_t>(entry, status_decoded.error_type, IcxDefs::DCU::decodeMsmcacod, "MSCOD");
        if (!entry.contains("MSCOD_decoded"))
        {
            Generic::DecodeField<uint32_t>(entry, status_decoded.mscod, IcxDefs::DCU::decodeMscod, "MSCOD");
        }
        Generic::DCU::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::DCU::decodeMcacod);
    }

  public:
    IcxMcaBankDcu(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint32_t>(status_decoded.mscod));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.cecnt));
        entry["ENH_MCA_AVAIL"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail));
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

class IcxMcaBankDtlb final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, enh_mca_avail0 : 6, corrected_error_count : 15, enh_mca_avail1 : 4, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, IcxDefs::DTLB::decodeMscod, "MSCOD");
        Generic::DTLB::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::DTLB::decodeMcacod);
    }

  public:
    IcxMcaBankDtlb(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["ENH_MCA_AVAIL1"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail1));
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

class IcxMcaBankMlc final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, enh_mca_avail0 : 6, corrected_error_count : 15, green_tracking : 1,
                yellow_tracking : 1, enh_mca_avail1 : 2, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, IcxDefs::MLC::decodeMsmacod, "MSCOD");
        if (!entry.contains("MSCOD_decoded"))
        {
            Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, IcxDefs::MLC::decodeMscod, "MSCOD");
        }
        Generic::MLC::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::MLC::decodeMcacod);
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    IcxMcaBankMlc(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["GREEN_TRACKING"] = int_to_hex(static_cast<bool>(status_decoded.green_tracking));
        entry["YELLOW_TRACKING"] = int_to_hex(static_cast<bool>(status_decoded.yellow_tracking));
        entry["ENH_MCA_AVAIL1"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail1));
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

class IcxMcaBankPcu final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mccod : 16, mscod_19_16 : 4, mscod_23_20 : 4, mscod_31_24 : 8, enh_mca_avail0 : 6,
                corrected_error_count : 15, corr_err_sts_ind : 2, enh_mca_avail1 : 2, pcc : 1, addrv : 1, miscv : 1, en : 1,
                uc : 1, overflow : 1, valid : 1;
        };
        struct
        {
            uint64_t error_type : 32, reserved : 32;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mccod, IcxDefs::PCU::decodeMcacod, "MCACOD");

        if (status_decoded.mccod == 0x40c)
        {
            Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, IcxDefs::PCU::decodeShutdown, "MSCOD");
            if (entry.contains("MSCOD_decoded"))
            {
                entry["MSCOD_19_16_decoded"] = entry["MSCOD_decoded"];
            }
            else
            {
                entry["MSCOD_19_16_decoded"] = "";
            }
        }
        else
        {
            Generic::PCU::DecodeMscod(entry, status_decoded.mscod_19_16, IcxDefs::PCU::decodeMscod1916,
                                      status_decoded.mscod_23_20, IcxDefs::PCU::decodeMscod2320, status_decoded.mscod_31_24,
                                      IcxDefs::PCU::decodeMscod3124);
        }
    }

  public:
    IcxMcaBankPcu(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mccod));
        entry["MSEC_UC"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_19_16));
        entry["MSEC_HW"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_23_20));
        entry["MSEC_FW"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_31_24));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_sts_ind));
        entry["ENH_MCA_AVAIL1"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail1));
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

class IcxMcaBankUpi final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod_ll : 2, mcacod_ii : 2, mcacod_rrrr : 4, mcacod_t : 1, mcacod_pp : 2, mcacod_int : 1, mcacod_rsvd : 4,
                mscod_code : 6, mscod_spare : 10, other_info : 6, corrected_error_count : 15, corr_err_sts_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;
    Generic::UPI::MC_MISC misc_decoded;

    void DecodeMcacod(nlohmann::json& entry)
    {
        std::stringstream ss;
        auto mcacodLl = getDecoded<uint8_t>(IcxDefs::UPI::decodeMcacodLl, status_decoded.mcacod_ll);
        if (mcacodLl)
        {
            entry["MCACOD_LL_decoded"] = *mcacodLl;
            ss << *mcacodLl << " | ";
        }

        auto mcacodIi = getDecoded<uint8_t>(IcxDefs::UPI::decodeMcacodIi, status_decoded.mcacod_ii);
        if (mcacodIi)
        {
            entry["MCACOD_II_decoded"] = *mcacodIi;
            ss << *mcacodIi << " | ";
        }

        auto mcacodPp = getDecoded<uint8_t>(IcxDefs::UPI::decodeMcacodPp, status_decoded.mcacod_pp);
        if (mcacodPp)
        {
            entry["MCACOD_PP_decoded"] = *mcacodPp;
            ss << *mcacodPp;
        }

        if (ss.gcount() != 0)
        {
            entry["MCACOD_decoded"] = ss.str();
        }
    }

    void decodeKnownFields()
    {
        DecodeMcacod(entry);
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod_code, IcxDefs::UPI::decodeMscod, "MSCOD");
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::DecodeFieldBits<uint16_t>(entry, status_decoded.mscod_spare, Generic::UPI::decodeMscodSpare, "MSCOD_SPARE");
        Generic::UPI::DecodeMisc(entry, misc_decoded);
    }

  public:
    IcxMcaBankUpi(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod_ll | status_decoded.mcacod_ii << 2 |
                                                           status_decoded.mcacod_rrrr << 4 | status_decoded.mcacod_t << 8 |
                                                           status_decoded.mcacod_pp << 9 | status_decoded.mcacod_int << 11 |
                                                           status_decoded.mcacod_rsvd << 12));
        entry["MCACOD_LL"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_ll));
        entry["MCACOD_II"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_ii));
        entry["MCACOD_RRRR"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_rrrr));
        entry["MCACOD_T"] = int_to_hex(static_cast<bool>(status_decoded.mcacod_t));
        entry["MCACOD_PP"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_pp));
        entry["MCACOD_INT"] = int_to_hex(static_cast<bool>(status_decoded.mcacod_int));
        entry["MCACOD_RSVD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_rsvd));
        entry["MSCOD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_code));
        entry["MSCOD_SPARE"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod_spare));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_sts_ind));
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

class IcxMcaBankIio final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 6, corrected_error_count : 15, corr_err_sts_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved0 : 13, segment_log : 3, function_log : 3, device_log : 5, bus_log : 8, reserved1 : 32;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, IcxDefs::IIO::decodeMscod, "MSCOD");
        Generic::IIO::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::IIO::decodeMcacod, status_decoded.miscv,
                                   misc_decoded.bus_log, misc_decoded.device_log, misc_decoded.function_log,
                                   misc_decoded.segment_log);
    }

  public:
    IcxMcaBankIio(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_sts_ind));
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

class IcxMcaBankCha final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t aggregation : 32, max_instance : 5, health0 : 1, corrected_error_count : 15, raw_cha : 5, ar : 1, s : 1,
                pcc : 1, uc : 1, health1 : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;

    void decodeKnownFields()
    {
        std::optional<std::string> rawChaDecoded;
        if (mca.bank == 9)
        {
            rawChaDecoded = getDecoded<uint8_t>(IcxDefs::CHA::rawChaDecoding9, status_decoded.raw_cha);
        }
        else if (mca.bank == 10)
        {
            rawChaDecoded = getDecoded<uint8_t>(IcxDefs::CHA::rawChaDecoding10, status_decoded.raw_cha);
        }
        else if (mca.bank == 11)
        {
            rawChaDecoded = getDecoded<uint8_t>(IcxDefs::CHA::rawChaDecoding11, status_decoded.raw_cha);
        }
        if (rawChaDecoded)
        {
            entry["INSTANCE_ID_decoded"] = *rawChaDecoded;
        }
    };

  public:
    IcxMcaBankCha(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["AGGREGATION"] = int_to_hex(static_cast<uint32_t>(status_decoded.aggregation));
        entry["MAX_INSTANCE"] = int_to_hex(static_cast<uint8_t>(status_decoded.max_instance));
        entry["HEALTH0"] = int_to_hex(static_cast<bool>(status_decoded.health0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["INSTANCE_ID"] = int_to_hex(static_cast<uint8_t>(status_decoded.raw_cha));
        entry["AR"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["S"] = int_to_hex(static_cast<bool>(status_decoded.s));
        entry["PCC"] = int_to_hex(static_cast<bool>(status_decoded.pcc));
        entry["UC"] = int_to_hex(static_cast<bool>(status_decoded.uc));
        entry["HEALTH1"] = int_to_hex(static_cast<bool>(status_decoded.health1));
        entry["VALID"] = int_to_hex(static_cast<bool>(status_decoded.valid));
        decodeKnownFields();

        return entry;
    }
};

class IcxMcaBankM2m final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 8, mscodddrtype : 2, mscodmiscerrs : 6, other_info : 6, corrected_error_count : 15,
                corr_err_sts_ind : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };
    union MC_MISC
    {
        struct
        {
            uint64_t reserved0 : 6, addrmode : 3, reserved1 : 2, errortype : 10, trkid : 9, reserved2 : 1, smiopcode : 4,
                smimsgclass : 3, smiad : 1, reserved3 : 1, mccmdopcode : 6, mccmdmemregion : 4, reserved4 : 10,
                dataerrorchunk : 2, reserved5 : 2;
        };
        uint64_t mc_misc;
    };

    void DecodeSmiopcode()
    {

        std::string decoded;
        if (misc_decoded.smiad == 1 && misc_decoded.smimsgclass == 0)
        {
            auto smiopcode = getDecoded<uint8_t>(Generic::M2M::decodeSmiopcodeGeneric, misc_decoded.smiopcode);
            if (smiopcode)
            {
                decoded = *smiopcode;
            }
        }
        else if (misc_decoded.smimsgclass == 5)
        {
            auto smiopcode = getDecoded<uint8_t>(IcxDefs::M2M::decodeSmiopcode, misc_decoded.smiopcode);
            if (smiopcode)
            {
                decoded = *smiopcode;
            }
        }
        else
        {
            decoded = "Illegal/Unexpected";
        }

        entry["SMIOPCODE_decoded"] = decoded;
    }

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::M2M::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::M2M::decodeMcacod150, IcxDefs::M2M::decodeMcacod157,
                                   IcxDefs::M2M::decodeMcacod64, IcxDefs::M2M::decodeMcacod30);
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscodddrtype, IcxDefs::M2M::decodeDdrType, "MSCODDDRTYPE");
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod, IcxDefs::M2M::decodeMscod, "MSCOD");

        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeFieldBits<uint16_t>(entry, misc_decoded.errortype, IcxDefs::M2M::decodeErrortype, "ERRORTYPE");
        Generic::M2M::DecodeTrkid(entry, misc_decoded.trkid);
        DecodeSmiopcode();
        Generic::M2M::DecodeMemreg(entry, misc_decoded.mccmdmemregion, IcxDefs::M2M::decodeMemreg);
        Generic::DecodeField<uint8_t>(entry, misc_decoded.mccmdopcode, IcxDefs::M2M::decodeMemopcode, "MCCMDOPCODE");
        Generic::DecodeField<uint8_t>(entry, misc_decoded.dataerrorchunk, IcxDefs::M2M::decodeErrorChunk, "DATAERRORCHUNK");
    }

  public:
    IcxMcaBankM2m(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod));
        entry["MSCODDDRTYPE"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscodddrtype));
        entry["MSCODMISCERRS"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscodmiscerrs));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_sts_ind));
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

class IcxMcaBankImc final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 6, corrected_error_count : 15, corr_err_sts_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    Generic::IMC::MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, IcxDefs::IMC::decodeMscod, "MSCOD");
        Generic::IMC::DecodeMcacod(entry, status_decoded.mcacod & 0xff, IcxDefs::IMC::decodeErrSpecific, false);
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    IcxMcaBankImc(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_sts_ind));
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

class IcxMcaBankCbo final : public IcxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 6, corrected_error_count : 15, corr_err_sts_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved : 39, torid : 5, reserved2 : 1, origreq : 11, reserved3 : 8;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::CHA::DecodeMcacod(entry, status_decoded.mcacod, IcxDefs::CBO::decodeMcacod);
        Generic::CHA::DecodeMscod(entry, status_decoded.mscod, IcxDefs::CBO::decodeMscod, misc_decoded.origreq, IcxOpCodeDecode,
                                  misc_decoded.torid);
    }

  public:
    IcxMcaBankCbo(const MCAData& mca) : IcxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_sts_ind));
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