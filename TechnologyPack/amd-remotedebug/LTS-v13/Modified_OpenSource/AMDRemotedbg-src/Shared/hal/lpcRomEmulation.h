/*************************************************************************/
/*! \file hal/lpcRomEmulation.h LPC ROM Emulator interface HAL header file.
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

#ifndef HAL_LPC_ROM_EMULATOR_H
#define HAL_LPC_ROM_EMULATOR_H

#include <stdint.h>
#include <vector>
#include "hal/errs.h"

using namespace std;

namespace yaap {
namespace hal {

/*******************************************************************************/
/*! This interface provides access to an LPC ROM emulator.
 *******************************************************************************/
class ILpcRomEmulator
{
 public:

    /*! Enable or disable the device.
     * 
     * \param[in] enable TRUE to enable the device, FALSE to disable it
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t enableSet(bool enable) = 0;

    /*! Query the current enabled/disabled status of the device.
     * 
     * \param[out] enabled TRUE indicates that the device is currently enabled
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t enableGet(bool& enabled) = 0;

    /*! Get the current setup.
     * 
     * \param[out] bytes Size of the emulated device, in bytes
     * \param[out] device Part number (i.e. "SST49LF160C" for 2 MB SST device)
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t setupGet(uint32_t& bytes, const char *& device) = 0;

    /*! Configure the emulated device.
     * 
     * \param[in] device Part number (i.e. "SST49LF160C")
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSUPPORTED_DEVICE Specified device is not supported
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t setupSet(const char *device) = 0;

    /*! Get a list of supported devices.
     * 
     * \param[out] devices Vector that is populated by the callee with pointers part numbers of all supported devices
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t devicesGet(vector<const char *>& devices) = 0;

    /*! Write data to the emulated ROM memory.
     * 
     * \param[in] startAddress The starting address of the location to be written to
     * \param[in] buffer Pointer to the data to write
     * \param[in] byteCount Number of bytes to write
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_TOO_BIG The specified image is too large for the selected ROM
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t write(uint32_t startAddress, uint8_t *buffer, uint32_t byteCount) = 0;

    /*! Read data from the emulated ROM memory.
     * 
     * \param[in] startAddress The starting address of the location to be read from
     * \param[in] buffer Pointer to a buffer where the read data should be stored
     * \param[in] byteCount Number of bytes to read
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_TOO_BIG The specified size is too large for the selected ROM
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */    
    virtual uint32_t read(uint32_t startAddress, uint8_t *buffer, uint32_t byteCount) = 0;
};


} // namespace hal
} // namespace yaap

#endif // HAL_LPC_ROM_EMULATOR_H


