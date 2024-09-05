/**************************************************************************/
/*! \file output_stream.cpp YAARP output stream implementation.
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

#include "yaarp/output_stream.h"

#ifndef htonll
#ifdef YAAP_LITTLE_ENDIAN
namespace {
	/**
	 * 64-bit network-to-host conversion.
	 */
	uint64_t htonll( uint64_t val ) {
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
/***************************************************************************/
/*! 64-bit host-to-network conversion.
 * This is a no-op since we're running on a BE system.  If this is
 * run in a LE system, this will have to be implemented.
 ***************************************************************************/
#define htonll(val) val
#endif // YAAP_LITTLE_ENDIAN
#endif // htonll

using namespace YAARP;

OutputStream::OutputStream() :
	m_error(false),
	m_supportUnaligned(UNALIGNED_ACCESSES_SUPPORTED),
	m_totalSize(0),
	m_blockSizePtr(NULL),
	m_blockSize(0)
{
	m_currBuf = &m_firstBuf;
}

OutputStream::~OutputStream()
{
	StreamBuflet *b = m_firstBuf.getNext();
	while (b)
	{
		StreamBuflet *n = b->getNext();
		delete b;
		b = n;
	}
}

void OutputStream::putByte(uint8_t val)
{
	if( m_error )
	{
		return;
	}
	
	const uint32_t size = sizeof(val);

	if( m_currBuf->spaceLeft() < size )
	{
		allocateNewOutputBuflet();
	}

	*m_currBuf->ptr() = val;
	advance(size);
}

void OutputStream::putInt(uint32_t val)
{
	if( m_error )
	{
		return;
	}

	const uint32_t size = sizeof(val);

	
	if( m_currBuf->spaceLeft() < size )
	{
		allocateNewOutputBuflet();
	}

	uint32_t valToWrite = ntohl(val);

	if( !((size_t)m_currBuf->ptr() & 3) || m_supportUnaligned )
	{
		*(uint32_t *)m_currBuf->ptr() = valToWrite;
	}
	else 
	{
		memcpy(m_currBuf->ptr(), reinterpret_cast<uint8_t *>(&valToWrite), size);
	}

	advance(size);
}

void OutputStream::putLong(uint64_t val)
{
	if( m_error )
	{
		return;
	}

	const uint32_t size = sizeof(val);

	if( m_currBuf->spaceLeft() < size )
	{
		allocateNewOutputBuflet();
	}

	uint64_t valToWrite = htonll(val);

	if( !((size_t)m_currBuf->ptr() & 7) || m_supportUnaligned )
	{
		*(uint64_t *)m_currBuf->ptr() = valToWrite;
	}
	else 
	{
		memcpy(m_currBuf->ptr(), reinterpret_cast<uint8_t *>(&valToWrite), size);
	}

	advance(size);
}

void OutputStream::beginBlockSize()
{
	if( m_error )
	{
		return;
	}

	const uint32_t size = sizeof(uint32_t);

	if( m_currBuf->spaceLeft() < size )
	{
		allocateNewOutputBuflet();
	}
	
	// Keep track of where the size is going to be stored.
	m_blockSizePtr = reinterpret_cast<uint32_t*>(m_currBuf->ptr());
	m_blockSize = 0;

	// Note, we don't call advance(), because we don't want the block size to
	// include itself.
	m_currBuf->advancePtr(size);
	m_totalSize += size;
}

void OutputStream::endBlockSize()
{
	if( m_error )
	{
		return;
	}

	const int size = sizeof(uint32_t);

	uint32_t valToWrite = ntohl(m_blockSize);

	if( !((size_t)m_blockSizePtr & 3) || m_supportUnaligned )
	{
		*m_blockSizePtr = valToWrite;
	}
	else 
	{
		memcpy(m_blockSizePtr, reinterpret_cast<uint8_t *>(&valToWrite), size);
	}

	m_blockSizePtr = NULL;
	m_blockSize = 0;
}

void OutputStream::putByteArray(size_t count, const uint8_t *src)
{
	if (m_error)
	{
		return;
	}

	putArrayCount( count );

	if( m_error ) 
	{
		return;
	}

	while (count)
	{
		int bytesToWrite = ((count < m_currBuf->spaceLeft()) ? count : m_currBuf->spaceLeft());
		
		memcpy(m_currBuf->ptr(), src, bytesToWrite);
		
		count -= bytesToWrite;
		src = &(src[bytesToWrite]);
		
		advance(bytesToWrite);
		
		if (count)
		{
			allocateNewOutputBuflet();
		}
		if (m_error)
		{
			return;
		}
	}
}

void OutputStream::putIntArray(size_t count, const uint32_t *src)
{
	if (m_error)
	{
		return;
	}

	putArrayCount( count );

	if( m_error ) 
	{
		return;
	}

	while (count)
	{
		size_t intsToWrite = ((count < (m_currBuf->spaceLeft()>>2)) ? count : (m_currBuf->spaceLeft()>>2));;
		size_t bytesToWrite = intsToWrite << 2;
		
		memcpy(m_currBuf->ptr(), (uint8_t *)src, bytesToWrite);

		// We swap after we copy, so we don't modify the data passed into us.
#ifdef LITTLE_ENDIAN	// !!! Should this be YAAP_LITTLE_ENDIAN?  -RDB
		uint32_t * bufIntPtr = reinterpret_cast<uint32_t *>(m_currBuf->ptr());
		for (size_t i = 0; i < intsToWrite; i++)
		{
			bufIntPtr[i] = htonl(bufIntPtr[i]);
		}
#endif // LITTLE_ENDIAN
		
		count -= intsToWrite;
		src = &(src[intsToWrite]);

		advance(bytesToWrite);
		
		if (count)
		{
			allocateNewOutputBuflet();
		}
		if (m_error)
		{
			return;
		}
	}
}

void OutputStream::putLongArray(size_t count, const uint64_t *src)
{
	if (m_error)
	{
		return;
	}

	putArrayCount( count );

	if( m_error ) 
	{
		return;
	}

	while (count)
	{
		size_t longsToWrite = ((count < (m_currBuf->spaceLeft()>>3)) ? count : (m_currBuf->spaceLeft()>>3));;
		size_t bytesToWrite = longsToWrite << 3;
		
		memcpy(m_currBuf->ptr(), (uint8_t *)src, bytesToWrite);

		// We swap after we copy, so we don't modify the data passed into us.
#ifdef LITTLE_ENDIAN	// !!! Should this be YAAP_LITTLE_ENDIAN?  -RDB
		uint64_t * bufLongPtr = reinterpret_cast<uint64_t *>(m_currBuf->ptr());
		for (size_t i = 0; i < longsToWrite; i++)
		{
			bufLongPtr[i] = htonll(bufLongPtr[i]);
		}
#endif // LITTLE_ENDIAN
		
		count -= longsToWrite;
		src = &(src[longsToWrite]);
		
		advance(bytesToWrite);
		
		if (count)
		{
			allocateNewOutputBuflet();
		}
		if (m_error)
		{
			return;
		}
	}
}

void OutputStream::allocateNewOutputBuflet(void)
{
	StreamBuflet * next = m_currBuf->addNext();

	if (!next)
	{
		m_error = true;
	}
	else
	{
		m_currBuf = next;
	}
}
