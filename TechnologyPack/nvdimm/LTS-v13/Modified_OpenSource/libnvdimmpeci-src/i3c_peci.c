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

#include "i3c_peci.h"

#include "basetype.h"
#include "common.h"
#include "fis.h"

#include <math.h>
#include <time.h>

lpss_reg_t lpssRegs[] = {
    // Register name, Bar, Seg, Bus, Dev, Func, Offset, Addr Type, size
    {"lpss0_device_control", 0, 255, 30, 1, 0, 0x4, MMIO_ATYPE, 4},
    {"lpss1_device_control", 0, 255, 30, 1, 1, 0x4, MMIO_ATYPE, 4},
    {"lpss0_reset_ctrl", 0, 255, 30, 1, 0, 0x10, MMIO_ATYPE, 4},
    {"lpss1_reset_ctrl", 0, 255, 30, 1, 1, 0x10, MMIO_ATYPE, 4},
    {"lpss0_intr_status", 0, 255, 30, 1, 0, 0x20, MMIO_ATYPE, 4},
    {"lpss1_intr_status", 0, 255, 30, 1, 1, 0x20, MMIO_ATYPE, 4},
    {"lpss0_intr_status_enable", 0, 255, 30, 1, 0, 0x24, MMIO_ATYPE, 4},
    {"lpss1_intr_status_enable", 0, 255, 30, 1, 1, 0x24, MMIO_ATYPE, 4},
    {"lpss0_intr_signal_enable", 0, 255, 30, 1, 0, 0x28, MMIO_ATYPE, 4},
    {"lpss1_intr_signal_enable", 0, 255, 30, 1, 1, 0x28, MMIO_ATYPE, 4},
    {"lpss0_command_queue_port", 0, 255, 30, 1, 0, 0xc0, MMIO_ATYPE, 4},
    {"lpss1_command_queue_port", 0, 255, 30, 1, 1, 0xc0, MMIO_ATYPE, 4},
    {"lpss0_response_queue_port", 0, 255, 30, 1, 0, 0xc4, MMIO_ATYPE, 4},
    {"lpss1_response_queue_port", 0, 255, 30, 1, 1, 0xc4, MMIO_ATYPE, 4},
    {"lpss0_tx_data_port", 0, 255, 30, 1, 0, 0xc8, MMIO_ATYPE, 4},
    {"lpss1_tx_data_port", 0, 255, 30, 1, 1, 0xc8, MMIO_ATYPE, 4},
    {"lpss0_data_buffer_thld_ctrl", 0, 255, 30, 1, 0, 0xd4, MMIO_ATYPE, 4},
    {"lpss1_data_buffer_thld_ctrl", 0, 255, 30, 1, 1, 0xd4, MMIO_ATYPE, 4},
    {"lpss0_scl_i3c_od_timing", 0, 255, 30, 1, 0, 0x214, MMIO_ATYPE, 4},
    {"lpss1_scl_i3c_od_timing", 0, 255, 30, 1, 1, 0x214, MMIO_ATYPE, 4},
    {"lpss0_scl_i3c_pp_timing", 0, 255, 30, 1, 0, 0x218, MMIO_ATYPE, 4},
    {"lpss1_scl_i3c_pp_timing", 0, 255, 30, 1, 1, 0x218, MMIO_ATYPE, 4},
    {"lpss0_scl_i2c_fm_timing", 0, 255, 30, 1, 0, 0x21c, MMIO_ATYPE, 4},
    {"lpss1_scl_i2c_fm_timing", 0, 255, 30, 1, 1, 0x21c, MMIO_ATYPE, 4},
    {"lpss0_scl_i2c_fmp_timing", 0, 255, 30, 1, 0, 0x220, MMIO_ATYPE, 4},
    {"lpss1_scl_i2c_fmp_timing", 0, 255, 30, 1, 1, 0x220, MMIO_ATYPE, 4},
    {"lpss0_scl_i2c_ss_timing", 0, 255, 30, 1, 0, 0x224, MMIO_ATYPE, 4},
    {"lpss1_scl_i2c_ss_timing", 0, 255, 30, 1, 1, 0x224, MMIO_ATYPE, 4},
    {"lpss0_scl_ext_lcnt_timing", 0, 255, 30, 1, 0, 0x228, MMIO_ATYPE, 4},
    {"lpss1_scl_ext_lcnt_timing", 0, 255, 30, 1, 1, 0x228, MMIO_ATYPE, 4},
    {"lpss0_present_state_debug", 0, 255, 30, 1, 0, 0x24c, MMIO_ATYPE, 4},
    {"lpss1_present_state_debug", 0, 255, 30, 1, 1, 0x24c, MMIO_ATYPE, 4},
    {"lpss0_periodic_poll_command_enable", 0, 255, 30, 1, 0, 0x26c, MMIO_ATYPE,
     4},
    {"lpss1_periodic_poll_command_enable", 0, 255, 30, 1, 1, 0x26c, MMIO_ATYPE,
     4},
};

semp_reg_t sempRegs[] = {
    // Register name, Seg, Bus, Dev, Func, Offset, size
    {"systemaqusemp0", 0, 30, 0, 2, 0x188, 4},
    {"systemaqusemp1", 0, 30, 0, 2, 0x18c, 4},
    {"systemimpaqusemp0", 0, 30, 0, 2, 0x198, 4},
    {"systemimpaqusemp1", 0, 30, 0, 2, 0x19c, 4},
    {"systemheadsemp0", 0, 30, 0, 2, 0x1a8, 4},
    {"systemheadsemp1", 0, 30, 0, 2, 0x1ac, 4},
    {"systemtailsemp0", 0, 30, 0, 2, 0x1b8, 4},
    {"systemtailsemp1", 0, 30, 0, 2, 0x1bc, 4},
    {"systemreleasesemp0", 0, 30, 0, 2, 0x1c8, 4},
    {"systemreleasesemp1", 0, 30, 0, 2, 0x1cc, 4}};

EPECIStatus bus30ToPostEnumeratedBus(uint8_t addr, uint8_t* const postEnumBus)
{
    uint32_t cpubusno_valid = 0;
    uint32_t cpubusno7 = 0;
    uint8_t cc = 0;
    EPECIStatus ret;

    ret = peci_RdEndPointConfigPciLocal(
        addr, 0, POST_ENUM_QUERY_BUS, POST_ENUM_QUERY_DEVICE,
        POST_ENUM_QUERY_FUNCTION, POST_ENUM_QUERY_VALID_BIT_OFFSET,
        sizeof(cpubusno_valid), (uint8_t*)&cpubusno_valid, &cc);

    if (ret != PECI_CC_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Unable to read cpubusno_valid - cc: 0x%x ret: 0x%x\n", cc,
                    ret);
        // Need to return 1 for all failures
        return ret;
    }

    // Bit 30 is for checking bus 30 contains valid post enumerated bus#
    if (0 == CHECK_BIT(cpubusno_valid, 30))
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Bus 30 does not contain valid post enumerated bus"
                    "number! (0x%x)\n",
                    cpubusno_valid);
        return ret;
    }

    ret = peci_RdEndPointConfigPciLocal(
        addr, 0, POST_ENUM_QUERY_BUS, POST_ENUM_QUERY_DEVICE,
        POST_ENUM_QUERY_FUNCTION, POST_ENUM_QUERY_BUS_NUMBER_OFFSET,
        sizeof(cpubusno7), (uint8_t*)&cpubusno7, &cc);

    if (ret != PECI_CC_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Unable to read cpubusno7 - cc: 0x%x\n ret: 0x%x\n", cc,
                    ret);
        // Need to return 1 for all failures
        return ret;
    }

    // CPUBUSNO7[23:16] for Bus 30
    *postEnumBus = ((cpubusno7 >> 16) & 0xff);
    return ret;
}

static uint8_t getSPDBusNum(uint8_t mc)
{
    return mc >> 1;
}

PECIStatus waitForCmdData(peci_lpss_t lpss, bool check_cmd, bool check_tx,
                          bool check_rx)
{
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = I3C_PECI_TIMEOUT_INTERVAL_NS;
    int timeout_count = 0;
    // MC0/MC1 uses LPSS0; MC2/MC3 uses LPSS1
    uint8_t spd_bus = getSPDBusNum(lpss.mc);
    uint32_t intr_status =
        (spd_bus == 0 ? lpss0_intr_status : lpss1_intr_status);

    uint32_t data = 0;
    uint32_t cmd_queue_ready_stat = 0;
    uint32_t tx_thld_stat = 0;
    uint32_t rx_thld_stat = 0;
    uint8_t cc = 0;

    EPECIStatus peci_status = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    do
    {
        nanosleep(&req, NULL);
        timeout_count += I3C_PECI_TIMEOUT_INTERVAL_MS;

        peci_status = lpssRegRd(lpss.peci_addr, lpssRegs[intr_status], &data, &cc);
        if (peci_status != PECI_CC_SUCCESS)
        {
            status = PECI_I3C_FAIL;
            goto Finish;
        }

        cmd_queue_ready_stat =
            getFields(data, CMD_QUEUE_READY_STAT, CMD_QUEUE_READY_STAT);
        tx_thld_stat = getFields(data, TX_THLD_STAT, TX_THLD_STAT);
        rx_thld_stat = getFields(data, RX_THLD_STAT, RX_THLD_STAT);
    } while ((timeout_count <= I3C_PECI_TIMEOUT_MS) &&
             (((1 ^ cmd_queue_ready_stat) & check_cmd) |
              ((1 ^ tx_thld_stat) & check_tx) |
              ((1 ^ rx_thld_stat) & check_rx)));

    if (timeout_count > I3C_PECI_TIMEOUT_MS)
    {
        status = PECI_TIMEOUT;
    }
Finish:
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: waitForCmdData:(%d)\n", status);
        return status;
    }
    return status;
}

PECIStatus restartSpdController(peci_lpss_t lpss)
{
    uint8_t spd_bus = getSPDBusNum(lpss.mc);
    uint32_t intr_status =
        (spd_bus == 0 ? lpss0_intr_status : lpss1_intr_status);
    uint32_t device_control =
        (spd_bus == 0 ? lpss0_device_control : lpss1_device_control);
    uint32_t reset_ctrl = (spd_bus == 0 ? lpss0_reset_ctrl : lpss1_reset_ctrl);
    uint32_t present_state_debug =
        (spd_bus == 0 ? lpss0_present_state_debug : lpss1_present_state_debug);

    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = I3C_PECI_TIMEOUT_INTERVAL_NS;
    int timeout_count = 0;
    uint32_t data = 0;
    uint32_t device_control_data = 0;
    uint32_t cm_tfr_st_status = 0;
    uint8_t cc = 0;

    EPECIStatus peci_status = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    peci_status =
        lpssRegRd(lpss.peci_addr, lpssRegs[intr_status], &data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_RD_FAIL;
        goto Finish;
    }

    setFields(&data, TRANSFER_ERR_STAT, TRANSFER_ERR_STAT, 1);
    peci_status =
        lpssRegWr(lpss.peci_addr, lpssRegs[intr_status], data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_WR_FAIL;
        goto Finish;
    }

    peci_status = lpssRegRd(lpss.peci_addr, lpssRegs[device_control],
                            &device_control_data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_RD_FAIL;
        goto Finish;
    }

    setFields(&device_control_data, ENABLE, ENABLE, 0);
    peci_status = lpssRegWr(lpss.peci_addr, lpssRegs[device_control],
                            device_control_data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_WR_FAIL;
        goto Finish;
    }

    peci_status =
        lpssRegWr(lpss.peci_addr, lpssRegs[reset_ctrl], 0x1e, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_WR_FAIL;
        goto Finish;
    }

    setFields(&device_control_data, ENABLE, ENABLE, 1);
    peci_status = lpssRegWr(lpss.peci_addr, lpssRegs[device_control],
                            device_control_data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_WR_FAIL;
        goto Finish;
    }

    do
    {
        nanosleep(&req, NULL);
        timeout_count += I3C_PECI_TIMEOUT_INTERVAL_MS;

        peci_status = lpssRegRd(lpss.peci_addr, lpssRegs[present_state_debug],
                                &data, &cc);
        if (peci_status != PECI_CC_SUCCESS)
        {
            status = PECI_I3C_REG_RD_FAIL;
            goto Finish;
        }

        cm_tfr_st_status = getFields(data, CM_TFR_ST_MSB, CM_TFR_ST_LSB);
        if (cm_tfr_st_status == CM_TFR_ST_VAL)
        {
            setFields(&device_control_data, RESUME, RESUME, 1);
            peci_status = lpssRegWr(lpss.peci_addr, lpssRegs[device_control],
                                    device_control_data, &cc);
            if (peci_status != PECI_CC_SUCCESS)
            {
                status = PECI_I3C_REG_RD_FAIL;
                goto Finish;
            }
        }
    } while ((CM_TFR_ST_VAL == cm_tfr_st_status) &&
             (timeout_count <= I3C_PECI_TIMEOUT_MS));

    if (timeout_count > I3C_PECI_TIMEOUT_MS)
    {
        status = PECI_TIMEOUT;
    }

Finish:
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: restartSpdController:(%d)\n", status);
        return status;
    }
    return status;
}

PECIStatus waitForResponse(peci_lpss_t lpss, bool restart_on_error)
{
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = I3C_PECI_TIMEOUT_INTERVAL_NS;
    int timeout_count = 0;
    // MC0/MC1 uses LPSS0; MC2/MC3 uses LPSS1
    uint8_t spd_bus = getSPDBusNum(lpss.mc);
    uint32_t intr_status =
        (spd_bus == 0 ? lpss0_intr_status : lpss1_intr_status);
    uint32_t present_state_debug =
        (spd_bus == 0 ? lpss0_present_state_debug : lpss1_present_state_debug);
    uint32_t response_queue_port =
        (spd_bus == 0 ? lpss0_response_queue_port : lpss1_response_queue_port);

    uint32_t data = 0;
    uint32_t cm_tfr_st_status = 0;
    uint32_t resp_ready_stat = 0;
    uint8_t cc = 0;

    EPECIStatus peci_status = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    peci_status = lpssRegRd(lpss.peci_addr, lpssRegs[present_state_debug],
                            &data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_RD_FAIL;
        goto Finish;
    }

    cm_tfr_st_status = getFields(data, CM_TFR_ST_MSB, CM_TFR_ST_LSB);

    do
    {
        nanosleep(&req, NULL);
        timeout_count += I3C_PECI_TIMEOUT_INTERVAL_MS;
        peci_status = lpssRegRd(lpss.peci_addr, lpssRegs[intr_status], &data, &cc);
        if (peci_status != PECI_CC_SUCCESS)
        {
            status = PECI_I3C_FAIL;
            goto Finish;
        }

        resp_ready_stat = getFields(data, RESP_READY_STAT, RESP_READY_STAT);

        if (cm_tfr_st_status == CM_TFR_ST_VAL)
        {
            if (restart_on_error)
            {
                status = restartSpdController(lpss);
                if (status != PECI_SUCCESS)
                {
                    goto Finish;
                }
            }
            break;
        }

    } while ((0 == resp_ready_stat) && (timeout_count <= I3C_PECI_TIMEOUT_MS));

    peci_status = lpssRegRd(lpss.peci_addr, lpssRegs[response_queue_port],
                            &data, &cc);
    if (peci_status != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_REG_RD_FAIL;
        goto Finish;
    }

    if (timeout_count > I3C_PECI_TIMEOUT_MS && restart_on_error)
    {
        status = restartSpdController(lpss);
        if (status != PECI_SUCCESS)
        {
            goto Finish;
        }
    }

Finish:
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: waitForResponse:(%d)\n", status);
        return status;
    }
    return status;
}

EPECIStatus lpssRegRd(int addr, lpss_reg_t reg, void* val,
                      uint8_t* cc)
{
    return peci_RdEndPointConfigMmio(
        addr, reg.seg, reg.bus, reg.dev, reg.func, reg.bar, reg.addr_type,
        reg.offset, reg.size, val, cc);
}

EPECIStatus lpssRegWr(int addr, lpss_reg_t reg, uint32_t val,
                      uint8_t* cc)
{
    uint64_t val64 = val;
    return peci_WrEndPointConfigMmio(addr, reg.seg, reg.bus, reg.dev,
                                         reg.func, reg.bar, reg.addr_type,
                                         reg.offset, reg.size, val64, cc);
}

EPECIStatus sempRegRd(int addr, semp_reg_t reg, uint32_t* val, uint8_t* cc)
{
    return peci_RdEndPointConfigPciLocal(addr, reg.seg, reg.bus, reg.dev,
                                         reg.func, reg.offset, reg.size,
                                         (uint8_t*)val, cc);
}

EPECIStatus sempRegWr(int addr, semp_reg_t reg, uint32_t val, uint8_t* cc)
{
    return peci_WrEndPointPCIConfigLocal(addr, reg.seg, reg.bus, reg.dev,
                                         reg.func, reg.offset, reg.size, val,
                                         cc);
}

PECIStatus getDeviceControlFields(int addr, uint8_t mc, bool* enabled,
                                  bool* i2c_slave_present)
{
    uint8_t device_control =
        (getSPDBusNum(mc) == 0 ? lpss0_device_control : lpss1_device_control);
    uint32_t data = 0;
    uint8_t cc = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    if (enabled == NULL || i2c_slave_present == NULL)
    {
        status = PECI_INVALID_ARG;
        goto Finish;
    }

    peciStatus = lpssRegRd(addr, lpssRegs[device_control], &data, &cc);

    if (peciStatus != PECI_CC_SUCCESS)
    {
        status = PECI_I3C_FAIL;
        goto Finish;
    }

    *enabled = getFields(data, ENABLE, ENABLE);
    *i2c_slave_present = getFields(data, I2C_SLAVE_PRESENT, I2C_SLAVE_PRESENT);

Finish:
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: getDeviceControlFields:(%d)\n", status);
        return status;
    }
    return status;
}

PECIStatus wrRdRegular(peci_lpss_t lpss)
{
    // MC0/MC1 uses LPSS0; MC2/MC3 uses LPSS1
    uint8_t spd_bus = getSPDBusNum(lpss.mc);
    uint32_t command_queue_port =
        (spd_bus == 0 ? lpss0_command_queue_port : lpss1_command_queue_port);
    uint32_t tx_data_port =
        (spd_bus == 0 ? lpss0_tx_data_port : lpss1_tx_data_port);
    uint32_t data_buffer_thld_ctrl =
        (spd_bus == 0 ? lpss0_data_buffer_thld_ctrl
                      : lpss1_data_buffer_thld_ctrl);
    uint32_t data = 0;
    uint8_t cc = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_FAIL;
    uint32_t tx_start_thld_old = 0, command_low = 0, command_high = 0, wdata = 0;
    uint8_t toc = 0, roc = 0, rnw = 0, tid = 1, cmd_attr = 0, i2cni3c = 0, byte_num = 0, data_port_cnt_total = 0;
    uint8_t slave_address = lpss.addr >> 1;   
    uint8_t tx_data_port_size = lpssRegs[tx_data_port].size;
    // save old tx start buf thld
    peciStatus = lpssRegRd(lpss.peci_addr, lpssRegs[data_buffer_thld_ctrl],
                           &data, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }
    tx_start_thld_old =
        getFields(data, TX_START_THLD_MSB, TX_START_THLD_LSB);
    // make controller wait for all data before sending command to bus
    setFields(&data, TX_START_THLD_MSB, TX_START_THLD_LSB, 1);
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[data_buffer_thld_ctrl],
                           data, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }

    // Setup lower dword for command queue port
    // Use regular descriptor
    switch ((int)lpss.mode)
    {
        case I2C_MODE_0:
        case I2C_MODE_1:
            i2cni3c = 1;
            break;
        case I3C_MODE_0:
        case I3C_MODE_1:
        case I3C_MODE_2:
        case I3C_MODE_3:
            i2cni3c = 0;
            break;
        default:
            break;
    }
    
    command_low =
        (uint32_t)((toc << 31) | (roc << 30) | (rnw << 29) |
                   ((lpss.mode_speed & 0x7) << 26) |
                   ((slave_address & 0x7f) << 16) | ((lpss.cp & 1) << 15) |
                   ((lpss.cmd & 0xff) << 7) | (i2cni3c << 6) |
                   ((tid & 0x7) << 3) | (cmd_attr & 0x7));

    // Setup upper dword for command queue port
    command_high = (uint32_t)(lpss.wdata_size & 0xffff) << 16;

    status = waitForCmdData(lpss, TRUE, TRUE, FALSE);
    if (status != PECI_SUCCESS)
    {
        goto Finish;
    }

    // Send lower dword to command queue port
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[command_queue_port],
                           command_low, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }
    // Send upper dword to command queue port
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[command_queue_port],
                           command_high, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }

    // Setup lower dword for command queue port
    // Use Regular Data Transfer descriptor
    toc = 1;
    roc = 1;
    rnw = 1;
    slave_address = lpss.addr >> 1;
    tid = 1;
    cmd_attr = 0;

    command_low =
        (uint32_t)((toc << 31) | (roc << 30) | (rnw << 29) |
                   ((lpss.mode_speed & 0x7) << 26) |
                   ((slave_address & 0x7f) << 16) | ((lpss.cp & 1) << 15) |
                   ((lpss.cmd & 0xff) << 7) | (i2cni3c << 6) |
                   ((tid & 0x7) << 3) | (cmd_attr & 0x7));

    command_high = (uint32_t)(lpss.wdata_size & 0xffff) << 16;

    // write command_low
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[command_queue_port],
                           command_low, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }

    // write command_high
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[command_queue_port],
                           command_high, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }

    data_port_cnt_total =
        (uint8_t)floor(lpss.wdata_size / tx_data_port_size) + (lpss.wdata_size % tx_data_port_size > 0);

    for (uint8_t i = 0; i < data_port_cnt_total; i++)
    {
        wdata = 0;
        byte_num = 0;

        if ((lpss.wdata_size % tx_data_port_size != 0) &&
            (i == (data_port_cnt_total - 1)))
        {
            byte_num = lpss.wdata_size % tx_data_port_size;
        }
        else
        {
            byte_num = tx_data_port_size;
        }
        for (uint8_t b = 0; b < byte_num; b++)
        {
            wdata |= (uint32_t)(lpss.wdata_list[i * tx_data_port_size + b] << (b * 8));
        }
        peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[tx_data_port], wdata, &cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            goto Finish;
        }
    }

    status = waitForResponse(lpss, TRUE);
    if (status != PECI_SUCCESS)
    {
        goto Finish;
    }

    for (uint8_t i = 0; i < floor((lpss.wdata_size - 1) / 4) + 1; i++)
    {
        peciStatus = lpssRegRd(lpss.peci_addr, lpssRegs[tx_data_port], &data, &cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            goto Finish;
        }
        lpss.rdata_list[i] = data;
    }

    // restore old thld value
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[data_buffer_thld_ctrl],
                           tx_start_thld_old, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }

Finish:
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "ERROR: I3C LPSS wrRdRegular failed status:(%d) cc:(0x%x)\n",
            status, cc);
        return status;
    }
    if (peciStatus == PECI_CC_SUCCESS)
    {
        return status;
    }
    else
    {
        status = PECI_I3C_RW_FAIL;
        DEBUG_PRINT(
            PMEM_ERROR,
            "ERROR: I3C LPSS wrRdRegular failed peciStatus:(%d) cc:(0x%x)\n",
            peciStatus, cc);
        return status;
    }
}

PECIStatus wrRegular(peci_lpss_t lpss)
{
    // MC0/MC1 uses LPSS0; MC2/MC3 uses LPSS1
    uint8_t spd_bus = getSPDBusNum(lpss.mc);
    uint32_t command_queue_port =
        (spd_bus == 0 ? lpss0_command_queue_port : lpss1_command_queue_port);
    uint32_t tx_data_port =
        (spd_bus == 0 ? lpss0_tx_data_port : lpss1_tx_data_port);
    uint8_t cc = 0;
    bool controller_enabled = false;
    bool i2c_slave_present = false;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_FAIL;
    uint8_t roc = 1 , rnw = 0, tid = 1, cmd_attr = 0, i2cni3c =0, byte_num = 0, data_port_cnt_total = 0;
    uint8_t tx_data_port_size = lpssRegs[tx_data_port].size;
    uint32_t command_low = 0, command_high = 0, wdata = 0;
    uint8_t slave_address = lpss.addr >> 1;
    lpss.toc = 1;
    status = getDeviceControlFields(lpss.peci_addr, lpss.mc,
                                    &controller_enabled, &i2c_slave_present);
    
    if (!controller_enabled)
    {
        goto Finish;
    }


    // Setup lower dword for command queue port
    // Use regular descriptor
    
    switch ((int)lpss.mode)
    {
        case I2C_MODE_0:
        case I2C_MODE_1:
            i2cni3c = 1;
            break;
        case I3C_MODE_0:
        case I3C_MODE_1:
        case I3C_MODE_2:
        case I3C_MODE_3:
            i2cni3c = 0;
            break;
        default:
            break;
    }

    command_low =
        (uint32_t)((lpss.toc << 31) | (roc << 30) | (rnw << 29) |
                   ((lpss.mode_speed & 0x7) << 26) |
                   ((slave_address & 0x7f) << 16) | ((lpss.cp & 1) << 15) |
                   ((lpss.cmd & 0xff) << 7) | (i2cni3c << 6) |
                   ((tid & 0x7) << 3) | (cmd_attr & 0x7));

    // Setup upper dword for command queue port
    command_high = (uint32_t)((lpss.wdata_size & 0xffff) << 16);

    status = waitForCmdData(lpss, TRUE, TRUE, FALSE);
    if (status != PECI_SUCCESS)
    {
        goto Finish;
    }

    data_port_cnt_total =
        (uint8_t)floor(lpss.wdata_size / tx_data_port_size) + (lpss.wdata_size % tx_data_port_size > 0);

    for (uint8_t i = 0; i < data_port_cnt_total; i++)
    {
        wdata = 0;
        byte_num = 0;

        if ((lpss.wdata_size % tx_data_port_size != 0) &&
            (i == (data_port_cnt_total - 1)))
        {
            byte_num = lpss.wdata_size % tx_data_port_size;
        }
        else
        {
            byte_num = tx_data_port_size;
        }
        for (uint8_t b = 0; b < byte_num; b++)
        {
            wdata |=
                (uint32_t)(lpss.wdata_list[i * tx_data_port_size + b] << (b * 8));
        }
        peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[tx_data_port], wdata, &cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            goto Finish;
        }
    }
    // write command_low
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[command_queue_port],
                           command_low, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }
    // write command_high
    peciStatus = lpssRegWr(lpss.peci_addr, lpssRegs[command_queue_port],
                           command_high, &cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        goto Finish;
    }

    status = waitForResponse(lpss, TRUE);
    if (status != PECI_SUCCESS)
    {
        goto Finish;
    }


Finish:
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: I3C LPSS wrRegular failed status:(%d) cc:(0x%x)\n",
                    status, cc);
        return status;
    }
    if (peciStatus == PECI_CC_SUCCESS)
    {
        return status;
    }
    else
    {
        status = PECI_I3C_W_FAIL;
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: I3C LPSS wrRegular failed peciStatus:(%d) "
                    "cc:(0x%x)\n",
                    peciStatus, cc);
        return status;
    }
}

PECIStatus i3cBlockRead(int addr, uint8_t saddr, uint8_t channel, uint8_t dev,
                        uint8_t func, uint16_t offset, uint8_t opcode,
                        uint8_t numbytes, uint32_t* val)
{
    int ret = 0;
    uint8_t postEnumBus = 0;
    EPECIStatus eret;
    eret = bus30ToPostEnumeratedBus(addr, &postEnumBus);
    if (eret != PECI_CC_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "Unable to read cpubusno_valid - ret: 0x%x\n",
                    eret);
        return PECI_I3C_RW_FAIL;
    }

    peci_lpss_t lpss;

    for (int i = lpss0_device_control; i < numberOfLpssRegisters; i++)
    {
        lpssRegs[i].bus = postEnumBus;
    }

    if (ret != PECI_CC_SUCCESS)
    {
        return ret;
    }
    PECIStatus status = PECI_SUCCESS;

    bool controller_enabled = false;
    bool i2c_slave_present = false;
    uint32_t read = 1;
    uint8_t dev_func;
    uint32_t header;
    lpss.channel = channel % CHANNELS_PER_IMC;
    lpss.mc = channel / CHANNELS_PER_IMC;
    lpss.wdata_size = sizeof(header);
    lpss.addr = saddr;
    lpss.peci_addr = addr;
    lpss.mode = I3C_MODE_0;

    status = getDeviceControlFields(lpss.peci_addr, lpss.mc,
                                    &controller_enabled, &i2c_slave_present);
    if (status != PECI_SUCCESS)
    {
        goto Finish;
    }

    if (controller_enabled && i2c_slave_present)
    {
        lpss.mode = I2C_MODE_0;
    }

    dev_func = (func << 5) | dev;
    header = (uint32_t)(offset << 16) | (uint32_t)(opcode << 12) |
             (uint32_t)(read << 11) | dev_func;

    lpss.wdata_list = (uint8_t*)calloc(numbytes, sizeof(uint8_t));
    if (lpss.wdata_list == NULL)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: Failed to allocate wdata memory\n");
        goto Finish;
    }
    lpss.wdata_list[0] = header & 0xff;
    lpss.wdata_list[1] = header >> 8 & 0xff;
    lpss.wdata_list[2] = header >> 16 & 0xff;
    lpss.wdata_list[3] = header >> 24 & 0xff;

    lpss.rdata_list = (uint32_t*)calloc(1, sizeof(uint32_t));
    if (lpss.rdata_list == NULL)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: Failed to allocate rdata memory\n");
        free(lpss.wdata_list);
        goto Finish;
    }

    status = wrRdRegular(lpss);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: wrRdRegular(%d)\n", status);
        free(lpss.rdata_list);
        free(lpss.wdata_list);
        goto Finish;
    }

    if (val != NULL)
    {
        *val = lpss.rdata_list[0];
    }

    free(lpss.rdata_list);
    free(lpss.wdata_list);

Finish:
    return status;
}

PECIStatus i3cBlockWrite(int addr, uint8_t saddr, uint8_t channel, uint8_t dev,
                         uint8_t func, uint16_t offset, uint8_t opcode,
                         uint8_t numbytes, uint32_t val)
{
    int ret = 0;
    uint8_t postEnumBus = 0;
    EPECIStatus eret;
    eret = bus30ToPostEnumeratedBus(addr, &postEnumBus);
    if (eret != PECI_CC_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "Unable to read cpubusno_valid - ret: 0x%x\n",
                    eret);
        return PECI_I3C_RW_FAIL;
    }

    peci_lpss_t lpss;

    for (int i = lpss0_device_control; i < numberOfLpssRegisters; i++)
    {
        lpssRegs[i].bus = postEnumBus;
    }

    if (ret != PECI_CC_SUCCESS)
    {
        return ret;
    }
    PECIStatus status = PECI_SUCCESS;

    bool controller_enabled = false;
    bool i2c_slave_present = false;
    uint32_t read = 0;
    uint8_t dev_func;
    uint32_t header;
    lpss.channel = channel % CHANNELS_PER_IMC;
    lpss.mc = channel / CHANNELS_PER_IMC;
    lpss.wdata_size = sizeof(header) + numbytes;
    lpss.addr = saddr;
    lpss.peci_addr = addr;
    lpss.mode = I3C_MODE_0;

    status = getDeviceControlFields(lpss.peci_addr, lpss.mc,
                                    &controller_enabled, &i2c_slave_present);
    if (status != PECI_SUCCESS)
    {
        goto Finish;
    }

    if (controller_enabled && i2c_slave_present)
    {
        lpss.mode = I2C_MODE_0;
    }

    dev_func = (func << 5) | dev;
    header = (uint32_t)(offset << 16) | (uint32_t)(opcode << 12) |
             (uint32_t)(read << 11) | dev_func;

    lpss.wdata_list = (uint8_t*)calloc(lpss.wdata_size, sizeof(uint8_t));
    if (lpss.wdata_list == NULL)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: Failed to allocate wdata memory\n");
        goto Finish;
    }
    lpss.wdata_list[0] = header & 0xff;
    lpss.wdata_list[1] = header >> 8 & 0xff;
    lpss.wdata_list[2] = header >> 16 & 0xff;
    lpss.wdata_list[3] = header >> 24 & 0xff;
    lpss.wdata_list[4] = val & 0xff;
    lpss.wdata_list[5] = val >> 8 & 0xff;
    lpss.wdata_list[6] = val >> 16 & 0xff;
    lpss.wdata_list[7] = val >> 24 & 0xff;


    lpss.rdata_list = (uint32_t*)calloc(1, sizeof(uint32_t));
    if (lpss.rdata_list == NULL)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: Failed to allocate rdata memory\n");
        free(lpss.wdata_list);
        goto Finish;
    }

    status = wrRegular(lpss);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: wrRegular(%d)\n", status);
        free(lpss.wdata_list);
        free(lpss.rdata_list);
        goto Finish;
    }

    free(lpss.rdata_list);
    free(lpss.wdata_list);

Finish:
    return status;
}
