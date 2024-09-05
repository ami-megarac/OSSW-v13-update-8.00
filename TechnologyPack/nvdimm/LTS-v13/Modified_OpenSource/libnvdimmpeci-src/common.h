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

#ifndef __UTILS_H__
#define __UTILS_H__

#include "peci.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define POST_ENUM_QUERY_BUS 8
#define POST_ENUM_QUERY_DEVICE 3
#define POST_ENUM_QUERY_FUNCTION 0
#define POST_ENUM_QUERY_VALID_BIT_OFFSET 0x1a0
#define POST_ENUM_QUERY_BUS_NUMBER_OFFSET 0x1cc

#define PECI_CC_UA(cc)                                                         \
    (((cc) != PECI_DEV_CC_SUCCESS && (cc) != PECI_DEV_CC_FATAL_MCA_DETECTED)   \
         ? true                                                                \
         : false)

#define SET_BIT(val, pos) ((val) |= ((uint64_t)1 << ((uint64_t)pos)))
#define CHECK_BIT(val, pos) ((val) & ((uint64_t)1 << ((uint64_t)pos)))

    void setFields(uint32_t* value, uint32_t msb, uint32_t lsb,
                   uint32_t setVal);

    uint32_t getFields(uint32_t value, uint32_t msb, uint32_t lsb);

    uint32_t bitField(uint32_t offset, uint32_t size, uint32_t val);

    typedef enum
    {
        PECI_SUCCESS = 0,
        PECI_FAIL,
        PECI_INVALID_ARG,
        PECI_TIMEOUT,
        PECI_REG_FAIL,
        PECI_SMB_FAIL,
        PECI_SMB_STATUS_CHECK_FAIL,
        PECI_I3C_REG_RD_FAIL,
        PECI_I3C_REG_WR_FAIL,
        PECI_I3C_STATUS_CHECK_FAIL,
        PECI_I3C_FAIL,
        PECI_I3C_RW_FAIL,
        PECI_I3C_W_FAIL,
        PECI_CSR_FAIL,
    } PECIStatus;

    typedef enum
    {
        I2C_MODE_0 = 0,
        I2C_MODE_1,
        I3C_MODE_0,
        I3C_MODE_1,
        I3C_MODE_2,
        I3C_MODE_3,
        I3C_MODE_4,
    } SPDMode;

    typedef enum
    {
        ekv = 0x8086097a,
        bwv = 0x8086097b,
        cwv = 0x8086097c,
    } NVDIDS;

    typedef struct
    {
        uint8_t opcode;
        uint8_t subopcode;
        uint32_t* indw;
        uint8_t indwSize;
    } ApiParams;

    /**
     * PECI Device address
     */
    typedef struct _peci_device_addr
    {
        uint8_t cpu;
        uint8_t imc;
        uint8_t peci_addr;
        uint8_t slot;
        uint8_t channel;
    } peci_device_addr_t;

    typedef struct _peci_lpss
    {
        int peci_addr;
        int fd;
        uint8_t mc;
        uint8_t channel;
        uint8_t addr;
        uint8_t* wdata_list;
        uint8_t wdata_size;
        uint32_t* rdata_list;
        uint8_t rdata_size;
        uint8_t cp;
        uint8_t cmd;
        uint8_t toc;
        SPDMode mode;
        uint8_t mode_speed;
        bool restart_on_error;
    } peci_lpss_t;

#ifdef __cplusplus
}
#endif

#endif
