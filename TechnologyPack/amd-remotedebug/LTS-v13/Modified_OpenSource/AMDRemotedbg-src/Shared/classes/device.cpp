/**************************************************************************/
/*! \file device.cpp YAAP \a device class implementation.
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

#include "classes/device.h"

#define YAAP_DEVICE_VERSION yaapVersion(1, 0, 9)

namespace yaap {
namespace classes {

/******************************************************************************/

device::device(const char *name, hal::ISystem *sysHw)
    : base(name, "device", YAAP_DEVICE_VERSION), m_sysHw(sysHw)
{
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, device, yaap_info,        "info",        "struct { char [] deviceName; struct { int major; int minor; int point; } version; struct { char [] type; struct { int major; int minor; int point; } version; } [] firmwareInfo; struct { char [] partNumber; char [] revision; char [] serialNumber; } [] boardInfo; } device.info(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, device, yaap_rootMembers, "rootMembers", "struct { char [] name; int id; } [] device.rootMembers(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, device, yaap_lock,        "lock",        "int device.lock(char [] username, char [] password)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED,     device, yaap_unlock,      "unlock",      "void device.unlock(char [] username, char [] password)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, device, yaap_getKey,      "getKey",      "char [] device.getKey()"));
    
    m_firmwareHw = dynamic_cast<hal::ISystem_firmware *>(sysHw);
    m_boardHw = dynamic_cast<hal::ISystem_boardInfo *>(sysHw);
    
    initialize();
    m_baseSysHw = m_sysHw;
}

/******************************************************************************/

uint32_t device::yaap_info(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    const char *name = "";
    vector<hal::fwInfo_t> firmwares;
    vector<hal::boardInfo_t> boards;
    
    retval = m_sysHw->getDeviceName(name);
    if (retval == E_SUCCESS)
    {
        if (m_firmwareHw != NULL)
        {
            retval = m_firmwareHw->getFirmwareInfo(firmwares);
        }
        else
        {
            firmwares.clear();
            retval = E_SUCCESS;
        }
    }
    if (retval == E_SUCCESS)
    {
        if (m_boardHw != NULL)
        {
            retval = m_boardHw->getBoardInfo(boards);
        }
        else
        {
            boards.clear();
            retval = E_SUCCESS;
        }
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putString(name);
    os.putInt(YAAP_VERSION_MAJOR);
    os.putInt(YAAP_VERSION_MINOR);
    os.putInt(YAAP_VERSION_POINT);

    os.putInt(firmwares.size());
    for (vector<hal::fwInfo_t>::const_iterator i = firmwares.begin(); i != firmwares.end(); i++)
    {
        os.putString((*i).type);
        os.putInt((*i).majorVer);
        os.putInt((*i).minorVer);
        os.putInt((*i).pointVer);
    }
    
    os.putInt(boards.size());
    for (vector<hal::boardInfo_t>::const_iterator i = boards.begin(); i != boards.end(); i++)
    {
        os.putString((*i).name);
        os.putString((*i).partNo);
        os.putString((*i).revision);
        os.putString((*i).serialNo);
    }
    os.endBlockSize();
    
    return retval;            
}

/******************************************************************************/

uint32_t device::yaap_rootMembers(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;
    vector<base *> rootmem = getRootMembers();
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(rootmem.size());  // Array size
    for (vector<base*>::const_iterator i = rootmem.begin(); i != rootmem.end(); i++)
    {
        os.putString((*i)->getName());
        os.putInt((*i)->getId());
    }
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t device::yaap_lock(YAAP_METHOD_PARAMS)
{
    size_t usernameSize = is.getArrayCount();
    string username((const char *)is.getRawBytePtr(usernameSize), usernameSize);
    size_t passwordSize = is.getArrayCount();
    string password((const char *)is.getRawBytePtr(passwordSize), passwordSize);
    uint32_t retval = checkInputStream(is);
    uint32_t lockId = 0;
    
    if (retval == E_SUCCESS)
    {
        retval = lockDevice(username.c_str(), password.c_str(), lockId);
    }
    
    if (retval == E_BAD_CREDENTIALS)
    {
        es.add(E_DEVICE_LOCKED, m_sid); // Client expects this error at the YAAP level (as opposed to only at the method level).
    }
    
    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(lockId);
    os.endBlockSize();

    return retval;
}

/******************************************************************************/

uint32_t device::yaap_unlock(YAAP_METHOD_PARAMS)
{
    size_t usernameSize = is.getArrayCount();
    string username((const char *)is.getRawBytePtr(usernameSize), usernameSize);
    size_t passwordSize = is.getArrayCount();
    string password((const char *)is.getRawBytePtr(passwordSize), passwordSize);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        retval = unlockDevice(username.c_str(), password.c_str());
    }
    
    if (retval == E_BAD_CREDENTIALS)
    {
        es.add(E_DEVICE_LOCKED, m_sid); // Client expects this error at the YAAP level (as opposed to only at the method level).
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();
    
    return retval;
}

/******************************************************************************/

uint32_t device::yaap_getKey(YAAP_METHOD_PARAMS)
{
    const uint8_t *key = NULL;
    uint32_t retval = m_sysHw->getYaapKey(key);
    
    os.beginBlockSize();
    os.putInt(retval);
    if (retval == E_SUCCESS)
    {
        os.putByteArray(16, key);
    }
    else
    {
        os.putInt(0); // Array size
    }
    os.endBlockSize();
    
    return retval;
}

} // namespace classes
} // namespace yaap
