/**************************************************************************/
/*! \file hal/relay.h Relay HAL layer.
 *
 * <pre>
 * Copyright (C) 2011-2012, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef HAL_RELAY_H
#define HAL_RELAY_H

#include <stdint.h>

#include "hal/errs.h"

namespace yaap {
namespace hal {

/******************************************************************************/
/*! This interface provides access to a relay.
 * 
 * The expectation is that a relay behaves as a single-pole, single-throw switch.
 * When "closed", the two terminals are connected, when "opened" they are disconnected.
 * HDT makes use of three relays: 
 *    -# The power switch
 *    -# The cold reset line (i.e. PWRGD)
 *    -# The warm reset line (i.e. RESET_L)
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a IRelay_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a IRelay. 
 * 
 * \see IRelay_pulse
 ******************************************************************************/
class IRelay
{
  public:
    
    /*! Set the relay state.
     * 
     * A functional implementation of this method is required.
     * 
     * \param[in] close TRUE to close the relay; FALSE to open it
     * 
     * \retval #E_SUCCESS Operation succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t set(bool close) = 0;
    
    /*! Get the state of the relay.
     * 
     * \param[out] close On return, shall hold the current state of the relay (TRUE = closed).
     * 
     * \retval #E_SUCCESS Operation succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t get(bool& close) = 0;
};

/******************************************************************************/
/*! Interface for pulsing the relay.
 * 
 * If the hardware is capable of pulsing the relay in an atomic action, then this
 * interface should be implemented by the same class implementing IRelay.
 ******************************************************************************/
class IRelay_pulse
{
  public:
  
    /*! Pulse the relay.
     * 
     * \param[in,out] msec Amount of time (msec) to hold the relay in the "other" position.  Implementations should hold the 
     *                     relay for a duration as close as possible to the requested one, then place the actual duration 
     *                     back into this parameter before returning.
     * 
     * \retval #E_SUCCESS Operation succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t pulse(uint32_t& msec) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_RELAY_H
