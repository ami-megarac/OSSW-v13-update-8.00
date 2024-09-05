/******************************************************************************/
/*! \file bmc/hw/lpcPostCode_hw.cpp bmc:: LpcPostCode HW driver
 *
 * <pre>
 * Copyright (C) 2012-2020, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#include "lpcPostCode_hw.h"
#include <stdio.h>
#include "infrastructure/debug.h"
#include <sys/ioctl.h>

#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#define IOCTL_SMS_ATN               0x1557
#define IOCTL_SETUP_KCS_ADDRESS     0x1554
#define IOCTL_GET_KCS_ISR_STATUS    0x1555
#define IOCTL_GET_POSTCODES         0x1556
#define POST_CODE_SIZE              0
#define POST_BUF_LEN                1024
#define POST_DMA_BUF_LEN            4096

struct st_POSTBuf {
    unsigned short int w_Post_Index;
    unsigned short int w_PostLen;
    unsigned char b_PostCode;
    unsigned char b_PostCodeOld;
    unsigned short int tv_secOffset;    /* seconds, __kernel_time_t */
    unsigned short int tv_msec;     /* microseconds, __kernel_suseconds_t */
};
typedef struct {
    unsigned short int w_Post_Index;
    struct st_POSTBuf a_PostArray[POST_BUF_LEN];
    unsigned short int w_Post_DMA_Index;
    unsigned char a_PostDMAArray[POST_DMA_BUF_LEN];
} St_Snoopy;


bmc::LpcPostCodeHw::LpcPostCodeHw() : m_enabled(false), m_port(0x80), m_passive(false), m_circular(true), m_discardSequential(false), m_autoClear(false)
{
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::enableSet(bool enabled)
{
    m_enabled = enabled;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::enableGet(bool& enabled)
{
    enabled = m_enabled;
    return E_SUCCESS;
}

/***************************************************************/


uint32_t bmc::LpcPostCodeHw::postCodeGet(uint32_t& postCode)
{
    postCode = 0xDEADBEEF;

    DEBUG_PRINT(DEBUG_INFO, " post code = %x ",postCode );
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::portSet(int port)
{
    m_port = port;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::portGet(int& port)
{
    port = m_port;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::passiveSet(bool passive)
{
    m_passive = passive;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::passiveGet(bool& passive)
{
    passive = m_passive;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::connectedGet(bool& connected)
{
    connected = true;
    return E_SUCCESS;
}


/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoDump(vector<hal::postCodeFifoEntry>& fifoContents)
{

    int file_fd;
    int index = 0;
    int offset = 0;

    St_Snoopy snoopy;

    memset(&snoopy, 0, sizeof(St_Snoopy));

    if ((file_fd = open("/dev/kcs0", O_RDWR)) == -1)    {
        DEBUG_PRINT(DEBUG_ERROR, " /dev/kcs0 open failed! ");
        return E_METHOD_ERROR;
    }
    ioctl(file_fd, IOCTL_GET_POSTCODES, &snoopy);
    close(file_fd);

    DEBUG_PRINT(DEBUG_INFO, " Reading %d POST codes ",snoopy.w_Post_Index);

    for(uint32_t i = 0; i <  snoopy.w_Post_Index; i++) {
        if(!i) {
            for(index =0,offset=0; index < snoopy.a_PostArray[0].w_PostLen; index++) {
                hal::postCodeFifoEntry postCodeEntry;

                int byte = snoopy.a_PostDMAArray[index];
                postCodeEntry.data = byte;
                postCodeEntry.statusFlag = 0;
                postCodeEntry.timestamp = snoopy.a_PostArray[i].tv_secOffset*1000 + snoopy.a_PostArray[i].tv_msec;

                postCodeEntry.offset = offset;

                if(snoopy.a_PostArray[i].w_PostLen <= 4)
                offset++;

                DEBUG_PRINT(DEBUG_INFO, "  count = %03d , PostArray[%02d].PostLen = %02d , code %02x",index,(i),snoopy.a_PostArray[0].w_PostLen,byte);

                fifoContents.push_back(postCodeEntry);
            }
        }
        if(i) {
            for(index = snoopy.a_PostArray[i-1].w_PostLen,offset=0; index < snoopy.a_PostArray[i].w_PostLen; index++) {
                hal::postCodeFifoEntry postCodeEntry;

                int byte = snoopy.a_PostDMAArray[index];
                postCodeEntry.data = byte;

                postCodeEntry.statusFlag = 0;
                postCodeEntry.timestamp = snoopy.a_PostArray[i].tv_secOffset*1000 + snoopy.a_PostArray[i].tv_msec;
                postCodeEntry.offset =offset;

                if((snoopy.a_PostArray[(i)].w_PostLen-snoopy.a_PostArray[(i-1)].w_PostLen) <= 4)
                offset++;

                DEBUG_PRINT(DEBUG_INFO, "  count = %03d , PostArray[%02d].PostLen = %02d , code %02x",index,(i),(snoopy.a_PostArray[(i)].w_PostLen-snoopy.a_PostArray[(i-1)].w_PostLen),byte);

                fifoContents.push_back(postCodeEntry);
            }
        }
     }

    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoClear()
{
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoStatusGet(int& entries, bool& overflow, bool& full, bool& empty)
{
    int file_fd;
    int interval_time;
    St_Snoopy snoopy;
    entries = 0;
    memset(&snoopy, 0, sizeof(St_Snoopy));

    if ((file_fd = open("/dev/kcs0", O_RDWR)) == -1)    {
        DEBUG_PRINT(DEBUG_ERROR, " /dev/kcs0 open failed! ");
        return E_METHOD_ERROR;
    }
    ioctl(file_fd, IOCTL_GET_POSTCODES, &snoopy);
    close(file_fd);

    for(uint32_t i = 1; i <  snoopy.w_Post_Index; i++) {
        for(interval_time = snoopy.a_PostArray[i-1].w_PostLen; interval_time < snoopy.a_PostArray[i].w_PostLen; interval_time++) {
        entries++;
        }
    }

    if(entries)
    entries = entries+4;

    overflow = false;
    full = false;
    empty = false;
    DEBUG_PRINT(DEBUG_INFO, " FIFO status %d entries ",entries );
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoStatusClear(void)
{
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoSetupGet(int& entries, bool& circular, bool& discard, bool& autoClear)
{
    entries = 512;
    circular = m_circular;
    discard = m_discardSequential;
    autoClear = m_autoClear;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoCircularSet(bool circular)
{
    m_circular = circular;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoDiscardSequentialSet(bool discard)
{
    m_discardSequential = discard;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::LpcPostCodeHw::fifoAutoClearSet(bool autoClear)
{
    m_autoClear = autoClear;
    return E_SUCCESS;
}
