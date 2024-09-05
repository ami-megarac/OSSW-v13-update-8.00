/**************************************************************************/
/*! \file system.cpp YAAP \a system class implementation.
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

#include "classes/system.h"
#include <sstream>

#define YAAP_SYSTEM_VERSION yaapVersion(1, 0, 9)

namespace {
/******************************************************************************/
/**
 * Intentionally simple.  "Slow" is OK.
 * Returns char* to first found, or NULL if not found.
 */
const char* findString(const char *fw_contents, size_t fw_size, const std::string &s)
{
    for( uint32_t i = 0; i <= (fw_size - s.size()) ;++i )
    {
        if (::strncmp( (fw_contents + i), s.c_str(), s.size() ) == 0) {
            return fw_contents + i;
        }
    }
    return NULL;
}

/******************************************************************************/
/**
 * Examines firmware image for AT45DB641E flash indicator.
 *
 * Old FW contains a section which reads:
 *   AT45DB011B..AT45DB021B..AT45DB041x..AT45DB081B..AT45DB161x..AT45DB321x..AT45DB642x..
 * New FW includes the AT45DB641E:
 *   AT45DB011B..AT45DB021B..AT45DB041x..AT45DB081B..AT45DB161x..AT45DB321x..AT45DB641E..AT45DB642x..
 */
bool isNewFlashImage( const uint8_t *firmwareData, size_t arrayCnt )
{
    bool b = findString((const char *)(firmwareData), arrayCnt, "AT45DB641E") != NULL;
    return b;
}
}    // namespace

namespace yaap {
namespace classes {

/******************************************************************************/

system::system(const char *name, hal::ISystem *sysHw)
    : base(name, "system", YAAP_SYSTEM_VERSION), m_sysHw(sysHw)
{
    m_boardInfo = dynamic_cast<hal::ISystem_boardInfo *>(sysHw);
    m_firmware = dynamic_cast<hal::ISystem_firmware *>(sysHw);
    m_reset = dynamic_cast<hal::ISystem_reset *>(sysHw);
    
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, system, yaap_deviceReset,    "deviceReset",    "void system.deviceReset()"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, system, yaap_firmwareUpdate, "firmwareUpdate", "void system.firmwareUpdate(char [] firmwareType, uint8_t [] firmwareData)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED, system, yaap_firmwareStatus, "firmwareStatus", "struct { int status; int percent; } system.firmwareStatus(char [] firmwareType)"));
    
    DEBUG_PRINT_SUPPORTED_FUNC("System", sysHw, hal::ISystem_boardInfo);
    DEBUG_PRINT_SUPPORTED_FUNC("System", sysHw, hal::ISystem_firmware);
    DEBUG_PRINT_SUPPORTED_FUNC("System", sysHw, hal::ISystem_reset);

    initialize();
}

/******************************************************************************/

uint32_t system::yaap_deviceReset(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_NOT_IMPLEMENTED;
    if (m_reset != NULL)
    {
        retval = m_reset->reset();
    }
    
    if (retval == E_SUCCESS)
    {
        base::m_resetRequested = true;
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

 /******************************************************************************/
 /*
  * bool isOldWombat()
  *
  * Returns true only if it positively identifies an old Wombat. revision 400 and lower.
  * Given a part number and version of "0100774-301", for example, an old Wombat will
  * have part no. 0100774 and version 301 or lower.  Its revision will be 304 or lower.
  *
  * Not intended to be forward compatible, and gives false on error.
  */
bool system::isOldWombat()
{
    uint32_t retval = E_SUCCESS;
    static const std::string old_partno("0100774");
    static const int old_version = 301;
    static const int old_revision = 304;

    vector<hal::boardInfo_t> boards;
    retval = m_boardInfo->getBoardInfo(boards);

    if (retval == E_SUCCESS)
    {
        for (vector<hal::boardInfo_t>::const_iterator bi = boards.begin();
            bi != boards.end() ;++bi)
        {
            // from: wombat/lowlevel/src/WombatSystem.cpp:
            if (std::string("wombat") == bi->name)
            {
                if (std::string(bi->partNo).substr(0,old_partno.length()) == old_partno)
                {
                    std::istringstream iver(std::string(bi->partNo).substr(8,3));
                    int ver;
                    iver >> ver;
                    if (!iver.fail() && ver <= old_version)
                    { 
                        std::istringstream irev(std::string(bi->revision));
                        int rev;
                        irev >> rev;

                        // ID only if conversion worked and rev is non-negative.
                        if (!irev.fail() && rev >= 0 && rev <= 304)
                            return true;
                        break;        // Otherwise, take the stairs.
                    }
                }
            }
        }
    }

    return false;
}
 /******************************************************************************/

uint32_t system::yaap_firmwareUpdate(YAAP_METHOD_PARAMS)
{
    size_t arrayCnt = is.getArrayCount();
    string firmwareTypeStr((const char *)is.getRawBytePtr(arrayCnt), arrayCnt);
    arrayCnt = is.getArrayCount();
    uint8_t *firmwareData = is.getRawBytePtr(arrayCnt);
    uint32_t retval = checkInputStream(is);
    
    if ((retval == E_SUCCESS) && (m_firmware == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        // An old flash image will corrupt a new Wombat when it tries to update.
        if ("os" == firmwareTypeStr && !isOldWombat() && !isNewFlashImage(firmwareData, arrayCnt))
        {
            retval = E_SYSTEM_INVALID_FW_TYPE;
        }
        else
        {
            retval = m_firmware->updateFirmware(firmwareTypeStr.c_str(), firmwareData, arrayCnt);
        }
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t system::yaap_firmwareStatus(YAAP_METHOD_PARAMS)
{
    size_t arrayCnt = is.getArrayCount();
    string firmwareTypeStr((const char *)is.getRawBytePtr(arrayCnt), arrayCnt);
    uint32_t retval = checkInputStream(is);
    
    int status = 0;
    int percent = 0;
    if ((retval == E_SUCCESS) && (m_firmware == NULL))
    {
        retval = E_NOT_IMPLEMENTED;
    }
    if (retval == E_SUCCESS)
    {
        retval = m_firmware->getFirmwareStatus(firmwareTypeStr.c_str(), status, percent);
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(status);
    os.putInt(percent);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

} // namespace classes
} // namespace yaap
