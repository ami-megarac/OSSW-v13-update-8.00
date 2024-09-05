/**************************************************************************/
/*! \file relay.cpp YAAP \a relay class implementation.
 *
 * <pre>
 * Copyright (C) 2009-2012, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
 **************************************************************************/

#include "classes/relay.h"

#define YAAP_RELAY_VERSION yaapVersion(1, 0, 9)

namespace yaap {
namespace classes {

/******************************************************************************/

relay::relay(const char *name, hal::IRelay *relayHw)
    : base(name, "relay", YAAP_RELAY_VERSION), m_relayHw(relayHw)
{
    m_relayPulse = dynamic_cast<hal::IRelay_pulse *>(relayHw);

    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, relay, yaap_set, "set", "void relay.set(bool closed)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, relay, yaap_get, "get", "bool relay.get(void)"));

    if (m_relayPulse != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, relay, yaap_toggle, "toggle", "void relay.toggle(uint32_t msecPulseTime)"));
    }
    
    DEBUG_PRINT_SUPPORTED_FUNC("Relay", relayHw, hal::IRelay_pulse);

    initialize();
}

/******************************************************************************/

uint32_t relay::yaap_get(YAAP_METHOD_PARAMS)
{
    bool state = false;
    
    uint32_t retval = m_relayHw->get(state);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(state ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t relay::yaap_set(YAAP_METHOD_PARAMS)
{
    bool state = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_relayHw->set(state);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t relay::yaap_toggle(YAAP_METHOD_PARAMS)
{
    uint32_t pulseWidthMsec = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_relayPulse == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_relayPulse->pulse(pulseWidthMsec);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

} // namespace classes
} // namespace yaap
