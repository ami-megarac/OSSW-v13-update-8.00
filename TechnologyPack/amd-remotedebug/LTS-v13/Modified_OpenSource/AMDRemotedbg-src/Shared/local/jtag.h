/******************************************************************************/
/*! \file local/jtag.h Local JTAG class header file.
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
 ******************************************************************************/

#ifndef LOCAL_JTAG_CLASS_H
#define LOCAL_JTAG_CLASS_H

#include "classes/jtag.h"

namespace yaap {
    namespace local {

        /******************************************************************************/
        /*! The local \a jtag class.
         * 
         * Use this to perform JTAG shifts from locally-running software.  No software
         * state is shared with the YAAP JTAG instance(s).
         ******************************************************************************/
        class jtag : public yaap::classes::jtag
        {
        public:

            /*! Constructor.
             * 
             * \param jtagHw  Pointer to the low-level JTAG driver associated with this instance.
             * \param hdrsHw  Pointer to the low-level header driver associated with this instance.
             */
            jtag(IJtagController *jtagHw, IHeaders *hdrsHw);
            
            /*! Configure retry settings.
             * 
             * \param en        Enable retries
             * \param cnt       Maximum number of retries
             * \param bits      Number of bits to compare
             * \param mask      Pointer to the mask value
             * \param expected  Pointer to the expected value (post-masking)
             */
            void configureRetry(bool en, int cnt, int bits, uint8_t *& mask, uint8_t *& expected);
            
            /*! Configure repeat settings.
             * 
             * \param en  Enable repeats
             * \param cnt Number of repeats
             */
            void configureRepeat(bool en, int cnt);
            
            /*! Configure pre-/post-TCK toggles and terminal state.
             * 
             * \param preTck    Number of TCK toggles to perform before each shift
             * \param postTck   Number of TCK toggles to perform after each shift
             * \param termState Default terminal state for the shift
             * 
             * \retval #E_SUCCESS Call succeeded
             */
            uint32_t configureTckAndState(uint32_t preTck, uint32_t postTck, enum jtagState termState);
            
            /*! Perform a shift.
             * 
             * \param op       Operation to perform (IR or DR)
             * \param numBits  Number of bits to shift
             * \param inData   Points to the data to shift in
             * \param outData  On return, points to the data shifted out
             * \param outBytes On return, contains the number of bytes in the buffer at \a outData
             * 
             * \retval #E_SUCCESS Call succeeded
             * \retval #E_OUT_OF_MEMORY No shift performed; there is not enough memory for the output buffer.
             */
            uint32_t shift(enum jtagOp op, uint32_t numBits, uint8_t *& inData, uint8_t *& outData, size_t& outBytes);

        };
    } // namespace local
} // namespace yaap

#endif // LOCAL_JTAG_CLASS_H
