/*******************************************************************************
* @file smApiDefs.h
*
* @brief This file provides the System Manager API related data structures.
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

#if !defined SM_API_DEFN_H
#define SM_API_DEFN_H

/** @addtogroup smApi SwitchLib API data types and definitions
 *
 *  Provides structures and definitions for SwitchLib API support.
 *
 *  @{ */

/*
 *  Include files
 */
#include "PexSmDefs.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*****************************************
 * STRUCTURES and DEFINITIONS
 ****************************************/

/**
 * All API Response structures need to be packed
 * Save current pragma state to be restored after all structure definitions
 */
#pragma pack(4)

/** Unique number to identify port/device in the topology */

typedef union _SM_DEV_IDEN_TYPE
{
    U8      port;
    struct
    {
#ifdef CPU_BIG_ENDIAN
        U8      Dev : 5;                /**< Device number */
        U8      Func : 3;               /**< Function number */
#else
        U8      Func : 3;               /**< Function number */
        U8      Dev : 5;                /**< Device number */
#endif
    }FD;
}SM_DEV_IDEN_TYPE;


typedef struct _SM_PORT_GID
{
#ifdef CPU_BIG_ENDIAN
    union
    {
        U32     word;
        struct
        {
            SM_DEV_IDEN_TYPE    DevIden;
            U8                  Bus;                    /**< Bus number */
            U8                  Domain;                 /**< Domain number */
            U8                  AddressType : 1;        /**< Port Type identifier, if 1 - the identifier is based on switch, domain and BDF number */
                                                        /**< If 0 - the identifier is based on switch, domain and port number */
            U8                  SwitchNum : 7;          /**< Switch number within domain */
        }GID;
    }u;
#else
    union
    {
        U32     word;
        struct
        {
            U8                  SwitchNum : 7;          /**< Switch number within domain */
            U8                  AddressType : 1;        /**< Port Type identifier, if 1 - the identifier is based on switch, domain and BDF number */
                                                        /**< If 0 - the identifier is based on switch, domain and port number */
            U8                  Domain;                 /**< Domain number */
            U8                  Bus;                    /**< Bus number */
            SM_DEV_IDEN_TYPE    DevIden;
        }GID;
    }u;
#endif
}SM_PORT_GID;

/**
 * Get Firmware Version
 *
 * API command 'Get Firmware Version' response data structure
 */
typedef struct _SM_FW_VERSION
{
    union
    {
        /** Structured access to firmware version. */
        struct
        {
            U8                  Dev;                    /**< Development Version */
            U8                  Unit;                   /**< Unit Version */
            U8                  Minor;                  /**< Minor Version */
            U8                  Major;                  /**< Major Version */
        } Field;
        /** Word access to firmware version. */
        U32                     Word;                   /**< Switch FW version in U32 */
    } FwVer;

    union
    {
        /** Structured access to API version used in firmware */
        struct
        {
            U8                  Reserved[2];            /**< Reserved to DWORD Align */
            U8                  Minor;                  /**< Minor Version */
            U8                  Major;                  /**< Major Version */
        } Field;
        /** Word access to API version */
        U32                     Word;                   /**< FW API version in U32 */
    } SmApiVer;
} SM_FW_VERSION,
 *PTR_SM_FW_VERSION;

/**
 * Get Switch Mode
 *
 * API 'Get Switch Mode' response data structure
 */
typedef struct _SM_SWITCH_MODE
{
    PMG_SWITCH_ID               SwitchId;               /**< Switch Id */
    U8                          SwMode;                 /**< Switch mode type (PMG_SW_MODE) */
    U8                          Reserved;               /**< Reserved */
} SM_SWITCH_MODE,
*PTR_SM_SWITCH_MODE;

/**
 * Get Switch Attributes
 *
 * API 'Get Switch Attributes' response data structure
 */
typedef struct _SM_SWITCH_ATTR
{
    PMG_SWITCH_PROP             SwProp;                 /**< Switch Properties */
    PMG_PORT_MASK_T             ( ActivePortMask );     /**< Mask of enabled ports */
    PMG_PORT_MASK_T             ( HostPortMask );       /**< Mask of ports set as host type */
    PMG_PORT_MASK_T             ( FabricPortMask );     /**< Mask of ports set as fabric type */
    PMG_PORT_MASK_T             ( DsPortMask );         /**< Mask of ports set as downstream type */
} SM_SWITCH_ATTR,
 *PTR_SM_SWITCH_ATTR;

/**
 * Get Port Attributes
 *
 * API 'Get Port Attributes' response data structure
 */
typedef struct _SM_PORT_ATTR
{
    PMG_SWITCH_ID               SwitchId;               /**< Switch Id to which the Port belongs */
    U8                          Reserved;               /**< Reserved */
    U8                          ClockMode;              /**< Clock Mode for Port */
    U8                          PortNum;                /**< Port Number whose attributes are requested */
    U8                          Stn;                    /**< Station port is in */
    U8                          StnPort;                /**< Port number in station */
    U8                          Type;                   /**< Port type */
    U32                         GID;                    /**< Unique Global ID of port */
    PCIE_PORT_PROP              Pcie;                   /**< PCIe properties of port */
    union
    {                                                   /**< Type specific attributes */
        U64                     Default;                /**< For Invalid Port Types or Disabled Ports */
        struct                                          /**< DS Port Attributes */
        {
            U8                  GblBusSec;              /**< Global DS Secondary Bus */
            U8                  GblBusSub;              /**< Global DS Subordinate Bus */
            U16                 DsDevCount;             /**< Number of Downstream devices present */
            U8                  HostPortNum;            /**< Host port number assigned to, if any */
            U8                  Reserved[3];            /**< Reserved */
        } DsPort;

        struct                                          /**< Host Port Attributes */
        {
            U8                  GblPciBus;              /**< Assigned Global Bus */
            U8                  VSlotCount;             /**< Number of Virtual Slots Assigned */
            U16                 HostFlags;              /**< Host Flags: TWC, MPT, GDMA, etc */
            U8                  Reserved[4];            /**< Reserved */
        } HostPort;

        struct                                          /**< Fabric Port Attributes */
        {
            U8                  Valid;                  /**< Is Valid Connection */
            U8                  PeerPort;               /**< Peer switch connected port number */
            PMG_SWITCH_ID       PeerID;                 /**< Switch ID of connected peer switch */
            U8                  Reserved[4];            /**< Reserved */
        } FabricPort;
    } u;
} SM_PORT_ATTR,
 *PTR_SM_PORT_ATTR;

/**
 * Get Port Error Counters
 *
 * API 'Get Port Error Counters' response data structure
 */
typedef struct _SM_PORT_ERR_CNTRS
{
    PMG_SWITCH_ID               SwitchId;                               /**< Switch Id to which the Port belongs */
    U8                          Stn;                                    /**< Station number of port device is connected to */
    U8                          StnPort;                                /**< Station port number of port device is connected to */
    U8                          PortNum;                                /**< Port Number in switch whose error counters are requested */
    U8                          Reserved[3];                            /**< Reserved */
    U32                         GID;                                    /**< Unique Global ID of port */
    SM_ERR_COUNTER              ErrCounterInfo[MAX_PORT_ERR_CNTRS];     /**< Array of SM_ERR_COUNTER */
} SM_PORT_ERR_CNTRS,
 *PTR_SM_PORT_ERR_CNTRS;

/**
 * Get RAM Error Counters for a given station
 *
 *
 * API 'Get RAM Error Counters' response data structure
 */
typedef struct _SM_RAM_ERR_CNTRS
{
    PMG_SWITCH_ID               SwitchId;                               /**< Switch Id to which the station belongs */
    U16                         Reserved;                               /**< Reserved */
    SM_ERR_COUNTER              RAMErrType[MAX_RAM_ERR_CNTRS];      /**< Array of SM_ERR_COUNTER */
} SM_RAM_ERR_CNTRS,
*PTR_SM_RAM_ERR_CNTRS;

#ifdef __cplusplus
}
#endif


/** @} */


#endif /* !defined SM_API_DEFN_H */


