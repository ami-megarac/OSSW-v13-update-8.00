/*******************************************************************************
* @file mctp_7E_drvr_intfc.h
*
* @brief Defines the MCTP 7E driver interfaces and status completion codes.
*
* Backward compatibility with earlier versions of this interface is not guarenteed. 
* The responsibility of the compatibility check is with MCTP 7E Driver. The MCTP 7E driver 
* is expected to strictly check for the interface compatibility and if incompatible 
* terminate the registration. 
*
* @note
*
* Following lists the supported data flow directions
*
* [in]     - [MCTP Driver -->]  : Populated and sent by MCTP driver to agent
*
* [out]    - [MCTP Driver <--]  : Populated and sent by agent to the MCTP driver
*
* [in,out] - [MCTP Driver <-->] : Populated and sent by agent to the MCTP driver and vice versa 
*
* Notations:
* 
*  pciID : A U32 value to uniquely identify an PCIe device (an MCTP endpoint) within a system.  
*
*          MCTP 7E driver may provide 
*          - MCTP-EID or 
*          - A unique ID created by self for an MCTP endpoint within a system. 
*            As MCTP EID is used to route MCTP messages to a endpoint, the mapping of 
*            this unique ID to MCTP-EID will be maintained by the MCTP 7E driver.  
*
*  7E    : MCTP Message Type 0x7E, Vendor Defined - PCI 
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

#ifndef _MCTP_7E_DRVR_INTFC_H
#define _MCTP_7E_DRVR_INTFC_H

/**
 * @brief defines the completion codes for send/receive
 */
typedef enum {
    MCTP_7E_CC_SUCCESS                            =0x00,  /**< Send/Receive completed sucessfully */
    MCTP_7E_CC_INVALID_PARAMS                     =0x01,  /**< Input parameter has invalid value (such as null function ptr) */
    MCTP_7E_CC_INVALID_PCIID                      =0x02,  /**< Input pciID is not being handled by this agent */
    MCTP_7E_CC_LENGTH_EXCEEDED                    =0x03,  /**< Response length exceeded buffer size */
    MCTP_7E_CC_TIMEOUT                            =0x04,  /**< Response not received within specified timeout */
    MCTP_7E_CC_PCIID_BUSY                         =0x05,  /**< Attempt to send to an pciID that is already being waited on */
    MCTP_7E_CC_SEND_ERROR                         =0x06,  /**< MCTP message send failed/did not complete */
    MCTP_7E_CC_RECV_OVERRUN                       =0x07,  /**< The BMC's receiver was overrun and some data has been dropped */
    MCTP_7E_CC_DRVR_FAULT                         =0x08,  /**< MCTP Driver is not currently functional enough to send/recv */
    MCTP_7E_CC_RECV_ERROR                         =0x09,  /**< MCTP message receive failed/did not complete */
    MCTP_7E_CC_INCOMPATIBLE_DRVR_INTFC_VERSION    =0x0A,  /**< Incompatible MCTP 7E Driver interface (INTFC) version */	
    MCTP_7E_CC_MAX
} MCTP_7E_CC;


/**
 * @brief Version of the MCTP 7E driver interface in this file.
 */
 #define MCTP_7E_DRIVER_INTFC_VERSION        (1)
 
/**
 * @brief defines the MCTP send/recv 7E message command structure
 *
 * @note
 * Used in mctp7ESendRecvMsg(...), initiated by the agent to send a request and optionally get a response
 *
 *        [in]      - reqSizeInBytes
 *        [in,out]  - rspSizeInBytes
 *        [out]     - ptrReqData
 *        [in]      - ptrRspData
 *
 * Used in ptrMctp7ERecvAsyncMsgFunc(...), initiated by MCTP 7E driver to send a request and optionally get a response
 *
 *        [out]     - reqSizeInBytes
 *        [in,out]  - rspSizeInBytes
 *        [in]      - ptrReqData
 *        [out]     - ptrRspData 
 */
typedef struct {
    U32    reqSizeInBytes;        /**< Number of bytes of ptrReqData sent */
    U32    rspSizeInBytes;        /**< Number of bytes of ptrRspData. If the response is less than  
                                       the requested "rspSizeInBytes" bytes then this field should 
                                       be modified to indicate the valid bytes copied to ptrRspData */
    PU8    ptrReqData;            /**< Pointer to MCTP_7E request message payload */
    PU8    ptrRspData;            /**< Pointer to MCTP_7E response message payload */
} MCTP_7E_SEND_RECV_MSG_CMD, *PTR_MCTP_7E_SEND_RECV_MSG_CMD;

/**
 * @brief Send/Recv message
 *
 * Send an MCTP 7E request to an MCTP endpoint and optionally get a response.
 *
 * The message is sent as Message Type 0x7E, Vendor Defined - PCI.  
 * The request/response payload is placed after the MCTP Transport header.
 *
 * The MCTP driver will encapsulate and fragment the MCTP 7E request into MCTP packets based on the MTU.
 * It will re-assemble the response and remove these headers (Medium (I2C/PCIe), Transport(MCTP))
 * before returning the MCTP 7E response.
 *
 * The caller is blocked until the response is received or a timeout occurs. Timeout is the total timeout to wait for a response. 
 * timeout = (maxMCTP7EMessageResponseTimeoutInSec (MCTP Endpoint supported Value) + transmission time (added by MCTP driver as required)).

 * Only one send can be active at a time for a given destination pciID.
 *
 * If the ptrRspData pointer or rspSizeInBytes is zero, no response is expected/waited for.
 *
 * MCTP 7E driver should decide when a message should be retried, the maximum retry count and the retry interval. 
 *
 * @param  [out]        pciID                    - Unique identifier to identify an PCIe device (MCTP Endpoint) within a system 
 * @param  [in,out]     ptrMctp7ESendRecvMsgCmd  - Pointer to MCTP 7E SEND/RECV Message command structure 
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
extern MCTP_7E_CC mctp7ESendRecvMsg(U32 pciID, PTR_MCTP_7E_SEND_RECV_MSG_CMD ptrMctp7ESendRecvMsgCmd);

/**
 * @brief Receive asynchronous message callback
 *
 * Process an incoming message request and optionally send a response
 *
 * Only one callback is made at a time per registered agent. Any additional requests received for
 * that agent before the callback completes will be queued in the driver until the current callback completes.
 *
 * If the rspSizeInBytes is zero, no response is sent. If the rspSizeInBytes is non zero then the agent will 
 * copy the response to ptrRspData and update the rspSizeInBytes to indicate the actual bytes copied to ptrRspData.
 *
 * The MCTP 7E driver is responsible for allocating the response buffer (ptrRspData) of the negotiated  
 * "maxMctpMsgSizeInBytes" size and the driver is responsible for freeing up this memory.
 *
 * @param  [in]        pciID                    - Unique identifier to identify an PCIe device (MCTP Endpoint) within a system  
 * @param  [in,out]    ptrMctp7ESendRecvMsgCmd  - Pointer to MCTP 7E SEND/RECV Message command structure. 
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
typedef MCTP_7E_CC (*ptrMctp7ERecvAsyncMsgFunc)(U32 pciID, PTR_MCTP_7E_SEND_RECV_MSG_CMD ptrMctp7ESendRecvMsgCmd);

/**
 * @brief defines the standard MCTP 7E API array header structure (8 bytes)
 */
typedef struct {
    U16    numberOfArrayElements;           /**< Populated number of array elements in the
                                                 underlying data structure */
    U16    sizeOfEachElementInBytes;        /**< Size of each array element in bytes. To find
                                                 out the next array element this many bytes
                                                 needs to be skipped */
    U8     reserved[4];                     /**< Padding and reserved for future use */						
} MCTP_7E_ARRAY_HEADER;

/**
 * @brief defines the 4 part PCIe device IDs from PCI Config space.
 */
typedef struct {
    U16    supportBitmap;          /**< Indicates which of the 4 part PCIe Device IDs is valid 
                                        Bit 0 : deviceID, Bit 1 : vendorID, Bit 2 : SubDeviceID, Bit 3 : subVendorID 
                                        Bit value 0 - not valid 
                                        Bit value 1 - valid */ 	
    U16    deviceID;               /**< PCI Device ID */
    U16    vendorID;               /**< PCI Vendor ID */
    U16    subDeviceID;            /**< PCI Subsystem Device ID */
    U16    subVendorID;            /**< PCI Subsystem Vendor ID */
    U8     pad[6];                 /**< Pad for alignment */	
} MCTP_7E_PCI_CONFIG_SPACE_INFO;

/**
 * @brief defines the MCTP Vendor Defined Message Support command response. 
 * Vendor ID information is based on the capability set (vendorIDSet) selected. 
 */
typedef struct {
    U8      VendorIDSet;             /**< Vendor ID capability set selector */
    U8      VendorIDFormat;          /**< Numeric value that indicates the definition space and format of the VendorIDData */
    U16     VendorDefinedData;       /**< Identify a particular command set type. The VendorIDSet defines what is in this data */	
    union {
        struct {
#ifdef __MCTP_7E_BIG_ENDIAN__
            U32 reserved:16;         /**< Reserved for future use */
            U32 VendorIDData16:16;   /**< 16 bits VendorIDData */
#else 
            U32 VendorIDData16:16;   /**< 16 bits VendorIDData */
            U32 reserved:16;         /**< Reserved for future use */
#endif 
        }bits;                       /**< 32 bits structure */ 
        U32    VendorIDData32;       /**< 32 bits Vendor ID */
    }VendorIDData;                   /**< ID of the vendor that defined the capability set */
} MCTP_7E_VENDOR_ID_INFO;

/**
 * @brief defines the list of the vendor specific capability sets obtained via MCTP control message
 * "Get Vendor defined Message Support"
 *
 *        [in]       - arrayHeader 
 *        [in]       - _mctp7EVendorIDInfoArray 
 */
typedef struct {
    MCTP_7E_ARRAY_HEADER arrayHeader;                  /**< Detailed information about this array */
    MCTP_7E_VENDOR_ID_INFO _mctp7EVendorIDInfoArray;   /**< Vendor ID Info array */
} MCTP_7E_VENDOR_ID_INFO_LIST, *PTR_MCTP_7E_VENDOR_ID_INFO_LIST;

/**
 * @brief defines the MCTP 7E endpoint info sturcture
 *
 * Description of an MCTP 7E endpoint with a particular MCTP 7E pciID. This is an both direction structure. 
 *
 *        [in]    - pciID 
 *        [out]   - supported
 *        [in]    - mctp7EPciConfigSpaceInfo
 *        [in]    - ptrMctp7EVendorIDInfoList (variable length)
 * 
 * @note
 * MCTP 7E driver must populate both mctp7EPciConfigSpaceInfo and ptrMctp7EVendorIDInfoList info if possible. 
 * Atleast one of the information must be populated.
 * 
 * This is a mandatory info for an agent, used to determine if it will support the endpoints. 
 * If both the information is not available the agent will set the "supported" flag to zero, 
 * an indication that the device will not be managed by the agent. 
 *
 * If MCTP 7E driver has access to config space it may populate MCTP_7E_PCI_CONFIG_SPACE_INFO else 
 * must provide the list of the vendor specific capability sets "ptrMctp7EVendorIDInfoList" obtained via 
 * MCTP control message "Get Vendor Defined Message Support"
 *  
 */
typedef struct {
    U32    pciID;                                                 /**< Unique ID to identify an PCIe device (MCTP Endpoint) within a system */	
    U8     supported;                                             /**< The agent will set this flag to indicate it will manage the device.
                                                                       0 - Not supported, 1 - Supported */
    U8     pad[3];                                                /**< Pad for alignment */
    MCTP_7E_PCI_CONFIG_SPACE_INFO mctp7EPciConfigSpaceInfo;       /**< 4 part PCIe device IDs obtained from PCI config space */
    PTR_MCTP_7E_VENDOR_ID_INFO_LIST ptrMctp7EVendorIDInfoList;    /**< List of the vendor specific capability sets obtained via  
                                                                       MCTP control message "Get Vendor defined Message Support" */
}MCTP_7E_ENDPOINT_INFO;

/**
 * @brief defines the MCTP 7E Endpoint Info list
 *
 *        [in]       - arrayHeader 
 *        [in,out]   - _endpointInfoArray 
 */
typedef struct {
    MCTP_7E_ARRAY_HEADER arrayHeader;              /**< Detailed information about this array */
    MCTP_7E_ENDPOINT_INFO _endpointInfoArray;      /**< Endpoint information array */
} MCTP_7E_ENDPOINT_INFO_LIST, *PTR_MCTP_7E_ENDPOINT_INFO_LIST;

/**
 * @brief Endpoint list callback
 * 
 * The list is all responding MCTP 7E endpoints from vendor defined by vendor ID registered by this agent. 
 * The agent identifies which ones it will handle and update the supported bit in respective pciID 
 * MCTP_7E_ENDPOINT_INFO entry in MCTP_7E_ENDPOINT_INFO_LIST.
 *
 * The input MCTP_7E_ENDPOINT_INFO_LIST size is  
 * (sizeof(MCTP_7E_ENDPOINT_INFO_LIST) - sizeof(MCTP_7E_ARRAY_HEADER.sizeOfEachElementInBytes)) + 
 * (MCTP_7E_ARRAY_HEADER.numberOfArrayElements * MCTP_7E_ARRAY_HEADER.sizeOfEachElementInBytes)
 *
 * When the system powers off or resets, this is called with zero MCTP_7E_ARRAY_HEADER.numberOfArrayElements.
 * After the reset when PCIe enumeration & MCTP EID and pciID assignment are complete, this is called with
 * a non-zero MCTP_7E_ARRAY_HEADER.numberOfArrayElements if any MCTP 7E capable endpoints are found.
 *
 * @param  [in,out]    ptrMctp7EEndpointInfoList   - Pointer to the list of the MCTP 7E endpoints  
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
typedef MCTP_7E_CC (*ptrMctp7EEndpointListFunc)(PTR_MCTP_7E_ENDPOINT_INFO_LIST ptrMctp7EEndpointInfoList);

/**
 * @brief defines the MCTP_7E register command structure. This is an both direction structure.
 *
 *        [in]     - registerID 
 *        [out]    - vendorID
 *        [out]    - mctp7EDrvrIntfcVersion
 *        [in]     - mctp7EDrvrIntfcVersionInUse
 *        [out]    - ptrListFunc
 *        [out]    - ptrRecvFunc
 */
typedef struct {
    U32    registerID;                                  /**< Unique registration identifier given by MCTP 7E driver */
    U16    vendorID;                                    /**< PCI vendor ID */
    U16    mctp7EDrvrIntfcVersion;                      /**< MCTP 7E driver interface version in this file. This version will be incremented 
                                                             only for inoperable cases like broken backward compatibilty */
    U16    mctp7EDrvrIntfcVersionInUse;                 /**< Given by MCTP 7E driver. This is the MCTP 7E driver interface version currently 
                                                             being used by MCTP 7E driver */
    U16    pad[3];                                      /**< Pad for alignment */
    ptrMctp7EEndpointListFunc    ptrListFunc;           /**< Endpoint list callback function*/
    ptrMctp7ERecvAsyncMsgFunc    ptrRecvFunc;           /**< Receive message callback function*/
} MCTP_7E_REGISTER_CMD, *PTR_MCTP_7E_REGISTER_CMD;

/**
 * @brief Register agent
 *
 * Called once only to register the callbacks for an agent.
 *
 * @param  [in,out]    ptrMctp7ERegisterCmd   - Pointer to MCTP 7E driver register command 
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
extern MCTP_7E_CC mctp7ERegister(PTR_MCTP_7E_REGISTER_CMD ptrMctp7ERegisterCmd);

/**
 * @brief Unregister agent
 *
 * Unregister the calbacks registered for an agent.
 * 
 * @param  [out]    registerID   - MCTP 7E Driver registration ID 
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
extern MCTP_7E_CC mctp7EUnregister(U32 registerID);

/**
 * @brief Modifiable OOB platform parameters 
 *
 *        [out]    - maxMctpMsgSizeInBytes
 *        [out]    - maxMctpPktPayloadSizeInBytes
 *        [out]    - maxMctpMsgRspTimeoutInSec
 */
typedef struct {
    U16    maxMctpMsgSizeInBytes;               /**< Max MCTP 7E Message Size */
    U16    maxMctpPktPayloadSizeInBytes;        /**< Max MCTP Packet Payload Size */
    U16    maxMctpMsgRspTimeoutInSec;           /**< Max MCTP 7E Message Response Timeout */
    U16    pad;                                 /**< Pad for alignment */
} MCTP_7E_OOB_PARAM_SET_CMD, *PTR_MCTP_7E_OOB_PARAM_SET_CMD;

/**
 * @brief Set OOB platform parameters
 *
 * Called by the registered agent to send negotiated OOB platform parameters to MCTP 7E driver for an particular MCTP endpoint.
 *
 * @param  [out]    pciID                     - Unique identifier to identify an PCIe device (MCTP Endpoint) within a system  
 * @param  [out]    ptrSetMctp7EOobParamCmd   - Pointer to MCTP 7E OOB paramters set command structure  
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
extern MCTP_7E_CC mctp7EOobParamsSet(U32 pciID, PTR_MCTP_7E_OOB_PARAM_SET_CMD ptrSetMctp7EOobParamCmd);

/**
 * @brief OOB platform parameters to read from MCTP 7E driver  
 *
 *        [in]    - maxMctpMsgSizeInBytes
 *        [in]    - asyncCmdMode
 *        [in]    - asyncEventMode
 *        [in]    - eventPollIntervalInSec
 *        [in]    - transportMedium
 */
typedef struct {
    U16   maxMctpMsgSizeInBytes;             /**< Max MCTP 7E Message Size */
    U8    asyncCmdMode;                      /**< Support Async Flow mechanism for commands initiated by BMC
                                                  0 - Polling, 1 - Async */
    U8    asyncEventMode;                    /**< Support Async Flow mechanism for events
                                                  0 - Polling, 1 - Async */
    U8    eventPollIntervalInSec;            /**< Event polling interval. This is applicable only when "asyncEventMode" is set to Polling */
    U8    transportMedium;                   /**< 0 - I2C, 1 - PCIe */   
    U8    pad[2];                            /**< Pad for alignment */
} MCTP_7E_OOB_PARAM_GET_CMD, *PTR_MCTP_7E_OOB_PARAM_GET_CMD;

/**
 * @brief Get OOB platform parameters from MCTP 7E driver
 *
 * Called by the registered agent to read OOB platform parameters supported by MCTP 7E driver for an particular MCTP endpoint. 
 *
 * @param  [out]    pciID                     - Unique identifier to identify an PCIe device (MCTP Endpoint) within a system 
 * @param  [in]     ptrGetMctp7EOobParamCmd   - Pointer to MCTP 7E OOB paramters get command structure   
 *
 * @return
 *  Generic MCTP driver 7E message transmision completion codes, refer @ref MCTP_7E_CC
 */
extern MCTP_7E_CC mctp7EOobParamsGet(U32 pciID, PTR_MCTP_7E_OOB_PARAM_GET_CMD ptrGetMctp7EOobParamCmd);

#endif // _MCTP_7E_DRVR_INTFC_H
