#pragma once
#include <stdint.h>

#include <map>
#include <vector>

namespace IcxDefs
{
namespace IFU
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x00005, "REGISTER_FILE_PARITY_ERROR"},
    {0x10005, "DSB_PARITY_ERROR"},
    {0x20005, "MS_uRAM_PARITY_ERROR"},
    {0x20150, "IFU_INCLUSION_ERROR"},
    {0x30005, "INSTRUCTION_QUEUE_PARITY_ERROR"},
    {0x40150, "IFU_ICACHE_DATA_PARITY_POISON"},
    {0x50150, "IFU_ICACHE_TAG_PARITY_ERROR"},
    {0x60010, "IFU_ITLB_PARITY_ERROR"},
    {0x70005, "IDQ_PARITY_ERROR"},
    {0x80005, "BRANCH_ARRAY_PARITY_ERROR"},
    {0x90005, "MS_PARITY_ERROR"},
    {0xA0005, "MS_PARITY_ERROR"},
    {0xB0150, "IFU_INCLUSION_ERROR"},
    {0xC0150, "IFU_POISON_ERROR"},
    {0xD0005, "SDB_PARITY_ERROR"},
    {0xE0005, "IDQ_IMM_PARITY_ERROR"},
    {0xF0150, "IFU_INCLUSION_ERROR"},
    {0x00406, "INTERNAL_FIRMWARE_ERROR"},
    {0x20406, "INTERNAL_FIRMWARE_ERROR"},
    {0xF040A, "EXECUTION_ERROR"},
    {0x0040A, "EXECUTION_ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x5, "Internal Parity Error"},
    {0x10, "ITLB Parity error"},
    {0x150, "IFU Icache Data Parity/Data Posion/Tag Parity Erro"},
    {0x406, "Internal Firmware Error"},
    {0x40a, "AEDC Error"},
};
} // namespace IFU

namespace DCU
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x000000, "Non-APIC Error"},
    {0x000010, "WBINVD or Poison SRAR Error"},
    {0x000020, "APIC Error"},
};

const std::map<uint64_t, const char*> decodeMsmcacod = {
    {0x000114, "NON-APIC_LOAD_ERROR"}, {0x000124, "NON-APIC_STORE_ERROR"}, {0x100134, "LOAD_POISON"},
    {0x110134, "LOAD_POISON"},         {0x0000174, "EVICT_PARITY_ERROR"},  {0x100174, "WBINVD_ERROR"},
    {0x000184, "SNOOP_ERROR"},         {0x200401, "APIC_ERROR"},           {0x200404, "APIC_ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x114, "Cache Errors: RD.D.L0"},    {0x124, "Cache Errors: WR.D.L0"},
    {0x134, "Cache Errors: DRD.D.L0"},   {0x164, "Cache Errors: PREF.D.L0"},
    {0x174, "Cache Errors: EVICT.D.L0"}, {0x184, "Cache Errors: SNOOP.D.L0"},
    {0x401, "WB Access to APIC Memory"}, {0x404, "Tag/Data Parity Error on APIC load /store"},
};
} // namespace DCU

namespace DTLB
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x00014, "Tag Parity Error"}, {0x10014, "Data Parity Error"}, {0x10019, "Data Parity Error"},
    {0x20005, "Parity Error"},     {0x30019, "Data Parity Error"}, {0x40005, "SRF"},
    {0x30406, "TRUSTED_PATHS"},    {0x40406, "TRUSTED_PATHS"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x14, "DTLB error code"},
};
} // namespace DTLB

namespace MLC
{
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
};

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

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x0400, "Watchdog Timer (3-strike timeout) Error / Data Prefetch Errors"},
    {0x0135, "Data Read Errors"},
    {0x0109, "Tag/MESI State Error"},
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
    {0x402, "PCU"},
    {0x406, "Internal firmware errors"},
    {0x40c, "Shutdown Error"},
};

const std::map<uint8_t, const char*> decodeMscod3124 = {
    {0xd, "LLC_BIST_ACTIVE_TIMEOUT ; Got timeout on LLC-BIST active."},
    {0xe, "DMI_TRAINING_TIMEOUT ; During reset, DMI training failed to "
          "complete within time-out. Possible DMI related failure."},
    {0xf, "DMI_STRAP_SET_ARRIVAL_TIMEOUT ; STRAP SET Message form PCH was "
          "not received in time; Possible DMI Issue."},
    {0x10, "DMI_CPU_RESET_ACK_TIMEOUT ; During reset, DMI message for CPU "
           "Reset Ack did not arrive within timeout. Possible DMI related "
           "failure."},
    {0x11, "MORE_THAN_ONE_LT_AGENT ; During Boot Mode Processing, >1 Intel "
           "TXT Agent detected. Possible Intel UPI BIOS/BMC setting issue."},
    {0x14, "INCOMPATIBLE_PCH_TYPE ; PCH being used is not compatible with "
           "the CPU it is paired with; Possible Platform configuration issue."},
    {0x1e, "BIOS_RST_CPL_INVALID_SEQ ; BIOS violated BIOS Reset CPL "
           "sequencing requirements. Possible BIOS related issue."},
    {0x1f, "BIOS_INVALID_PKG_STATE_CONFIG ; BIOS Invalid PKG State "
           "Configuration. Possible BIOS issue."},
    {0x2d, "PCU_PMAX_CALIB_ERROR ; pmax calibration error"},
    {0x2e, "TSC100_SYNC_TIMEOUT ; Hang on TSC100 sync with xtal clock TSC"},
    {0x3a, "GPSB_TIMEOUT ; Sideband does not respond within timeout value"},
    {0x3b, "PMSB_TIMEOUT ; Sideband does not respond within timeout value"},
    {0x3e, "IOSFSB_PMREQ_CMP_TIMEOUT; Timeout waiting for PMReq.CMP to be "
           "set - IOSF-SB driver"},
    {0x40, "SVID_VCCIN_VR_ICC_MAX_FAILURE ; Fused CPU Icc-max exceeds "
           "Vccin VR's limit"},
    {0x42, "SVID_VCCIN_VR_VOUT_FAILURE ; Fused CPU boot voltage exceeds "
           "Vccin VR's limit"},
    {0x43, "SVID_CPU_VR_CAPABILITY_ERROR ; A CPU VR found that does not "
           "support IOUT (IMON polling)"},
    {0x44, "SVID_CRITICAL_VR_FAILED ; Failure of critical VR detected "
           "during reset"},
    {0x45, "SVID_SA_ITD_ERROR ; Failure updating SA VR VID for ITD"},
    {0x46, "SVID_READ_REG_FAILED ; SVID command to read a register "
           "failed."},
    {0x47, "SVID_WRITE_REG_FAILED ; SVID command to write a register "
           "failed"},
    {0x4a, "SVID_PKGC_REQUEST_FAILED ; SVID Pkgc request failed"},
    {0x4b, "SVID_IMON_REQUEST_FAILED ; SVID IMON request failed"},
    {0x4c, "SVID_ALERT_REQUEST_FAILED ; SVID ALERT request failed"},
    {0x4d, "SVID_MCP_VR_RAMP_ERROR ; MCP VR failed to ramp"},
    {0x56, "FIVR_PD_HARDERR ; PD_HARDERR in IO_PM_EVENT_LOG[17]"},
    {0x58, "WATCHDOG_TIMEOUT_PKGC_SLAVE ; PkgC slave timed-out waiting for "
           "PmRsp from Master"},
    {0x59, "WATCHDOG_TIMEOUT_PKGC_MASTER ; PkgC master timed-out waiting "
           "for PmRsp from PCH"},
    {0x5a, "WATCHDOG_TIMEOUT_PKGS_MASTER ; PkgS master timed-out waiting "
           "for PmRsp from slave"},
    {0x5b, "WATCHDOG_TIMEOUT_MSG_CH_FSM ; Timeout waiting for "
           "IO_MESSAGE_CHANNEL FSM to go idle"},
    {0x5c, "WATCHDOG_TIMEOUT_BULK_CR_FSM ; Timeout waiting for "
           "SA_BULK_CR_READ FSM to go idle"},
    {0x5d, "WATCHDOG_TIMEOUT_IOSFSB_FSM ; Timeout waiting for IOSF-SB FSM "
           "to go idle"},
    {0x60, "PKGS_SAFE_WP_TIMEOUT ; Got timeout to change VID / Freq for "
           "safe GV WP at PKGS."},
    {0x61, "PKGS_CPD_UNCPD_TIMEOUT ; Got timeout to CPD-S1 or to UNCPD-S1 "
           "direct GV."},
    {0x62, "PKGS_INVALID_REQ_PCH ; PkgS Master detected improper request "
           "from PCH"},
    {0x63, "PKGS_INVALID_REQ_INTERNAL ; PkgS Slave detected improper "
           "request from Master"},
    {0x64, "PKGS_INVALID_RSP_INTERNAL ; PkgS Master detected improper "
           "response from Slave"},
    {0x65, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x66, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x67, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x68, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x69, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x6a, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x6b, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x6c, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x6d, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x6e, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x6f, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x70, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x71, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x72, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x73, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x74, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x75, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x76, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x77, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x78, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x79, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x7a, "PKGS_RESET_PREP_TIMEOUT ; IP failed to acknowledge a reset "
           "prep message"},
    {0x7b, "PKGS_SMBUS_VPP_PAUSE_TIMEOUT ; SMBUS VPP Pause Timeout. "
           "Possible SMBUS issue."},
    {0x7c, "PKGS_SMBUS_MCP_PAUSE_TIMEOUT ; SMBUS MCP Pause Timeout. "
           "Possible SMBUS issue."},
    {0x7d, "PKGS_SMBUS_SPD_PAUSE_TIMEOUT ; SMBUS SPD Pause Timeout. "
           "Possible SMBUS issue."},
    {0x80, "PKGC_DISP_BUSY_TIMEOUT ; PKGC exit timed out waiting for "
           "dispatcher to be not busy"},
    {0x81, "PKGC_INVALID_RSP_PCH ; PkgC Master detected improper response "
           "from PCH for EA request"},
    {0x83, "PKGC_WATCHDOG_HANG_CBZ_DOWN ; Pkg C-state hung at transient "
           "state: In Buffer Zone, going Down (towards C2/C3), waiting for "
           "PM_RSP(EA=0)."},
    {0x84, "PKGC_WATCHDOG_HANG_CBZ_UP ; Pkg C-state hung at transient "
           "state: In Buffer Zone, going Up (towards C0), waiting for "
           "PM_RSP(EA=1)."},
    {0x87, "PKGC_WATCHDOG_HANG_C2_BLKMASTER ; Hung in transient state"},
    {0x88, "PKGC_WATCHDOG_HANG_C2_PSLIMIT ; Hung in transient state"},
    {0x89, "PKGC_WATCHDOG_HANG_SETDISP ; Pkg C-state hung at transient "
           "state: In C2, going down towards C3. Ring Off was sent, but not ack."},
    {0x8b, "PKGC_ALLOW_L1_ERROR ; Allow L1 did not respond back"},
    {0x90, "RECOVERABLE_DIE_THERMAL_TOO_HOT ; During slow loop, it was "
           "detected that internal die thermals is too hot."},
    {0xa0, "ADR_SIGNAL_TIMEOUT ; eADR: timeout waiting for ADR to be "
           "signaled"},
    {0xa1, "BCLK_FREQ_OC_ABOVE_THRESHOLD ; Bclk govenor detected Bclk "
           "frequency above fused threshold"},
    {0xb0, "DISPATCHER_RUN_BUSY_TIMEOUT ; Dispatcher timed out"}};

const std::map<uint8_t, const char*> decodeMscod2320 = {
    {0x4, "Clock/power IP response timeout"},
    {0x5, "SMBus controller raised SMI"},
    {0x9, "PM controller received invalid transaction"},
};

const std::map<uint8_t, const char*> decodeMscod1916 = {
    {0x1, "Instruction address out of valid space"},
    {0x2, "Double bit RAM error on Instruction Fetch"},
    {0x3, "Invalid OpCode seen"},
    {0x4, "Stack Underflow"},
    {0x5, "Stack Overflow"},
    {0x6, "Data address out of valid space"},
    {0x7, "Double bit RAM error on Data Fetch"},
};
} // namespace PCU

namespace UPI
{
const std::map<uint8_t, const char*> decodeMscod = {
    {0x0, "UC Phy Initialization Failure"},
    {0x1, "UC Phy Detected Drift Buffer Alarm"},
    {0x2, "UC Phy Detected Latency Buffer Rollover"},
    {0x10, "UC LL Rx detected CRC error"},
    {0x11, "UC LL Rx Unsupported/Undefined packet"},
    {0x12, "UC LL or Phy Control Error: unexpected Tx Protocol flit"},
    {0x13, "UC LL Rx Parameter Exception"},
    {0x1f, "UC LL Detected Control Error from M3UPI Correctable (COR)"},
    {0x20, "COR Phy Initialization Abort"},
    {0x21, "COR Phy Reset"},
    {0x22, "COR Phy Lane failure, recovery in x8 width "},
    {0x23, "COR Phy L0c error"},
    {0x24, "COR Phy L0c error triggering Phy Reset"},
    {0x25, "COR Phy L0p exit error triggering Phy Reset"},
    {0x30, "COR LL Rx detected CRC error: successful LLR without Phy "
           "Reinit"},
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
    {0x800a, "SMI Timeout"},
    {0x800b, "Lock Master Timeout"},
    {0x800e, "Semaphore Error"},
    {0x800f, "AK Egress Write Valid Entry"},
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
    {0x801a, "MS2IOSF to Ubox Overflow (NCB)"},
    {0x801b, "MS2IOSF to Ubox Overflow (NCS)"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0xE0B, "Generic I/O Error"},
    {0x406, "Internal Firmware Error"},
    {0x407, "General UBox Error"},
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
    {0x0, "No error logged (DDR4 or NVMDIMM)"},
    {0x1, "Error specifically on DDR4"},
    {0x2, "Error specifically on NVMDIMM"},
    {0x3, "Error for this transaction was detected on both DDR4 and "
          "NVMDIMM"},
};

const std::map<uint8_t, const char*> decodeMscod = {
    {0x00, "No error (default)"},
    {0x01, "Read ECC error"},
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

const std::map<uint8_t, const char*> decodeMemopcode = {
    {0x0, "MEMRD"},         {0x1, "MEMSPECRD"}, {0x2, "MEMRDDATA"},    {0x4, "MEMRDXTOS"},    {0x5, "MEMRDXTOI"},
    {0x6, "MEMRDXTOA"},     {0x8, "MEMINV"},    {0x9, "MEMINVXTOI"},   {0xA, "MEMINVXTOA"},   {0xC, "MEMINVITOX"},
    {0x10, "MEMWR"},        {0x13, "MEMWRNI"},  {0x14, "MEMWRPTL"},    {0x15, "PATROL"},      {0x17, "MEMWRPTLNI"},
    {0x1B, "MEMWRFLUSH"},   {0x28, "FMFILLRD"}, {0x2E, "DEALLOCSBRD"}, {0x2F, "DEALLOCSBPF"}, {0x30, "DIRUPD"},
    {0x31, "NMFILLWR"},     {0x33, "FMWR4NM"},  {0x37, "FMWRPTL4NM"},  {0x38, "FMEVICTWR"},   {0x3E, "DEALLOCSBWR"},
    {0x3F, "DEALLOCSBALL"},
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
    {{3}, "NM ECC-uncorrectable fatal"},
    {{2}, "NM data-only-ECC-uncorrectable"},
};
} // namespace M2M

namespace IMC
{
const std::map<uint16_t, const char*> decodeErrSpecific = {
    {0x5, "Parity error on structures"},
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
    {0x0080, "HA Corr Rd Error"},
    {0x00A0, "HA UnCorr Rd Error"},
    {0x00C0, "UnCorr MetaData"},
    {0x0100, "WDB Read Parity Error"},
    {0x0102, "DDRT WDB parity error"},
    {0x0103, "RPQ0 parity error"},
    {0x0104, "RPQ1 parity error"},
    {0x0105, "WPQ parity error"},
    {0x0106, "DDRT HA write data parity error"},
    {0x0107, "DDRT HA write BE parity error"},
    {0x0108, "DDR/DDRT link fail"},
    {0x0111, "PCLS CAM error"},
    {0x0112, "PCLS data error"},
    {0x0200, "DDR4 CA Parity"},
    {0x0400, "RPQ Parity (primary) Error"},
    {0x0401, "RPQ Parity (buddy) Error"},
    {0x0404, "WPQ Parity (primary) Error"},
    {0x0405, "WPQ Parity (buddy) Error"},
    {0x0408, "RPB Parity (primary) Error"},
    {0x0409, "RPB Parity (buddy) Error"},
    {0x0800, "DDRT bad request"},
    {0x0801, "Read response to an invalid scoreboard entry"},
    {0x0802, "unexpected read response"},
    {0x0803, "DDR4 completion to an invalid scoreboard entry"},
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
};
} // namespace IMC

namespace CBO
{
const std::map<uint16_t, const char*> decodeMcacod = {
    {0x0136, "Error during data read (UCNA)"},
    {0x017A, "Error during Explicit Write Back (SRAO)"},
    {0x0152, "Error during instruction read (UCNA)"},
};

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
};
} // namespace CBO

namespace CHA
{
const std::map<uint8_t, const char*> rawChaDecoding9 = {
    {0x0, "Most severe Error ID = RAW CHA0"},  {0x1, "Most severe Error ID = RAW CHA3"},
    {0x2, "Most severe Error ID = RAW CHA6"},  {0x3, "Most severe Error ID = RAW CHA9"},
    {0x4, "Most severe Error ID = RAW CHA12"}, {0x5, "Most severe Error ID = RAW CHA15"},
    {0x6, "Most severe Error ID = RAW CHA18"}, {0x7, "Most severe Error ID = RAW CHA21"},
    {0x8, "Most severe Error ID = RAW CHA24"}, {0x9, "Most severe Error ID = RAW CHA27"},
    {0xA, "Most severe Error ID = RAW CHA30"}, {0xB, "Most severe Error ID = RAW CHA33"},
    {0xC, "Most severe Error ID = RAW CHA36"}, {0xD, "Most severe Error ID = RAW CHA39"}};

const std::map<uint8_t, const char*> rawChaDecoding10 = {
    {0x0, "Most severe Error ID = RAW CHA1"},  {0x1, "Most severe Error ID = RAW CHA4"},
    {0x2, "Most severe Error ID = RAW CHA7"},  {0x3, "Most severe Error ID = RAW CHA10"},
    {0x4, "Most severe Error ID = RAW CHA13"}, {0x5, "Most severe Error ID = RAW CHA16"},
    {0x6, "Most severe Error ID = RAW CHA19"}, {0x7, "Most severe Error ID = RAW CHA22"},
    {0x8, "Most severe Error ID = RAW CHA25"}, {0x9, "Most severe Error ID = RAW CHA28"},
    {0xA, "Most severe Error ID = RAW CHA31"}, {0xB, "Most severe Error ID = RAW CHA34"},
    {0xC, "Most severe Error ID = RAW CHA37"}};

const std::map<uint8_t, const char*> rawChaDecoding11 = {
    {0x0, "Most severe Error ID = RAW CHA2"},  {0x1, "Most severe Error ID = RAW CHA5"},
    {0x2, "Most severe Error ID = RAW CHA8"},  {0x3, "Most severe Error ID = RAW CHA11"},
    {0x4, "Most severe Error ID = RAW CHA14"}, {0x5, "Most severe Error ID = RAW CHA17"},
    {0x6, "Most severe Error ID = RAW CHA20"}, {0x7, "Most severe Error ID = RAW CHA23"},
    {0x8, "Most severe Error ID = RAW CHA26"}, {0x9, "Most severe Error ID = RAW CHA29"},
    {0xA, "Most severe Error ID = RAW CHA32"}, {0xB, "Most severe Error ID = RAW CHA35"},
    {0xC, "Most severe Error ID = RAW CHA38"}, {0xD, "Most severe Error ID = RAW CHA41"}};
} // namespace CHA

} // namespace IcxDefs
