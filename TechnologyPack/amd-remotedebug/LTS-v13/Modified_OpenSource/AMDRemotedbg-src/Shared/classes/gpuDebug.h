/******************************************************************************/
/*! \file classes/gpuDebug.h YAAP \a gpuDebug class
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

#ifndef YAAP_GPU_DEBUG_CLASS_H
#define YAAP_GPU_DEBUG_CLASS_H

#include "classes/base.h"
#include "classes/i2c.h"
#include "classes/jtag.h"
#include "classes/gpuScan.h"
#include "hal/gpuDebug.h"

namespace yaap {
namespace classes {

/******************************************************************************/
/*! The YAAP \a GpuDebug class.
 * 
 * This class implements the GPU debug functionality (JTAG and I2C/SMBus).
 ******************************************************************************/
class gpuDebug : public base
{
 public:

    /*! Constructor.
     * 
     * Constructs a YAAP GpuDebug object, along with its member instances of \a i2c, 
     * \a jtag, \a header, and \a gpuScan.
     * 
     * \param[in] name      The name of the object.  HDT expects this to be "gpuDebug".
     * \param[in] jtagHdrHw Pointer to the HAL header driver associated with the JTAG connection.
     * \param[in] i2cHdrHw  Pointer to the HAL header driver associated with the I2C/Scan connection.
     * \param[in] i2cHw     Pointer to the HAL I2C driver associated with this instance.
     * \param[in] jtagHw    Pointer to the HAL JTAG driver associated with this instance.
     * \param[in] gpuDbgHw  Pointer to the HAL GPU debug driver associated with this instance.
     */
    gpuDebug(const char *name, hal::IHeader *jtagHdrHw, hal::IHeader *i2cHdrHw, hal::II2c *i2cHw, hal::IJtag *jtagHw, hal::IGpuDebug *gpuDbgHw);
    
 protected:

    //! YAAP Method: Get current header status.
    uint32_t yaap_headerStatusGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Clear the header changed flag.
    uint32_t yaap_headerStatusClear(YAAP_METHOD_PARAMS); 

    //! YAAP Method: Get current header mode.
    uint32_t yaap_headerModeGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Set header mode.
    uint32_t yaap_headerModeSet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Get current state of PX_EN signal.
    uint32_t yaap_ioPxEnGet(YAAP_METHOD_PARAMS);
    
    i2c m_i2c;                  //!< I2C child associated with this instance.
    jtag m_jtag;                //!< JTAG child associated with this instance.
    gpuScan m_gpuScan;          //!< GpuScan child associated with this instance.
    hal::IHeader *m_jtagHdrHw;  //!< The JTAG header hardware driver.
    hal::IHeader_changeDetect *m_jtagHdrChg;    //!< JTAG header change-detect functionality.
    hal::IHeader *m_i2cHdrHw;   //!< The I2C header hardware driver.
    hal::IHeader_changeDetect *m_i2cHdrChg;     //!< I2C header change-detect functionality.
    hal::IGpuDebug *m_gpuDbgHw; //!< The GPU debug hardware driver.
};

} // namespace classes
} // namespace yaap

#endif // YAAP_GPU_DEBUG_CLASS_H

