/******************************************************************************/
/*! \file classes/jtag.h YAAP JTAG class common header file.
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

#ifndef YAAP_JTAG_CLASS_H
#define YAAP_JTAG_CLASS_H

#include <vector>
#include "classes/base.h"
#include "hal/jtag.h"

using namespace std;

namespace yaap {
namespace classes {

/******************************************************************************/
/*! The YAAP \a jtag class.
 * 
 * This provides the YAAP interface to a JTAG controller in the YAAP device.
 ******************************************************************************/
class jtag : public base
{
public:

    /*! Constructor.
     * 
     * \param[in] name    The name of the object.
     * \param[in] jtagHw  Pointer to the low-level JTAG driver associated with this instance.
     * \param[in] hdrHw   Pointer to the low-level virtual header driver associated with this instance.
     * \param[in] preTCK  Default value for number of clocks before each shift.
     * \param[in] postTCK Default value for number of clocks after each shift.
     */
    jtag(const char *name, hal::IJtag *jtagHw, hal::IHeader *hdrHw, uint32_t preTCK = 5, uint32_t postTCK = 5);

protected:

    //! Shift parameters struct (repeats, retries, pre/post-TCK, terminal state).
    typedef struct
    {
        bool repeatEn;            //!< Enable for repeats
        bool retryEn;             //!< Enable for retries
        uint32_t repeatCnt;       //!< Number of repeats
        uint32_t retryCnt;        //!< Maximum number of retries (data & mask == expected)
        uint32_t preTck;          //!< Number of TCK toggles to perform before each shift
        uint32_t postTck;         //!< Number of TCK toggles to perform after each shift
        uint8_t *retryMask;       //!< Pointer to the TDO mask
        uint8_t *retryExpected;   //!< Pointer to the expected TDO data (post masking)
        uint32_t retryBits;       //!< Number of bits to check for retries
        enum hal::jtagState termState; //!< Default terminal state for the shift
    } shiftParams;

    //! YAAP Method: Perform a shift-IR operation.
    uint32_t yaap_shiftIR(YAAP_METHOD_PARAMS);

    //! YAAP Method: Perform a shift-DR operation.
    uint32_t yaap_shiftDR(YAAP_METHOD_PARAMS);

    //! YAAP Method: Perform a non-destructive shift-IR read operation.
    //uint32_t yaap_shiftIRRead(YAAP_METHOD_PARAMS);

    //! YAAP Method: Perform a non-destructive shift-DR read operation.
    //uint32_t yaap_shiftDRRead(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the TDO mask and expected values.
    uint32_t yaap_optionTdoExpectedGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the TDO mask and expected values.
    uint32_t yaap_optionTdoExpectedSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the retry settings.
    uint32_t yaap_optionRetryGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the retry settings.
    uint32_t yaap_optionRetrySet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the repeat settings.
    uint32_t yaap_optionRepeatGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the repeat settings.
    uint32_t yaap_optionRepeatSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current TAP state machine state.
    uint32_t yaap_tapStateGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the TAP state machine state.
    uint32_t yaap_tapStateSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the TCK frequency.
    uint32_t yaap_optionTckFrequencyGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the TCK frequency.
    uint32_t yaap_optionTckFrequencySet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the post-shift terminal states.
    uint32_t yaap_optionTerminationStateGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the post-shift terminal states.
    uint32_t yaap_optionTerminationStateSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the pre- and post-shift TCK tick counts
    uint32_t yaap_optionPrePostTckGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the pre- and post-shift TCK tick counts
    uint32_t yaap_optionPrePostTckSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the header's enabled status
    uint32_t yaap_enableSet(YAAP_METHOD_PARAMS);   // !!! Code duplication; move this to a new YAAP header class
    
    //! YAAP Method: Get the header's enabled status
    uint32_t yaap_enableGet(YAAP_METHOD_PARAMS);   // !!! Code duplication; move this to a new YAAP header class

    //! YAAP Method: Set Trst
    uint32_t yaap_ioTrstSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get Trst
    uint32_t yaap_ioTrstGet(YAAP_METHOD_PARAMS);

    /*! Perform a shift operation.
     *
     * Note that when repeat is active, the output buffer size will be a multiple of the input buffer size
     * (according to the number of repeats).  Each repeat in the output buffer is byte-aligned; if the
     * bitCnt is 9 and the repeatCnt is 3, the output buffer will be 6 bytes long.  The first repeat TDO
     * data is in byte 0 and the lsb of byte 1; the second repeat TDO data is in byte 2 and the lsb of byte 3;
     * etc.
     * 
     * \param[in] op        The shift operation to perform (IR or DR).
     * \param[in] numBits   The number of bits to shift.
     * \param[in] inData    Pointer to the data to shift in (lsb first).
     * \param[out] outBytes On return, contains the number of bytes in the TDO buffer.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_JTAG_RETRIES_EXHAUSTED Shift was performed with retries enabled, but the expected value was not seen and all retries were exhausted
     * \retval #E_JTAG_HEADER_NOT_ENABLED No shift performed; the JTAG device is not enabled
     * \retval #E_JTAG_HEADER_NOT_CONNECTED No shift performed; no target device is connected
     * \retval #E_JTAG_HEADER_STATUS_CHANGED No shift performed; the target device connection has changed
     * \retval #E_JTAG_INVALID_IR_STATE No shift performed; invalid terminal state requested
     * \retval #E_JTAG_INVALID_DR_STATE No shift performed; invalid terminal state requested
     * \retval #E_OUT_OF_MEMORY No shift performed; there is not enough memory for the output buffer
     * \retval #E_UNSPECIFIED_HW_ERROR No shift performed; unspecified low-level error
     */
    uint32_t shiftCommon(enum hal::jtagOp op, uint32_t numBits, uint8_t *& inData, size_t& outBytes);

    /*! Set the shift parameters for IR or DR shifts.
     *
     * \param[in] params   The parameters struct to set.
     * \param[in] numBits  The number of bits to check in the retry logic.
     * \param[in] mask     The mask to use in the retry logic.
     * \param[in] expected The expected value to use in the retry logic.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_OUT_OF_MEMORY Call required memory allocation, which failed.
     */
    uint32_t setShiftParams(shiftParams& params, uint32_t numBits, uint8_t *mask, uint8_t *expected);

    /*! Resize the output data (TDO) buffer.
     * 
     * \param[in] newSize The requested new buffer size.
     *
     * \retval TRUE The reallocation succeeded
     * \retval FALSE The reallocation failed (the buffer remains its previous size)
     */
    bool resizeOutputBuffer(uint32_t newSize);

    uint8_t *m_tdoBuffer;               //!< This is the buffer used for TDO data.
    uint32_t m_tdoBufSize;              //!< The current size of the TDO buffer; use resizeOutputBuffer() to resize it.
    shiftParams m_shiftIrParams;        //!< Shift-specific params for Shift-IR.
    shiftParams m_shiftDrParams;        //!< Shift-specific params for Shift-DR.
    
    hal::IJtag *m_jtagHw;                               //!< The JTAG HAL
    hal::IJtag_nonDestructiveRead *m_jtagNonDestruct;   //!< Non-destructive JTAG read HAL
    hal::IJtag_tapState *m_tapState;                    //!< TAP state set/get HAL
    hal::IJtag_tckFrequency *m_tckFreq;                 //!< TCK frequency set/get HAL
    
    hal::IHeader *m_hdrHw;              //!< The header hardware interface.
    hal::IHeader_setEnabled *m_hdrSet;  //!< Header enable/disable HAL.
    hal::IHeader_trst *m_hdrTrst;        //!< Header TRST HAL.
    
    static vector<jtag *> m_instances;  //!< All instances of this class (to allow the output buffer to be shared).
};

} // namespace classes
} // namespace yaap

#endif // YAAP_JTAG_CLASS_H
