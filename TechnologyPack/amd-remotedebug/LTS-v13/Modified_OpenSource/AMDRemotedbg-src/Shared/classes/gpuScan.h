/******************************************************************************/
/*! \file classes/gpuScan.h YAAP \a gpuScan class header.
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

#ifndef YAAP_GPU_SCAN_CLASS_H
#define YAAP_GPU_SCAN_CLASS_H

#include "classes/base.h"
#include "hal/header.h"
#include "hal/gpuDebug.h"

using namespace yaap;

namespace yaap {
namespace classes {

/******************************************************************************/
/*! The YAAP \a gpuScan class.
 * 
 * This provides the interface to GPU scan hardware.
 ******************************************************************************/
class gpuScan : public base
{
public:

    /*! Constructor.
     * 
     * \param[in] name       The name of the object.
     * \param[in] gpuDebugHw Pointer to the underlying GPU I2C/Scan hardware.
     * \param[in] hdrHw      Pointer to the underlying header hardware.
     */
    gpuScan(const char *name, hal::IGpuDebug *gpuDebugHw, hal::IHeader *hdrHw);

protected:

    //! YAAP Method: Perform scan.
    uint32_t yaap_scan(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Get the port enabled status.
    uint32_t yaap_enableGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Enable or disable the port.
    uint32_t yaap_enableSet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Get the SCK frequency.
    uint32_t yaap_optionSckFrequencyGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Set the SCK frequency.
    uint32_t yaap_optionSckFrequencySet(YAAP_METHOD_PARAMS);
        
    /*! Resize the read buffer to accomodate \a newSize bytes.
     * 
     * \param[in] newSize Number of bytes the read buffer needs to accomodate.
     * 
     * \retval #E_SUCCESS The resize was successful
     * \retval #E_OUT_OF_MEMORY There is insufficient memory for the resize.
     */
    uint32_t resizeReadBuf(int newSize);

    hal::IGpuDebug *m_gpuDbgHw; //!< The I2C/GPU Scan hardware driver
    hal::IGpuDebug_sckFrequency *m_sckFreq;  //!< SCK frequency get/set HAL
    hal::IHeader *m_hdrHw;      //!< The header driver
    hal::IHeader_setEnabled *m_hdrSet;  //!< The header enable/disable interface
    uint8_t *m_readBuf;         //!< The read buffer
    int m_readBufSize;          //!< The current size of the read buffer (bytes)
    
};

} // namespace classes
} // namespace yaap

#endif // YAAP_GPU_SCAN_CLASS_H
