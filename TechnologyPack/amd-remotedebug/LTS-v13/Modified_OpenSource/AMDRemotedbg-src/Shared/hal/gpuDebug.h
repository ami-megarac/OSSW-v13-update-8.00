/*************************************************************************/
/*! \file hal/gpuDebug.h GPU Debug (I2C/Scan) interface HAL header file.
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

#ifndef HAL_GPU_DEBUG_H
#define HAL_GPU_DEBUG_H

#include <stdint.h>
#include "hal/errs.h"

namespace yaap {
namespace hal {

/*******************************************************************************/
/*! This interface provides access to a dual-purpose I2C and GPU Scan controller.
 * 
 * The GPU Debug interface has two connections, one for JTAG and the other 
 * selectable between I2C or GPU Scan.  This HAL interface contains methods
 * primarily for controlling the I2C/Scan header.
 *******************************************************************************/
class IGpuDebug
{
 public:
 
    static const int HEADER_MODE_I2C = 0;       //!< Header configured for I2C mode
    static const int HEADER_MODE_GPU_SCAN = 1;  //!< Header configured for GPU Scan mode
    static const int HEADER_MODE_OTHER = 2;     //!< Header configured for any other mode
    
    /*! Return the current state of the PX_EN pin.
     * 
     * \param[out] asserted Indicates whether or not the pin is asserted (logic high).
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t ioPxEnGet(bool& asserted) = 0;
    
    /*! Get the current header mode for the I2C/Scan header.
     * 
     * This method queries the I2C/Scan header hardware to see what mode it is currently in.
     * This method does not concern itelf with whether the header is currently connected, only
     * the mode that it is currently configured to operate in.
     * 
     * \param[out] mode Current mode: HEADER_MODE_I2C, HEADER_MODE_GPU_SCAN, or HEADER_MODE_OTHER
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t hdrModeGet(int &mode) = 0;
    
    /*! Set the current header mode for the I2C/Scan header.
     * 
     * This method configures the I2C/Scan header hardware mode.
     * 
     * \param[in] mode Requested mode: HEADER_MODE_I2C or HEADER_MODE_GPU_SCAN
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_INVALID_MODE Cannot set the requested mode (i.e. connecter may be in JTAG mode)
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t hdrModeSet(int mode) = 0;
    
    /*! Perform a scan.
     * 
     * Performs a scan of the requested number of bits.  Before calling this method, 
     * the core YAAP code will have checked to ensure that the header is enabled and
     * connected; this method need not check those again.  The caller handles allocation
     * and deallocation of the buffer at \a readBuf.
     * 
     * \param[in] numBits  Number of bits to shift
     * \param[in] readBuf  Buffer in which the data shifted out should be stored
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_INVALID_MODE The connecter is not in GPU scan mode
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t scan(int numBits, uint8_t *readBuf) = 0;

};

/*******************************************************************************/
/*! This interface provides control of the SCK clock frequency for the GPU Scan port.
 * 
 * If the hardware supports a programmable SCK frequency, then this 
 * interface should be implemented by the same class implementing IGpuDebug.
 *******************************************************************************/
class IGpuDebug_sckFrequency
{
 public:
 
    /*! Set the SCK frequency.
     *
     * Implementations should set the SCK frequency as near as possible to the requested value, then place the actual 
     * frequency achieved in the \a freqHz parameter before returning.  Not achieving the requested frequency 
     * is not considered an error.
     * 
     * \param[in,out] freqHz The requested frequency, in Hz.  On return, this variable
     *                       shall be updated with the actual value used; this may differ from the input
     *                       value due to hardware constraints.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see sckFreqGet()
     */
    virtual uint32_t sckFreqSet(uint32_t& freqHz) = 0;

    /*! Get the SCK clock frequency.
     *
     * \param[out] freqHz On return, shall hold the SCK clock frequency in Hz.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see sckFreqSet()
     */
    virtual uint32_t sckFreqGet(uint32_t& freqHz) = 0;
};

} // namespace hal
} // namespace yaap
        
#endif // HAL_GPU_DEBUG_H

