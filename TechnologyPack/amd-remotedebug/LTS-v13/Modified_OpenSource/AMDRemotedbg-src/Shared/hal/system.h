/**************************************************************************/
/*! \file hal/system.h YAAP device system header file.
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

#ifndef HAL_SYSTEM_H
#define HAL_SYSTEM_H

#include <stdint.h>
#include <vector>

#include "hal/errs.h"

using namespace std;

namespace yaap {
namespace hal {

//! Describes a firmware image (i.e. "os", "fpga", etc.).
typedef struct
{
    const char *type;       //!< The name of the image
    int majorVer;           //!< Image major version number
    int minorVer;           //!< Image minor version number
    int pointVer;           //!< Image point version number
} fwInfo_t;

//! Describes a board (i.e. "Wombat", "Numbat", etc.).
typedef struct
{
    const char *name;       //!< Board name
    const char *partNo;     //!< Board part number
    const char *revision;   //!< Board revision
    const char *serialNo;   //!< Board serial number
} boardInfo_t;

//! Indicates a change in YAAP session lock status.
enum lockChangeType
{
    LOCKED,                 //!< Device was locked by a new connection
    UNLOCKED,               //!< Device was actively unlocked
    LOCK_TIMED_OUT,         //!< Lock has timed out
    LOCK_RESTORED           //!< Previously timed-out lock was restored
};

/******************************************************************************/
/*! This interface provides access to the system/device running the YAAP server.
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a ISystem_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a ISystem. 
 ******************************************************************************/
class ISystem
{
 public: 

    /*! Get the YAAP device name (i.e. "Wombat").
     * 
     * \param[out] name On return, shall point to the device name.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t getDeviceName(const char *& name) = 0;
    
    /*! Get the YAAP key for this server.
     * 
     * A functional implementation of this method is required.  It retrieves the 16-byte YAAP server
     * key assigned by AMD.  A valid server key is required for the server to be recognized by 
     * client software.  The key should be stored as an array of 16 bytes; there is no need for a 
     * NULL-terminator.
     * 
     * \param[out] key On return, shall point to the 16-byte YAAP key.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t getYaapKey(const uint8_t *& key) = 0;
    
    /*! Callback executed when the lock status changes.
     * 
     * This allows the low-level code to receive notifications when the device's YAAP session lock status changes.  
     * It may be used, for example, to update a "session active" indicator on the device (such as an LED).
     * 
     * HAL implementations needn't include any implementation of this method, as it has a default
     * implementation which does nothing.
     * 
     * \param[in] changeType The type of change.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_NOT_IMPLEMENTED Functionality not implemented
     */
    virtual uint32_t lockChanged(enum lockChangeType changeType) { return E_NOT_IMPLEMENTED; }
	
	virtual uint32_t configMux(bool enable) = 0;
};

/******************************************************************************/
/*! Interface for working with debug device firmware images.
 * 
 * If the debug device contains updatable firmware(s), then this
 * interface should be implemented by the same class implementing ISystem.
 ******************************************************************************/
class ISystem_firmware
{
  public:

    /*! Get information about the YAAP device's firmware image(s).
     * 
     * \param[in,out] firmwares On return, shall contain information on all firmware images used by the device.
     *                Note that this vector is empty when passed in; the callee must either add elements to it, or change
     *                the reference to a different vector which is populated.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t getFirmwareInfo(vector<fwInfo_t>& firmwares) = 0;

    /*! Initiate a firmware update.
     *
     * This method begins a firmware update process.  Note that the implementation should fork 
     * a new thread for the update; the main thread will be used to query the status of the
     * update process so that progress can be reported to the user.
     * 
     * \param[in] fwType     The firmware type name.
     * \param[in] fwData     Pointer to the firmware image.
     * \param[in] fwDataSize Size of the firmware image in bytes.
     * 
     * \retval #E_SUCCESS Call succeeded; firmware update is in progress
     * \retval #E_SYSTEM_INVALID_FW_TYPE The specified firmware type is invalid
     * \retval #E_SYSTEM_FW_NOT_READY The system is not ready to update the specified firmware
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t updateFirmware(const char *fwType, uint8_t *fwData, size_t fwDataSize) = 0;

    /*! Check the status of a firmware update process.
     * 
     * When a firmware update has been initiated by a call to updateFirmware(), this 
     * method is used to query the current status of the update process.
     * 
     * \param[in]  fwType  The firmware type name for which status should be retrieved.
     * \param[out] status  On return, shall contain the firmware status (-1 = error; 0 = ready; 1 = updating).
     * \param[out] percent On return, shall contain the completion percentage of the current update.
     * 
     * \retval #E_SUCCESS Call succeeded; firmware update is in progress
     * \retval #E_SYSTEM_INVALID_FW_TYPE The specified firmware type is invalid
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t getFirmwareStatus(const char *fwType, int& status, int& percent) = 0;
};

/******************************************************************************/
/*! Interface for retrieving information about debug device hardware.
 * 
 * If the debug device supports querying of its hardware details, then this
 * interface should be implemented by the same class implementing ISystem.
 ******************************************************************************/
class ISystem_boardInfo
{
  public:

    /*! Get information about the YAAP device hardware.
     * 
     * \param[in,out] boards On return, shall contain information on all boards which are part of the device.
     *                Note that this vector is empty when passed in; the callee must add elements to it, or change
     *                the reference to a different vector which is already populated.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t getBoardInfo(vector<boardInfo_t>& boards) = 0; 
};

/******************************************************************************/
/*! Interface for resetting debug device hardware.
 * 
 * If the debug device supports reset via YAAP, then this
 * interface should be implemented by the same class implementing ISystem.
 ******************************************************************************/
class ISystem_reset
{
  public:
  
    /*! Reset the system (device) running the YAAP server.
     *
     * This method should cause the YAAP device to reset.  It is currently used by HDT only 
     * to restart a system after updating firmware.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t reset(void) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_SYSTEM_H
