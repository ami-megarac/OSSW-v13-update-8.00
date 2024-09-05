/******************************************************************************/
/*! \file connectionHandler.cpp YAARP connection handler.
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

#include <inttypes.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "connectionHandler.h"
#include "debug.h"
#include "profile.h"
#include "classes/base.h"

using namespace std;
using namespace yaap::classes;

//#define DEBUG_EXECUTION_TIMES

#ifndef YAARP_SIZE_INT
#define YAARP_SIZE_INT         4    //!< Size of an integer in YAARP
#endif

#ifndef YAARP_PROTOCOL_TIMEOUT
#define YAARP_PROTOCOL_TIMEOUT 10   //!< Timeout in seconds
#endif

namespace yaap {
    
    char connectionHandler::tmpStringBuf[TMP_STRING_BUF_SIZE]; 

    /**************************************************************************/

    inline bool connectionHandler::sendRawData(int fd, uint32_t bytes, uint8_t *ptr, int flags)
    {
        for (uint32_t bytesSent = 0; bytesSent < bytes; )
        {
            //PROF_PIN_GET(prevProf);
            //PROF_PIN_SET(CONN_PRF_SEND_DATA);

#ifdef DEBUG_EXECUTION_TIMES
            writeBytesToDebugFile(ptr, bytes, false);
#endif



            int retval = send(fd, &(ptr[bytesSent]), (bytes - bytesSent), flags);
            
            //PROF_PIN_SET(prevProf);
            
            if (retval == -1)
            {
                perror("send()");
                return false;
            }
            bytesSent += retval;
        }
        return true;
    }

    /**************************************************************************/

    inline bool connectionHandler::sendErrorStatus(int fd, ErrorStatusStream& s)
    {
        uint32_t bytes = s.totalSize();
        if (bytes)
        {
            bytes += YAARP_SIZE_INT;  // The extra int is number of entries
        }
        uint32_t swappedSize = htonl(bytes);

        if (!sendRawData(fd, YAARP_SIZE_INT, (uint8_t *)&swappedSize, MSG_MORE)) {return false;}

        if (bytes)
        {
            // Send the number of entries
            uint32_t swappedEntries = htonl(s.size());
            if (!sendRawData(fd, YAARP_SIZE_INT, (uint8_t *)&swappedEntries, MSG_MORE)) {return false;}
        
            // Send the entries
            std::vector<errorStatusEntry>::iterator iter;
            for (iter = s.begin(); iter != s.end(); iter++)
            {
                uint32_t swappedCode = htonl((*iter).id);
                uint32_t size = (*iter).str.size();
                uint32_t swappedSize = htonl(size);

                if (!sendRawData(fd, YAARP_SIZE_INT, (uint8_t *)&swappedCode, MSG_MORE)) {return false;}
                if (!sendRawData(fd, YAARP_SIZE_INT, (uint8_t *)&swappedSize, MSG_MORE)) {return false;}
                if (!sendRawData(fd, size, (uint8_t *)(*iter).str.c_str())) {return false;}
            }
        }

        return true;
    }

    /**************************************************************************/
    
    void connectionHandler::writeBytesToDebugFile(uint8_t *buf, uint32_t cnt, bool incoming)
    {
        static bool first = true;
        static FILE *fd = NULL;

        if (first)
        {
            fd = fopen("/tmp/yaapd_debug.txt", "w");
            if (fd == NULL)
            {
                perror("fopen");
            }
            first = false;
        }

        if (fd)
        {
            fprintf(fd, "-------------- %s %04d bytes -----------------\n", incoming == true ? "Received" : "Sent", cnt);

            unsigned int i = 0;
            uint8_t *b = buf;

            while (i < cnt)
            {
                bool done = false;
                int chars = 0;
                int idx = 0;

                for (int k = 0; k < 20; k++)
                {
                    switch (k)
                    {
                    case 4:
                    case 9:
                    case 14:
                    case 19:
                        fprintf(fd, " ");
                        break;

                    default:
                        if (done)
                        {
                            fprintf(fd, "  ");
                        }
                        else
                        {
                            fprintf(fd, "%02X", b[idx++]);
                            chars++;
                            if (++i == cnt)
                            {
                                done = true;
                            }
                        }
                        break;
                    }
                }

                for (int k = 0; k < chars; k++)
                {
                    fprintf(fd, "%c", isprint(b[k]) ? b[k] : '.');
                }

                b += chars;

                fprintf(fd, "\n");
            }
            fflush(fd);
        }
    }

    /**************************************************************************/
    
    inline uint32_t connectionHandler::timeDiffMsec(struct timeval& first, struct timeval& second)
    {
        uint64_t retval = second.tv_sec - first.tv_sec;
        retval *= 1000;  // Make it msec

        int32_t usecDiff = second.tv_usec - first.tv_usec;
        if (usecDiff < 0)
        {
            usecDiff += 1000000;
        }
        usecDiff += 500;
        usecDiff /= 1000;  // Make it msec
        return retval + usecDiff;
    }
        
    /**************************************************************************/
    
    enum ConnectionState connectionHandler::processYaarpConnection(yaapConnection *conn, uint8_t * rxBufBase)
    {
        int fd = conn->fd;                           // The connection's file descriptor
        int retval;                                 // Used for socket calls
        uint8_t *rxBuf = rxBufBase;                 // The receive buffer
        int rxBytes = 0;                            // Total bytes received
        InputStatesEnum state = LOCK_ID;            // State machine state
        int remainingBytesInState = YAARP_SIZE_INT; // Bytes left in this state
        vector<pair<uint32_t, const char *> > status; // Code/string pairs for status field in response
        vector<pair<uint32_t, const char *> > error;  // Code/string pairs for error field in response
        uint32_t methodCounter = 0;                 // Counts methods as we receive them
        InputStream is;                             // Input stream object
        OutputStream os;                            // Output stream object
        ErrorStatusStream ss;                       // Status stream object
        ErrorStatusStream es;                       // Error stream object
        uint32_t methodPayloadSize = 0;             // Size of payload being received
        uint32_t pendingUnreadBytes = 0;            // Bytes not yet parsed (straddling packets)
        struct timeval tv, now;                     // Timeout for select

        PROF_PIN_SET(CONN_PRF_YAARP_ST);

        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        if (gettimeofday(&now, NULL) == -1)
        {
            perror("gettimeofday()");
        }

#ifdef DEBUG_EXECUTION_TIMES
        // Used for timing the execution...
        struct timeval timeStartRx, timeStartExec, timeStartTx, timeDone;
        timeStartRx = now;
#endif

        for (bool done = false; !done; )
        {
            // Our timeout.  If we go this long without receiving anything, we consider it a protocol error.
            tv.tv_sec = YAARP_PROTOCOL_TIMEOUT;
            tv.tv_usec = 0;

            // Wait for available data.  Doing this with a select() rather than a blocking recv() allows us 
            // to detect a timeout condition, which probably indicates that the connection was lost.
            PROF_PIN_SET(CONN_PRF_SELECT);
            retval = select(fd + 1, &rfds, NULL, NULL, &tv);

            if (retval == -1)
            {
                perror("select()");
                return KEEP_OPEN;
            }
            else if (retval == 0)
            {
                DEBUG_PRINT(DEBUG_ERROR, "Timed out while receiving YAARP message; closing connection.");
                return CLOSE_CONNECTION;
            }
            else if (rxBytes == RX_BUF_SIZE)
            {
                DEBUG_PRINT(DEBUG_ERROR, "Packet is too large.");
                return CLOSE_CONNECTION;
            }

            // Read as much as we can
            PROF_PIN_SET(CONN_PRF_RECV);
            int bytesToProcess = recv(fd, rxBuf + rxBytes, RX_BUF_SIZE - rxBytes, 0);

#ifdef DEBUG_EXECUTION_TIMES
//            writeBytesToDebugFile(rxBuf + rxBytes, bytesToProcess);
            writeBytesToDebugFile(rxBuf + rxBytes, bytesToProcess, true);
#endif

            DEBUG_PRINT(DEBUG_VERBOSE, "Read %d bytes from socket", bytesToProcess);

            if (bytesToProcess == 0)
            {
                // The other side has closed the socket

                DEBUG_PRINT(DEBUG_MOREINFO, "Other side closed connection %d", fd);
                return CLOSE_CONNECTION;
            }
            
            // Note: the InputStream object takes care of alignment problems for us.
            is.Reset(rxBuf + rxBytes - pendingUnreadBytes, bytesToProcess + pendingUnreadBytes);

            while (bytesToProcess && (state != DONE))
            {
                // Run the state machine on the received bytes
                int consumedBytes = (bytesToProcess < remainingBytesInState) ? bytesToProcess : remainingBytesInState;

                remainingBytesInState -= consumedBytes;
                rxBytes += consumedBytes;
                bytesToProcess -= consumedBytes;

                if (remainingBytesInState)
                {
                    // The element currently being recieved straddles this packet and the next.
                    pendingUnreadBytes += consumedBytes;
                }
                else
                {
                    // This element can be fully received... process a state transition.
                    uint32_t tmpInt;

                    PROF_PIN_SET(CONN_PRF_YAARP_SM);

                    pendingUnreadBytes = 0;
                    switch (state)
                    {
                    case LOCK_ID:
                        tmpInt = is.getInt();
                        // tmpInt is ignored here; check lock ID later
                        state = METHOD_CNT;
                        remainingBytesInState = YAARP_SIZE_INT;
                        DEBUG_PRINT(DEBUG_VERBOSE, "State LOCK_ID (%08X) -> METHOD_CNT", tmpInt);
                        break;
                        
                    case METHOD_CNT:
                        methodCounter = is.getInt();
                        if (methodCounter > 0)
                        {
                            state = METHOD_INSTANCE_ID;
                            remainingBytesInState = YAARP_SIZE_INT;
                            DEBUG_PRINT(DEBUG_VERBOSE, "State METHOD_CNT (%d) -> METHOD_INSTANCE_ID", methodCounter);
                            methodCounter--;  // For the method we're currently receiving
                        }
                        else
                        {
                            // No methods in this packet.  That's wierd...
                            state = DONE;
                            remainingBytesInState = 0;
                            DEBUG_PRINT(DEBUG_WARNING, "No methods in the packet");
                        }
                        break;

                    case METHOD_INSTANCE_ID:
                        tmpInt = is.getInt();
                        // tmpInt is ignored here; check instance ID later
                        state = METHOD_METHOD_ID;
                        remainingBytesInState = YAARP_SIZE_INT;
                        DEBUG_PRINT(DEBUG_VERBOSE, "State METHOD_INSTANCE_ID (%d) -> METHOD_METHOD_ID", tmpInt);
                        break;
                        
                    case METHOD_METHOD_ID:
                        tmpInt = is.getInt();
                        // tmpInt is ignored here; check method ID later
                        state = METHOD_PAYLOAD_SIZE;
                        remainingBytesInState = YAARP_SIZE_INT;
                        DEBUG_PRINT(DEBUG_VERBOSE, "State METHOD_METHOD_ID (%d) -> METHOD_PAYLOAD_SIZE", tmpInt);
                        break;

                    case METHOD_PAYLOAD_SIZE:
                        tmpInt = is.getInt();
                        remainingBytesInState = methodPayloadSize = tmpInt;
                        if (methodPayloadSize)
                        {
                            state = METHOD_PAYLOAD;
                            DEBUG_PRINT(DEBUG_VERBOSE, "State METHOD_PAYLOAD_SIZE (%d) -> METHOD_PAYLOAD",  tmpInt);
                            break;
                        }
                        // FALLTHROUGH

                    case METHOD_PAYLOAD:
                        is.getRawBytePtr(methodPayloadSize);
                        if (methodCounter)
                        {
                            methodCounter--;
                            state = METHOD_INSTANCE_ID;
                            remainingBytesInState = YAARP_SIZE_INT;
                            DEBUG_PRINT(DEBUG_VERBOSE, "State METHOD_PAYLOAD -> METHOD_INSTANCE_ID");
                        }
                        else
                        {
                            state = DONE;
                            remainingBytesInState = 0;
                            DEBUG_PRINT(DEBUG_VERBOSE, "State METHOD_PAYLOAD -> DONE");
                        }
                        break;

                    case DONE:
                    default:
                        break;

                    } // End of: switch (state)
                } // End of: if (remainingInStateBytes == 0)
            } // End of: while (bytesToProcess && (state != DONE))

//            PROF_PIN_SET(CONN_PRF_YAARP_ALLOC);

            if (state == DONE)
            {
                if (bytesToProcess)
                {
                    fprintf(stderr, "Ignoring extra bytes in YAARP request.\n");
                }
                done = true;
            }
        } // End of: while (!done)

#ifdef DEBUG_EXECUTION_TIMES
        if (gettimeofday(&timeStartExec, NULL) == -1)
        {
            perror("gettimeofday()");
        }
#endif

        // Now we have a complete message; process it and send the response

        is.Reset(rxBufBase, rxBytes);

        uint32_t lockId = is.getInt();
        uint32_t methodCnt = is.getInt();
        uint32_t methodsRun = 0;  // Tracks number of methods actually invoked.

        DEBUG_PRINT(DEBUG_MOREINFO, "Lock ID = %08X, Method count = %d", lockId, methodCnt);

        if (is.isError())
        {
            // This shouldn't happen, since we successfully completed the state machine above.  
            DEBUG_PRINT(DEBUG_ERROR, "YAARP state machine error");
            es.add(E_MALFORMED_REQUEST, "Error reading YAARP header");
        }
        else
        {
            for (methodsRun = 0; methodsRun < methodCnt; )
            {
                PROF_PIN_SET(CONN_PRF_YAAP_DEC);
                
                uint32_t instanceId = is.getInt();
                uint32_t methodId = is.getInt();
                uint32_t payloadSize = is.getInt();
                uint32_t bytesBefore = is.getBytesRemaining();

                base *obj = base::getInstance(instanceId);

                if (is.isError())
                {
                    DEBUG_PRINT(DEBUG_ERROR, "Input stream error before call # %d", methodsRun);
                    es.add(E_MALFORMED_REQUEST, "Error reading input before method call");
                    break;
                }
                else if (!base::isValidInstance(instanceId))
                {
                    snprintf(tmpStringBuf, TMP_STRING_BUF_SIZE, "Bad instance ID (%d) in method call %d", instanceId, methodsRun);
                    DEBUG_PRINT(DEBUG_ERROR, "Bad instance ID (%d) in method call %d", instanceId, methodsRun);
                    es.add(E_INVALID_INSTANCE_ID, tmpStringBuf);
                    break;
                }
                else if (!base::isValidMethod(instanceId, methodId))
                {
                    snprintf(tmpStringBuf, TMP_STRING_BUF_SIZE, "Bad method ID (%d.%d) in method call %d", instanceId, methodId, methodsRun);
                    DEBUG_PRINT(DEBUG_ERROR, "Bad method ID (%d:%d) in method call %d", instanceId, methodId, methodsRun);
                    es.add(E_INVALID_METHOD_ID, tmpStringBuf);
                    break;
                }
                else
                {
                    lockIdCheckRetval lockCheck = obj->checkLock(lockId, methodId);
                    if (lockCheck.is_locked && !lockCheck.lock_id_matches && lockCheck.lock_required)
                    {
                        // Device is locked by someone else, and this method requires lock
                        DEBUG_PRINT(DEBUG_INFO, "Attempt to invoke method %s.%s; wrong lock ID provided", obj->getName(), obj->getMethodName(methodId));
                        es.add(E_DEVICE_LOCKED, base::getLocker());
                        break;
                    }
                    if (lockCheck.lock_timed_out && lockCheck.lock_required)
                    {
                        // Lock timed out, and another user has since locked & unlocked it
                        DEBUG_PRINT(DEBUG_INFO, "Attempt to invoke method %s.%s; lock timed out, and another user accessed the device in the meantime", obj->getName(), obj->getMethodName(methodId));
                        es.add(E_LOCK_MISMATCH, "Your lock has timed out and another user locked the device in the meantime");
                        break;
                    }
                    if (!lockCheck.ok_to_exe)
                    {
                        // Any other case of lock check failure
                        DEBUG_PRINT(DEBUG_INFO, "Attempt to invoke method %s.%s; execution not allowed (lock not held)", obj->getName(), obj->getMethodName(methodId));
                        es.add(E_LOCK_MISMATCH, "You do not hold the lock on the device");
                        break;
                    }
                }

                conn->lastAccess = now; // Reset the timout counter on this connection.
                DEBUG_PRINT(DEBUG_MOREINFO, "Invoking method \"%s.%s\"", obj->getName(), obj->getMethodName(methodId));
                methodsRun++;

                PROF_PIN_SET(CONN_PRF_YAAP_EXEC);

                uint32_t methodRet = obj->invoke(methodId, is, os, ss, es);

                PROF_PIN_SET(CONN_PRF_YAAP_CHECK);

                if (methodRet != E_SUCCESS)
                {
                    DEBUG_PRINT(DEBUG_ERROR, "Method returned %u; terminating multicall; %d of %d methods executed", methodRet, methodsRun, methodCnt);
                    es.add(E_METHOD_ERROR, "Method returned nonzero value");
                    break;
                }

                if (is.getBytesRemaining() != (bytesBefore - payloadSize))
                {
                    DEBUG_PRINT(DEBUG_ERROR, "Expected method to consume %d bytes, but it consumed %zu", payloadSize, bytesBefore - is.getBytesRemaining());
                    es.add(E_MALFORMED_REQUEST, "Method consumed wrong amount of input data");
                    break;
                }

                if (is.isError())
                {
                    DEBUG_PRINT(DEBUG_WARNING, "Input stream error after call # %d", methodsRun);
                    es.add(E_MALFORMED_REQUEST, "Error in input stream after method call");
                    break;
                }
                if (os.isError())
                {
                    DEBUG_PRINT(DEBUG_WARNING, "Output stream error after call # %d", methodsRun);
                    es.add(E_SERVER_ERROR, "Error in output stream after method call");
                    break;
                }
            }
        }

        PROF_PIN_SET(CONN_PRF_YAARP_TX_ST);

#ifdef DEBUG_EXECUTION_TIMES
        if (gettimeofday(&timeStartTx, NULL) == -1)
        {
            perror("gettimeofday()");
        }
#endif

        // Now send out the response.  First, the status...
        if (!sendErrorStatus(fd, ss))
        {
            DEBUG_PRINT(DEBUG_ERROR, "Send status failed");
            return KEEP_OPEN;
        }
        DEBUG_PRINT(DEBUG_VERBOSE, "Sent status (%zu entries; %d bytes)", ss.size(), ss.totalSize());

        PROF_PIN_SET(CONN_PRF_YAARP_TX_ER);

        // Now send the error 
        if (!sendErrorStatus(fd, es))
        {
            DEBUG_PRINT(DEBUG_ERROR, "Send error failed");
            return KEEP_OPEN;
        }
        DEBUG_PRINT(DEBUG_VERBOSE, "Sent error (%zu entries; %d bytes)", es.size(), es.totalSize());

        PROF_PIN_SET(CONN_PRF_YAARP_TX_MT);

        // Now send the number of method responses
        uint32_t swappedNumResponses = htonl(methodsRun);
        if (!sendRawData(fd, YAARP_SIZE_INT, (uint8_t *)&swappedNumResponses, MSG_MORE))
        {
            DEBUG_PRINT(DEBUG_ERROR, "Send response count failed");
            return KEEP_OPEN;
        }
            
        // Now send the method response(s)
        for (OutputStream::StreamBuflet *sb = os.begin(); sb; sb = sb->getNext())
        {
            if (!sendRawData(fd, sb->validBytes(), sb->begin()))
            {
                DEBUG_PRINT(DEBUG_ERROR, "Send data failed");
                return KEEP_OPEN;
            }
            DEBUG_PRINT(DEBUG_VERBOSE, "Sent %zu byte(s) of data", sb->validBytes());
        }

        PROF_PIN_SET(CONN_PRF_YAARP_EXIT);
        
#ifdef DEBUG_EXECUTION_TIMES
        if (gettimeofday(&timeDone, NULL) == -1)
        {
            perror("gettimeofday()");
        }

        DEBUG_PRINT(DEBUG_VERBOSE, "Network RX time:  %d msec", timeDiffMsec(timeStartRx, timeStartExec));
        DEBUG_PRINT(DEBUG_VERBOSE, "Method exec time: %d msec", timeDiffMsec(timeStartExec, timeStartTx));
        DEBUG_PRINT(DEBUG_VERBOSE, "Network TX time:  %d msec", timeDiffMsec(timeStartTx, timeDone));
#endif

        if (base::resetRequested())
        {
            return SHUT_DOWN_SERVER;
        }

        return KEEP_OPEN;
    }

} // namespace yaap
