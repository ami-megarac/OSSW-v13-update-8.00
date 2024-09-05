/**************************************************************************/
/*! \file gpuDebug.cpp YAAP \a GpuDebug class implementation.
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

#include "classes/gpuDebug.h"

#define YAAP_GPU_DEBUG_VERSION yaapVersion(1, 1, 0)

namespace yaap {
namespace classes {

/******************************************************************************/

gpuDebug::gpuDebug(const char *name, hal::IHeader *jtagHdrHw, hal::IHeader *i2cHdrHw, hal::II2c *i2cHw, hal::IJtag *jtagHw, hal::IGpuDebug *gpuDbgHw)
    : base(name, "gpuDebug", YAAP_GPU_DEBUG_VERSION),
      m_i2c("i2c", i2cHw, i2cHdrHw), m_jtag("jtag", jtagHw, jtagHdrHw), m_gpuScan("gpuScan", gpuDbgHw, i2cHdrHw), 
      m_jtagHdrHw(jtagHdrHw), m_i2cHdrHw(i2cHdrHw), m_gpuDbgHw(gpuDbgHw)
{
    addMember(&m_i2c);
    addMember(&m_jtag);
    addMember(&m_gpuScan);
    
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuDebug, yaap_headerStatusGet,   "headerStatusGet",   "struct { struct { string name; bool connected; bool changed; bool conflicted; } [] hdrs; struct { } [] conflicts; } hdt.headerStatusGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuDebug, yaap_headerStatusClear, "headerStatusClear", "void gpuDebug.headerStatusClear()"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuDebug, yaap_headerModeGet,     "headerModeGet",     "int gpuDebug.headerModeGet()"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuDebug, yaap_headerModeSet,     "headerModeSet",     "void gpuDebug.headerModeSet(int mode)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuDebug, yaap_ioPxEnGet,         "ioPxEnGet",         "bool gpuDebug.ioPxEnGet()"));
    
    DEBUG_PRINT_SUPPORTED_FUNC("GPU JTAG Header", jtagHdrHw, hal::IHeader_setEnabled);
    DEBUG_PRINT_SUPPORTED_FUNC("GPU JTAG Header", jtagHdrHw, hal::IHeader_changeDetect);
    DEBUG_PRINT_SUPPORTED_FUNC("GPU I2C Header", i2cHdrHw, hal::IHeader_setEnabled);
    DEBUG_PRINT_SUPPORTED_FUNC("GPU I2C Header", i2cHdrHw, hal::IHeader_changeDetect);
    DEBUG_PRINT_SUPPORTED_FUNC("GPU Scan", gpuDbgHw, hal::IGpuDebug_sckFrequency);

    m_jtagHdrChg = dynamic_cast<hal::IHeader_changeDetect *>(jtagHdrHw);
    m_i2cHdrChg = dynamic_cast<hal::IHeader_changeDetect *>(i2cHdrHw);

    initialize();
}

/******************************************************************************/

uint32_t gpuDebug::yaap_headerStatusGet(YAAP_METHOD_PARAMS)
{
    bool connected[2] = { false, false };
    const char *mode[2] = { "", "" };
    bool changed[2] = { false, false };
    bool conflicted[2] = { false, false };
    
    uint32_t retval = m_jtagHdrHw->isInConflict(conflicted[0]);
    if (retval == E_SUCCESS)
    {
        retval = m_jtagHdrHw->isConnected(connected[0], mode[0]);
    }
    if ((retval == E_SUCCESS) && (m_jtagHdrChg != NULL))
    {
        retval = m_jtagHdrChg->isChanged(changed[0]);
    }
    if (retval == E_SUCCESS)
    {
        retval = m_i2cHdrHw->isInConflict(conflicted[1]);
    }
    if (retval == E_SUCCESS)
    {
        retval = m_i2cHdrHw->isConnected(connected[1], mode[1]);
    }
    if ((retval == E_SUCCESS) && (m_i2cHdrChg != NULL))
    {
        retval = m_i2cHdrChg->isChanged(changed[1]);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(2);  // number of headers
    os.putString(mode[0]);
    os.putBool(connected[0]);
    os.putBool(changed[0]);
    os.putBool(conflicted[0]);
    os.putString(mode[1]);
    os.putBool(connected[1]);
    os.putBool(changed[1]);
    os.putBool(conflicted[1]);
    os.putInt(0);  // zero-length array of conflict info
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t gpuDebug::yaap_headerStatusClear(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    
    if ((m_jtagHdrChg == NULL) && (m_i2cHdrChg == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    
    if ((retval == E_SUCCESS) && (m_jtagHdrChg != NULL))
    {
        retval = m_jtagHdrChg->clearChanged();
    }
    
    if ((retval == E_SUCCESS) && (m_i2cHdrChg != NULL))
    {
        retval = m_i2cHdrChg->clearChanged();
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t gpuDebug::yaap_headerModeGet(YAAP_METHOD_PARAMS)
{
    int mode = -1;
    uint32_t retval = m_gpuDbgHw->hdrModeGet(mode);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(mode);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t gpuDebug::yaap_headerModeSet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    int mode = is.getInt();
    
    if (is.isError())
    {
        DEBUG_PRINT(DEBUG_ERROR, "Input stream error");
        retval = E_MALFORMED_REQUEST;
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_gpuDbgHw->hdrModeSet(mode);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t gpuDebug::yaap_ioPxEnGet(YAAP_METHOD_PARAMS)
{
    bool asserted = false;
    uint32_t retval = m_gpuDbgHw->ioPxEnGet(asserted);

    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(asserted);
    os.endBlockSize();
    
    return retval;
}

} // namespace classes
} // namespace yaap
