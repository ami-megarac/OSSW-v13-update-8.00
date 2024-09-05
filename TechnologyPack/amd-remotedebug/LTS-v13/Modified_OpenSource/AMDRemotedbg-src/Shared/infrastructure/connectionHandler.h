/**************************************************************************/
/*! \file connectionHandler.h YAARP connection handler header.
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

#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "yaarp/streams.h"
#include "socketHandler.h"

using namespace YAARP;

#define RX_BUF_SIZE (4 << 20)       //!< Size of receive buffer (bytes)
#define TMP_STRING_BUF_SIZE 1024    //!< Used for creating dynamic error strings

namespace yaap {

    //! The possible return values for processYaarpConnection()
    enum ConnectionState
    {
        KEEP_OPEN,            //!< Keep the connection open for further processing
        CLOSE_CONNECTION,     //!< Close the connection
        SHUT_DOWN_SERVER,     //!< Shut down the server
    };

    /**************************************************************************/
    /*! This is a singleton class for processing a single YAARP connection.
     * 
     * No state is maintained across different YAARP connections.
     **************************************************************************/
    class connectionHandler
    {
    public:
        
        /*! Process a YAARP connection.
         * 
         * \param conn      Pointer to the connection descriptor
         * \param rxBufBase Base of the receive buffer
         *
         * \return Code indicating whether to close the connection or shut down the server
         */
        static enum ConnectionState processYaarpConnection(yaapConnection *conn, uint8_t *rxBufBase);
        
    private: 
    
        /*! Send data to the output stream.
         * 
         * \param fd    The connection FD
         * \param bytes Number of bytes to send
         * \param ptr   Pointer to first byte to send
         * \param flags Flags to pass into the send system call (i.e. MSG_MORE)
         * 
         * \return TRUE if all bytes were successfully sent, FALSE otherwise
         */
        static bool sendRawData(int fd, uint32_t bytes, uint8_t *ptr, int flags = 0);
        
        /*! Send an error/status chunk.
         *
         * \param fd The connection FD
         * \param s  The error/status stream to send
         *
         * \return TRUE if all data was sent properly, FALSE otherwise
         */
        static bool sendErrorStatus(int fd, ErrorStatusStream& s);
        
        /*! Write some bytes to the debug file "/tmp/yaapd_debug.txt".
         *
         * \param buf Pointer to the bytes to write
         * \param cnt Number of bytes to write
         */
        static void writeBytesToDebugFile(uint8_t *buf, uint32_t cnt, bool incomming);
    
        /*! Return the difference between two timestamps, in msec.
         * 
         * \param first  First timestamp
         * \param second Second timestamp
         * 
         * \return Difference between first and second, in msec.
         */
        static uint32_t timeDiffMsec(struct timeval& first, struct timeval& second);
    
        //! States for the input stream state machine.
        enum InputStatesEnum
        {
            LOCK_ID,              //!< Receiving the 4-byte lock ID
            METHOD_CNT,           //!< Receiving the 4-byte method count 

            METHOD_INSTANCE_ID,   //!< Receiving the 4-byte instance ID
            METHOD_METHOD_ID,     //!< Receiving the 4-byte method ID
            METHOD_PAYLOAD_SIZE,  //!< Receiving the 4-byte payload size
            METHOD_PAYLOAD,       //!< Receiving the arbitrary-sized method payload

            DONE,                 //!< Done processing the input stream

            INVALID,              //!< Invalid state
        };
        
        static char tmpStringBuf[TMP_STRING_BUF_SIZE];  //!< Used for creating error strings
    };

} // namespace yaap

#endif // CONNECTION_HANDLER_H
