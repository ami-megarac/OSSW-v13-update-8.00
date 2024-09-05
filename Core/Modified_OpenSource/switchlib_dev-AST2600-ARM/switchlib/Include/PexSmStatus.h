/*******************************************************************************
* @file PexSmStatus.h
*
* @brief This file defines all the status code of the SM APIs
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

#ifndef __PEXSM_STATUS_H
#define __PEXSM_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************
 *             Definitions
 *****************************************/

// Return type
typedef U32    PEXSM_STATUS;

// API Return Code Values
typedef enum _PEXSM_STATUS_CODE
{
    PEXSM_STATUS_SUCCESS = 0,                 /**< Success */
    PEXSM_STATUS_FAILED,                      /**< Failed */
    PEXSM_STATUS_UNSUPPORTED,                 /**< Unsupported */
    PEXSM_STATUS_NULL_PARAM,                  /**< Function parameter was NULL */
    PEXSM_STATUS_INVALID_ADDR,                /**< Invalid address */
    PEXSM_STATUS_INVALID_DATA,                /**< Invalid data */
    PEXSM_STATUS_NO_RESOURCE,                 /**< No resource available */
    PEXSM_STATUS_TIMEOUT,                     /**< Timeout */
    PEXSM_STATUS_IN_USE,                      /**< In use */
    PEXSM_STATUS_DISABLED,                    /**< Disabled */
    PEXSM_STATUS_PENDING,                     /**< Pending */
    PEXSM_STATUS_NOT_FOUND,                   /**< Not found */
    PEXSM_STATUS_INVALID_STATE,               /**< Invalid state */
    PEXSM_STATUS_INVALID_PORT,                /**< Invalid Port */
    PEXSM_STATUS_INVALID_OBJECT,              /**< Invalid Object */
    PEXSM_STATUS_BUFFER_TOO_SMALL,            /**< Supplied buffer too small for result */
    PEXSM_STATUS_INVALID_SIZE,                /**< Invalid Size */
    PEXSM_STATUS_RETRY,                       /**< Retry */
    PEXSM_STATUS_ABORT,                       /**< Abort */

    PEXSM_STATUS_NO_DRIVER = 128,             /**< No Driver Found */
    PEXSM_STATUS_NO_DEVICES,                  /**< No Switch Found */
    PEXSM_STATUS_INVALID_SLID,                /**< Invalid SLID */
    PEXSM_STATUS_INVALID_DS_PORT,             /**< Invalid Downstream port */
    PEXSM_STATUS_INVALID_HS_PORT,             /**< Invalid Host port */
    PEXSM_STATUS_INVALID_STN,                 /**< Invalid Station */
    PEXSM_STATUS_STN_NOT_CFG,                 /**< Station not configured */
    PEXSM_STATUS_ADDRTYPE_UNSUPPORTED,        /**< Invalid address type */
    PEXSM_STATUS_MEM_ALLOC_FAILED,            /**< Memory allocation Failed */
    PEXSM_STATUS_SL_LOAD_FAILED,              /**< Failed to load shim library */
    PEXSM_STATUS_SL_SYM_ADDR_FAILED,          /**< Failed to get the symbol */
    PEXSM_STATUS_INCOMP_DRV_INTFC_VER,        /**< Incompatible Driver interface version */
    PEXSM_STATUS_RECV_OVERRUN,                /**< Recieved data more than expected */
    PEXSM_STATUS_RECV_ERROR,                  /**< Receive Error */
    PEXSM_STATUS_SEND_ERROR,                  /**< Send Error */
    PEXSM_STATUS_LENGTH_EXCEEDED,             /**< Length Exceeded */
    PEXSM_STATUS_INVALID_PARAM,               /**< Invalid parameter */
    PEXSM_STATUS_DRV_FAULT,                   /**< Driver is in fault state */
    PEXSM_STATUS_NOT_READY,                   /**< The bus or driver is not ready */
    PEXSM_STATUS_UNKNOWN,                      /**< Unknown Error */

    PEXSM_STATUS_DEFAULT = 0xFFFF             /**< Default initialized value */
} PEXSM_STATUS_CODE;

#ifdef __cplusplus
}
#endif


#endif
