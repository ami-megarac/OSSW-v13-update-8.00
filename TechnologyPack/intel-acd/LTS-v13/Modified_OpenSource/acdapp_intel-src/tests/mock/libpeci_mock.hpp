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

#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

extern "C" {

#include "engine/crashdump.h"
}

#include "fake_peci.hpp"

using namespace ::testing;
using ::testing::Return;

class LibPeciMock
{
  public:
    LibPeciMock()
    {
        fake_ = std::make_unique<FakePeci>();
    }

    virtual ~LibPeciMock()
    {
    }

    MOCK_METHOD8(peci_CrashDump_Discovery,
                 EPECIStatus(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t,
                             uint8_t, uint8_t*, uint8_t*));

    MOCK_METHOD5(peci_RdIAMSR,
                 EPECIStatus(uint8_t, uint8_t, uint16_t, uint64_t*, uint8_t*));

    MOCK_METHOD6(peci_RdPkgConfig, EPECIStatus(uint8_t, uint8_t, uint16_t,
                                               uint8_t, uint8_t*, uint8_t*));

    MOCK_METHOD7(peci_CrashDump_GetFrame,
                 EPECIStatus(uint8_t, uint16_t, uint16_t, uint16_t, uint8_t,
                             uint8_t*, uint8_t*));

    MOCK_METHOD1(peci_Ping, EPECIStatus(uint8_t));

    MOCK_METHOD4(peci_GetCPUID,
                 EPECIStatus(const uint8_t, CPUModel*, uint8_t*, uint8_t*));

    MOCK_METHOD1(peci_Unlock, void(int));

    MOCK_METHOD2(peci_Lock, EPECIStatus(int*, int));

    MOCK_METHOD10(peci_RdEndPointConfigPciLocal_seq,
                  EPECIStatus(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,
                              uint16_t, uint8_t, uint8_t*, int, uint8_t*));

    MOCK_METHOD(EPECIStatus, peci_RdEndPointConfigMmio,
                (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,
                 uint64_t, uint8_t, uint8_t*, uint8_t*));

    MOCK_METHOD(EPECIStatus, peci_RdEndPointConfigMmio_seq,
                (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t,
                 uint64_t, uint8_t, uint8_t*, int, uint8_t*));

    MOCK_METHOD(EPECIStatus, peci_RdEndPointConfigPciLocal,
                (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint8_t,
                 uint8_t*, uint8_t*));

    MOCK_METHOD(EPECIStatus, peci_WrPkgConfig,
                (uint8_t, uint8_t, uint16_t, uint32_t, uint8_t, uint8_t*));

    MOCK_METHOD8(peci_Telemetry_Discovery,
                 EPECIStatus(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t,
                             uint8_t, uint8_t*, uint8_t*));

    MOCK_METHOD6(peci_Telemetry_GetCrashlogSample,
                 EPECIStatus(uint8_t, uint16_t, uint16_t, uint8_t, uint8_t*,
                             uint8_t*));

    MOCK_METHOD6(peci_Telemetry_ConfigWatcherRd,
                 EPECIStatus(uint8_t, uint16_t, uint16_t, uint8_t, uint8_t*,
                             uint8_t*));

    MOCK_METHOD6(peci_Telemetry_ConfigWatcherWr,
                 EPECIStatus(uint8_t, uint16_t, uint16_t, uint8_t, uint8_t*,
                             uint8_t*));

    void DelegateToFakeCrashdumpDiscovery()
    {
        ON_CALL(*this, peci_CrashDump_Discovery)
            .WillByDefault([this](uint8_t target, uint8_t subopcode,
                                  uint8_t param0, uint16_t param1,
                                  uint8_t param2, uint8_t u8ReadLen,
                                  uint8_t* pData, uint8_t* cc) {
                return fake_->peci_CrashDump_Discovery(target, subopcode,
                                                       param0, param1, param2,
                                                       u8ReadLen, pData, cc);
            });
    }

    void DelegateToFakeGetFrame()
    {
        ON_CALL(*this, peci_CrashDump_GetFrame)
            .WillByDefault([this](uint8_t target, uint16_t param0,
                                  uint16_t param1, uint16_t param2,
                                  uint8_t u8ReadLen, uint8_t* pData,
                                  uint8_t* cc) {
                return fake_->peci_CrashDump_GetFrame(
                    target, param0, param1, param2, u8ReadLen, pData, cc);
            });
    }

    virtual void DelegateForUncorePci(){};
    virtual void DelegateForUncoreMmio(){};
    virtual void DelegateForUncoreRdIAMSR(){};
    virtual void DelegateForBigCore(){};
    virtual void DelegateForBigCoreWithGoodReturnWithDelay(){};
    virtual void
        DelegateForBigCoreWithGoodReturnNeedRetryCompletionCodeWithDelay(){};
    virtual void DelegateForCrashdumpSetUp(){};
    virtual void DelegateForCrashdumpSetUpFor2Sockets(){};

  private:
    std::unique_ptr<FakePeciBase> fake_;
};

class LibPeciMockWithDelay : public LibPeciMock
{
  public:
    LibPeciMockWithDelay(int delay)
    {
        peciWithDelay_ = std::make_unique<PeciWithDelay>(delay);
    }

    virtual ~LibPeciMockWithDelay()
    {
    }

    void DelegateForUncorePci()
    {
        ON_CALL(*this, peci_RdEndPointConfigPciLocal_seq)
            .WillByDefault([this](uint8_t target, uint8_t u8Seg, uint8_t u8Bus,
                                  uint8_t u8Device, uint8_t u8Fcn,
                                  uint16_t u16Reg, uint8_t u8ReadLen,
                                  uint8_t* pPCIData, int peci_fd, uint8_t* cc) {
                return peciWithDelay_->peci_RdEndPointConfigPciLocal_seq(
                    target, u8Seg, u8Bus, u8Device, u8Fcn, u16Reg, u8ReadLen,
                    pPCIData, peci_fd, cc);
            });
    }

    void DelegateForUncoreMmio()
    {
        ON_CALL(*this, peci_RdEndPointConfigMmio_seq)
            .WillByDefault([this](uint8_t target, uint8_t u8Seg, uint8_t u8Bus,
                                  uint8_t u8Device, uint8_t u8Fcn,
                                  uint8_t u8Bar, uint8_t u8AddrType,
                                  uint64_t u64Offset, uint8_t u8ReadLen,
                                  uint8_t* pMmioData, int peci_fd,
                                  uint8_t* cc) {
                return peciWithDelay_->peci_RdEndPointConfigMmio_seq(
                    target, u8Seg, u8Bus, u8Device, u8Fcn, u8Bar, u8AddrType,
                    u64Offset, u8ReadLen, pMmioData, peci_fd, cc);
            });
    }

    void DelegateForUncoreRdIAMSR()
    {
        ON_CALL(*this, peci_RdIAMSR)
            .WillByDefault([this](uint8_t target, uint8_t threadID,
                                  uint16_t MSRAddress, uint64_t* u64MsrVal,
                                  uint8_t* cc) {
                return peciWithDelay_->peci_RdIAMSR(target, threadID,
                                                    MSRAddress, u64MsrVal, cc);
            });
    }

    void DelegateForBigCore()
    {

        ON_CALL(*this, peci_CrashDump_GetFrame)
            .WillByDefault([this](uint8_t target, uint16_t param0,
                                  uint16_t param1, uint16_t param2,
                                  uint8_t u8ReadLen, uint8_t* pData,
                                  uint8_t* cc) {
                return peciWithDelay_->peci_CrashDump_GetFrame(
                    target, param0, param1, param2, u8ReadLen, pData, cc);
            });
    }

    void DelegateForBigCoreWithGoodReturnWithDelay()
    {

        ON_CALL(*this, peci_CrashDump_GetFrame)
            .WillByDefault([this](uint8_t target, uint16_t param0,
                                  uint16_t param1, uint16_t param2,
                                  uint8_t u8ReadLen, uint8_t* pData,
                                  uint8_t* cc) {
                return peciWithDelay_
                    ->peci_CrashDump_GetFrame_GoodReturnWithDelay(
                        target, param0, param1, param2, u8ReadLen, pData, cc);
            });
    }

    void DelegateForBigCoreWithGoodReturnNeedRetryCompletionCodeWithDelay()
    {

        ON_CALL(*this, peci_CrashDump_GetFrame)
            .WillByDefault([this](uint8_t target, uint16_t param0,
                                  uint16_t param1, uint16_t param2,
                                  uint8_t u8ReadLen, uint8_t* pData,
                                  uint8_t* cc) {
                return peciWithDelay_
                    ->peci_CrashDump_GetFrame_GoodReturnRetryCCWithDelay(
                        target, param0, param1, param2, u8ReadLen, pData, cc);
            });
    }

    void DelegateForRdPkgConfigGoodReturnWithDelay()
    {
        ON_CALL(*this, peci_RdPkgConfig)
            .WillByDefault([this](uint8_t target, uint8_t u8Index,
                                  uint16_t u16Value, uint8_t u8ReadLen,
                                  uint8_t* pPkgConfig, uint8_t* cc) {
                return peciWithDelay_->peci_RdPkgConfig_GoodReturnWithDelay(
                    target, u8Index, u16Value, u8ReadLen, pPkgConfig, cc);
            });
    }

    void CrashdumpDiscoveryTORSetUp()
    {
        uint8_t cc = 0x80;
        uint8_t u8CrashdumpEnabled = 0;
        uint16_t u16CrashdumpNumAgents = 2;
        uint8_t u64UniqueId[8] = {0xFF, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00};
        uint8_t PayloadExp[8] = {0x03, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00};

        EXPECT_CALL(*this, peci_CrashDump_Discovery)
            .WillOnce(DoAll(SetArgPointee<6>(u8CrashdumpEnabled),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArgPointee<6>(u16CrashdumpNumAgents),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<6>(u64UniqueId, u64UniqueId + 8),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArrayArgument<6>(PayloadExp, PayloadExp + 8),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));
    }

    void CrashdumpDiscoveryBigCoreSetUp()
    {
        uint8_t cc = 0x80;
        uint8_t u8CrashdumpDisabled = 0;
        uint16_t u16CrashdumpNumAgentsBigCore = 2;
        uint64_t u64UniqueIdBigCore = 0xabcdef;
        uint64_t u64PayloadExp = 0x2b0;
        EXPECT_CALL(*this, peci_CrashDump_Discovery)
            .WillOnce(DoAll(SetArgPointee<6>(u8CrashdumpDisabled),
                            Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArgPointee<6>(u16CrashdumpNumAgentsBigCore),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArgPointee<6>(u64UniqueIdBigCore),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)))
            .WillOnce(DoAll(SetArgPointee<6>(u64PayloadExp),
                            SetArgPointee<7>(cc), Return(PECI_CC_SUCCESS)));
    }

    void DelegateForCrashdumpSetUp()
    {
        InSequence seq;

        DelegateForUncorePci();
        DelegateForUncoreMmio();
        DelegateForUncoreRdIAMSR();
        CrashdumpDiscoveryTORSetUp();
        CrashdumpDiscoveryBigCoreSetUp();
        DelegateForRdPkgConfigGoodReturnWithDelay();
        DelegateForBigCoreWithGoodReturnNeedRetryCompletionCodeWithDelay();
    }

    void DelegateForCrashdumpSetUpFor2Sockets()
    {
        InSequence seq;

        DelegateForUncorePci();
        DelegateForUncoreMmio();
        DelegateForUncoreRdIAMSR();
        for (int i = 0; i < 2; i++)
        {
            CrashdumpDiscoveryTORSetUp();
        }
        DelegateForRdPkgConfigGoodReturnWithDelay();
        for (int i = 0; i < 2; i++)
        {
            CrashdumpDiscoveryBigCoreSetUp();
        }
        DelegateForBigCoreWithGoodReturnNeedRetryCompletionCodeWithDelay();
    }

  private:
    std::unique_ptr<FakePeciBase> peciWithDelay_;
};