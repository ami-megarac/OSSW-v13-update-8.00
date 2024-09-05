/******************************************************************************/
/*! \file bmc/hw/jtag_hw.cpp bmc:: JTAG HW driver
 *
 * <pre>
 * Copyright (C) 2011-2021, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

/* Change History

   Chieng-Hwa Lin
   Mar, 13, 2019

   -  Added the debug message to display the ioctl status

 */

#include <cstring>
#include <fstream>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "jtag_hw.h"
#include <stdio.h>
#include <vector>
#include "infrastructure/debug.h"

extern "C"
{
    #include "AmdRemDbgCfg.h"
}

#define  COOL_RESET_MAX_SIZE  (64)

int bmc::JtagHw::default_jtag_xfer_mode = JTAG_XFER_HW_MODE;

uint8_t uCoolResetTbl1P[] = {
0,	8,	0x03,	
1,	6,	0x01,	
0,	8,	0x13,	
1,	2,	0x01,	
1,	11,	0x13,	0x02,	
1,	17,	0x00,	0x88,	0x00,	
1,	26,	0x13,	0x00,	0x10,	0x01,	
1,	62,	0x00,	0x00,	0x00,	0x00,	0x20,	0x00,	0x00,	0x11,	
1,	71,	0x03,	0x00,	0x00,	0x00,	0x00,	0x40,	0x00,	0x00,	0x22,	
1,	66,	0x01,	0x00,	0x00,	0x00,	0x00,	0x02,	0x00,	0x10,	0x01,	
1,	76,	0x9F,	0x05,	0x00,	0x00,	0x00,	0x00,	0x08,	0x00,	0x40,	0x04,	
1,	77,	0x0D,	0x08,	0x00,	0x00,	0x00,	0x00,	0x10,	0x00,	0x80,	0x08,	
1,	77,	0x05,	0x08,	0x00,	0x00,	0x00,	0x00,	0x10,	0x00,	0x80,	0x08,	
1,	77,	0x06,	0x08,	0x00,	0x00,	0x00,	0x00,	0x10,	0x00,	0x80,	0x08,	
1,	77,	0x0A,	0x44,	0x00,	0x00,	0x00,	0x00,	0x00,	0x08,	0x00,	0x0C
};

uint8_t uCoolResetTbl2P[] = {
0,	16,		0x03,	0x03,	
1,	12,		0x41,	0x00,	
0,	16,		0x13,	0x13,	
1,	4,		0x05,	
1,	22,		0x13,	0x9A,	0x10,	
1,	34,		0x00,	0x88,	0x00,	0x10,	0x01,	
1,	52,		0x13,	0x00,	0x10,	0x4D,	0x00,	0x40,	0x04,	
1,	124,	0x00,	0x00,	0x00,	0x00,	0x20,	0x00,	0x00,	0x11,	0x00,	0x00,	0x00,	0x00,	0x08,	0x00,	0x40,	0x04,	
1,	142,	0x03,	0x00,	0x00,	0x00,	0x00,	0x40,	0x00,	0x00,	0xA2,	0x01,	0x00,	0x00,	0x00,	0x00,	0x20,	0x00,	0x00,	0x11,	
1,	132,	0x01,	0x00,	0x00,	0x00,	0x00,	0x02,	0x00,	0x10,	0x05,	0x00,	0x00,	0x00,	0x00,	0x08,	0x00,	0x40,	0x04,	
1,	152,	0x9F,	0x05,	0x00,	0x00,	0x00,	0x00,	0x08,	0x00,	0x40,	0xF4,	0x59,	0x00,	0x00,	0x00,	0x00,	0x80,	0x00,	0x00,	0x44,	
1,	154,	0x0D,	0x08,	0x00,	0x00,	0x00,	0x00,	0x10,	0x00,	0x80,	0xA8,	0x01,	0x01,	0x00,	0x00,	0x00,	0x00,	0x02,	0x00,	0x10,	0x01,	
1,	154,	0x05,	0x08,	0x00,	0x00,	0x00,	0x00,	0x10,	0x00,	0x80,	0xA8,	0x00,	0x01,	0x00,	0x00,	0x00,	0x00,	0x02,	0x00,	0x10,	0x01,	
1,	154,	0x06,	0x08,	0x00,	0x00,	0x00,	0x00,	0x10,	0x00,	0x80,	0xc8,	0x01,	0x01,	0x00,	0x00,	0x00,	0x00,	0x02,	0x00,	0x10,	0x01,	
1,	154,	0x0A,	0x44,	0x00,	0x00,	0x00,	0x00,	0x00,	0x08,	0x00,	0x4C,	0x81,	0x08,	0x00,	0x00,	0x00,	0x00,	0x00,	0x01,	0x80,	0x01,	
};

int printx(const char *msg,unsigned char *x, int len)
{
    int i=0,j=0;
    DEBUG_PRINT(DEBUG_INFO,"\n%s %d bytes\n",msg,len);
    for(i=0;i<len;i=i+16){
        if((len - i) > 15)
            DEBUG_PRINT(DEBUG_INFO, "%02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n", x[i+0],x[i+1],x[i+2],x[i+3],x[i+4],x[i+5],x[i+6],x[i+7],x[i+8],x[i+9],x[i+10],x[i+11],x[i+12],x[i+13],x[i+14],x[i+15]);
        else {
            for(j=0;j<(len-i);j++)
                DEBUG_PRINT(DEBUG_INFO,"%02x ",x[i+j]);
            DEBUG_PRINT(DEBUG_INFO," \n");
        }
    }
    return 0;
}

void bmc::JtagHw::coolreset()
{
    uint8_t inData[COOL_RESET_MAX_SIZE] = {0};
    uint8_t outData[COOL_RESET_MAX_SIZE] = {0};
    uint8_t operation = 0;
    int     bits = 0;
    uint8_t *ptr, *ptrend;
    uint8_t ubits = 0;
    FILE *file;
    enum yaap::hal::jtagOp op;
    char filePath[COOL_RESET_MAX_SIZE] = {0};

	printf( "HDT Cool Reset\n" );
	
    // Enable jtag mux
    HDT_HDTSel_Control( true );
    
    // Reset Tap State
    tapStateSet(yaap::hal::JTAG_STATE_TLR);

    if(is2PSystem == true)
    {
      DEBUG_PRINT(DEBUG_INFO, "2P System\n");
      //strcpy(filePath, "/etc/amd-yaap/2P/coolreset.txt");
      ptr = uCoolResetTbl2P;
      ptrend = ptr + sizeof(uCoolResetTbl2P);
    }
    else
    {
      DEBUG_PRINT(DEBUG_INFO, "1P System\n");
      //strcpy(filePath, "/etc/amd-yaap/1P/coolreset.txt");
      ptr = uCoolResetTbl1P;
      ptrend = ptr + sizeof(uCoolResetTbl1P);
    }

    while(ptr < ptrend)
    {
	  operation = *ptr++;
	  ubits = *ptr++;
	  bits = (int)ubits;
      int bytes = ((bits + 7) >> 3);
      for (int i = 0; i < bytes; i++)
      {
		inData[i] = *ptr++;
      }
      op = (operation == 0)? (yaap::hal::JTAG_OP_SHIFT_IR) : (yaap::hal::JTAG_OP_SHIFT_DR);
      std::string message = "Cool Reset Input Data";
      //printx(message.c_str(), inData, bytes);
      shift(op, inData, outData, bits, 0, 0, yaap::hal::JTAG_STATE_RTI);

      memset (&inData[0], 0, COOL_RESET_MAX_SIZE);
      memset (&outData[0], 0, COOL_RESET_MAX_SIZE);
    }
    
    // Release Jtag mux
    HDT_HDTSel_Control( false );
}

bmc::JtagHw::JtagHw(int driver, uint32_t default_tck_freq, bool use_sw_mode )
    : m_freq(default_tck_freq), m_state(yaap::hal::JTAG_STATE_TLR)
{
    unsigned int rc;

    fd = driver;
#ifdef  YAAP_ENTRY_DEBUG
    DEBUG_PRINT(DEBUG_INFO, "yaap bmc::JtagHw::bmc::JtagHw entry !!!.\n");
#endif
    memset (&xfer, 0, sizeof(struct jtag_xfer));

    mode.feature = JTAG_XFER_MODE;
    mode.mode = ( use_sw_mode ) ? JTAG_XFER_SW_MODE : JTAG_XFER_HW_MODE;
    auto test = ioctl(fd, JTAG_SIOCMODE, &mode);
    if (test < 0) {
      DEBUG_PRINT(DEBUG_INFO, "JtagHw::JTAG_SIOCMODE Failure: 0x%x, errno: %d.\n error message:%s", test, errno, strerror(errno));
    }


    rc = ioctl (fd, JTAG_SIOCFREQ, &m_freq);
    if (rc < 0) {
      DEBUG_PRINT(DEBUG_FATAL, "JtagHw::JTAG_SIOCFREQ Failure: 0x%x, errno: %d.\n error message:%s", rc, errno, strerror(errno));
    }

    unsigned int freq = 0;
    rc = ioctl (fd, JTAG_GIOCFREQ, &freq);
    if (rc < 0) {
      DEBUG_PRINT(DEBUG_FATAL, "JtagHw::JTAG_GIOCFREQ Failure: 0x%x, errno: %d.\n error message:%s", rc, errno, strerror(errno));
    }

    m_freq = freq;
    DEBUG_PRINT(DEBUG_INFO, "JtagHw::set TCK freq is %u\n", freq);

}

/***************************************************************/

uint32_t pulse_tck( int fd, uint32_t cycles ) {

  auto rc = ioctl(fd, JTAG_RUNTEST, cycles);

  if (rc < 0) {
    printf( "pulse_tck: Failure: 0x%x, errno: %d.\n error message:%s", rc, errno, strerror(errno));
    return 1;
  }
  return 0;
}

uint32_t bmc::JtagHw::shift(enum yaap::hal::jtagOp operation, uint8_t *inData, uint8_t *outData, int bits, int preTck, int postTck, enum yaap::hal::jtagState termState)
{
    int test;
    int remain_bits;

    DEBUG_PRINT(DEBUG_INFO, "YAAP IOCTL SHIFT Operation:%d, BITs Count: 0x%x, TermState: 0x%x\n", operation, bits, termState);

    if (bits == 0) {
     return E_SUCCESS;
    }

    remain_bits = bits;

    int bytes = ((remain_bits + 7) >> 3);

    pulse_tck( fd, preTck );

    xfer.type = (operation == yaap::hal::JTAG_OP_SHIFT_DR) ? JTAG_SDR_XFER: JTAG_SIR_XFER;
    xfer.direction = JTAG_READ_WRITE_XFER;
    xfer.tdio = (uint64_t)(inData);
    xfer.padding = 0;
    xfer.length = bits;
    xfer.from = JTAG_STATE_CURRENT;
    xfer.endstate = termState;

    test = ioctl(fd, JTAG_IOCXFER, &xfer);
    if (test < 0) {
      DEBUG_PRINT(DEBUG_INFO, "JtagHw::shift: JTAG_IOCXFER Failure: 0x%x, errno: %d.\n error message:%s", test, errno, strerror(errno));
      return E_SERVER_ERROR;
    }

    pulse_tck( fd, postTck );

    if (outData) {
      std::memcpy( outData, (uint8_t *)xfer.tdio, BYTECOUNT(remain_bits) );
    }

    m_state = termState;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::JtagHw::resetTap(void)
{
#ifdef  YAAP_ENTRY_DEBUG
    DEBUG_PRINT(DEBUG_INFO, "yaap bmc::JtagHw::resetTap entry !!!.\n");
#endif
    int rc;
    tap_state.reset = JTAG_FORCE_RESET;
    tap_state.from = JTAG_STATE_CURRENT;
    tap_state.endstate = JTAG_STATE_TLRESET;
    tap_state.tck = 10;

    rc = ioctl (fd, JTAG_SIOCSTATE, &tap_state);
    if (rc < 0) {
        DEBUG_PRINT(DEBUG_ERROR, "yaap ioctl JTAG_RESET, error: 0x%x.\n", rc);
        return E_SERVER_ERROR;
    }

#ifdef  YAAP_DEBUG
        DEBUG_PRINT(DEBUG_ERROR, "yaap ioctl JTAG_RESET, status: 0x%x.\n", rc);
#endif
    m_state = yaap::hal::JTAG_STATE_TLR;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::JtagHw::tapStateSet(enum yaap::hal::jtagState state)
{
#ifdef  YAAP_DEBUG
    DEBUG_PRINT(DEBUG_INFO, "yaap bmc::JtagHw::tapStateSet entry !!!.\n");
#endif
    DEBUG_PRINT(DEBUG_INFO, "tapState to Set %d ", state);
    if(yaap::hal::JTAG_STATE_TLR == state)
    {
        resetTap();
    }
    m_state = state;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::JtagHw::tapStateGet(enum yaap::hal::jtagState& state)
{
#ifdef  YAAP_ENTRY_DEBUG
    DEBUG_PRINT(DEBUG_INFO, "yaap bmc::JtagHw::tapStateGet entry !!!.\n");
#endif
    state = m_state;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::JtagHw::tckFreqSet(uint32_t& freqHz)
{
        int rc;

#ifdef  YAAP_ENTRY_DEBUG
    DEBUG_PRINT(DEBUG_INFO, "yaap bmc::JtagHw::tckFreqSet entry !!!.\n");
#endif
        DEBUG_PRINT(DEBUG_INFO, "input freq is %u ", freqHz);

        rc = ioctl (fd, JTAG_SIOCFREQ, &freqHz);
        if (rc < 0) {
            DEBUG_PRINT(DEBUG_ERROR, "yaap ioctl JTAG_SIOCFREQ failed, status: 0x%x.\n", rc);
            printf("yaap ioctl JTAG_SIOCFREQ failed, status: 0x%x.\n", rc);
        }
#ifdef  YAAP_DEBUG
        DEBUG_PRINT(DEBUG_INFO, "yaap ioctl JTAG_SIOCFREQ, status: 0x%x.\n", rc);
#endif

        rc = ioctl (fd, JTAG_GIOCFREQ, &freqHz);
        if (rc < 0) {
            DEBUG_PRINT(DEBUG_ERROR, "yaap ioctl JTAG_GIOCFREQ failed, status: 0x%x.\n", rc);
            printf( "yaap ioctl JTAG_GIOCFREQ failed, status: 0x%x.\n", rc);
        }
#ifdef  YAAP_DEBUG
        DEBUG_PRINT(DEBUG_INFO, "yaap ioctl JTAG_GIOCFREQ, status: 0x%x.\n", rc);

    DEBUG_PRINT(DEBUG_INFO, "calculated result freq is %u \n", freqHz);
#endif

    m_freq = freqHz;
    return E_SUCCESS;
}

/***************************************************************/

uint32_t bmc::JtagHw::tckFreqGet(uint32_t& freqHz)
{
    unsigned int freq = 0;
    int test = 0;

#ifdef  YAAP_ENTRY_DEBUG
    DEBUG_PRINT(DEBUG_INFO, "yaap bmc::JtagHw::tckFreqGet entry !!!.\n");
#endif
    test = ioctl (fd, JTAG_GIOCFREQ, &freq);
    if(test <0) {
      DEBUG_PRINT(DEBUG_ERROR, "Get freq failed IOCTL error, status: 0x%x", test);
      return E_METHOD_ERROR;
    }

    DEBUG_PRINT(DEBUG_INFO, "Get freq is %u ", freq);

    m_freq = freq;
    freqHz = m_freq;
    return E_SUCCESS;
}
