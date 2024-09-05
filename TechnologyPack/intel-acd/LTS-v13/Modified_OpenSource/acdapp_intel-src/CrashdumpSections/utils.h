/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2019 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 *
 ******************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include "cJSON.h"
#include <stdbool.h>
#include <stdint.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/file.h>

#include "crashdump.h"

#define ACDPDK_LIB 		"/usr/local/lib/libacdcfgpdk.so"
#define ERRCHECKMAXCOUNT        10 //1 second //300 //30 seconds - (delay of 100ms)*300

#define DIR_NAME_LEN 		128
#define ACD_INSTANCE_LOCK 	"crashdump-lock"
#define DEFAULT_INPUT_FILE "/var/crashdump/input/crashdump_input_%s.json"
#define OVERRIDE_INPUT_FILE "/var/crashdump/input/crashdump_input_%s.json"
#define DEFAULT_TELEMETRY_FILE "/var/crashdump/input/telemetry_input_%s.json"
#define OVERRIDE_TELEMETRY_FILE "/var/crashdump/input/telemetry_input_%s.json"
#define COPY_JSON_FILE "cp /home/crashdump/input/crashdump_input_%s.json /var/crashdump/input/"

#define ACD_INSTANCE_LOCK   "crashdump-lock"
#define PECI_CC_SKIP_CORE(cc)                                                  \
    (((cc) == PECI_DEV_CC_CATASTROPHIC_MCA_ERROR ||                            \
      (cc) == PECI_DEV_CC_NEED_RETRY || (cc) == PECI_DEV_CC_OUT_OF_RESOURCE || \
      (cc) == PECI_DEV_CC_UNAVAIL_RESOURCE ||                                  \
      (cc) == PECI_DEV_CC_INVALID_REQ || (cc) == PECI_DEV_CC_MCA_ERROR)        \
         ? true                                                                \
         : false)

#define PECI_CC_SKIP_SOCKET(cc)                                                \
    (((cc) == PECI_DEV_CC_PARITY_ERROR_ON_GPSB_OR_PMSB ||                      \
      (cc) == PECI_DEV_CC_PARITY_ERROR_ON_GPSB_OR_PMSB_IERR ||                 \
      (cc) == PECI_DEV_CC_PARITY_ERROR_ON_GPSB_OR_PMSB_MCA)                    \
         ? true                                                                \
         : false)

#define PECI_CC_UA(cc)                                                         \
    (((cc) != PECI_DEV_CC_SUCCESS && (cc) != PECI_DEV_CC_FATAL_MCA_DETECTED)   \
         ? true                                                                \
         : false)

#define FREE(ptr)                                                              \
    do                                                                         \
    {                                                                          \
        free(ptr);                                                             \
        ptr = NULL;                                                            \
    } while (0)

#define SET_BIT(val, pos) ((val) |= ((uint64_t)1 << ((uint64_t)pos)))
#define CLEAR_BIT(val, pos) ((val) &= ~((uint64_t)1 << ((uint64_t)pos)))
#define CHECK_BIT(val, pos) ((val) & ((uint64_t)1 << ((uint64_t)pos)))
#define CPU_STR_LEN 7
#define NAME_STR_LEN 255
#define DEFAULT_VALUE -1

void setFields(uint32_t* value, uint32_t msb, uint32_t lsb, uint32_t inputVal);
uint32_t getFields(uint32_t value, uint32_t msb, uint32_t lsb);
uint32_t bitField(uint32_t offset, uint32_t size, uint32_t val);
int fillInputFile(CPUInfo* cpuInfo, char* cSectionName, cJSON* pJsonChild,
                  InputFileInfo* inputFileInfo);
int logResetDetected(cJSON* metadata, CPUInfo cpuInfo,
                     PlatformState platformState);
int fillMeVersion(char* cSectionName, cJSON* pJsonChild);
int fillCrashdumpVersion(char* cSectionName, cJSON* pJsonChild);
void only_one_instance(char *crashdump_json_dir);
// crashdump_input_xxx.json #define(s) and c-helper functions
#define RECORD_ENABLE "_record_enable"
#define RECORD_ENABLE_SECTIONS "RecordEnable"
#define CD_ENABLE 0
#define ENABLE 0
#define UT_REG_NAME_LEN 32
#define UT_REG_DWORD 4
#define UT_REG_QWORD 8
#define SI_JSON_STRING_LEN 48
#define SI_JSON_SOCKET_NAME "cpu%d"
#define MD_NA "N/A"
#define RESET_DETECTED_DEFAULT "NONE"
#define INPUT_FILE_ERROR_STR "Error while reading input file"
#define SI_BMC_VER_LEN 64
#define SI_BIOS_ID_LEN 64
#define SUMMARY_ENABLE "SummaryEnable"
#define TRIAGE_ENABLE "TriageEnable"

typedef enum
{
    EXECUTION_ALL_REGISTERS,
    EXECUTION_TILL_ABORT,
    EXECUTION_ABORTED
} executionStatus;

typedef enum
{
    FLAG_ENABLE,
    FLAG_DISABLE,
    FLAG_NOT_PRESENT
} inputField;

typedef union
{
    uint64_t u64;
    uint32_t u32[2];
} SRegValue;

typedef struct
{
    SRegValue uValue;
    uint8_t cc;
    int ret;
} SRegRawData;

typedef struct
{
    char regName[UT_REG_NAME_LEN];
    uint8_t u8Seg;
    uint8_t u8Bus;
    uint8_t u8Dev;
    uint8_t u8Func;
    uint16_t u16Reg;
    uint8_t u8Size;
} SRegPci;

static const SRegPci sPciReg[] = {
    // Register, Seg, Bus, Dev, Func, Offset, Size
    {"ierrloggingreg", 0, 30, 0, 0, 0xA4, 4},
    {"mcerrloggingreg", 0, 30, 0, 0, 0xA8, 4},
    {"firstierrtsc", 0, 31, 30, 4, 0xFCF8, 8},
    {"firstmcerrtsc", 0, 31, 30, 4, 0xF4F0, 8},
    {"mca_err_src_log", 0, 31, 30, 2, 0xEC, 4},
};

extern struct timespec crashdumpStart;

void updateRecordEnable(cJSON* root, bool enable);
cJSON* getCrashDataSection(cJSON* root, char* section, bool* enable);
cJSON* getCrashDataSectionRegList(cJSON* root, char* section, char* regType,
                                  bool* enable);
int getCrashDataSectionVersion(cJSON* root, char* section);
cJSON* getNewCrashDataSection(cJSON* root, char* section);
cJSON* getNewCrashDataSectionObjectOneLevel(cJSON* root, char* section,
                                            const char* firstLevel);
bool readInputFileFlag(cJSON* pJsonChild, bool defaultValue, char* sectionName);
cJSON* readInputFile(const char* filename);
int cd_snprintf_s(char* str, size_t len, const char* format, ...);
cJSON* getCrashDataSectionBigCoreRegList(cJSON* root, char* version);
bool isBigCoreRegVersionMatch(cJSON* root, uint32_t version);
uint32_t getCrashDataSectionBigCoreSize(cJSON* root, char* version);
void storeCrashDataSectionBigCoreSize(cJSON* root, char* version,
                                      uint32_t totalSize);
cJSON* getCrashDataSectionObject(cJSON* root, char* section, char* firstLevel,
                                 char* secondLevel, bool* enable);
 void only_one_instance(char *crashdump_json_dir);
cJSON* selectAndReadInputFile(Model cpuModel, char** filename,
                              bool isTelemetry);
cJSON* getCrashDataSectionAddressMapRegList(cJSON* root);
cJSON* getCrashDataSectionObjectOneLevel(cJSON* root, char* section,
                                         const char* firstLevel, bool* enable);
void addDelayToSection(cJSON* cpu, CPUInfo* cpuInfo, char* sectionName,
                       struct timespec* crashdumpStart);
uint64_t tsToNanosecond(struct timespec* ts);
void updateMcaRunTime(cJSON* root, struct timespec* start);
struct timespec calculateTimeRemaining(uint32_t maxWaitTimeFromInputFileInSec);
uint32_t getDelayFromInputFile(CPUInfo* cpuInfo, char* sectionName);
int getPciRegister(CPUInfo* cpuInfo, SRegRawData* sRegData, uint8_t u8index);
bool getSkipFromInputFile(CPUInfo* cpuInfo, char* sectionName);
bool getSkipFromNewInputFile(CPUInfo* cpuInfo, char* sectionName);
cJSON* getPeciAccessType(cJSON* root);
inputField getFlagValueFromInputFile(CPUInfo* cpuInfo, char* sectionName,
                                     char* flagName);
uint32_t getCollectionTimeFromInputFile(CPUInfo* cpuInfo);

executionStatus checkMaxTimeElapsed(uint32_t maxTime,
                                    struct timespec sectionStartTime);
double getTimeRemainingFromStart(uint32_t maxTime,
                                 struct timespec sectionStartTime);
void adjustTotalTime(struct timespec* crashdumpTime);
uint8_t getNumberOfSections(CPUInfo* cpuInfo);
int cJSONToInt(cJSON* cjonObj, int base);
/* NVD Related functions */
cJSON* getNVDSection(cJSON* root, const char* const section,
                     bool* const enable);
cJSON* getNVDSectionRegList(cJSON* root, const char* const section,
                            bool* const enable);
cJSON* getPMEMSectionErrLogList(cJSON* root, const char* const section,
                                bool* const enable);

/* Crashlog Related functions */
cJSON* getCrashlogExcludeList(cJSON* root);
cJSON* getCrashlogAgentsFromInputFile(cJSON* root);
uint8_t getMaxCollectionCoresFromInputFile(CPUInfo* cpuInfo);

/* SPRHBM Related functions */
bool isSprHbm(const CPUInfo* cpuInfo);

#endif // UTILS_H
