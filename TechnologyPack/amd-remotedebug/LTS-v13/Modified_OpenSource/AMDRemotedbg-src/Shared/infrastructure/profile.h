/******************************************************************************/
/*! \file profile.h Profiling helps.
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

#ifndef PROFILE_H
#define PROFILE_H

// These are the failsafe definitions for profiling.  Real definitions should
// be handled in a project specific profile.h in the top level 
// that is include by a -imacros option in the Makefile

#ifndef PROFILE_PINS_ENABLED
  #define PROF_PIN_INIT 
  #define PROF_PIN_SET(value)
  #define PROF_PIN_GET(value)
  #define PROF_PIN_0 
  #define PROF_PIN_1
  #define PROF_PIN_2
  #define PROF_PIN_3
  #define PROF_PIN_4
  #define PROF_PIN_5
  #define PROF_PIN_6
  #define PROF_PIN_7
  #define PROF_PIN_8
  #define PROF_PIN_9
  #define PROF_PIN_A
  #define PROF_PIN_B
  #define PROF_PIN_C
  #define PROF_PIN_D
  #define PROF_PIN_E
  #define PROF_PIN_F
#endif // PROFILE_PINS_ENABLED
#endif // PROFILE_H

#ifndef PROFILE_VALUES_H
#define PROFILE_VALUES_H
// These defines are values to be put on the pins
// They are separate from the hardware specific implementaion
// so that a common decoding can be done easily

//! Start of getReadyFd and waiting for connection
#define SKT_PRF_GET_FD_S PROF_PIN_0
  
//! Connection processing
#define SKT_PRF_NEW_FD_S PROF_PIN_1

//! Exit from getReadyFd should get yaarp connection very soon after
#define SKT_PRF_GET_FD_P PROF_PIN_1

//! Start of processYaarpConnection and data receiving
#define CONN_PRF_YAARP_ST PROF_PIN_2

//! Before call to socket select
#define CONN_PRF_SELECT PROF_PIN_3

//! Before call to socket Recv
#define CONN_PRF_RECV PROF_PIN_1

//! Packet validation (?) state machine 
#define CONN_PRF_YAARP_SM PROF_PIN_4

//! Yaarp stream allocation and verification
#define CONN_PRF_YAARP_ALLOC PROF_PIN_3

//! Yaap method decode and verification
#define CONN_PRF_YAAP_DEC PROF_PIN_3

//! Right before invoke method is called
#define CONN_PRF_YAAP_EXEC PROF_PIN_2

//! Checking for valid execution
#define CONN_PRF_YAAP_CHECK PROF_PIN_4

//! Yaarp status transmit
#define CONN_PRF_YAARP_TX_ST PROF_PIN_3

//! Yaarp error transmit
#define CONN_PRF_YAARP_TX_ER PROF_PIN_2

//! Yaarp method response transmit
#define CONN_PRF_YAARP_TX_MT PROF_PIN_3

//! processYaarpConnection exit
#define CONN_PRF_YAARP_EXIT PROF_PIN_1

//! YAAP Class Function start
#define YAAP_PRF_FUNC_ST PROF_PIN_5

//! YAAP Class output stream put
#define YAAP_PRF_FUNC_OUT PROF_PIN_6



//! YAAP JTAG Class shiftCommon start
#define YAAP_PRF_SHF_ST PROF_PIN_6

//! YAAP JTAG Class shiftCommon Decode(after header checks)
#define YAAP_PRF_SHF_DEC PROF_PIN_7

//! YAAP JTAG Class shiftCommon Exec(inside repeat loop)
#define YAAP_PRF_SHF_EXEC PROF_PIN_5

//! YAAP JTAG Class shiftCommon save to OutBuffer
#define YAAP_PRF_SHF_STR PROF_PIN_3

//! YAAP JTAG Class shiftCommon ReTryCheck
#define YAAP_PRF_SHF_RTC PROF_PIN_4

//! YAAP JTAG Class Exit
#define YAAP_PRF_SHFDR_EXIT PROF_PIN_6

//! sendRawData
#define CONN_PRF_SEND_DATA PROF_PIN_7

#endif // PROFILE_VALUES_H


