/**************************************************************************/
/*! \file hal/hdt.h HDT sideband signals HAL interface definition.
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

#ifndef HAL_HDT_H
#define HAL_HDT_H

#include <stdint.h>
#include "hal/errs.h"
#include "hal/trigger.h"

using namespace std;

namespace yaap {
namespace hal {

//! Specifies the possible sources of DBREQ assertion.
enum dbreqSource
{
    DBREQ_SRC_NONE = 0,     //!< DBREQ has not been asserted.
    DBREQ_SRC_HOST = 1,     //!< DBREQ was asserted by the host (i.e. HDT software).
    DBREQ_SRC_DBRDY = 2,    //!< DBREQ asserted due to DBRDY assertion.
    DBREQ_SRC_RESET = 3,    //!< DBREQ asserted due to detection of target reset.
    DBREQ_SRC_TRIGIN = 4,   //!< DBREQ asserted due to external trigger.
};

//! Specifies the possible trigger permutations for trigger-caused DBREQ assertion.
enum triggerSource
{
    TRIG_SRC_A = 0,        //!< TrigA
    TRIG_SRC_B = 1,        //!< TrigB
    TRIG_SRC_A_OR_B = 2,   //!< TrigA || TrigB
    TRIG_SRC_A_AND_B = 3,  //!< TrigA && TrigB
    TRIG_SRC_A_XOR_B = 4,  //!< TrigA ^ TrigB
};

/******************************************************************************/
/*! This interface provides access to the HDT sideband signals.
 *
 * The sideband signals include DBREQ_L, DBRDY (per socket), a buffered copy of RESET_L
 * driven from FCH to CPU/APU, and a buffered copy of PWROK driven from FCH to CPU/APU.
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a IHdt_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a IHdt.
 * 
 * \see IHdt_dbrdyMask, IHdt_dbrdySnapshot, IHdt_dbreqOnDbrdy, IHdt_dbreqOnReset, IHdt_dbreqOnTrigger, 
 *      IHdt_dbreqPulseWidthSet, IHdt_dbreqSourceSnapshot
 ******************************************************************************/
class IHdt
{
 public: 

    /*! Get the current state of the DBREQ_L signal.
     *
     * A functional implementation of this method is required.
     * 
     * \param[out] asserted On return, shall indicate whether or not the DBREQ_L signal is asserted.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqSet()
     */
    virtual uint32_t dbreqGet(bool& asserted) = 0;

    /*! Set the current state of the DBREQ_L signal.
     *
     * A functional implementation of this method is required.
     * 
     * \param[in] asserted Indicates whether to assert or de-assert DBREQ_L.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqGet()
     */
    virtual uint32_t dbreqSet(bool asserted) = 0;

    /*! Get the current state of the DBRDY signals.
     *
     * A functional implementation of this method is required.  Returns the current state of the per-socket
     * DBRDY signals.  The DBRDY values may be masked by the hardware  (see IHdt_dbrdyMask::dbrdyMaskSet()).
     * 
     * \param[out] asserted On return, shall indicate DBRDY status: bit 0 indicates if DBRDY0 is asserted, 
     *                      bit 1 indicates DBRDY1, etc.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see IHdt_dbrdyMask::dbrdyMaskSet()
     * \see IHdt_dbrdyMask::dbrdyMaskGet()
     */
    virtual uint32_t dbrdyGet(uint8_t& asserted) = 0;
    
    /*! Retrieves the pulse width for DBREQ pulses.
     *
     * This method returns the pulse width used by the hardware for DBREQ pulses.
     * A functional implementation of this method is required, even if the hardware does
     * not support a programmable DBREQ pulse-width.
     *
     * \param[out] usec On return, shall hold the pulse width in microseconds.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see IHdt_dbreqPulseWidthSet::dbreqPulseWidthSet()
     */
    virtual uint32_t dbreqPulseWidthGet(uint32_t& usec) = 0;
};

/******************************************************************************/
/*! Interface for DBRDY Snapshot hardware.
 * 
 * If the hardware is capable of capturing a snapshot of all DBRDY pin states at the moment
 * that any of them becomes asserted, then this interface should be implemented by the 
 * same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbrdySnapshot
{
  public:
 
    /*! Get a snapshot of the state of the DBRDY signals at the moment the first one became asserted.
     *
     * This method returns a snapshot of all DBRDY signals captured at the moment the first one asserted.
     * Once captured, the snapshot is cleared by a call to dbrdySnapshotClear(). 
     * 
     * \param[out] asserted On return, shall contain DBRDY snapshot: bit 0 contains the DBRDY0 snapshot, 
     *                      bit 1 contains DBRDY1, etc.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbrdySnapshotClear()
     * \see IHdt_dbrdyMask::dbrdyMaskSet()
     * \see IHdt::dbrdyGet()
     */
    virtual uint32_t dbrdySnapshotGet(uint8_t& asserted) = 0;

    /*! Clear the DBRDY state snapshot.
     *
     * This method clears the DBRDY snapshot, and re-arms the hardware to capture the next snapshot.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbrdySnapshotGet()
     */
    virtual uint32_t dbrdySnapshotClear(void) = 0;
};

/******************************************************************************/
/*! Interface for DBREQ source snapshot hardware.
 * 
 * If the hardware is capable of capturing a snapshot of the source of a DBREQ assertion, 
 * then this interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbreqSourceSnapshot
{
  public:
 
    /*! Get the source of the first DBREQ assertion following the last call to dbreqSourceSnapshotClear().
     *
     * This method returns the source of the first DBREQ assertion following the last call
     * to dbreqSourceSnapshotClear().
     * 
     * \param[out] source On return, shall indicate the source of the DBREQ assertion.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqSourceSnapshotClear()
     */
    virtual uint32_t dbreqSourceSnapshotGet(enum dbreqSource& source) = 0;

    /*! Clear the DBREQ source snapshot.
     *
     * This method clears the DBREQ source snapshot, and re-arms the hardware to capture the next snapshot.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqSourceSnapshotGet()
     */
    virtual uint32_t dbreqSourceSnapshotClear(void) = 0;
};

/******************************************************************************/
/*! Interface for RESET_L line sensing hardware.
 * 
 * If the hardware is capable of sensing the state of the RESET_L line, then this
 * interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_resetGet
{
  public:
  
    /*! Get the current state of the RESET_L signal.
     *
     * This method returns the current state of the processor reset line.
     * 
     * \param[out] asserted On return, shall indicate whether RESET_L is currently asserted (low).
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t resetGet(bool& asserted) = 0;
};

/******************************************************************************/
/*! Interface for PWROK line sensing hardware.
 * 
 * If the hardware is capable of sensing the state of the PWROK line, 
 * then this interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_pwrokGet
{
  public:

    /*! Get the current state of the PWROK signal.
     *
     * This method returns the current state of the PWROK signal.
     * 
     * \param[out] asserted On return, shall indicate whether PWROK is currently asserted (high).
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t pwrokGet(bool& asserted) = 0;
};

/******************************************************************************/
/*! Interface for DBRDY/DBREQ reflection (a.k.a. DMOD) hardware.
 * 
 * If the hardware is capable of reflecting a DBRDY assertion on the DBREQ line, 
 * then this interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbreqOnDbrdy
{
  public:

    /*! Configure the hardware to pulse DBREQ whenever any of the DBRDY signals asserts.
     *
     * This method configures the hardware to pulse DBREQ whenever any of the
     * per-socket DBRDY signals asserts.  This behavior has previously been known as "DMOD".  
     * The DBREQ pulse width may be programmed via IHdt_dbreqPulseWidth::dbreqPulseWidthSet().  
     * Hardware should default the pulse width to 8 usec. 
     * 
     * \param[in] enabled Indicates whether DBREQ-on-DBRDY functionality should be enabled.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqOnDbrdyGet()
     * \see IHdt_dbreqPulseWidth::dbreqPulseWidthSet()
     */
    virtual uint32_t dbreqOnDbrdySet(bool enabled) = 0;

    /*! Returns whether the hardware is configured to pulse DBREQ whenever any of the DBRDY signals asserts.
     *
     * This method queries whether the hardware is configured to pulse DBREQ whenever any of the
     * DBRDY signals asserts.  
     * 
     * \param[out] enabled On return, shall indicate whether DBREQ-on-DBRDY functionality is enabled.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqOnDbrdySet()
     */
    virtual uint32_t dbreqOnDbrdyGet(bool& enabled) = 0;
};

/******************************************************************************/
/*! Interface for DBREQ-on-reset hardware.
 * 
 * If the hardware is capable of automatically asserting DBREQ when the processor resets, 
 * then this interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbreqOnReset
{
  public:

    /*! Configure the hardware to pulse DBREQ whenever RESET_L asserts.
     *
     * This method configures the hardware to pulse DBREQ whenever the RESET_L
     * signal asserts. The DBREQ pulse width is programmed via dIHdt_dbreqPulseWidth::breqPulseWidthSet().  
     * Hardware should default the pulse width to 8 usec. 
     * 
     * \param[in] enabled Indicates whether DBREQ-on-RESET_L functionality should be enabled.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqOnResetGet()
     * \see IHdt_dbreqPulseWidthSet::dbreqPulseWidthSet()
     */
    virtual uint32_t dbreqOnResetSet(bool enabled) = 0;

    /*! Returns whether the hardware is configured to pulse DBREQ whenever RESET_L asserts.
     *
     * This method queries whether the hardware is configured to pulse DBREQ 
     * whenever the RESET_L signal asserts. 
     * 
     * \param[out] enabled On return, shall indicate whether DBREQ-on-RESET_L functionality is enabled.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbreqOnResetSet()
     */
    virtual uint32_t dbreqOnResetGet(bool& enabled) = 0;
};

/******************************************************************************/
/*! Interface for DBREQ-on-trigger hardware.
 * 
 * If the hardware is capable of asserting DBREQ due to a trigger-in assertion, 
 * then this interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbreqOnTrigger
{
  public:

    /*! Configure the hardware to assert DBREQ based on one or more triggers.
     *
     * This method configures the hardware to assert DBREQ based on the hardware triggers on the debug module. 
     * 
     * \param[in] enabled Indicates whether DBREQ-on-trigger functionality should be enabled.
     * \param[in] source Indicates the source trigger(s) and permutation for the functionality.
     * \param[in] invert Indicates whether the permutation in \a source should be inverted.
     * \param[in] event Indicates the trigger sensitivity.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_HDT_INVALID_TRIGGER_CFG Invalid or unsupported trigger configuration requested
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see hal::triggers
     * \see dbreqOnTriggerGet()
     */
    virtual uint32_t dbreqOnTriggerSet(bool enabled, enum triggerSource source, bool invert, enum triggerEvent event) = 0;

    /*! Returns the current configuration of DBREQ-on-trigger functionality.
     *
     * This method returns the current configuration relative to DBREQ-on-trigger functionality.
     * 
     * \param[out] enabled On return, shall indicate whether DBREQ-on-trigger functionality is enabled.
     * \param[out] source On return, shall indicate the source trigger(s) and permutation for the functionality.
     * \param[out] invert On return, shall indicate whether the permutation in \a source is inverted.
     * \param[out] event On return, shall indicate the trigger sensitivity.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see hal::triggers
     * \see dbreqOnTriggerSet()
     */
    virtual uint32_t dbreqOnTriggerGet(bool& enabled, enum triggerSource& source, bool& invert, enum triggerEvent& event) = 0;
};

/******************************************************************************/
/*! Interface for setting the DBREQ pulse width.
 * 
 * If the hardware supports a programmable DBREQ pulse width, 
 * then this interface should be implemented by the same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbreqPulseWidthSet
{
  public:

    /*! Configure the pulse width for DBREQ pulses.
     *
     * This method configures the DBREQ pulse width to be used by the hardware.
     * The HAL implementation should calculate and use the closest achievable value to the 
     * requested value in \a usec, and update \a usec with that value upon return.
     * The default pulse width should be 8 usec.
     * 
     * \param[in,out] usec Pulse width in microseconds.  On return, shall contain the actual pulse width set.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see IHdt::dbreqPulseWidthGet()
     */
    virtual uint32_t dbreqPulseWidthSet(uint32_t& usec) = 0;
};

/******************************************************************************/
/*! Interface for DBRDY masking hardware.
 * 
 * If the hardware supports a programmable DBRDY mask, 
 * then this interface should be implemented by same class implementing IHdt.
 ******************************************************************************/
class IHdt_dbrdyMask
{
  public:

    /*! Configure the DBRDY mask used by the hardware.
     *
     * This method allows individual DBRDY signals to be masked in the 
     * hadware.  This affects the values returned by IHdt::dbrdyGet() and IHdt_dbrdySnapshot::dbrdySnapshotGet(), 
     * as well as the IHdt_dbreqOnDbrdy functionality.
     * 
     * \param[in] enabled Each bit of this integer corresponds to a DBRDY signal (bit 0 for socket 0, etc.).
     *            1 means that the signal is enabled, 0 means that it is masked.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbrdyMaskGet()
     * \see IHdt::dbrdyGet()
     * \see IHdt_dbrdySnapshot::dbrdySnapshotGet()
     * \see IHdt_dbreqOnDbrdy::dbreqOnDbrdySet()
     */
    virtual uint32_t dbrdyMaskSet(uint8_t enabled) = 0;

    /*! Retrieve the DBRDY mask used by the hardware.
     *
     * This method returns the DBRDY mask set by dbrdyMaskSet()
     * 
     * \param[out] enabled On return, shall contain a bitfield of the per-socket enable bits (bit 0 for socket 0, etc.).
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see dbrdyMaskSet().
     */
    virtual uint32_t dbrdyMaskGet(uint8_t& enabled) = 0;
};
    
} // namespace hal
} // namespace yaap

#endif // HAL_HDT_H
