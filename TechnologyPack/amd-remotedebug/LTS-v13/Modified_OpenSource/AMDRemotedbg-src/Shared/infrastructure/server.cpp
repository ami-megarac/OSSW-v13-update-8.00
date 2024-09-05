/**************************************************************************/
/*! \file server.cpp YAARP server implementation.
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

#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include "server.h"
#include "socketHandler.h"
#include "connectionHandler.h"
extern "C"
{
#include "AmdRemDbgCfg.h"
}
using namespace std;

#define REMOTE_DEBUG_PID_FILE "/var/run/AMDRemoteDebugServer.pid"
#define MAX_PID_NUMBER 4194304

namespace yaap {

    int __debugLevel__ = DEBUG_LEVEL;    //!< The current debug level
    const char __dbgType__[] = ".EWIMV"; //!< The single-character log markers
    const char *__malformed__ = "Input stream error";  //!< Save space by having one string

    /***************************************************************************/
    void make_pid_file(void)
    {
        FILE *fp;
        fp = fopen(REMOTE_DEBUG_PID_FILE,"wb+");
        if(fp!=NULL){
            fprintf(fp,"%d",getpid());
            fclose(fp);
        }
    }

    void destory_pid_file(void)
    {
        FILE *fp;
        fp = fopen(REMOTE_DEBUG_PID_FILE,"rb");
        if(fp!=NULL){
            char szBuffer[50];
            fgets(szBuffer,50,fp);
            if(atoi(szBuffer)>0&&atoi(szBuffer)<MAX_PID_NUMBER)
                process_linux_cmd((char*)"kill",szBuffer);

            fclose(fp);
            unlink(REMOTE_DEBUG_PID_FILE);
        }
    }

    int process_linux_cmd(char *Cmd,char* Param)
    {
        char szBuffer[256];
        int s32Ret = 0;
        char strCmd[256];
        sprintf(strCmd,"%s %s",Cmd,Param);
        FILE *fp = popen(strCmd, "r");
        if(fp!=NULL){
            while(!feof(fp)){
                s32Ret = fread(szBuffer,1,sizeof(szBuffer),fp); 
                if(s32Ret > 0){
                    printf("szBuffer = %s\n",szBuffer);
                    break;
                }
            }

            pclose(fp);
        }   
        return s32Ret;
    }
    
    int startServer(int port, int debugLevel)
    {
        int retval;                 // Used for return values 
        int reuseAddr = 1;          // Used to set SO_REUSEADDR on the socket
        int listenFd;               // The listening socket file descriptor 
        struct addrinfo hints;      // Used for the getaddrinfo call 
        struct addrinfo *servinfo;  // Server address info 
        uint8_t *rx_buf_base;       // Pointer to dynamically allocated rx buffer   
        char portStr[6];            // String to store port number

        // Parse options
        if (port > 65535) 
        {
            DEBUG_PRINT(DEBUG_ERROR, "Invalid port number.");
            exit(-1);
        } 
        else
        {
            snprintf(portStr, 6, "%u", port);
        }
        
        if (debugLevel > DEBUG_LAST) 
        {
            DEBUG_PRINT(DEBUG_ERROR, "Invalid debug level (%d).", debugLevel);
            exit(-1);
        } 
        else 
        {
            __debugLevel__ = debugLevel;
        }

        memset(&hints, 0, sizeof(hints));
		// Enable IPV6 socket, it will listen on both, IPV4 and IPV6
        hints.ai_family = AF_INET6;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        rx_buf_base = new (nothrow) uint8_t [RX_BUF_SIZE];
        if (rx_buf_base == NULL)
        {
            DEBUG_PRINT(DEBUG_ERROR, "Unable to allocate receive buffer (%d bytes).", RX_BUF_SIZE);
            exit(-1);
        }

        // Now fire up the server...
        retval = getaddrinfo(NULL, portStr, &hints, &servinfo);

        if (retval != 0)
        {
            fprintf(stderr, "getaddrinfo(): %s\n", gai_strerror(retval));
            freeaddrinfo(servinfo);
            exit(-1);
        }

        // Create the server socket 
        listenFd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

        DEBUG_PRINT(DEBUG_INFO, "Opened socket");

        if (listenFd < 0)
        {
            perror("socket()");
            freeaddrinfo(servinfo);
            exit(-1);
        }

        // set SO_REUSEADDR to rebind without waiting
        setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));

        // Bind the server socket to port YAARP_PORT 

        if (bind(listenFd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
        {
            perror("bind()");
            freeaddrinfo(servinfo);
            exit(-1);
        }

        DEBUG_PRINT(DEBUG_INFO, "Bound socket to port %d", port);

        freeaddrinfo(servinfo);

        // Start the server listening 
        socketHandler sockHndlr(listenFd);

        DEBUG_PRINT(DEBUG_INFO, "Now listening for connections");

        make_pid_file();
        // Now handle requests 
        for (;;)
        {
            yaapConnection *conn = sockHndlr.getReadyConnection();

            DEBUG_PRINT(DEBUG_VERBOSE, "Got data on descriptor %d", conn->fd);

            // Handle the request and send back the response 
            enum ConnectionState state = connectionHandler::processYaarpConnection(conn, rx_buf_base);

            DEBUG_PRINT(DEBUG_VERBOSE, "Finished processing on descriptor %d", conn->fd);

            if (state == CLOSE_CONNECTION)
            {
                sockHndlr.closeConnection(conn);
            }
            else if (state == SHUT_DOWN_SERVER)
            {
                DEBUG_PRINT(DEBUG_INFO, "YAAP server is going down...");
                sockHndlr.closeConnection(conn);
                break;
            }
        }

        delete [] rx_buf_base;

        return 0;
    }

} // namespace yaap
