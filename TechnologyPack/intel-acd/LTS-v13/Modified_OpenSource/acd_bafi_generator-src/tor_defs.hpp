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
#include <array>

class ErrLoggingReg
{
    uint8_t dataOffset = 0;
    uint8_t dataLength = 0;
    uint8_t validOffset = 0;
    uint32_t value = 0;

  public:
    ErrLoggingReg(uint8_t dataOffsetIn, uint8_t dataLengthIn, uint8_t validOffsetIn, uint32_t valueIn) :
        dataOffset(dataOffsetIn), dataLength(dataLengthIn), validOffset(validOffsetIn), value(valueIn)
    {
    }

    uint32_t getData()
    {
        return ((value >> dataOffset) << (32 - dataLength)) >> (32 - dataLength);
    }
    bool isValid()
    {
        return value & (1 << validOffset);
    }
};

class IerrLoggingRegGeneric : public ErrLoggingReg
{
  public:
    IerrLoggingRegGeneric(uint32_t value) : ErrLoggingReg(0, 8, 8, value)
    {
    }
};

class IerrLoggingRegSpr : public ErrLoggingReg
{
  public:
    IerrLoggingRegSpr(uint32_t value) : ErrLoggingReg(0, 10, 10, value)
    {
    }
};

class IerrLoggingRegBhs : public ErrLoggingReg
{
  public:
    IerrLoggingRegBhs(uint32_t value) : ErrLoggingReg(0, 16, 16, value)
    {
    }
};

class McerrLoggingRegGeneric : public ErrLoggingReg
{
  public:
    McerrLoggingRegGeneric(uint32_t value) : ErrLoggingReg(0, 8, 8, value)
    {
    }
};

class McerrLoggingRegSpr : public ErrLoggingReg
{
  public:
    McerrLoggingRegSpr(uint32_t value) : ErrLoggingReg(0, 10, 10, value)
    {
    }
};

class McerrLoggingRegBhs : public ErrLoggingReg
{
  public:
    McerrLoggingRegBhs(uint32_t value) : ErrLoggingReg(0, 16, 16, value)
    {
    }
};

class RMcerrLoggingRegGeneric : public ErrLoggingReg
{
  public:
    RMcerrLoggingRegGeneric(uint32_t value) : ErrLoggingReg(0, 10, 10, value)
    {
    }
};

class RMcerrLoggingRegBhs : public ErrLoggingReg
{
  public:
    RMcerrLoggingRegBhs(uint32_t value) : ErrLoggingReg(0, 16, 16, value)
    {
    }
};

class NcEventsCrSmiSrcLogIcx : public ErrLoggingReg
{
  public:
    NcEventsCrSmiSrcLogIcx(uint32_t value) : ErrLoggingReg(8, 8, 16, value)
    {
    }
};

class NcEventsCrSmiSrcLogSpr : public ErrLoggingReg
{
  public:
    NcEventsCrSmiSrcLogSpr(uint32_t value) : ErrLoggingReg(8, 10, 18, value)
    {
    }
};

class NcEventsCrSmiSrcLogBhs : public ErrLoggingReg
{
  public:
    NcEventsCrSmiSrcLogBhs(uint32_t value) : ErrLoggingReg(16, 16, 15, value)
    {
    }
};

class NcEventsCrSmiSrcLogGeneric : public ErrLoggingReg
{
  public:
    NcEventsCrSmiSrcLogGeneric() : ErrLoggingReg(0, 0, 0, 0)
    {
    }
};

std::map<std::vector<uint8_t>, const char*> decodeMcerrErrorSourceGeneric = {
    {{30}, "ExtIERR"},       {{29}, "ExtMCERR"},           {{27}, "IntIERR"},
    {{26}, "IntMCERR"},      {{24}, "MSMI_MCP_SMBUS_ERR"}, {{22}, "ExtMSMI_IERR"},
    {{21}, "ExtMSMI_MCERR"}, {{19}, "IntMSMI_IERR"},       {{18}, "IntMSMI_MCERR"},
};

std::map<std::vector<uint8_t>, const char*> decodeMcerrErrorSourceSpr = {
    {{30}, "ExtIERR"},        {{29}, "ExtMCERR"},           {{27}, "IntIERR"},      {{26}, "IntMCERR"},
    {{25}, "CAM_PARITY_ERR"}, {{24}, "MSMI_MCP_SMBUS_ERR"}, {{22}, "ExtMSMI_IERR"}, {{21}, "ExtMSMI_MCERR"},
    {{19}, "IntMSMI_IERR"},   {{18}, "IntMSMI_MCERR"},      {{15}, "IntRMCA"},      {{14}, "IntRMSMI"},
};

std::map<std::vector<uint8_t>, const char*> decodeMcerrErrorSourceBhs = {
    {{31}, "caterr"},
    {{30}, "ierr"},
    {{29}, "mcerr"},
    {{28}, "caterr_internal"},
    {{27}, "ierr_internal"},
    {{26}, "mcerr_internal"},
    {{25}, "cam_parity_err"},
    {{24}, "msmi_mcp_smbus"},
    {{23}, "msmi"},
    {{22}, "msmi_ierr"},
    {{21}, "msmi_mcerr"},
    {{20}, "msmi_internal"},
    {{19}, "msmi_ierr_internal"},
    {{18}, "msmi_mcerr_internal"},
    {{17}, "recoverable_mca"},
    {{16}, "recoverable_msmi"},
    {{15}, "rmca_internal"},
    {{14}, "rmsmi_internal"},
};

struct SocketCtx
{
    std::shared_ptr<ErrLoggingReg> ierr;
    std::shared_ptr<ErrLoggingReg> mcerr;
    std::shared_ptr<ErrLoggingReg> rMcerrErr;
    std::shared_ptr<ErrLoggingReg> ncEventsCrSmiSrcLog;
    uint32_t mcerrErrValue;
};

enum class SadValues
{
    HOM = 0,
    MMIO,
    CFG,
    MMIOPartialRead,
    IO,
    IntelReserved0,
    SPC,
    IntelReserved1
};

union BDFFormatter
{
    struct
    {
        uint32_t reserved0 : 12, func : 3, dev : 5, bus : 8, reserved1 : 4;
    };
    uint32_t address;
};

const std::array<const char*, 8> SAD_RESULT = {
    "HOM", "MMIO", "CFG", "MMIO Partial Read", "IO", "Intel Reserved", "SPC", "Intel Reserved",
};

class TORDataGen
{
  public:
    virtual ~TORDataGen()
    {
    }

    // Generic variables
    uint32_t cha;
    uint32_t idx;
    virtual uint8_t getCoreId() = 0;
    virtual uint8_t getThreadId() = 0;
    virtual uint16_t getRequestOpCode() = 0;
    virtual uint8_t getInPipe() = 0;
    virtual uint8_t getRetry() = 0;
    virtual uint8_t getFsm() = 0;
    virtual uint8_t getLcs() = 0;
    virtual uint8_t getTarget() = 0;
    virtual uint8_t getSad() = 0;
    virtual uint64_t getAddress() = 0;
    uint8_t core_id;
    uint8_t thread_id;
    uint16_t request_opCode;
    uint8_t in_pipe;
    uint8_t retry;
    uint8_t fsm;
    uint8_t lcs;
    uint8_t target;
    uint8_t sad;

    // Used only for SPR different steppings
    uint8_t core_id_2_0;
    uint8_t core_id_6_3;
    uint8_t core_id2;
    uint8_t core_id3;
    uint8_t orig_req_6_0;
    uint8_t orig_req_10_7;
    uint8_t orig_req2_3_0;
    uint8_t orig_req2_10_4;
    uint8_t orig_req3_1_0;
    uint8_t orig_req3_10_2;
    uint8_t fsm_3_0;
    uint8_t fsm_4_4;
    uint8_t fsm2_3_0;
    uint8_t fsm2_4_4;
    uint8_t thread_id2;
    uint8_t target2;
    uint8_t target3;
    uint8_t sad2;
    uint8_t sad3;
    uint8_t lcs2;

    // Address bits
    // ICX
    // SPR, SPR-HBM
    uint8_t address_6_6_spr;
    uint16_t address_16_7_spr;
    uint8_t address_17_17_spr;
    uint16_t address_27_18_spr;
    uint8_t address_28_28_spr;
    uint16_t address_38_29_spr;
    uint8_t address_39_39_spr;
    uint16_t address_49_40_spr;
    uint8_t address_50_50_spr;
    uint8_t address_51_51_spr;
    uint16_t address_16_6_spr3;
    uint16_t address_27_17_spr3;
    uint16_t address_38_28_spr3;
    uint16_t address_49_39_spr3;
    uint8_t address_51_50_spr3;
    // TORDataCpxSkx
    uint8_t address;
    uint16_t addr_lo;
    // GNR
    uint8_t address_0_5_gnr;
    uint8_t address_6_11_gnr;
    uint8_t address_12_17_gnr;
    uint8_t address_18_23_gnr;
    uint8_t address_24_29_gnr;
    uint8_t address_30_35_gnr;
    uint8_t address_36_41_gnr;
    uint8_t address_42_45_gnr;
};

class TORDataIcx : public TORDataGen
{
    /*
    private:
        uint8_t address_8_6;
        uint8_t address_16_9;
        uint8_t address_19_17;
        uint8_t address_27_20;
        uint8_t address_30_28;
        uint8_t address_38_31;
        uint8_t address_41_39;
        uint8_t address_49_42;
        uint8_t address_51_50;
        uint8_t core_id;
        uint8_t thread_id;
        uint16_t request_opCode;
        uint8_t in_pipe;
        uint8_t retry;
        uint8_t fsm;
        uint8_t lcs;
        uint8_t target;
        uint8_t sad;
    */
  public:
    virtual uint8_t getCoreId()
    {
        return core_id;
    }
    virtual uint8_t getThreadId()
    {
        return thread_id;
    }
    virtual uint16_t getRequestOpCode()
    {
        return request_opCode;
    }
    virtual uint8_t getInPipe()
    {
        return in_pipe;
    }
    virtual uint8_t getRetry()
    {
        return retry;
    }
    virtual uint8_t getFsm()
    {
        return fsm;
    }
    virtual uint8_t getLcs()
    {
        return lcs;
    }
    virtual uint8_t getTarget()
    {
        return target;
    }
    virtual uint8_t getSad()
    {
        return sad;
    }
    virtual uint64_t getAddress()
    {
        return static_cast<uint64_t>(address_8_6) << 6 | static_cast<uint64_t>(address_16_9) << 9 |
               static_cast<uint64_t>(address_19_17) << 17 | static_cast<uint64_t>(address_27_20) << 20 |
               static_cast<uint64_t>(address_30_28) << 28 | static_cast<uint64_t>(address_38_31) << 31 |
               static_cast<uint64_t>(address_41_39) << 39 | static_cast<uint64_t>(address_49_42) << 42 |
               static_cast<uint64_t>(address_51_50) << 50;
    }

    union
    {
        struct
        {
            uint32_t reserved0 : 2, valid : 1, reserved1 : 2, retry : 1, reserved2 : 1, in_pipe : 1, reserved3 : 24;
        };
        uint32_t tordump0_subindex0;
    };
    union
    {
        struct
        {
            uint32_t reserved4 : 29, address_8_6 : 3;
        };
        uint32_t tordump1_subindex0;
    };
    union
    {
        struct
        {
            uint32_t address_16_9 : 8, reserved5 : 24;
        };
        uint32_t tordump2_subindex0;
    };
    union
    {
        struct
        {
            uint32_t reserved6 : 8, thread_id : 3, reserved7 : 21;
        };
        uint32_t tordump0_subindex1;
    };
    union
    {
        struct
        {
            uint32_t reserved8 : 29, address_19_17 : 3;
        };
        uint32_t tordump1_subindex1;
    };
    union
    {
        struct
        {
            uint32_t address_27_20 : 8, reserved9 : 24;
        };
        uint32_t tordump2_subindex1;
    };
    union
    {
        struct
        {
            uint32_t reserved10 : 7, target : 5, reserved11 : 6, sad : 3, reserved12 : 11;
        };
        uint32_t tordump0_subindex2;
    };
    union
    {
        struct
        {
            uint32_t reserved13 : 29, address_30_28 : 3;
        };
        uint32_t tordump1_subindex2;
    };
    union
    {
        struct
        {
            uint32_t address_38_31 : 8, reserved14 : 24;
        };
        uint32_t tordump2_subindex2;
    };
    union
    {
        struct
        {
            uint32_t reserved15 : 21, core_id : 6, reserved16 : 5;
        };
        uint32_t tordump0_subindex3;
    };
    union
    {
        struct
        {
            uint32_t reserved17 : 29, address_41_39 : 3;
        };
        uint32_t tordump1_subindex3;
    };
    union
    {
        struct
        {
            uint32_t address_49_42 : 8, reserved18 : 24;
        };
        uint32_t tordump2_subindex3;
    };
    union
    {
        struct
        {
            uint32_t reserved19 : 17, request_opCode : 11, reserved20 : 4;
        };
        uint32_t tordump0_subindex4;
    };
    union
    {
        struct
        {
            uint32_t reserved21 : 29, address_51_50 : 2, reserved22 : 1;
        };
        uint32_t tordump1_subindex4;
    };
    union
    {
        struct
        {
            uint32_t reserved24 : 3, lcs : 4, reserved25 : 25;
        };
        uint32_t tordump0_subindex7;
    };
    union
    {
        struct
        {
            uint32_t reserved26 : 1, fsm : 5, reserved27 : 26;
        };
        uint32_t tordump1_subindex7;
    };
    uint32_t tordump2_subindex4;
    uint32_t tordump2_subindex7;
};

class TORDataCpxSkx : public TORDataGen
{
  public:
    virtual uint8_t getCoreId()
    {
        return core_id;
    }
    virtual uint8_t getThreadId()
    {
        return thread_id;
    }
    virtual uint16_t getRequestOpCode()
    {
        return request_opCode;
    }
    virtual uint8_t getInPipe()
    {
        return in_pipe;
    }
    virtual uint8_t getRetry()
    {
        return retry;
    }
    virtual uint8_t getFsm()
    {
        return fsm;
    }
    virtual uint8_t getLcs()
    {
        return lcs;
    }
    virtual uint8_t getTarget()
    {
        return target;
    }
    virtual uint8_t getSad()
    {
        return sad;
    }
    virtual uint64_t getAddress()
    {
        return static_cast<uint64_t>(address) << 14 | static_cast<uint64_t>(addr_lo);
    }

    union
    {
        struct
        {
            uint32_t master_valid : 1, valid : 1, retry : 1, in_pipe : 1, cha_inside : 5, tor : 5, core_id : 6, thread_id : 1,
                request_opCode : 11;
        };
        uint32_t subindex0;
    };
    union
    {
        struct
        {
            uint32_t addr_lo : 14, fsm : 6, target : 5, sad : 3, lcs : 4;
        };
        uint32_t subindex1;
    };
    union
    {
        uint32_t address;
        uint32_t subindex2;
    };
};

class TORDataSpr : public TORDataGen
{
  public:
    virtual uint8_t getCoreId()
    {
        return core_id3;
    }
    virtual uint8_t getCoreIdSteppingA()
    {
        return static_cast<uint8_t>(core_id_2_0) | static_cast<uint8_t>(core_id_6_3) << 3;
    }
    virtual uint8_t getCoreIdSteppingB()
    {
        return core_id2;
    }
    virtual uint8_t getThreadId()
    {
        return thread_id2;
    }
    virtual uint8_t getThreadIdSteppingAB()
    {
        return thread_id;
    }
    virtual uint16_t getRequestOpCode()
    {
        return static_cast<uint16_t>(orig_req3_1_0) | static_cast<uint16_t>(orig_req3_10_2) << 2;
    }
    virtual uint16_t getRequestOpCodeSteppingA()
    {
        return static_cast<uint16_t>(orig_req_6_0) | static_cast<uint16_t>(orig_req_10_7) << 7;
    }
    virtual uint16_t getRequestOpCodeSteppingB()
    {
        return static_cast<uint16_t>(orig_req2_3_0) | static_cast<uint16_t>(orig_req2_10_4) << 4;
    }
    virtual uint8_t getInPipe()
    {
        return in_pipe;
    }
    virtual uint8_t getRetry()
    {
        return retry;
    }
    virtual uint8_t getFsm()
    {
        return static_cast<uint8_t>(fsm2_3_0) | static_cast<uint8_t>(fsm2_4_4) << 4;
    }
    virtual uint8_t getFsmSteppingAB()
    {
        return static_cast<uint8_t>(fsm_3_0) | static_cast<uint8_t>(fsm_4_4) << 4;
    }
    virtual uint8_t getLcs()
    {
        return lcs2;
    }
    virtual uint8_t getLcsSteppingA()
    {
        return lcs;
    }
    virtual uint8_t getTarget()
    {
        return target3;
    }
    virtual uint8_t getTargetSteppingB()
    {
        return target2;
    }
    virtual uint8_t getTargetSteppingA()
    {
        return target;
    }
    virtual uint8_t getSad()
    {
        return sad3;
    }
    virtual uint8_t getSadSteppingB()
    {
        return sad2;
    }
    virtual uint8_t getSadSteppingA()
    {
        return sad;
    }
    virtual uint64_t getAddress()
    {
        return static_cast<uint64_t>(address_6_6_spr) << 6 | static_cast<uint64_t>(address_16_7_spr) << 7 |
               static_cast<uint64_t>(address_17_17_spr) << 17 | static_cast<uint64_t>(address_27_18_spr) << 18 |
               static_cast<uint64_t>(address_28_28_spr) << 28 | static_cast<uint64_t>(address_38_29_spr) << 29 |
               static_cast<uint64_t>(address_39_39_spr) << 39 | static_cast<uint64_t>(address_49_40_spr) << 40 |
               static_cast<uint64_t>(address_50_50_spr) << 50 | static_cast<uint64_t>(address_51_51_spr) << 51;
    }
    virtual uint64_t getAddressSteppingSpecific()
    {
        return static_cast<uint64_t>(address_16_6_spr3) << 6 | static_cast<uint64_t>(address_27_17_spr3) << 17 |
               static_cast<uint64_t>(address_38_28_spr3) << 28 | static_cast<uint64_t>(address_49_39_spr3) << 39 |
               static_cast<uint64_t>(address_51_50_spr3) << 50;
    }

    // SPR CPU_ID = 0x806f0, 0x806f5
    union
    {
        struct
        {
            uint32_t valid : 1, reserved29 : 2, retry : 1, reserved30 : 3, in_pipe : 1, reserved31 : 24;
        };
        uint32_t tordump0_subindex0_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved32 : 9, thread_id : 3, reserved33 : 20;
        };
        uint32_t tordump0_subindex1_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved34 : 7, target : 5, reserved35 : 9, sad : 3, reserved36 : 2, non_coherent : 1, reserved37 : 5;
        };
        uint32_t tordump0_subindex2_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved38 : 28, core_id_2_0 : 3, reserved39 : 1;
        };
        uint32_t tordump0_subindex3_spr;
    };
    union
    {
        struct
        {
            uint32_t core_id_6_3 : 4, reserved40 : 20, orig_req_6_0 : 7, reserved41 : 1;
        };
        uint32_t tordump0_subindex4_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved42 : 3, lcs : 4, reserved43 : 25;
        };
        uint32_t tordump0_subindex7_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved44 : 31, address_6_6_spr : 1;
        };
        uint32_t tordump1_subindex0_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved45 : 31, address_17_17_spr : 1;
        };
        uint32_t tordump1_subindex1_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved46 : 31, address_28_28_spr : 1;
        };
        uint32_t tordump1_subindex2_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved47 : 31, address_39_39_spr : 1;
        };
        uint32_t tordump1_subindex3_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved48 : 31, address_50_50_spr : 1;
        };
        uint32_t tordump1_subindex4_spr;
    };
    union
    {
        struct
        {
            uint32_t reserved49 : 6, request_opCode2_1_0 : 2, reserved50 : 24;
        };
        uint32_t tordump1_subindex5_spr;
    };
    union
    {
        struct
        {
            uint32_t request_opCode2_5_3 : 3, reserved51 : 29;
        };
        uint32_t tordump1_subindex6_spr;
    };
    union
    {
        struct
        {
            uint32_t address_16_7_spr : 10, reserved52 : 22;
        };
        uint32_t tordump2_subindex0_spr;
    };
    union
    {
        struct
        {
            uint32_t address_27_18_spr : 10, reserved53 : 22;
        };
        uint32_t tordump2_subindex1_spr;
    };
    union
    {
        struct
        {
            uint32_t address_38_29_spr : 10, reserved54 : 22;
        };
        uint32_t tordump2_subindex2_spr;
    };
    union
    {
        struct
        {
            uint32_t address_49_40_spr : 10, reserved55 : 22;
        };
        uint32_t tordump2_subindex3_spr;
    };
    union
    {
        struct
        {
            uint32_t address_51_51_spr : 1, reserved56 : 31;
        };
        uint32_t tordump2_subindex4_spr;
    };
    // SPR CPU_ID = 0x806f1
    union
    {
        struct
        {
            uint32_t reserved57 : 7, target2 : 5, reserved58 : 10, sad2 : 3, reserved59 : 2, non_coherent2 : 1, reserved60 : 4;
        };
        uint32_t tordump0_subindex2_spr2;
    };
    union
    {
        struct
        {
            uint32_t core_id2 : 7, reserved61 : 20, orig_req2_3_0 : 4, reserved62 : 1;
        };
        uint32_t tordump0_subindex4_spr2;
    };
    union
    {
        struct
        {
            uint32_t orig_req2_10_4 : 7, reserved63 : 25;
        };
        uint32_t tordump0_subindex5_spr2;
    };
    union
    {
        struct
        {
            uint32_t reserved64 : 4, fsm_3_0 : 4, reserved65 : 24;
        };
        uint32_t tordump1_subindex6_spr2;
    };
    union // SAME FOR 0x806f2, 0x806f3, ...
    {
        struct
        {
            uint32_t reserved66 : 6, lcs2 : 4, reserved67 : 21, fsm_4_4 : 1;
        };
        uint32_t tordump0_subindex7_spr2;
    };
    // SPR CPU_ID = 0x806f2, 0x806f3, ...
    union
    {
        struct
        {
            uint32_t reserved68 : 8, thread_id2 : 3, reserved69 : 21;
        };
        uint32_t tordump0_subindex1_spr3;
    };
    union
    {
        struct
        {
            uint32_t reserved70 : 5, target3 : 6, reserved71 : 10, sad3 : 3, reserved72 : 2, non_coherent3 : 1, reserved73 : 5;
        };
        uint32_t tordump0_subindex2_spr3;
    };
    union
    {
        struct
        {
            uint32_t reserved74 : 3, core_id3 : 7, reserved75 : 20, orig_req3_1_0 : 2;
        };
        uint32_t tordump0_subindex4_spr3;
    };
    union
    {
        struct
        {
            uint32_t orig_req3_10_2 : 9, reserved76 : 23;
        };
        uint32_t tordump0_subindex5_spr3;
    };
    union
    {
        struct
        {
            uint32_t reserved77 : 5, fsm2_3_0 : 4, reserved78 : 23;
        };
        uint32_t tordump1_subindex6_spr3;
    };
    union
    {
        struct
        {
            uint32_t fsm2_4_4 : 1, reserved79 : 31;
        };
        uint32_t tordump1_subindex7_spr3;
    };
    union
    {
        struct
        {
            uint32_t address_16_6_spr3 : 11, reserved80 : 21;
        };
        uint32_t tordump2_subindex0_spr3;
    };
    union
    {
        struct
        {
            uint32_t address_27_17_spr3 : 11, reserved81 : 21;
        };
        uint32_t tordump2_subindex1_spr3;
    };
    union
    {
        struct
        {
            uint32_t address_38_28_spr3 : 11, reserved82 : 21;
        };
        uint32_t tordump2_subindex2_spr3;
    };
    union
    {
        struct
        {
            uint32_t address_49_39_spr3 : 11, reserved83 : 21;
        };
        uint32_t tordump2_subindex3_spr3;
    };
    union
    {
        struct
        {
            uint32_t address_51_50_spr3 : 2, reserved84 : 30;
        };
        uint32_t tordump2_subindex4_spr3;
    };
    union
    {
        struct
        {
            uint32_t orig_req_10_7 : 4, reserved90 : 28;
        };
        uint32_t tordump0_subindex5_spr;
    };

    uint32_t tordump2_subindex5_spr;
    uint32_t tordump1_subindex7_spr;
    uint32_t tordump2_subindex7_spr;
    uint32_t tordump0_subindex6_spr;
    uint32_t tordump2_subindex6_spr;
};

class TORDataBhs : public TORDataGen
{
  public:
    virtual uint8_t getCoreId()
    {
        return core_id;
    }
    virtual uint8_t getThreadId()
    {
        return thread_id;
    }
    virtual uint16_t getRequestOpCode()
    {
        return request_opCode;
    }
    virtual uint8_t getInPipe()
    {
        return in_pipe;
    }
    virtual uint8_t getRetry()
    {
        return retry;
    }
    virtual uint8_t getFsm()
    {
        return fsm;
    }
    virtual uint8_t getLcs()
    {
        return lcs;
    }
    virtual uint8_t getTarget()
    {
        return target;
    }
    virtual uint8_t getSad()
    {
        return sad;
    }
    virtual uint64_t getAddress()
    {
        return static_cast<uint64_t>(address_0_5_gnr) << 6 | static_cast<uint64_t>(address_6_11_gnr) << 12 |
               static_cast<uint64_t>(address_12_17_gnr) << 18 | static_cast<uint64_t>(address_18_23_gnr) << 24 |
               static_cast<uint64_t>(address_24_29_gnr) << 30 | static_cast<uint64_t>(address_30_35_gnr) << 36 |
               static_cast<uint64_t>(address_36_41_gnr) << 42 | static_cast<uint64_t>(address_42_45_gnr) << 48;
    }

    // tordump 0 ------------------------------------------------------------------------------------------------
    union
    {
        struct
        {
            uint32_t reserved_gnr1 : 5, valid : 1, reserved_gnr2 : 1, in_pipe : 1, reserved_gnr3 : 18, thread_id : 3,
                reserved_gnr4 : 3;
        };
        uint32_t tordump0_subindex0_gnr;
    };
    union
    {
        struct
        {
            uint32_t reserved_gnr5 : 13, target : 7, reserved_gnr6 : 12;
        };
        uint32_t tordump0_subindex1_gnr;
    };
    union
    {
        struct
        {
            uint32_t sad : 3, reserved_gnr7 : 29;
        };
        uint32_t tordump0_subindex2_gnr;
    };

    uint32_t tordump0_subindex3_gnr;

    union
    {
        struct
        {
            uint32_t reserved_gnr8 : 9, core_id : 8, reserved_gnr9 : 15;
        };
        uint32_t tordump0_subindex4_gnr;
    };

    union
    {
        struct
        {
            uint32_t reserved_gnr10 : 7, request_opCode : 11, reserved_gnr11 : 14;
        };
        uint32_t tordump0_subindex5_gnr;
    };

    uint32_t tordump0_subindex6_gnr;

    union
    {
        struct
        {
            uint32_t reserved_gnr12 : 9, lcs : 4, reserved_gnr13 : 19;
        };
        uint32_t tordump0_subindex7_gnr;
    };

    // tordump 1 ------------------------------------------------------------------------------------------------

    uint32_t tordump1_subindex0_gnr;
    uint32_t tordump1_subindex1_gnr;
    uint32_t tordump1_subindex2_gnr;
    uint32_t tordump1_subindex3_gnr;
    uint32_t tordump1_subindex4_gnr;
    uint32_t tordump1_subindex5_gnr;
    uint32_t tordump1_subindex6_gnr;

    union
    {
        struct
        {
            uint32_t reserved_gnr14 : 2, fsm : 5, reserved_gnr15 : 25;
        };
        uint32_t tordump1_subindex7_gnr;
    };

    // tordump 2 ------------------------------------------------------------------------------------------------
    union
    {
        struct
        {
            uint32_t address_0_5_gnr : 6, reserved_gnr16 : 26;
        };
        uint32_t tordump2_subindex0_gnr;
    };
    union
    {
        struct
        {
            uint32_t address_6_11_gnr : 6, reserved_gnr17 : 26;
        };
        uint32_t tordump2_subindex1_gnr;
    };
    union
    {
        struct
        {
            uint32_t address_12_17_gnr : 6, reserved_gnr18 : 26;
        };
        uint32_t tordump2_subindex2_gnr;
    };
    union
    {
        struct
        {
            uint32_t address_18_23_gnr : 6, reserved_gnr19 : 26;
        };
        uint32_t tordump2_subindex3_gnr;
    };
    union
    {
        struct
        {
            uint32_t address_24_29_gnr : 6, reserved_gnr20 : 26;
        };
        uint32_t tordump2_subindex4_gnr;
    };
    union
    {
        struct
        {
            uint32_t address_30_35_gnr : 6, reserved_gnr21 : 26;
        };
        uint32_t tordump2_subindex5_gnr;
    };

    union
    {
        struct
        {
            uint32_t address_36_41_gnr : 6, reserved_gnr22 : 26;
        };
        uint32_t tordump2_subindex6_gnr;
    };

    union
    {
        struct
        {
            uint32_t address_42_45_gnr : 4, reserved_gnr23 : 26;
        };
        uint32_t tordump2_subindex7_gnr;
    };
};

using TORDataIcxMap = std::map<uint32_t, std::pair<SocketCtx, std::vector<TORDataIcx>>>;

using TORDataCpxSkxMap = std::map<uint32_t, std::pair<SocketCtx, std::vector<TORDataCpxSkx>>>;

using TORDataSprMap = std::map<uint32_t, std::pair<SocketCtx, std::vector<TORDataSpr>>>;

using TORDataBhsMap = std::map<uint32_t, std::pair<SocketCtx, std::map<std::string, std::vector<TORDataBhs>>>>;
