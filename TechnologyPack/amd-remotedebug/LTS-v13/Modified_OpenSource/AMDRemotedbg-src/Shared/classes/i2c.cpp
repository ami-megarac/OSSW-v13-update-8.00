/******************************************************************************/
/*! \file i2c.cpp YAAP I2C class common implementation.
 *
 * This file contains the implementation of the common portions of the I2C class.
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

#include "classes/i2c.h"

#define YAAP_I2C_VERSION yaapVersion(1, 0, 9)

namespace yaap {
namespace classes {

/******************************************************************************/

i2c::i2c(const char *name, hal::II2c *i2cHw, hal::IHeader *hdrHw)
    : base(name, "i2c", YAAP_I2C_VERSION), 
      m_i2cHw(i2cHw), m_hdrHw(hdrHw), m_readBuf(NULL), m_readBufSize(0)
{
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, i2c, yaap_transfer, "transfer", "struct { int bytesWritten; int bytesRead; char [] readData; } i2c.transfer(int deviceAddr, int wrCnt, char [] wrData, int rdCnt)"));

    m_i2cFreq = dynamic_cast<hal::II2c_sclFrequency *>(hdrHw);

    if (m_i2cFreq != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, i2c, yaap_optionSclFrequencyGet, "optionSclFrequencyGet", "int i2c.optionSclFrequencyGet()"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, i2c, yaap_optionSclFrequencySet, "optionSclFrequencySet", "void i2c.optionSclFrequencySet(int hz)"));
    }
    
    DEBUG_PRINT_SUPPORTED_FUNC("I2C", i2cHw, hal::II2c_sclFrequency);

    initialize();
}

/******************************************************************************/

uint32_t i2c::resizeReadBuf(int newSize)
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

uint32_t i2c::yaap_transfer(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    uint32_t deviceAddr = is.getInt();
    int writeCnt = (int)is.getInt();
    size_t arraySize = is.getArrayCount();
    uint8_t *wrData = is.getRawBytePtr(arraySize);
    int readCnt = (int)is.getInt();
    
    if (is.isError())
    {
        DEBUG_PRINT(DEBUG_ERROR, "Input stream error");
        retval = E_MALFORMED_REQUEST;
    }
    
    if ((retval == E_SUCCESS) && (readCnt > m_readBufSize))
    {
        retval = resizeReadBuf(readCnt);
    }

    if (retval == E_SUCCESS)
    {
        deviceAddr >>= 1;  // YAAP spec includes the R/W bit in the address, but the HAL does not
        retval = m_i2cHw->transfer(deviceAddr, writeCnt, wrData, readCnt, m_readBuf);
    }
    else
    {
        writeCnt = 0;
        readCnt = 0;
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(writeCnt);
    os.putInt(readCnt);
    os.putByteArray(readCnt, m_readBuf);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t i2c::yaap_optionSclFrequencyGet(YAAP_METHOD_PARAMS)
{
    uint32_t freq = 0;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_i2cFreq != NULL)
    {
        retval = m_i2cFreq->getFrequency(freq);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(freq);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t i2c::yaap_optionSclFrequencySet(YAAP_METHOD_PARAMS)
{
    uint32_t freq = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_i2cFreq == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_i2cFreq->setFrequency(freq);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

} // namespace classes
} // namespace yaap
