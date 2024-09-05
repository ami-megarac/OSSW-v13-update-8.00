/******************************************************************************/
/*! \file classes/gpuScan.cpp YAAP \a gpuScan class implementation.
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

#include "classes/gpuScan.h"

#define YAAP_GPU_SCAN_VERSION yaapVersion(1, 5, 0)

namespace yaap {
namespace classes {

/******************************************************************************/

gpuScan::gpuScan(const char *name, hal::IGpuDebug *gpuDebugHw, hal::IHeader *hdrHw)
    : base(name, "gpuScan", YAAP_GPU_SCAN_VERSION),
      m_gpuDbgHw(gpuDebugHw), m_hdrHw(hdrHw), m_readBuf(NULL), m_readBufSize(0)
{
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuScan, yaap_scan,      "scan",      "char [] gpuScan.scan(int numBits)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuScan, yaap_enableGet, "enableGet", "bool gpuScan.enableGet()"));

    m_hdrSet = dynamic_cast<hal::IHeader_setEnabled *>(hdrHw);
    m_sckFreq = dynamic_cast<hal::IGpuDebug_sckFrequency *>(gpuDebugHw);
    
    if (m_hdrSet != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuScan, yaap_enableSet, "enableSet", "void gpuScan.enableSet(bool enabled)"));
    }
    
    if (m_sckFreq != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuScan, yaap_optionSckFrequencyGet, "optionSckFrequencyGet", "int gpuScan.optionSckFrequencyGet()"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, gpuScan, yaap_optionSckFrequencySet, "optionSckFrequencySet", "void gpuScan.optionSckFrequencySet(int freqHz)"));
    }
    
    initialize();
}

/******************************************************************************/

uint32_t gpuScan::resizeReadBuf(int newSize)
{
    if (m_readBufSize > 0)
    {
        delete [] m_readBuf;
    }
    m_readBuf = new uint8_t[newSize];
    
    if (m_readBuf == NULL)
    {
        m_readBufSize = 0;
        return E_OUT_OF_MEMORY;
    }
    m_readBufSize = newSize;
    return E_SUCCESS;
}


/******************************************************************************/

uint32_t gpuScan::yaap_scan(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    uint32_t numBits = is.getInt();
    int numBytes = (((int)numBits + 7) >> 3);
    
    if (is.isError())
    {
        DEBUG_PRINT(DEBUG_ERROR, "Input stream error");
        retval = E_MALFORMED_REQUEST;
    }
    
    if (retval == E_SUCCESS)
    {
        
        if (m_readBufSize < numBytes)
        {
            retval = resizeReadBuf(numBytes);
        }
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_gpuDbgHw->scan(numBits, m_readBuf);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    if (retval == E_SUCCESS)
    {
        os.putByteArray(numBytes, m_readBuf);
    }
    else
    {
        os.putInt(0); // Zero-length array
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t gpuScan::yaap_enableGet(YAAP_METHOD_PARAMS)
{
    bool enabled = false;
    uint32_t retval = m_hdrHw->isEnabled(enabled);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(enabled);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t gpuScan::yaap_enableSet(YAAP_METHOD_PARAMS)
{
    bool enabled = is.getByte() != 0;
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS) 
    {
        if (m_hdrSet != NULL)
        {
            retval = m_hdrSet->setEnabled(enabled);
        }
        else
        {
            retval = E_NOT_IMPLEMENTED;
        }
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t gpuScan::yaap_optionSckFrequencyGet(YAAP_METHOD_PARAMS)
{
    double maxFreq = 1 / 30e-9;     // !!! Max freq currently hard coded to 33.333333 MHz
    uint32_t freq = 0;
    uint32_t retval = E_NOT_IMPLEMENTED;
    uint32_t divisorInt = 0;
    if (m_sckFreq != NULL)
    {
        retval = m_sckFreq->sckFreqGet(freq);
        double divisor = maxFreq / (double)freq;
        divisorInt = (uint32_t)(divisor + 0.5);
        DEBUG_PRINT(DEBUG_VERBOSE, "int div: %d", divisorInt); 
        divisorInt--; //YAAP spec is sckFreq = maxFreq/(divisor + 1) so need to subtract 1 from calculated divisor
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.putDouble(maxFreq);
    os.putInt(divisorInt);
    os.endBlockSize();

    return retval;    
}

/******************************************************************************/

uint32_t gpuScan::yaap_optionSckFrequencySet(YAAP_METHOD_PARAMS)
{
    uint32_t divisor = is.getInt();
    uint32_t retval = checkInputStream(is);

    if ((retval == E_SUCCESS) && (m_sckFreq == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        DEBUG_PRINT(DEBUG_VERBOSE, "set div: %d", divisor); 
        uint32_t freq = (33333333 + (divisor >> 1)) / (divisor + 1);     // !!! Max freq currently hard coded to 33.333333 MHz - YAAP spec actual divisor as sent divisor+1
        DEBUG_PRINT(DEBUG_VERBOSE, "set calc freq: %d", freq); 
        retval = m_sckFreq->sckFreqSet(freq);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

} // namespace classes
} // namespace yaap
