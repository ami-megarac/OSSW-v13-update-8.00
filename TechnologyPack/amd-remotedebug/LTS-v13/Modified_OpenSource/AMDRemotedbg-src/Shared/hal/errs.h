/******************************************************************************/
/*! \file errs.h YAAP Error code definitions.
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

#ifndef ERRS_H
#define ERRS_H

#define E_SUCCESS                       0                       //!< Success

// Common errors
#define E_COM_BASE                      1000
#define E_LOCK_MISMATCH                 E_COM_BASE + 0x0        //!< Device isn't locked, but the given lockId is not the last one used
#define E_DEVICE_LOCKED                 E_COM_BASE + 0x1        //!< Device is locked by someone else
#define E_MALFORMED_REQUEST             E_COM_BASE + 0x2        //!< Problem in the input stream
#define E_INVALID_INSTANCE_ID           E_COM_BASE + 0x3        //!< No such instance
#define E_INVALID_METHOD_ID             E_COM_BASE + 0x4        //!< No such method
#define E_INVALID_PARAMETERS            E_COM_BASE + 0x5        //!< Deprecated
#define E_METHOD_ERROR                  E_COM_BASE + 0x6        //!< Method returned non-success
#define E_NOT_IMPLEMENTED               E_COM_BASE + 0x7        //!< Functionality is not implemented
#define E_ALREADY_RESERVED              E_COM_BASE + 0x8        //!< Resource is already reserved
#define E_NOT_RESERVED                  E_COM_BASE + 0x9        //!< Resource is not reserved
#define E_BAD_CREDENTIALS               E_COM_BASE + 0xA        //!< Invalid credentials supplied
#define E_SERVER_ERROR                  E_COM_BASE + 0xB        //!< Non-method error (i.e. problem with output YAARP stream)
#define E_INVALID_MODE                  E_COM_BASE + 0xC        //!< Invalid mode specified or detected

#define E_COM2_BASE                     2000
#define E_UNSPECIFIED_HW_ERROR          E_COM2_BASE + 0x0       //!< This indicates an unspecified low-level error 
#define E_OUT_OF_MEMORY                 E_COM2_BASE + 0x1       //!< Memory allocation failed
#define E_INVALID_SETTING               E_COM2_BASE + 0x2       //!< Invalid setting
#define E_WRITE_ONLY                    E_COM2_BASE + 0x3       //!< Attempt to read a write-only object
#define E_READ_ONLY                     E_COM2_BASE + 0x4       //!< Attempt to write a read-only object
#define E_TOO_BIG                       E_COM2_BASE + 0x5       //!< Object, buffer, etc. is too large
#define E_TOO_SMALL                     E_COM2_BASE + 0x6       //!< Object, buffer, etc. is too small
#define E_UNSUPPORTED_DEVICE            E_COM2_BASE + 0x7       //!< Specified device is not supported

// "system" class errors
#define E_SYSTEM_BASE                   2000
#define E_SYSTEM_INVALID_FW_TYPE        E_SYSTEM_BASE + 0x2     //!< Invalid Firmware type 
#define E_SYSTEM_FW_NOT_READY           E_SYSTEM_BASE + 0x3     //!< Firmware not ready (for upgrade)

// "jtag" class errors
#define E_JTAG_BASE                     2000
#define E_JTAG_LENGTH_MISMATCH          E_JTAG_BASE + 0x2       //!< Array length mismatch.
#define E_JTAG_HEADER_NOT_ENABLED       E_JTAG_BASE + 0x3       //!< Header is tristated.
#define E_JTAG_HEADER_NOT_CONNECTED     E_JTAG_BASE + 0x4       //!< Presence of connected device not detected.
#define E_JTAG_HEADER_STATUS_CHANGED    E_JTAG_BASE + 0x5       //!< A change in the header connection has occurred and needs to be cleared.
#define E_JTAG_HEADER_CONFLICT          E_JTAG_BASE + 0x6       //!< The header is conflicted.
#define E_JTAG_INVALID_IR_STATE         E_JTAG_BASE + 0x7       //!< Invalid terminal state for IR shifts.
#define E_JTAG_INVALID_DR_STATE         E_JTAG_BASE + 0x8       //!< Invalid terminal state for DR shifts.
#define E_JTAG_RETRIES_EXHAUSTED        E_JTAG_BASE + 0x9       //!< Maximum shift retries exhausted.
#define E_JTAG_FSM_ASYNC_RESET          E_JTAG_BASE + 0xA       //!< The TAP state machine was reset, i.e. due to header status change
#define E_JTAG_INVALID_STATE            E_JTAG_BASE + 0xB       //!< Invalid state (or state transition).

// "hdt" class errors
#define E_HDT_BASE                      2000
#define E_HDT_HEADER_NOT_ENABLED        E_HDT_BASE + 0x2        //!< Header is tristated.
#define E_HDT_HEADER_NOT_CONNECTED      E_HDT_BASE + 0x3        //!< Presence of connected device not detected.
#define E_HDT_HEADER_STATUS_CHANGED     E_HDT_BASE + 0x4        //!< A change in the header connection has occurred and needs to be cleared.
#define E_HDT_HEADER_CONFLICT           E_HDT_BASE + 0x5        //!< The header is conflicted.
#define E_HDT_DBRDY_TIMEOUT             E_HDT_BASE + 0x6        //!< Timed out waiting for DBRDY to match expected value
#define E_HDT_INVALID_TRIGGER_CFG       E_HDT_BASE + 0x7        //!< Invalid trigger configuration  
#define E_HDT_INVALID_TRIGGER_CHAN      E_HDT_BASE + 0x8        //!< Invalid trigger channel

// "i2c" class errors
#define E_I2C_BASE                      2000
#define E_I2C_LENGTH_MISMATCH           E_I2C_BASE + 0x2        //!< Array length mismatch.
#define E_I2C_HEADER_NOT_CONNECTED      E_I2C_BASE + 0x3        //!< Presence of connected device not detected.
#define E_I2C_HEADER_STATUS_CHANGED     E_I2C_BASE + 0x4        //!< A change in the header connection has occurred and needs to be cleared.
#define E_I2C_HEADER_CONFLICT           E_I2C_BASE + 0x5        //!< The header is conflicted.
#define E_I2C_BUS_BUSY_TIMEOUT          E_I2C_BASE + 0x6        //!< Timed out waiting for bus-busy condition to clear.
#define E_I2C_ACK_TIMEOUT               E_I2C_BASE + 0x7        //!< Timed out waiting for ACK.
#define E_I2C_ADDRESS_NAK               E_I2C_BASE + 0x8        //!< No slave ACKed the address.
#define E_I2C_WRITE_NAK                 E_I2C_BASE + 0x9        //!< The slave did not ACK the write.
#define E_I2C_TRANSFER_TOO_LARGE        E_I2C_BASE + 0xA        //!< Requested transfer is too large.
#define E_I2C_UNSUPPORTED_ADDRESS_TYPE  E_I2C_BASE + 0xB        //!< The address type (7-/10-bit) is not supported.
#define E_I2C_INVALID_COUNT             E_I2C_BASE + 0xC        //!< Requested count (i.e. retries) not supported.
#define E_I2C_DEVICE_NOT_OPEN           E_I2C_BASE + 0xD        //!< The device is not open.
#define E_I2C_INVALID_TIMEOUT           E_I2C_BASE + 0xE        //!< Requested timeout (i.e. bus busy) not supported.

// "gpuScan" class errors
#define E_GPUSCAN_BASE                  2000
#define E_GPUSCAN_HEADER_NOT_ENABLED    E_GPUSCAN_BASE + 0x2    //!< Header is tristated.
#define E_GPUSCAN_HEADER_NOT_CONNECTED  E_GPUSCAN_BASE + 0x3    //!< Presence of connected device not detected.
#define E_GPUSCAN_HEADER_STATUS_CHANGED E_GPUSCAN_BASE + 0x4    //!< A change in the header connection has occurred and needs to be cleared.
#define E_GPUSCAN_HEADER_CONFLICT       E_GPUSCAN_BASE + 0x5    //!< The header is conflicted.

#endif // ERRS_H
