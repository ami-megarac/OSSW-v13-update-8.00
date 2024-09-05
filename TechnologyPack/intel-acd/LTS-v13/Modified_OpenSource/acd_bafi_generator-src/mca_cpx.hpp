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
#include "mca_cpx_defs.hpp"
#include "mca_decode_utils.hpp"
#include "mca_defs.hpp"
#include "tor_defs_cpx.hpp"
#include "utils.hpp"
#include "mca_decode_utils.hpp"
#include "mca_cpx_defs.hpp"

#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;

class CpxMcaDecoder : public McaDecoder
{
  public:
    CpxMcaDecoder(const MCAData& mca) : McaDecoder(mca){};

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
        {0x6, "IIO"},
        {0x7, "IMC 0, Main"},
        {0x8, "IMC 1, Main"},
        {0x9, "CHA_A"},
        {0xA, "CHA_B"},
        {0xB, "CHA_C"},
        {0xC, "Intel UPI 1"},
        {0xD, "IMC 0, channel 0"},
        {0xE, "IMC 0, channel 1"},
        {0xF, "IMC 1, channel 0"},
        {0x10, "IMC 1, channel 1"},
        {0x11, "IMC 0, channel 2"},
        {0x12, "IMC 1, channel 2"},
        {0x13, "Intel UPI 2"},
        {0x14, "Intel UPI 3"},
        {0x15, "Intel UPI 4"},
        {0x16, "Intel UPI 5"},
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

class CpxMcaBankIfu final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, reserved0 : 6, corrected_error_count : 15, corr_err_status_ind : 2, ar : 1, s : 1,
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
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type & 0xFFFF07FF, CpxDefs::IFU::decodeMscod, "MSCOD");
        Generic::IFU::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::IFU::decodeMcacod);
    }

  public:
    CpxMcaBankIfu(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["RESERVED"] = int_to_hex(static_cast<uint8_t>(status_decoded.reserved0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
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

class CpxMcaBankDcu final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 22, corrected_error_count : 15, corr_err_status_ind : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint64_t>(entry, status_decoded.error_type, CpxDefs::DCU::decodeMsmacod, "MSCOD");
        if (!entry.contains("MSCOD_decoded"))
        {
            Generic::DecodeField<uint32_t>(entry, status_decoded.mscod, CpxDefs::DCU::decodeMscod, "MSCOD");
        }
        Generic::DCU::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::DCU::decodeMcacod);
    }

  public:
    CpxMcaBankDcu(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint32_t>(status_decoded.mscod));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
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

class CpxMcaBankDtlb final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, reserved0 : 6, corrected_error_count : 15, reserved1 : 4, pcc : 1, addrv : 1,
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
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, CpxDefs::DTLB::decodeMscod, "MSCOD");
        Generic::DTLB::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::DTLB::decodeMcacod);
    }

  public:
    CpxMcaBankDtlb(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["RESERVED0"] = int_to_hex(static_cast<uint8_t>(status_decoded.reserved0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["RESERVED1"] = int_to_hex(static_cast<uint8_t>(status_decoded.reserved1));
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

class CpxMcaBankMlc final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, reserved0 : 6, corrected_error_count : 15, corr_err_status_ind : 2, reserved1 : 2,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint32_t>(entry, status_decoded.error_type, CpxDefs::MLC::decodeMsmacod, "MSCOD");
        if (!entry.contains("MSCOD_decoded"))
        {
            Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, CpxDefs::MLC::decodeMscod, "MSCOD");
        }
        Generic::MLC::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::MLC::decodeMcacod);
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    CpxMcaBankMlc(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["RESERVED0"] = int_to_hex(static_cast<uint8_t>(status_decoded.reserved0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS_IND"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
        entry["RESERVED1"] = int_to_hex(static_cast<uint8_t>(status_decoded.reserved1));
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

class CpxMcaBankPcu final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod_19_16 : 4, mscod_23_20 : 4, mscod_31_24 : 8, enh_mca_avail0 : 6,
                corrected_error_count : 15, corr_err_status_ind : 2, enh_mca_avail1 : 2, pcc : 1, addrv : 1, miscv : 1, en : 1,
                uc : 1, overflow : 1, valid : 1;
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
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::PCU::DecodeMscod(entry, status_decoded.mscod_19_16, CpxDefs::PCU::decodeMscod1916, status_decoded.mscod_23_20,
                                  CpxDefs::PCU::decodeMscod2320, status_decoded.mscod_31_24, CpxDefs::PCU::decodeMscod3124);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, CpxDefs::PCU::decodeMcacod, "MCACOD");
    }

  public:
    CpxMcaBankPcu(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod_19_16 | status_decoded.mscod_23_20 << 4 |
                                                          status_decoded.mscod_31_24 << 8));
        entry["MSCOD_19_16"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_19_16));
        entry["MSCOD_23_20"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_23_20));
        entry["MSCOD_31_24"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_31_24));
        entry["ENH_MCA_AVAIL0"] = int_to_hex(static_cast<uint8_t>(status_decoded.enh_mca_avail0));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
        entry["CORR_ERR_STATUS"] = int_to_hex(static_cast<uint8_t>(status_decoded.corr_err_status_ind));
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

class CpxMcaBankUpi final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod_21_16 : 6, mscod_31_22 : 10, other_info : 6, corrected_error_count : 15,
                corr_err_status_ind : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union McacodData
    {
        struct
        {
            uint16_t reserved0 : 2, participation : 2, reserved1 : 4, request : 4, timeout : 1, reserved2 : 5;
        };
        uint16_t mcacod_15_0;
    };

    MC_STATUS status_decoded;
    json entry;
    Generic::UPI::MC_MISC misc_decoded;

    std::string DecodeMcacod()
    {
        std::stringstream ss;
        McacodData mcacodDecoded;
        mcacodDecoded.mcacod_15_0 = status_decoded.mcacod;

        auto request = getDecoded<uint8_t>(CpxDefs::UPI::decodeRequest, mcacodDecoded.request);
        auto participation = getDecoded<uint8_t>(CpxDefs::UPI::decodeParticipation, mcacodDecoded.participation);

        if (request)
        {
            ss << *request << "|";
        }
        if (mcacodDecoded.timeout == 1)
        {
            ss << "Timeout|";
        }
        if (participation)
        {
            ss << *participation;
        }

        return ss.str();
    }

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mcacod, CpxDefs::UPI::decodeMcacodGeneric, "MCACOD");
        if (!entry.contains("MCACOD_decoded"))
        {
            entry["MCACOD_decoded"] = DecodeMcacod();
        }

        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod_21_16, CpxDefs::UPI::decodeMscod, "MSCOD");
        Generic::DecodeFieldBits<uint16_t>(entry, status_decoded.mscod_31_22, Generic::UPI::decodeMscodSpare, "MSCOD_SPARE");

        Generic::UPI::DecodeMisc(entry, misc_decoded);
    }

  public:
    CpxMcaBankUpi(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod_21_16 | status_decoded.mscod_31_22 << 6));
        entry["MSCOD_21_16"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod_21_16));
        entry["MSCOD_31_22"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod_31_22));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
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

class CpxMcaBankIio final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 6, corr_err_cnt : 15, corr_err_status_ind : 2, ar : 1, s : 1, pcc : 1,
                addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved0 : 8, segment_log : 8, function_log : 3, device_log : 5, bus_log : 8, reserved1 : 32;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, CpxDefs::IIO::decodeMscod, "MSCOD");
        Generic::IIO::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::IIO::decodeMcacod, status_decoded.miscv,
                                   misc_decoded.bus_log, misc_decoded.device_log, misc_decoded.function_log,
                                   misc_decoded.segment_log);
    }

  public:
    CpxMcaBankIio(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mscod));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORR_ERR_CNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corr_err_cnt));
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

class CpxMcaBankM2m final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 8, mscodddrtype : 2, reserved : 6, other_info : 6, corrected_error_count : 15,
                corr_err_status_ind : 2, ar : 1, s : 1, pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
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
            auto smiopcode = getDecoded<uint8_t>(CpxDefs::M2M::decodeSmiopcode, misc_decoded.smiopcode);
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
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscodddrtype, CpxDefs::M2M::decodeDdrType, "MSCODDDRTYPE");
        Generic::DecodeField<uint8_t>(entry, status_decoded.mscod, CpxDefs::M2M::decodeMscod, "MSCOD");
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);

        Generic::M2M::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::M2M::decodeMcacod150, CpxDefs::M2M::decodeMcacod157,
                                   CpxDefs::M2M::decodeMcacod64, CpxDefs::M2M::decodeMcacod30);

        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
        Generic::DecodeFieldBits<uint16_t>(entry, misc_decoded.errortype, CpxDefs::M2M::decodeErrortype, "ERRORTYPE");
        Generic::M2M::DecodeTrkid(entry, misc_decoded.trkid);
        DecodeSmiopcode();
        Generic::M2M::DecodeMemreg(entry, misc_decoded.mccmdmemregion, CpxDefs::M2M::decodeMemreg);

        Generic::DecodeField<uint8_t>(entry, misc_decoded.mccmdopcode, CpxDefs::M2M::decodeMemopcode, "MCCMDOPCODE");
        Generic::DecodeField<uint8_t>(entry, misc_decoded.dataerrorchunk, CpxDefs::M2M::decodeErrorChunk, "DATAERRORCHUNK");
    }

  public:
    CpxMcaBankM2m(const MCAData& mca) : CpxMcaDecoder(mca)
    {
        status_decoded.mc_status = mca.mc_status;
        misc_decoded.mc_misc = mca.misc;
    }

    json decode_status() override
    {
        entry["MCACOD"] = int_to_hex(static_cast<uint16_t>(status_decoded.mcacod));
        entry["MSCOD"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscod));
        entry["MSCODDDRTYPE"] = int_to_hex(static_cast<uint8_t>(status_decoded.mscodddrtype));
        entry["OTHER_INFO"] = int_to_hex(static_cast<uint8_t>(status_decoded.other_info));
        entry["CORRECTED_ERROR_COUNT"] = int_to_hex(static_cast<uint16_t>(status_decoded.corrected_error_count));
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

class CpxMcaBankCha final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 6, corrected_error_count : 15, corr_err_status_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    union MC_MISC
    {
        struct
        {
            uint64_t reserved : 39, torid : 5, origreq : 10, reserved2 : 10;
        };
        uint64_t mc_misc;
    };

    MC_STATUS status_decoded;
    MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::CHA::DecodeMscod(entry, status_decoded.mscod, CpxDefs::CHA::decodeMscod, misc_decoded.origreq, CpxOpCodeDecode,
                                  misc_decoded.torid);
        Generic::CHA::DecodeMcacod(entry, status_decoded.mcacod, CpxDefs::CHA::decodeMcacod);
    }

  public:
    CpxMcaBankCha(const MCAData& mca) : CpxMcaDecoder(mca)
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

class CpxMcaBankImc final : public CpxMcaDecoder
{
  private:
    union MC_STATUS
    {
        struct
        {
            uint64_t mcacod : 16, mscod : 16, other_info : 6, corrected_error_count : 15, corr_err_status_ind : 2, ar : 1, s : 1,
                pcc : 1, addrv : 1, miscv : 1, en : 1, uc : 1, overflow : 1, valid : 1;
        };
        uint64_t mc_status;
    };

    MC_STATUS status_decoded;
    Generic::IMC::MC_MISC misc_decoded;
    json entry;

    void decodeKnownFields()
    {
        Generic::DecodeCorrErrStsInd(entry, status_decoded.corr_err_status_ind);
        Generic::DecodeField<uint16_t>(entry, status_decoded.mscod, CpxDefs::IMC::decodeMscod, "MSCOD");
        Generic::IMC::DecodeMcacod(entry, status_decoded.mcacod & 0xff, CpxDefs::IMC::decodeErrSpecific, false);
        Generic::DecodeAddrmode(entry, misc_decoded.addrmode);
    }

  public:
    CpxMcaBankImc(const MCAData& mca) : CpxMcaDecoder(mca)
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
