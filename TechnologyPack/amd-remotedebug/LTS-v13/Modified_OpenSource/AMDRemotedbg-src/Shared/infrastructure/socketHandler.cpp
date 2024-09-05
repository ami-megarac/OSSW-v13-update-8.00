/**************************************************************************/
/*! \file socketHandler.cpp Socket handler code implementation.
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

#include <cassert>

#include "classes/base.h"
#include "socketHandler.h"
#include "profile.h"
#include "debug.h"

namespace yaap {

int socketHandler::m_connectionTimeout = YAAP_TIMEOUT_SEC + 1;

/**************************************************************************/

socketHandler::socketHandler(int listenFd, int backlog)
    : m_listenFd(listenFd), m_highestFd(listenFd), m_currentTimeout(YAAP_TIMEOUT_SEC + 1), m_prngSeeded(false)
{
    if (listen(m_listenFd, backlog) < 0)
    {
        perror("listen()");
        exit(-1);
    }
      
    // Get the start time, to use in seeding the PRNG.
    if (gettimeofday(&m_startTime, NULL) == -1)
    {
        perror("gettimeofday()");
    }
}

/**************************************************************************/

socketHandler::~socketHandler()
{
    std::vector<yaapConnection *>::iterator iter;

    for (iter = m_connections.begin(); iter != m_connections.end(); iter++)
    {
        close((*iter)->fd);
        m_connections.erase(iter);
        delete *iter;
    }
    close(m_listenFd);
}

/**************************************************************************/

void socketHandler::closeConnection(yaapConnection *conn)
{
    std::vector<yaapConnection *>::iterator iter;

    for (iter = m_connections.begin(); iter != m_connections.end(); )
    {
        if ((*iter) == conn)
        {
            bool activeConnectionAtEntry = (m_connections.size() > 0);
            int fd = conn->fd;
            close(fd);
            m_connections.erase(iter);
            delete conn;
            DEBUG_PRINT(DEBUG_VERBOSE, "Connection %d closed", fd);
            if (activeConnectionAtEntry && (m_connections.size() == 0))
            {
                // Last active connection has closed
                hal::ISystem *sysHal = classes::base::getSystemHal();
                if (sysHal != NULL)
                {
                    /* retval ignored */ sysHal->lockChanged(hal::LOCK_TIMED_OUT);
                    sysHal->configMux(0);
                }
            }
            break;
        }
        else
        {
            iter++;
        }
    }

    m_highestFd = m_listenFd;
    for (iter = m_connections.begin(); iter != m_connections.end(); iter++)
    {
        if ((*iter)->fd > m_highestFd)
        {
            m_highestFd = (*iter)->fd;
        }
    }
}

/**************************************************************************/

yaapConnection *socketHandler::getReadyConnection()
{
    int retval = 0;
    struct timeval tv, now;
    fd_set rfds;
    vector<yaapConnection *>::iterator iter;

    PROF_PIN_SET(SKT_PRF_GET_FD_S);

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(m_listenFd, &rfds);
        for (iter = m_connections.begin(); iter != m_connections.end(); iter++)
        {
            FD_SET((*iter)->fd, &rfds);
        }

        tv.tv_sec = m_currentTimeout;
        tv.tv_usec = 0;
        retval = select(m_highestFd + 1, &rfds, NULL, NULL, &tv);

        if (retval > 0)
        {
            DEBUG_PRINT(DEBUG_VERBOSE, "select() returned %d; %zu active connection(s)", retval, m_connections.size());
        }

        if (gettimeofday(&now, NULL) == -1)
        {
            perror("gettimeofday()");
        }

        if (retval == 0)
        {
            // Connection timeout lapsed with no traffic. 
            processTimeouts(now);
        }
        else
        {
            PROF_PIN_SET(SKT_PRF_NEW_FD_S);
            
            if (FD_ISSET(m_listenFd, &rfds))
            {
                // Process a new connection.
                yaapConnection *newConnection = new yaapConnection;
                memset(newConnection, 0, sizeof(yaapConnection));
                socklen_t addr_size = sizeof(newConnection->peerAddr);
                newConnection->fd = accept(m_listenFd, &newConnection->peerAddr, &addr_size);
                newConnection->lastAccess = now;
                m_connections.push_back(newConnection);
                if (newConnection->fd > m_highestFd)
                {
                    m_highestFd = newConnection->fd;
                }
                DEBUG_PRINT(DEBUG_VERBOSE, "New connection fd %d", newConnection->fd);
            }
            else
            {
                // Process incoming message on existing connection.
                if (!m_prngSeeded) 
                {
                    seedPrng();
                }
                yaapConnection *retval = NULL;

                for (iter = m_connections.begin(); iter != m_connections.end(); )
                {
                    if (FD_ISSET((*iter)->fd, &rfds))
                    {
                        // Move to the end of the list
                        retval = *iter;
                        iter = m_connections.erase(iter);
                        m_connections.push_back(retval);
                        break;
                    }
                    else
                    {
                        iter++;
                    }
                }

                processTimeouts(now);

                // If this assertion fails, then somehow we got out of select without 
                // any of the known existing connections being set in the fd_set.
                assert(retval != NULL);

                PROF_PIN_SET(SKT_PRF_GET_FD_P);
                
                return retval;
            }
        }
    }
}

/**************************************************************************/

void socketHandler::processTimeouts(struct timeval& now)
{
    std::vector<yaapConnection *>::iterator iter;
    struct timeval result;
    bool activeConnectionAtEntry = (m_connections.size() > 0);
    m_currentTimeout = m_connectionTimeout;  // By default, we'll block for the full timeout on next call to select(). 
                                             // We'll decrease this to the timeout point of the next-to-time out socket below.

    for (iter = m_connections.begin(); iter != m_connections.end(); )
    {
        yaapConnection *c = *iter;
        timersub(&now, &c->lastAccess, &result);
        if (result.tv_sec >= m_connectionTimeout)
        {
            DEBUG_PRINT(DEBUG_VERBOSE, "Connection %d timed out; closing", c->fd);
            close(c->fd);
            iter = m_connections.erase(iter);
            delete c;
        }
        else
        {
            iter++;

            // We want the next timeout event to occur at the moment the next-to-time-out socket reaches its timeout
            if ((m_connectionTimeout - result.tv_sec) < m_currentTimeout)
            {
                m_currentTimeout = (m_connectionTimeout - result.tv_sec);
            }
        }
    }

    m_highestFd = m_listenFd;
    for (iter = m_connections.begin(); iter != m_connections.end(); iter++)
    {
        if ((*iter)->fd > m_highestFd)
        {
            m_highestFd = (*iter)->fd;
        }
    }
    
    if (activeConnectionAtEntry && (m_connections.size() == 0))
    {
        // Last active connection has timed out.  We'll notify the HAL that the lock has timed out.
        hal::ISystem *sysHal = classes::base::getSystemHal();
            
        if (sysHal != NULL)
        {
            /* retval ignored */ sysHal->lockChanged(hal::LOCK_TIMED_OUT);
	    DEBUG_PRINT(DEBUG_INFO, "Last active connection has timed out.  We'll notify the HAL that the lock has timed out");
	    sysHal->configMux(0);
        }
    }
}

/**************************************************************************/

void socketHandler::seedPrng(void)
{
    struct timeval now;
    int retval = gettimeofday(&now, NULL);
    if (retval == -1)
    {
        perror("gettimeofday()");
    }
    
    srand((uint32_t)((int)now.tv_usec - (int)m_startTime.tv_usec));
    m_prngSeeded = true;
}

} // namespace yaap
