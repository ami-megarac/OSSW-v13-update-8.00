#pragma once
#include <stdint.h>

#include <map>
#include <vector>

namespace SprDefs
{
namespace IFU
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x00000005, "REGISTER_FILE_PARITY_ERROR"},
    {0x00010005, "DSB_PARITY_ERROR"},
    {0x00020005, "MS_uRAM_PARITY_ERROR"},
    {0x00020150, "IFU_INCLUSION_ERROR"},
    {0x00030005, "INSTRUCTION_QUEUE_PARITY_ERROR"},
    {0x00040150, "IFU_ICACHE_DATA_PARITY_POISON_ERROR"},
    {0x00050150, "IFU_ICACHE_TAG_PARITY_ERROR"},
    {0x00060010, "IFU_ITLB_PARITY_ERROR"},
    {0x00070005, "IDQ_PARITY_ERROR"},
    {0x00080005, "BRANCH_ARRAY_PARITY_ERROR"},
    {0x00090005, "MS_PARITY_ERROR"},
    {0x000A0005, "MS_PARITY_ERROR"},
    {0x000B0150, "IFU_INCLUSION_ERROR"},
    {0x000C0150, "IFU_POISON_ERROR"},
    {0x000D0005, "SDB_PARITY_ERROR"},
    {0x000E0005, "RS/IDQ_IMM_PARITY_ERROR"},
    {0x000F0150, "IFU_INCLUSION_ERROR"},
    {0x00000406, "INTERNAL_FIRMWARE_ERROR"},
    {0x00020406, "INTERNAL_FIRMWARE_ERROR"},
    {0x000F040A, "EXECUTION_ERROR"},
    {0x0000040A, "EXECUTION_ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x5, "IFU Internal Parity Error"},
    {0x150, "IFU Icache Data Parity/Data Posion/Tag Parity Error"},
    {0x10, "ITLB Parity error"},
    {0x406, "Trusted Paths"},
    {0x40A, "Internal Unclassified Errors"},
};
} // namespace IFU

namespace DCU
{
const std::map<uint64_t, const char*> decodeMsmcacod = {
    {0x00000114, "NON-APIC_LOAD_ERROR"}, {0x00000124, "NON-APIC_STORE_ERROR"}, {0x00100134, "LOAD_POISON"},
    {0x00110134, "LOAD_POISON"},         {0x00000174, "EVICT_PARITY_ERROR"},   {0x00100174, "WBINVD_ERROR"},
    {0x00000184, "SNOOP_ERROR"},         {0x00200401, "APIC_ERROR"},           {0x00200404, "APIC_ERROR"},
};

const std::map<uint32_t, const char*> decodeMscod = {
    {0x0000, "Non-APIC Error"},
    {0x0010, "WBINVD or Poison SRAR Error"},
    {0x0011, "Stiffed Load Hitting Poisoned Data"},
    {0x0020, "APIC Error"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x0114, "L1 Read Error"},       {0x0124, "L1 Write Error"},           {0x0134, "L1 Data Read Error"},
    {0x0144, "L1 Data Store Error"}, {0x0164, "L1 Data Prefetch Error"},   {0x0174, "Eviction Error (e.g. WBINV)"},
    {0x0184, "L1 Data Snoop Error"}, {0x0401, "WB Access to APIC Memory"}, {0x0404, "Tag/Data Parity Error on APIC load/store"},
};
} // namespace DCU

namespace DTLB
{
const std::map<uint32_t, const char*> decodeMsmcacod = {
    {0x00000014, "Tag Parity Error"}, {0x00010014, "Data Parity Error"}, {0x00010019, "Data Parity Error"},
    {0x00020005, "Parity Error"},     {0x00030019, "Data Parity Error"}, {0x00040005, "SRF"},
    {0x00030406, "TRUSTED_PATHS"},    {0x00040406, "TRUSTED_PATHS"},
};
const std::map<uint16_t, const char*> decodeMcacod = {{0x0005, "Internal Parity Error"},
                                                      {0x0014, "DTLB error code: D.L0"},
                                                      {0x0019, "DTLB error code: G.L1"},
                                                      {0x0406, "Trusted Paths"}};
} // namespace DTLB

namespace MLC
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x00C0, "INTERNAL_ERROR"},
    {0x0080, "WATCHDOG_TIMER_3_STRIKE_ERROR"},
    {0x0040, "IDI_OR_SQ_PARITY_ERROR"},
    {0x0030, "POISONED_DATA_DETECTED"},
    {0x0020, "DATA_READ_UNCORRECTED_ERROR/PREFETCH_ERROR"},
    {0x0010, "DATA_READ_CORRECTED_ERROR/PREFETCH_ERROR"},
    {0x0008, "MESI_STATE_UNCORRECTED_ERROR"},
    {0x0004, "MESI_STATE_CORRECTED_ERROR"},
    {0x0002, "TAG_UNCORRECTED_ERROR"},
    {0x0001, "TAG_CORRECTED_ERROR"},
};

const std::map<uint32_t, const char*> decodeMsmacod = {
    {0x00000005, "ADDR_DATA_PARITY_ERROR"},
    {0x00400005, "ADDR_DATA_PARITY_ERROR"},
    {0x00000135, "DATA_READ_ERROR"},
    {0x00000151, "INSTRUCTION_FETCH_ERROR"},
    {0x00000165, "PREFETCH_ERROR"},
    {0x00000179, "EVICTION_ERROR"},
    {0x00000145, "DATA_WRITE_ERROR"},
    {0x00000185, "SNOOP_ERROR"},
    {0x00000189, "SNOOP_ERROR"},
    {0x00000129, "FLUSH_ERROR"},
    {0x00800400, "WATCHDOG_TIMER_3_STRIKE_ERROR"},
    {0x00C00406, "TRUST_ERROR"},
    {0x0000040E, "ACODE_PM_UCSS_ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x115, "Cache Errors: RD.D.L1"},
    {0x129, "Cache Errors: WR.G.L1"},
    {0x135, "Cache Errors: DRD.D.L1"},
    {0x145, "Cache Errors: DWR.D.L1"},
    {0x151, "Cache Errors: IRD.I.L1"},
    {0x165, "Data Prefetch Errors"},
    {0x179, "Cache Errors: EVICT.G.L1"},
    {0x185, "Cache Errors: SNOOP.D.L1"},
    {0x400, "Watchdog Timer (3-strike timeout) Error"},
    {0x405, "Internal / E2E Parity / ECC"},
    {0x406, "Trusted Paths"},
    {0x409, "Internal Unclassified Errors"},
};
} // namespace MLC

namespace PCU
{
const std::map<uint32_t, const char*> decodeShutdown = {
    {0x0001040c, "Shutdown Error: MCE when CR4.MCE is clear"},
    {0x0002040c, "Shutdown Error: MCE when MCIP bit is set"},
    {0x0003040c, "Shutdown Error: MCE under WPS"},
    {0x0004040c, "Shutdown Error: Unrecoverable error during security flow execution"},
    {0x0005040c, "Shutdown Error: SW triple fault shutdown"},
    {0x0007040c, "Shutdown Error: RSM consistency check failures"},
    {0x0008040c, "Shutdown Error: Invalid conditions on protected mode SMM entry"},
    {0x0009040c, "Shutdown Error: Unrecoverable error during security flow execution"},
    {0x0006040c, "Shutdown Error: VMX exit consistency check failures"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x400, "Core 3-strike, see firstierrsrcid in ubox for which core"},
    {0x401, "WB Access to APIC memory"},
    {0x402, "PCU errors"},
    {0x405, "Internal errors"},
    {0x406, "Internal Firmware errors"},
    {0x40b, "Internal Firmware errors"},
    {0x40c, "Shutdown errors"},
};

const std::map<uint8_t, const char*> decodeMscod3124 = {
    {0x01, "MCA_INTERNAL_TIMEOUT"},
    {0x02, "MCA_INTERNAL_TIMEOUT"},
    {0x03, "MCA_INTERNAL_TIMEOUT"},
    {0x04, "MCA_INTERNAL_TIMEOUT"},
    {0x05, "MCA_INTERNAL_TIMEOUT"},
    {0x0a, "MCA_INTERNAL_TIMEOUT"},
    {0x0d, "LLC_BIST_ACTIVE_TIMEOUT"},
    {0x0e, "DMI_TRAINING_TIMEOUT"},
    {0x0f, "DMI_STRAP_SET_ARRIVAL_TIMEOUT"},
    {0x10, "DMI_CPU_RESET_ACK_TIMEOUT"},
    {0x11, "MORE_THAN_ONE_LT_AGENT"},
    {0x14, "INCOMPATIBLE_PCH_TYPE"},
    {0x1e, "BIOS_RST_CPL_INVALID_SEQ"},
    {0x1f, "BIOS_INVALID_PKG_STATE_CONFIG"},
    {0x20, "MCA_INTERNAL_TIMEOUT"},
    {0x21, "MCA_INTERNAL_TIMEOUT"},
    {0x22, "MCA_INTERNAL_TIMEOUT"},
    {0x24, "MCA_INTERNAL_TIMEOUT"},
    {0x25, "MCA_INTERNAL_TIMEOUT"},
    {0x26, "MCA_INTERNAL_TIMEOUT"},
    {0x2d, "PCU_PMAX_CALIB_ERROR"},
    {0x2e, "TSC100_SYNC_TIMEOUT"},
    {0x3a, "GPSB_TIMEOUT"},
    {0x3b, "PMSB_TIMEOUT"},
    {0x3c, "MCA_INTERNAL_TIMEOUT"},
    {0x3d, "MCA_INTERNAL_ERROR"},
    {0x3e, "IOSFSB_PMREQ_CMP_TIMEOUT"},
    {0x40, "SVID_VCCIN_VR_ICC_MAX_FAILURE"},
    {0x42, "SVID_VCCIN_VR_VOUT_FAILURE"},
    {0x43, "SVID_CPU_VR_CAPABILITY_ERROR"},
    {0x44, "SVID_CRITICAL_VR_FAILED"},
    {0x45, "SVID_SA_ITD_ERROR"},
    {0x46, "SVID_READ_REG_FAILED"},
    {0x47, "SVID_WRITE_REG_FAILED"},
    {0x4a, "SVID_PKGC_REQUEST_FAILED"},
    {0x4b, "SVID_IMON_REQUEST_FAILED"},
    {0x4c, "SVID_ALERT_REQUEST_FAILED"},
    {0x4d, "SVID_MCP_VR_RAMP_ERROR"},
    {0x4e, "MCA_MEMORY_CAPACITY_EXCEEDED"},
    {0x50, "FIVR_ERROR"},
    {0x51, "FIVR_ERROR"},
    {0x52, "FIVR_ERROR"},
    {0x53, "FIVR_ERROR"},
    {0x54, "FIVR_ERROR"},
    {0x55, "FIVR_ERROR"},
    {0x56, "FIVR_PD_HARDERR"},
    {0x58, "WATCHDOG_TIMEOUT_PKGC_SLAVE"},
    {0x59, "WATCHDOG_TIMEOUT_PKGC_MASTER"},
    {0x5a, "WATCHDOG_TIMEOUT_PKGS_MASTER"},
    {0x5b, "WATCHDOG_TIMEOUT_MSG_CH_FSM"},
    {0x5c, "WATCHDOG_TIMEOUT_BULK_CR_FSM"},
    {0x5d, "WATCHDOG_TIMEOUT_IOSFSB_FSM"},
    {0x60, "PKGS_SAFE_WP_TIMEOUT"},
    {0x61, "PKGS_CPD_UNCPD_TIMEOUT"},
    {0x62, "PKGS_INVALID_REQ_PCH"},
    {0x63, "PKGS_INVALID_REQ_INTERNAL"},
    {0x64, "PKGS_INVALID_RSP_INTERNAL"},
    {0x65, "MCA_PKGS_TIMEOUT"},
    {0x66, "MCA_PKGS_TIMEOUT"},
    {0x67, "MCA_PKGS_TIMEOUT"},
    {0x68, "MCA_PKGS_TIMEOUT"},
    {0x69, "MCA_PKGS_TIMEOUT"},
    {0x6a, "MCA_PKGS_TIMEOUT"},
    {0x6b, "MCA_PKGS_TIMEOUT"},
    {0x6c, "MCA_PKGS_TIMEOUT"},
    {0x6d, "MCA_PKGS_TIMEOUT"},
    {0x6e, "MCA_PKGS_TIMEOUT"},
    {0x6f, "MCA_PKGS_TIMEOUT"},
    {0x70, "MCA_PKGS_TIMEOUT"},
    {0x71, "MCA_PKGS_TIMEOUT"},
    {0x72, "MCA_PKGS_TIMEOUT"},
    {0x73, "MCA_PKGS_TIMEOUT"},
    {0x74, "MCA_PKGS_TIMEOUT"},
    {0x75, "MCA_PKGS_TIMEOUT"},
    {0x76, "MCA_PKGS_TIMEOUT"},
    {0x77, "MCA_PKGS_TIMEOUT"},
    {0x78, "MCA_PKGS_TIMEOUT"},
    {0x79, "MCA_PKGS_TIMEOUT"},
    {0x7a, "MCA_PKGS_TIMEOUT"},
    {0x7b, "MCA_PKGS_TIMEOUT"},
    {0x7c, "MCA_PKGS_TIMEOUT"},
    {0x7d, "MCA_PKGS_TIMEOUT"},
    {0x7e, "MCA_PKGS_TIMEOUT"},
    {0x7f, "MCA_PKGS_TIMEOUT"},
    {0x80, "MCA_PKGS_TIMEOUT"},
    {0x81, "MCA_PKGS_TIMEOUT"},
    {0x82, "MCA_PKGS_TIMEOUT"},
    {0x83, "MCA_PKGS_TIMEOUT"},
    {0x84, "MCA_PKGS_TIMEOUT"},
    {0x86, "PKGS_SMBUS_VPP_PAUSE_TIMEOUT"},
    {0x87, "PKGS_SMBUS_MCP_PAUSE_TIMEOUT"},
    {0x88, "PKGS_SMBUS_SPD_PAUSE_TIMEOUT"},
    {0x89, "MCA_PKGS_TIMEOUT"},
    {0x90, "PKGC_DISP_BUSY_TIMEOUT"},
    {0x91, "PKGC_INVALID_RSP_PCH"},
    {0x93, "PKGC_WATCHDOG_HANG_CBZ_DOWN"},
    {0x94, "PKGC_WATCHDOG_HANG_CBZ_UP"},
    {0x98, "PKGC_WATCHDOG_HANG_C2_BLKMASTER"},
    {0x99, "PKGC_WATCHDOG_HANG_C2_PSLIMIT"},
    {0x9a, "PKGC_WATCHDOG_HANG_SETDISP"},
    {0x9b, "MCA_PKGC_WATCHDOG_HANG_C3"},
    {0x9c, "MCA_PKGC_WATCHDOG_HANG_BREAK"},
    {0x9d, "PKGC_ALLOW_L1_ERROR"},
    {0x9e, "MCA_INTERNAL_ERROR"},
    {0x9f, "MCA_INTERNAL_ERROR"},
    {0xa0, "RECOVERABLE_DIE_THERMAL_TOO_HOT"},
    {0xa4, "MCA_PKGS_INTERNAL_ERROR"},
    {0xb0, "ADR_SIGNAL_TIMEOUT"},
    {0xb1, "BCLK_FREQ_OC_ABOVE_THRESHOLD"},
    {0xb2, "MCA_GENERIC_TIMEOUT"},
    {0xb3, "MCA_GENERIC_TIMEOUT"},
    {0xb4, "PKGC_TIMEOUT"},
    {0xb5, "MCA_INTERNAL_TIMEOUT"},
    {0xb6, "MCA_INTERNAL_TIMEOUT"},
    {0xb7, "MCA_INTERNAL_TIMEOUT"},
    {0xb8, "MCA_SPI_TIMEOUT"},
    {0xb9, "MCA_GENERIC_TIMEOUT"},
    {0xc0, "DISPATCHER_RUN_BUSY_TIMEOUT"},
    {0xc1, "MCA_INTERNAL_ERROR"},
    {0xc2, "MCA_INTERNAL_ERROR"},
    {0xc3, "MCA_INTERNAL_ERROR"},
    {0xc4, "MCA_INTERNAL_ERROR"},
    {0xc5, "MCA_INTERNAL_ERROR"},
    {0xc6, "MCA_INTERNAL_ERROR"},
    {0xc7, "MCA_INTERNAL_ERROR"},
    {0xd0, "MCA_INTERNAL_ERROR"},
    {0xd1, "MCA_INTERNAL_ERROR"},
    {0xd2, "MCA_INTERNAL_ERROR"},
    {0xd3, "MCA_INTERNAL_ERROR"},
    {0xd4, "MCA_INTERNAL_ERROR"},
    {0xd5, "MCA_INTERNAL_ERROR"},
    {0xe0, "MCA_INTERNAL_ERROR"},
    {0xfe, "MCA_INTERNAL_ERROR"},
};

const std::map<uint8_t, const char*> decodeMscod2320 = {
    {0x4, "Clock/power IP response timeout"},
    {0x5, "SMBus controller raised SMI"},
    {0x9, "PM controller received invalid transaction"},
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
} // namespace PCU

namespace UPI
{
const std::map<uint8_t, const char*> decodeMscod = {
    {0x00, "UC Phy Initialization Failure"},
    {0x01, "UC Phy Detected Drift Buffer Alarm"},
    {0x02, "UC Phy Detected Latency Buffer Rollover"},
    {0x10, "UC LL Rx detected CRC error"},
    {0x11, "UC LL Rx Unsupported/Undefined packet"},
    {0x12, "UC LL or Phy Control Error: unexpected Tx Protocol flit"},
    {0x13, "UC LL Rx Parameter Exception"},
    {0x1F, "UC LL Detected Control Error from M3UPI"},
    {0x20, "COR Phy Initialization Abort"},
    {0x21, "COR Phy Reset"},
    {0x22, "COR Phy Lane failure, recovery in x8 width"},
    {0x23, "COR Phy L0c error"},
    {0x24, "COR Phy L0c error triggering Phy Reset"},
    {0x25, "COR Phy L0p exit error triggering Phy Reset"},
    {0x30, "COR LL Rx detected CRC error: successful LLR without Phy Reinit"},
    {0x31, "COR LL Rx detected CRC error: successful LLR with Phy Reinit"},
};

const std::map<uint8_t, const char*> decodeMcacodPp = {
    {0x2, "Unsupported/Undefined Packet"},
    {0x3, "Other Errors"},
};

const std::map<uint8_t, const char*> decodeMcacodIi = {
    {0x0, "Memory"},
    {0x1, "Reserved"},
    {0x2, "I/O"},
    {0x3, "Other"},
};

const std::map<uint8_t, const char*> decodeMcacodLl = {
    {0x0, "L0"},
    {0x1, "L1"},
    {0x2, "L2"},
    {0x3, "Generic LL"},
};
} // namespace UPI

namespace IIO
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x8000, "Poison"},
    {0x8001, "Unsupported Opcode"},
    {0x8002, "Misaligned CFG Rd (SMM)"},
    {0x8003, "Misaligned CFG Wr (SMM)"},
    {0x8004, "Misaligned CFG Rd (Non-SMM)"},
    {0x8005, "Misaligned CFG Wr (Non-SMM)"},
    {0x8006, "Misaligned MMIO Rd (SMM)"},
    {0x8007, "Misaligned MMIO Wr (SMM)"},
    {0x8008, "Misaligned MMIO Rd (Non-SMM)"},
    {0x8009, "Misaligned MMIO Wr (Non-SMM)"},
    {0x800A, "SMI Timeout"},
    {0x800B, "Lock Master Timeout"},
    {0x800E, "Semaphore Error"},
    {0x800F, "AK Egress Write Valid Entry"},
    {0x8010, "BL Egress Write Valid Entry"},
    {0x8011, "AD Egress Write Valid Entry"},
    {0x8012, "AK Egress Overflow"},
    {0x8013, "BL Egress Overflow"},
    {0x8014, "AD Egress Overflow"},
    {0x8015, "CHA to Ubox Overflow (NCB)"},
    {0x8016, "CHA to Ubox Overflow (NCS)"},
    {0x8017, "UPI to Ubox Overflow (NCB)"},
    {0x8018, "UPI to Ubox Overflow (NCS)"},
    {0x8019, "Ingress Parity Error"},
    {0x801A, "MS2IOSF to Ubox Overflow (NCB)"},
    {0x801B, "MS2IOSF to Ubox Overflow (NCS)"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0xe0b, "Generic I/O Error"},
    {0x405, "Generic I/O Error"},
    {0x406, "Internal Firmware Error"},
    {0x407, "General UBOX Error"},
};
} // namespace IIO

namespace M2M
{
const std::map<uint16_t, const char*> decodeMcacod150 = {
    {0x400, "Time-out"},
    {0x405, "Parity error on CMI"},
    {0x5, "Parity error on internal Mesh2mem structures"},
};

const std::map<uint8_t, const char*> decodeMcacod157 = {
    {0x4, "Near-Memory Cache controller error"},
    {0x1, "Last level memory controller error."},
};

const std::map<uint8_t, const char*> decodeMcacod64 = {{0x0, "Generic undefined request"},
                                                       {0x1, "Memory Read Error"},
                                                       {0x2, "Memory Write Error"},
                                                       {0x3, "Address/Command Error"},
                                                       {0x4, "Memory Scrubbing Error"}};

const std::map<uint8_t, const char*> decodeMcacod30 = {
    {0x0, "Error on channel 0"},
    {0x1, "Error on channel 1"},
    {0x2, "Error on channel 2"},
    {0x15, "Channel not specified"},
};

const std::map<uint8_t, const char*> decodeDdrType = {
    {0x0, "Error not logged neither on DDR5 nor NVMDIMM"},
    {0x1, "Error specifically on DDR5"},
    {0x2, "Error specifically on NVMDIMM"},
    {0x3, "Error for this transaction was detected on both DDR5 and NVMDIMM"},
};

const std::map<uint8_t, const char*> decodeMscod = {
    {0x00, "No error (default)"},
    {0x01, "Read ECC error (MemSpecRd, MemRd, MemRdData, MemRdXto*, MemInv, MemInvXto*, MemInvItoX)"},
    {0x02, "Bucket1 error"},
    {0x03, "RdTrkr Parity error"},
    {0x04, "Secure mismatch"},
    {0x05, "prefetch channel mismatch"},
    {0x06, "failover while reset prep"},
    {0x07, "read completion parity error"},
    {0x08, "response parity error"},
    {0x09, "timeout error"},
    {0x0a, "CMI reserved credit pool error"},
    {0x0b, "CMI total credit count error"},
    {0x0c, "CMI credit oversubscription error"},
};

const std::map<std::vector<uint8_t>, const char*> decodeErrortype = {
    {{9}, "Txn had a transient error"},
    {{8}, "Txn had an ECC corrected error"},
    {{7}, "Txn could not be corrected by ECC"},
    {{6}, "Scrub check rd returned good data"},
    {{5}, "Scrub check rd returned uncorrected data"},
    {{4}, "Txn caused permanent hardware channel failover during mirroring"},
    {{3}, "NM ECC-uncorrectable fatal"},
    {{2}, "NM data-only-ECC-uncorrectable"},
};

const std::map<uint8_t, const char*> decodeMemreg = {
    {0x0, "1LM DDR5 access"},
    {0x1, "NVMDIMM access"},
    {0x2, "DDR5 NM cache access(non-Pmem)"},
    {0x3, "DDR5 NM cache access for Pmem"},
};

const std::map<uint8_t, const char*> decodeMemopcode = {
    {0x0, "MEMRD"},         {0x1, "MEMSPECRD"}, {0x2, "MEMRDDATA"},    {0x4, "MEMRDXTOS"},    {0x5, "MEMRDXTOI"},
    {0x6, "MEMRDXTOA"},     {0x8, "MEMINV"},    {0x9, "MEMINVXTOI"},   {0xa, "MEMINVXTOA"},   {0xc, "MEMINVITOX"},
    {0x10, "MEMWR"},        {0x13, "MEMWRNI"},  {0x14, "MEMWRPTL"},    {0x15, "PATROL"},      {0x17, "MEMWRPTLNI"},
    {0x1b, "MEMWRFLUSH"},   {0x28, "FMFILLRD"}, {0x2e, "DEALLOCSBRD"}, {0x2f, "DEALLOCSBPF"}, {0x30, "DIRUPD"},
    {0x31, "NMFILLWR"},     {0x33, "FMWR4NM"},  {0x37, "FMWRPTL4NM"},  {0x38, "FMEVICTWR"},   {0x3e, "DEALLOCSBWR"},
    {0x3f, "DEALLOCSBALL"},
};
} // namespace M2M

namespace IMC
{
const std::map<uint16_t, const char*> decodeErrSpecific = {
    {0x405, "Parity error on structures"},
};

const std::map<uint8_t, const char*> decodeMcacod157 = {
    {0x1, "(1LM mode)"},
    {0x5, "(2LM mode)"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x0001, "Address Parity Error"},
    {0x0002, "Wr data parity Error"},
    {0x0003, "CMI Uncorr/Corr ECC error"},
    {0x0004, "Wr BE parity Error"},
    {0x0005, "Received UnCorr data"},
    {0x0006, "Received UnCorr MetaData"},
    {0x0007, "Transaction ID Parity Error (APPP)"},
    {0x0008, "Corr Patrol Scrub Error"},
    {0x0010, "UnCorr Patrol Scrub Error"},
    {0x0020, "Corr Spare Error"},
    {0x0040, "UnCorr Spare Error"},
    {0x0080, "Corr Rd Error"},
    {0x00A0, "UnCorr Rd Error"},
    {0x00C0, "UnCorr MetaData"},
    {0x0100, "WDB Read Parity Error"},
    {0x0102, "DDRT WDB parity error"},
    {0x0103, "RPQ0 parity error"},
    {0x0104, "RPQ1 parity error"},
    {0x0105, "WPQ parity error"},
    {0x0106, "DDRT write data parity error"},
    {0x0107, "DDRT write BE parity error"},
    {0x0108, "DDR/DDRT link fail"},
    {0x0111, "PCLS CAM error"},
    {0x0112, "PCLS data error"},
    {0x0200, "DDR5 CA Parity"},
    {0x0220, "HBM Command/Address Parity Error"},
    {0x0221, "HBM data parity Error"},
    {0x0400, "RPQ Parity (primary) Error"},
    {0x0401, "RPQ Parity (buddy) Error"},
    {0x0404, "WPQ Parity (primary) Error"},
    {0x0405, "WPQ Parity (buddy) Error"},
    {0x0408, "RPB Parity (primary) Error"},
    {0x0409, "RPB Parity (buddy) Error"},
    {0x0800, "DDRT bad request"},
    {0x0801, "Read response to an invalid scoreboard entry"},
    {0x0802, "unexpected read response"},
    {0x0803, "DDR5 completion to an invalid scoreboard entry"},
    {0x0804, "DDRT completion to an invalid scoreboard entry"},
    {0x0805, "Completion FIFO overflow"},
    {0x0806, "ERID Correctable parity error"},
    {0x0807, "ERID Uncorrectable error"},
    {0x0808, "Interrupt received while outstanding interrupt was not ACKed"},
    {0x0809, "ERID FIFO overflow"},
    {0x080a, "Error on Write credits"},
    {0x080b, "Error on Read credits"},
    {0x080c, "Scheduler error"},
    {0x080d, "DDRT error event"},
    {0x080e, "FNV thermal error"},
    {0x080f, "CMI packet while idle"},
    {0x0810, "DDRT RPQ REQ PARITY Error"},
    {0x0811, "DDRT WPQ REQ PARITY Error"},
    {0x0812, "2LM NMFILLWR CAM Error"},
    {0x0813, "CMI CREDIT OVERSUB Error"},
    {0x0814, "CMI CREDIT TOTAL Error"},
    {0x0815, "CMI CREDIT RSVD POOL Error"},
    {0x0816, "DDRT RD Error"},
    {0x0817, "WDB FIFO Error"},
    {0x0818, "CMI REQ FIFO OVERFLOW"},
    {0x0819, "CMI REQ FIFO UNDERFLOW"},
    {0x081a, "CMI RSP FIFO OVERFLOW"},
    {0x081b, "CMI RSP FIFO UNDERFLOW"},
    {0x081c, "CMI MISC MC CRDT Errors"},
    {0x081d, "CMI MISC MC ARB Errors"},
    {0x081e, "DDRT WR CMPL FIFO OVERFLOW"},
    {0x081f, "DDRT WR CMPL FIFO UNDERFLOW"},
    {0x0820, "CMI RD CPL FIFO OVERFLOW"},
    {0x0821, "CMI RD CPL FIFO UNDERFLOW"},
    {0x0822, "TME KEY PAR Error"},
    {0x0823, "TME CMI MISC Error"},
    {0x0824, "TME CMI OVFL Error"},
    {0x0825, "TME CMI UFL Error"},
    {0x0826, "TME TEM SECURE Error"},
    {0x0827, "TME UFILL PAR Error"},
    {0x0828, "2LM SB OVERFLOW Error"},
    {0x0829, "MC Internal Error"},
    {0x1008, "Corr Patrol Scrub Error (Mirror Secondary)"},
    {0x1010, "UnCorr Patrol Scrub Error (Mirror Secondary)"},
    {0x1020, "Corr Spare Error (Mirror Secondary)"},
    {0x1040, "UnCorr Spare Error (Mirror Secondary)"},
    {0x1080, "Corr Rd Error (Mirror Secondary)"},
    {0x10A0, "UnCorr Rd Error (Mirror Secondary)"},
};
} // namespace IMC

namespace CHA
{
const std::map<uint8_t, const char*> rawChaDecoding9 = {
    {0x0, "Most severe Error ID = RAW CHA0"},  {0x1, "Most severe Error ID = RAW CHA3"},
    {0x2, "Most severe Error ID = RAW CHA6"},  {0x3, "Most severe Error ID = RAW CHA9"},
    {0x4, "Most severe Error ID = RAW CHA12"}, {0x5, "Most severe Error ID = RAW CHA15"},
    {0x6, "Most severe Error ID = RAW CHA18"}, {0x7, "Most severe Error ID = RAW CHA21"},
    {0x8, "Most severe Error ID = RAW CHA24"}, {0x9, "Most severe Error ID = RAW CHA27"},
    {0xA, "Most severe Error ID = RAW CHA30"}, {0xB, "Most severe Error ID = RAW CHA33"},
    {0xC, "Most severe Error ID = RAW CHA36"}, {0xD, "Most severe Error ID = RAW CHA39"},
};

const std::map<uint8_t, const char*> rawChaDecoding10 = {
    {0x0, "Most severe Error ID = RAW CHA1"},  {0x1, "Most severe Error ID = RAW CHA4"},
    {0x2, "Most severe Error ID = RAW CHA7"},  {0x3, "Most severe Error ID = RAW CHA10"},
    {0x4, "Most severe Error ID = RAW CHA13"}, {0x5, "Most severe Error ID = RAW CHA16"},
    {0x6, "Most severe Error ID = RAW CHA19"}, {0x7, "Most severe Error ID = RAW CHA22"},
    {0x8, "Most severe Error ID = RAW CHA25"}, {0x9, "Most severe Error ID = RAW CHA28"},
    {0xA, "Most severe Error ID = RAW CHA31"}, {0xB, "Most severe Error ID = RAW CHA34"},
    {0xC, "Most severe Error ID = RAW CHA37"},
};

const std::map<uint8_t, const char*> rawChaDecoding11 = {
    {0x0, "Most severe Error ID = RAW CHA2"},  {0x1, "Most severe Error ID = RAW CHA5"},
    {0x2, "Most severe Error ID = RAW CHA8"},  {0x3, "Most severe Error ID = RAW CHA11"},
    {0x4, "Most severe Error ID = RAW CHA14"}, {0x5, "Most severe Error ID = RAW CHA17"},
    {0x6, "Most severe Error ID = RAW CHA20"}, {0x7, "Most severe Error ID = RAW CHA23"},
    {0x8, "Most severe Error ID = RAW CHA26"}, {0x9, "Most severe Error ID = RAW CHA29"},
    {0xA, "Most severe Error ID = RAW CHA32"}, {0xB, "Most severe Error ID = RAW CHA35"},
    {0xC, "Most severe Error ID = RAW CHA38"}, {0xD, "Most severe Error ID = RAW CHA41"},
};
} // namespace CHA

namespace CBO
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x0001, "UNCORRECTABLE_DATA_ERROR"},
    {0x0002, "UNCORRECTABLE_TAG_ERROR"},
    {0x0003, "SAD_ERR_WB_TO_MMIO"},
    {0x0004, "SAD_ERR_IA_ACCESS_TO_GSM"},
    {0x0005, "SAD_ERR_CORRUPTING_OTHER"},
    {0x0006, "SAD_ERR_NON_CORRUPTING_OTHER"},
    {0x0007, "CORRECTABLE_DATA_ERROR"},
    {0x0008, "MEM_POISON_DATA_ERROR"},
    {0x0009, "SAD_ERR_CRABABORT"},
    {0x000A, "PARITY_DATA_ERROR"},
    {0x000B, "CORE_WB_MISS_LLC"},
    {0x000C, "TOR_TIMEOUT"},
    {0x000D, "ISMQ_REQ_ERROR"},
    {0x000E, "HA_PARITY_ERROR"},
    {0x000F, "COH_TT_ERROR"},
    {0x0010, "INTERNAL EROR_0x10"},
    {0x0011, "LLC_TAG_CORR_ERR"},
    {0x0012, "LLC_STATE_CORR_ERR"},
    {0x0013, "LLC_STATE_UNCORR_ERR"},
    {0x0014, "LLC_CV_CORR_ERR"},
    {0x0015, "LLC_CV_UNCORR_ERR"},
    {0x0016, "MULT_TOR_ENTRY_ERROR"},
    {0x0017, "MULT_LLC_WAY_TAG_MATCH"},
    {0x0018, "BL_REQ_RTID_TABLE_MISS"},
    {0x0019, "AK_REQ_RTID_TABLE_MISS"},
    {0x001C, "IDI_JITTER_ERROR"},
    {0x001D, "Internal Parity Error"},
    {0x001E, "Internal Parity Error"},
    {0x001F, "Internal Parity Error"},
    {0x0021, "UNCORRECTABLE_SnoopFilter_TAG_ERROR"},
    {0x0022, "SF_TAG_CORR_ERR"},
    {0x0023, "SF_STATE_CORR_ERR"},
    {0x0024, "SF_STATE_UNCORR_ERR"},
    {0x0025, "SF_CV_CORR_ERR"},
    {0x0026, "SF_CV_UNCORR_ERR"},
    {0x0027, "SAD_ERR_LTMEMLOCK"},
    {0x0028, "LLC_TWOLM_CORR_ERR"},
    {0x0029, "LLC_TWOLM_UNCORR_ERR"},
    {0x002A, "ISMQ_UNEXP_RSP"},
    {0x002B, "TWOLM_MULT_HIT"},
    {0x002C, "HA_UNEXP_RSP"},
    {0x002D, "SAD_ERR_RRQWBQ_TO_NONHOM"},
    {0x002E, "SAD_ERR_IIOTONONHOM"},
    {0x002F, "Parity Internal Error"},
    {0x0031, "SF_TWOLM_CORR_ERR"},
    {0x0032, "SF_TWOLM_UNCORR_ERR"},
    {0x0033, "AK_BL_UQID_PTY_ERROR"},
    {0x0034, "WXSNP_WITH_SNPCOUNT_ZERO"},
    {0x0035, "MEM_PUSH_WR_NS_S"},
    {0x0036, "SAD_ERR_UNSECURE_UPI_ACCESS"},
    {0x0037, "CLFLUSH_MMIO_HIT_M"},
    {0x0038, "SAD_ERR_IAL_ABORT"},
    {0x0039, "RSF_ST_CORR_ERR"},
    {0x003A, "RSF_TAG_UNCORR_ERR"},
    {0x003B, "RSF_ST_UNCORR_ERR"},
    {0x003C, "RSF_TAG_CORR_ERR"},
    {0x003D, "SAD_ERR_TDX_ABORT"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x136, "Error during data read (UCNA)"},
    {0x17a, "Error during Explicit Write Back (SRAO)"},
    {0x152, "Error during instruction read (UCNA)"},
};
} // namespace CBO

namespace MDF
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x40, "Parity Error"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x405, "Parity Error"},
};
} // namespace MDF

namespace HBM
{
namespace M2M
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x00, "No error (default)"},
    {0x01, "Read ECC error (MemSpecRd, MemRd, MemRdData, MemRdXto*, MemInv, MemInvXto*, MemInvItoX)"},
    {0x02, "Bucket1 error"},
    {0x03, "RdTrkr Parity error"},
    {0x04, "Secure mismatch"},
    {0x05, "prefetch channel mismatch"},
    {0x06, "failover while reset prep"},
    {0x07, "read completion parity error"},
    {0x08, "response parity error"},
    {0x09, "timeout error"},
    {0x0a, "CMI reserved credit pool error"},
    {0x0b, "CMI total credit count error"},
    {0x0c, "CMI credit oversubscription error"},
};
}

namespace IMC
{
const std::map<uint8_t, const char*> decodeOtherinfo = {
    {0x02, "Valid Bit for EXTRA_ERR_INFO (MISC register)"},
    {0x08, "PCLS data parity error"},
    {0x10, "HBM DQ parity error"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x0001, "Address Parity Error"},
    {0x0002, "Wr data parity Error"},
    {0x0003, "CMI Uncorr/Corr ECC error"},
    {0x0004, "Wr BE parity Error"},
    {0x0005, "Received UnCorr data"},
    {0x0006, "Received UnCorr MetaData"},
    {0x0007, "Transaction ID Parity Error (APPP)"},
    {0x0008, "Corr Patrol Scrub Error"},
    {0x0010, "UnCorr Patrol Scrub Error"},
    {0x0020, "Corr Spare Error"},
    {0x0040, "UnCorr Spare Error"},
    {0x0080, "Corr Rd Error"},
    {0x00A0, "UnCorr Rd Error"},
    {0x00C0, "UnCorr MetaData"},
    {0x0100, "WDB Read Parity Error"},
    {0x0102, "DDRT WDB parity error"},
    {0x0103, "RPQ0 parity error"},
    {0x0104, "RPQ1 parity error"},
    {0x0105, "WPQ parity error"},
    {0x0106, "DDRT write data parity error"},
    {0x0107, "DDRT write BE parity error"},
    {0x0108, "DDR/DDRT link fail"},
    {0x0111, "PCLS CAM error"},
    {0x0112, "PCLS data error"},
    {0x0200, "DDR5 CA Parity"},
    {0x0220, "HBM Command/Address Parity Error"},
    {0x0221, "HBM data parity Error"},
    {0x0400, "RPQ Parity (primary) Error"},
    {0x0401, "RPQ Parity (buddy) Error"},
    {0x0404, "WPQ Parity (primary) Error"},
    {0x0405, "WPQ Parity (buddy) Error"},
    {0x0408, "RPB Parity (primary) Error"},
    {0x0409, "RPB Parity (buddy) Error"},
    {0x0800, "DDRT bad request"},
    {0x0801, "Read response to an invalid scoreboard entry"},
    {0x0802, "unexpected read response"},
    {0x0803, "DDR5 completion to an invalid scoreboard entry"},
    {0x0804, "DDRT completion to an invalid scoreboard entry"},
    {0x0805, "Completion FIFO overflow"},
    {0x0806, "ERID Correctable parity error"},
    {0x0807, "ERID Uncorrectable error"},
    {0x0808, "Interrupt received while outstanding interrupt was not ACKed"},
    {0x0809, "ERID FIFO overflow"},
    {0x080a, "Error on Write credits"},
    {0x080b, "Error on Read credits"},
    {0x080c, "Scheduler error"},
    {0x080d, "DDRT error event"},
    {0x080e, "FNV thermal error"},
    {0x080f, "CMI packet while idle"},
    {0x0810, "DDRT RPQ REQ PARITY Error"},
    {0x0811, "DDRT WPQ REQ PARITY Error"},
    {0x0812, "2LM NMFILLWR CAM Error"},
    {0x0813, "CMI CREDIT OVERSUB Error"},
    {0x0814, "CMI CREDIT TOTAL Error"},
    {0x0815, "CMI CREDIT RSVD POOL Error"},
    {0x0816, "DDRT RD Error"},
    {0x0817, "WDB FIFO Error"},
    {0x0818, "CMI REQ FIFO OVERFLOW"},
    {0x0819, "CMI REQ FIFO UNDERFLOW"},
    {0x081a, "CMI RSP FIFO OVERFLOW"},
    {0x081b, "CMI RSP FIFO UNDERFLOW"},
    {0x081c, "CMI MISC MC CRDT Errors"},
    {0x081d, "CMI MISC MC ARB Errors"},
    {0x081e, "DDRT WR CMPL FIFO OVERFLOW"},
    {0x081f, "DDRT WR CMPL FIFO UNDERFLOW"},
    {0x0820, "CMI RD CPL FIFO OVERFLOW"},
    {0x0821, "CMI RD CPL FIFO UNDERFLOW"},
    {0x0822, "TME KEY PAR Error"},
    {0x0823, "TME CMI MISC Error"},
    {0x0824, "TME CMI OVFL Error"},
    {0x0825, "TME CMI UFL Error"},
    {0x0826, "TME TEM SECURE Error"},
    {0x0827, "TME UFILL PAR Error"},
    {0x0828, "2LM SB OVERFLOW Error"},
    {0x0829, "MC Internal Error"},
    {0x1008, "Corr Patrol Scrub Error (Mirror Secondary)"},
    {0x1010, "UnCorr Patrol Scrub Error (Mirror Secondary)"},
    {0x1020, "Corr Spare Error (Mirror Secondary)"},
    {0x1040, "UnCorr Spare Error (Mirror Secondary)"},
    {0x1080, "Corr Rd Error (Mirror Secondary)"},
    {0x10A0, "UnCorr Rd Error (Mirror Secondary)"},
};
} // namespace IMC
} // namespace HBM
} // namespace SprDefs
