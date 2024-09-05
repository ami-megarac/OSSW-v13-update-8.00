/*******************************************************************************
* @file PexSmDefs.h
*
* @brief This file provides the header files and structures for System Manager API
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

#ifndef __PEXSM_DEFS_H
#define __PEXSM_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************
 * STRUCTURES and DEFINITIONS
 ****************************************/
#define PMG_MAX_PORT                    (128)               /**< Max port number */
#define PMG_MAX_STN                     8                   /**< Max stations in chip */
#define MAX_PORT_ERR_CNTRS              4                   /**< Max Port Error Counters */
#define MAX_RAM_ERR_CNTRS               17                  /**< Max RAM Error Counters */
#define INVALID_DRV_VER                 0xFFFFFFFF

 /** PMG_PORT_MASK_xx --> PMG_BITMASK conversions */
#define PMG_BITMASK_T(Name,Bits)        U32 (Name)[ ((Bits) + 31) / 32 ]
#define PMG_PORT_MASK_T( Name )         PMG_BITMASK_T( (Name), PMG_MAX_PORT )

 /** Test whether a specific bit is set in generic bitmask variable */
#define PMG_MASK_TEST(Mask,Bit)         ( ((Mask)[ (Bit) / 32 ] & ((U32)1 << ((Bit) % 32))) != 0 )

 /** Copies one mask to another */
#define PMG_MASK_COPY(Src,Dest)         memcpy( (Dest), (Src), sizeof((Dest)) )

/** Defines the transport binding */
typedef enum _PEXSM_TRANSP_BINDING
{
    PEXSM_NT2 = 1,          /** NT2 Driver*/
    PEXSM_MCTP_PCIE,        /** MCTP over PCIe*/
    PEXSM_MCTP_I2C          /** MCTP over I2C*/
} PEXSM_TRANSP_BINDING;

 /** Common station configurations after combining quads */
typedef enum _PMG_STN_CFG
{
    PMG_ATLAS_STN_CFG_1x16 = 0x0000,        /**< 1 port x16 */
    PMG_ATLAS_STN_CFG_2x8 = 0x7777,         /**< 2 port x8 */
    PMG_ATLAS_STN_CFG_4x4 = 0x1111,         /**< 4 port x4 */
    PMG_ATLAS_STN_CFG_x4x4x8 = 0x1177,      /**< 3 port x4x4x8 */
    PMG_ATLAS_STN_CFG_x8x4x4 = 0x7711,      /**< 3 port x8x4x4 */
    PMG_ATLAS_STN_CFG_8x2 = 0x2222,         /**< 8 port x2 */
    PMG_ATLAS_STN_CFG_2_x1 = 0xFFFD,        /**< SW use only - Special config for S7 (2x1) */
    PMG_ATLAS_STN_CFG_UNKNOWN = 0xFFEE,     /**< SW use only - Unknown config */
    PMG_ATLAS_STN_CFG_DISABLED = 0xFFFF     /**< SW use only - All quads disabled */
} PMG_STN_CFG;

/** Switch mode types */
typedef enum _PMG_SW_MODE
{
    PMG_SW_MODE_BASE = 0,               /**< Standard PCIe fan-out */
    PMG_SW_MODE_SMART_SWITCH = 1,       /**< Smart switch */
    PMG_SW_MODE_USE_DEFAULT = 3         /**< Config page request to use default mode */
} PMG_SW_MODE;

/** Switch-specific flags */
typedef enum _PMG_SW_PROP_FLAGS
{
    PMG_SW_PROP_FLAG_NONE                = 0,        /**< No switch property flags */
    PMG_SW_PROP_FLAG_RESV_BIT_1_0        = (0 << 0), /**< [1:0] Reserved bits */
    PMG_SW_PROP_FLAG_SINGLE_MPT_MODE     = (1 << 2), /**< [2] Only 1 MPT to cover all host ports */
    PMG_SW_PROP_FLAG_RESV_BIT_6_3        = (0 << 3)  /**< [6:3] Reserved bits */
} PMG_SW_PROP_FLAGS;

/** Switch port types */
typedef enum _PMG_SW_PORT_TYPE
{
    PMG_SW_PORT_TYPE_DS          = 0,       /**< Downstream Port type */
    PMG_SW_PORT_TYPE_FABRIC      = 1,       /**< Fabric Port type */
    PMG_SW_PORT_TYPE_MGMT        = 2,       /**< Management Port type */
    PMG_SW_PORT_TYPE_HOST        = 3,       /**< Host Port type */
    PMG_SW_PORT_TYPE_DISABLED    = 0xFF     /**< Disabled Port type */
} PMG_SW_PORT_TYPE;

/** Host port-specific flags */
typedef enum _PMG_HOST_PORT_FLAGS
{
    PMG_HOST_PORT_FLAG_NONE = 0,        /**< No host flags set */
    PMG_HOST_PORT_FLAG_DUAL_SYNTH = (1 << 1), /**< Synthetic tree is 2 levels versus 1 */
    PMG_HOST_PORT_FLAG_MPT = (1 << 2), /**< Enable export of synthetic MPT EP */
    PMG_HOST_PORT_FLAG_TWC = (1 << 3), /**< Enable export of synthetic TWC EP */
    PMG_HOST_PORT_FLAG_GDMA = (1 << 4), /**< Enable export of synthetic gDMA EP */
    PMG_HOST_PORT_FLAG_GDMA_MULT = (1 << 5), /**< Enable multi-gDMA endpoints per host t */
    PMG_HOST_PORT_FLAG_ONE_TO_ONE = (1 << 6), /**< Enable 1:1 mode between synth/phys DS */
    PMG_HOST_PORT_FLAG_FORCE_64B_PF = (1 << 7), /**< Force 64b BARs as prefetchable to hosts */
    PMG_HOST_PORT_FLAG_LIMIT_MPS_128B = (1 << 8), /**< Set P2P bridge MPS to 128B */
    PMG_HOST_PORT_FLAG_DISABLE_MGMT = (1 << 9),  /**< Disable super-host Management (SwitchLib) */
    PMG_HOST_PORT_FLAG_VMD_LED_EN = (1 << 10) /**< Enable VMD LED control instead of PCIe std */
} PMG_HOST_PORT_FLAGS;

/** SM API command 'Get library Version' response data structure*/
typedef struct _SM_VERSION
{
    union
    {
        /** Structured access to library version. */
        struct
        {
            U8                  Dev;                    /**< Development Version */
            U8                  Unit;                   /**< Unit Version */
            U8                  Minor;                  /**< Minor Version */
            U8                  Major;                  /**< Major Version */
        } Ver;
        /** Word access to version. */
        U32                     Word;
    } Lib;

    union
    {
        /** Structured access to Driver version. */
        struct
        {
            U8                  Dev;                    /**< Development Version */
            U8                  Unit;                   /**< Unit Version */
            U8                  Minor;                  /**< Minor Version */
            U8                  Major;                  /**< Major Version */
        } Ver;
        /** Word access to version. */
        U32                     Word;
    } Drv;

    union
    {
        /** Structured access to API version used in the library. */
        struct
        {
            U8                  Minor;                  /**< Minor Version */
            U8                  Major;                  /**< Major Version */
        } Ver;
        /** Word access to version. */
        U16                     Word;
    } LibAPI;
} SM_VERSION,
*PTR_SM_VERSION;

/** Switch ID unique within fabric */
typedef struct _PMG_SWITCH_ID
{
    union
    {
        U16 ID;                             /**< Switch ID */
        struct
        {
            U8 Number;                      /**< Chip number within domain (0-based) */
            U8 Domain;                      /**< Fabric domain number */
        } DN;
    } u;
} PMG_SWITCH_ID, *PTR_PMG_SWITCH_ID;

/** Switch properties */
typedef struct _PMG_SWITCH_PROP
{
    PMG_SWITCH_ID SwitchID;                 /**< Switch ID */
    U16           ChipType;                 /**< Chip type from HW device ID */
    U16           ChipID;                   /**< Chip ID */
    U8            ChipRev;                  /**< Chip revision */
    U8            StnMask;                  /**< Mask to denote which stations enabled in chip */
    U8            StnCount;                 /**< Number of stations in chip */
    U8            PortsPerStn;              /**< Number of ports per station */
    U8            MgmtPortNum;              /**< Management(iSSW) / Upstream(BSW) port number */
    U8            Flags;                    /**< Switch-specific flags */
    struct
    {
        U8  Flags;                          /**< Station-specific flags */
        U8  ActivePortCount;                /**< Number of active ports in station */
        U16 Config;                         /**< Station port configuration */
    } Stn[PMG_MAX_STN];
} PMG_SWITCH_PROP;


/** PCIe-specific properties */
typedef struct _PCIE_PORT_PROP
{
    U8  PortType;                          /**< PCIe defined port type (PCIE_PORT_TYPE enum) */
    U8  PortNumber;                        /**< Internal port number */
    U8  LinkWidth;                         /**< Negotiated link width */
    U8  MaxLinkWidth;                      /**< Max link width device is capable of */
    U8  LinkSpeed;                         /**< Negotiated link speed */
    U8  MaxLinkSpeed;                      /**< Max link speed device is capable of */
    U16 MaxReadReqSize;                    /**< Max read request size allowed */
    U16 MaxPayloadSize;                    /**< Max payload size setting */
    U16 MaxPayloadSupported;               /**< Max payload size supported by device */
} PCIE_PORT_PROP, *PTR_PCIE_PORT_PROP;


/** API 'Get NT Host Information' response data structure */
typedef struct _SM_NT_HOST_INFO
{
    PMG_SWITCH_ID  HpSwitchId;             /**< Switch Id to which the Port belongs */
    U8             HostPortNum;            /**< Host Port Number of the NT Host being requested */
    U8             NtFlag    : 1;          /**< NT Flag */
    U8             Reserved  : 7;          /**< Reserved */

} SM_NT_HOST_PORT_INFO,
 *PTR_SM_NT_HOST_PORT_INFO;

/** Error counters */
typedef enum _PMG_ERR_COUNTER
{
    PORT_RECEIVER_ERROR_COUNTER,           /**< Port receiver error counter */
    BAD_TLP_ERROR_COUNTER,                 /**< Bad TLP error counter */
    BAD_DLLP_ERROR_COUNTER,                /**< Bad DLLP error counter */
    RECOVERY_DIAG_ERROR_COUNTER,           /**< Recovery diagnostic error counter*/
    TEC_PLD_RAM0_INST0_ERROR_COUNTER,      /**< TEC PLD RAM0 instance 0 error counter */
    TEC_PLD_RAM0_INST1_ERROR_COUNTER,      /**< This counter is deprecated, zero will be returned on access */
    TEC_PLD_RAM1_INST0_ERROR_COUNTER,      /**< TEC PLD RAM1 instance 0 error counter */
    TEC_PLD_RAM1_INST1_ERROR_COUNTER,      /**< This counter is deprecated, zero will be returned on access */
    TEC_HDR_RAM0_INST0_ERROR_COUNTER,      /**< TEC HDR RAM0 instance 0 error counter */
    TEC_HDR_RAM0_INST1_ERROR_COUNTER,      /**< This counter is deprecated, zero will be returned on access */
    TEC_HDR_RAM1_INST0_ERROR_COUNTER,      /**< TEC HDR RAM1 instance 0 error counter */
    TEC_HDR_RAM1_INST1_ERROR_COUNTER,      /**< This counter is deprecated, zero will be returned on access */
    TEC_PLL_RAM_ERROR_COUNTER,             /**< TEC PLL RAM error counter */
    MISC_DQD_RAM_ERROR_COUNTER,            /**< Misc DQD RAM error counter */
    MISC_DQLL_RAM_ERROR_COUNTER,           /**< Misc DQLL RAM error counter */
    MISC_SQLL_RAM_ERROR_COUNTER,           /**< Misc SQLL RAM error counter */
    MISC_RETRY_BUF_RAM_ERROR_COUNTER,      /**< Misc retry buffer RAM error counter */
    RDR_MISC_DMA_RAM0_ERROR_COUNTER,       /**< RDR misc DMA RAM0 error counter */
    RDR_MISC_DMA_RAM1_ERROR_COUNTER,       /**< RDR misc DMA RAM1 error counter */
    RDR_MISC_ALUT_RAM_ERROR_COUNTER,       /**< RDR misc ALUT RAM error counter */
    RDR_MISC_TEC_GBL2LCL_RAM_ERROR_COUNTER /**< RDR misc TEC global o local RAM error counter */
} PMG_ERR_COUNTER;

typedef struct _SM_ERR_COUNTER
{
    U16         OpCode;                    /**< enum PMG_ERR_COUNTER */
    U16         Status;                    /**< status of individual RAM error type */
    U32         ErrCounter;                /**< RAM error counter/value */
}SM_ERR_COUNTER,
*PTR_SM_ERR_COUNTER;

#ifdef __cplusplus
}
#endif


#endif
