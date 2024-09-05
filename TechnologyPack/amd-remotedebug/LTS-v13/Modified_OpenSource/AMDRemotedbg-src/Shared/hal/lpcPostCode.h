/*************************************************************************/
/*! \file hal/lpcPostCode.h LPC Post Code interface HAL header file.
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
**************************************************************************/

#ifndef HAL_LPC_POST_CODE_H
#define HAL_LPC_POST_CODE_H

#include <stdint.h>
#include <vector>
#include "hal/errs.h"

using namespace std;

namespace yaap {
namespace hal {

//! This struct represents one entry in the POST code FIFO.
struct postCodeFifoEntry
{
    uint64_t timestamp; //!< Timestamp of this entry relative to the previous one
    uint8_t data;       //!< The data byte written
    uint8_t offset;     //!< Offset of the data byte (0-3)
    bool statusFlag;    //!< True indicates that byte and offset are repurposed as status bits.
};

/*******************************************************************************/
/*! This interface provides access to an LPC Post Code reader.
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a ILpcPostCode_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a ILpcPostCode. 
 *******************************************************************************/
class ILpcPostCode
{
 public:

    /*! Enable or disable the POST code reader.
     * 
     * \param[in] enabled TRUE to enable the reader; FALSE to disable it.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see enableGet()
     */
    virtual uint32_t enableSet(bool enabled) = 0;

    /*! Check whether POST code reader is currently enabled.
    * 
    * \param[out] enabled TRUE indicates that the reader is enabled, FALSE that it is disabled.
    * 
    * \retval #E_SUCCESS Call succeeded
    * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
    * 
    * \see enableSet()
    */
    virtual uint32_t enableGet(bool& enabled) = 0;

    /*! Return the current POST code.
     * 
     * \param[out] postCode Callee updates with the current POST code.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t postCodeGet(uint32_t& postCode) = 0;

    /*! Set the I/O port from which POST codes are to be read.
     * 
     * If an implementation does not support changing the port, it may return #E_NOT_IMPLEMENTED
     * for this method.
     *
     * \param[in] port Port number (typically 0x80, 0x84, 0xE0, or 0x160).
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_INVALID_PORT Specified port number is invalid or unsupported
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * \retval #E_NOT_IMPLEMENTED Method is not implemented
     * 
     * \see portGet()
     */
    virtual uint32_t portSet(int port) = 0;

    /*! Get the I/O port from which POST codes are to be read.
     * 
     * \param[out] port Port number (typically 0x80, 0x84, 0xE0, or 0x160; should default to 0x80).
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see portSet()
     */
    virtual uint32_t portGet(int& port) = 0;

    /*! Specify passive or active monitoring of POST codes.
     * 
     * With passive monitoring, the POST code reader will not claim the LPC cycle.  With active monitoring,
     * it will claim the LPC cycle.
     * 
     * If an implementation does not support changing this parameter, it may return #E_NOT_IMPLEMENTED
     * for this method.
     * 
     * \param[in] passive TRUE for passive monitoring, FALSE for active
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * \retval #E_NOT_IMPLEMENTED Method is not implemented
     * 
     * \see passiveGet()
     */
    virtual uint32_t passiveSet(bool passive) = 0;

    /*! Get the current passive or active monitoring mode setting.
     * 
     * With passive monitoring, the POST code reader will not claim the LPC cycle.  With active monitoring,
     * it will claim the LPC cycle.
     * 
     * \param[out] passive TRUE indicates passive monitoring, FALSE indicates active monitoring
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * \retval #E_NOT_IMPLEMENTED Method is not implemented
     * 
     * \see passiveSet()
     */
    virtual uint32_t passiveGet(bool& passive) = 0;
    
    /*! Check whether the LPC connection is present.
     * 
     * \param[out] connected TRUE indicates that the LPC connection is sensed, FALSE indicates it is not connected.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t connectedGet(bool& connected) = 0;
};

/******************************************************************************/
/*! Interface for working with LPC POST Code FIFO hardware.
* 
* If the debug device a POST code FIFO, then this
* interface should be implemented by the same class implementing ILpcPostCode.
******************************************************************************/
class ILpcPostCode_fifo
{
 public:

    /*! Retrieve the current FIFO contents.
     * 
     * This method is used to dump the entire contents of the POST code FIFO.
     * 
     * \param[in,out] fifoContents A reference to a vector of FifoEntry objects which the callee should populate.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t fifoDump(vector<hal::postCodeFifoEntry>& fifoContents) = 0;

    /*! Clear the FIFO.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t fifoClear() = 0;

    /*! Read status info for the FIFO.
     * 
     * \param[out] entries Number of valid entries in the FIFO
     * \param[out] overflow Sticky-bit indicating FIFO overflow
     * \param[out] full Indicates whether the FIFO is currently full
     * \param[out] empty Indicates whether the FIFO is currently empty
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see fifoStatusClear
     */
    virtual uint32_t fifoStatusGet(int& entries, bool& overflow, bool& full, bool& empty) = 0;

    /*! Clear the FIFO status flags.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see fifoStatusGet
     */
    virtual uint32_t fifoStatusClear(void) = 0;

    /*! Retrieve the FIFO configuration.
     * 
     * \param[out] entries Size of the FIFO (number of entries)
     * \param[out] circular TRUE indicates circular buffer mode; FALSE indicates one-shot mode
     * \param[out] discard TRUE indicates the identical sequential codes are discarded; FALSE keeps all codes
     * \param[out] autoClear TRUE indicates that LPC power down, disconnect, or clock stop automatically clears the FIFO
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see fifoSetupSet
     */
    virtual uint32_t fifoSetupGet(int& entries, bool& circular, bool& discard, bool& autoClear) = 0;

    /*! Configure the FIFO's wrap behavior.
     * 
     * \param[in] circular TRUE for circular buffer mode; FALSE for one-shot mode
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * \retval #E_NOT_IMPLEMENTED Functionality is not implemented
     * 
     * \see fifoSetupGet
     */
    virtual uint32_t fifoCircularSet(bool circular) = 0;

    /*! Configure the FIFO's behavior with identical sequential codes.
     * 
     * \param[in] discard TRUE indicates that identical sequential POST codes should be discarded; FALSE to keep all codes
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * \retval #E_NOT_IMPLEMENTED Functionality is not implemented
     * 
     * \see fifoSetupGet
     */
    virtual uint32_t fifoDiscardSequentialSet(bool discard) = 0;

    /*! Configure the FIFO's auto-clear behavior.
     * 
     * \param[in] autoClear TRUE indicates that LPC power down, disconnect, or clock stop should automatically clear the FIFO
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * \retval #E_NOT_IMPLEMENTED Functionality is not implemented
     * 
     * \see fifoSetupGet
     */
    virtual uint32_t fifoAutoClearSet(bool autoClear) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_LPC_POST_CODE_H


