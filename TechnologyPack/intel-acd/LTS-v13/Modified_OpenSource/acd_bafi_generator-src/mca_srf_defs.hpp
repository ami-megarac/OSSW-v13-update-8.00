#pragma once
#include <stdint.h>

#include <map>
#include <vector>

namespace SrfDefs
{
namespace FEC
{
const std::map<uint32_t, const char*> decodeMsmcacod = {
    {0x00000006, "SMM External Call Trap"},
    {0x10000151, "Poison Consumption, Instruction Fetch"},
    {0x20000151, "GO-ERR, Instruction Fetch"},
    {0x40000151, "L1I - Corrected Data Parity Error, Instruction Fetch"},
    {0x80000151, "L1I - Corrected Tag Parity Error, Instruction Fetch"},
    {0x80000151, "L1I - Corrected Tag Parity Error, Instruction Snoop"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x1, "Poisoned Data"},
    {0x2, "Other Transaction Error"},
    {0x4, "Data Parity Error"},
    {0x8, "Tag Parity Error"},
};

const std::map<uint8_t, const char*> decodeMscodLl = {
    {0x4, "L1I Corrected Data Parity Error, Instruction Fetch"},
    {0x8, "L1I Corrected Tag Parity Error, Instruction Fetch"},
};

const std::map<uint16_t, const char*> decodeMscodArr = {
    {0x1, "ARR ROALLOCM"},
    {0x2, "ARR ROBWALKERM"},
    {0x4, "ARR ARTHEAPI"},
    {0x8, "ARR ARTHEAPF"},
    {0x10, "ARR EXPRF"},
    {0x20, "ARR FPRF"},
    {0x40, "ARR EXPRF"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x5, "Internal Parity Error"},
    {0x6, "SMM Handler Code Access Violation"},
    {0x11, "ITLB Parity error"},
    {0x151, "Chache L1 IRD Error"},
    {0x181, "Cache L1 SNOOP Error"},
};

const std::map<uint8_t, const char*> decodeCtl = {
    {0x1, "FEC"},  {0x2, "FSCP"}, {0x4, "SRF"}, {0x8, "STLB"}, {0x10, "Page Walk"}, {0x20, "ICLB"}
};
} // namespace FEC

namespace L2
{
const std::map<uint32_t, const char*> decodeMsmcacod = {
    {0x110A, "L2 - Corrected Data Error, Filtered"},
};

const std::map<uint8_t, const char*> decodeOtherInfo = {
    {0x0, "L2 Data Error"},
    {0x1, "L2 Tag Error"},
    {0x2, "L2 State Error"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x010a, "Cache L2 Error"},          {0x110a, "Cache L2 Error"}
};

const std::map<uint8_t, const char*> decodeCtl = {
    {0x1, "L2"},  {0x2, "APIC HW"}, {0x4, "APIC SW"}, {0x8, "WBINVLD"}, {0x10, "Page Poison"}
};
} // namespace L2

namespace MEC
{
const std::map<uint32_t, const char*> decodeMsmcacod = {
    {0x10019, "AGUTLB - Parity Error"},
    {0x40135, "L1D - Uncorrected Tag Parity Error"},
    {0x60135, "DCFILL - Uncorrected Parity"},
    {0x90135, "L2/IDI Uncorrected Parity"},
    {0x80135, "DCWCB - GO-ERR"},
    {0x70135, "DCWCB - Poison"},
    {0x40134, "Tag- Data Error, Data Read"},
    {0x60134, "DCFILL - Data Error , Data Read"},
    {0x90134, "Bus to MEC- Data Error, Data Read"},
    {0x50134, "DCSTD Corrected Parity Error"},
    {0x70135, "DCWCB - Data Error, Data Read"},
    {0x30174, "Data Error Eviction"},
    {0x30134, "Data Error, Data Read"},
};

const std::map<uint32_t, const char*> decodeMsmcacodUn = {
    {0x30135, "L1D Data Error"},
    {0x30175, "L1D Data Error"},
    {0x50135, "DCSTD Parity Error"},
};

const std::map<uint8_t, const char*> decodeMscod = {
    {0x1, "Poisoned Data"},
    {0x2, "Other Transaction Error"},
    {0x4, "Data Parity Error"},
    {0x8, "Tag Parity Error"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x19, "GTLB Error"},
    {0x135, "L1 Data Read Error"},
    {0x165, "L1 Data Prefetch Error"},
    {0x175, "Data Cache L1 EVICT Error"},
    {0x185, "L1 Data Snoop Error"},
};

const std::map<uint8_t, const char*> decodeCtl = {
    {0x1, "Tag error"},
    {0x2, "SQDB/IDI Error"},
    {0x4, "Data Error"},
    {0x8, "Mesi Error"},
    {0x10, "Poison with poison FWD disabled"},
    {0x20, "IDI addr parity Errors"},
};
} // namespace MEC

namespace BUS
{
const std::map<uint32_t, const char*> decodeMsmcacod = {
    {0x00000400, "ROB Timeout"},
    {0x0000040b, "UROM Selfcheck Error"},
    {0x0100040b, "FuSA - Module Key-on/off Mem BIST Fail"},
    {0x0200040b, "FuSA - Module Key-on/off Scan BIST Fail"},
    {0x0300040b, "FuSA - Core Periodic Mem BIST Fail"},
    {0x0500040b, "FuSA - PLL Lock or VSS Power Loss"},
    {0x0600040b, "FuSA - Core Periodic Scan BIST Fail"},
};

const std::map<uint16_t, const char*> decodeMscodMcacod = {
    {0x4, "FuSA - Lockstep mis-compare"},
    {0x420, "IDI BGF Error"},
    {0x80c, "Go-ERR (CLFLUSH*/CLWB), U2C RSP Parity"},
    {0x810, "U2C Error (convert to poison)"},
    {0x81c, "Go-ERR (SetMonitor)"},
    {0x820, "Go-ERR / IDI Write Error, RSP Parity Error"},
    {0xa0f, "U2C Address Parity Error, HDR Parity Error"},
};

const std::map<uint8_t, const char*> decodeParticipation = {
    {0x0, "LOCAL_PROCESSOR_ORIGINATED_REQUEST"},
    {0x1, "LOCAL_PROCESSOR_RESPONDED_REQUEST"},
    {0x2, "LOCAL_PROCESSOR_OBSERVED_ERROR"},
    {0x3, "GENERIC"},
};

const std::map<uint8_t, const char*> decodeRequest = {
    {0x0, "GENERIC_ERROR"},
    {0x1, "GENERIC_READ"},
    {0x2, "GENERIC_WRITE"},
};

const std::map<uint8_t, const char*> decodeMemoryIo = {
    {0x0, "MEMORY_ACCESS"},
    {0x3, "TRANSACTION_BY_HARDWARE"},
};

const std::map<uint8_t, const char*> decodeLevel = {
    {0x0, "LEVEL_0"},
    {0x3, "GENERIC_ERROR_LEVEL_ENCODING"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x4, "FRC Error"},
    {0x400, "Watchdog Timer (3-strike timeout) Error"},
    {0x420, "Internal Unclassified Error"},
    {0x40b, "Internal Unclassified Error -BIST"},
    {0x80c, "BUSLG Error No Timeout Error"},
    {0x810, "BUS/Interconnect Error Memory Read"},
    {0x81c, "BUSL0 SRC RD No Timeout Error"},
    {0x820, "BUSL0 SRC WR M No Timeout Error"},
    {0xa0f, "BUSLG RES Error No Timeout Error"},
};

const std::map<uint16_t, const char*> decodeCtl = {
    {0x1, "PRF"},
    {0x2, "MS"},
    {0x4, "IQ"},
    {0x8, "BUS"},
    {0x10, "DSB"},
    {0x20, "IDQ"},
    {0x80, "BIT"},
    {0x200, "SDB"},
    {0x400, "EXE"},
    {0x800, "ES/ROB"},
    {0x1000, "ImmdFolding"},
};
} // namespace BUS

namespace UBOX
{
const std::map<uint16_t, const char*> decodeMscod = {
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
    {0x800C, "GPSB Parity Error"},
    {0x800D, "SAI Error"},
    {0x800E, "Semaphore Error"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0xe0b, "Generic I/O Error"},
    {0x405, "Generic I/O Error"},
    {0x406, "Internal Firmware errors"},
    {0x407, "General UBox error"},
    {0x40c, "Shutdown Error"},
};
} // namespace UBOX

namespace UPI
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x0000, "UC Phy Initialization Failure"},
    {0x0001, "UC Phy Detected Drift Buffer Alarm"},
    {0x0002, "UC Phy Detected Latency Buffer Rollover"},
    {0x0010, "UC LL Rx detected CRC error"},
    {0x0011, "UC LL Rx Unsupported/Undefined packet"},
    {0x0012, "UC LL or Phy Control Error: unexpected Tx Protocol flit"},
    {0x0013, "UC LL Rx Parameter Exception"},
    {0x0014, "UC LL TDX Failure"},
    {0x0015, "UC LL SGX Failure"},
    {0x0016, "UC LL TX SDC Parity Error"},
    {0x0017, "UC LL RX SDC Parity Error"},
    {0x0018, "UC LL FLE Failure"},
    {0x001F, "UC LL Detected Control Error from M3UPI"},
    {0x0020, "COR Phy Initialization Abort"},
    {0x0021, "COR Phy Reset"},
    {0x0022, "COR Phy Lane failure, recovery in x8 width"},
    {0x0023, "COR Phy L0c error"},
    {0x0024, "COR Phy L0c error triggering Phy Reset"},
    {0x0025, "COR Phy L0p exit error triggering Phy Reset"},
    {0x0030, "COR LL Rx detected CRC error: successful LLR without Phy Reinit"},
    {0x0031, "COR LL Rx detected CRC error: successful LLR with Phy Reinit"},
};

const std::map<uint16_t, const char*> decodeMcacod = {
    {0x0, "L0"},
    {0x1, "L1"},
    {0x2, "L2"},
    {0x3, "Generic LL"},
};

const std::map<uint8_t, const char*> decodeCorrerr = {
    {0x20, "COR Phy Initialization Abort"},
    {0x21, "COR Phy Reset"},
    {0x22, "COR Phy Lane failure"},
    {0x23, "COR Phy L0c error corrected without Phy reset"},
    {0x24, "COR Phy L0c error triggering Phy Reset"},
    {0x25, "COR Phy L0p exit error triggering Phy Reset"},
    {0x30, "COR LL Rx detected CRC error: successful LLR without Phy Reinit"},
    {0x31, "COR LL Rx detected CRC error: successful LLR with Phy Reinit"},
};

const std::map<uint16_t, const char*> decodeNerr = {
    {0x0, "UC Phy Initialization Failure (NumInit)"},
    {0x1, "UC Phy Detected Drift Buffer Alarm"},
    {0x2, "UC Phy Detected Latency Buffer Rollover"},
    {0x10, "UC LL Rx detected CRC error: unsuccessful LLR"},
    {0x11, "UC LL Rx Unsupported/Undefined packet"},
    {0x12, "UC LL or Phy Control Error"},
    {0x13, "UC LL Rx Parameter Exception"},
    {0x14, "UC LL TDX Failure"},
    {0x15, "UC LL SGX Failure"},
    {0x16, "UC LL Tx SDC Parity Error"},
    {0x17, "UC LL Rx SDC Parity Error"},
    {0x18, "UC LL FLE Error"},
    {0x1f, "UC LL detected Control Error From UFI2UPI"},
};

const std::map<uint8_t, const char*> decodePhystate = {
    {0x0, "24 lane flit mux phase 0"},
    {0x1, "24 lane flit mux phase 1"},
    {0x2, "24 lane flit mux phase 2"},
    {0x3, "24 lane flit mux phase 3"},
    {0x4, "24 lane flit mux phase 4"},
    {0x5, "Lower 8 logical lanes active in L0 (failover)"},
    {0x6, "Upper 8 logical lanes active in L0 (failover)"},
    {0x7, "Lower 8 logical lane active in L0p"},
};
} // namespace UPI

namespace PUNIT
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
    {0x402, "PUnit errors"},
    {0x405, "Internal errors"},
    {0x406, "Internal Firmware errors"},
    {0x40b, "Internal Firmware errors"},
    {0x40c, "Shutdown errors"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x02, "Power Management Unit MicroController Double-bit ECC ERROR"},
    {0x03, "Power Management Unit MicroController Double-bit ECC ERROR"},
    {0x08, "Power Management Unit MicroController ERROR"},
    {0x09, "Power Management Unit MicroController ERROR"},
    {0x0a, "Power Management Unit MicroController Patch load ERROR"},
    {0x0b, "Power Management Unit MicroController POReqValid ERROR"},
    {0x10, "Power Management Unit MicroController TeleSRAM Double-bit ECC ERROR"},
    {0x20, "Power Management Agent Signaled Error"},
    {0x80, "Non-Power Management Unit Generic Signaled Error"},
    {0xa0, "Power Management Unit HPMSRAM Double-bit ECC ERROR"},
};
} // namespace PUNIT

namespace B2CMI
{
const std::map<uint16_t, const char*> decodeMscod = {
    {0x00, "No error (default)"},
    {0x01, "Read ECC error (MemSpecRd, MemRd, MemRdData, MemRdXto*, MemInv, MemInvXto*, MemInvItoX)"},
    {0x02, "Bucket1 error"},
    {0x03, "Tracker Parity error"},
    {0x04, "Secure mismatch"},
    {0x07, "read completion parity error"},
    {0x08, "response parity error"},
    {0x09, "timeout error"},
    {0x0a, "CMI reserved credit pool error"},
    {0x0c, "CMI credit oversubscription error"},
    {0x0d, "SAI Mismatch Error"},
};

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

const std::map<uint8_t, const char*> decodeMemopcode = {
    {0x0, "MEMRD"},         {0x1, "MEMSPECRD"},    {0x2, "MEMRDDATA"},    {0x4, "MEMRDXTOS"},   {0x5, "MEMRDXTOI"},
    {0x6, "MEMRDXTOA"},     {0x8, "MEMINV"},       {0x9, "MEMINVXTOI"},   {0xa, "MEMINVXTOA"},  {0xc, "MEMINVITOX"},
    {0x10, "MEMWR"},        {0x13, "MEMWRNI"},     {0x14, "MEMWRPTL"},    {0x27, "MEMWRPTLNI"}, {0x15, "PATROL"},
    {0x1b, "MEMWRFLUSH"},   {0x2e, "DEALLOCSBRD"}, {0x2f, "DEALLOCSBPF"}, {0x28, "FMFILLRD"},   {0x30, "DIRUPD"},
    {0x31, "NMFILLWR"},     {0x33, "FMWR4NM"},     {0x37, "FMWRPTL4NM"},  {0x38, "FMEVICTWR"},  {0x3e, "DEALLOCSBWR"},
    {0x3f, "DEALLOCSBALL"},
};

const std::map<uint8_t, const char*> decodeBankindex = {
    {0x0, "B2CMI0"}, {0x1, "B2CMI1"}, {0x2, "B2CMI2"}, {0x3, "B2CMI3"}, {0x4, "B2CMI4"},  {0x5, "B2CMI5"},
    {0x6, "B2CMI6"}, {0x7, "B2CMI7"}, {0x8, "B2CMI8"}, {0x9, "B2CMI9"}, {0xa, "B2CMI10"}, {0xb, "B2CMI11"},
};

const std::map<uint8_t, const char*> decodeSmiopcode = {
    {0x0, "R: MemRd /W: MemWr"},
    {0x1, "R: MemSpecRd"},
    {0x2, "R: MemRdData"},
    {0x3, "W: MemWrNi"},
    {0x4, "R: MemRdXtoS /W: MemWrPtl"},
    {0x5, "R: MemRdXtoI"},
    {0x6, "R: MemRdXtoA"},
    {0x7, "W: MemWrPtlNI"},
    {0x8, "R: MemInv"},
    {0x9, "R: MemInvXtoI"},
    {0xa, "R: MemInvXtoA"},
    {0xb, "W: MemWrFlush"},
    {0xc, "R: MemInvItoX"},
};

const std::map<uint16_t, const char*> decodeErrortype = {
    {0x1, "Txn had a transient error (coorected by retry without ECC)"},
    {0x2, "Txn had an ECC Corected error( corrected by ECC during retry)"},
    {0x4, "Txn cound not be corrected by ECC"},
    {0x8, "Scrub check rd returned good data"},
    {0x10, "Scrub check rd returned uncorrected data"},
    {0x20, "Txn caused permanent hardware channel failover during mirroring"},
    {0x40, "Uncorrectable NM$ access because peristently uncorrectable meta-data"},
    {0x80, "Reserved"},
    {0x100, "Reserved"},
    {0x200, "Reserved"},
};
} // namespace B2CMI

namespace MCCHAN
{

const std::map<uint8_t, const char*> decodeEccmode = {
    {0x1, "SDDC 128b 1LM"}, {0x2, "SDDC 125b 1LM"}, {0x3, "SDDC 96b 1LM"},     {0x4, "SDDC 96b 2LM"},
    {0x5, "ADDDC 80b 1LM"}, {0x6, "ADDDC 80b 2LM"}, {0x7, "ADDDC 64b 1LM"},    {0x8, "9x4 61b 1LM"},
    {0x9, "9x4 32b 1LM"},   {0xa, "9x4 32b 2LM"},   {0x0, "Invalid ECC mode"},
};

const std::map<uint16_t, const char*> decodeErrSpecific = {
    {0x405, "Parity error on structures"},
};

const std::map<uint8_t, const char*> decodeMcacod157 = {
    {0x1, "(1LM mode)"},
    {0x5, "(2LM mode)"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x0001, "Address  Parity Error (APPP)"},
    {0x0002, "CMI Wr Data parity Error on sCH0"},
    {0x0003, "CMI Uncorr/cprr ECC error on sCH0"},
    {0x0004, "CMI Wr Be Parity Error on sCH0"},
    {0x0005, "CMI Wr MAC parity Error on sCH0"},
    {0x0008, "Corr Patrol Scrub Error"},
    {0x0010, "UnCorr Patrol Scrub Error"},
    {0x0020, "Corr Spare Error"},
    {0x0040, "UnCorr Spare Error"},
    {0x0080, "Transient or correctable Error for Demand or Underfill Reads"},
    {0x00a0, "Uncorrectable Error for Demand or Underfill Reads"},
    {0x00b0, "Poison was Read from memory when poison was disable in memory controller"},
    {0x00c0, "Read 2LM Metadata Error"},
    {0x0100, "WDB Read Parity Error on sCH0"},
    {0x0102, "WDB Read uncorr/corre ECC Error on sCH0"},
    {0x0104, "WBD BE Read Parity Error on SCH0"},
    {0x0106, "WBD Read Persistent Corr Ecc Error on sCH0"},
    {0x0108, "DDR/DDRT link fail"},
    {0x0109, "Iligal Incoming Opcode"},
    {0x0200, "DDR5 CA Parity"},
    {0x0400, "Scheduler Address Parity Error"},
    {0x0800, "Internal Propietary Errors"},
    {0x0801, "Internal Propietary Errors"},
    {0x0802, "Internal Propietary Errors"},
    {0x0803, "Internal Propietary Errors"},
    {0x0804, "Internal Propietary Errors"},
    {0x0805, "Internal Propietary Errors"},
    {0x0806, "Internal Propietary Errors"},
    {0x0807, "Internal Propietary Errors"},
    {0x0808, "Internal Propietary Errors"},
    {0x0809, "Internal Propietary Errors"},
    {0x080a, "Internal Propietary Errors"},
    {0x080b, "Internal Propietary Errors"},
    {0x080c, "Internal Propietary Errors"},
    {0x080d, "Internal Propietary Errors"},
    {0x080e, "Internal Propietary Errors"},
    {0x080f, "Internal Propietary Errors"},
    {0x0810, "Internal Propietary Errors"},
    {0x0811, "Internal Propietary Errors"},
    {0x0812, "Internal Propietary Errors"},
    {0x0813, "Internal Propietary Errors"},
    {0x0814, "Internal Propietary Errors"},
    {0x0815, "Internal Propietary Errors"},
    {0x0816, "Internal Propietary Errors"},
    {0x0817, "Internal Propietary Errors"},
    {0x0818, "Internal Propietary Errors"},
    {0x0819, "Internal Propietary Errors"},
    {0x081a, "Internal Propietary Errors"},
    {0x081b, "Internal Propietary Errors"},
    {0x081c, "Internal Propietary Errors"},
    {0x081d, "Internal Propietary Errors"},
    {0x081e, "Internal Propietary Errors"},
    {0x081f, "Internal Propietary Errors"},
    {0x0820, "Internal Propietary Errors"},
    {0x0821, "Internal Propietary Errors"},
    {0x0822, "Internal Propietary Errors"},
    {0x0823, "Internal Propietary Errors"},
    {0x0824, "Internal Propietary Errors"},
    {0x0825, "Internal Propietary Errors"},
    {0x0826, "Internal Propietary Errors"},
    {0x0827, "Internal Propietary Errors"},
    {0x0828, "Internal Propietary Errors"},
    {0x0829, "Internal Propietary Errors"},
    {0x082a, "Internal Propietary Errors"},
    {0x082b, "Internal Propietary Errors"},
    {0x082c, "Internal Propietary Errors"},
    {0x082d, "Internal Propietary Errors"},
    {0x082e, "Internal Propietary Errors"},
    {0x082f, "Internal Propietary Errors"},
    {0x0832, "MC Internal Error"},
    {0x0833, "MCTracker Address RF Parity Error"},
    {0x2002, "CMI Wr Data parity Error on sCH1"},
    {0x2003, "CMI Uncorr/cprr ECC error on sCH1"},
    {0x2004, "CMI Wr Be Parity Error on sCH1"},
    {0x2005, "CMI Wr MAC parity Error on sCH1"},
    {0x2100, "WDB Read Parity Error on sCH1"},
    {0x2102, "WDB Read uncorr/corre ECC Error on sCH1"},
    {0x2104, "WBD BE Read Parity Error on SCH1"},
    {0x2106, "WBD Read Persistent Corr Ecc Error on sCH1"},
};
} // namespace MCCHAN

namespace LLC
{
const std::map<uint16_t, const char*> decodeMcacod = {
    {0x136, "Error during data read (UCNA)"},
    {0x17a, "Error during Explicit Write Back (SRAO)"},
    {0x152, "Error during instruction read (UCNA)"},
};

const std::map<uint16_t, const char*> decodeMscod = {
    {0x00, "NO_ERROR"},
    {0x01, "UNCORRECTABLE_DATA_ERROR"},
    {0x02, "UNCORRECTABLE_TAG_ERR"},
    {0x07, "CORRECTABLE_DATA_ERROR"},
    {0x08, "MEM_POISON_DATA_ERROR"},
    {0x0a, "PARITY_DATA_ERROR"},
    {0x11, "LLC_TAG_CORR_ERR"},
    {0x12, "LLC_STATE_CORR_ERR"},
    {0x13, "LLC_STATE_UNCORR_ERR"},
    {0x21, "SF_TAG_UNCORR_ERR"},
    {0x22, "SF_TAG_CORR_ERR"},
    {0x23, "SF_STATE_CORR_ERR"},
    {0x24, "SF_STATE_UNCORR_ERR"},
    {0x28, "LLC_2LM_CORR_ERR"},
    {0x29, "LLC_2LM_UNCORR_ERR"},
    {0x31, "SF_2LM_CORR_ERR"},
    {0x32, "SF_2LM_UNCORR_ERR"},
    {0x39, "RSF_ST_CORR_ERROR"},
    {0x3a, "RSF_TAG_UNCORR_ERROR"},
    {0x3b, "RSF_ST_UNCORR_ERROR"},
    {0x3c, "RSF_TAG_CORR_ERROR"},
};
} // namespace LLC

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
    {0x0016, "MULT_TOR_ENTRY_ERROR"},
    {0x0017, "MULT_LLC_WAY_TAG_MATCH"},
    {0x0018, "BL_REQ_RTID_TABLE_MISS"},
    {0x0019, "AK_REQ_RTID_TABLE_MISS"},
    {0x001C, "IDI_JITTER_ERROR"},
    {0x001F, "Internal Parity Error"},
    {0x0025, "SF_CV_CORR_ERROR"},
    {0x0026, "SF_CV_UNCORR_ERROR"},
    {0x0027, "SAD_ERR_LTMEMLOCK"},
    {0x002A, "ISMQ_UNEXP_RSP"},
    {0x002B, "2LM_MULT_HIT"},
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

} // namespace SrfDefs
