/**************************************************************************/
/*! \file hdt.cpp YAAP HDT class implementation.
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
 **************************************************************************/

#include "classes/hdt.h"
#include "stdio.h"

#define YAAP_HDT_VERSION yaapVersion(1, 1, 9)

namespace yaap {
namespace classes {

/******************************************************************************/

hdt::hdt(const char *name, hal::IHdt *hdtHw, hal::ITriggers *trigHw, hal::IHeader *hdrHw) 
    : base(name, "hdt", YAAP_HDT_VERSION), m_hdrHw(hdrHw), m_hdtHw(hdtHw), m_triggersHw(trigHw)
{
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbreqGet,         "ioDbreqGet",         "bool hdt.ioDbreqGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbreqSet,         "ioDbreqSet",         "void hdt.ioDbreqSet(bool asserted)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbrdyGet,         "ioDbrdyGet",         "bool [] hdt.ioDbrdyGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_enableGet,          "enableGet",          "bool hdt.enableGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_headerStatusGet,    "headerStatusGet",    "struct { struct { string name; bool connected; bool changed; bool conflicted; } [] hdrs; struct { } [] conflicts; } hdt.headerStatusGet(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionFrequencyGet, "optionFrequencyGet", "double hdt.optionFrequencyGet(void)"));

    m_dbrdyMask = dynamic_cast<hal::IHdt_dbrdyMask *>(hdtHw);
    m_dbrdySnapshot = dynamic_cast<hal::IHdt_dbrdySnapshot *>(hdtHw);
    m_dmod = dynamic_cast<hal::IHdt_dbreqOnDbrdy *>(hdtHw);
    m_dbreqOnReset = dynamic_cast<hal::IHdt_dbreqOnReset *>(hdtHw);
    m_dbreqOnTrig = dynamic_cast<hal::IHdt_dbreqOnTrigger *>(hdtHw);
    m_dbreqPulseWidth = dynamic_cast<hal::IHdt_dbreqPulseWidthSet *>(hdtHw);
    m_dbreqSnapshot = dynamic_cast<hal::IHdt_dbreqSourceSnapshot *>(hdtHw);
    m_pwrOkGet = dynamic_cast<hal::IHdt_pwrokGet *>(hdtHw);
    m_resetGet = dynamic_cast<hal::IHdt_resetGet *>(hdtHw);
    m_hdrEnable = dynamic_cast<hal::IHeader_setEnabled *>(hdrHw);
    m_hdrChange = dynamic_cast<hal::IHeader_changeDetect *>(hdrHw);
    m_trigManual = NULL; // To be set later

    if (m_dbrdySnapshot != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbrdySnapshotGet,   "ioDbrdySnapshotGet",   "bool [] hdt.ioDbrdySnapshotGet(void)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbrdySnapshotClear, "ioDbrdySnapshotClear", "void hdt.ioDbrdySnapshotClear(void)"));
    }
    
    if (m_dbreqSnapshot != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbreqSourceSnapshotGet,   "ioDbreqSourceSnapshotGet",   "int hdt.ioDbreqSourceSnapshotGet(void)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioDbreqSourceSnapshotClear, "ioDbreqSourceSnapshotClear", "void hdt.ioDbreqSourceSnapshotClear(void)"));
    }
    
    if (m_resetGet != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioResetGet, "ioResetGet", "bool hdt.ioResetGet(void)"));
    }
    
    if (m_pwrOkGet != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioPwrokGet, "ioPwrokGet", "bool hdt.ioPwrokGet(void)"));
    }
    
    if (m_triggersHw != NULL)
    {
        m_trigManual = dynamic_cast<hal::ITriggers_manual *>(m_triggersHw);

        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionTriggerSetupSet, "optionTriggerSetupSet", "void hdt.optionTriggerSetupSet(int channel, int direction, int level, int inputPolarity, int outputSource, int outputEvent, int outputPolarity, int outputPulseWidth)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionTriggerSetupGet, "optionTriggerSetupGet", "struct { int direction; int level; int inputPolarity; int outputSource; int outputEvent; int outputPolarity; int outputPulseWidth; } hdt.optionTriggerSetupGet(void)"));

        if (m_trigManual != NULL)
        {
            m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioTriggerSet, "ioTriggerSet", "void hdt.ioTriggerSet(int channel, bool asserted)"));
            m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_ioTriggerGet, "ioTriggerGet", "bool hdt.ioTriggerGet(int channel)"));
        }
        if (m_dbreqOnTrig != NULL)
        {
            m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqOnTriggerSet, "optionDbreqOnTriggerSet", "void hdt.optionDbreqOnTriggerSet(bool enable, int source, bool sourceInvert, int event)"));
            m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqOnTriggerGet, "optionDbreqOnTriggerGet", "struct { bool enable; int source; bool sourceInvert; int event; } hdt.optionDbreqOnTriggerGet(void)"));
        }
        
        DEBUG_PRINT_SUPPORTED_FUNC("HDT Triggers", m_triggersHw, hal::ITriggers_manual);
        DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbreqOnTrigger);
    }
    
    if (m_dmod != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqOnDbrdySet, "optionDbreqOnDbrdySet", "void hdt.optionDbreqOnDbrdySet(bool enabled)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqOnDbrdyGet, "optionDbreqOnDbrdyGet", "bool hdt.optionDbreqOnDbrdyGet(void)"));
    }
    
    if (m_dbreqOnReset != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqOnResetSet, "optionDbreqOnResetSet", "void hdt.optionDbreqOnResetSet(bool enabled)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqOnResetGet, "optionDbreqOnResetGet", "bool hdt.optionDbreqOnResetGet(void)"));
    }
    
    if (m_dbreqPulseWidth != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqPulseWidthSet, "optionDbreqPulseWidthSet", "void hdt.optionDbreqPulseWidthSet(int clocks)"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbreqPulseWidthGet, "optionDbreqPulseWidthGet", "int hdt.optionDbreqPulseWidthGet(void)"));
    }
    
    if (m_dbrdyMask != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbrdyMaskSet, "optionDbrdyMaskSet", "void hdt.optionDbrdyMaskSet(bool maskBits[])"));
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_optionDbrdyMaskGet, "optionDbrdyMaskGet", "bool [] hdt.optionDbrdyMaskGet(void)"));
    }

    if (m_hdrEnable != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_enableSet, "enableSet", "void hdt.enableSet(void)"));
    }
    
    if (m_hdrChange != NULL)
    {
        m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, hdt, yaap_headerStatusClear, "headerStatusClear", "void hdt.headerStatusClear(void)"));
    }

    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbrdyMask);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbrdySnapshot);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbreqOnDbrdy);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbreqOnReset);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbreqOnTrigger);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbreqPulseWidthSet);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_dbreqSourceSnapshot);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_pwrokGet);
    DEBUG_PRINT_SUPPORTED_FUNC("HDT", m_hdtHw, hal::IHdt_resetGet);

    initialize();
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbreqGet(YAAP_METHOD_PARAMS)
{
    bool asserted = false;
    
    uint32_t retval = m_hdtHw->dbreqGet(asserted);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(asserted ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbreqSet(YAAP_METHOD_PARAMS)
{
    bool assert = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        retval = m_hdtHw->dbreqSet(assert);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbrdyGet(YAAP_METHOD_PARAMS)
{
    uint8_t asserted = 0;
    uint32_t retval = m_hdtHw->dbrdyGet(asserted);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(8);   // !!! We should probably just return the byte here and let the client unpack it (future YAAP change)
    for (int i = 0; i < 8; i++)
    {
        os.putByte((asserted & (1 << i)) ? 1 : 0);
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbrdySnapshotGet(YAAP_METHOD_PARAMS)
{
    uint8_t asserted = 0;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbrdySnapshot != NULL)
    {
        retval = m_dbrdySnapshot->dbrdySnapshotGet(asserted);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(8);   // !!! We should probably just return the byte here
    for (int i = 0; i < 8; i++)
    {
        os.putByte((asserted & (1 << i)) ? 1 : 0);
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbrdySnapshotClear(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbrdySnapshot != NULL)
    {
        retval = m_dbrdySnapshot->dbrdySnapshotClear();
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbreqSourceSnapshotGet(YAAP_METHOD_PARAMS)
{
    enum hal::dbreqSource src;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbreqSnapshot != NULL)
    {
        retval = m_dbreqSnapshot->dbreqSourceSnapshotGet(src);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt((int)src);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioDbreqSourceSnapshotClear(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbreqSnapshot != NULL)
    {
        retval = m_dbreqSnapshot->dbreqSourceSnapshotClear();
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioResetGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_NOT_IMPLEMENTED;
    bool asserted = false;
    
    if (m_resetGet != NULL)
    {
        retval = m_resetGet->resetGet(asserted);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(asserted ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioPwrokGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_NOT_IMPLEMENTED;
    bool asserted = false;
    
    if (m_pwrOkGet != NULL)
    {
        retval = m_pwrOkGet->pwrokGet(asserted);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(asserted ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioTriggerSet(YAAP_METHOD_PARAMS)
{
    uint32_t channel = is.getInt();
    bool asserted = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_trigManual == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_trigManual->triggerSet(channel, asserted);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_ioTriggerGet(YAAP_METHOD_PARAMS)
{
    bool asserted = false;
    uint32_t channel = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_trigManual == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_trigManual->triggerGet(channel, asserted);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(asserted ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqOnDbrdySet(YAAP_METHOD_PARAMS)
{
    bool en = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_dmod == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_dmod->dbreqOnDbrdySet(en);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqOnDbrdyGet(YAAP_METHOD_PARAMS)
{
    bool en = false;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dmod != NULL)
    {
        retval = m_dmod->dbreqOnDbrdyGet(en);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(en ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqOnResetSet(YAAP_METHOD_PARAMS)
{
    bool en = (is.getByte() != 0);
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_dbreqOnReset == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_dbreqOnReset->dbreqOnResetSet(en);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqOnResetGet(YAAP_METHOD_PARAMS)
{
    bool en = false;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbreqOnReset != NULL)
    {
        retval = m_dbreqOnReset->dbreqOnResetGet(en);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(en ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqOnTriggerSet(YAAP_METHOD_PARAMS)
{
    bool enable = (is.getByte() != 0);
    enum hal::triggerSource source = (enum hal::triggerSource)is.getInt();
    bool invert = (is.getByte() != 0);
    enum hal::triggerEvent event = (enum hal::triggerEvent)is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_dbreqOnTrig == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_dbreqOnTrig->dbreqOnTriggerSet(enable, source, invert, event);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqOnTriggerGet(YAAP_METHOD_PARAMS)
{
    bool enable = false;
    enum hal::triggerSource source;
    bool invert = false;
    enum hal::triggerEvent event;
    uint32_t retval = E_NOT_IMPLEMENTED;
    
    if (m_dbreqOnTrig != NULL)
    {
        retval = m_dbreqOnTrig->dbreqOnTriggerGet(enable, source, invert, event);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(enable ? 1 : 0);
    os.putInt((int)source);
    os.putByte(invert ? 1 : 0);
    os.putInt((int)event);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqPulseWidthSet(YAAP_METHOD_PARAMS)
{
    uint32_t clocks = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    // YAAP spec passes the pulse width in clocks; we're hard-coded to 66 MHz (Wombat) for now.
    // This YAAP method should be deprecated and replaced by a method that passes the value in usec.
    
    uint32_t usec = (clocks + 12) / 25;
    
    if ((retval == E_SUCCESS) && (m_dbreqPulseWidth == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_dbreqPulseWidth->dbreqPulseWidthSet(usec);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbreqPulseWidthGet(YAAP_METHOD_PARAMS)
{
    uint32_t usec = 0;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbreqPulseWidth != NULL)
    {
        retval = m_hdtHw->dbreqPulseWidthGet(usec);
    }
    
    // YAAP spec passes the pulse width in clocks; we're hard-coded to 66 MHz (Wombat) for now.
    // This YAAP method should be deprecated and replaced by a method that passes the value in usec.
    
    uint32_t clocks = usec * 25;
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(clocks);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbrdyMaskSet(YAAP_METHOD_PARAMS)
{
    size_t arraySize = is.getArrayCount();
    uint8_t *array = is.getRawBytePtr(arraySize);
    uint32_t retval = checkInputStream(is);
    
    uint8_t enabled = 0;
    for (unsigned int i = 0; i < arraySize; i++)
    {
        if (array[i])
        {
            enabled |= (1 << i);
        }
    }

    if ((retval == E_SUCCESS) && (m_dbrdyMask == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_dbrdyMask->dbrdyMaskSet(enabled);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionDbrdyMaskGet(YAAP_METHOD_PARAMS)
{
    uint8_t enabled = 0;
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_dbrdyMask != NULL)
    {
        retval = m_dbrdyMask->dbrdyMaskGet(enabled);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(8);
    for (int i = 0; i < 8; i++)
    {
        os.putByte((enabled & (1 << i)) ? 1 : 0);
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionTriggerSetupSet(YAAP_METHOD_PARAMS)
{
    struct hal::triggerConfig cfg;
    
    cfg.channel = is.getInt();
    cfg.direction = (enum hal::triggerDirection)is.getInt();
    cfg.voltage = (enum hal::triggerLevel)is.getInt();
    cfg.inputPolarity = (enum hal::triggerPolarity)is.getInt();
    cfg.outputSource = (enum hal::triggerOutputSrc)is.getInt();
    cfg.outputEvent = (enum hal::triggerEvent)is.getInt();
    cfg.outputPolarity = (enum hal::triggerPolarity)is.getInt();
    cfg.outputPulseNsec = is.getInt();
    uint32_t retval = checkInputStream(is);

    if (retval == E_SUCCESS)
    {
        retval = m_triggersHw->triggerSetupSet(cfg);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}


/******************************************************************************/

uint32_t hdt::yaap_optionTriggerSetupGet(YAAP_METHOD_PARAMS)
{
    struct hal::triggerConfig cfg;
    uint32_t channel = is.getInt();
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = m_triggersHw->triggerSetupGet(channel, cfg);
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt((int)cfg.direction);
    os.putInt((int)cfg.voltage);
    os.putInt((int)cfg.inputPolarity);
    os.putInt((int)cfg.outputSource);
    os.putInt((int)cfg.outputEvent);
    os.putInt((int)cfg.outputPolarity);
    os.putInt(cfg.outputPulseNsec);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_enableGet(YAAP_METHOD_PARAMS)
{
    bool enabled = false;
    uint32_t retval = m_hdrHw->isEnabled(enabled);
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(enabled ? 1 : 0);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_enableSet(YAAP_METHOD_PARAMS)
{
    bool enabled = is.getByte() != 0;
    uint32_t retval = checkInputStream(is);

    if ((retval == E_SUCCESS) && (m_hdrEnable == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        m_hdrEnable->setEnabled(enabled);
    }
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_optionFrequencyGet(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putDouble(1 / 4.04040404e-8);    // !!! Hard-coded to 24.75MHz
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_headerStatusGet(YAAP_METHOD_PARAMS)
{
    bool connected = false;
    const char *mode = "";
    bool changed = false;
    bool conflicted = false;
    
    uint32_t retval = m_hdrHw->isInConflict(conflicted);
    if (retval == E_SUCCESS)
    {
        retval = m_hdrHw->isConnected(connected, mode);
    }
    if ((retval == E_SUCCESS) && (m_hdrChange != NULL))
    {
        retval = m_hdrChange->isChanged(changed);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(1);  // number of headers
    os.putString(mode);
    os.putBool(connected);
    os.putBool(changed);
    os.putBool(conflicted);
    os.putInt(0);  // zero-length array of conflict info
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t hdt::yaap_headerStatusClear(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_hdrChange != NULL)
    {
        retval = m_hdrChange->clearChanged();
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

} // namespace classes
} // namespace yaap
