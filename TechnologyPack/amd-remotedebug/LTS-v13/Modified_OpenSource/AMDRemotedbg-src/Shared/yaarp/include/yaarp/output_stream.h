/*! \file output_stream.h Main header for YAARP binary RPC output stream.
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

#ifndef YAARP_OUTPUT_STREAM_H
#define YAARP_OUTPUT_STREAM_H

#include <inttypes.h>
#include <cstring>

//! We allocate output buffer chunks of this size
#define YAARP_OUTPUT_BUF_SZ 24480

namespace YAARP
{
	/*! \brief This class encapsulates the encoding functionality of 
	 * an outgoing binary YAARP stream. 
	 */
	class OutputStream 
	{
	public:
		// Pre-declare, so people can read the interface more easily.
		class StreamBuflet;

		/*! \brief Construct new output object.
		 *
		 */
		OutputStream();

		/*! Default destructor
		 *
		 */
		~OutputStream();

		/*! \brief Check for previous errors in the output stream.
		 *
		 * \returns TRUE if an error has been encountered, FALSE otherwise.
		 */
		bool isError(void)
		{ 
			return m_error; 
		}

		/*! \brief Write byte \a val into the stream.
		 *
		 * \param val Byte to write.
		 */
		void putByte(uint8_t val);

		/*! \brief Write 32-bit integer \a val into the stream.
		 *
		 * \param val Integer to write.
		 */
		void putInt (uint32_t val);

		/*! \brief Write 64-bit integer \a val into the stream.
		 *
		 * \param val Long to write.
		 */
		void putLong (uint64_t val);

		/*! \brief Write Double \a val into the stream.
		 *
		 * \param val Double to write.
		 */
		void putDouble(double val)
		{ 
			uint64_t iVal = *(reinterpret_cast<uint64_t*>(&val));
			putLong(iVal); 
		}

        /*! \brief Write a boolean \a val into the stream
         * 
         * \param val Value to write.
         */
        void putBool(bool val) { putByte(val ? 1 : 0); }

		/**
		 * Inserts an integer into the stream which will store the number of 
		 * bytes added to the stream until endBlockSize() is called.
		 *
		 * This can be used when the stream needs to encode the size of a block
		 * of data, but the size is not known before the block is generated.
		 *
		 * @note Calls to beginBlockSize() and endBlockSize() cannot be nested.
		 */
		void beginBlockSize();

		/**
		 * Sets the temporary integer stored in the stream when startBlockSize
		 * was called to the number of bytes stored since that call.
		 *
		 * This can be used when the stream needs to encode the size of a block
		 * of data, but the size is not known before the block is generated.
		 *
		 * @note Calls to beginBlockSize() and endBlockSize() cannot be nested.
		 */
		void endBlockSize();

		/**
		 * Inserts the array count value which should be at the beginning of 
		 * every array.
		 */
		void putArrayCount( size_t count ) {
			putInt(count);
		}

		/*! \brief Write an array of bytes into the stream.
		 *
		 * The array count will be written to the stream, followed
		 * by the data (copied from the source buffer).
		 *
		 * \param count Number of bytes to write
		 * \param src   Pointer to source buffer.
		 */
		void putByteArray(size_t count, const uint8_t *src);

		/*! \brief Write an array of integers into the stream.
		 *
		 * The array count will be written to the stream, followed
		 * by the data (copied from the source buffer and 
		 * byte-swapped as necessary).
		 *
		 * \param count Number of ints to write
		 * \param src   Pointer to source buffer.
		 *
		 * \note This may not work correctly on little-endian systems.  Refer to notes in CPP file for details.
		 */
		void putIntArray(size_t count, const uint32_t *src);

		/*! \brief Write an array of longs into the stream.
		 *
		 * The array count will be written to the stream, followed
		 * by the data (copied from the source buffer and 
		 * byte-swapped as necessary).
		 *
		 * \param count Number of longs to write
		 * \param src   Pointer to source buffer.
		 *
		 * \note This may not work correctly on little-endian systems.  Refer to notes in CPP file for details.
		 */
		void putLongArray(size_t count, const uint64_t *src);

		/*! \brief Write an array of doubles into the stream.
		 *
		 * The array count will be written to the stream, followed
		 * by the data (copied from the source buffer and 
		 * byte-swapped as necessary).
		 *
		 * \param count Number of doubles to write
		 * \param src   Pointer to source buffer.
		 */
		void putDoubleArray(size_t count, const double *src)
		{
			putLongArray(count, (uint64_t *)src);
		}

		/*! \brief Write a string into the stream.
		 *
		 * This is equivalent to putByteArray, using strlen to 
		 * compute the size of the array.  The string will be
		 * copied into the output stream.  The input string
		 * must be NULL-terminated.
		 *
		 * \param str Pointer to string to write.
		 */
		void putString(const char *str)
		{
			putByteArray(strlen(str), (uint8_t *)str);
		}

		/**
		 * Returns the first StreamBuflet in the OutputStream.
		 */
		StreamBuflet * begin() { return &m_firstBuf; }

		/**
		 * Returns the number of bytes stored in the output buffer.
		 */
		size_t getSize() { return m_totalSize; }

		/*! \brief This class is used to manage the output stream data while
		 * minimizing heap allocations and deallocations.
		 */
		class StreamBuflet
		{
		public:
			
			/*! \brief Construct new object.
			 *
			 */
			StreamBuflet() : next(NULL), 
				buf(reinterpret_cast<uint8_t*>(alignedBuf)),
				size(0), idx(0)
			{}

			/*! \brief Get number of valid bytes in the buflet.
			 *
			 * \returns Number of valid bytes in this buflet.
			 */
			size_t validBytes(void) const
			{
				return size;
			}
			
			/*! \brief Check how much space is left in the buflet.
			 *
			 * \returns Number of free bytes remaining in this buflet.
			 */
			size_t spaceLeft(void) const
			{
				// It's possible that the last call to advance() advanced us 
				// past the end of the buffer, so we need to watch for the 
				// negative case, since size_t is unsigned.
				int bytesLeft = YAARP_OUTPUT_BUF_SZ - idx;
				
				if( bytesLeft > 0 )
				{
					return bytesLeft;
				} else {
					return 0;
				}
			}
			
			/*! \brief Get current pointer.
			 *
			 * \returns Current location (read/write) in the buflet.
			 */
			uint8_t *ptr(void)
			{
				return &buf[idx]; 
			}

			/**
			 * Returns the beginning of the buffer.
			 */
			uint8_t * begin()
			{
				return buf;
			}

			/*! \brief Advance the current pointer.
			 *
			 * After writing data to the current pointer, call this method 
			 * to advance the pointer and the count of valid bytes.
			 *
			 * \param bytes Number of bytes to advance the current pointer.
			 */
			void advancePtr(size_t bytes)
			{
				idx += bytes;
				size += bytes;
			}

			/*! \brief Reset current pointer to beginning of the buflet.
			 *
			 */
			void resetPtr(void)
			{
				idx = 0;
			}

			/**
			 * Returns the next StreamBuflet in the output stream, or NULL if 
			 * there is not one.
			 */
			StreamBuflet * getNext() {
				return next;
			}

			/**
			 * Adds a new streamBuflet to the output stream.
			 *
			 * If this buflet already has a "next" buflet, this function 
			 * returns NULL.
			 */
			StreamBuflet * addNext() {
				if( !next ) {
					next = new StreamBuflet();

					size = idx;

					return next;
				} else {
					// It is an error to be asking for a new "next" when we 
					// already have one. This NULL will bubble up to the 
					// OutputStream's m_error flag.
					return NULL;
				}
			}
			
		private:
			StreamBuflet *next;       //!< Pointer to next buflet

			/// The actual buffer.
			/// We declare it as an array of 64-bit ints to get the best 
			/// possible alignment from the uBlaze compiler.
			uint64_t alignedBuf[YAARP_OUTPUT_BUF_SZ/4];

			uint8_t * buf; //!< The 8-bit pointer to the buffer
			int size;                 //!< Number of valid bytes in this buflet
			int idx;                  //!< Current access index
		};
    
	private:

		StreamBuflet m_firstBuf;   //!< First output buflet
		StreamBuflet *m_currBuf;    //!< Output buflet currently accessed

		bool m_error;               //!< Set on parse error
		bool m_supportUnaligned;    //!< Whether HW supports unaligned accesses

		size_t m_totalSize;         //!< The total size of the output buffer.

		uint32_t * m_blockSizePtr;	//!< A pointer to the size variable 
									//!  inserted via startBlockSize, or NULL
									//!  if a block is not being measured.

		uint32_t m_blockSize;		//!< The actual calculated block size. We
									//   store it separately so we don't have 
									//   to constantly worry about the byte 
									//   order or alignment of the version 
									//   stored in the stream itself.

		/*! \brief Allocate a new output buflet.
		 * 
		 * Sets error to FALSE on error.
		 */
		void allocateNewOutputBuflet(void);

		/**
		 * Advances in the stream size bytes.
		 *
		 * This updates the total size, the pointer into the current buffer, 
		 * and the current block size, if we are calculating one.
		 */
		void advance( size_t size ) {
			if( m_blockSizePtr ) {
				m_blockSize += size;
			}

			m_currBuf->advancePtr(size);
			m_totalSize += size;
		}
	};
} // namespace YAARP

#endif // YAARP_OUTPUT_STREAM_H

