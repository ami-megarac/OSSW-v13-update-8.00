/******************************************************************************/
/*! \file bmc/hw/hdt_hw.cpp bmc:: HDT HW driver
 *
 * <pre>
 * Copyright (C) 2011-2021, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 * AMD Confidential Proprietary
 *
 * AMD is granting you permission to use this software (the Materials)
 * pursuant to the terms and conditions of your Software License Agreement
 * with AMD.  This header does *NOT* give you permission to use the Materials
 * or any rights under AMD's intellectual property.  Your use of any portion
 * of these Materials shall constitute your acceptance of those terms and
 * conditions.  If you do not agree to the terms and conditions of the Software
 * License Agreement, please do not use any portion of these Materials.
 *
 * CONFIDENTIALITY:  The Materials and all other information, identified as
 * confidential and provided to you by AMD shall be kept confidential in
 * accordance with the terms and conditions of the Software License Agreement.
 *
 * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
 * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
 * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
 * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
 * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
 * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
 * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
 * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
 * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
 *
 * AMD does not assume any responsibility for any errors which may appear in
 * the Materials or any other related information provided to you by AMD, or
 * result from use of the Materials or any related information.
 *
 * You agree that you will not reverse engineer or decompile the Materials.
 *
 * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 * further information, software, technical information, know-how, or show-how
 * available to you.  Additionally, AMD retains the right to modify the
 * Materials at any time, without notice, and is not obligated to provide such
 * modified Materials to you.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
 * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
 * subject to the restrictions as set forth in FAR 52.227-14 and
 * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
 * Government constitutes acknowledgement of AMD's proprietary rights in them.
 *
 * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
 * direct product thereof will be exported directly or indirectly, into any
 * country prohibited by the United States Export Administration Act and the
 * regulations thereunder, without the required authorization from the U.S.
 * government nor will be used for any purpose prohibited by the same.
 * </pre>
 ******************************************************************************/

#include <cstring>
#include <cinttypes>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include "hdt_hw.h"
#include "infrastructure/debug.h"

extern "C"
{
    #include "AmdRemDbgCfg.h"
}

int JTAG_get_idcode (int fd)
{
    uint64_t idcode;
    uint32_t idcode32_l;
    uint32_t idcode32_h;	
    struct jtag_tap_state tap_state;
    struct jtag_xfer xfer;
    struct jtag_mode mode;
    int test;
	
    //select jtag mux setting '0'
	HDT_HDTSel_Control( true );

    // TEST LOGIC RESET
    tap_state.reset = JTAG_FORCE_RESET;
    tap_state.from = JTAG_STATE_CURRENT;
    tap_state.endstate = JTAG_STATE_TLRESET;
    tap_state.tck = 10;

    test = ioctl (fd, JTAG_SIOCSTATE, &tap_state);
    if (test < 0) {
        DEBUG_PRINT(DEBUG_INFO, "JTAG_get_idcode::JTAG_SIOCSTATE Failure: 0x%x, errno: %d.\n error message:%s", test, errno, strerror(errno));
        return -1;
    }

    memset(&xfer, 0, sizeof(struct jtag_xfer));
    xfer.type = JTAG_SDR_XFER;
    xfer.tdio = (uint64_t)(&idcode);
    xfer.length = sizeof(idcode) * BITS_PER_BYTE;
    xfer.from = JTAG_STATE_CURRENT;
    xfer.endstate = JTAG_STATE_IDLE;
    xfer.direction = JTAG_READ_XFER;

    DEBUG_PRINT(DEBUG_INFO, "JTAG_get_idcode::xfer.tdio: 0x%" PRIx64 ", idcode: 0x%x", xfer.tdio, idcode);

    test = ioctl (fd, JTAG_IOCXFER, &xfer);
    if (test < 0) {
     DEBUG_PRINT(DEBUG_INFO, "JTAG_get_idcode::XFER Failure: 0x%x, errno: %d.\n error message:%s", test, errno, strerror(errno));
     return -1;
    }

    idcode = *(uint64_t *)(xfer.tdio);

    //DEBUG_PRINT(DEBUG_INFO, "%s_IDCODE=0x%" PRIx64 "\n",device?"CPU":"FPGA",idcode);

    idcode32_l = (uint32_t)(idcode & 0xFFFFFFFF);
    idcode32_h = (uint32_t)((idcode & 0xFFFFFFFF00000000) >> 32);

    DEBUG_PRINT(DEBUG_INFO, "Lower_IDCODE=0x%x, Higher_IDCODE=0x%x\n",idcode32_l, idcode32_h);
    if (idcode32_l == idcode32_h)
    {
      DEBUG_PRINT(DEBUG_INFO, "2P System\n");
      is2PSystem = true;

    }

    //select jtag mux setting '1',default
	HDT_HDTSel_Control( false );	

    return 0;
}

bmc::HdtHw::HdtHw(int driver)
    : m_dbreq(false), m_dbreqOnDbrdy(true), m_dbreqOnReset(false), m_dbreqOnTrigger(false), m_dbreqTrigger(yaap::hal::TRIG_SRC_A_OR_B),
      m_dbreqOnTriggerInvert(false), m_dbreqTriggerEvent(yaap::hal::TRIG_EVENT_BOTH_EDGES), m_dbreqPulseWidth(24750)
{
    fd = driver;
    m_clksPerUsec = ((24750000 + 500000) / 1000000); // 25

    //HDT Conn and ARM Debug Conn (default) => MUX_S = High, MUX_OE = High, HDT_SEL = low
    HDT_HW_INIT();
    m_dbreq = false;

    JTAG_get_idcode (fd);
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqGet(bool& asserted)
{

    asserted = m_dbreq;

    DEBUG_PRINT(DEBUG_INFO,"DBREQ_L  %s", m_dbreq?"asserted":"de-asserted");

    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqSet(bool asserted)
{
	HDT_DBReq_Control( asserted );

    m_dbreq = asserted;

    DEBUG_PRINT(DEBUG_INFO,"DBREQ_L  %s", m_dbreq?"asserted":"deasserted");

    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbrdyGet(uint8_t& asserted)
{

    asserted = 1;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbrdySnapshotGet(uint8_t& asserted)
{
    asserted = 1;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbrdySnapshotClear(void)
{
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqSourceSnapshotGet(enum yaap::hal::dbreqSource& source)
{
    source = yaap::hal::DBREQ_SRC_HOST;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqSourceSnapshotClear(void)
{
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::resetGet(bool& asserted)
{
    asserted = false;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::pwrokGet(bool& asserted)
{
    // The Daytona and EthanolX both are using the GPIOE6 for PWR_OK signal.
    // GPIO addres for Ethanol is not defined in the .h file, so used DAYTONA address, it's common for both.
    asserted = false;
    if (Get_Host_PWR_Status())
    {
        asserted = true;
    }

    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqOnDbrdySet(bool enabled)
{
    m_dbreqOnDbrdy = enabled;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqOnDbrdyGet(bool& enabled)
{
    enabled = m_dbreqOnDbrdy;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqOnResetSet(bool enabled)
{
    m_dbreqOnReset = enabled;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqOnResetGet(bool& enabled)
{
    enabled = m_dbreqOnReset;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqOnTriggerSet(bool enabled, enum yaap::hal::triggerSource source, bool invert, enum yaap::hal::triggerEvent event)
{
    m_dbreqOnTrigger = enabled;
    if (enabled)
    {
        m_dbreqTrigger = source;
        m_dbreqOnTriggerInvert = invert;
        m_dbreqTriggerEvent = event;
    }
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqOnTriggerGet(bool& enabled, enum yaap::hal::triggerSource& source, bool& invert, enum yaap::hal::triggerEvent& event)
{
    enabled = m_dbreqOnTrigger;
    source = m_dbreqTrigger;
    invert = m_dbreqOnTriggerInvert;
    event = m_dbreqTriggerEvent;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqPulseWidthSet(uint32_t& usec)
{
    m_dbreqPulseWidth = usec * m_clksPerUsec;
    DEBUG_PRINT(DEBUG_INFO, "calculated DBREQ pulse is %u clocks\n", m_dbreqPulseWidth);
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbreqPulseWidthGet(uint32_t& usec)
{
    usec = m_dbreqPulseWidth/m_clksPerUsec;
    DEBUG_PRINT(DEBUG_INFO, "calculated DBREQ pulse is %u usec\n", usec);
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbrdyMaskSet(uint8_t enabled)
{
    m_dbrdyMask = enabled;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::HdtHw::dbrdyMaskGet(uint8_t& enabled)
{
    enabled = m_dbrdyMask;
    return E_SUCCESS;
}
