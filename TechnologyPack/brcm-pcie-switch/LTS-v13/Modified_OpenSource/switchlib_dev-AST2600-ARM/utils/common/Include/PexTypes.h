/*******************************************************************************
* @file PexTypes.h
*
* @brief This file defines the basic data types
*
******************************************************************************/
 
/*******************************************************************************
* Copyright 2021 Broadcom Inc.
*
* Redistribution and use in source and binary forms, with or without modification, 
* are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice, 
*    this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, 
*    this list of conditions and the following disclaimer in the documentation 
*    and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its contributors 
*    may be used to endorse or promote products derived from this software without 
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/
#ifndef __PEX_TYPES_H
#define __PEX_TYPES_H

 /** Check the endianess macro */
#if defined(PEXNT2_LINUX)
#include <endian.h>
#if (__BYTE_ORDER == __BIG_ENDIAN)
    #define CPU_BIG_ENDIAN      1
#endif
#endif

#if defined(PEXNT2_WDM_DRIVER)
    #include <wdm.h>            // WDM Driver types
#endif

#if defined(PEXNT2_NT_DRIVER)
    #include <ntddk.h>          // NT Kernel Mode Driver (ie PEXNT2 Service)
#endif

#if defined(PEXNT2_MSWINDOWS)
    #if !defined(PEXNT2_DRIVER)
        #include <wtypes.h>     // Windows application level types
    #endif
#endif

// Must be placed before <linux/types.h> to prevent compile errors
#if defined(PEXNT2_LINUX) && !defined(PEXNT2_LINUX_DRIVER)
    #include <memory.h>         // To automatically add mem*() set of functions
#endif

#if defined(PEXNT2_LINUX) || defined(PEXNT2_LINUX_DRIVER)
    #include <linux/types.h>    // Linux types
#endif

#if defined(PEXNT2_LINUX)
    #include <limits.h>         // For MAX_SCHEDULE_TIMEOUT in Linux applications
#endif


#ifdef __cplusplus
extern "C" {
#endif



/*******************************************
 *   Linux Application Level Definitions
 ******************************************/
#if defined(PEXNT2_LINUX)
    typedef __s8                  S8;
    typedef __u8                  U8;
    typedef __s16                 S16;
    typedef __u16                 U16;
    typedef __s32                 S32;
    typedef __u32                 U32;
    typedef __s64                 S64;
    typedef __u64                 U64;
    #define PEX_SIZE_64           8
    typedef signed long           PEX_INT_PTR;        // For 32/64-bit code compatability
    typedef unsigned long         PEX_UINT_PTR;
    typedef __u8*                 PU8;

    #if !defined(MAX_SCHEDULE_TIMEOUT)
        #define MAX_SCHEDULE_TIMEOUT    LONG_MAX
    #endif
#endif



/*******************************************
 *    Linux Kernel Level Definitions
 ******************************************/
#if defined(PEXNT2_LINUX_DRIVER)
    typedef s8                    S8;
    typedef u8                    U8;
    typedef s16                   S16;
    typedef u16                   U16;
    typedef s32                   S32;
    typedef u32                   U32;
    typedef s64                   S64;
    typedef u64                   U64;
    #define PEX_SIZE_64           8
    typedef signed long           PEX_INT_PTR;        // For 32/64-bit code compatability
    typedef unsigned long         PEX_UINT_PTR;
    typedef u8*                   PU8;
#endif



/*******************************************
 *      Windows Type Definitions
 ******************************************/
#if defined(PEXNT2_MSWINDOWS)
    typedef signed char           S8;
    typedef unsigned char         U8;
    typedef signed short          S16;
    typedef unsigned short        U16;
    typedef signed long           S32;
    typedef unsigned long         U32;
    typedef signed _int64         S64;
    typedef unsigned _int64       U64;
    typedef INT_PTR               PEX_INT_PTR;        // For 32/64-bit code compatability
    typedef UINT_PTR              PEX_UINT_PTR;
    #define PEX_SIZE_64           8

    #if defined(_DEBUG)
        #define PEX_DEBUG
    #endif
#endif

/*******************************************
 *        DOS Type Definitions
 ******************************************/
#if defined(PEXNT2_DOS)
    typedef signed char           S8;
    typedef unsigned char         U8;
    typedef signed short          S16;
    typedef unsigned short        U16;
    typedef signed long           S32;
    typedef unsigned long         U32;
    typedef signed long long      S64;
    typedef unsigned long long    U64;
    typedef S32                   PEX_INT_PTR;        // For 32/64-bit code compatability
    typedef U32                   PEX_UINT_PTR;
    #define PEX_SIZE_64           8

    #if !defined(_far)
        #define _far
    #endif
#endif



/*******************************************
 *    Volatile Basic Type Definitions
 ******************************************/
typedef volatile S8           VS8;
typedef volatile U8           VU8;
typedef volatile S16          VS16;
typedef volatile U16          VU16;
typedef volatile S32          VS32;
typedef volatile U32          VU32;
typedef volatile S64          VS64;
typedef volatile U64          VU64;

#if defined(PEXNT2_MSWINDOWS)
    #define Pex_sscanf                  sscanf_s
    #define Pex_sprintf                 sprintf_s
    #define Pex_vsprintf                vsprintf_s
    #define Pex_strncpy                 strncpy_s
    #define Pex_strcpy                  strcpy_s
    #define Pex_strcat                  strcat_s
    #define Pex_strtok                  strtok_s
    #define Pex_strdup                  _strdup
    #define Pex_strnlen                 strnlen_s

#elif defined(PEXNT2_LINUX)
    #define Pex_sscanf                          sscanf
    #define Pex_sprintf(str, sz, fmt, args...)  sprintf(str, fmt, args)
    #define Pex_vsprintf(str, sz, fmt, args...) vsprintf(str, fmt, args)
    #define Pex_strncpy(dst, dstsz, src, cnt)   strncpy(dst, src, cnt)
    #define Pex_strcpy(dst, dstsz, src)         strcpy(dst, src)
    #define Pex_strcat(dst, sz, src)            strcat(dst, src)
    #define Pex_strtok(str, delim, ctx)         strtok(str, delim)
    #define Pex_strdup                          strdup
    #define Pex_strnlen                         strnlen

#endif

#ifdef __cplusplus
}
#endif

#endif
