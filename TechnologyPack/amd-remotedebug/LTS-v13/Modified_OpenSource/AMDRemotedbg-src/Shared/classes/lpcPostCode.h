/******************************************************************************/
/*! \file classes/lpcPostCode.h YAAP LPC Post Code reader header file.
 *
 * <pre>
 * Copyright (C) 2012, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef YAAP_LPC_POST_CODE_CLASS_H
#define YAAP_LPC_POST_CODE_CLASS_H

#include "classes/base.h"
#include "hal/lpcPostCode.h"

using namespace std;

namespace yaap {
namespace classes {

/******************************************************************************/
/*! The YAAP \a LpcPostCode class.
 * 
 * This provides the YAAP interface to a POST code reader.
 ******************************************************************************/
class lpcPostCode : public base
{
public:

    /*! Constructor.
     * 
     * \param[in] name The name of the object.
     * \param[in] hw   Pointer to the low-level driver.
     */
    lpcPostCode(const char *name, hal::ILpcPostCode *hw);

protected:

    //! YAAP Method: Get last POST code.
    uint32_t yaap_postCodeGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Get the depth of the FIFO.
    uint32_t yaap_fifoSizeGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Retrieve the contents of the FIFO.
    uint32_t yaap_fifoGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Clear the FIFO.
    uint32_t yaap_fifoClear(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the FIFO status.
    uint32_t yaap_fifoStatusGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Clear the FIFO status.
    uint32_t yaap_fifoStatusClear(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the IO port on which POST codes will be read.
    uint32_t yaap_optionPortSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the IO port on which POST codes will be read.
    uint32_t yaap_optionPortGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the device for active or passive POST code monitoring.
    uint32_t yaap_optionPassiveSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current active/passive monitoring setting.
    uint32_t yaap_optionPassiveGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Configure the FIFO for one-shot or circular operation.
    uint32_t yaap_optionFifoCircularSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current one-shot or circular operation setting.
    uint32_t yaap_optionFifoCircularGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Configure the device to capture or discard identical sequential POST codes.
    uint32_t yaap_optionFifoDiscardSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current setting for handling of identical sequential POST codes.
    uint32_t yaap_optionFifoDiscardGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Configure the device to optionally automatically clear the FIFO on certain system conditions.
    uint32_t yaap_optionFifoAutoResetSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current setting for automatic clearing of the FIFO on certain system conditions.
    uint32_t yaap_optionFifoAutoResetGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Enable or disable POST code reading.
    uint32_t yaap_enableSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current enabled/disabled state.
    uint32_t yaap_enableGet(YAAP_METHOD_PARAMS);

    hal::ILpcPostCode *m_postCodeHw;    //!< Driver for the basic POST code functionality
    hal::ILpcPostCode_fifo *m_fifoHw;   //!< Driver for the FIFO functionality

};

} // namespace classes
} // namespace yaap

#endif // YAAP_LPC_POST_CODE_H
