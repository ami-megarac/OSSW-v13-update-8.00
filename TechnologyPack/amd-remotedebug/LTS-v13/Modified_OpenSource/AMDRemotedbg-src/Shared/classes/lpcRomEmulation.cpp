/******************************************************************************/
/*! \file lpcRomEmulation.cpp YAAP LpcRomEmulation class implementation.
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

#include "classes/lpcRomEmulation.h"

#define YAAP_LPC_ROM_EMULATION_VERSION yaapVersion(1, 0, 0)

namespace yaap {
namespace classes {

/******************************************************************************/

lpcRomEmulation::lpcRomEmulation(const char *name, hal::ILpcRomEmulator *hw) 
    : base(name, "lpcRomEmulation", YAAP_LPC_ROM_EMULATION_VERSION), m_hw(hw)
{
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_enableGet,        "enableGet",        "bool lpcRomEmulation.enableGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_enableSet,        "enableSet",        "void lpcRomEmulation.enableSet(bool enable)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_optionRomTypeGet, "optionRomTypeGet", "uint8_t lpcRomEmulation.optionRomTypeGet(void"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_optionRomTypeSet, "optionRomTypeSet", "void lpcRomEmulation.optionRomTypeSet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_sramGet,          "sramGet",          "uint8_t [] lpcRomEmulation.sramGet(uint32_t startAddr, uint32_t numBytes)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_sramSet,          "sramSet",          "void lpcRomEmulation.sramSet(uint32_t startAddr, uint8_t [] data)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcRomEmulation, yaap_sramSizeGet,      "sramSizeGet",      "int lpcRomEmulation.sramSizeGet(void)"));

    initialize();
}

/******************************************************************************/

uint32_t lpcRomEmulation::yaap_sramSizeGet(YAAP_METHOD_PARAMS)
{
    uint32_t bytes = 0;
    const char *device;
    
    uint32_t retval = m_hw->setupGet(bytes, device);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(bytes);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t lpcRomEmulation::yaap_sramSet(YAAP_METHOD_PARAMS)
{
    uint32_t startAddr = is.getInt();
    uint32_t nBytes = is.getArrayCount();
    uint8_t *buf = is.getRawBytePtr(nBytes);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_hw->write(startAddr, buf, nBytes);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t lpcRomEmulation::yaap_sramGet(YAAP_METHOD_PARAMS)
{
    uint32_t startAddr = is.getInt();
    uint32_t nBytes = is.getInt();
    uint32_t retval = checkInputStream(is);
    uint8_t *buf = NULL;
    
    if (retval == E_SUCCESS)
    {
        buf = new uint8_t[nBytes];
        if (buf == NULL)
        {
            retval = E_OUT_OF_MEMORY;
        }
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_hw->read(startAddr, buf, nBytes);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    if (retval == E_SUCCESS)
    {
        os.putByteArray(nBytes, buf);
    }
    else
    {
        os.putInt(0); // Array size 0
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

static const char *lpc_devices[] = { "SST49LF002B", "SST49LF004B", "SST49LF080A", "SST49LF160C" };

uint32_t lpcRomEmulation::yaap_optionRomTypeSet(YAAP_METHOD_PARAMS)
{
    uint8_t deviceCode = is.getByte();
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (deviceCode > 3))
    {
        retval = E_UNSUPPORTED_DEVICE;
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_hw->setupSet(lpc_devices[deviceCode]);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t lpcRomEmulation::yaap_optionRomTypeGet(YAAP_METHOD_PARAMS)
{
    uint32_t bytes = 0;
    const char *device = NULL;
    uint8_t deviceCode = 0;
    
    uint32_t retval = m_hw->setupGet(bytes, device);
    
    if (retval == E_SUCCESS)
    {
        int i;
        for (i = 0; i < 4; i++)
        {
            if (!strcmp(device, lpc_devices[i]))
            {
                deviceCode = i;
                break;
            }
        }
        if (i == 4)
        {
            retval = E_UNSUPPORTED_DEVICE;
        }
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(deviceCode);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t lpcRomEmulation::yaap_enableSet(YAAP_METHOD_PARAMS)
{
    bool enable = is.getByte() != 0;
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_hw->enableSet(enable);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t lpcRomEmulation::yaap_enableGet(YAAP_METHOD_PARAMS)
{
    bool enable = false;
    uint32_t retval = m_hw->enableGet(enable);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(enable);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

} // namespace classes
} // namespace yaap


