#pragma once
#include <stdint.h>

#include <map>

namespace SkxDefs
{
namespace IFU
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x00005, "INTERNAL PARITY ERROR"},
    {0x10005, "INTERNAL PARITY ERROR"},
    {0x20005, "INTERNAL PARITY ERROR"},
    {0x20150, "IFU INCLUSION ERROR"},
    {0x30005, "INTERNAL PARITY ERROR"},
    {0x40150, "IFU ICACHE DATA PARITY POISON ERROR"},
    {0x50150, "IFU ICACHE TAG PARITY ERROR"},
    {0x60010, "IFU ITLB PARITY ERROR"},
    {0x70005, "INTERNAL PARITY ERROR"},
    {0x80005, "INTERNAL PARITY ERROR"},
    {0xC0150, "IFU POISON ERROR"},
    {0xD0005, "INTERNAL PARITY ERROR"},
    {0xE0005, "INTERNAL PARITY ERROR"},
    {0xF0005, "INTERNAL PARITY ERROR"},
    {0xF0150, "IFU INCLUSION ERROR"},
    {0x00406, "INTERNAL FIRMWARE ERROR"},
    {0x20406, "INTERNAL FIRMWARE ERROR"},
    {0xF040A, "EXECUTION ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x5, "IFU Internal Error"},
    {0x150, "IFU Icache Data Parity/Data Posion/Tag Parity Error"},
    {0x10, "ITLB Parity error"},
};
} // namespace IFU

namespace DCU
{
const std::map<uint64_t, const char*> decodeMscod = {
    {0x000114, "LOAD ERROR"},  {0x000124, "STORE ERROR"},        {0x100134, "LOAD POISON"},
    {0x110134, "LOAD POISON"}, {0x000174, "EVICT PARITY ERROR"}, {0x100174, "WBINVD ERROR"},
    {0x000184, "SNOOP ERROR"}, {0x200401, "APIC ERROR"},         {0x020406, "UNCLASSIFIED ERROR"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x174, "Eviction Error (e.g. WBINV)"}, {0x134, "L1 Data Read Error"},  {0x144, "L1 Data Store Error"},
    {0x164, "L1 Data Prefetch Error"},      {0x184, "L1 Data Snoop Error"},
};

} // namespace DCU

namespace DTLB
{
const std::map<uint32_t, const char*> decodeMscod = {
    {0x00000014, "TAG PARITY ERROR"},  {0x00010014, "DATA PARITY ERROR"}, {0x00020005, "PARITY ERROR"},
    {0x00010019, "DATA PARITY ERROR"}, {0x00030019, "DATA PARITY ERROR"}, {0x00040005, "INTERNAL ERROR"},
    {0x00030406, "INTERNAL ERROR"},    {0x00040406, "INTERNAL ERROR"},
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
    {0x80, "MCA INTERNAL ERROR"},
    {0xff, "MCA INTERNAL ERROR"},
    {0x01, "MCA INTERNAL ERROR"},
    {0x02, "MCA INTERNAL ERROR"},
    {0x03, "MCA INTERNAL ERROR"},
    {0x04, "MCA INTERNAL ERROR"},
    {0x05, "MCA PLL TIMEOUT"},
    {0x06, "MCA PLL TIMEOUT"},
    {0x07, "MCA PLL TIMEOUT"},
    {0x08, "MCA PLL TIMEOUT"},
    {0x09, "MCA PLL TIMEOUT"},
    {0x0a, "MCA CORE C STATE TIMEOUT"},
    {0x0b, "MCA INTERNAL ERROR"},
    {0x0c, "MCA INTERNAL ERROR"},
    {0x0d, "MCA INTERNAL TIMEOUT"},
    {0x0e, "MCA DMI TRAINING TIMEOUT"},
    {0x0f, "MCA DMI STRAP SET ARRIVAL TIMEOUT"},
    {0x10, "MCA DMI CPU RESET ACK TIMEOUT"},
    {0x11, "MCA INTERNAL ERROR"},
    {0x12, "MCA INTERNAL TIMEOUT"},
    {0x13, "MCA INTERNAL ERROR"},
    {0x14, "MCA PCH TYPE ERROR"},
    {0x15, "MCA INTERNAL TIMEOUT"},
    {0x1e, "MCA BIOS RST CPL INVALID SEQ"},
    {0x1f, "MCA BIOS INVALID PKG STATE CONFIG"},
    {0x20, "MCA INTERNAL TIMEOUT"},
    {0x21, "MCA INTERNAL TIMEOUT"},
    {0x22, "MCA INTERNAL TIMEOUT"},
    {0x23, "MCA INTERNAL TIMEOUT"},
    {0x24, "MCA INTERNAL TIMEOUT"},
    {0x25, "MCA MSGCH TIMEOUT"},
    {0x26, "MCA MSGCH IO QUIESCING TIMEOUT"},
    {0x27, "MCA MSGCH PMREQ CMP TIMEOUT"},
    {0x28, "MCA INTERNAL ERROR"},
    {0x29, "MCA INTERNAL TIMEOUT"},
    {0x2a, "MCA INTERNAL TIMEOUT"},
    {0x2b, "MCA INTERNAL TIMEOUT"},
    {0x2c, "MCA INTERNAL TIMEOUT"},
    {0x2d, "MCA INTERNAL TIMEOUT"},
    {0x2e, "MCA DISPATCHER RUN BUSY TIMEOUT"},
    {0x30, "MCA PKGC DIRECT WAKE RING TIMEOUT"},
    {0x31, "MCA PKGC INVALID RSP PCH"},
    {0x32, "MCA PKGC WATCHDOG TIMEOUT"},
    {0x33, "MCA PKGC WATCHDOG HANG CBZ DOWN"},
    {0x34, "MCA PKGC WATCHDOG HANG CBZ UP"},
    {0x35, "MCA PKGC WATCHDOG TIMEOUT"},
    {0x36, "MCA PKGC WATCHDOG TIMEOUT"},
    {0x37, "MCA PKGC WATCHDOG TIMEOUT"},
    {0x38, "MCA PKGC WATCHDOG HANG C3 UP SF"},
    {0x39, "MCA PIT ERROR"},
    {0x3a, "MCA PIT ERROR"},
    {0x3b, "MCA SMB ERROR"},
    {0x3c, "MCA SMB ERROR"},
    {0x3d, "MCA SMB ERROR"},
    {0x3e, "MCA SMB ERROR"},
    {0x3f, "MCA PKGC WATCHDOG TIMEOUT"},
    {0x40, "MCA SVID VCCIN VR ICC MAX FAILURE"},
    {0x41, "MCA SVID COMMAND TIMEOUT"},
    {0x42, "MCA SVID VCCIN VR VOUT FAILURE"},
    {0x43, "MCA SVID CPU VR CAPABILITY ERROR"},
    {0x44, "MCA SVID CRITICAL VR FAILED"},
    {0x45, "MCA SVID SA ITD ERROR"},
    {0x46, "MCA SVID READ REG FAILED"},
    {0x47, "MCA SVID WRITE REG FAILED"},
    {0x48, "MCA SVID PKGC INIT FAILED"},
    {0x49, "MCA SVID PKGC CONFIG FAILED"},
    {0x4a, "MCA SVID PKGC REQUEST FAILED"},
    {0x4b, "MCA SVID IMON REQUEST FAILED"},
    {0x4c, "MCA SVID ALERT REQUEST FAILED"},
    {0x4d, "MCA SVID MCP VR RAMP ERROR"},
    {0x50, "MCA FIVR ERROR"},
    {0x51, "MCA FIVR CATAS OVERVOL FAULT"},
    {0x52, "MCA FIVR CATAS OVERCUR FAULT"},
    {0x53, "MCA FIVR ERROR"},
    {0x54, "MCA FIVR ERROR"},
    {0x55, "MCA FIVR TIMEOUT"},
    {0x58, "MCA WATCHDOG TIMEOUT PKGC SLAVE"},
    {0x59, "MCA WATCHDOG TIMEOUT PKGC MASTER"},
    {0x5a, "MCA WATCHDOG TIMEOUT PKGS MASTER"},
    {0x5b, "MCA IO MSGCH TIMEOUT"},
    {0x5c, "MCA INTERNAL ERROR"},
    {0x60, "MCA VID FREQ CHANGE TIMEOUT"},
    {0x61, "MCA PKGS CPD UNCPD TIMEOUT"},
    {0x62, "MCA PKGS TIMEOUT"},
    {0x63, "MCA PKGS INVALID REQ PCH"},
    {0x64, "MCA PKGS INVALID REQ INTERNAL"},
    {0x65, "MCA PKGS INVALID RSP INTERNAL"},
    {0x66, "MCA PKGS TIMEOUT"},
    {0x67, "MCA PKGS TIMEOUT"},
    {0x68, "MCA PKGS TIMEOUT"},
    {0x69, "MCA PKGS TIMEOUT"},
    {0x6a, "MCA PKGS ERROR"},
    {0x6b, "MCA PKGS SMBUS VPP PAUSE TIMEOUT"},
    {0x6c, "MCA INTERNAL ERROR"},
    {0x6e, "MCA VCODE ERROR"},
    {0x6f, "MCA IOT ERROR"},
    {0x70, "MCA RATIO CHANGE ERROR"},
    {0x7f, "MCA INTERNAL ERROR"},
    {0x81, "MCA RECOVERABLE DIE THERMAL TOO HOT"},
    {0x83, "MCA INTERNAL ERROR"},
    {0x84, "MCA INTERNAL ERROR"},
    {0x85, "MCA INTERNAL ERROR"},
    {0xfe, "MCA INTERNAL ERROR"},
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
    {0x1, "Address Parity Error"},
    {0x2, "HA Wr data parity Error"},
    {0x4, "HA Wr BE parity Error"},
    {0x8, "Corr Patrol Scrub Error"},
    {0x10, "UnCorr Patrol Scrub Error"},
    {0x20, "Corr Spare Error"},
    {0x40, "UnCorr Spare Error"},
    {0x80, "Any Ha Rd Error"},
    {0x100, "WDB Read Parity Error"},
    {0x108, "DDRT link down"},
    {0x200, "DDR4 CA Parity"},
    {0x400, "UnCorr Address Parity Error"},
    {0x800, "Unrecognized request type"},
    {0x801, "Read response to an invalid scoreboard entry"},
    {0x802, "Unexpected read response"},
    {0x803, "DDR4 completion to an invalid scoreboard entry"},
    {0x804, "Completion to an invalid scoreboard entry"},
    {0x805, "Completion FIFO overflow"},
    {0x806, "Correctable parity error"},
    {0x807, "Uncorr error"},
    {0x808, "Interrupt received while outstanding interrupt was not ACKed"},
    {0x809, "ERID FIFO overflow"},
    {0x80a, "Error on Write credits"},
    {0x80b, "Error on Read credits"},
    {0x80c, "Scheduler error"},
    {0x80d, "DDRT link retry"},
};
} // namespace IMC

namespace CHA
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
    {0x0021, "UNCORRECTABLE_SnoopFilter_TAG_ERROR"},
    {0x0022, "SnoopFilter_TAG_CORR_ERR"},
    {0x0023, "SnoopFilter_STATE_CORR_ERR"},
    {0x0024, "SnoopFilter_STATE_UNCORR_ERR"},
    {0x0025, "SnoopFilter_CV_CORR_ERR"},
    {0x0026, "SnoopFilter_CV_UNCORR_ERR"},
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
} // namespace SkxDefs
