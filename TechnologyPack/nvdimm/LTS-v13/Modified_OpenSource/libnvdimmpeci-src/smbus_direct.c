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

#include "smbus_direct.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#define MB_COMPLETE 0x1
#define STATUS_MASK 0xFF

/*
 * SMBus command codes
 */
#define SMB_CMD_CFG_READ 0x0
#define SMB_CMD_CFG_WRITE 0x1
#define SMB_CMD_READ_DWORD 0x0
#define SMB_CMD_WRITE_DWORD 0x3
#define SMB_CMD_WORD_OPERATION 0x1
#define SMB_CMD_BYTE_OPERATION 0x0

/*
 * AHB address limitation for PMEM
 */
#define CSR_START_ADDR 0xC0000000
#define CSR_FORBIDDEN_BITS 0x03FFE000
#define SPI_BASE 0x83000000

#define SMB_BOOT_STATUS_REGISER CSR_START_ADDR + 0x8000000
#define SMB_BOOT_STATUS_REGISER_H CSR_START_ADDR + 0x8000004
#define SMB_CMD_REGISTER_SPARE CSR_START_ADDR + 0x8000800
#define SMB_CMD_REGISTER CSR_START_ADDR + 0x8000804
#define SMB_NONCE0_REGISTER CSR_START_ADDR + 0x8000808
#define SMB_NONCE1_REGISTER CSR_START_ADDR + 0x800080C
#define SMB_NONCE2_REGISTER CSR_START_ADDR + 0x8000810
#define SMB_NONCE3_REGISTER CSR_START_ADDR + 0x8000814
#define SMB_INPUT_PAYLOAD_REGISTER_BASE CSR_START_ADDR + 0x8000818
#define SMB_STATUS_REGISTER CSR_START_ADDR + 0x8000898
#define SMB_STATUS_REGISTER_H CSR_START_ADDR + 0x800089C
#define SMB_OUTPAYLOAD_REGISTER_BASE CSR_START_ADDR + 0x80008A0
#define SMB_MAILBOX_REGISTER_SIZE (4)
#define SMB_INPUT_PAYLOAD_COUNT (32)
#define SMB_OUTPUT_PAYLOAD_COUNT (32)

/*
 * SPD specific information
 */
#define SPD_BSP_COMMAND_SIZE 1
#define SPD_BSP_LENGTH_SIZE 1
#define SPD_BSP_ADDRESS_SIZE 4
#define SPD_BSP_DATA_SIZE 4
#define SPD_BSP_STATUS_SIZE 1

#define SPD_BSP_CMD_BEGIN_BIT 0x80
#define SPD_BSP_CMD_END_BIT 0x40
#define SPD_BSP_CMD_PEC_EN_BIT 0x10
#define SPD_BSP_CMD_WRITE_DWORD 0xC
#define SPD_BSP_CMD_READ_DWORD 0
#define SPD_BSP_CMD_BLOCK_OP 0x2

#define SPD_BSP_CMD_INDEX 0
#define SPD_BSP_STATUS_INDEX 1
#define SPD_BSP_WR_LEN_INDEX 1
#define SPD_BSP_RSVD_INDEX 2
#define SPD_BSP_DEV_FUN_INDEX 3
#define SPD_BSP_REG_HI_INDEX 4
#define SPD_BSP_REF_LO_INDEX 5
#define SPD_BSP_WR_DATA_3_INDEX 6
#define SPD_BSP_WR_DATA_2_INDEX 7
#define SPD_BSP_WR_DATA_1_INDEX 8
#define SPD_BSP_WR_DATA_0_INDEX 9

#define SPD_BSP_RD_LEN_INDEX 0
#define SPD_BSP_RD_DATA_3_INDEX 2
#define SPD_BSP_RD_DATA_2_INDEX 3
#define SPD_BSP_RD_DATA_1_INDEX 4
#define SPD_BSP_RD_DATA_0_INDEX 5

#define SPD_BPS_DEV_POS_BIT 3
#define SPD_BPS_DEV_MASK 0xF8
#define SPD_BPS_FUN_MASK 0x07
#define SPD_BPS_HI_BYTE_POS_BIT 8
#define SPD_BPS_BYTE_MASK 0xFF

#define SPD_BPS_SUCCESS 1

#define WRITE_ARRAY_SIZE                                                       \
    (SPD_BSP_COMMAND_SIZE + SPD_BSP_LENGTH_SIZE + SPD_BSP_ADDRESS_SIZE +       \
     SPD_BSP_DATA_SIZE)
#define WRITE_READ_ARRAY_SIZE                                                  \
    (SPD_BSP_COMMAND_SIZE + SPD_BSP_LENGTH_SIZE + SPD_BSP_ADDRESS_SIZE)
#define READ_ARRAY_SIZE                                                        \
    (SPD_BSP_LENGTH_SIZE + SPD_BSP_STATUS_SIZE + SPD_BSP_DATA_SIZE)

smbus_read_interface smb_read_dword = smbus_read_direct;
smbus_write_interface smb_write_dword = smbus_write_direct;

void set_smbus_interface(smbus_read_interface pReadApi,
                         smbus_write_interface pWriteApi)
{
    smb_read_dword = pReadApi;
    smb_write_dword = pWriteApi;
}

int i2c_open_bus(const char* path)
{
    return open(path, O_RDWR | O_CLOEXEC);
}

static int
    i2c_writeread_on_bus(int busFd,
                         int slaveAddr,           // Target address
                         const uint8_t* writeBuf, // Ptr to outgoing data buffer
                         uint8_t* readBuf,        // Ptr to incoming data buffer
                         size_t writeCount,       // Number of write bytes
                         size_t readCount)        // Number of read bytes
{
    struct i2c_rdwr_ioctl_data ioctl_data;
    struct i2c_msg ioctl_msgs[2];
    int ret = -1;
    int i;
    int max_retries = 10;

    if (busFd < 0)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: invalid bus fd=%d\n", busFd);
        return ret;
    }

    size_t msgCount = 0;
    if (writeBuf != NULL && writeCount)
    {
        // write message
        ioctl_msgs[msgCount].addr = (short unsigned int)slaveAddr;
        ioctl_msgs[msgCount].len = (short unsigned int)writeCount;
        ioctl_msgs[msgCount].buf = (uint8_t*)(writeBuf);
        ioctl_msgs[msgCount].flags = 0;
        msgCount++;
    }
    if (readBuf != NULL && readCount)
    {
        // read message
        ioctl_msgs[msgCount].addr = (short unsigned int)slaveAddr;
        ioctl_msgs[msgCount].len = (short unsigned int)readCount;
        ioctl_msgs[msgCount].buf = readBuf;
        ioctl_msgs[msgCount].flags = I2C_M_RD;
        msgCount++;
    }
    if (!msgCount)
    {
        return ret;
    }

    ioctl_data.nmsgs = (unsigned int)msgCount;
    ioctl_data.msgs = ioctl_msgs;

    // ioctl
    for (i = 0; i < max_retries; ++i)
    {
        ret = ioctl(busFd, I2C_RDWR, &ioctl_data);
        if (ret == (int)ioctl_data.nmsgs)
        {
            return 0;
        }
    }
    DEBUG_PRINT(PMEM_ERROR, "ERROR: ioctl failed, ret=%d, errno=%d: %s\n", ret,
                errno, strerror(errno));
    return ret;
}

static int i2c_master_write_on_bus(
    int busFd,
    int slaveAddr,           // Target address
    const uint8_t* writeBuf, // Ptr to outgoing data buffer
    size_t writeCount)       // Number of write bytes
{
    return i2c_writeread_on_bus(busFd, slaveAddr, writeBuf, NULL, writeCount,
                                0);
}

int spd_write_dword_register(int busFd, uint8_t slaveAddress, uint8_t dev,
                             uint8_t func, uint16_t reg, uint32_t dword)
{
    int ret = -1;
    const size_t writeSize = SPD_BSP_COMMAND_SIZE + SPD_BSP_LENGTH_SIZE +
                             SPD_BSP_ADDRESS_SIZE + SPD_BSP_DATA_SIZE;
    uint8_t writeBuf[WRITE_ARRAY_SIZE] = {0};

    if (busFd < 0)
    {
        return -1;
    }

    // Address a register to read
    writeBuf[SPD_BSP_CMD_INDEX] = SPD_BSP_CMD_BEGIN_BIT | SPD_BSP_CMD_END_BIT |
                                  SPD_BSP_CMD_WRITE_DWORD |
                                  SPD_BSP_CMD_BLOCK_OP;

    writeBuf[SPD_BSP_WR_LEN_INDEX] = SPD_BSP_ADDRESS_SIZE + SPD_BSP_DATA_SIZE;
    writeBuf[SPD_BSP_RSVD_INDEX] = 0;
    writeBuf[SPD_BSP_DEV_FUN_INDEX] =
        (uint8_t)(((dev << SPD_BPS_DEV_POS_BIT) & SPD_BPS_DEV_MASK) |
                  (func & SPD_BPS_FUN_MASK));
    writeBuf[SPD_BSP_REG_HI_INDEX] = (uint8_t)(reg >> SPD_BPS_HI_BYTE_POS_BIT);
    writeBuf[SPD_BSP_REF_LO_INDEX] = (uint8_t)(reg & SPD_BPS_BYTE_MASK);
    writeBuf[SPD_BSP_WR_DATA_3_INDEX] = (uint8_t)(((uint8_t*)&dword)[3]);
    writeBuf[SPD_BSP_WR_DATA_2_INDEX] = (uint8_t)(((uint8_t*)&dword)[2]);
    writeBuf[SPD_BSP_WR_DATA_1_INDEX] = (uint8_t)(((uint8_t*)&dword)[1]);
    writeBuf[SPD_BSP_WR_DATA_0_INDEX] = (uint8_t)(((uint8_t*)&dword)[0]);
    ret = i2c_master_write_on_bus(busFd, slaveAddress, writeBuf, writeSize);
    if (ret < 0)
    {
        DEBUG_PRINT(PMEM_ERROR, "FAILED: ret=%d: SPD Write double word\n", ret);
    }

    return ret;
}

int spd_read_dword_register(int busFd, uint8_t slaveAddress, uint8_t dev,
                            uint8_t func, uint16_t reg, uint32_t* p_dword)
{
    int ret = -1;
    const size_t writeSize =
        SPD_BSP_COMMAND_SIZE + SPD_BSP_LENGTH_SIZE + SPD_BSP_ADDRESS_SIZE;

    uint8_t writeBuf[WRITE_READ_ARRAY_SIZE] = {0};
    const size_t readSize =
        SPD_BSP_LENGTH_SIZE + SPD_BSP_STATUS_SIZE + SPD_BSP_DATA_SIZE;

    uint8_t readBuf[READ_ARRAY_SIZE] = {0};

    if (NULL == p_dword || busFd < 0)
    {
        return -1;
    }

    // Address a register to read
    writeBuf[SPD_BSP_CMD_INDEX] = SPD_BSP_CMD_BEGIN_BIT | SPD_BSP_CMD_END_BIT |
                                  SPD_BSP_CMD_READ_DWORD | SPD_BSP_CMD_BLOCK_OP;

    writeBuf[SPD_BSP_WR_LEN_INDEX] = SPD_BSP_ADDRESS_SIZE;
    writeBuf[SPD_BSP_RSVD_INDEX] = 0;
    writeBuf[SPD_BSP_DEV_FUN_INDEX] =
        (uint8_t)(((dev << SPD_BPS_DEV_POS_BIT) & SPD_BPS_DEV_MASK) |
                  (func & SPD_BPS_FUN_MASK));
    writeBuf[SPD_BSP_REG_HI_INDEX] = (uint8_t)(reg >> SPD_BPS_HI_BYTE_POS_BIT);
    writeBuf[SPD_BSP_REF_LO_INDEX] = (uint8_t)(reg & SPD_BPS_BYTE_MASK);

    ret = i2c_master_write_on_bus(busFd, slaveAddress, writeBuf, writeSize);
    if (ret < 0)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "FAILED: ret=%d: SPD Read double word, write address\n",
                    ret);
        return ret;
    }
    // Read the data form the register
    ret = i2c_writeread_on_bus(busFd, slaveAddress, writeBuf, readBuf,
                               SPD_BSP_COMMAND_SIZE, readSize);
    if (ret < 0)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "FAILED: ret=%d: SPD Read double word, data read\n", ret);
        return ret;
    }
    // Copy read data
    if (readBuf[SPD_BSP_STATUS_INDEX] <= SPD_BPS_SUCCESS)
    { // Bit success doesn't have to be set
        ((uint8_t*)p_dword)[3] = readBuf[SPD_BSP_RD_DATA_3_INDEX];
        ((uint8_t*)p_dword)[2] = readBuf[SPD_BSP_RD_DATA_2_INDEX];
        ((uint8_t*)p_dword)[1] = readBuf[SPD_BSP_RD_DATA_1_INDEX];
        ((uint8_t*)p_dword)[0] = readBuf[SPD_BSP_RD_DATA_0_INDEX];
    }
    else
    {
        ret = -1;
    }
    return ret;
}

RETURN_STATUS
smbus_read_direct(void* device_addr, unsigned int reg_addr,
                  unsigned int* reg_val)
{
    smbus_device_addr_t* device = (smbus_device_addr_t*)device_addr;
    int status = 0;

    if (device_addr == NULL)
    {
        return RETURN_INVALID_PARAMETER;
    }

    // device 0/func 4 are bps specific to access config registers
    status = spd_read_dword_register(device->i2cBusFd, device->i2cAddr >> 1, 0,
                                     4, (uint16_t)reg_addr, reg_val);
    return status == 0 ? RETURN_SUCCESS : RETURN_DEVICE_ERROR;
}

RETURN_STATUS
smbus_write_direct(void* device_addr, unsigned int reg_addr,
                   unsigned int reg_val)
{
    smbus_device_addr_t* device = (smbus_device_addr_t*)device_addr;
    int status = 0;

    if (device_addr == NULL)
    {
        return RETURN_INVALID_PARAMETER;
    }

    // device 0/func 4 are bps specific to access config registers
    status = spd_write_dword_register(device->i2cBusFd, device->i2cAddr >> 1, 0,
                                      4, (uint16_t)reg_addr, reg_val);
    return status == 0 ? RETURN_SUCCESS : RETURN_DEVICE_ERROR;
}

/**
 * Read the BSR register from specified PMEM through SMBUS
 *
 * @param[in] device_addr SMBus PMEM address
 * @param[out] bsr Bootstatus_register
 * @retval RETURN_INVALID_PARAMETER One or more parameters are invalid
 * @retval RETURN_TIMEOUT Timed out during SMBus communication
 * @retval RETURN_ABORTED Error occurred on SMBus
 * @retval RETURN_SUCCESS Success
 */
RETURN_STATUS
smbus_get_BSR(smbus_device_addr_t device_addr, device_bsr_t* bsr)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    unsigned int BSR_L = 0;
    unsigned int BSR_H = 0;

    if (bsr == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    // Read the upper half first because during activation, the FAC bit is in
    // the upper half and we need to make sure we read the latest lower half in
    // case the FAC bit gets set between reads
    memset(bsr, 0, sizeof(*bsr));
    return_code =
        smb_read_dword(&device_addr, SMB_BOOT_STATUS_REGISER_H, &BSR_H);
    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

    return_code = smb_read_dword(&device_addr, SMB_BOOT_STATUS_REGISER, &BSR_L);

    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

    bsr->AsUint64 = ((uint64_t)BSR_H << 32) | (uint64_t)BSR_L;
Finish:
    return return_code;
}

/**
 * Read the SMBUS mailbox status register from specified PMEM
 *
 * @param[in] device_addr SMBus PMEM address
 * @param[out] status Status Register
 *
 * @retval RETURN_INVALID_PARAMETER One or more parameters are invalid
 * @retval RETURN_TIMEOUT Timed out during SMBus communication
 * @retval RETURN_ABORTED Error occurred on SMBus
 * @retval RETURN_SUCCESS Success
 */
RETURN_STATUS
smbus_get_Mb_status_register(smbus_device_addr_t device_addr, uint32_t* status)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;

    if (status == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    return_code = smb_read_dword(&device_addr, SMB_STATUS_REGISTER, status);

    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

Finish:
    return return_code;
}

/**
 * Check the doorbell bit of the SMBUS mailbox command register from specified
 * PMEM
 *
 * @param[in] device_addr SMBus PMEM address
 * @param[out] pDb Isolated doorbell bit
 *
 * @retval RETURN_INVALID_PARAMETER One or more parameters are invalid
 * @retval RETURN_TIMEOUT Timed out during SMBus communication
 * @retval RETURN_ABORTED Error occurred on SMBus
 * @retval RETURN_SUCCESS Success
 */
RETURN_STATUS smbus_get_Db_cmd_register(smbus_device_addr_t device_addr,
                                        uint64_t* pDb)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    unsigned int command_reg = 0;

    if (pDb == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    return_code = smb_read_dword(&device_addr, SMB_CMD_REGISTER, &command_reg);

    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

    *pDb = (uint64_t)(command_reg & (1 << (DB_SHIFT_32)));

Finish:
    return return_code;
}

/**
 * Poll Firmware command Completion
 * Poll the status register of the mailbox waiting for the
 * mailbox complete bit to be set
 *
 * @param[in] device_addr SMBus PMEM address
 * @param[in] Timeout The timeout, in microseconds, to use for the execution of
 * the protocol command. If Timeout is greater than zero, then this function
 * will return RETURN_TIMEOUT if the time required to execute the receive data
 * command is greater than Timeout.
 * @param[out] pStatus The Fw status to be returned when command completes
 * @param[out] pBsr The boot status register when the command completes OPTIONAL
 *
 * @retval RETURN_INVALID_PARAMETER One or more parameters are invalid
 * @retval RETURN_TIMEOUT Timed out during SMBus communication
 * @retval RETURN_ABORTED Error occurred on SMBus
 * @retval RETURN_SUCCESS Success
 */
RETURN_STATUS
poll_smbus_cmd_completion(smbus_device_addr_t device_addr, uint64_t timeout,
                          uint64_t* status, device_bsr_t* bsr)
{
    RETURN_STATUS return_code = RETURN_DEVICE_ERROR;
    uint64_t db = 0;
    uint64_t delay = 0;

    if (status == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    // Number of delay ticks before timeout
    delay = timeout / (SMBUS_PT_DELAY_PERIOD_IN_US);

    while (delay)
    {
        // Check doorbell cleared
        return_code = smbus_get_Db_cmd_register(device_addr, &db);
        if (RETURN_ERROR(return_code))
        {
            break;
        }
        if (db == 0)
        {
            return_code = RETURN_SUCCESS;
            break;
        }

        // Stall in microseconds
        usleep(SMBUS_PT_DELAY_PERIOD_IN_US);
        delay--;
    }

    // If timeout, still attempt to retrieve MbStatus and BSR
    // The timeout returncode takes precendence though.
    return_code = smbus_get_Mb_status_register(device_addr, (uint32_t*)status);
    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

    if (bsr != NULL)
    {
        return_code = smbus_get_BSR(device_addr, bsr);
        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }
    }

Finish:
    if (delay == 0)
    {
        return_code = RETURN_TIMEOUT;
    }
    return return_code;
}

/**
 * Read the SMBUS Output mailbox from specified PMEM
 *
 * @param[in] deviceaddr SMBus PMEM address
 * @param[out] regular_buffer Outputmailbox to read to
 * @param[in] num_of_bytes Number of bytes to read from OutputMailbox to
 * regular_buffer
 *
 * @retval RETURN_INVALID_PARAMETER One or more parameters are invalid
 * @retval RETURN_ABORTED Error occurred on SMBus
 * @retval RETURN_SUCCESS Success
 */
static RETURN_STATUS
    read_from_smbus_output_mailbox(smbus_device_addr_t device_addr,
                                   void* regular_buffer,
                                   unsigned int num_of_bytes)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint8_t* pTo = NULL;
    unsigned int num_of_segments = 0;
    unsigned int remain = 0;
    unsigned int index = 0;
    unsigned int line_size = SMB_MAILBOX_REGISTER_SIZE;
    unsigned int addr_from = SMB_OUTPAYLOAD_REGISTER_BASE;
    unsigned int remain_value = 0;
    unsigned int read_to = 0;

    if (regular_buffer == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    num_of_segments = num_of_bytes / line_size;
    remain = num_of_bytes % line_size;

    pTo = regular_buffer;

    for (index = 0; index < num_of_segments; index++)
    {
        return_code = smb_read_dword(&device_addr, addr_from, &read_to);

        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }
        memcpy(pTo, &read_to, line_size);
        pTo += line_size;
        addr_from += line_size;
        read_to = 0;
    }

    if (remain > 0)
    {
        return_code = smb_read_dword(&device_addr, addr_from, &remain_value);

        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }

        memcpy(pTo, &remain_value, remain);
    }

Finish:
    return return_code;
}

/**
 * Write the SMBUS input mailbox from specified PMEM
 *
 * @param[in] device_addr SMBus PMEM address
 * @param[out] regular_buffer InputMailbox to read from
 * @param[in] num_of_bytes Number of bytes to read from regular_buffer to Input
 *  Mailbox
 *
 * @retval RETURN_INVALID_PARAMETER One or more parameters are invalid
 * @retval RETURN_ABORTED Error occurred on SMBus
 * @retval RETURN_SUCCESS Success
 */
static RETURN_STATUS
    write_to_smbus_input_mailbox(smbus_device_addr_t device_addr,
                                 void* regular_buffer, uint32_t num_of_bytes)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint8_t* pFrom = NULL;
    uint32_t num_of_segments = 0;
    uint32_t remain = 0;
    uint32_t index = 0;
    uint32_t addr_to = SMB_INPUT_PAYLOAD_REGISTER_BASE;
    uint32_t remain_value = 0;
    uint32_t line_size = SMB_MAILBOX_REGISTER_SIZE;
    uint32_t write_from = 0;

    if (regular_buffer == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    num_of_segments = num_of_bytes / line_size;
    remain = num_of_bytes % line_size;

    pFrom = regular_buffer;

    for (index = 0; index < num_of_segments; index++)
    {
        memcpy(&write_from, pFrom, line_size);
        return_code = smb_write_dword(&device_addr, addr_to, write_from);

        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }

        pFrom += line_size;
        addr_to += line_size;
        write_from = 0;
    }

    if (remain > 0)
    {
        memcpy(&remain_value, pFrom, remain);

        return_code = smb_write_dword(&device_addr, addr_to, remain_value);

        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }
    }

Finish:
    return return_code;
}

/**
 * Pass thru command to FW via SMBUS Mailbox
 * Sends a command to FW and waits for response from firmware
 *
 * @param[in] device_addr PMEM address
 * @param[in,out] fw_cmd A firmware command structure
 * @param[in] Timeout The timeout, in 100ns units, to use for the execution of
 * the protocol command. A Timeout value of 0 means that this function will wait
 * indefinitely for the protocol command to execute. If Timeout is greater than
 * zero, then this function will return RETURN_TIMEOUT if the time required to
 * execute the receive data command is greater than Timeout.
 *
 * @retval RETURN_SUCCESS
 * @retval RETURN_INVALID_PARAMETER Invalid FW command Parameter
 * @retval RETURN_DEVICE_ERROR FW error received
 * @retval RETURN_TIMEOUT A timeout occurred while waiting for the protocol
 * command to execute.
 */

RETURN_STATUS
smbus_pass_thru(smbus_device_addr_t device_addr, fw_cmd_t* fw_cmd,
                uint64_t timeout)
{
    uint64_t return_code = RETURN_SUCCESS;
    device_bsr_t bsr;
    uint64_t command = 0;
    uint64_t status_register = 0;

    if (fw_cmd == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    if (verify_smbus_cmd(fw_cmd) != 0)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    return_code = smbus_get_BSR(device_addr, &bsr);
    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

    if ((bsr.Separated_Current_FIS.MBR != PMEM_BSR_MAILBOX_READY))
    {
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

    if (fw_cmd->inputPayloadSize > 0)
    {
        return_code = write_to_smbus_input_mailbox(
            device_addr, fw_cmd->inputPayload, fw_cmd->inputPayloadSize);

        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }
    }

    command = ((uint64_t)fw_cmd->opcode << OP_SHIFT) |
              ((uint64_t)fw_cmd->subOpcode << SUB_OP_SHIFT |
               ((uint64_t)1 << DB_SHIFT));

    smb_write_dword(&device_addr, SMB_CMD_REGISTER,
                    (unsigned int)(command >> 32));

    return_code =
        poll_smbus_cmd_completion(device_addr, timeout, &status_register, NULL);

    fw_cmd->status = (status_register >> 8) & STATUS_MASK;

    if (RETURN_ERROR(return_code))
    {
        goto Finish;
    }

    if (fw_cmd->status)
    {
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

    if (!(status_register & MB_COMPLETE))
    {
        return_code = RETURN_DEVICE_ERROR;
    }

    if (fw_cmd->outputPayloadSize > 0)
    {
        return_code = read_from_smbus_output_mailbox(
            device_addr, fw_cmd->outPayload, fw_cmd->outputPayloadSize);

        if (RETURN_ERROR(return_code))
        {
            goto Finish;
        }
    }

Finish:
    return return_code;
}

/**
 * Verify Smbus FW command
 * Perform boundary checks to ensure that the fw command can be executed on FW
 *
 * @param[in] fw_cmd The FW command to verify
 *
 * @retval RETURN_SUCCESS Success
 * @retval RETURN_INVALID_PARAMETER Invalid FW command Parameter
 */
RETURN_STATUS
verify_smbus_cmd(fw_cmd_t* fw_cmd)
{
    if (fw_cmd == NULL)
    {
        return RETURN_INVALID_PARAMETER;
    }
    if (fw_cmd->inputPayloadSize > IN_PAYLOAD_SIZE)
    {
        return RETURN_INVALID_PARAMETER;
    }

    if (fw_cmd->outputPayloadSize > OUT_PAYLOAD_SIZE)
    {
        return RETURN_INVALID_PARAMETER;
    }

    if (fw_cmd->largeInputPayloadSize > 0)
    {
        return RETURN_INVALID_PARAMETER;
    }

    if (fw_cmd->largeOutputPayloadSize > 0)
    {
        return RETURN_INVALID_PARAMETER;
    }

    if (fw_cmd->opcode > PtMax)
    {
        return RETURN_INVALID_PARAMETER;
    }

    return RETURN_SUCCESS;
}
