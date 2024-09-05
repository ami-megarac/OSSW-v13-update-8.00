/**************************************************************************/
/*! \file input_stream.cpp Input stream object implementation.
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

#include <cstring>

#ifdef WIN32
#include <Winsock2.h>
#else
#include <netinet/in.h>
#endif // WIN32

#include "yaarp/input_stream.h"

#ifndef ntohll
#ifdef YAAP_LITTLE_ENDIAN
namespace {
	/**
	 * 64-bit host-to-network conversion.
	 */
	uint64_t ntohll( uint64_t val ) {
		return ((val & 0xFF) << 56) 
			| (((val >> 8) & 0xFF) << 48) 
			| (((val >> 16) & 0xFF) << 40) 
			| (((val >> 24) & 0xFF) << 32) 
			| (((val >> 32) & 0xFF) << 24) 
			| (((val >> 40) & 0xFF) << 16) 
			| (((val >> 48) & 0xFF) << 8) 
			| ((val >> 56) & 0xFF);
	}
} // anonymous namespace
#else // Big endian (no-op).
/**********************************************************************/
/*! \brief 64-bit network-to-host conversion.
 * This is a no-op since we're running on a BE system.  If this is
 * run in a LE system, this will have to be implemented.
 **********************************************************************/
#define ntohll(val) val
#endif // YAAP_LITTLE_ENDIAN
#endif // ntohll

using namespace YAARP;

InputStream::InputStream(uint8_t *buffer, size_t size) :
m_begin(buffer), 
m_currentLocation(buffer),
m_totalBytes(size),
m_remainingBytes(size),
m_error(false),
m_supportUnaligned(UNALIGNED_ACCESSES_SUPPORTED)
{
}

void InputStream::Reset(uint8_t *buffer, size_t size)
{
    m_begin = buffer;
    m_currentLocation = buffer;
    m_totalBytes = size;
    m_remainingBytes = size;
    m_error = false;
    m_supportUnaligned = UNALIGNED_ACCESSES_SUPPORTED;
}

uint8_t InputStream::getByte(void)
{
	const uint32_t size = 1;  // Size of this element in the packet
	uint8_t retval = 0;

	if (m_remainingBytes < size)
	{
		m_error = true;
	}
	if (m_error)
	{
		return retval;
	}
	
	retval = *m_currentLocation;
	m_currentLocation += size;
	m_remainingBytes -= size;

	return retval;
}

uint32_t InputStream::getInt(void)
{
	const uint32_t size = 4;  // Size of this element in the packet
	uint32_t retval = 0;
	
	if (m_remainingBytes < size)
	{
		m_error = true;
	}
	if (m_error)
	{
		return retval;
	}

	if (!((size_t)m_currentLocation & 3) || m_supportUnaligned)
	{
		retval = (uint32_t)ntohl(*(uint32_t *)m_currentLocation);
	}
	else 
	{
		retval = (((uint32_t)m_currentLocation[0] << 24) |
			      ((uint32_t)m_currentLocation[1] << 16) |
			      ((uint32_t)m_currentLocation[2] <<  8) |
				  ((uint32_t)m_currentLocation[3] <<  0));
	}
	
	m_currentLocation += size;
	m_remainingBytes -= size;

	return retval;
}

uint64_t InputStream::getLong(void)
{
	const uint32_t size = 8;  // Size of this element in the packet
	uint64_t retval = 0;

	if (m_remainingBytes < size)
	{
		m_error = true;
	}
	if (m_error)
	{
		return retval;
	}
	

	if (!((size_t)m_currentLocation & 7) || m_supportUnaligned)
	{
		retval = (uint64_t)ntohll(*(uint64_t *)m_currentLocation);
	}
	else
	{
		// Safe for unaligned addresses, but slower
		retval = (((uint64_t)(m_currentLocation[0]) << 56) |
			      ((uint64_t)(m_currentLocation[1]) << 48) |
			      ((uint64_t)(m_currentLocation[2]) << 40) |
			      ((uint64_t)(m_currentLocation[3]) << 32) |
			      ((uint64_t)(m_currentLocation[4]) << 24) |
			      ((uint64_t)(m_currentLocation[5]) << 16) |
			      ((uint64_t)(m_currentLocation[6]) <<  8) |
			      ((uint64_t)(m_currentLocation[7]) <<  0));
	}
	
	m_currentLocation += size;
	m_remainingBytes -= size;

	return retval;
}	

void InputStream::getByteArray(size_t count, uint8_t *buf)
{
	if( m_error )
	{
		return;
	}
	
	if( m_remainingBytes < count )
	{
		m_error = true;
		return;
	}

	memcpy(buf, m_currentLocation, count);
	m_currentLocation += count;
	m_remainingBytes -= count;
}

void InputStream::getIntArray(size_t count, uint32_t *buf)
{
	if (m_error)
	{
		return;
	}
	
	const size_t size = (count << 2);

	if( m_remainingBytes < size )
	{
		m_error = true;
		return;
	}

	memcpy((uint8_t *)buf, m_currentLocation, size);
	m_currentLocation += size;
	m_remainingBytes -= size;

#ifdef LITTLE_ENDIAN	// !!! Should this be YAAP_LITTLE_ENDIAN?  -RDB
	for (size_t i = 0; i < count; i++)
	{
		buf[i] = ntohl(buf[i]);
	}
#endif // LITTLE_ENDIAN
}

void InputStream::getLongArray(size_t count, uint64_t *buf)
{
	if (m_error)
	{
		return;
	}
	
	const size_t size = (count << 3);

	if( m_remainingBytes < size )
	{
		m_error = true;
	}

	memcpy((uint8_t *)buf, m_currentLocation, size);
	m_currentLocation += size;
	m_remainingBytes -= size;

#ifdef LITTLE_ENDIAN	// !!! Should this be YAAP_LITTLE_ENDIAN?  -RDB
	for (size_t i = 0; i < count; i++)
	{
		buf[i] = ntohll(buf[i]);
	}
#endif // LITTLE_ENDIAN
}

uint8_t *InputStream::getRawBytePtr(size_t advBytes)
{
	uint8_t *retval = NULL;

	if (m_remainingBytes < advBytes)
	{
		m_error = true;
	}
	if (m_error)
	{
		return NULL;
	}
	
	retval = m_currentLocation;
	m_currentLocation += advBytes;
	m_remainingBytes -= advBytes;
	
	return retval;
}
