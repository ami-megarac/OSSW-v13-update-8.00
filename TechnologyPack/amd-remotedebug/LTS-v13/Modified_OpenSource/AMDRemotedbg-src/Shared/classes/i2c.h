/******************************************************************************/
/*! \file classes/i2c.h YAAP \a i2c class header.
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
 ******************************************************************************/

#ifndef YAAP_I2C_CLASS_H
#define YAAP_I2C_CLASS_H

#include "classes/base.h"
#include "hal/i2c.h"
#include "hal/header.h"

using namespace yaap;

namespace yaap {
namespace classes {

/******************************************************************************/
/*! The YAAP \a i2c class.
 * 
 * This provides the interface to an I2C controller in the YAAP device.
 ******************************************************************************/
class i2c : public base
{
public:

    /*! Constructor.
     * 
     * \param[in] name    The name of the object.
     * \param[in] i2cHw   Pointer to the underlying I2C hardware.
     * \param[in] hdrHw   Pointer to the underlying header hardware.
     */
    i2c(const char *name, hal::II2c *i2cHw, hal::IHeader *hdrHw);

protected:

    //! YAAP Method: Perform an I2C transfer.
    uint32_t yaap_transfer(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Get the SCL frequency.
    uint32_t yaap_optionSclFrequencyGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Set the SCL frequency.
    uint32_t yaap_optionSclFrequencySet(YAAP_METHOD_PARAMS);
                
    /*! Resize the read buffer to accomodate \a newSize bytes.
     * 
     * \param[in] newSize Number of bytes the read buffer needs to accomodate.
     * 
     * \retval #E_SUCCESS The resize was successful
     * \retval #E_OUT_OF_MEMORY There is insufficient memory for the resize.
     */
    uint32_t resizeReadBuf(int newSize);
    
    hal::II2c *m_i2cHw;                 //!< The I2C HAL
    hal::II2c_sclFrequency *m_i2cFreq;  //!< I2C frequency HAL
    
    hal::IHeader *m_hdrHw;              //!< The header driver
    
    uint8_t *m_readBuf;                 //!< The read buffer
    int m_readBufSize;                  //!< The current size of the read buffer (bytes)
    
};

} // namespace classes
} // namespace yaap

#endif // YAAP_I2C_CLASS_H