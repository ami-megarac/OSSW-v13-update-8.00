/******************************************************************************/
/*! \file jtag.cpp YAAP JTAG class common implementation.
 *
 * This file contains the implementation of the common portions of the JTAG class.
 *
 * <pre>
 * Copyright (C) 2009-2012, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
#include <inttypes.h>
#include <sys/time.h>
#include <ctime>

#include "classes/jtag.h"
#include "infrastructure/profile.h"

//! Define this to get shift details in the output log
//#define LOG_SHIFT_DATA
#define LOG_SHIFT_LEVEL DEBUG_VERBOSE

#define YAAP_JTAG_VERSION yaapVersion(1, 2, 9)

using namespace yaap;

namespace yaap {
namespace classes {

vector<jtag *> jtag::m_instances;

/******************************************************************************/

jtag::jtag(const char *name, hal::IJtag *jtagHw, hal::IHeader *hdrHw,  uint32_t preTCK, uint32_t postTCK) 
    : base(name, "jtag", YAAP_JTAG_VERSION), m_jtagHw(jtagHw), m_hdrHw(hdrHw)
{
    memset(&m_shiftIrParams, 0, sizeof(shiftParams));
    memset(&m_shiftDrParams, 0, sizeof(shiftParams));

    // Set the termination state default to RTI per spec
    m_shiftIrParams.termState = hal::JTAG_STATE_RTI;
    m_shiftDrParams.termState = hal::JTAG_STATE_RTI;

    m_shiftIrParams.preTck = preTCK;
    m_shiftIrParams.postTck = postTCK;
    m_shiftDrParams.preTck = preTCK;
    m_shiftDrParams.postTck = postTCK;

#ifdef JTAG_INSTANCES_SHARE_OUTPUT_BUFFER
    if (m_instances.size() > 0)
    {
        m_tdoBuffer = m_instances[0].m_tdoBuffer;
        m_tdoBufSize = m_instances[0].m_tdoBufSize;
    }
    else
    {
        m_tdoBuffer = NULL;
        m_tdoBufSize = 0;
    }
#else
    m_tdoBuffer = NULL;
    m_tdoBufSize = 0;
#endif

    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_shiftIR,                   "shiftIR",                   "char [] jtag.shiftIR(int numBits, char inData[])"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_shiftDR,                   "shiftDR",                   "char [] jtag.shiftDR(int numBits, char inData[])"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionTdoExpectedGet,      "optionTdoExpectedGet",      "struct { int irBits; char irMask[]; char irExpected[]; int drBits; char drMask[]; char drExpected[]; } jtag.optionTdoExpectedGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionTdoExpectedSet,      "optionTdoExpectedSet",      "void jtag.optionTdoExpectedSet(int irBits, char irMask[], char irExpected[], int drBits, char drMask[], char drExpected[])"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionRetryGet,            "optionRetryGet",            "struct { int irCnt; bool irEn; int drCnt; bool drEn } jtag.optionRetryGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionRetrySet,            "optionRetrySet",            "void jtag.optionRetrySet(int irCnt, bool irEn, int drCnt, bool drEn)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionRepeatGet,           "optionRepeatGet",           "struct { int irCnt; bool irEn; int drCnt; bool drEn } jtag.optionRepeatGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionRepeatSet,           "optionRepeatSet",           "void jtag.optionRepeatSet(int irCnt, bool irEn, int drCnt, bool drEn)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionTerminationStateGet, "optionTerminationStateGet", "struct { int irTermState; int drTermState } jtag.optionTerminationStateGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionTerminationStateSet, "optionTerminationStateSet", "void jtag.optionTerminationStateSet(int irTermState, int drTermState)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionPrePostTckGet,       "optionPrePostTckGet",       "struct { int irPre; int irPost; int drPre; int drPost; } jtag.optionPrePostTckGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionPrePostTckSet,       "optionPrePostTckSet",       "void jtag.optionPrePostTckSet(int irPre, int irPost, int drPre, int drPost)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_enableSet,                 "enableSet",                 "void jtag.enableSet(bool)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_enableGet,                 "enableGet",                 "bool jtag.enableGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_ioTrstSet,                 "ioTrstSet", "void jtag.ioTrstSet(bool assert, bool enable)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_ioTrstGet,                 "ioTrstGet", "struct { bool asserted, bool enabled } jtag.ioTrstGet(void)"));

    m_jtagNonDestruct = dynamic_cast<hal::IJtag_nonDestructiveRead *>(jtagHw);
    m_tapState = dynamic_cast<hal::IJtag_tapState *>(jtagHw);
    m_tckFreq = dynamic_cast<hal::IJtag_tckFrequency *>(jtagHw);
    m_hdrSet = dynamic_cast<hal::IHeader_setEnabled *>(hdrHw);
    m_hdrTrst = dynamic_cast<hal::IHeader_trst *>(hdrHw);

    if (m_jtagNonDestruct != NULL)
    {
#if 0                
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_shiftIRRead, "shiftIRRead", "char [] jtag.shiftIRRead(int numBits)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_shiftDRRead, "shiftDRRead", "char [] jtag.shiftDRRead(int numBits)"));
#endif
    }
    
    if (m_tapState != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_tapStateSet, "tapStateSet", "void jtag.tapStateSet(int state)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_tapStateGet, "tapStateGet", "int jtag.tapStateGet(void)"));
    }

    if (m_tckFreq != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionTckFrequencySet, "optionTckFrequencySet", "void jtag.optionTckFrequencySet(double freqHz)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, jtag, yaap_optionTckFrequencyGet, "optionTckFrequencyGet", "double freqHz jtag.optionTckFrequencyGet(void)"));
    }

    m_instances.push_back(this);
    
    DEBUG_PRINT_SUPPORTED_FUNC("JTAG", jtagHw, hal::IJtag_nonDestructiveRead);
    DEBUG_PRINT_SUPPORTED_FUNC("JTAG", jtagHw, hal::IJtag_tapState);
    DEBUG_PRINT_SUPPORTED_FUNC("JTAG", jtagHw, hal::IJtag_tckFrequency);

    initialize();
}

/******************************************************************************/

bool jtag::resizeOutputBuffer(uint32_t newSize)
{
    if (m_tdoBuffer != NULL)
    {
        delete [] m_tdoBuffer;
    }
    m_tdoBuffer = new uint8_t[newSize];
    if (m_tdoBuffer == NULL)
    {
        m_tdoBuffer = new uint8_t[m_tdoBufSize];  // Try to get the memory back that we just had
        if (m_tdoBuffer == NULL)
        {
            m_tdoBufSize = 0;
        }
    }
    else
    {
        m_tdoBufSize = newSize;
    }

#ifdef JTAG_INSTANCES_SHARE_OUTPUT_BUFFER
    for (vector<jtag*>::iterator i = m_instances.begin(); i != m_instances.end(); i++)
    {
        if ((*i) != this)
        {
            (*i)->m_tdoBuffer = m_tdoBuffer;
            (*i)->m_tdoBufSize = m_tdoBufSize;
        }
    }
#endif

    return (m_tdoBufSize == newSize);
}

/******************************************************************************/

inline uint32_t timeDiffusec(struct timeval &first, struct timeval &second) {
  uint64_t retval = second.tv_sec - first.tv_sec;
  retval *= 1000000;  // Make it usec

  int32_t usecDiff = second.tv_usec - first.tv_usec;
  if (usecDiff < 0) {
    usecDiff += 1000000;
  }
//  usecDiff += 500;
//  usecDiff /= 1000;  // Make it msec
  return retval + usecDiff;
}


uint32_t jtag::shiftCommon(enum hal::jtagOp operation, uint32_t numBits, uint8_t *& inData, size_t& outBytes)
{
    uint32_t retval = E_SUCCESS;
    bool connected = true;
    const char *mode;
    bool enabled = true;
    bool changed = false;

    PROF_PIN_SET(YAAP_PRF_SHF_ST);
    
    if (m_hdrHw != NULL)
    {
        retval = m_hdrHw->isConnected(connected, mode);
        
        if ((retval == E_SUCCESS) && !connected)
        {
            retval = E_JTAG_HEADER_NOT_CONNECTED;
        }
        
        if (retval == E_SUCCESS)
        {
            retval = m_hdrHw->isEnabled(enabled);
        }
        
        if ((retval == E_SUCCESS) && !enabled)
        {
            retval = E_JTAG_HEADER_NOT_ENABLED;
        }
        
        if (retval == E_SUCCESS)
        {
            // 5/17/12 - disabling as HDT doesn't currently handle this  -RDB
            //retval = m_hdrHw->isChanged(changed);
        }
        
        if ((retval == E_SUCCESS) && changed)
        {
            retval = E_JTAG_HEADER_STATUS_CHANGED;
        }
    }

    PROF_PIN_SET(YAAP_PRF_SHF_DEC);
    if (retval == E_SUCCESS)
    {
        shiftParams& params = (operation == hal::JTAG_OP_SHIFT_IR) ? m_shiftIrParams : m_shiftDrParams;
        uint32_t shiftCnt = 1;
        if (params.repeatEn)
        {
            shiftCnt += params.repeatCnt;
        }

        uint32_t shiftBytes = (numBits + 7) >> 3;
        outBytes = shiftBytes * shiftCnt;
       
        if (outBytes > m_tdoBufSize)
        {
            if (!resizeOutputBuffer(outBytes))
            {
                outBytes = 0;
                return E_OUT_OF_MEMORY;
            }
        }
        
        uint8_t *currentOutputBuf = m_tdoBuffer;
        for (unsigned int i = 0; ((i < shiftCnt) && (retval == E_SUCCESS)); i++)   // REPEAT loop
        {
            PROF_PIN_SET(YAAP_PRF_SHF_EXEC);
            int triesLeft = 1;
            uint32_t retryBytes = 0;
            if (params.retryEn)
            {
                triesLeft += params.retryCnt;
                retryBytes = (params.retryBits + 7) >> 3;
#ifdef LOG_SHIFT_DATA
                if (DEBUG_LEVEL_SELECTED(LOG_SHIFT_LEVEL))
                {
                    char *printBuf = new char[(retryBytes * 3) + 1]; // Extra char for the NULL terminator
                    char *bufPtr = printBuf;
                    if (printBuf != NULL)
                    {
                        for (int j = (int)retryBytes - 1; j >= 0; j--)
                        {
                            sprintf(bufPtr, "%02X ", params.retryMask[j]);
                            bufPtr += 3;
                        } 
                        DEBUG_PRINT(LOG_SHIFT_LEVEL, "Retry Mask     (%d bits): %s", params.retryBits, printBuf);
                        
                        bufPtr = printBuf;
                        for (int j = (int)retryBytes - 1; j >= 0; j--)
                        {
                            sprintf(bufPtr, "%02X ", params.retryExpected[j]);
                            bufPtr += 3;
                        } 
                        DEBUG_PRINT(LOG_SHIFT_LEVEL, "Retry Expected (%d bits): %s", params.retryBits, printBuf);
                        
                        delete [] printBuf;
                    }
                }
#endif // LOG_SHIFT_DATA
            }

            for ( ; triesLeft > 0; triesLeft--)         // RETRY loop
            {
                PROF_PIN_SET(YAAP_PRF_SHF_EXEC);

//  {                                                         
//    struct timeval debug_time_start, debug_time_stop;       
//    gettimeofday(&debug_time_start, NULL);

                retval = m_jtagHw->shift(operation, inData, currentOutputBuf, numBits, params.preTck, params.postTck, params.termState);


//   gettimeofday(&debug_time_stop, NULL);                         
//   printf( "YAAPD %s %d BITS TIME: %dus\n", "m_jtagHW->shift", numBits,     
//                timeDiffusec(debug_time_start, debug_time_stop));
//}





#ifdef LOG_SHIFT_DATA
                if (DEBUG_LEVEL_SELECTED(LOG_SHIFT_LEVEL))
                {
                    DEBUG_PRINT(LOG_SHIFT_LEVEL, "Shift: preTCK %d, bits %d, postTCK %d, repeat %c (%d), retry %c (%d), return %08X", 
                                params.preTck, numBits, params.postTck, params.repeatEn ? 'Y' : 'N', i, params.retryEn ? 'Y' : 'N', params.retryEn ? (params.retryCnt - triesLeft + 1) : 0, retval);
                    char *printBuf = new char[(shiftBytes * 3) + 1];  // Extra char for the NULL terminator
                    if (printBuf != NULL)
                    {
                        char *bufPtr = printBuf;
                        for (int j = (int)shiftBytes - 1; j >= 0; j--)
                        {
                            sprintf(bufPtr, "%02X ", inData[j]);
                            bufPtr += 3;
                        }
                        DEBUG_PRINT(LOG_SHIFT_LEVEL, "Shift in : %s", printBuf);
                        
                        bufPtr = printBuf;
                        for (int j = (int)shiftBytes - 1; j >= 0; j--)
                        {
                            sprintf(bufPtr, "%02X ", currentOutputBuf[j]);
                            bufPtr += 3;
                        }
                        DEBUG_PRINT(LOG_SHIFT_LEVEL, "Shift out: %s", printBuf);
                        delete [] printBuf;
                    }
                }
#endif // LOG_SHIFT_DATA
                
                if (retval != E_SUCCESS)
                {
                    break;
                }
                
                if (params.retryEn)
                {
                    PROF_PIN_SET(YAAP_PRF_SHF_RTC);
                    bool retryNeeded = false;
                    if (retval == E_SUCCESS)
                    {
                        for (unsigned int j = 0; j < retryBytes; j++)
                        {
                            if ((currentOutputBuf[j] & params.retryMask[j]) != params.retryExpected[j])
                            {
                                retryNeeded = true;
                                retval = E_JTAG_RETRIES_EXHAUSTED;  // In case this was the last retry
                                break;
                            }
                        }
                    }
                    if (!retryNeeded)
                    {
                        break;
                    }
                }
            } //End of Retry loop
            PROF_PIN_SET(YAAP_PRF_SHF_STR);
            currentOutputBuf += shiftBytes;
        } //End of Repeat loop
    } //End of if header checks successufl

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_shiftIR(YAAP_METHOD_PARAMS)
{
    PROF_PIN_SET(YAAP_PRF_FUNC_ST);
    uint32_t numBits = is.getInt();
    size_t inDataSize = is.getArrayCount();
    uint8_t *inData = is.getRawBytePtr(inDataSize);
    size_t outBytes = 0;
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        //DEBUG_PRINT(DEBUG_MOREINFO, "jtag.shiftIR: numBits = %d", numBits);
        retval = shiftCommon(hal::JTAG_OP_SHIFT_IR, numBits, inData, outBytes);
    }

    PROF_PIN_SET(YAAP_PRF_FUNC_OUT);
    os.beginBlockSize();
    os.putInt(retval);
    os.putByteArray(outBytes, m_tdoBuffer);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_shiftDR(YAAP_METHOD_PARAMS)
{
    PROF_PIN_SET(YAAP_PRF_FUNC_ST);
    uint32_t numBits = is.getInt();
    size_t inDataSize = is.getArrayCount();
    uint8_t *inData = is.getRawBytePtr(inDataSize);
    size_t outBytes = 0;
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        //DEBUG_PRINT(DEBUG_MOREINFO, "jtag.shiftDR: numBits = %d", numBits);
        retval = shiftCommon(hal::JTAG_OP_SHIFT_DR, numBits, inData, outBytes);
    }

    PROF_PIN_SET(YAAP_PRF_FUNC_OUT);
    os.beginBlockSize();
    os.putInt(retval);
    os.putByteArray(outBytes, m_tdoBuffer);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/
#if 0
uint32_t jtag::yaap_shiftIRRead(YAAP_METHOD_PARAMS)
{
    uint32_t numBits = is.getInt();
    size_t outBytes = 0;
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        //DEBUG_PRINT(DEBUG_MOREINFO, "jtag.shiftIR: numBits = %d", numBits);
        //retval = shiftCommon(JTAG_OP_SHIFT_IR, numBits, inData, outBytes);  // !!!
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.putByteArray(outBytes, m_tdoBuffer);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_shiftDRRead(YAAP_METHOD_PARAMS)
{
    uint32_t numBits = is.getInt();
    size_t outBytes = 0;
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        //DEBUG_PRINT(DEBUG_MOREINFO, "jtag.shiftDR: numBits = %d", numBits);
        //retval = shiftCommon(JTAG_OP_SHIFT_DR, numBits, inData, outBytes);  // !!!
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.putByteArray(outBytes, m_tdoBuffer);
    os.endBlockSize();

    return retval;
}
#endif
/******************************************************************************/

uint32_t jtag::yaap_tapStateGet(YAAP_METHOD_PARAMS)
{
    enum hal::jtagState state;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_tapState != NULL)
    {
        retval = m_tapState->tapStateGet(state);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt((int)state);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_tapStateSet(YAAP_METHOD_PARAMS)
{
    uint32_t tapState = is.getInt();
    uint32_t retval = checkInputStream(is);

    if ((retval == E_SUCCESS) && (m_tapState == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_tapState->tapStateSet((enum hal::jtagState)tapState);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionTdoExpectedGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_shiftIrParams.retryBits);                      // IR params
    uint32_t arraySize = (m_shiftIrParams.retryBits + 7) >> 3;
    os.putByteArray(arraySize, m_shiftIrParams.retryMask);
    os.putByteArray(arraySize, m_shiftIrParams.retryExpected);
    os.putInt(m_shiftDrParams.retryBits);                      // DR params
    arraySize = (m_shiftDrParams.retryBits + 7) >> 3;
    os.putByteArray(arraySize, m_shiftDrParams.retryMask);
    os.putByteArray(arraySize, m_shiftDrParams.retryExpected);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t jtag::setShiftParams(shiftParams& params, uint32_t numBits, uint8_t *mask, uint8_t *expected)
{
    uint32_t numBytes = (numBits + 7) >> 3;
    uint32_t arraySize = (params.retryBits + 7) >> 3;
    if (arraySize < numBytes)
    {
        if (params.retryMask != NULL)
        {
            delete [] params.retryMask;
        }
        if (params.retryExpected != NULL)
        {
            delete [] params.retryExpected;
        }

        params.retryMask = new uint8_t[numBytes];
        params.retryExpected = new uint8_t[numBytes];

        if ((params.retryMask == NULL) || (params.retryExpected == NULL))
        {
            params.retryBits = 0;
            return E_OUT_OF_MEMORY;
        }
    }

    params.retryBits = numBits;
    memcpy(params.retryMask, mask, numBytes);
    memcpy(params.retryExpected, expected, numBytes);

    return E_SUCCESS;
}

/******************************************************************************/

uint32_t jtag::yaap_optionTdoExpectedSet(YAAP_METHOD_PARAMS)
{
    uint32_t irNumBits = is.getInt();
    size_t irMaskArraySize = is.getArrayCount();
    uint8_t *irTdoMask = is.getRawBytePtr(irMaskArraySize);
    size_t irExpectedArraySize = is.getArrayCount();
    uint8_t *irTdoExpected = is.getRawBytePtr(irExpectedArraySize);
    uint32_t drNumBits = is.getInt();
    size_t drMaskArraySize = is.getArrayCount();
    uint8_t *drTdoMask = is.getRawBytePtr(drMaskArraySize);
    size_t drExpectedArraySize = is.getArrayCount();
    uint8_t *drTdoExpected = is.getRawBytePtr(drExpectedArraySize);
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        retval = setShiftParams(m_shiftIrParams, irNumBits, irTdoMask, irTdoExpected);
    }
    if (retval == E_SUCCESS)
    {
        retval = setShiftParams(m_shiftDrParams, drNumBits, drTdoMask, drTdoExpected);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionRetryGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_shiftIrParams.retryCnt);
    os.putByte(m_shiftIrParams.retryEn ? 1 : 0);
    os.putInt(m_shiftDrParams.retryCnt);
    os.putByte(m_shiftDrParams.retryEn ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionRetrySet(YAAP_METHOD_PARAMS)
{
    uint32_t irRetryCnt = is.getInt();
    bool irRetryEn = (is.getByte() != 0);
    uint32_t drRetryCnt = is.getInt();
    bool drRetryEn = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        m_shiftIrParams.retryEn = irRetryEn;
        m_shiftIrParams.retryCnt = irRetryCnt;
        m_shiftDrParams.retryEn = drRetryEn;
        m_shiftDrParams.retryCnt = drRetryCnt;
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionRepeatGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_shiftIrParams.repeatCnt);
    os.putByte(m_shiftIrParams.repeatEn ? 1 : 0);
    os.putInt(m_shiftDrParams.repeatCnt);
    os.putByte(m_shiftDrParams.repeatEn ? 1 : 0);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionRepeatSet(YAAP_METHOD_PARAMS)
{
    uint32_t irRepeatCnt = is.getInt();
    bool irRepeatEn = (is.getByte() != 0);
    uint32_t drRepeatCnt = is.getInt();
    bool drRepeatEn = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        m_shiftIrParams.repeatEn = irRepeatEn;
        m_shiftIrParams.repeatCnt = irRepeatCnt;
        m_shiftDrParams.repeatEn = drRepeatEn;
        m_shiftDrParams.repeatCnt = drRepeatCnt;
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/
namespace {
  const uint32_t bmc_2600_pclk = 200000000;
}
uint32_t jtag::yaap_optionTckFrequencyGet(YAAP_METHOD_PARAMS)
{
    uint32_t freq = 0;
    uint32_t divisorInt = 0;

    uint32_t retval = m_tckFreq->tckFreqGet(freq);

    double divisor = (bmc_2600_pclk/freq);
    divisorInt = (uint32_t)(divisor + 0.5);
    DEBUG_PRINT(DEBUG_INFO, "int div: %d", divisorInt);
    divisorInt--; //YAAP spec is TckFreq = maxFreq/(divisor + 1) so need to subtract 1 from calculated divisor

    os.beginBlockSize();
    os.putInt(retval);
    os.putDouble(bmc_2600_pclk);
    os.putInt(divisorInt);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionTckFrequencySet(YAAP_METHOD_PARAMS)
{
    uint32_t divisor = is.getInt();
    uint32_t retval = checkInputStream(is);

    if ((retval == E_SUCCESS) && (m_tckFreq == NULL)) {
      retval = E_NOT_IMPLEMENTED;

    } else if (retval == E_SUCCESS) {
      DEBUG_PRINT(DEBUG_INFO, "set div: %d", divisor);

      uint32_t freq = bmc_2600_pclk / ((0xfff & divisor) + 1);
      DEBUG_PRINT(DEBUG_INFO, "set calc freq: %d", freq);

      retval = m_tckFreq->tckFreqSet(freq);
    } else {
      printf("Failed to get divisor from input stream\n");
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionTerminationStateGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt((uint32_t)m_shiftIrParams.termState);
    os.putInt((uint32_t)m_shiftDrParams.termState);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionTerminationStateSet(YAAP_METHOD_PARAMS)
{
    uint32_t irTermState = is.getInt();
    uint32_t drTermState = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        switch (irTermState)
        {
            case hal::JTAG_STATE_TLR:
            case hal::JTAG_STATE_RTI:
            case hal::JTAG_STATE_SHIFT_IR:
            case hal::JTAG_STATE_PAUSE_IR:
                break;
                
            default:
                retval = E_JTAG_INVALID_IR_STATE;
        }
        
        switch (drTermState)
        {
            case hal::JTAG_STATE_TLR:
            case hal::JTAG_STATE_RTI:
            case hal::JTAG_STATE_SHIFT_DR:
            case hal::JTAG_STATE_PAUSE_DR:
                break;
                
            default:
                retval = E_JTAG_INVALID_DR_STATE;
        }
    }

    if (retval == E_SUCCESS)
    {
        m_shiftIrParams.termState = (enum hal::jtagState)irTermState;
        m_shiftDrParams.termState = (enum hal::jtagState)drTermState;
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionPrePostTckGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_shiftIrParams.preTck);
    os.putInt(m_shiftIrParams.postTck);
    os.putInt(m_shiftDrParams.preTck);
    os.putInt(m_shiftDrParams.postTck);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_optionPrePostTckSet(YAAP_METHOD_PARAMS)
{
    uint32_t irPreTck = is.getInt();
    uint32_t irPostTck = is.getInt();
    uint32_t drPreTck = is.getInt();
    uint32_t drPostTck = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        m_shiftIrParams.preTck = irPreTck;
        m_shiftIrParams.postTck = irPostTck;
        m_shiftDrParams.preTck = drPreTck;
        m_shiftDrParams.postTck = drPostTck;
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_enableSet(YAAP_METHOD_PARAMS)
{
    bool enabled = is.getByte() != 0;
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_hdrSet == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_hdrSet->setEnabled(enabled);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_enableGet(YAAP_METHOD_PARAMS)
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

uint32_t jtag::yaap_ioTrstSet(YAAP_METHOD_PARAMS)
{
    bool assert = is.getByte() != 0;
    bool enable = is.getByte() != 0;
    bool enabled = false;
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_hdrHw->isEnabled(enabled);
    }
    if ((retval == E_SUCCESS) && !enabled)
    {
        if (!enable)
        {
            retval = E_HDT_HEADER_NOT_ENABLED;
        }
        else if (m_hdrSet == NULL)
        {
            retval = E_NOT_IMPLEMENTED;
        }
        else
        {
            retval = m_hdrSet->setEnabled(true);
        }
    }

    if (retval == E_SUCCESS)
    {
        if (m_hdrTrst == NULL)
        {
            retval = E_NOT_IMPLEMENTED;
        }
        else
        {
            retval = m_hdrTrst->trstSet(assert);
        }
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t jtag::yaap_ioTrstGet(YAAP_METHOD_PARAMS)
{
    bool asserted = false;
    bool enabled = false;
    uint32_t retval = E_SUCCESS;

    if (m_hdrTrst == NULL || m_hdrHw == NULL)
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_hdrHw->isEnabled(enabled);
    }
    if (retval == E_SUCCESS)
    {
        retval = m_hdrTrst->trstGet(asserted);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(asserted);
    os.putBool(enabled);
    os.endBlockSize();

    return retval;
}


/******************************************************************************/

} // namespace classes
} // namespace yaap
