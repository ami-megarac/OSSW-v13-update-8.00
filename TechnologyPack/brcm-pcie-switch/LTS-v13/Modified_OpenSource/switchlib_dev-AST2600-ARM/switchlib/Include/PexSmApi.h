/*******************************************************************************
* @file PexSmApi.h
*
* @brief This file provides the header files and functions for Switch Lib API
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PexTypes.h"
#include "PexSmDefs.h"
#include "PexSmStatus.h"
#include "smApiDefs.h"

#ifdef __cplusplus
extern   "C" {
#endif


/**
 * Definitions
 */
#ifndef EXPORT
    #define EXPORT
#endif

 /**
 * @brief PexMctp_Init Initialize the shim library and get store the MCTP endpoint details.
 *
 * @param Nill
 *
 * @retval PEXSM_STATUS_SUCCESS The function returned successfully.
 * @retval PEXSM_STATUS_NO_RESOURCE Memory allocation failed.
 */
PEXSM_STATUS EXPORT
PexSm_Init();


/**
* @brief PexSm_SLIDGetCount function provides the count of SLID's.
*        SLID is a handle to a specific switch over a single specific
*        transport and is a parameter supplied the SwitchLib APIs
*        to denote what interface and switch the request is sent to.
*
* @param [in,out] PtrSLIDCount Pointer to a U32 array that holds the SLID's
*                              on successful return. Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_NO_DEVICES No switchlib capable devices discovered.
*/
PEXSM_STATUS EXPORT
PexSm_SLIDGetCount(
    U32     *PtrSLIDCount
    );

/**
* @brief PexSm_SLIDGetList function provides the list of SLID's.
*        SLID is a handle to a specific switch over a single specific
*        transport and is a parameter supplied the SwitchLib APIs
*        to denote what interface and switch the request is sent to.
*
* @param [in]     ListSize Size of SLID List (in bytes).
* @param [in,out] PtrSLIDList Pointer to a U32 array that holds the
*                             SLID's on successful return. 
*                             Initialize the array with number of
*                             of SLID's that needs to be copied.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_NULL_PARAM One or more parameters in NULL.
* @retval PEXSM_STATUS_NO_DEVICES No switchlib capable devices discovered.
*/
PEXSM_STATUS EXPORT
PexSm_SLIDGetList(
    U32     ListSize,
    U32     *PtrSLIDList
    );

/**
* @brief PexSm_SwitchLibGetVer function provides the Switchlib, Driver and API ver
*
* @param [in,out] PtrVer Pointer to SM_VERSION data structure that holds the
*                        Switchlib, Driver and API version on successful return.
*                        Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_NULL_PARAM One or more parameters in NULL.
*/
PEXSM_STATUS EXPORT
PexSm_SwitchLibGetVer(
    SM_VERSION      *PtrVer
    );

/**
* @brief PexSm_SwitchGetFwVer function gives the firmware version for the given 
*        SLID and switch ID.
*
* @param [in]     SLID Is a handle to a specific switch over a single
*                 specific transport.
* @param [in]     SwitchId Id of the switch for which the info is requested.
* @param [in,out] PtrFwVer Pointer to SM_FW_VERSION data structure that holds
*                          the firmware version on successful return.
*                          Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_SwitchGetFwVer(
    U32             SLID,
    PMG_SWITCH_ID   SwitchId,
    SM_FW_VERSION   *PtrFwVer
    );

/**
* @brief PexSm_SwitchGetMode function provides the current switch operating mode for
*        the given SLID and switch ID.
*
* @param [in]     SLID Is a handle to a specific switch over a single specific transport.
* @param [in]     SwitchId Id of the switch for which the info is requested.
* @param [in,out] PtrSwMode Pointer to U32 that holds switch mode on
*                           successful return. Initializaiton is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_SwitchGetMode(
    U32             SLID,
    PMG_SWITCH_ID   SwitchId,
    U8              *PtrSwMode
    );

/**
* @brief PexSm_SwitchGetAttr function provides the switch attributes for
*        the given SLID and switch ID.
*
* @param [in]     SLID Is a handle to a specific switch over a single specific transport.
* @param [in]     SwitchId Id of the switch for which the info is requested.
* @param [in,out] PtrSwAttr Pointer to SM_SWITCH_ATTR data structure that
*                           holds the switch attributes on successful return.
*                           Initializaiton is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_SwitchGetAttr(
    U32             SLID,
    PMG_SWITCH_ID   SwitchId,
    SM_SWITCH_ATTR  *PtrSwAttr
    );

/**
* @brief PexSm_PortGetType function provides the port type of the physical port in the
*        given port GID.
*
* @param [in]     SLID Is a handle to a specific switch over a single specific transport.
* @param [in]     PortGID Unique number to identify the physical port in the topology.
* @param [in,out] PortType Pointer to a U32 that holds the port type on successful return.
*                          Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_ADDRTYPE_UNSUPPORTED The address type is not supported
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_PORT The given port number is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
* @retval PEXSM_STATUS_DISABLED The given port number is disabled.
*/
PEXSM_STATUS EXPORT
PexSm_PortGetType(
    U32             SLID,
    SM_PORT_GID     PortGID,
    U8              *PtrPortType
    );

/**
* @brief PexSm_PortGetAttr function provides the port attributes of the physical port
*        in the given port GID.
*
* @param [in]     SLID Is a handle to a specific switch over a single specific transport.
* @param [in]     PortGID Unique number to identify the physical port in the topology.
* @param [in,out] PtrPortAttr Pointer to SM_PORT_ATTR data structure that holds the port
*                             attributes on successful return.
*                             Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_ADDRTYPE_UNSUPPORTED The address type is not supported
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_PORT The given port number is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
* @retval PEXSM_STATUS_DISABLED The given port number is disabled.
*/
PEXSM_STATUS EXPORT
PexSm_PortGetAttr(
    U32             SLID,
    SM_PORT_GID     PortGID,
    SM_PORT_ATTR    *PtrPortAttr
    );

/**
* @brief PexSm_PortGetErrCounters function provides the error counters of the physical port in
*        the given port GID.
*
* @param [in]     SLID Is a handle to a specific switch over a single specific transport.
* @param [in]     PortGID Unique number to identify port in the topology.
* @param [in,out] PtrPortErrCntrs Pointer to SM_PORT_ERR_CNTRS data structure that holds
*                                 the port error counters on successful return.
*                                 Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_ADDRTYPE_UNSUPPORTED The address type is not supported
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_PORT The given port number is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
* @retval PEXSM_STATUS_DISABLED The given port number is disabled.
*/
PEXSM_STATUS EXPORT
PexSm_PortGetErrCounters(
    U32                 SLID,
    SM_PORT_GID         PortGID,
    SM_PORT_ERR_CNTRS   *PtrPortErrCntrs
    );

/**
* @brief PexSm_PortResetErrCounters function resets the error counters of the physical port in
*        the given port GID.
*
* @param [in] SLID Is a handle to a specific switch over a single specific transport.
* @param [in] PortGID Unique number to identify the physial port in the topology.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_ADDRTYPE_UNSUPPORTED The address type is not supported
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_PORT The given port number is not valid.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
* @retval PEXSM_STATUS_DISABLED The given port number is disabled.
*/
PEXSM_STATUS EXPORT
PexSm_PortResetErrCounters(
    U32                 SLID,
    SM_PORT_GID         PortGID
    );

/**
* @brief PexSm_StationGetRAMErrCounters function provides all the RAM error counters for the given 
*        station number.
*
* @param [in]     SLID Is a handle to a specific switch over a single specific transport.
* @param [in]     SwitchId Id of the switch for which the info is requested.
* @param [in]     StnNum Station number whose RAM error counter values are requested.
* @param [in,out] PtrRAMErrCntrs Pointer to SM_RAM_ERR_CNTRS that holds the RAM error counters on
*                                successful return. Initialization is not required.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_STN The given station number is not valid.
* @retval PEXSM_STATUS_STN_NOT_CFG The given station number is not configured.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_StationGetRAMErrCounters(
    U32                 SLID,
    PMG_SWITCH_ID       SwitchId,
    U8                  StnNum,
    SM_RAM_ERR_CNTRS    *PtrRAMErrCntrs
    );

/**
* @brief PexSm_StationResetRAMErrCounters function resets all the RAM error counters for the given
*        station number.
*
* @param [in] SLID Is a handle to a specific switch over a single specific transport.
* @param [in] SwitchId Id of the switch for which the info is requested.
* @param [in] StnNum Station number whose RAM error counter values to be reset.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_STN The given station number is not valid.
* @retval PEXSM_STATUS_STN_NOT_CFG The given station number is not configured.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_StationResetRAMErrCounters(
    U32                 SLID,
    PMG_SWITCH_ID       SwitchId,
    U8                  StnNum
    );

/**
* @brief PexSm_PortDsUnassign function un-assigns a given downstream port from the host port
*        it is currently assigned to. Any endpoints under the downstream
*        port that are shared to the host synthetic tree will be un-assigned.
*
* @param [in] SLID Is a handle to a specific switch over a single specific transport.
* @param [in] PortDsGID Unique number to identify the physical downstream port
*                       or an endpoint in the topology.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_ADDRTYPE_UNSUPPORTED The address type is not supported.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_PORT The given port number is not valid.
* @retval PEXSM_STATUS_INVALID_DS_PORT The given port is not a valid downstream port.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_PortDsUnassign(
    U32                 SLID,
    SM_PORT_GID         PortDsGID
    );

/**
* @brief PexSm_PortDsAssign function assigns a given downstream port to a host port.
*        All the endpoints under the downstream port will be shared to
*        host. If the downstream port is already assigned to another
*        host, it must first be released.
*
* @param [in] SLID Is a handle to a specific switch over a single specific transport.
* @param [in] PortDsGID Unique number to identify the physical downstream
*                       port or an endpoint in the topology.
* @param [in] PortHostGID Unique number to identify the physical host port 
*                         in the topology.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_ADDRTYPE_UNSUPPORTED The address type is not supported.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_INVALID_PORT The given port number is invalid.
* @retval PEXSM_STATUS_INVALID_DS_PORT The given port is not a valid downstream port.
* @retval PEXSM_STATUS_INVALID_HS_PORT The given port is not a valid host port.
* @retval PEXSM_STATUS_IN_USE The given DS port is already assigned to another host port.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_PortDsAssign(
    U32                 SLID,
    SM_PORT_GID         PortDsGID,
    SM_PORT_GID         PortHostGID
    );

/**
* @brief PexSm_SwitchGetTemp Get Switch (Chip) Temperature in degree celsius
*
* @param [in] SLID Is a handle to a specific switch over a single specific transport.
* @param [in] PtrChipTemp Pointer to S32, on success it holds chip temperature
*                       in degree celsius.
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_INVALID_SLID The given SLID is not valid.
* @retval PEXSM_STATUS_FAILED The function returned failure.
* @retval PEXSM_STATUS_UNSUPPORTED Command not supported.
*/
PEXSM_STATUS EXPORT
PexSm_SwitchGetTemp(
    U32                 SLID,
    PMG_SWITCH_ID       SwitchId,
    S32                 *PtrChipTemp
    );

/**
* @brief PexSm_Exit function cleans up heap allocated objects within the
*        library.
*
* @param Nill
*
* @retval PEXSM_STATUS_SUCCESS The function returned successfully.
* @retval PEXSM_STATUS_FAILED The function returned failure.
*/
PEXSM_STATUS EXPORT
PexSm_Exit();

#ifdef __cplusplus
    }
#endif

