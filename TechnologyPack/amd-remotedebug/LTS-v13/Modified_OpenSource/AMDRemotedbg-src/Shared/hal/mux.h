/**************************************************************************/
/*! \file hal/mux.h HAL definition for a mux.
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
 **************************************************************************/

#ifndef HAL_MUX_H
#define HAL_MUX_H

#include <stdint.h>
#include <map>

#include "hal/errs.h"

using namespace std;

namespace yaap {
namespace hal {

/*******************************************************************************/
/*! This interface provides access to single multiplexer.
 *******************************************************************************/
class IMux
{
 public:

    /*! Set the mux setting.
     * 
     * This method is used to set the mux.  
     * 
     * \param[in] setting The setting to which the mux is to be set.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_INVALID_SETTING The requested setting is not valid for the mux
     * \retval #E_UNSPECIFIED_HW_ERROR Call failed due to an unspecified low-level error
     */
    virtual uint32_t set(int setting) = 0;
    
    /*! Get the mux setting.
     * 
     * This method is used to read the current mux setting. 
     * 
     * \param[out] setting Callee sets this to the current mux setting
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Call failed due to an unspecified low-level error
     */
    virtual uint32_t get(int& setting) = 0;
    
    /*! Get the available settings for the mux.
     * 
     * This method is used to query the available settings for the mux.  Each setting 
     * consists of an integer value (which is used by the get() and set() methods) and a 
     * string description.  
     * 
     * An implementation may return E_NOT_IMPLEMENTED for this method.
     * 
     * \param[in,out] settings An STL \a map with key = mux setting (integer) and value = 
     *                description of setting (pointer to string).  The caller will pass a reference to an object
     *                allocated on its stack; the callee may populate this object, or change
     *                the reference to point to a compatible object under its own management.
     *                As it is allocated on the stack, the caller will not attempt to free
     *                the \a settings object.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Call failed due to an unspecified low-level error
     * \retval #E_NOT_IMPLEMENTED Functionality is not implemented
     */
    virtual uint32_t options(map<int, const char *>& settings) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_MUX_H
