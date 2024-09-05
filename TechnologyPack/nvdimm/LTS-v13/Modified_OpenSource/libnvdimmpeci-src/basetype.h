/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 *
 ******************************************************************************/

#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_BIT 0x8000000000000000ULL

    //
    // Status codes common to all execution phases
    //
    typedef unsigned long long RETURN_STATUS;

    /*
     * log type :
     * success : 0
     * error :1
     * warning : 2
     * info : 3
     */
    typedef enum
    {
        PMEM_SUCCESS,
        PMEM_ERROR,
        PMEM_WARNING,
        PMEM_INFO
    } log_type;

#define LOG_LEVEL 3
#if 0
#ifdef DEBUG_SUCCESS
#define LOG_LEVEL 0
#elif DEBUG_ERROR
#define LOG_LEVEL 1
#elif DEBUG_WARNING
#define LOG_LEVEL 2
#elif DEBUG_INFO
#define LOG_LEVEL 3
#else
#define NO_DEBUG 1
#endif
#endif

/*
 * DEBUG_PRINT: Prints logs based on compilation flags provided
 * For example:
 *  DEBUG_SUCCESS will print success logs
 *  DEBUG_ERROR will print success and error logs with function and line
 */
#ifndef NO_DEBUG
#define DEBUG_PRINT(log_type, fmt, ...)                                        \
    do                                                                         \
    {                                                                          \
        if (LOG_LEVEL >= log_type)                                             \
        {                                                                      \
            if (log_type == 0)                                                 \
            {                                                                  \
                fprintf(stderr, fmt, ##__VA_ARGS__);                           \
            }                                                                  \
            else                                                               \
            {                                                                  \
                fprintf(stderr, "%s : %d " fmt, __func__, __LINE__,            \
                        ##__VA_ARGS__);                                        \
            }                                                                  \
        }                                                                      \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...)                                                  \
    {}
#endif

/**
 * Returns TRUE if a specified RETURN_STATUS code is an error code.
 *
 * This function returns TRUE if StatusCode has the high bit set.  Otherwise,
 * FALSE is returned.
 *
 * @param  StatusCode    The status code value to evaluate.
 *
 * @retval TRUE          The high bit of StatusCode is set.
 * @retval FALSE         The high bit of StatusCode is clear.
 */
#define RETURN_ERROR(statusCode)                                               \
    (((long long)(unsigned long long)(statusCode)) < 0)

/**
 * Produces a RETURN_STATUS code with the highest bit set.
 *
 * @param  StatusCode    The status code value to convert into a warning code.
 * StatusCode must be in the range 0x00000000..0x7FFFFFFF.
 * @return The value specified by StatusCode with the highest bit set.
 */
#define ENCODE_ERROR(StatusCode) ((RETURN_STATUS)(MAX_BIT | (StatusCode)))

///
/// The operation completed successfully.
///
#define RETURN_SUCCESS 0

///
/// The image failed to load.
///
#define RETURN_LOAD_ERROR ENCODE_ERROR(1)

///
/// The parameter was incorrect.
///
#define RETURN_INVALID_PARAMETER ENCODE_ERROR(2)

///
/// The operation is not supported.
///
#define RETURN_UNSUPPORTED ENCODE_ERROR(3)

///
/// The buffer was not the proper size for the request.
///
#define RETURN_BAD_BUFFER_SIZE ENCODE_ERROR(4)

///
/// The buffer was not large enough to hold the requested data.
/// The required buffer size is returned in the appropriate
/// parameter when this error occurs.
///
#define RETURN_BUFFER_TOO_SMALL ENCODE_ERROR(5)

///
/// The physical device reported an error while attempting the
/// operation.
///
#define RETURN_DEVICE_ERROR ENCODE_ERROR(7)

///
/// The resource has run out.
///
#define RETURN_OUT_OF_RESOURCES ENCODE_ERROR(9)

///
/// The item was not found.
///
#define RETURN_NOT_FOUND ENCODE_ERROR(14)

///
/// A timeout time expired.
///
#define RETURN_TIMEOUT ENCODE_ERROR(18)

///
/// The end of the file was reached.
///
#define RETURN_END_OF_FILE ENCODE_ERROR(31)

///
/// Boolean true value.  UEFI Specification defines this value to be 1,
/// but this form is more portable.
///
#ifndef TRUE
#define TRUE ((bool)(1 == 1))
#endif

///
/// Boolean false value.  UEFI Specification defines this value to be 0,
/// but this form is more portable.
///
#ifndef FALSE
#define FALSE ((bool)(0 == 1))
#endif

/**
 * BCD to Two decimal conversion
 */
#define BCD_TO_TWO_DEC(BUFF) ((((BUFF) >> 4) * 10) + ((BUFF)&0xF))

#ifdef __cplusplus
}
#endif

#endif
