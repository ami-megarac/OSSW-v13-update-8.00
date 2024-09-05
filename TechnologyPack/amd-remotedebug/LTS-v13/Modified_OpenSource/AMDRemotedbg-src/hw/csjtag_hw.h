/******************************************************************************/
/*! \file bmc/hw/csjtag_hw.h BMC CSJTAG HW driver
 *
 * <pre>
 * Copyright (C) 2011-2020, Advanced Micro Devices, Inc.
 * AMD Confidential Proprietary.
 * </pre>
 ******************************************************************************/

#ifndef BMC_CSJTAG_HW_H
#define BMC_CSJTAG_HW_H

#include "aspeed.h"
#include "hal/jtag.h"

using namespace yaap::hal;

namespace bmc
{
class csJtagHw : public IJtag, public IJtag_tapState, public IJtag_tckFrequency
{
   private:
    enum yaap::hal::jtagState m_state;
    uint32_t m_freq;
    int fd;

   public:
    csJtagHw(int driver);
    uint32_t isConnected(bool& connected, char* mode);
    uint32_t isInConflict(bool& conflicted);
    uint32_t shift(enum yaap::hal::jtagOp operation, uint8_t* inData, uint8_t* outData, int bits, int preTck, int postTck, enum yaap::hal::jtagState termState);
    uint32_t resetTap(void);
    uint32_t tapStateSet(enum yaap::hal::jtagState state);
    uint32_t tapStateGet(enum yaap::hal::jtagState& state);
    uint32_t tckFreqSet(uint32_t& freqHz);
    uint32_t tckFreqGet(uint32_t& freqHz);
};
}  // namespace bmc

#endif  // BMC_CSJTAG_HW_H
