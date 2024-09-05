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

#ifndef __I3C_PECI_H__
#define __I3C_PECI_H__

#pragma once
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wcast-align"

#include "common.h"

#include "peci.h"
#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define REG_NAME_LEN 64
#define MMIO_ATYPE 6

#define MILI_TO_NANO(a) (a * 1000 * 1000)
#define I3C_PECI_TIMEOUT_INTERVAL_MS 1
#define I3C_PECI_TIMEOUT_MS 500
#define I3C_PECI_TIMEOUT_INTERVAL_NS MILI_TO_NANO(I3C_PECI_TIMEOUT_INTERVAL_MS)

// intr status bits
#define TRANSFER_ERR_STAT 9
#define RESP_READY_STAT 4
#define CMD_QUEUE_READY_STAT 3
#define RX_THLD_STAT 1
#define TX_THLD_STAT 0

// reset ctrl bits
#define RX_FIFO_RST 4
#define TX_FIFO_RST 3
#define RESP_QUEUE_RST 2
#define CMD_QUEUE_RST 1

// device control bits
#define ENABLE 31
#define RESUME 30
#define I2C_SLAVE_PRESENT 7

// present state debug bits
#define CM_TFR_ST_MSB 21
#define CM_TFR_ST_LSB 16

#define CM_TFR_ST_VAL 0x13

// data buffer thld ctrl bits
#define TX_START_THLD_MSB 18
#define TX_START_THLD_LSB 16

    typedef struct
    {
        char name[REG_NAME_LEN];
        uint8_t bar;
        uint8_t seg;
        uint8_t bus;
        uint8_t dev;
        uint8_t func;
        uint64_t offset;
        uint8_t addr_type;
        uint8_t size;
    } lpss_reg_t;

    extern lpss_reg_t lpssRegs[];

    enum lpssRegname
    {
        lpss0_device_control,
        lpss1_device_control,
        lpss0_reset_ctrl,
        lpss1_reset_ctrl,
        lpss0_intr_status,
        lpss1_intr_status,
        lpss0_intr_status_enable,
        lpss1_intr_status_enable,
        lpss0_intr_signal_enable,
        lpss1_intr_signal_enable,
        lpss0_command_queue_port,
        lpss1_command_queue_port,
        lpss0_response_queue_port,
        lpss1_response_queue_port,
        lpss0_tx_data_port,
        lpss1_tx_data_port,
        lpss0_data_buffer_thld_ctrl,
        lpss1_data_buffer_thld_ctrl,
        lpss0_scl_i3c_od_timing,
        lpss1_scl_i3c_od_timing,
        lpss0_scl_i3c_pp_timing,
        lpss1_scl_i3c_pp_timing,
        lpss0_scl_i2c_fm_timing,
        lpss1_scl_i2c_fm_timing,
        lpss0_scl_i2c_fmp_timing,
        lpss1_scl_i2c_fmp_timing,
        lpss0_scl_i2c_ss_timing,
        lpss1_scl_i2c_ss_timing,
        lpss0_scl_ext_lcnt_timing,
        lpss1_scl_ext_lcnt_timing,
        lpss0_present_state_debug,
        lpss1_present_state_debug,
        lpss0_periodic_poll_cmd_en,
        lpss1_periodic_poll_cmd_en,
        numberOfLpssRegisters
    };

    typedef struct
    {
        char name[REG_NAME_LEN];
        uint8_t seg;
        uint8_t bus;
        uint8_t dev;
        uint8_t func;
        uint64_t offset;
        uint8_t size;
    } semp_reg_t;

    extern semp_reg_t sempRegs[];

    enum sempRegname
    {
        systemaqusemp0,
        systemaqusemp1,
        systemimpaqusemp0,
        systemimpaqusemp1,
        systemheadsemp0,
        systemheadsemp1,
        systemtailsemp0,
        systemtailsemp1,
        systemreleasesemp0,
        systemreleasesemp1,
        numberOfSempRegisters
    };

    EPECIStatus bus30ToPostEnumeratedBus(uint8_t addr,
                                         uint8_t* const postEnumBus);

    EPECIStatus lpssRegRd(int addr, lpss_reg_t reg, void* val,
                          uint8_t* cc);

    EPECIStatus lpssRegWr(int addr, lpss_reg_t reg, uint32_t val,
                          uint8_t* cc);

    EPECIStatus sempRegRd(int addr, semp_reg_t reg, uint32_t* val, uint8_t* cc);

    EPECIStatus sempRegWr(int addr, semp_reg_t reg, uint32_t val, uint8_t* cc);

    PECIStatus i3cBlockRead(int addr, uint8_t saddr, uint8_t channel,
                            uint8_t dev, uint8_t func, uint16_t offset,
                            uint8_t opcode, uint8_t numbytes, uint32_t* val);

    PECIStatus i3cBlockWrite(int addr, uint8_t saddr, uint8_t channel,
                             uint8_t dev, uint8_t func, uint16_t offset,
                             uint8_t opcode, uint8_t numbytes, uint32_t val);

#ifdef __cplusplus
}
#endif

#endif
