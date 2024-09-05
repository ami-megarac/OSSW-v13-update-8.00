/******************************************************************************/
/*! \file bmc/hw/relay_hw.cpp bmc:: relay HW driver
 *
 * <pre>
 * Copyright (C) 2011-2020, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#include "relay_hw.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "aspeed.h"

#define ETHANOLX_ID 4
#define DAYTONAX_ID 6
#define I2C_CMD_BUFF_SIZE 32
extern "C"
{
    #include "AmdRemDbgCfg.h"
}

bmc::RelayHw::RelayHw(int fd, int channel)
    : m_state(false)
{
    m_fd= fd;
    m_channel = 1;
    m_position = channel;

	// Fix board id in hawaii
    m_board_id = 0xFF;
}


/***************************************************************/

uint32_t bmc::RelayHw::set(bool state)
{
    m_state = state;
    if(state)
	{
	    switch(m_position) {
			case 0:
                HDT_PowerBtn_Control (state);
			break;
			case 1:
                HDT_ResetBtn_Control (state);
			break;
			default:
				DEBUG_PRINT(DEBUG_WARNING, "Feature not implemented");
			}
    }
    else
    {
	    switch(m_position) {
			case 0:
                HDT_PowerBtn_Control (state);
			break;
			case 1:
                HDT_ResetBtn_Control (state);
			break;
			default:
				DEBUG_PRINT(DEBUG_WARNING, "Feature not implemented");
			}
    }
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::RelayHw::pulse(uint32_t& msec)
{
    bool currentState;
    uint32_t retVal;
    unsigned long long usec = msec * 1000;
    unsigned long long elapsed;

    retVal = get(currentState);
    if(retVal != E_SUCCESS){
        DEBUG_PRINT(DEBUG_WARNING, "Could not read value to toggle relay");
        return E_UNSPECIFIED_HW_ERROR;
    }

    retVal = set(!currentState);
    if(retVal != E_SUCCESS){
        DEBUG_PRINT(DEBUG_WARNING, "Could not change value to toggle relay");
        return E_UNSPECIFIED_HW_ERROR;
    }

    elapsed = usleep(usec);

    retVal = set(currentState);
    if(retVal != E_SUCCESS){
         DEBUG_PRINT(DEBUG_WARNING, "Could not reset value to toggle relay");
    }
    msec = elapsed/1000;

    return E_SUCCESS;
}

uint32_t bmc::RelayHw::get(bool& state)
{
   	switch(m_position) {
	case 0:
		m_state = Get_HDT_PowerBtn_Status();
		break;
	case 1:
		m_state = Get_HDT_ResetBtn_Status();
		break;
	default:
		DEBUG_PRINT(DEBUG_WARNING, "Feature not implemented");
	}
    state = m_state;
    return E_SUCCESS;
}
