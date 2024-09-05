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

#ifndef __SMBUS_PECI_H__
#define __SMBUS_PECI_H__

#pragma once
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wcast-align"

#include "basetype.h"
#include "common.h"
#include "fwutility.h"
#include "smbus_direct.h"

#include "peci.h"
#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SMB_CSR_REG_NAME_LEN 34
#define IMCS_PER_SOCKET 4
#define BUSES_PER_SOCKET 2
#define CHANNELS_PER_IMC 2
#define SLOTS_PER_CHANNEL 2

// mb_smus_cmd fields
#define SEQUENCE_BIT  31
#define DOORBELL_BIT  16
#define SUBOPCODE_BIT  8
#define OPCODE_BIT     0

    // Reference: Crow Pass Controller EDS doc#631921
    typedef struct
    {
        char name[SMB_CSR_REG_NAME_LEN];
        uint8_t dev;
        uint8_t func;
        uint16_t offset;
    } SmBusCsrEntry;

    typedef struct
    {
        char name[SMB_CSR_REG_NAME_LEN];
        uint8_t seg;
        uint8_t bus;
        uint8_t dev;
        uint8_t func;
        uint16_t offset;
        uint8_t size;
    } RegEntry;

    // A structure to store the conversion between
    // bus/saddr and imc/channel/slot indexes
    typedef struct
    {
        uint8_t bus;
        uint8_t saddr;
        uint8_t imc_index;
        uint8_t channel_index;
        uint8_t slot_index;
    } BusAddrAndLoc;

    RETURN_STATUS peci_pass_thru(peci_device_addr_t device_addr,
                                 fw_cmd_t* fw_cmd);

    PECIStatus api(const int addr, const uint8_t device, const ApiParams params,
                   uint32_t* payload, const uint32_t payloadSize, uint8_t* cc);

    EPECIStatus regRd(int addr, RegEntry reg, uint32_t* val, uint8_t* cc);

    EPECIStatus regWr(int addr, RegEntry reg, uint32_t val, uint8_t* cc);

    PECIStatus smbusRd(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                       uint8_t bytewidth, uint32_t* val, uint8_t* cc,
                       bool check);

    PECIStatus smbusWr(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                       uint8_t bytewidth, uint32_t val, uint8_t* cc,
                       bool check);

    PECIStatus csrRd(int addr, uint8_t device, SmBusCsrEntry csr, uint32_t* val,
                     uint8_t* cc, bool check);

    PECIStatus csrWr(int addr, uint8_t device, SmBusCsrEntry csr, uint32_t val,
                     uint8_t* cc, bool check);

    uint16_t discovery(const int addr, uint16_t* mask);

    RETURN_STATUS
    peci_get_BSR(peci_device_addr_t* device_addr, device_bsr_t* bsr);

    RETURN_STATUS peci_get_Db_cmd_register(peci_device_addr_t* device_addr,
                                           uint64_t* pDb);

    RETURN_STATUS peci_get_Mb_status_register(peci_device_addr_t* device_addr,
                                              uint32_t* status);

    RETURN_STATUS peci_send_activation(peci_device_addr_t* device_addr);

    // Notes: The list order is important for accessing register BDF correctly.
    typedef enum
    {
        a_perm_err_log,
        a_ffw_digest1_csr_0,
        a_ffw_digest1_csr_1,
        a_ffw_digest1_csr_2,
        a_ffw_digest1_csr_3,
        a_ffw_digest1_csr_4,
        a_ffw_digest1_csr_5,
        a_ffw_digest1_csr_6,
        a_ffw_digest1_csr_7,
        a_ffw_digest_csr_0,
        a_ffw_digest_csr_1,
        a_ffw_digest_csr_2,
        a_ffw_digest_csr_3,
        a_ffw_digest_csr_4,
        a_ffw_digest_csr_5,
        a_ffw_digest_csr_6,
        a_ffw_digest_csr_7,
        d_fw_status,
        d_fw_status_h,
        dn_perm_err_log,
        ddrt_perm_err_log,
        ddrt_dn_cfg_unimpl_log,
        scratch_0,
        scratch_1,
        scratch_2,
        scratch_3,
        fw_revid,
        fw_misc_0,
        fw_misc_1,
        fw_misc_2,
        fw_misc_3,
        fw_misc_4,
        fw_misc_5,
        fw_misc_6,
        fw_misc_7,
        fw_misc_8,
        fw_misc_9,
        fw_misc_10,
        fw_misc_11,
        mb_smm_cmd_spare,
        mb_smm_cmd,
        mb_smm_nonce_0,
        mb_smm_nonce_1,
        mb_smm_nonce_2,
        mb_smm_nonce_3,
        mb_smm_input_payload_0,
        mb_smm_input_payload_1,
        mb_smm_input_payload_2,
        mb_smm_input_payload_3,
        mb_smm_input_payload_4,
        mb_smm_input_payload_5,
        mb_smm_input_payload_6,
        mb_smm_input_payload_7,
        mb_smm_input_payload_8,
        mb_smm_input_payload_9,
        mb_smm_input_payload_10,
        mb_smm_input_payload_11,
        mb_smm_input_payload_12,
        mb_smm_input_payload_13,
        mb_smm_input_payload_14,
        mb_smm_input_payload_15,
        mb_smm_input_payload_16,
        mb_smm_input_payload_17,
        mb_smm_input_payload_18,
        mb_smm_input_payload_19,
        mb_smm_input_payload_20,
        mb_smm_input_payload_21,
        mb_smm_input_payload_22,
        mb_smm_input_payload_23,
        mb_smm_input_payload_24,
        mb_smm_input_payload_25,
        mb_smm_input_payload_26,
        mb_smm_input_payload_27,
        mb_smm_input_payload_28,
        mb_smm_input_payload_29,
        mb_smm_input_payload_30,
        mb_smm_input_payload_31,
        mb_smm_status,
        mb_smm_spare_status,
        mb_smm_output_payload_0,
        mb_smm_output_payload_1,
        mb_smm_output_payload_2,
        mb_smm_output_payload_3,
        mb_smm_output_payload_4,
        mb_smm_output_payload_5,
        mb_smm_output_payload_6,
        mb_smm_output_payload_7,
        mb_smm_output_payload_8,
        mb_smm_output_payload_9,
        mb_smm_output_payload_10,
        mb_smm_output_payload_11,
        mb_smm_output_payload_12,
        mb_smm_output_payload_13,
        mb_smm_output_payload_14,
        mb_smm_output_payload_15,
        mb_smm_output_payload_16,
        mb_smm_output_payload_17,
        mb_smm_output_payload_18,
        mb_smm_output_payload_19,
        mb_smm_output_payload_20,
        mb_smm_output_payload_21,
        mb_smm_output_payload_22,
        mb_smm_output_payload_23,
        mb_smm_output_payload_24,
        mb_smm_output_payload_25,
        mb_smm_output_payload_26,
        mb_smm_output_payload_27,
        mb_smm_output_payload_28,
        mb_smm_output_payload_29,
        mb_smm_output_payload_30,
        mb_smm_output_payload_31,
        mb_smm_abort,
        mb_os_cmd_spare,
        mb_os_cmd,
        sxextflushstate_0,
        sxextflushstate_1,
        extflushstate_0,
        extflushstate_1,
        mb_os_input_payload_0,
        mb_os_input_payload_1,
        mb_os_input_payload_2,
        mb_os_input_payload_3,
        mb_os_input_payload_4,
        mb_os_input_payload_5,
        mb_os_input_payload_6,
        mb_os_input_payload_7,
        mb_os_input_payload_8,
        mb_os_input_payload_9,
        mb_os_input_payload_10,
        mb_os_input_payload_11,
        mb_os_input_payload_12,
        mb_os_input_payload_13,
        mb_os_input_payload_14,
        mb_os_input_payload_15,
        mb_os_input_payload_16,
        mb_os_input_payload_17,
        mb_os_input_payload_18,
        mb_os_input_payload_19,
        mb_os_input_payload_20,
        mb_os_input_payload_21,
        mb_os_input_payload_22,
        mb_os_input_payload_23,
        mb_os_input_payload_24,
        mb_os_input_payload_25,
        mb_os_input_payload_26,
        mb_os_input_payload_27,
        mb_os_input_payload_28,
        mb_os_input_payload_29,
        mb_os_input_payload_30,
        mb_os_input_payload_31,
        mb_os_status,
        mb_os_spare_status,
        mb_os_output_payload_0,
        mb_os_output_payload_1,
        mb_os_output_payload_2,
        mb_os_output_payload_3,
        mb_os_output_payload_4,
        mb_os_output_payload_5,
        mb_os_output_payload_6,
        mb_os_output_payload_7,
        mb_os_output_payload_8,
        mb_os_output_payload_9,
        mb_os_output_payload_10,
        mb_os_output_payload_11,
        mb_os_output_payload_12,
        mb_os_output_payload_13,
        mb_os_output_payload_14,
        mb_os_output_payload_15,
        mb_os_output_payload_16,
        mb_os_output_payload_17,
        mb_os_output_payload_18,
        mb_os_output_payload_19,
        mb_os_output_payload_20,
        mb_os_output_payload_21,
        mb_os_output_payload_22,
        mb_os_output_payload_23,
        mb_os_output_payload_24,
        mb_os_output_payload_25,
        mb_os_output_payload_26,
        mb_os_output_payload_27,
        mb_os_output_payload_28,
        mb_os_output_payload_29,
        mb_os_output_payload_30,
        mb_os_output_payload_31,
        mb_os_abort,
        mb_oob_cmd_spare,
        mb_oob_cmd,
        mb_oob_nonce_0,
        mb_oob_nonce_1,
        mb_oob_nonce_2,
        mb_oob_nonce_3,
        mb_oob_input_payload_0,
        mb_oob_input_payload_1,
        mb_oob_input_payload_2,
        mb_oob_input_payload_3,
        mb_oob_input_payload_4,
        mb_oob_input_payload_5,
        mb_oob_input_payload_6,
        mb_oob_input_payload_7,
        mb_oob_input_payload_8,
        mb_oob_input_payload_9,
        mb_oob_input_payload_10,
        mb_oob_input_payload_11,
        mb_oob_input_payload_12,
        mb_oob_input_payload_13,
        mb_oob_input_payload_14,
        mb_oob_input_payload_15,
        mb_oob_input_payload_16,
        mb_oob_input_payload_17,
        mb_oob_input_payload_18,
        mb_oob_input_payload_19,
        mb_oob_input_payload_20,
        mb_oob_input_payload_21,
        mb_oob_input_payload_22,
        mb_oob_input_payload_23,
        mb_oob_input_payload_24,
        mb_oob_input_payload_25,
        mb_oob_input_payload_26,
        mb_oob_input_payload_27,
        mb_oob_input_payload_28,
        mb_oob_input_payload_29,
        mb_oob_input_payload_30,
        mb_oob_input_payload_31,
        mb_oob_status,
        mb_oob_spare_status,
        mb_oob_output_payload_0,
        mb_oob_output_payload_1,
        mb_oob_output_payload_2,
        mb_oob_output_payload_3,
        mb_oob_output_payload_4,
        mb_oob_output_payload_5,
        mb_oob_output_payload_6,
        mb_oob_output_payload_7,
        mb_oob_output_payload_8,
        mb_oob_output_payload_9,
        mb_oob_output_payload_10,
        mb_oob_output_payload_11,
        mb_oob_output_payload_12,
        mb_oob_output_payload_13,
        mb_oob_output_payload_14,
        mb_oob_output_payload_15,
        mb_oob_output_payload_16,
        mb_oob_output_payload_17,
        mb_oob_output_payload_18,
        mb_oob_output_payload_19,
        mb_oob_output_payload_20,
        mb_oob_output_payload_21,
        mb_oob_output_payload_22,
        mb_oob_output_payload_23,
        mb_oob_output_payload_24,
        mb_oob_output_payload_25,
        mb_oob_output_payload_26,
        mb_oob_output_payload_27,
        mb_oob_output_payload_28,
        mb_oob_output_payload_29,
        mb_oob_output_payload_30,
        mb_oob_output_payload_31,
        mb_oob_abort,
        mb_smbus_cmd_spare,
        mb_smbus_cmd,
        mb_smbus_nonce_0,
        mb_smbus_nonce_1,
        mb_smbus_nonce_2,
        mb_smbus_nonce_3,
        mb_smbus_input_payload_0,
        mb_smbus_input_payload_1,
        mb_smbus_input_payload_2,
        mb_smbus_input_payload_3,
        mb_smbus_input_payload_4,
        mb_smbus_input_payload_5,
        mb_smbus_input_payload_6,
        mb_smbus_input_payload_7,
        mb_smbus_input_payload_8,
        mb_smbus_input_payload_9,
        mb_smbus_input_payload_10,
        mb_smbus_input_payload_11,
        mb_smbus_input_payload_12,
        mb_smbus_input_payload_13,
        mb_smbus_input_payload_14,
        mb_smbus_input_payload_15,
        mb_smbus_input_payload_16,
        mb_smbus_input_payload_17,
        mb_smbus_input_payload_18,
        mb_smbus_input_payload_19,
        mb_smbus_input_payload_20,
        mb_smbus_input_payload_21,
        mb_smbus_input_payload_22,
        mb_smbus_input_payload_23,
        mb_smbus_input_payload_24,
        mb_smbus_input_payload_25,
        mb_smbus_input_payload_26,
        mb_smbus_input_payload_27,
        mb_smbus_input_payload_28,
        mb_smbus_input_payload_29,
        mb_smbus_input_payload_30,
        mb_smbus_input_payload_31,
        mb_smbus_status,
        mb_smbus_spare_status,
        mb_smbus_output_payload_0,
        mb_smbus_output_payload_1,
        mb_smbus_output_payload_2,
        mb_smbus_output_payload_3,
        mb_smbus_output_payload_4,
        mb_smbus_output_payload_5,
        mb_smbus_output_payload_6,
        mb_smbus_output_payload_7,
        mb_smbus_output_payload_8,
        mb_smbus_output_payload_9,
        mb_smbus_output_payload_10,
        mb_smbus_output_payload_11,
        mb_smbus_output_payload_12,
        mb_smbus_output_payload_13,
        mb_smbus_output_payload_14,
        mb_smbus_output_payload_15,
        mb_smbus_output_payload_16,
        mb_smbus_output_payload_17,
        mb_smbus_output_payload_18,
        mb_smbus_output_payload_19,
        mb_smbus_output_payload_20,
        mb_smbus_output_payload_21,
        mb_smbus_output_payload_22,
        mb_smbus_output_payload_23,
        mb_smbus_output_payload_24,
        mb_smbus_output_payload_25,
        mb_smbus_output_payload_26,
        mb_smbus_output_payload_27,
        mb_smbus_output_payload_28,
        mb_smbus_output_payload_29,
        mb_smbus_output_payload_30,
        mb_smbus_output_payload_31,
        mb_smbus_abort,
        ds_perm_err_log,
        i3c_perm_err_log,
        temp_csr,
        sgx_a,
        vendor_device_id,
        revision_mfg_id,
        fnv_rom_ctrl,
        fnv_perpartid_l,
        fnv_perpartid_h,
        fnv_sku_num,
        fnv_fuse_ctrl,
        tsod_mirror_csr,
        fnv_aep_mfg,
        jtag_payload_0,
        jtag_payload_1,
        jtag_payload_2,
        jtag_payload_3,
        jtag_tunnel_ctrl,
        fnv_debug_lock,
        msc_feature_stat_0,
        smbus_controls,
        smbus_xtrigger,
        die_temp_csr,
        msc_temp_override_csr,
        msc_temp_cntrl_csr,
        msc_temp_low_csr,
        msc_temp_high_csr,
        msc_temp_crit_csr,
        msc_temp_cal_csr,
        msc_temp_hipcntrl_csr,
        msc_temp_status_csr,
        sgx_b,
        numberOfCsrRegisters,
    } SMBUS_CSR;

    extern const SmBusCsrEntry csrs[];

#ifdef __cplusplus
}
#endif

#endif
