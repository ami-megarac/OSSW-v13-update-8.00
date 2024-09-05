/*************************************************************************/
/*! \file hal/i2c.h I2C interface HAL header file.
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

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>

#include "hal/errs.h"
#include "hal/header.h"

namespace yaap {
namespace hal {

/*******************************************************************************/
/*! This interface provides access to an I2C controller.  
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a II2c_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a II2c.
 * 
 * \see II2c_sclFrequency
 *******************************************************************************/
class II2c
{
  public :

    /*! Perform an I2C transfer, consisting of a write and/or a read.  
     * 
     * A read is performed by calling this method with \a wrBytes = 0 and \a wrData = NULL.  A write is 
     * performed by calling this method with \a rdBytes = 0 and \a rdData = NULL.  If both write and
     * read data are specified, then a write will be performed, followed by a read; this is commonly done
     * to read from a specific address within a device.
     *
     * \param[in] deviceAddr  The I2C bus address of the target device.  Only 7-bit addresses are supported.
     *                        This is only the 7-bit address; the driver must construct the first byte of the
     *                        transfer with the address in bits 7:1 and the R/W flag in bit 0.
     * \param[in,out] wrBytes Number of bytes to write.  On return, holds the actual number of bytes written.
     * \param[in] wrData      Pointer to the data to write.
     * \param[in,out] rdBytes Number of bytes to read.  On return, holds the actual number of bytes read.
     * \param[in] rdData      Pointer to the buffer where the read data will be placed.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_I2C_DEVICE_NOT_OPEN The I2C device is not open
     * \retval #E_I2C_TRANSFER_TOO_LARGE The requested transfer is too large
     * \retval #E_I2C_ADDRESS_NAK No slave ACKed the address
     * \retval #E_I2C_ACK_TIMEOUT Timed out waiting for write ACK
     * \retval #E_I2C_BUS_BUSY_TIMEOUT Timed out waiting for bus-busy condition to clear
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t transfer(uint32_t deviceAddr, int& wrBytes, uint8_t *wrData, int& rdBytes, uint8_t *rdData) = 0; 
};

/******************************************************************************/
/*! Interface for changing the SCL frequency.
 * 
 * If the hardware supports changing the SCL frequency, 
 * then this interface should be implemented by same class implementing IHeader.
 ******************************************************************************/
class II2c_sclFrequency
{
  public:

    /*! Set the SCL frequency.
     *
     * The implementation should set the frequency as close to the requested value as possible, then return the 
     * actual frequency it used in the \a hz parameter.  No error shall be returned as a result of not being 
     * able to achieve the requested frequency, no matter how different the actual value is from the requested.
     * 
     * \param[in,out] hz Contains the requested SCL frequency, in Hz.  Upon return, this variable shall be 
     *                   updated with the actual SCL frequency attained.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see getFrequency()
     */
    virtual uint32_t setFrequency(uint32_t& hz) = 0;

    /*! Get the SCL frequency.
     *
     * \param[out] hz Shall indicate the current SCL frequency on return.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see setFrequency()
     */
    virtual uint32_t getFrequency(uint32_t& hz) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_I2C_H
