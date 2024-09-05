/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2022 Intel Corporation.
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

extern "C" {
#include <peci.h>
#include <safe_mem_lib.h>
}

#include <chrono>
#include <thread>

class FakePeciBase
{
  public:
    virtual ~FakePeciBase()
    {
    }

    virtual EPECIStatus peci_CrashDump_Discovery(
        uint8_t target, uint8_t subopcode, uint8_t param0, uint16_t param1,
        uint8_t param2, uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc) = 0;

    virtual EPECIStatus peci_CrashDump_GetFrame(
        uint8_t target, uint16_t param0, uint16_t param1, uint16_t param2,
        uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc) = 0;

    virtual EPECIStatus peci_RdEndPointConfigPciLocal_seq(
        uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
        uint8_t u8Fcn, uint16_t u16Reg, uint8_t u8ReadLen, uint8_t* pPCIData,
        int peci_fd, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_RdEndPointConfigMmio(
        uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
        uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType, uint64_t u64Offset,
        uint8_t u8ReadLen, uint8_t* pMmioData, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_RdEndPointConfigMmio_seq(
        uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
        uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType, uint64_t u64Offset,
        uint8_t u8ReadLen, uint8_t* pMmioData, int peci_fd, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_RdIAMSR(uint8_t target, uint8_t threadID,
                                     uint16_t MSRAddress, uint64_t* u64MsrVal,
                                     uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_CrashDump_GetFrame_GoodReturnWithDelay(
        uint8_t target, uint16_t param0, uint16_t param1, uint16_t param2,

        uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_CrashDump_GetFrame_GoodReturnRetryCCWithDelay(
        uint8_t target, uint16_t param0, uint16_t param1, uint16_t param2,
        uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_RdPkgConfig_GoodReturnWithDelay(
        uint8_t target, uint8_t u8Index, uint16_t u16Value, uint8_t u8ReadLen,
        uint8_t* pPkgConfig, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    
    virtual EPECIStatus peci_WrPkgConfig(uint8_t target, uint8_t u8Index,
                                         uint16_t u16Param, uint32_t u32Value,
                                         uint8_t u8WriteLen, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }

    virtual EPECIStatus peci_RdPkgConfig(uint8_t target, uint8_t u8Index,
                                         uint16_t u16Param, uint32_t u32Value,
                                         uint8_t u8WriteLen, uint8_t* cc)
    {
        return PECI_CC_SUCCESS;
    }
};

class FakePeci : public FakePeciBase
{
  public:
    virtual ~FakePeci()
    {
    }

    EPECIStatus peci_CrashDump_Discovery(uint8_t target, uint8_t subopcode,
                                         uint8_t param0, uint16_t param1,
                                         uint8_t param2, uint8_t u8ReadLen,
                                         uint8_t* pData, uint8_t* cc) override
    {
        *cc = 0x40;
        uint8_t data[8] = {0};
        memcpy_s(pData, 8, data, 8);
        return PECI_CC_SUCCESS;
    }

    EPECIStatus peci_CrashDump_GetFrame(uint8_t target, uint16_t param0,
                                        uint16_t param1, uint16_t param2,
                                        uint8_t u8ReadLen, uint8_t* pData,
                                        uint8_t* cc) override
    {
        *cc = 0x40;
        uint8_t data[8] = {0xde, 0xad, 0xbe, 0xef};
        memcpy_s(pData, 8, data, 8);
        return PECI_CC_SUCCESS;
    }
};

class PeciWithDelay : public FakePeciBase
{
  public:
    PeciWithDelay(int delayMilliseconds)
    {
        delayMilliseconds_ = delayMilliseconds;
    }

    virtual ~PeciWithDelay()
    {
    }

    EPECIStatus peci_CrashDump_Discovery(uint8_t target, uint8_t subopcode,
                                         uint8_t param0, uint16_t param1,
                                         uint8_t param2, uint8_t u8ReadLen,
                                         uint8_t* pData, uint8_t* cc) override
    {
        return PECI_CC_SUCCESS;
    }

    EPECIStatus peci_CrashDump_GetFrame(uint8_t target, uint16_t param0,
                                        uint16_t param1, uint16_t param2,
                                        uint8_t u8ReadLen, uint8_t* pData,
                                        uint8_t* cc) override
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        *cc = PECI_DEV_CC_NEED_RETRY;
        uint8_t data[8] = {0xab, 0xfa, 0xed, 0xfe,
                           0xef, 0xbe, 0xad, 0xde}; // 0xdeadbeeffeedfaab;
        memcpy_s(pData, 8, data, 8);

        return PECI_CC_TIMEOUT;
    }

    EPECIStatus peci_CrashDump_GetFrame_GoodReturnWithDelay(
        uint8_t target, uint16_t param0, uint16_t param1, uint16_t param2,
        uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc) override
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        *cc = PECI_CC_SUCCESS;
        uint8_t data[8] = {0x03, 0x90, 0x02, 0x04,
                           0xb0, 0x05, 0x00, 0x03}; // 0x030005b004029003;
        memcpy_s(pData, 8, data, 8);

        return PECI_CC_SUCCESS;
    }

    EPECIStatus peci_CrashDump_GetFrame_GoodReturnRetryCCWithDelay(
        uint8_t target, uint16_t param0, uint16_t param1, uint16_t param2,
        uint8_t u8ReadLen, uint8_t* pData, uint8_t* cc) override
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        *cc = PECI_DEV_CC_NEED_RETRY;
        uint8_t data[8] = {0x03, 0x90, 0x02, 0x04,
                           0xb0, 0x05, 0x00, 0x03}; // 0x030005b004029003;
        memcpy_s(pData, 8, data, 8);

        return PECI_CC_SUCCESS;
    }

    EPECIStatus peci_RdEndPointConfigPciLocal_seq(
        uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
        uint8_t u8Fcn, uint16_t u16Reg, uint8_t u8ReadLen, uint8_t* pPCIData,
        int peci_fd, uint8_t* cc) override
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        uint8_t returnValue[2] = {0x86, 0x80}; // 0x8086;
        memcpy_s(pPCIData, 2, returnValue, 2);
        *cc = PECI_DEV_CC_NEED_RETRY;
        return PECI_CC_TIMEOUT;
    }

    EPECIStatus peci_RdEndPointConfigMmio_seq(
        uint8_t target, uint8_t u8Seg, uint8_t u8Bus, uint8_t u8Device,
        uint8_t u8Fcn, uint8_t u8Bar, uint8_t u8AddrType, uint64_t u64Offset,
        uint8_t u8ReadLen, uint8_t* pMmioData, int peci_fd,
        uint8_t* cc) override
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        uint8_t returnValue[2] = {0x86, 0x80}; // 0x8086;
        memcpy_s(pMmioData, 2, returnValue, 2);
        *cc = PECI_DEV_CC_NEED_RETRY;
        return PECI_CC_TIMEOUT;
    }

    EPECIStatus peci_RdIAMSR(uint8_t target, uint8_t threadID,
                             uint16_t MSRAddress, uint64_t* u64MsrVal,
                             uint8_t* cc) override
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        uint8_t returnValue[2] = {0x86, 0x80}; // 0x8086;
        memcpy_s(u64MsrVal, 2, returnValue, 2);
        *cc = PECI_DEV_CC_NEED_RETRY;
        return PECI_CC_TIMEOUT;
    }

    EPECIStatus peci_RdPkgConfig_GoodReturnWithDelay(
        uint8_t target, uint8_t u8Index, uint16_t u16Value, uint8_t u8ReadLen,
        uint8_t* pPkgConfig, uint8_t* cc)
    {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(delayMilliseconds_));

        *pPkgConfig = 1;
        *cc = PECI_DEV_CC_NEED_RETRY;
        return PECI_CC_SUCCESS;
    }

  private:
    int delayMilliseconds_;
};