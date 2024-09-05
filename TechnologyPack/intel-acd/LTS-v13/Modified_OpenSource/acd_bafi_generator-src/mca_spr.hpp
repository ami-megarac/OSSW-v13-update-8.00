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
#include "mca_spr_defs.hpp"
#include "tor_defs_spr.hpp"
#include "utils.hpp"
#include "mca_decode_utils.hpp"
#include "mca_spr_defs.hpp"

#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;

class SprMcaDecoder : public McaDecoder
{
  public:
    SprMcaDecoder(const MCAData& mca) : McaDecoder(mca){};

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
        {0x0, "Core IFU"},
        {0x1, "Core DCU"},
        {0x2, "Core DTLB"},
        {0x3, "Core MLC"},
        {0x4, "PCU"},
        {0x6, "IIO"},
        {0x9, "CHA_A"},
        {0xA, "CHA_B"},
        {0xB, "CHA_C"},
        {0xD, "IMC 0, channel 0"},
        {0xE, "IMC 0, channel 1"},
        {0xF, "IMC 1, channel 0"},
        {0x10, "IMC 1, channel 1"},
        {0x11, "IMC 2, channel 0"},
        {0x12, "IMC 2, channel 1"},
        {0x13, "IMC 3, channel 0"},
        {0x14, "IMC 3, channel 1"},
    };

    std::string decodeBankName(MCAData mca)
    {
        std::stringstream ss;
        if (mca.type == McaTypeFlag::CBO)
        {
            ss << "CHA" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::MDF)
        {
            ss << "MDF" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::M2M)
        {
            ss << "M2MEM" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::UPI)
        {
            ss << "UPI" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::HBM_M2M)
        {
            ss << "HBM_M2MEM" << mca.bank;
        }
        else if (mca.type == McaTypeFlag::HBM_IMC)
        {
            ss << "HBM_IMC_CHDP" << mca.bank;
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

    union IMC_MCACOD_DECODE
    {
        struct
        {
            uint16_t reserved1 : 7, data : 3, reserved2 : 6;
        };
        uint16_t mcacod;
    };

    void ImcDecodeMcacod(nlohmann::json& entry, uint16_t mcacod, bool hbm)
    {
        IMC_MCACOD_DECODE decode;
        decode.mcacod = mcacod;
        auto mcacod157Decoded = getDecoded<uint8_t>(SprDefs::IMC::decodeMcacod157, decode.data);
        if (mcacod157Decoded)
        {
            Generic::IMC::DecodeMcacod(entry, mcacod & 0xff, SprDefs::IMC::decodeErrSpecific, hbm, *mcacod157Decoded);
        }
        else
        {
            Generic::IMC::DecodeMcacod(entry, mcacod & 0xff, SprDefs::IMC::decodeErrSpecific, hbm);
        }
    }
};

class SprMcaBankIfu final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 11, reserved0 : 5, mscod : 16, reserved1 : 17, corrected_err_cnt : 14, reserved2 : 3, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type & 0xFFFF07FF, SprDefs::IFU::decodeMscod, "MSCOD");
        Generic::IFU::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::IFU::decodeMcacod);
    }

  public:
    SprMcaBankIfu(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
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

class SprMcaBankDcu final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 22, corrected_err_cnt : 15, enh_mca_avail : 2, ar : 1, s : 1, pcc : 1, addrv : 1,
                miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeField<uint64_t>(entry, status_decoded.error_type, SprDefs::DCU::decodeMsmcacod, "MSCOD");
        if (!entry.contains("MSCOD_decoded"))
        {
            Generic::DecodeField<uint32_t>(entry, status_decoded.mscod, SprDefs::DCU::decodeMscod, "MSCOD");
        }
        Generic::DCU::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::DCU::decodeMcacod);
    }

  public:
    SprMcaBankDcu(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint32_t>(status_decoded.mscod));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
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

class SprMcaBankDtlb final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, enh_mca_avail0 : 6, corrected_err_cnt : 15, enh_mca_avail1 : 4, pcc : 1, addrv : 1,
                miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SprDefs::DTLB::decodeMsmcacod, "MSCOD");
        Generic::DTLB::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::DTLB::decodeMcacod);
    }

  public:
    SprMcaBankDtlb(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
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

class SprMcaBankMlc final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, enh_mca_avail0 : 6, corrected_err_cnt : 15, green_tracking : 1, yellow_tracking : 1,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        uint32_t msmcacod = status_decoded.error_type;

        // Special case: SPR upper 16 bits may be different than 0x80 code
        // and lower 16 bits is 0x0400 then change to msmcacod
        // hardcode value
        if ((msmcacod & 0xffff) == 0x400)
        {
            msmcacod = 0x00800400;
        }

        Generic::DecodeField<uint32_t>(entry, msmcacod, SprDefs::MLC::decodeMsmacod, "MSCOD");
        if (!entry.contains("MSCOD_decoded"))
        {
            Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SprDefs::MLC::decodeMscod, "MSCOD");
        }
        Generic::MLC::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::MLC::decodeMcacod);
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    SprMcaBankMlc(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["GREEN_TRACKING"] = int_to_hex(static_cast<bool>(status_decoded.green_tracking));
        entry["YELLOW_TRACKING"] = int_to_hex(static_cast<bool>(status_decoded.yellow_tracking));
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

class SprMcaBankPcu final : public SprMcaDecoder
{
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod_19_16 : 4, mscod_23_20 : 4, mscod_31_24 : 8, other_info : 5, fw_upd : 1,
                corrected_err_cnt : 15, thrs_err_st : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1,
                overflow : 1, valid : 1;
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
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, SprDefs::PCU::decodeMcacod, "MCACOD");

        if (status_decoded.mcacod == 0x40c)
        {
            entry["MSCOD_23_20_decoded"] = "";
            entry["MSCOD_31_24_decoded"] = "";
            Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, SprDefs::PCU::decodeShutdown, "MSCOD");
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
            Generic::PCU::DecodeMscod(entry, status_decoded.mscod_19_16, SprDefs::PCU::decodeMscod1916,
                                      status_decoded.mscod_23_20, SprDefs::PCU::decodeMscod2320, status_decoded.mscod_31_24,
                                      SprDefs::PCU::decodeMscod3124);
        }
    }

  public:
    SprMcaBankPcu(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod_19_16 | status_decoded.mscod_23_20 << 4 |
                                                          status_decoded.mscod_31_24 << 8));
        entry["MSCOD_19_16"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_19_16));
        entry["MSCOD_23_20"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_23_20));
        entry["MSCOD_31_24"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_31_24));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW_UPD"] = int_to_hex(static_cast<bool>(status_decoded.fw_upd));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
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

class SprMcaBankUpi final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod_ll : 2, mcacod_ii : 2, mcacod_rrrr : 4, mcacod_t : 1, mcacod_pp : 2, mcacod_int : 1, mcacod_rsvd : 4,
                mscod : 6, mscod_spare : 10, other_info : 5, fw_upd : 1, corrected_err_cnt : 15, corr_err_status_ind : 2, ar : 1,
                s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;
    Generic::UPI::MC_MISC misc_decoded;

    void DecodeMcacod(nlohmann::json& entry)
    {
        std::stringstream ss;
        auto mcacodLl = getDecoded<uint8_t>(SprDefs::UPI::decodeMcacodLl, status_decoded.mcacod_ll);
        if (mcacodLl)
        {
            entry["MCACOD_LL_decoded"] = *mcacodLl;
            ss << *mcacodLl << " | ";
        }

        auto mcacodIi = getDecoded<uint8_t>(SprDefs::UPI::decodeMcacodIi, status_decoded.mcacod_ii);
        if (mcacodIi)
        {
            entry["MCACOD_II_decoded"] = *mcacodIi;
            ss << *mcacodIi << " | ";
        }

        auto mcacodPp = getDecoded<uint8_t>(SprDefs::UPI::decodeMcacodPp, status_decoded.mcacod_pp);
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
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod, SprDefs::UPI::decodeMscod, "MSCOD");
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeFieldBits<uint16_t>(entry, status_decoded.mscod_spare, Generic::UPI::decodeMscodSpare, "MSCOD_SPARE");
        Generic::UPI::DecodeMisc(entry, misc_decoded);
    }

  public:
    SprMcaBankUpi(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
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
        entry["MCACOD_T"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_t));
        entry["MCACOD_PP"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_pp));
        entry["MCACOD_INT"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_int));
        entry["MCACOD_RSVD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mcacod_rsvd));
        entry["MSCOD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod));
        entry["MSCOD_SPARE"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod_spare));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW_UPD"] = int_to_hex(static_cast<bool>(status_decoded.fw_upd));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
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

class SprMcaBankIio final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw_upd : 1, corrected_err_cnt : 15, corr_err_status_ind : 2, ar : 1,
                s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SprDefs::IIO::decodeMscod, "MSCOD");
        Generic::IIO::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::IIO::decodeMcacod, status_decoded.miscv,
                                   misc_decoded.bus_log, misc_decoded.device_log, misc_decoded.function_log,
                                   misc_decoded.segment_log);
    }

  public:
    SprMcaBankIio(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW_UPD"] = int_to_hex(static_cast<bool>(status_decoded.ar));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
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

class SprMcaBankM2m final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 8, mscodddrtype : 2, mscodfailoverwhileresetprep : 1, mscodmiscerrs : 5, other_info : 6,
                corrected_err_cnt : 15, corr_err_status_ind : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1,
                overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved0 : 6, addrmode : 3, reserved1 : 2, errortype : 10, trkid : 9, reserved2 : 2, smiopcode : 4,
                reserved3 : 7, mccmdopcode : 6, mccmdmemregion : 4, reserved4 : 11;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::M2M::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::M2M::decodeMcacod150, SprDefs::M2M::decodeMcacod157,
                                   SprDefs::M2M::decodeMcacod64, SprDefs::M2M::decodeMcacod30);
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscodddrtype, SprDefs::M2M::decodeDdrType, "MSCODDDRTYPE");
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod, SprDefs::M2M::decodeMscod, "MSCOD");

        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeFieldBits<uint16_t>(entry, misc_decoded.errortype, SprDefs::M2M::decodeErrortype, "ERRORTYPE");
        Generic::M2M::DecodeTrkid(entry, misc_decoded.trkid);
        Generic::DecodeField<uint8_t>(entry, misc_decoded.smiopcode, Generic::M2M::decodeSmiopcodeGeneric, "SMIOPCODE",
                                      "Unexpected");
        Generic::M2M::DecodeMemreg(entry, misc_decoded.mccmdmemregion, SprDefs::M2M::decodeMemreg);
        Generic::DecodeField<uint8_t>(entry, misc_decoded.mccmdopcode, SprDefs::M2M::decodeMemopcode, "MCCMDOPCODE");
    }

  public:
    SprMcaBankM2m(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["MSCODDDRTYPE"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscodddrtype));
        entry["MSCOD_FAILOVER_WHILE_RESET_PREP"] = int_to_hex(static_cast<bool>(status_decoded.mscodfailoverwhileresetprep));
        entry["MSCOD_MISC_ERRS"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscodmiscerrs));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
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

class SprMcaBankCha final : public SprMcaDecoder
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
            rawChaDecoded = getDecoded<uint8_t>(SprDefs::CHA::rawChaDecoding9, status_decoded.raw_cha);
        }
        else if (mca.bank == 10)
        {
            rawChaDecoded = getDecoded<uint8_t>(SprDefs::CHA::rawChaDecoding10, status_decoded.raw_cha);
        }
        else if (mca.bank == 11)
        {
            rawChaDecoded = getDecoded<uint8_t>(SprDefs::CHA::rawChaDecoding11, status_decoded.raw_cha);
        }
        if (rawChaDecoded)
        {
            entry["INSTANCE_ID_decoded"] = *rawChaDecoded;
        }
    }

  public:
    SprMcaBankCha(const MCAData& mca) : SprMcaDecoder(mca)
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

class SprMcaBankImc final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw_upd : 1, corrected_err_cnt : 15, corr_err_status_ind : 2, ar : 1,
                s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    Generic::IMC::MC_MISC misc_decoded;
    json entry;

    void DecodeOtherinfo()
    {
        std::stringstream ss;
        ss << "MC logs the first error device: " << static_cast<uint32_t>(status_decoded.other_info);
        entry["OTHER_INFO_decoded"] = ss.str();
    }

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, SprDefs::IMC::decodeMscod, "MSCOD");
        ImcDecodeMcacod(entry, status_decoded.mcacod, false);
        DecodeOtherinfo();
    }

  public:
    SprMcaBankImc(const MCAData& mca) : SprMcaDecoder(mca)
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
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
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

class SprMcaBankCbo final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, enh_mca_avail0 : 6, corrected_error_count : 15, corr_err_sts_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved : 37, torid : 5, origreq : 11, reserved2 : 11;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::CHA::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::CBO::decodeMcacod);
        Generic::CHA::DecodeMscod(entry, status_decoded.mscod, SprDefs::CBO::decodeMscod, misc_decoded.origreq, SprOpCodeDecode,
                                  misc_decoded.torid);
    }

  public:
    SprMcaBankCbo(const MCAData& mca) : SprMcaDecoder(mca)
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

class SprMcaBankMdf final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 22, corrected_error_count : 15, corr_err_sts_ind : 2, ar : 1, s : 1, pcc : 1, addrv : 1,
                miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, SprDefs::MDF::decodeMcacod, "MCACOD");
        Generic::DecodeField<uint32_t>(entry, status_decoded.mscod, SprDefs::MDF::decodeMscod, "MSCOD");
    }

  public:
    SprMcaBankMdf(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint32_t>(status_decoded.mscod));
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

class SprHbmMcaBankM2m final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw_update : 1, corrected_error_count : 15, corr_err_sts_ind : 2,
                ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved0 : 6, addrmode : 3, reserved1 : 2, errortype : 10, trkid : 9, reserved2 : 2, smiopcode : 4,
                reserved3 : 7, mccmdopcode : 6, mccmdmemregion : 4, reserved4 : 11;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeMscod()
    {
        std::stringstream ss;
        if ((status_decoded.mscod & 300) >> 8 == 1)
        {
            ss << "HBM Error|";
        }
        auto mscodDecoded = getDecoded<uint16_t>(SprDefs::HBM::M2M::decodeMscod, status_decoded.mscod);
        if (mscodDecoded)
        {
            ss << *mscodDecoded;
        }
        entry["MSCOD_decoded"] = ss.str();
    }

    void decodeKnownFields()
    {
        Generic::M2M::DecodeMcacod(entry, status_decoded.mcacod, SprDefs::M2M::decodeMcacod150, SprDefs::M2M::decodeMcacod157,
                                   SprDefs::M2M::decodeMcacod64, SprDefs::M2M::decodeMcacod30);
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_sts_ind);
        decodeMscod();

        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeFieldBits<uint16_t>(entry, misc_decoded.errortype, SprDefs::M2M::decodeErrortype, "ERRORTYPE");
        Generic::M2M::DecodeTrkid(entry, misc_decoded.trkid);
        Generic::DecodeField<uint8_t>(entry, misc_decoded.smiopcode, Generic::M2M::decodeSmiopcodeGeneric, "SMIOPCODE",
                                      "Unexpected");
        Generic::M2M::DecodeMemreg(entry, misc_decoded.mccmdmemregion, SprDefs::M2M::decodeMemreg);
        Generic::DecodeField<uint8_t>(entry, misc_decoded.mccmdopcode, SprDefs::M2M::decodeMemopcode, "MCCMDOPCODE");
    }

  public:
    SprHbmMcaBankM2m(const MCAData& mca) : SprMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["FW_UPDATE"] = int_to_hex(static_cast<uint8_t>(status_decoded.fw_update));
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

class SprHbmMcaBankImc final : public SprMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 5, fw_upd : 1, corrected_err_cnt : 15, corr_err_status_ind : 2, ar : 1,
                s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    void DecodeMscod()
    {
        std::stringstream ss;

        if ((status_decoded.mc_status & 0x8000) != 0)
        {
            ss << "HBM CONTROLLER ";
        }
        else
        {
            ss << "DDR CONTROLLER ";
        }

        auto mscodDecoded = getDecoded<uint16_t>(SprDefs::HBM::IMC::decodeMscod, status_decoded.mscod);
        if (mscodDecoded)
        {
            ss << *mscodDecoded;
        }
        entry["MSCOD_decoded"] = ss.str();
    }

    void DecodeOtherinfo()
    {
        std::stringstream ss;
        auto decoded = getDecoded<uint8_t>(SprDefs::HBM::IMC::decodeOtherinfo, status_decoded.other_info);
        if (decoded)
        {
            ss << *decoded;
        }
        else
        {
            ss << "MC logs the first error device: " << static_cast<uint32_t>(status_decoded.other_info);
        }

        entry["OTHER_INFO_decoded"] = ss.str();
    }

    void DecodeMiscExtra()
    {
        std::stringstream ss;

        if ((misc_decoded.extra_err_info & 0x7fffffffff0000) == 0)
        {
            uint32_t csrIdx = misc_decoded.extra_err_info & 0xffff;
            ss << "PCLS CSR parity error, CSR index = 0x" << std::hex << csrIdx;

            entry["MISC_EXTRA_decoded"] = ss.str();
            return;
        }

        if ((misc_decoded.extra_err_info & 0x7fffffff800000) == 0)
        {
            uint32_t miscInfo = misc_decoded.extra_err_info & 0x7fffff;
            ss << "2LM/CMI errors, misc info = 0x" << std::hex << miscInfo;

            entry["MISC_EXTRA_decoded"] = ss.str();
            return;
        }

        if ((misc_decoded.extra_err_info & 0x40000000000000) > 0)
        {
            ss << "Transient Error ";
        }

        if ((misc_decoded.extra_err_info & 0x3c000000000000) == 0)
        {
            ss << "sddc 2LM ";
        }
        else if ((misc_decoded.extra_err_info & 0x3c000000000000) >> 50 == 1)
        {
            ss << "sddc 1LM ";
        }

        uint32_t bankIndex = (misc_decoded.extra_err_info & 0x1f0000000000) >> 40;
        uint32_t failedDevice = (misc_decoded.extra_err_info & 0xfc00000000) >> 34;
        uint32_t bankId = (misc_decoded.extra_err_info & 0x3f0000000) >> 28;
        uint32_t rowAddr = (misc_decoded.extra_err_info & 0xffffc00) >> 10;
        uint32_t columnAddr = misc_decoded.extra_err_info & 0x3ff;

        ss << std::hex << "bank index = 0x" << bankIndex << "failed dev = 0x" << failedDevice << "bank id = 0x" << bankId
           << "row = 0x" << rowAddr << "col = 0x" << columnAddr;

        entry["MISC_EXTRA_decoded"] = ss.str();
        return;
    }

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        DecodeMscod();
        ImcDecodeMcacod(entry, status_decoded.mcacod, true);
        DecodeMiscExtra();
    }

    MC_STATUS status_decoded;
    Generic::IMC::MC_MISC misc_decoded;
    json entry;

  public:
    SprHbmMcaBankImc(const MCAData& mca) : SprMcaDecoder(mca)
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
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_err_cnt));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
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