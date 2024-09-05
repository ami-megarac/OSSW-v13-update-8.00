#pragma once
#include <stdint.h>

#include <map>

namespace CpxDefs
{
namespace IFU
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x000005, "REGISTER_FILE_PARITY_ERROR"},
    {0x010005, "DSB_PARITY_ERROR"},
    {0x020005, "MS_uRAM_PARITY_ERROR"},
    {0x020150, "IFU_INCLUSION_ERROR"},
    {0x030005, "INSTRUCTION_QUEUE_PARITY_ERROR"},
    {0x040150, "IFU_ICACHE_DATA_PARITY_POISON_ERROR"},
    {0x050150, "IFU_ICACHE_TAG_PARITY_ERROR"},
    {0x060010, "IFU_ITLB_PARITY_ERROR"},
    {0x070005, "IDQ_PARITY_ERROR"},
    {0x080005, "BRANCH_ARRAY_PARITY_ERROR"},
    {0x090005, "MS_PARITY_ERROR"},
    {0x0A0005, "MS_PARITY_ERROR"},
    {0x0B0150, "IFU_INCLUSION_ERROR"},
    {0x0C0150, "IFU_POISON_ERROR"},
    {0x0D0005, "SDB_PARITY_ERROR"},
    {0x0E0005, "RS/IDQ_IMM_PARITY_ERROR"},
    {0x0F0150, "IFU_INCLUSION_ERROR"},
    {0x000406, "INTERNAL_FIRMWARE_ERROR"},
    {0x020406, "INTERNAL_FIRMWARE_ERROR"},
    {0x0F040A, "EXECUTION_ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x005, "IFU Internal Error"}, {0x150, "IFU Icache Data Parity/Data Posion/Tag Parity Error"},
    {0x010, "ITLB Parity error"},  {0x406, "Internal Firmware Error"},
    {0x40A, "AEDC Error"},
};
} // namespace IFU

namespace DCU
{
const std::map<uint64_t, const char*> decodeMsmacod = {
    {0x000114, "NON-APIC_LOAD_ERROR"}, {0x000124, "NON-APIC_STORE_ERROR"}, {0x100134, "LOAD_POISON"}, {0x110134, "LOAD_POISON"},
    {0x000174, "EVICT_PARITY_ERROR"},  {0x100174, "WBINVD_ERROR"},         {0x000184, "SNOOP_ERROR"}, {0x200401, "APIC_ERROR"},
    {0x200404, "APIC_ERROR"},          {0x020406, "UNCLASSIFIED_ERROR"},
};

const std::map<uint32_t, const char*> decodeMscod = {
    {0x000000, "Non-APIC Error"},
    {0x000010, "WBINVD or Poison SRAR Error"},
    {0x000020, "APIC Error"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x0114, "L1 Read Error"},       {0x0124, "L1 Write Error"},           {0x0134, "L1 Data Read Error"},
    {0x0144, "L1 Data Store Error"}, {0x0164, "L1 Data Prefetch Error"},   {0x0174, "Eviction Error (e.g. WBINV)"},
    {0x0184, "L1 Data Snoop Error"}, {0x0401, "WB Access to APIC Memory"}, {0x0404, "Tag/Data Parity Error on APIC load/store"},
};
} // namespace DCU

namespace DTLB
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x000014, "Tag Parity Error"}, {0x010014, "Data Parity Error"}, {0x010019, "Data Parity Error"},
    {0x020005, "Parity Error"},     {0x030019, "Data Parity Error"}, {0x040005, "SRF"},
    {0x030406, "TRUSTED_PATHS"},    {0x040406, "TRUSTED_PATHS"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x14, "DTLB error code"},
};
} // namespace DTLB

namespace MLC
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x0000C0, "INTERNAL ERROR"},
    {0x000080, "WATCHDOG TIMER 3 STRIKE ERROR"},
    {0x000040, "IDI OR SQ PARITY ERROR"},
    {0x000030, "POISONED DATA DETECTED"},
    {0x000020, "DATA READ UNCORRECTED ERROR/PREFETCH ERROR"},
    {0x000010, "DATA READ CORRECTED ERROR/PREFETCH ERROR"},
    {0x000008, "MESI STATE UNCORRECTED ERROR"},
    {0x000004, "MESI STATE CORRECTED ERROR"},
    {0x000002, "TAG UNCORRECTED ERROR"},
    {0x000001, "TAG CORRECTED ERROR"},
};

const std::map<uint32_t, const char*> decodeMsmacod = {
    {0x000005, "ADDR DATA PARITY ERROR"},
    {0x400005, "ADDR DATA PARITY ERROR"},
    {0x000135, "DATA READ ERROR"},
    {0x000151, "INSTRUCTION FETCH ERROR"},
    {0x000165, "PREFETCH ERROR"},
    {0x000179, "EVICTION ERROR"},
    {0x000145, "DATA WRITE ERROR"},
    {0x000185, "SNOOP ERROR"},
    {0x000189, "SNOOP ERROR"},
    {0x000129, "FLUSH ERROR"},
    {0x800400, "WATCHDOG TIMER 3 STRIKE ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x400, "Watchdog Timer (3-strike timeout) Error"},
    {0x165, "Data Prefetch Errors"},
    {0x135, "Data Read Errors"},
    {0x109, "Tag/MESI State Error"},
};
} // namespace MLC

namespace PCU
{
const std::map<uint16_t, const char*> decodeMcacod = {
    {0x402, "PCU"},
    {0x403, "VCU"},
    {0x406, "Internal firmware errors"},
    {0x407, "Other UBOX errors"},
};

const std::map<uint8_t, const char*> decodeMscod1916 = {
    {0x1, "Instruction address out of valid space"},
    {0x2, "Double bit RAM error on Instruction Fetch"},
    {0x3, "Invalid Opcode seen"},
    {0x4, "Stack Underflow"},
    {0x5, "Stack Overflow"},
    {0x6, "Data address out of valid space"},
    {0x7, "Double bit RAM error on Data Fetch"},
};

const std::map<uint8_t, const char*> decodeMscod2320 = {
    {0x3, "Trusted Path Error"},
    {0x4, "Other Ubox error"},
};

const std::map<uint8_t, const char*> decodeMscod3124 = {
    {0x80, "MCA_INTERNAL_ERROR"},
    {0xff, "MCA_INTERNAL_ERROR"},
    {0x01, "MCA_INTERNAL_ERROR"},
    {0x02, "MCA_INTERNAL_ERROR"},
    {0x03, "MCA_INTERNAL_ERROR"},
    {0x04, "MCA_INTERNAL_ERROR"},
    {0x05, "MCA_PLL_TIMEOUT"},
    {0x06, "MCA_PLL_TIMEOUT"},
    {0x07, "MCA_PLL_TIMEOUT"},
    {0x08, "MCA_PLL_TIMEOUT"},
    {0x09, "MCA_PLL_TIMEOUT"},
    {0x0a, "MCA_CORE_C_STATE_TIMEOUT"},
    {0x0b, "MCA_INTERNAL_ERROR"},
    {0x0c, "MCA_INTERNAL_ERROR"},
    {0x0d, "MCA_INTERNAL_TIMEOUT"},
    {0x0e, "MCA_DMI_TRAINING_TIMEOUT"},
    {0x0f, "MCA_DMI_STRAP_SET_ARRIVAL_TIMEOUT"},
    {0x10, "MCA_DMI_CPU_RESET_ACK_TIMEOUT"},
    {0x11, "MCA_INTERNAL_ERROR"},
    {0x12, "MCA_MEMORY_RCOMP_TIMEOUT"},
    {0x13, "MCA_INTERNAL_ERROR"},
    {0x14, "MCA_PCH_TYPE_ERROR"},
    {0x15, "MCA_INTERNAL_TIMEOUT"},
    {0x1e, "MCA_BIOS_RST_CPL_INVALID_SEQ"},
    {0x1f, "MCA_BIOS_INVALID_PKG_STATE_CONFIG"},
    {0x20, "MCA_INTERNAL_TIMEOUT"},
    {0x21, "MCA_INTERNAL_TIMEOUT"},
    {0x22, "MCA_INTERNAL_TIMEOUT"},
    {0x23, "MCA_INTERNAL_TIMEOUT"},
    {0x24, "MCA_INTERNAL_TIMEOUT"},
    {0x25, "MCA_MSGCH_TIMEOUT"},
    {0x26, "MCA_MSGCH_IO_QUIESCING_TIMEOUT"},
    {0x27, "MCA_MSGCH_PMREQ_CMP_TIMEOUT"},
    {0x28, "MCA_INTERNAL_ERROR"},
    {0x29, "MCA_INTERNAL_TIMEOUT"},
    {0x2a, "MCA_INTERNAL_TIMEOUT"},
    {0x2b, "MCA_INTERNAL_TIMEOUT"},
    {0x2c, "MCA_INTERNAL_TIMEOUT"},
    {0x2d, "MCA_INTERNAL_TIMEOUT"},
    {0x2e, "MCA_INTERNAL_TIMEOUT"},
    {0x30, "MCA_PKGC_DIRECT_WAKE_RING_TIMEOUT"},
    {0x31, "MCA_PKGC_INVALID_RSP_PCH"},
    {0x32, "MCA_PKGC_WATCHDOG_TIMEOUT"},
    {0x33, "MCA_PKGC_WATCHDOG_HANG_CBZ_DOWN"},
    {0x34, "MCA_PKGC_WATCHDOG_HANG_CBZ_UP"},
    {0x35, "MCA_PKGC_WATCHDOG_TIMEOUT"},
    {0x36, "MCA_PKGC_WATCHDOG_TIMEOUT"},
    {0x37, "MCA_PKGC_WATCHDOG_TIMEOUT"},
    {0x38, "MCA_PKGC_WATCHDOG_HANG_C3_UP_SF"},
    {0x39, "MCA_PIT_ERROR"},
    {0x3a, "MCA_PIT_ERROR"},
    {0x3b, "MCA_SMB_ERROR"},
    {0x3c, "MCA_SMB_ERROR"},
    {0x3d, "MCA_SMB_ERROR"},
    {0x3e, "MCA_SMB_ERROR"},
    {0x3f, "MCA_PKGC_WATCHDOG_TIMEOUT"},
    {0x40, "MCA_SVID_VCCIN_VR_ICC_MAX_FAILURE"},
    {0x41, "MCA_SVID_COMMAND_TIMEOUT"},
    {0x42, "MCA_SVID_VCCIN_VR_VOUT_FAILURE"},
    {0x43, "MCA_SVID_CPU_VR_CAPABILITY_ERROR"},
    {0x44, "MCA_SVID_CRITICAL_VR_FAILED"},
    {0x45, "MCA_SVID_SA_ITD_ERROR"},
    {0x46, "MCA_SVID_READ_REG_FAILED"},
    {0x47, "MCA_SVID_WRITE_REG_FAILED"},
    {0x48, "MCA_SVID_PKGC_INIT_FAILED"},
    {0x49, "MCA_SVID_PKGC_CONFIG_FAILED"},
    {0x4a, "MCA_SVID_PKGC_REQUEST_FAILED"},
    {0x4b, "MCA_SVID_IMON_REQUEST_FAILED"},
    {0x4c, "MCA_SVID_ALERT_REQUEST_FAILED"},
    {0x4d, "MCA_SVID_MCP_VR_RAMP_ERROR"},
    {0x4e, "MCA_MEMORY_CAPACITY_EXCEEDED"},
    {0x50, "MCA_FIVR_ERROR"},
    {0x51, "MCA_FIVR_CATAS_OVERVOL_FAULT"},
    {0x52, "MCA_FIVR_CATAS_OVERCUR_FAULT"},
    {0x53, "MCA_FIVR_ERROR"},
    {0x54, "MCA_FIVR_ERROR"},
    {0x55, "MCA_FIVR_TIMEOUT"},
    {0x58, "MCA_WATCHDOG_TIMEOUT_PKGC_SLAVE"},
    {0x59, "MCA_WATCHDOG_TIMEOUT_PKGC_MASTER"},
    {0x5a, "MCA_WATCHDOG_TIMEOUT_PKGS_MASTER"},
    {0x5b, "MCA_IO_MSG_CH_TIMEOUT"},
    {0x5c, "MCA_INTERNAL_ERROR"},
    {0x60, "MCA_VID_FREQ_CHANGE_TIMEOUT"},
    {0x61, "MCA_PKGS_CPD_UNCPD_TIMEOUT"},
    {0x62, "MCA_PKGS_TIMEOUT"},
    {0x63, "MCA_PKGS_INVALID_REQ_PCH"},
    {0x64, "MCA_PKGS_INVALID_REQ_INTERNAL"},
    {0x65, "MCA_PKGS_INVALID_RSP_INTERNAL"},
    {0x66, "MCA_PKGS_TIMEOUT"},
    {0x67, "MCA_PKGS_TIMEOUT"},
    {0x68, "MCA_PKGS_TIMEOUT"},
    {0x69, "MCA_PKGS_TIMEOUT"},
    {0x6a, "MCA_PKGS_ERROR"},
    {0x6b, "MCA_PKGS_SMBUS_VPP_PAUSE_TIMEOUT"},
    {0x6c, "MCA_INTERNAL_ERROR"},
    {0x6e, "MCA_VCODE_ERROR"},
    {0x6f, "MCA_IOT_ERROR"},
    {0x70, "MCA_RATIO_CHANGE_ERROR"},
    {0x7f, "MCA_INTERNAL_ERROR"},
    {0x81, "MCA_RECOVERABLE_DIE_THERMAL_TOO_HOT"},
    {0x83, "MCA_INTERNAL_ERROR"},
    {0x84, "MCA_INTERNAL_ERROR"},
    {0x85, "MCA_INTERNAL_ERROR"},
    {0xfe, "MCA_INTERNAL_ERROR"},
};
} // namespace PCU

namespace UPI
{
const std::map<uint16_t, const char*> decodeMcacodGeneric = {
    {0xc0f, "Unsupported/Undefined Packet"},
    {0xe0f, "For all other corrected and uncorrected errors"},
};

const std::map<uint8_t, const char*> decodeParticipation = {
    {0x2, "Unsupported/Undefined Packet"},
    {0x3, "Other Errors"},
};

const std::map<uint8_t, const char*> decodeRequest = {
    {0xc, "Memory or I/O"},
    {0x3, "Level encoding"},
};

const std::map<uint8_t, const char*> decodeMscod = {
    {0x00, "UC Phy Initialization Failure"},
    {0x01, "UC Phy Detected Drift Buffer Alarm"},
    {0x02, "UC Phy Detected Latency Buffer Rollover"},
    {0x10, "UC LL Rx detected CRC error"},
    {0x11, "UC LL Rx Unsupported/Undefined packet"},
    {0x12, "UC LL or Phy Control Error: unexpected Tx Protocol flit"},
    {0x13, "UC LL Rx Parameter Exception"},
    {0x1F, "UC LL Detected Control Error from M3UPI Correctable (COR)"},
    {0x20, "COR Phy Initialization Abort"},
    {0x21, "COR Phy Reset"},
    {0x22, "COR Phy Lane failure, recovery in x8 width"},
    {0x23, "COR Phy L0c error"},
    {0x24, "COR Phy L0c error triggering Phy Reset"},
    {0x25, "COR Phy L0p exit error triggering Phy Reset"},
    {0x30, "COR LL Rx detected CRC error: successful LLR without Phy Reinit"},
    {0x31, "COR LL Rx detected CRC error: successful LLR with Phy Reinit"},
};
} // namespace UPI

namespace IIO
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x0, "This value is always 0"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0xe0b, "Generic I/O Error"},
    {0x405, "Generic I/O Error"},
};
} // namespace IIO

namespace M2M
{
const std::map<uint8_t, const char*> decodeMscod = {
    {0x01, "DATA_READ_ERROR"},  {0x04, "PARTIAL_WRITE_ERROR"},
    {0x08, "FULL_WRITE_ERROR"}, {0x10, "CLOCK_DOMAIN_CROSSING_BUFFER_(BGF)_ERROR"},
    {0x20, "TIME_OUT"},         {0x40, "TRACKER_PARITY_ERROR"},
    {0x80, "BUCKET1_ERRORS"},
};

const std::map<uint8_t, const char*> decodeDdrType = {
    {0x0, "Error not logged neither on DDR4 nor NVMDIMM"},
    {0x1, "Error specifically on DDR4"},
    {0x2, "Error specifically on NVMDIMM"},
    {0x3, "Error for this transaction was detected on both DDR4 and NVMDIMM"},
};

const std::map<uint16_t, const char*> decodeMcacod150 = {
    {0x400, "Time-out"},
    {0x021, "Read Error, Error from NM"},
    {0x022, "PWR Error, Error from NM"},
    {0x00A, "PWR Error, Error from last level of memory(pmem/block/1LM DDR"},
    {0x009, "Read Error, Error from last level of memory(pmem/block/1LM DDR"},
    {0x005, "Parity error on internal Mesh2mem structures"},
};

const std::map<uint8_t, const char*> decodeMcacod157 = {
    {0x4, "Near-Memory Cache controller error"},
    {0x1, "Last level memory controller error."},
};

const std::map<uint8_t, const char*> decodeMcacod64 = {
    {0x0, "Generic undefined request"}, {0x1, "Memory Read Error"},      {0x2, "Memory Write Error"},
    {0x3, "Address/Command Error"},     {0x4, "Memory Scrubbing Error"},
};

const std::map<uint8_t, const char*> decodeMcacod30 = {
    {0x0, "Error on channel 0"},
    {0x1, "Error on channel 1"},
    {0x2, "Error on channel 2"},
    {0x15, "Channel not specified"},
};

const std::map<uint8_t, const char*> decodeMemopcode = {
    {0x0, "MEMRD"},         {0x1, "MEMSPECRD"}, {0x2, "MEMRDDATA"},    {0x4, "MEMRDXTOS"},    {0x5, "MEMRDXTOI"},
    {0x6, "MEMRDXTOA"},     {0x8, "MEMINV"},    {0x9, "MEMINVXTOI"},   {0x10, "MEMINVXTOA"},  {0x12, "MEMINVITOX"},
    {0x16, "MEMWR"},        {0x19, "MEMWRNI"},  {0x20, "MEMWRPTL"},    {0x21, "PATROL"},      {0x23, "MEMWRPTLNI"},
    {0x27, "MEMWRFLUSH"},   {0x40, "FMFILLRD"}, {0x46, "DEALLOCSBRD"}, {0x47, "DEALLOCSBPF"}, {0x48, "DIRUPD"},
    {0x49, "NMFILLWR"},     {0x51, "FMWR4NM"},  {0x55, "FMWRPTL4NM"},  {0x56, "FMEVICTWR"},   {0x62, "DEALLOCSBWR"},
    {0x63, "DEALLOCSBALL"},
};

const std::map<uint8_t, const char*> decodeMemreg = {
    {0x0, "1LM DDR4 access"},
    {0x1, "NVMDIMM access"},
    {0x2, "DDR4 NM cache access(non-Pmem)"},
    {0x3, "DDR4 NM cache access for Pmem"},
};

const std::map<uint8_t, const char*> decodeErrorChunk = {
    {0x0, "No data error"},
    {0x1, "Data error signal came on 1st half of cache line (critical chunk)"},
    {0x2, "Data error signal came on 2nd half of cache line (non-critical chunk)"},
    {0x3, "Data error signal asserted on both 1st and 2nd half of cache line"},
};

const std::map<uint8_t, const char*> decodeSmiopcode = {
    {0x0, "MemWr"}, {0x3, "MemWrNI"}, {0x4, "MemWrPtl"}, {0x7, "MemWrPtlNI"}, {0xb, "MemWrFlush"},
};

const std::map<std::vector<uint8_t>, const char*> decodeErrortype = {
    {{9}, "Txn had a transient error"},
    {{8}, "Txn had an ECC corrected error"},
    {{7}, "Txn could not be corrected by ECC"},
    {{6}, "Scrub check rd returned good data"},
    {{5}, "Scrub check rd returned uncorrected data"},
    {{4}, "Txn caused permanent hardware channel failover during mirroring"},
    {{3}, "Uncorrectable NM access as persistently uncorrectable"},
};
} // namespace M2M

namespace IMC
{
const std::map<uint16_t, const char*> decodeErrSpecific = {
    {0x5, "Parity error on structures"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x0001, "ADDRESS_PARITY_ERROR"},
    {0x0002, "HA_WR_DATA_PARITY_ERROR"},
    {0x0004, "HA_WR_BE_PARITY_ERROR"},
    {0x0008, "CORR_PATROL_SCRUB_ERROR"},
    {0x0010, "UNCORR_PATROL_SCRUB_ERROR"},
    {0x0020, "CORR_SPARE_ERROR"},
    {0x0040, "UNCORR_SPARE_ERROR"},
    {0x0080, "ANY_HA_RD_ERROR"},
    {0x0100, "WDB_READ_PARITY_ERROR"},
    {0x0102, "DDRT WDB parity error"},
    {0x0103, "RPQ0 parity error"},
    {0x0104, "RPQ1 parity error"},
    {0x0105, "WPQ parity error"},
    {0x0106, "DDRT write data parity error"},
    {0x0107, "DDRT write BE parity error"},
    {0x0108, "DDR/DDRT_LINK_DOWN"},
    {0x0200, "DDR4_CA_PARITY"},
    {0x0400, "UNCORR_ADDRESS_PARITY_ERROR"},
    {0x0800, "UNRECOGNIZED_REQUEST_TYPE"},
    {0x0801, "READ_RESPONSE_TO_AN_INVALID_SCOREBOARD_ENTRY"},
    {0x0802, "UNEXPECTED_READ_RESPONSE"},
    {0x0803, "DDR4_COMPLETION_TO_AN_INVALID_SCOREBOARD_ENTRY"},
    {0x0804, "COMPLETION_TO_AN_INVALID_SCOREBOARD_ENTRY"},
    {0x0805, "COMPLETION_FIFO_OVERFLOW"},
    {0x0806, "CORRECTABLE_PARITY_ERROR"},
    {0x0807, "UNCORR_ERROR."},
    {0x0808, "INTERRUPT_RECEIVED_WHILE_OUTSTANDING_INTERRUPT_WAS_NOT_ACKED"},
    {0x0809, "ERID_FIFO_OVERFLOW"},
    {0x080a, "ERROR_ON_WRITE_CREDITS"},
    {0x080b, "ERROR_ON_READ_CREDITS"},
    {0x080c, "SCHEDULER_ERROR"},
    {0x080d, "DDRT_LINK_RETRY"},
    {0x080e, "FNV thermal error"},
};
} // namespace IMC

namespace CHA
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x0001, "UNCORR_DATA_ERROR"},
    {0x0002, "UNCORR_TAG_ERROR"},
    {0x0003, "SAD_ERR_WB_TO_MMIO"},
    {0x0004, "SAD_ERR_IA_ACCESS_TO_GSM"},
    {0x0005, "SAD_ERR_CORRUPTING_OTHER"},
    {0x0006, "SAD_ERR_NON_CORRUPTING_OTHER"},
    {0x0007, "CORR_DATA_ERROR"},
    {0x0008, "MEM_POISON_DATA_ERROR"},
    {0x0009, "SAD_ERR_CRABABORT"},
    {0x000A, "PARITY_DATA_ERROR"},
    {0x000B, "CORE_WB_MISS_LLC"},
    {0x000C, "TOR_TIMEOUT"},
    {0x000D, "ISMQ_REQ_ERROR"},
    {0x000E, "HA_PARITY_ERROR"},
    {0x000F, "COH_TT_ERROR"},
    {0x0010, "INTERNAL_ERROR"},
    {0x0011, "LLC_TAG_CORR_ERR"},
    {0x0012, "LLC_STATE_CORR_ERR"},
    {0x0013, "LLC_STATE_UNCORR_ERR"},
    {0x0014, "LLC_CV_CORR_ERR"},
    {0x0015, "LLC_CV_UNCORR_ERR"},
    {0x0016, "MULT_TOR_ENTRY_ERROR"},
    {0x0017, "MULT_LLC_WAY_TAG_MATCH"},
    {0x0018, "BL_REQ_RTID_TABLE_MISS"},
    {0x0019, "AK_REQ_RTID_TABLE_MISS"},
    {0x001A, "INTERNAL_ERROR"},
    {0x001B, "INTERNAL_ERROR"},
    {0x001C, "IDI_JITTER_ERROR"},
    {0x001D, "INTERNAL_PARITY_ERROR"},
    {0x001E, "INTERNAL_PARITY_ERROR"},
    {0x001F, "INTERNAL_PARITY_ERROR"},
    {0x0020, "INTERNAL_ERROR"},
    {0x0021, "UNCORRECTABLE_SNOOPFILTER_TAG_ERROR"},
    {0x0022, "SNOOPFILTER_TAG_CORR_ERR"},
    {0x0023, "SNOOPFILTER_STATE_CORR_ERR"},
    {0x0024, "SNOOPFILTER_STATE_UNCORR_ERR"},
    {0x0025, "SNOOPFILTER_CV_CORR_ERR"},
    {0x0026, "SNOOPFILTER_CV_UNCORR_ERR"},
    {0x0027, "SAD_ERR_LTMEMLOCK"},
    {0x0028, "LLC_TWOLM_CORR_ERR"},
    {0x0029, "LLC_TWOLM_UNCORR_ERR"},
    {0x002A, "ISMQ_UNEXP_RSP"},
    {0x002B, "TWOLM_MULT_HIT"},
    {0x002C, "HA_UNEXP_RSP"},
    {0x002D, "SAD_ERR_RRQWBQ_TO_NONHOM"},
    {0x002E, "SAD_ERR_IIOTONONHOM"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x136, "Error during data read (UCNA)"},
    {0x17a, "Error during Explicit Write Back (SRAO)"},
    {0x152, "Error during instruction read (UCNA)"},
};
} // namespace CHA
} // namespace CpxDefs