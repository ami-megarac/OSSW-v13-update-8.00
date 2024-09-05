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

#ifndef __SMBUS_DIRECT_H__
#define __SMBUS_DIRECT_H__

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcpp"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#pragma GCC diagnostic pop

#include "basetype.h"
#include "fwutility.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PtMax 0xF2

// Smbus delay period in microseconds
#define SMBUS_PT_DELAY_PERIOD_IN_US 100

#define PMEM_BSR_MAJOR_NO_POST_CODE 0x0
#define PMEM_BSR_MAJOR_CHECKPOINT_INIT_FAILURE 0xA1  // Unrecoverable FW error
#define PMEM_BSR_MAJOR_CHECKPOINT_CPU_EXCEPTION 0xE1 // CPU exception
#define PMEM_BSR_MAJOR_CHECKPOINT_INIT_COMPLETE                                \
    0xF0 // FW initialization complete
#define PMEM_BSR_MAILBOX_READY 0x1
#define PMEM_BSR_MAILBOX_NOT_READY 0x0
#define PMEM_BSR_MEDIA_NOT_TRAINED 0x0
#define PMEM_BSR_MEDIA_TRAINED 0x1
#define PMEM_BSR_MEDIA_ERROR 0x2
#define PMEM_BSR_MEDIA_DISABLED 0x1
#define PMEM_BSR_ACTIVATION_COMPLETED 0x1

// FIS >= 1.5
#define PMEM_BSR_AIT_DRAM_NOTTRAINED 0x0
#define PMEM_BSR_AIT_DRAM_TRAINED_NOTLOADED 0x1
#define PMEM_BSR_AIT_DRAM_ERROR 0x2
#define PMEM_BSR_AIT_DRAM_TRAINED_LOADED_READY 0x3

#define PMEM_BSR_OIE_ENABLED 0x1
#define PMEM_BSR_REBOOT_REQUIRED 0x1
#define PMEM_BSR_MEDIA_INTERFACE_ENGINE_STALLED 0x01

#define REGISTER_BSR_STR "BSR"
#define REGISTER_OS_STR "OS"

    typedef union
    {
        uint64_t AsUint64;
        struct
        {
            uint64_t Major : 8; // 7:0
            uint64_t Minor : 8; // 15:8
            uint64_t MR : 2;    // 17:16
            uint64_t DT : 1;    // 18
            uint64_t PCR : 1;   // 19
            uint64_t MBR : 1;   // 20
            uint64_t WTS : 1;   // 21
            uint64_t FRCF : 1;  // 22
            uint64_t CR : 1;    // 23
            uint64_t MD : 1;    // 24
            uint64_t OIE : 1;   // 25
            uint64_t OIWE : 1;  // 26
            uint64_t DR : 2;    // 28:27
            uint64_t RR : 1;    // 29
            uint64_t Rsvd : 34; // 63:30
        } Separated_FIS_1_13;
        struct
        {
            uint64_t Major : 8;  // 7:0
            uint64_t Minor : 8;  // 15:8
            uint64_t MR : 2;     // 17:16
            uint64_t DT : 1;     // 18
            uint64_t PCR : 1;    // 19
            uint64_t MBR : 1;    // 20
            uint64_t WTS : 1;    // 21
            uint64_t FRCF : 1;   // 22
            uint64_t CR : 1;     // 23
            uint64_t MD : 1;     // 24
            uint64_t OIE : 1;    // 25
            uint64_t OIWE : 1;   // 26
            uint64_t DR : 2;     // 28:27
            uint64_t RR : 1;     // 29
            uint64_t LFOPB : 1;  // 30
            uint64_t SVNWC : 1;  // 31
            uint64_t Rsvd : 2;   // 33:32
            uint64_t DTS : 2;    // 35:34
            uint64_t FAC : 1;    // 36
            uint64_t Rsvd1 : 27; // 63:37
        } Separated_Current_FIS;
    } device_bsr_t;

    /**
     * SMBus DEVICE address
     */
    typedef struct _smbus_device_addr
    {
        int i2cBusFd;
        uint8_t i2cAddr;
        uint32_t spdSmbCmdAddr;
        uint32_t spdSmbStatusAddr;
        uint32_t spdSmbDataAddr;
        uint32_t spdSmbLogicalChannel;
        uint32_t socketLogicalChannel;
    } smbus_device_addr_t;

    typedef RETURN_STATUS (*smbus_read_interface)(void* device_addr,
                                                  unsigned int reg_addr,
                                                  unsigned int* reg_val);

    typedef RETURN_STATUS (*smbus_write_interface)(void* device_addr,
                                                   unsigned int reg_addr,
                                                   unsigned int reg_val);

    RETURN_STATUS
    smbus_read_direct(void* device_addr, unsigned int reg_addr,
                      unsigned int* reg_val);

    RETURN_STATUS
    smbus_write_direct(void* device_addr, unsigned int reg_addr,
                       unsigned int reg_val);

    RETURN_STATUS
    verify_smbus_cmd(fw_cmd_t* fw_cmd);

    int spd_write_dword_register(int busFd, uint8_t slaveAddress, uint8_t dev,
                                 uint8_t func, uint16_t reg, uint32_t dword);

    int spd_read_dword_register(int busFd, uint8_t slaveAddress, uint8_t dev,
                                uint8_t func, uint16_t reg, uint32_t* p_dword);

    RETURN_STATUS smbus_get_Db_cmd_register(smbus_device_addr_t device_addr,
                                            uint64_t* pDb);

    RETURN_STATUS smbus_get_Mb_status_register(smbus_device_addr_t device_addr,
                                               uint32_t* status);

    RETURN_STATUS
    smbus_get_BSR(smbus_device_addr_t device_addr, device_bsr_t* bsr);

    RETURN_STATUS
    poll_smbus_cmd_completion(smbus_device_addr_t device_addr, uint64_t timeout,
                              uint64_t* status, device_bsr_t* bsr);

    RETURN_STATUS
    smbus_pass_thru(smbus_device_addr_t device_addr, fw_cmd_t* fw_cmd,
                    uint64_t timeout);

    int i2c_open_bus(const char* path);

    void set_smbus_interface(smbus_read_interface pReadApi,
                             smbus_write_interface pWriteApi);

#define PCI_LIB_ADDRESS(Bus, Device, Function, Register)                       \
    (((Register)&0xfff) | (((Function)&0x07) << 12) |                          \
     (((Device)&0x1f) << 15) | (((Bus)&0xff) << 20))

#ifdef __cplusplus
}
#endif

#endif
