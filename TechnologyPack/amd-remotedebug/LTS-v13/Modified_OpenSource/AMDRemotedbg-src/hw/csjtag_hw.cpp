/******************************************************************************/
/*! \file bmc/hw/csjtag_hw.cpp BMC CSJTAG HW driver
 *
 * <pre>
 * Copyright (C) 2011-2020, Advanced Micro Devices, Inc.
 * AMD Confidential Proprietary.
 * </pre>
 ******************************************************************************/

#include <cstring>
#include <sys/ioctl.h>
#include "csjtag_hw.h"

bmc::csJtagHw::csJtagHw(int driver)
{
    fd = driver;
}

/***************************************************************/

uint32_t bmc::csJtagHw::isConnected(bool& connected, char * mode)
{
    connected = true;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::isInConflict(bool& conflicted)
{
    conflicted = false;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::shift(enum yaap::hal::jtagOp operation, uint8_t *inData, uint8_t *outData, int bits, int preTck, int postTck, enum yaap::hal::jtagState termState)
{
    // not used for iHDT , if required please see jtag_hw.cpp for similar implementation.

    if (operation == yaap::hal::JTAG_OP_SHIFT_IR)
    {
    }
    else if (operation == yaap::hal::JTAG_OP_SHIFT_DR)
    {
    }
    else
    {
        return E_SERVER_ERROR;
    }

    m_state = termState;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::resetTap(void)
{
    m_state = yaap::hal::JTAG_STATE_TLR;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::tapStateSet(enum yaap::hal::jtagState state)
{
    m_state = state;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::tapStateGet(enum yaap::hal::jtagState& state)
{
    state = m_state;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::tckFreqSet(uint32_t& freqHz)
{
    if (freqHz > 5000000)
    {
        freqHz = 5000000;
    }
    freqHz = freqHz + 1;  // Test feedback channel
    m_freq = freqHz;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::csJtagHw::tckFreqGet(uint32_t& freqHz)
{
    freqHz = m_freq;
    return E_SUCCESS;
}

/***************************************************************/
