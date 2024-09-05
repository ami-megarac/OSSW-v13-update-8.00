/******************************************************************************/
/*! \file base.h YAAP Base class common header.
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

#ifndef YAAP_BASE_CLASS_H
#define YAAP_BASE_CLASS_H

#include <inttypes.h>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>

#include "hal/errs.h"
#include "hal/system.h"
#include "yaarp/streams.h"
#include "infrastructure/debug.h"

#define YAAP_VERSION_MAJOR 1
#define YAAP_VERSION_MINOR 0 
#define YAAP_VERSION_POINT 0

//! Common parameters for all YAAP methods.
#define YAAP_METHOD_PARAMS InputStream& is, OutputStream& os, ErrorStatusStream& ss, ErrorStatusStream& es

//! Defines a new method descriptor for method named \a NAME on YAAP class \a TYPE, implemented in \a METHOD and with method signature \a SIGNATURE.  Requires YAAP lock if \a LOCK is true.
#define METHOD_DESC(LOCK, TYPE, METHOD, NAME, SIGNATURE) new methodDesc<TYPE>(this, &TYPE::METHOD, NAME, SIGNATURE, LOCK)

//! \a base class constructor argument (for clarity)
#define LOCK_REQUIRED true

//! \a base class constructor argument (for clarity)
#define LOCK_NOT_REQUIRED false

//! Maximum length of the username and password credentials
#define CREDENTIALS_LEN 32

//! YAAP connection timeout in minutes
#define YAAP_TIMEOUT_MIN 30

//! YAAP connection timeout in seconds
#define YAAP_TIMEOUT_SEC (YAAP_TIMEOUT_MIN * 60)

//! For dumping HAL support to debug console
#define DEBUG_PRINT_SUPPORTED_FUNC(HAL_DESC, HAL_INSTANCE, HAL_CLASS) DEBUG_PRINT(DEBUG_VERBOSE, " --> " HAL_DESC " HAL implements " #HAL_CLASS ": %c", (dynamic_cast<HAL_CLASS *>(HAL_INSTANCE) != NULL) ? 'Y' : 'N')

//! The Device class must have a fixed instance ID of 0
#define DEVICE_CLASS_INSTANCE_ID 0

using namespace std;
using namespace YAARP;

namespace yaap {
namespace classes {

/******************************************************************************/
/*! Method descriptor abstract base class.
 *
 * The YAAP base class invokes YAAP methods using this abstract base class.
 ******************************************************************************/
class methodDescBase
{
  public:
  
    /*! Constructor.
     * 
     * \param[in] n Name of the method.
     * \param[in] s Signature of the method.
     * \param[in] l Whether the method requires a YAAP lock.
     */
    methodDescBase(const char *n, const char *s, bool l) 
        : name(n), signature(s), requiresLock(l) { }

    /*! Function operator.
     * 
     * Invokes the method.
     * 
     * \return Method return value.
     */
    virtual uint32_t operator()(YAAP_METHOD_PARAMS) = 0;

    const char * const name;        //!< Method name.
    const char * const signature;   //!< Method signature.
    bool requiresLock;              //!< Whether the method requires a YAAP lock.
};

/******************************************************************************/
/*! Method descriptor class.
 *
 * This is a type-specific YAAP method descriptor.  It is capable of invoking
 * a method on a specific YAAP object of the templated class type.
 * 
 * \tparam T YAAP class type.
 ******************************************************************************/
template <class T>
class methodDesc : public methodDescBase
{
  public:
  
    /*! Constructor.
     * 
     * \param[in] o The YAAP object to which this descriptor belongs.
     * \param[in] m Pointer to the method to invoke.
     * \param[in] n Name of the method.
     * \param[in] s Signature of the method.
     * \param[in] l Whether the method requires a YAAP lock.
     */
    methodDesc(T* o, uint32_t (T::*m)(YAAP_METHOD_PARAMS), const char *n, const char *s, bool l) 
        : methodDescBase(n, s, l), obj(o), method(m) { }
    
    /*! Function operator.
     * 
     * Invokes the method.
     * 
     * \return Method return value.
     */
    virtual uint32_t operator()(YAAP_METHOD_PARAMS)
    {
        return (obj->*method)(is, os, ss, es);
    }
    
  private:
  
    T *obj;                                     //!< Object on which to invoke the method.
    uint32_t (T::*method)(YAAP_METHOD_PARAMS);  //!< Pointer to the method.
};

/******************************************************************************/
/*! Represents a YAAP class version.
 ******************************************************************************/
class yaapVersion
{
  public:

    /*! Constructor.
     * 
     * \param[in] maj Major version number.
     * \param[in] min Minor version number.
     * \param[in] pnt Point version number.
     */
    yaapVersion(int maj, int min, int pnt)
    {
        major = maj;
        minor = min;
        point = pnt;
    }

    uint32_t major;  //!< Major number
    uint32_t minor;  //!< Minor number
    uint32_t point;  //!< Point number
};

/******************************************************************************/
/*! This is the bitfield returned by \a checkLock().
 ******************************************************************************/
typedef union
{
    //! Use this to access bitfields
    struct 
    {
        unsigned int ok_to_exe        : 1; //!< Set if the method may be executed
        unsigned int is_locked        : 1; //!< Set when the device is locked
        unsigned int lock_id_matches  : 1; //!< If locked, set when the provided lockID matches
        unsigned int lock_required    : 1; //!< Set if the requested method requires the lock
        unsigned int lock_restored    : 1; //!< Set if the lock had timed out, but now restored
        unsigned int lock_timed_out   : 1; //!< Set if the lock has timed out
        unsigned int bad_id           : 1; //!< Set if the instance or method ID is bad
    };

    unsigned int raw;                      //!< Use this to access all members
} lockIdCheckRetval;

/******************************************************************************/
/*! The YAAP \a base class.
 * 
 * All YAAP classes derive from this class.
 ******************************************************************************/
class base
{
  public:

    /*! Constructor.
     * 
     * \param[in] name    The name of the object.
     * \param[in] type    The YAAP type name of the object.
     * \param[in] version The YAAP version of the object.
     */
    base(const char *name, const char *type, yaapVersion version);

    /*! Check the lock ID prior to invoking a method.
     * 
     * \param[in] lockId   The lock value supplied by the user
     * \param[in] methodId The method ID to check
     * 
     * \return Bitfield containing the result of the check
     */
    lockIdCheckRetval checkLock(uint32_t lockId, uint32_t methodId);
    
    /*! Get the username of the current user (or NULL, if currently unlocked).
     * 
     * \return Username of the current owner of the lock
     */
    static const char *getLocker(void);
    
    /*! Gets a flag indicating whether a method has requested that the device be reset.
     * 
     * \return TRUE means the device should be reset
     */
    static bool resetRequested(void);

    /*! Invoke a method on this object.
     * 
     * \param methodId ID of the method to invoke.
     *
     * \return Method return code.
     */
    uint32_t invoke(uint32_t methodId, YAAP_METHOD_PARAMS);

    /*! Get this instance's name.
     * 
     * \return Instance's name (string).
     */
    const char *getName(void) const;
    
    /*! Get this instance's ID.
     * 
     * \return Instance's ID.
     */
    uint32_t getId(void) const;

    /*! Get this instance's YAAP type.
     * 
     * \return Instance's YAAP type (string).
     */
    const char *getType(void) const;
    
    /*! Get this instance's parent object.
     * 
     * \return Instance's parent, or NULL for a root member.
     */
    base *getParent(void) const;

    /*! Get the YAAP class version implemented by this instance.
     * 
     * \returns Reference to the YAAP version struct.
     */
    const yaapVersion& getVersion(void) const;

    /*! Get the name of a method, given its ID.
     * 
     * \param[in] methodId The instance ID.
     * 
     * \returns Method name (string), or NULL on error.
     */
    const char *getMethodName(uint32_t methodId) const;

    /*! Check whether the specified method ID is valid.
     * 
     * \param[in] methodId The method ID.
     *
     * \retval TRUE The method ID is valid.
     * \retval FALSE The method ID is invalid.
     */
    bool isValidMethod(uint32_t methodId) const;
    
    /*! Dump internal information about this class.
     * 
     * Uses the standard debug stream to dump out information about the 
     * members and methods of this instance.
     */
    void dump(void); 

    /*! Check whether the specified instance and method ID's are valid.
     * 
     * \param[in] instanceId The instance ID.
     * \param[in] methodId   The method ID.
     *
     * \retval TRUE The method is valid.
     * \retval FALSE the instance or method ID is invalid.
     */
    static bool isValidMethod(uint32_t instanceId, uint32_t methodId);

    /*! Check whether the specified instance is valid.
     * 
     * \param[in] instanceId The instance ID.
     *
     * \retval TRUE The instance is valid.
     * \retval FALSE the instance is invalid.
     */
    static bool isValidInstance(uint32_t instanceId);

    /*! Get a pointer to the specified instance.
     * 
     * \param[in] instanceId The instance ID.
     *
     * \returns Pointer to the instance, or NULL on error.
     */
    static base *getInstance(uint32_t instanceId);
    
    /*! Get a vector of all of the root members
     * 
     * \returns A vector of pointers to the root members.
     */
    static vector<base *> getRootMembers(void);
    
    /*! Get a pointer to the System HAL implementation.
     * 
     * \returns A pointer to the System HAL implementation.
     */
    static yaap::hal::ISystem *getSystemHal(void);
    
  protected:

    //! YAAP Method: Get the name of the type of this instance.
    uint32_t yaap_type(YAAP_METHOD_PARAMS);

    //! YAAP Method: Query the version of this instance.
    uint32_t yaap_version(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the members belonging to this instance.
    uint32_t yaap_members(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the methods belonging to this instance.
    uint32_t yaap_methods(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the methods belonging to this instance (verbose).
    uint32_t yaap_methodsVerbose(YAAP_METHOD_PARAMS);

    //! YAAP Method: Check whether this instance is available for reservation.
    uint32_t yaap_available(YAAP_METHOD_PARAMS);

    //! YAAP Method: Reserve this instance.
    uint32_t yaap_reserve(YAAP_METHOD_PARAMS);

    //! YAAP Method: Un-reserve this instance.
    uint32_t yaap_unreserve(YAAP_METHOD_PARAMS);

    /*! Initialize this object.
     * 
     * This method should be called at the end of the constructor in all derived classes.
     */
    void initialize();

    /*! Add a member YAAP object to this instance.
     *
     * This method is used by derived classes to add members to a YAAP class.  This
     * call will add \a obj to the calling instance's member table, and will set the
     * \a parent member in \a obj to point to the calling instance.
     * 
     * \param[in] obj Pointer to the object to add.
     *
     * \retval #E_SUCCESS Call succeeded
     */
    void addMember(base *obj);
    
    /*! Lock the device.
     * 
     * \param[in] sid     Credentials: session identifier.
     * \param[in] spass   Credentials: session password.
     * \param[out] lockId On return, holds the lock ID.
     * 
     * \retval #E_SUCCESS Device successfully locked
     * \retval #E_BAD_CREDENTIALS Device is already locked to other credentials
     */
    uint32_t lockDevice(const char *sid, const char *spass, uint32_t& lockId);
    
    /*! Unlock the device.
     * 
     * \param[in] sid   Credentials: session identifier.
     * \param[in] spass Credentials: session password.
     * 
     * \retval #E_SUCCESS Device successfully locked
     * \retval #E_BAD_CREDENTIALS Device is locked to other credentials
     */
    uint32_t unlockDevice(const char *sid, const char *spass);
    
    /*! Check the input stream for errors.
     * 
     * \param[in] is Input stream
     * 
     * \retval #E_SUCCESS The input stream is error-free
     * \retval #E_MALFORMED_REQUEST The input stream has errors
     */
    uint32_t checkInputStream(InputStream& is);
    
    uint32_t m_id;                          //!< ID of this instance (assigned at registration)
    const char *m_name;                     //!< Name of this instance.
    const char *m_type;                     //!< Type of this instance.
    yaapVersion m_version;                  //!< The version of this class.
    bool m_reserved;                        //!< Whether this instance is currently reserved.
    string m_reservationName;               //!< Reservation name.
    string m_reservationComment;            //!< Reservation comment.
    vector<base *> m_members;               //!< Members belonging to this instance.
    base *m_parent;                         //!< This instances' parent instance.
    uint32_t m_numMethods;                  //!< The number of methods this class has.
    vector<methodDescBase *> m_methods;     //!< This instance's methods.

    static vector<base *> m_allObjs;        //!< Registry of all YAAP objects.
    static bool m_locked;                   //!< Indicates whether the YAAP device is current locked.
    static char m_sid[CREDENTIALS_LEN];     //!< Session id of the current lock holder.
    static char m_spass[CREDENTIALS_LEN];   //!< Session password of the current lock holder.
    static uint32_t m_lockId;               //!< Current lock ID
    static struct timeval m_lastAccess;     //!< Time of last access (for evaluation of lock timeout).
    static bool m_resetRequested;           //!< Indicates that the server is going down.
    static hal::ISystem *m_baseSysHw;       //!< The ISystem HAL implementation.
};

/******************************************************************************/

inline lockIdCheckRetval base::checkLock(uint32_t lockId, uint32_t methodId)
{
    lockIdCheckRetval retval;
    retval.raw = 0;
    
    if (!isValidMethod(methodId))
    {
        retval.bad_id = 1;
    }
    else
    {
        retval.lock_required = m_methods[methodId]->requiresLock ? 1 : 0;
        
        // Check for timeout
        struct timeval now;
        if (gettimeofday(&now, NULL) == -1)
        {
            perror("gettimeofday()");
        }

        if (m_locked && ((now.tv_sec - m_lastAccess.tv_sec) > YAAP_TIMEOUT_SEC))
        {
            if (lockId == m_lockId)
            {
                DEBUG_PRINT(DEBUG_INFO, "Restoring timed-out lock, lockId=%08X, for session ID \"%s\"", m_lockId, m_sid);
                m_lastAccess = now;
                retval.lock_restored = 1;
                if (m_baseSysHw != NULL)
                {
                    m_baseSysHw->lockChanged(hal::LOCK_RESTORED);
                    m_baseSysHw->configMux(1);
                }
            }
            else
            {
                DEBUG_PRINT(DEBUG_INFO, "Lock, lockId=%08X, timed out for session ID \"%s\"", m_lockId, m_sid);
                m_locked = false;
                retval.lock_timed_out = 1;
            }
        }
        
        if (m_locked && (lockId == m_lockId))
        {
            // Another call from the current lock holder; reset the timeout counter.
            m_lastAccess = now;
            retval.lock_id_matches = 1;
            retval.ok_to_exe = 1;
        }
        else if (!m_locked && (m_lockId != 0) && (lockId == m_lockId))
		{
			// If unlocked, but lockId is non-zero and matches the previously used lockId, relock
			DEBUG_PRINT(DEBUG_INFO, "Restoring last used lock ID, lockId=%08X, for session ID \"%s\"", m_lockId, m_sid);
			m_locked = true;
			m_lastAccess = now;
			retval.lock_id_matches = 1;
			retval.ok_to_exe = 1;
			if (m_baseSysHw != NULL)
				m_baseSysHw->configMux(1);
        }
        else if (!retval.lock_required)
        {
            retval.ok_to_exe = 1;
        }

        retval.is_locked = (m_locked ? 1 : 0);
    }
    return retval;
}

/******************************************************************************/

inline const char *base::getLocker(void)
{
    return (m_locked ? m_sid : NULL);
}

/******************************************************************************/
inline uint32_t base::invoke(uint32_t methodId, YAAP_METHOD_PARAMS)
{
    if (methodId >= m_numMethods)
    {
        return E_INVALID_METHOD_ID;
    }
    return (*m_methods[methodId])(is, os, ss, es);
}

/******************************************************************************/

inline const char *base::getName(void) const
{
    return m_name;
}

/******************************************************************************/

inline uint32_t base::getId(void) const
{
    return m_id;
}

/******************************************************************************/

inline const char *base::getType(void) const
{
    return m_type;
}

/******************************************************************************/

inline base *base::getParent(void) const
{
    return m_parent;
}

/******************************************************************************/

inline const yaapVersion& base::getVersion(void) const
{
    return m_version;
}

/******************************************************************************/

inline const char *base::getMethodName(uint32_t methodId) const
{
    if (methodId < m_numMethods)
    {
        return m_methods[methodId]->name;
    }
    return NULL;
}

/******************************************************************************/

inline bool base::isValidMethod(uint32_t instanceId, uint32_t methodId)
{
    base *obj = getInstance(instanceId);
    if (obj == NULL)
    {
        return false;
    }
    return (methodId < obj->m_numMethods);
}

/******************************************************************************/

inline bool base::isValidMethod(uint32_t methodId) const
{
    return (methodId < m_numMethods);
}

/******************************************************************************/

inline bool base::isValidInstance(uint32_t instanceId)
{
    return (getInstance(instanceId) != NULL);
}

/******************************************************************************/

inline base *base::getInstance(uint32_t instanceId)
{
    if (instanceId >= m_allObjs.size())
    {
        return NULL;
    }
    return m_allObjs[instanceId];
}

/******************************************************************************/

inline bool base::resetRequested(void)
{
    return m_resetRequested;
}

/******************************************************************************/

inline void base::initialize(void)
{
    m_numMethods = m_methods.size();
    //dump();  // Don't leave this uncommented in production code; it will hurt performance.  -RB
}

/******************************************************************************/

inline uint32_t base::checkInputStream(InputStream& is)
{
    uint32_t retval = E_SUCCESS;
    if (is.isError())
    {
        DEBUG_PRINT(DEBUG_ERROR, "Input stream error");
        retval = E_MALFORMED_REQUEST;
    }
    return retval;
}

/******************************************************************************/

} // namespace classes
} // namespace yaap

#endif // YAAP_BASE_CLASS_H

