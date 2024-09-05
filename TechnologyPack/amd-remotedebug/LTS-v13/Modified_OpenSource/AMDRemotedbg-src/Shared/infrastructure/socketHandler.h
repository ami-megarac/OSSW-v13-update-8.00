/**************************************************************************/
/*! \file socketHandler.h Socket handler header file for Linux.
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

#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <inttypes.h>

using namespace std;

namespace yaap {

//! A connection descriptor
typedef struct
{
    int fd;                       //!< Connection FD
    struct timeval lastAccess;    //!< Time of the last access
    struct sockaddr peerAddr;     //!< Peer address information
} yaapConnection;

/**************************************************************************/
/*! This class controls the YAAP socket. 
 * 
 * It manages the queueing of connections, connection timeouts, etc.
 **************************************************************************/
class socketHandler
{
 public:

    /*! Constructor.
     *
     * Construct new object to listen on \a listenFd, with an accept queue
     * depth of \a backlog.
     * 
     * \param listenFd File descriptor on which to listen
     * \param backlog  Maximum length of the incoming connection queue (see man 2 listen)
     */
    socketHandler(int listenFd, int backlog = 25);

    /*! Destructor.
     *
     * Closes all remaining connections.
     */
    ~socketHandler();

    /*! Set the connection timeout to \a timeout seconds.
     *
     * This is the timeout between messages.  If this much time elapses between
     * messages, the connection is closed.  The default is 1800 seconds (30 minutes).
     * 
     * \param timeout Connection timeout, in seconds
     */
    static void setConnectionTimeout(int timeout)
    {
        m_connectionTimeout = timeout;
    }
    
    /*! Close a connection.
     *
     * \param conn Pointer to the connection descriptor for the connection to be closed.
     */
    void closeConnection(yaapConnection *conn);

    /*! Get number of active connections.
     *
     * \return Number of active connections
     */
    int activeConnection(void) 
    {
        return m_connections.size();
    }

    /*! Get a readable connection.
     *
     * This method blocks until one or more FD's are readable, then it returns
     * the FD from that set which least-recently received any communication.
     * The peer address information is filled in \a peer_addr.
     * 
     * Connection timeout is also handled in this method.
     * 
     * \return Pointer to the connection descriptor for the ready-to-receive connection.
     */
    yaapConnection *getReadyConnection(void);

 private:

    /*! Process timeouts on all active sockets
     *
     * This method will cycle through all of the active sockets, checking for those
     * which may have timed out (i.e. gone too long without any communication).  Timed
     * out connections will be removed from the queue of active sockets.
     */
    void processTimeouts(struct timeval& now);

    /*! Seed the PRNG.
     *
     * We use the time elapsed between startup and the arrival of the first
     * message to seed the system PRNG.
     */
    void seedPrng(void);

    vector<yaapConnection *> m_connections; //!< List of active connections
    int m_listenFd;                   //!< The listening FD
    int m_highestFd;                  //!< The highest FD among the listen FD and all connection FDs
    int m_currentTimeout;             //!< The amount of time until the next connection timeout
    bool m_prngSeeded;                //!< Flag indicating whether the PRNG has been seeded
    struct timeval m_startTime;       //!< The startup time (used for PRNG seeding)

    static int m_connectionTimeout;   //!< The connection timeout, in seconds
};

} // namespace yaap

#endif // SOCKETHANDLER_H
