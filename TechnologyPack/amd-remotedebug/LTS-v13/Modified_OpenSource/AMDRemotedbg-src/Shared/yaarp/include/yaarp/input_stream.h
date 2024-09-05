/*! \file input_stream.h Main header for YAARP binary RPC protocol input stream.
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
 */

#ifndef YAARP_INPUT_STREAM_H
#define YAARP_INPUT_STREAM_H

#include <inttypes.h>
#include <cstring>

namespace YAARP
{

	/*! \brief This class encapsulates the parsing functionality of 
	 * an incoming binary YAARP stream. 
	 */
	class InputStream 
	{
	public:

		/*! \brief Construct new input object from the given buffer.
		 *
		 * \param buffer Beginning of the input stream buffer.
		 * \param size Size of the input stream buffer.
		 */
		InputStream(uint8_t *buffer = NULL, size_t size = 0);

        /*! \brief Reset the input object to operate from the given buffer.
         *
         * This is equivalent to constructing a new InputStream object, but allows
         * you to avoid the cost of allocating a new object by reusing an existing one.
         * 
         * \param buffer Beginning of the input stream buffer.
		 * \param size Size of the input stream buffer.
         */
        void Reset(uint8_t *buffer, size_t size);

		/*! \brief Check for previous errors in the input stream.
		 *
		 * \returns TRUE if an error has been encountered, FALSE otherwise.
		 */
		bool isError(void) const
		{ 
			return m_error; 
		}

		/*! \brief Read a byte value from the stream and return it.
		 *
		 * \returns The value read out of the stream (0 if the 
		 *          stream is in error).
		 */
		uint8_t getByte(void);

		/*! \brief Read a 32-bit integer value from the stream and return it.
		 *
		 * \returns The value read out of the stream (0 if the 
		 *          stream is in error).
		 */
		uint32_t getInt(void);

		/*! \brief Read a 64-bit integer value from the stream and return it.
		 *
		 * \returns The value read out of the stream (0 if the 
		 *          stream is in error).
		 */
		uint64_t getLong(void);

		/*! \brief Read a double-precision float value from the stream.
		 *
		 * \returns The value read out of the stream (0 if the 
		 *          stream is in error).
		 */
		double getDouble(void) 
		{ 
			uint64_t l = getLong();

			return *(reinterpret_cast<double*>(&l));
		}

		/*! \brief Read an array count from the stream and return it.
		 *
		 * The proper and safe way to extract an array from an
		 * input stream is:
		 *
		 *  -# Call this method to get the number of elements.
		 *  -# Allocate a buffer of sufficient size.
		 *  -# Call the appropriate getArray method to extract the data;
		 *     it will be copied into the provided buffer.
		 *
		 * \returns The value read out of the stream (0 if the 
		 *          stream is in error); this represents an array count.
		 */
		size_t getArrayCount(void) 
		{ 
			return (size_t)getInt(); 
		}

		/*! \brief Read an array of bytes out of the stream.
		 *
		 * Reads \a count bytes starting at the current location in the
		 * input stream, and copies them into the buffer starting at \a buf.
		 *
		 * \param count Number of bytes to read out.
		 * \param buf Address to copy the bytes to.
		 */
		void getByteArray(size_t count, uint8_t *buf);

		/*! \brief Read an array of integers out of the stream.
		 *
		 * Reads \a count integers starting at the current location in the
		 * input stream, and copies them into the buffer starting at \a buf.
		 * Assumes that \a buf has proper alignment.
		 *
		 * \param count Number of integers to read out.
		 * \param buf Address to copy the integers to.
		 *
		 * \note This may not work correctly on little-endian systems.  Refer to notes in CPP file for details.
		 */
		void getIntArray(size_t count, uint32_t *buf);

		/*! \brief Read an array of longs out of the stream.
		 *
		 * Reads \a count longs starting at the current location in the
		 * input stream, and copies them into the buffer starting at \a buf.
		 *
		 * \param count Number of longs to read out.
		 * \param buf Address to copy the longs to.
		 *
		 * \note This may not work correctly on little-endian systems.  Refer to notes in CPP file for details.
		 */
		void getLongArray(size_t count, uint64_t *buf);

		/*! \brief Read an array of doubles out of the stream.
		 *
		 * Reads \a count doubles starting at the current location in the
		 * input stream, and copies them into the buffer starting at \a buf.
		 *
		 * \param count Number of doubles to read out.
		 * \param buf Address to copy the longs to.
		 */
		void getDoubleArray(size_t count, double *buf)
		{ 
			getLongArray(count, (uint64_t *)buf); 
		}

		/*! \brief Get a raw byte pointer & advance stream.
		 *
		 * Returns the current pointer in the stream, and advances the stream
		 * by \a advBytes.  If there isn't enough data left in the stream
		 * to advance \a advBytes, it sets the error flag, does not advance
		 * the stream at all, and returns NULL.
		 *
		 * Use this method carefully.  There is no copy made of the data;
		 * The caller must be sure that they don't try to use the
		 * value after the input stream object is gone.
		 *
		 * \param advBytes Number of bytes by which to advance the input stream.
		 *
		 * \returns Raw pointer to the current location in the stream.
		 */
		uint8_t *getRawBytePtr(size_t advBytes);

		/**
		 * Returns the number of bytes in the input stream.
		 */
		size_t getSize() { return m_totalBytes; }

		/**
		 * Returns the number of bytes which have not been read from the input
		 * stream.
		 */
		size_t getBytesRemaining() { return m_remainingBytes; }

		/**
		 * Reads and discards the requested number of bytes from the input stream.
		 *
		 * @param count The number of bytes to discard
		 *
		 * @returns The number of bytes actually read
		 */
		void discardBytes(size_t count)
		{
			getRawBytePtr(count);
		}
			
    
	private:

		uint8_t *m_begin;           //!< Pointer to beginning of buffer
		uint8_t *m_currentLocation; //!< Pointer to current location in buffer
		size_t m_totalBytes;      //!< Size of buffer (bytes)
		size_t m_remainingBytes;  //!< Bytes remaining in buffer
		bool m_error;               //!< Set on parse error
		bool m_supportUnaligned;    //!< Whether HW supports unaligned accesses
	};
} // namespace YAARP

#endif // YAARP_INPUT_STREAM_H

