/**************************************************************************/
/*! \file base.cpp YAAP Base class common implementation.
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

#include <string>
#include "classes/base.h"
#include "infrastructure/debug.h"

#define YAAP_BASE_VERSION yaapVersion(1, 1, 0)

namespace yaap {
namespace classes {

bool base::m_locked = false;
uint32_t base::m_lockId = 0;
vector<base *> base::m_allObjs;
char base::m_sid[CREDENTIALS_LEN];
char base::m_spass[CREDENTIALS_LEN];
struct timeval base::m_lastAccess;
bool base::m_resetRequested = false;
hal::ISystem *base::m_baseSysHw = NULL;

/*************************************************/

base::base(const char *name, const char *type, yaapVersion version) 
    : m_name(name), m_type(type), m_version(version), m_reserved(false), m_reservationName(""),
      m_reservationComment(""), m_parent(NULL), m_numMethods(0)
{
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, base, yaap_type,           "type",           "string base.type(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, base, yaap_version,        "version",        "struct { int major; int minor; int point } version(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, base, yaap_members,        "members",        "struct { string name; int id } [] members(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, base, yaap_methods,        "methods",        "struct { string name; int id } [] methods(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, base, yaap_methodsVerbose, "methodsVerbose", "struct { string name; int id; string signature; } [] methodsVerbose(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_NOT_REQUIRED, base, yaap_available,      "available",      "struct { bool reserved; string name; string comment; } available(void)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED,     base, yaap_reserve,        "reserve",        "void reserve(string name, string comment)"));
    m_methods.push_back(METHOD_DESC(LOCK_REQUIRED,     base, yaap_unreserve,      "unreserve",      "void unreserve(void)"));

    if (m_allObjs.size() == 0)
    {
        // This is the first object registered.  The 'device' class must have ID 0, so we'll 
        // do some checking to ensure that is the case.
        if (string(type) == "device")
        {
            m_id = DEVICE_CLASS_INSTANCE_ID;
            m_allObjs.push_back(this);
        }
        else
        {
            m_id = 1;
            m_allObjs.push_back(NULL);  // This is a placeholder for the device class.
            m_allObjs.push_back(this);
        }
    }
    else
    {
        if (string(type) == "device")
        {
            m_id = DEVICE_CLASS_INSTANCE_ID;
            m_allObjs[0] = this;
        }
        else
        {
            m_id = m_allObjs.size();
            m_allObjs.push_back(this);
        }
    }
    
    DEBUG_PRINT(DEBUG_MOREINFO, "Registering YAAP class of type %s, version %d.%d.%d; name = \"%s\"; ID = %d", type, m_version.major, m_version.minor, m_version.point, name, m_id);
}

/*************************************************/

void base::addMember(base *obj)
{
    m_members.push_back(obj);
    obj->m_parent = this;
}

/*************************************************/

uint32_t base::yaap_type(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putString(m_type);
    os.endBlockSize();

    return retval;
}

/*************************************************/

uint32_t base::yaap_version(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_version.major);  // Note: this gives the most-derived class version
    os.putInt(m_version.minor);
    os.putInt(m_version.point);
    os.endBlockSize();

    return retval;
}

/*************************************************/

uint32_t base::yaap_members(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_members.size());
    for (vector<base *>::const_iterator i = m_members.begin(); i != m_members.end(); i++)
    {
        os.putString((*i)->m_name);
        os.putInt((*i)->m_id);
    }
    os.endBlockSize();

    return retval;
}

/*************************************************/

uint32_t base::yaap_methods(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_numMethods);
    for (unsigned int i = 0; i < m_numMethods; i++)
    {
        os.putString(m_methods[i]->name);
        os.putInt(i);
    }
    os.endBlockSize();
    
    return retval;
}

/*************************************************/

uint32_t base::yaap_methodsVerbose(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putInt(m_numMethods);
    for (unsigned int i = 0; i < m_numMethods; i++)
    {
        os.putString(m_methods[i]->name);
        os.putInt(i);
        os.putString(m_methods[i]->signature);
    }
    os.endBlockSize();
    
    return retval;
}

/*************************************************/

uint32_t base::yaap_available(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    os.beginBlockSize();
    os.putInt(retval);
    os.putByte(m_reserved ? 1 : 0);
    os.putString(m_reservationName.c_str());
    os.putString(m_reservationComment.c_str());
    os.endBlockSize();

    return retval;
}

/*************************************************/

uint32_t base::yaap_reserve(YAAP_METHOD_PARAMS)
{
    size_t arraySize = is.getArrayCount();
    string name((const char *)is.getRawBytePtr(arraySize), arraySize);
    arraySize = is.getArrayCount();
    string comment((const char *)is.getRawBytePtr(arraySize), arraySize);
    uint32_t retval = checkInputStream(is);
    
    if (retval == E_SUCCESS)
    {
        if (m_reserved)
        {
            retval = E_ALREADY_RESERVED;
        }
        else
        {
            m_reserved = true;
            m_reservationName = name;
            m_reservationComment = comment;
        }
    }

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;
}

/*************************************************/

uint32_t base::yaap_unreserve(YAAP_METHOD_PARAMS)
{
    uint32_t retval = E_SUCCESS;

    if (!m_reserved)
    {
        retval = E_NOT_RESERVED;
    }

    m_reserved = false;
    m_reservationName = "";
    m_reservationComment = "";

    os.beginBlockSize();
    os.putInt(retval);
    os.endBlockSize();

    return retval;            
}

/******************************************************************************/

uint32_t base::lockDevice(const char *sid, const char *spass, uint32_t& lockId)
{
    struct timeval now;
    if (gettimeofday(&now, NULL) == -1)
    {
        perror("gettimeofday()");
    }

    if (m_locked)
    {
        if ((!strncasecmp(sid, m_sid, CREDENTIALS_LEN)) &&
	    		(!strncmp(spass, m_spass, CREDENTIALS_LEN)))
	    {
	    	// Already locked by the same user.  Return the same lock ID.
	    	DEBUG_PRINT(DEBUG_INFO, "Detected new concurrent connection for session ID \"%s\", lockId=%08X", sid, m_lockId);
	    	lockId = m_lockId;
	    	m_locked = true;
		    m_lastAccess = now;
		    if (m_baseSysHw != NULL)
		    {
			    m_baseSysHw->configMux(1);
		    }
		 
		    return E_SUCCESS;
	    }
	    if (m_locked)
	    {
		    // Already locked by another user.
		    DEBUG_PRINT(DEBUG_INFO, "Lock failed for session ID \"%s\"; already locked by session ID \"%s\"", sid, m_sid);
		    return E_BAD_CREDENTIALS;
	    }
    }// if (m_locked)
 
    strncpy(m_sid, sid, CREDENTIALS_LEN);
    strncpy(m_spass, spass, CREDENTIALS_LEN);
    m_locked = true;
#ifdef FORCE_LOCK_ID
    m_lockId = FORCE_LOCK_ID;
#else
    do
    {
        m_lockId = (uint32_t)rand();
    } while (m_lockId == 0);
#endif
    lockId = m_lockId;
    m_lastAccess = now;
    DEBUG_PRINT(DEBUG_INFO, "\nLock acquired by session ID \"%s\", lockId=%08X", m_sid, m_lockId);
    
    if (m_baseSysHw != NULL)
    {
        m_baseSysHw->lockChanged(hal::LOCKED);
        m_baseSysHw->configMux(1);
    }
    
    return E_SUCCESS;
}

/******************************************************************************/

uint32_t base::unlockDevice(const char *sid, const char *spass)
{
    if (m_locked)
    {
        if ((!strncasecmp(sid, m_sid, CREDENTIALS_LEN)) &&
            (!strncmp(spass, m_spass, CREDENTIALS_LEN)))
        {
            DEBUG_PRINT(DEBUG_INFO, "Lock released by session ID \"%s\"", sid);
            m_locked = false;
            
            if (m_baseSysHw != NULL)
            {
                m_baseSysHw->lockChanged(hal::UNLOCKED);
                m_baseSysHw->configMux(0);
            }
            
            return E_SUCCESS;
        }
        else
        {
            DEBUG_PRINT(DEBUG_INFO, "Wrong credentials to unlock; session ID %s, pass %s, but locked by %s", sid, spass, m_sid);
            return E_BAD_CREDENTIALS;
        }
    }
    return E_SUCCESS; // Was already unlocked
}

/******************************************************************************/
        
vector<base *> base::getRootMembers(void)
{
    vector<base *> retval;
    for (vector<base*>::const_iterator i = m_allObjs.begin(); i != m_allObjs.end(); i++)
    {
        if ((*i)->getParent() == NULL)
        {
            retval.push_back(*i);
        }
    }
    return retval;
}

/******************************************************************************/

void base::dump(void)
{
    DEBUG_PRINT(DEBUG_VERBOSE, "*** Dumping class data for instance %d ***", m_id);
    DEBUG_PRINT(DEBUG_VERBOSE, "    Type = \"%s\"", m_type);
    DEBUG_PRINT(DEBUG_VERBOSE, "    Name = \"%s\"", m_name);
    DEBUG_PRINT(DEBUG_VERBOSE, "    ID   = %d", m_id);
    DEBUG_PRINT(DEBUG_VERBOSE, "    Ver  = %d.%d.%d", m_version.major, m_version.minor, m_version.point);
    DEBUG_PRINT(DEBUG_VERBOSE, "    Members: %d", (int)m_members.size());
    for (unsigned int i = 0; i < m_members.size(); i++)
    {
        DEBUG_PRINT(DEBUG_VERBOSE, "      %d: \"%s\"", i, m_members[i]->m_name);
    }
    DEBUG_PRINT(DEBUG_VERBOSE, "    Methods: %d", (int)m_methods.size());
    for (unsigned int i = 0; i < m_methods.size(); i++)
    {
        DEBUG_PRINT(DEBUG_VERBOSE, "      %d: \"%s\"  [ %s ]", i, m_methods[i]->name, m_methods[i]->signature);
    }
    DEBUG_PRINT(DEBUG_VERBOSE, "*** END dumping class data for instance %d ***", m_id);
}        

/******************************************************************************/

yaap::hal::ISystem *base::getSystemHal(void)
{
    return m_baseSysHw;
}

/******************************************************************************/

} // namespace classes
} // namespace yaap
