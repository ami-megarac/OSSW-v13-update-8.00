/*************************************************************************/
/*! \file hal/jtag.h JTAG interface HAL header file.
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

#ifndef HAL_JTAG_H
#define HAL_JTAG_H

#include <stdint.h>

#include "hal/errs.h"
#include "hal/header.h"

namespace yaap {
namespace hal {

//! Specifies the possible states of the JTAG controller.
enum jtagState
{
    JTAG_STATE_TLR = 0,          //!< Test-Logic-Reset
    JTAG_STATE_RTI = 1,          //!< Run-Test-Idle
    JTAG_STATE_SELECT_DR = 2,    //!< Select Data Register
    JTAG_STATE_CAPTURE_DR = 3,   //!< Capture Data Register
    JTAG_STATE_SHIFT_DR = 4,     //!< Shift Data Register
    JTAG_STATE_EXIT1_DR = 5,     //!< Exit1 Data Register
    JTAG_STATE_PAUSE_DR = 6,     //!< Pause Data Register
    JTAG_STATE_EXIT2_DR = 7,     //!< Exit2 Data Register
    JTAG_STATE_UPDATE_DR = 8,    //!< Update Data Register
    JTAG_STATE_SELECT_IR = 9,    //!< Select Instruction Register
    JTAG_STATE_CAPTURE_IR = 10,  //!< Capture Instruction Register
    JTAG_STATE_SHIFT_IR = 11,    //!< Shift Instruction Register
    JTAG_STATE_EXIT1_IR = 12,    //!< Exit1 Instruction Register
    JTAG_STATE_PAUSE_IR = 13,    //!< Pause Instruction Register
    JTAG_STATE_EXIT2_IR = 14,    //!< Exit2 Instruction Register
    JTAG_STATE_UPDATE_IR = 15,   //!< Update Instruction Register

    JTAG_STATE_LAST = JTAG_STATE_UPDATE_IR,
};

//! Specifies the types of JTAG operations supported.
enum jtagOp
{
    JTAG_OP_SHIFT_IR,            //!< Shift Instruction Register
    JTAG_OP_SHIFT_DR,            //!< Shift Data Register
};

/*******************************************************************************/
/*! This interface provides access to a JTAG controller.  
 *
 * It defines methods to perform JTAG shifts, to enable/disable the JTAG connection,
 * and to report on the status of the JTAG connection.  If an implementation contains
 * more than one JTAG controller, there should be an instance of a class implementing
 * this interface for each controller.  Any "behind the scenes" work needed for a JTAG
 * controller must be implemented in the class(es) implementing this interface.  
 * For example, if more than one "virtual" JTAG controller (each with their own object 
 * implementing this class) share a single hardware TAP controller, the low-level 
 * implementations of the controllers must handle the mux switching (or equivalent)
 * necessary to grant the associated virtual TAP control of the hardware, as well as the
 * signalling between different TAPs to preserve state across switches.
 * 
 * It is vital for JTAG HAL implementations to get the sequencing correct.  Single-bit
 * granularity is required in the pre-TCK, data, and post-TCK phases.  The pre-/post-TCK
 * clocks must occur within the same TAP state machine sequence as the main data transfer;
 * i.e. the pre-TCK clocks occur in RTI before transitioning to Select-DR-Scan, and the
 * post-TCK clocks occur in RTI after exiting Update-DR or Update-IR.  This is depicted in
 * the following diming diagram:
 * 
 * \image html shift_timing.png
 * \image latex shift_timing.eps
 * 
 * Note that this timing diagram shows an even duty cycle clock, which is not necessary in 
 * the implementation.  What is important is the location of the TCK edges with respect
 * to the sequencing of the TAP state machine, which is controlled by TMS.
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a IJtag_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a IJtag.
 * 
 * \see IJtag_nonDestructiveRead, IJtag_tapState, IJtag_tckFrequency
 ********************************************************************************/
class IJtag
{
  public :

    /*! Perform a JTAG shift operation.
     *
     * This method performs a JTAG shift operation, blocking until the operation is complete.
     * A functional implementation of this method is required.  The input and output data buffers 
     * are contiguous, and are each large enough to accomodate the number of bits to be shifted. 
     * The input and output buffers \b do \b not include data shifted during the \a preTck and \a postTck clocks.
     * 
     * Implementations of this method should not check the status of any headers (enabled/connected/changed), as that is
     * done by the calling code.  Implementions do not need to include any repeat/retry logic, as that is implemented
     * in the calling code.  Implementations must handle the appropriate transitions through the 1149.1 states.
     *
     * \param[in] operation Specifies the operation to perform, #JTAG_OP_SHIFT_IR or #JTAG_OP_SHIFT_DR.
     * \param[in] inData    Pointer to data to be shifted in.  If NULL, zeros shall be shifted in.
     *                      Data shall be shifted least-significant bit first, starting with the first
     *                      byte pointed to by \a inData.  The callee must not modify any of the data
     *                      pointed to by \a inData.
     * \param[in] outData   Pointer to buffer where data shifted out will be stored.  If NULL,
     *                      the data shifted out shall be discarded.  The first byte contains
     *                      the first bits shifted out, and within each byte the lsb contains 
     *                      the first bit shifted out.
     * \param[in] bits      Number of bits to shift, not including any \a preTck or \a postTck ticks.
     * \param[in] preTck    TCK shall be toggled this many times before starting to shift data.
     * \param[in] postTck   TCK shall be toggled this many times after finishing the shift.
     * \param[in] termState Terminal TAP state.  Can be #JTAG_STATE_RTI (default), 
     *                      #JTAG_STATE_TLR, #JTAG_STATE_SHIFT_IR, #JTAG_STATE_SHIFT_DR, 
     *                      #JTAG_STATE_PAUSE_DR, or #JTAG_STATE_PAUSE_IR.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_JTAG_FSM_ASYNC_RESET No shift performed; the TAP state machine has been reset
     * \retval #E_JTAG_INVALID_IR_STATE No shift performed; invalid or unsupported terminal TAP state requested
     * \retval #E_JTAG_INVALID_DR_STATE No shift performed; invalid or unsupported terminal TAP state requested
     * \retval #E_UNSPECIFIED_HW_ERROR No shift performed; unspecified low-level error encountered
     */
    virtual uint32_t shift(enum jtagOp operation, uint8_t *inData, uint8_t *outData, int bits, int preTck, int postTck, enum jtagState termState) = 0;

    /*! Reset the TAP state machine in the debug hardware.
     *
     * This method causes the low-level to reset the TAP state machine to the TLR state
     * by holding TMS high and cycling through at least 5 TCK cycles.
     * 
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see tapStateSet()
     */
    virtual uint32_t resetTap(void) = 0;
};

/******************************************************************************/
/*! Interface for nondestructive JTAG read functionality.
 * 
 * If the hardware is capable of performing a nondestructive JTAG read, then this 
 * interface should be implemented by the same class implementing IJtag.
 ******************************************************************************/
class IJtag_nonDestructiveRead
{
  public:

    /*! Perform a non-destructive JTAG read operation.
     *
     * This method performs a JTAG shift operation, blocking until the operation is complete.
     * Zeros are to be shifted in, and the data shifted out is shifted back in without ever leaving the Shift-IR or
     * Shift-DR state.  The data shifted out is then returned in \a outData.  This means that the 
     * system-under-test should be completely undisturbed by this read operation.  If low-level code cannot support 
     * a truly nondestructive read (i.e. perform the two shifts without cycling through the state machine between them),
     * this method should be left unimplemented.
     *
     * Implementations of this method should not check the status of any headers (enabled/connected/changed), as that is
     * done by the calling code.  Implementions do not need to include any repeat/retry logic, as that is implemented
     * in the calling code.
     * 
     * \param[in] operation Specifies the operation to perform, #JTAG_OP_SHIFT_IR or #JTAG_OP_SHIFT_DR.
     * \param[in] outData   Pointer to buffer where data shifted out will be stored.  The first byte contains
     *                      the first bits shifted out, and within each byte the lsb contains 
     *                      the first bit shifted out.
     * \param[in] bits      Number of bits to shift, not including any \a preTck or \a postTck ticks.
     *                      Note that the number of TCK toggles may be as high as 2 x \a bits, due to 
     *                      the need to feed the data back into the DR.
     * \param[in] preTck    TCK shall be toggled this many times before starting to shift data.
     * \param[in] postTck   TCK shall be toggled this many times after finishing the shift.
     * \param[in] termState Terminal TAP state.  Can be #JTAG_STATE_RTI (default), 
     *                      #JTAG_STATE_TLR, #JTAG_STATE_PAUSE_DR, or #JTAG_STATE_PAUSE_IR.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_JTAG_INVALID_IR_STATE No shift performed; invalid or unsupported terminal TAP state requested
     * \retval #E_JTAG_INVALID_DR_STATE No shift performed; invalid or unsupported terminal TAP state requested
     * \retval #E_UNSPECIFIED_HW_ERROR No shift performed; unspecified low-level error encountered
     */
    virtual uint32_t shiftNonDestructive(enum jtagOp operation, uint8_t *outData, int bits, int preTck, int postTck, enum jtagState termState) = 0;
};

/******************************************************************************/
/*! Interface for controlling the TAP state.
 * 
 * If the hardware supports access to or control of the TAP state, then this 
 * interface should be implemented by the same class implementing IJtag.
 ******************************************************************************/
class IJtag_tapState
{
  public:
  
    /*! Get the current TAP controller state.
     *
     * \param[out] state Upon return, shall contain the current state of the controller.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see tapStateSet()
     */
    virtual uint32_t tapStateGet(enum jtagState& state) = 0;

    /*! Set the TAP controller to a specific state.
     *
     * \param[in] state JTAG state to set the controller to.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_JTAG_INVALID_STATE The requested state or transition is invalid or unsupported
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see tapStateGet()
     */
    virtual uint32_t tapStateSet(enum jtagState state) = 0;    
};

/******************************************************************************/
/*! Interface for controlling the JTAG frequency.
 * 
 * If the hardware supports a programmable JTAG frequency, then this 
 * interface should be implemented by the same class implementing IJtag.
 ******************************************************************************/
class IJtag_tckFrequency
{
  public:
  
    /*! Set the TCK frequency.
     *
     * Implementations should set the TCK frequency as near as possible to the requested value, then place the actual 
     * frequency achieved in the \a freqHz parameter before returning.  If the hardware does not support changing the TCK
     * frequency, this method should be left unimplemented.  Not achieving the requested frequency is not considered an error.
     * 
     * \param[in,out] freqHz The requested frequency, in Hz.  On return, this variable
     *                       shall be updated with the actual value used; this may differ from the input
     *                       value due to hardware constraints.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see tckFreqGet()
     */
    virtual uint32_t tckFreqSet(uint32_t& freqHz) = 0;

    /*! Get the TCK clock frequency.
     *
     * \param[out] freqHz On return, shall hold the TCK clock frequency in Hz.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see tckFreqSet()
     */
    virtual uint32_t tckFreqGet(uint32_t& freqHz) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_JTAG_H
