/******************************************************************************/
/*! \file lpcPostCode.cpp YAAP LpcPostCode class implementation.
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

#include "classes/lpcPostCode.h"

#define YAAP_LPC_POST_CODE_VERSION yaapVersion(1, 0, 0)

namespace yaap {
namespace classes {

/******************************************************************************/

lpcPostCode::lpcPostCode(const char *name, hal::ILpcPostCode *hw)
    : base(name, "lpcPostCode", YAAP_LPC_POST_CODE_VERSION),  
      m_postCodeHw(hw)
{
    m_fifoHw = dynamic_cast<hal::ILpcPostCode_fifo *>(hw);

    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_postCodeGet,            "postCodeGet",            "int lpcPostCode.postCodeGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_fifoSizeGet,            "fifoSizeGet",            "int lpcPostCode.fifoSizeGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_fifoGet,                "fifoGet",                "struct { uint8_t data; uint8_t offset; bool flag; uint64_t timestamp; } [] lpcPostCode.fifoGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_fifoClear,              "fifoClear",              "void lpcPostCode.fifoClear(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_fifoStatusGet,          "fifoStatusGet",          "struct { int count; bool overflow; bool underflow; bool full; bool empty; } lpcPostCode.fifoStatusGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_fifoStatusClear,        "fifoStatusClear",        "void lpcPostCode.fifoStatusClear(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionPortSet,          "optionPortSet",          "void lpcPostCode.optionPortSet(uint8_t port)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionPortGet ,         "optionPortGet",          "uint8_t lpcPostCode.optionPortGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionPassiveSet,       "optionPassiveSet",       "void lpcPostCode.optionPassiveSet(bool passive)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionPassiveGet,       "optionPassiveGet",       "bool lpcPostCode.optionPassiveGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionFifoCircularSet,  "optionFifoCircularSet",  "void lpcPostCode.optionFifoCircularSet(bool circular)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionFifoCircularGet,  "optionFifoCircularGet",  "bool lpcPostCode.optionFifoCircularGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionFifoDiscardSet,   "optionFifoDiscardSet",   "void lpcPostCode.optionFifoDiscardSet(bool discard)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionFifoDiscardGet,   "optionFifoDiscardGet",   "bool lpcPostCode.optionFifoDiscardGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionFifoAutoResetSet, "optionFifoAutoResetSet", "void lpcPostCode.optionFifoAutoResetSet(bool autoReset)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_optionFifoAutoResetGet, "optionFifoAutoResetGet", "bool lpcPostCode.optionFifoAutoResetGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_enableSet,              "enableSet",              "void lpcPostCode.enableSet(bool enable)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, lpcPostCode, yaap_enableGet,              "enableGet",              "bool lpcPostCode.enableGet(void)"));
    
    DEBUG_PRINT_SUPPORTED_FUNC("LPC Post Code Reader", hw, hal::ILpcPostCode_fifo);

    initialize();
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_postCodeGet(YAAP_METHOD_PARAMS)
{
    uint32_t code;
    uint32_t retval = m_postCodeHw->postCodeGet(code);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(code);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_fifoSizeGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    int entries = 0;
    bool circular = false;
    bool discard = false;
    bool autoClear = false;
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoSetupGet(entries, circular, discard, autoClear);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(entries);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_fifoGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    vector<hal::postCodeFifoEntry> fifo;
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoDump(fifo);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    if (retval == E_SUCCESS)
    {
        os.putInt(fifo.size()); // Array count
        for (vector<hal::postCodeFifoEntry>::const_iterator i = fifo.begin(); i != fifo.end(); i++)
        {
            os.putByte((*i).data);
            os.putByte((*i).offset);
            os.putBool((*i).statusFlag);
            os.putLong((*i).timestamp);
        }
    }
    else
    {
        os.putInt(0); // Zero-length array
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_fifoClear(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoClear();
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;    
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_fifoStatusGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    int entries = 0;
    bool overflow = false;
    bool full = false;
    bool empty = false;
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoStatusGet(entries, overflow, full, empty);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(entries);
    os.putBool(overflow);
    os.putBool(false); // Underflow
    os.putBool(full);
    os.putBool(empty);
    os.endBlockSize();
    
    return retval;    
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_fifoStatusClear(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoStatusClear();
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;    
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionPortSet(YAAP_METHOD_PARAMS)
{
    // Hard-coded to match the existing YAAP spec.
    int ports[4] = { 0x80, 0x84, 0xE0, 0x160 };

    uint8_t portCode = is.getByte();
    uint32_t retval = checkInputStream(is);
    
    if (portCode > 3) // Must match size of 'ports' array
    {
        retval = E_INVALID_SETTING;
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_postCodeHw->portSet(ports[portCode]);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;    
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionPortGet(YAAP_METHOD_PARAMS)
{
    // Hard-coded to match the existing YAAP spec.
    int ports[4] = { 0x80, 0x84, 0xE0, 0x160 };

    int port = 0;
    int portCode = -1;
    uint32_t retval = m_postCodeHw->portGet(port);
    
    if (retval == E_SUCCESS)
    {
        for (int i = 0; i < 4; i++)  // Must match the size of 'ports' array
        {
            if (port == ports[i])
            {
                portCode = i;
                break;
            }
        }
    }
    
    if (portCode == -1)
    {
        portCode = 0;
        retval = E_INVALID_SETTING;
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(portCode);
    os.endBlockSize();
    
    return retval;    
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionPassiveSet(YAAP_METHOD_PARAMS)
{
    bool passive = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_postCodeHw->passiveSet(passive);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionPassiveGet(YAAP_METHOD_PARAMS)
{
    bool passive = false;
    uint32_t retval = m_postCodeHw->passiveGet(passive);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(passive);
    os.endBlockSize();
    
    return retval;            
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionFifoCircularSet(YAAP_METHOD_PARAMS)
{
    bool circular = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoCircularSet(circular);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionFifoCircularGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    int entries = 0;
    bool circular = false;
    bool discard = false;
    bool autoClear = false;
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoSetupGet(entries, circular, discard, autoClear);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(circular);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionFifoDiscardSet(YAAP_METHOD_PARAMS)
{
    bool discard = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoDiscardSequentialSet(discard);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionFifoDiscardGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    int entries = 0;
    bool circular = false;
    bool discard = false;
    bool autoClear = false;
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoSetupGet(entries, circular, discard, autoClear);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(discard);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionFifoAutoResetSet(YAAP_METHOD_PARAMS)
{
    bool autoClear = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    }
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoAutoClearSet(autoClear);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_optionFifoAutoResetGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = ((m_fifoHw == NULL) ? E_NOT_IMPLEMENTED : E_SUCCESS);
    int entries = 0;
    bool circular = false;
    bool discard = false;
    bool autoClear = false;
    
    if (retval == E_SUCCESS)
    {
        retval = m_fifoHw->fifoSetupGet(entries, circular, discard, autoClear);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(autoClear);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_enableSet(YAAP_METHOD_PARAMS)
{
    bool en = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_postCodeHw->enableSet(en);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;        
}

/******************************************************************************/
    
uint32_t lpcPostCode::yaap_enableGet(YAAP_METHOD_PARAMS)
{
    bool en = false;
    uint32_t retval = m_postCodeHw->enableGet(en);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putBool(en);
    os.endBlockSize();
    
    return retval;
}


/******************************************************************************/

} // namespace classes
} // namespace yaap
