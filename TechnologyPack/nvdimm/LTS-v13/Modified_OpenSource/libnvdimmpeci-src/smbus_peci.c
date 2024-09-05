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

#include "smbus_peci.h"

#include "i3c_peci.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define SMBUS_TIMEOUT_INTERVAL_MS 1
#define SMBUS_TIMEOUT_MS 10
#define SMBUS_WAIT_LOOPS 100
#define MAX_DEVICE_PER_BUS 8
#define BASE_ADDR 0xb0
#define API_CHECK_INTERVAL_US 1000000
#define API_MAX_RETRIES 10

const RegEntry regs[] = {
    // Register name, Seg, Bus, Dev, Func, Offset
    {"smb_cmd_cfg_spd0", 0x0, 0xd, 0xb, 0, 0x80, 4},
    {"smb_cmd_cfg_spd1", 0x0, 0xd, 0xb, 1, 0x80, 4},
    {"smb_data_cfg_spd0", 0x0, 0xd, 0xb, 0, 0x88, 4},
    {"smb_data_cfg_spd1", 0x0, 0xd, 0xb, 1, 0x88, 4},
    {"smb_status_cfg_spd0", 0x0, 0xd, 0xb, 0, 0x84, 4},
    {"smb_status_cfg_spd1", 0x0, 0xd, 0xb, 1, 0x84, 4},
};

// Notes: The list order is important for accessing register BDF correctly.
enum regname
{
    smb_cmd_cfg_spd0,
    smb_cmd_cfg_spd1,
    smb_data_cfg_spd0,
    smb_data_cfg_spd1,
    smb_status_cfg_spd0,
    smb_status_cfg_spd1,
};

const SmBusCsrEntry csrs[] = {{"a_perm_err_log", 6, 6, 0x140},
                              {"a_ffw_digest1_csr_0", 6, 6, 0x2a0},
                              {"a_ffw_digest1_csr_1", 6, 6, 0x2a4},
                              {"a_ffw_digest1_csr_2", 6, 6, 0x2a8},
                              {"a_ffw_digest1_csr_3", 6, 6, 0x2ac},
                              {"a_ffw_digest1_csr_4", 6, 6, 0x2b0},
                              {"a_ffw_digest1_csr_5", 6, 6, 0x2b4},
                              {"a_ffw_digest1_csr_6", 6, 6, 0x2b8},
                              {"a_ffw_digest1_csr_7", 6, 6, 0x2bc},
                              {"a_ffw_digest_csr_0", 6, 6, 0x2d0},
                              {"a_ffw_digest_csr_1", 6, 6, 0x2d4},
                              {"a_ffw_digest_csr_2", 6, 6, 0x2d8},
                              {"a_ffw_digest_csr_3", 6, 6, 0x2dc},
                              {"a_ffw_digest_csr_4", 6, 6, 0x2e0},
                              {"a_ffw_digest_csr_5", 6, 6, 0x2e4},
                              {"a_ffw_digest_csr_6", 6, 6, 0x2e8},
                              {"a_ffw_digest_csr_7", 6, 6, 0x2ec},
                              {"d_fw_status", 4, 6, 0x000},
                              {"d_fw_status_h", 4, 6, 0x004},
                              {"dn_perm_err_log", 4, 6, 0x268},
                              {"ddrt_perm_err_log", 4, 6, 0x6f8},
                              {"ddrt_dn_cfg_unimpl_log", 4, 6, 0x778},
                              {"scratch_0", 0, 6, 0x000},
                              {"scratch_1", 0, 6, 0x004},
                              {"scratch_2", 0, 6, 0x008},
                              {"scratch_3", 0, 6, 0x00c},
                              {"fw_revid", 0, 6, 0x020},
                              {"fw_misc_0", 0, 6, 0x030},
                              {"fw_misc_1", 0, 6, 0x034},
                              {"fw_misc_2", 0, 6, 0x038},
                              {"fw_misc_3", 0, 6, 0x03c},
                              {"fw_misc_4", 0, 6, 0x040},
                              {"fw_misc_5", 0, 6, 0x044},
                              {"fw_misc_6", 0, 6, 0x048},
                              {"fw_misc_7", 0, 6, 0x04c},
                              {"fw_misc_8", 0, 6, 0x050},
                              {"fw_misc_9", 0, 6, 0x054},
                              {"fw_misc_10", 0, 6, 0x058},
                              {"fw_misc_11", 0, 6, 0x05c},
                              {"mb_smm_cmd_spare", 0, 6, 0x200},
                              {"mb_smm_cmd", 0, 6, 0x204},
                              {"mb_smm_nonce_0", 0, 6, 0x208},
                              {"mb_smm_nonce_1", 0, 6, 0x20c},
                              {"mb_smm_nonce_2", 0, 6, 0x210},
                              {"mb_smm_nonce_3", 0, 6, 0x214},
                              {"mb_smm_input_payload_0", 0, 6, 0x218},
                              {"mb_smm_input_payload_1", 0, 6, 0x21c},
                              {"mb_smm_input_payload_2", 0, 6, 0x220},
                              {"mb_smm_input_payload_3", 0, 6, 0x224},
                              {"mb_smm_input_payload_4", 0, 6, 0x228},
                              {"mb_smm_input_payload_5", 0, 6, 0x22c},
                              {"mb_smm_input_payload_6", 0, 6, 0x230},
                              {"mb_smm_input_payload_7", 0, 6, 0x234},
                              {"mb_smm_input_payload_8", 0, 6, 0x238},
                              {"mb_smm_input_payload_9", 0, 6, 0x23c},
                              {"mb_smm_input_payload_10", 0, 6, 0x240},
                              {"mb_smm_input_payload_11", 0, 6, 0x244},
                              {"mb_smm_input_payload_12", 0, 6, 0x248},
                              {"mb_smm_input_payload_13", 0, 6, 0x24c},
                              {"mb_smm_input_payload_14", 0, 6, 0x250},
                              {"mb_smm_input_payload_15", 0, 6, 0x254},
                              {"mb_smm_input_payload_16", 0, 6, 0x258},
                              {"mb_smm_input_payload_17", 0, 6, 0x25c},
                              {"mb_smm_input_payload_18", 0, 6, 0x260},
                              {"mb_smm_input_payload_19", 0, 6, 0x264},
                              {"mb_smm_input_payload_20", 0, 6, 0x268},
                              {"mb_smm_input_payload_21", 0, 6, 0x26c},
                              {"mb_smm_input_payload_22", 0, 6, 0x270},
                              {"mb_smm_input_payload_23", 0, 6, 0x274},
                              {"mb_smm_input_payload_24", 0, 6, 0x278},
                              {"mb_smm_input_payload_25", 0, 6, 0x27c},
                              {"mb_smm_input_payload_26", 0, 6, 0x280},
                              {"mb_smm_input_payload_27", 0, 6, 0x284},
                              {"mb_smm_input_payload_28", 0, 6, 0x288},
                              {"mb_smm_input_payload_29", 0, 6, 0x28c},
                              {"mb_smm_input_payload_30", 0, 6, 0x290},
                              {"mb_smm_input_payload_31", 0, 6, 0x294},
                              {"mb_smm_status", 0, 6, 0x298},
                              {"mb_smm_spare_status", 0, 6, 0x29c},
                              {"mb_smm_output_payload_0", 0, 6, 0x2a0},
                              {"mb_smm_output_payload_1", 0, 6, 0x2a4},
                              {"mb_smm_output_payload_2", 0, 6, 0x2a8},
                              {"mb_smm_output_payload_3", 0, 6, 0x2ac},
                              {"mb_smm_output_payload_4", 0, 6, 0x2b0},
                              {"mb_smm_output_payload_5", 0, 6, 0x2b4},
                              {"mb_smm_output_payload_6", 0, 6, 0x2b8},
                              {"mb_smm_output_payload_7", 0, 6, 0x2bc},
                              {"mb_smm_output_payload_8", 0, 6, 0x2c0},
                              {"mb_smm_output_payload_9", 0, 6, 0x2c4},
                              {"mb_smm_output_payload_10", 0, 6, 0x2c8},
                              {"mb_smm_output_payload_11", 0, 6, 0x2cc},
                              {"mb_smm_output_payload_12", 0, 6, 0x2d0},
                              {"mb_smm_output_payload_13", 0, 6, 0x2d4},
                              {"mb_smm_output_payload_14", 0, 6, 0x2d8},
                              {"mb_smm_output_payload_15", 0, 6, 0x2dc},
                              {"mb_smm_output_payload_16", 0, 6, 0x2e0},
                              {"mb_smm_output_payload_17", 0, 6, 0x2e4},
                              {"mb_smm_output_payload_18", 0, 6, 0x2e8},
                              {"mb_smm_output_payload_19", 0, 6, 0x2ec},
                              {"mb_smm_output_payload_20", 0, 6, 0x2f0},
                              {"mb_smm_output_payload_21", 0, 6, 0x2f4},
                              {"mb_smm_output_payload_22", 0, 6, 0x2f8},
                              {"mb_smm_output_payload_23", 0, 6, 0x2fc},
                              {"mb_smm_output_payload_24", 0, 6, 0x300},
                              {"mb_smm_output_payload_25", 0, 6, 0x304},
                              {"mb_smm_output_payload_26", 0, 6, 0x308},
                              {"mb_smm_output_payload_27", 0, 6, 0x30c},
                              {"mb_smm_output_payload_28", 0, 6, 0x310},
                              {"mb_smm_output_payload_29", 0, 6, 0x314},
                              {"mb_smm_output_payload_30", 0, 6, 0x318},
                              {"mb_smm_output_payload_31", 0, 6, 0x31c},
                              {"mb_smm_abort", 0, 6, 0x320},
                              {"mb_os_cmd_spare", 0, 6, 0x400},
                              {"mb_os_cmd", 0, 6, 0x404},
                              {"sxextflushstate_0", 0, 6, 0x408},
                              {"sxextflushstate_1", 0, 6, 0x40c},
                              {"extflushstate_0", 0, 6, 0x410},
                              {"extflushstate_1", 0, 6, 0x414},
                              {"mb_os_input_payload_0", 0, 6, 0x418},
                              {"mb_os_input_payload_1", 0, 6, 0x41c},
                              {"mb_os_input_payload_2", 0, 6, 0x420},
                              {"mb_os_input_payload_3", 0, 6, 0x424},
                              {"mb_os_input_payload_4", 0, 6, 0x428},
                              {"mb_os_input_payload_5", 0, 6, 0x42c},
                              {"mb_os_input_payload_6", 0, 6, 0x430},
                              {"mb_os_input_payload_7", 0, 6, 0x434},
                              {"mb_os_input_payload_8", 0, 6, 0x438},
                              {"mb_os_input_payload_9", 0, 6, 0x43c},
                              {"mb_os_input_payload_10", 0, 6, 0x440},
                              {"mb_os_input_payload_11", 0, 6, 0x444},
                              {"mb_os_input_payload_12", 0, 6, 0x448},
                              {"mb_os_input_payload_13", 0, 6, 0x44c},
                              {"mb_os_input_payload_14", 0, 6, 0x450},
                              {"mb_os_input_payload_15", 0, 6, 0x454},
                              {"mb_os_input_payload_16", 0, 6, 0x458},
                              {"mb_os_input_payload_17", 0, 6, 0x45c},
                              {"mb_os_input_payload_18", 0, 6, 0x460},
                              {"mb_os_input_payload_19", 0, 6, 0x464},
                              {"mb_os_input_payload_20", 0, 6, 0x468},
                              {"mb_os_input_payload_21", 0, 6, 0x46c},
                              {"mb_os_input_payload_22", 0, 6, 0x470},
                              {"mb_os_input_payload_23", 0, 6, 0x474},
                              {"mb_os_input_payload_24", 0, 6, 0x478},
                              {"mb_os_input_payload_25", 0, 6, 0x47c},
                              {"mb_os_input_payload_26", 0, 6, 0x480},
                              {"mb_os_input_payload_27", 0, 6, 0x484},
                              {"mb_os_input_payload_28", 0, 6, 0x488},
                              {"mb_os_input_payload_29", 0, 6, 0x48c},
                              {"mb_os_input_payload_30", 0, 6, 0x490},
                              {"mb_os_input_payload_31", 0, 6, 0x494},
                              {"mb_os_status", 0, 6, 0x498},
                              {"mb_os_spare_status", 0, 6, 0x49c},
                              {"mb_os_output_payload_0", 0, 6, 0x4a0},
                              {"mb_os_output_payload_1", 0, 6, 0x4a4},
                              {"mb_os_output_payload_2", 0, 6, 0x4a8},
                              {"mb_os_output_payload_3", 0, 6, 0x4ac},
                              {"mb_os_output_payload_4", 0, 6, 0x4b0},
                              {"mb_os_output_payload_5", 0, 6, 0x4b4},
                              {"mb_os_output_payload_6", 0, 6, 0x4b8},
                              {"mb_os_output_payload_7", 0, 6, 0x4bc},
                              {"mb_os_output_payload_8", 0, 6, 0x4c0},
                              {"mb_os_output_payload_9", 0, 6, 0x4c4},
                              {"mb_os_output_payload_10", 0, 6, 0x4c8},
                              {"mb_os_output_payload_11", 0, 6, 0x4cc},
                              {"mb_os_output_payload_12", 0, 6, 0x4d0},
                              {"mb_os_output_payload_13", 0, 6, 0x4d4},
                              {"mb_os_output_payload_14", 0, 6, 0x4d8},
                              {"mb_os_output_payload_15", 0, 6, 0x4dc},
                              {"mb_os_output_payload_16", 0, 6, 0x4e0},
                              {"mb_os_output_payload_17", 0, 6, 0x4e4},
                              {"mb_os_output_payload_18", 0, 6, 0x4e8},
                              {"mb_os_output_payload_19", 0, 6, 0x4ec},
                              {"mb_os_output_payload_20", 0, 6, 0x4f0},
                              {"mb_os_output_payload_21", 0, 6, 0x4f4},
                              {"mb_os_output_payload_22", 0, 6, 0x4f8},
                              {"mb_os_output_payload_23", 0, 6, 0x4fc},
                              {"mb_os_output_payload_24", 0, 6, 0x500},
                              {"mb_os_output_payload_25", 0, 6, 0x504},
                              {"mb_os_output_payload_26", 0, 6, 0x508},
                              {"mb_os_output_payload_27", 0, 6, 0x50c},
                              {"mb_os_output_payload_28", 0, 6, 0x510},
                              {"mb_os_output_payload_29", 0, 6, 0x514},
                              {"mb_os_output_payload_30", 0, 6, 0x518},
                              {"mb_os_output_payload_31", 0, 6, 0x51c},
                              {"mb_os_abort", 0, 6, 0x520},
                              {"mb_oob_cmd_spare", 0, 6, 0x600},
                              {"mb_oob_cmd", 0, 6, 0x604},
                              {"mb_oob_nonce_0", 0, 6, 0x608},
                              {"mb_oob_nonce_1", 0, 6, 0x60c},
                              {"mb_oob_nonce_2", 0, 6, 0x610},
                              {"mb_oob_nonce_3", 0, 6, 0x614},
                              {"mb_oob_input_payload_0", 0, 6, 0x618},
                              {"mb_oob_input_payload_1", 0, 6, 0x61c},
                              {"mb_oob_input_payload_2", 0, 6, 0x620},
                              {"mb_oob_input_payload_3", 0, 6, 0x624},
                              {"mb_oob_input_payload_4", 0, 6, 0x628},
                              {"mb_oob_input_payload_5", 0, 6, 0x62c},
                              {"mb_oob_input_payload_6", 0, 6, 0x630},
                              {"mb_oob_input_payload_7", 0, 6, 0x634},
                              {"mb_oob_input_payload_8", 0, 6, 0x638},
                              {"mb_oob_input_payload_9", 0, 6, 0x63c},
                              {"mb_oob_input_payload_10", 0, 6, 0x640},
                              {"mb_oob_input_payload_11", 0, 6, 0x644},
                              {"mb_oob_input_payload_12", 0, 6, 0x648},
                              {"mb_oob_input_payload_13", 0, 6, 0x64c},
                              {"mb_oob_input_payload_14", 0, 6, 0x650},
                              {"mb_oob_input_payload_15", 0, 6, 0x654},
                              {"mb_oob_input_payload_16", 0, 6, 0x658},
                              {"mb_oob_input_payload_17", 0, 6, 0x65c},
                              {"mb_oob_input_payload_18", 0, 6, 0x660},
                              {"mb_oob_input_payload_19", 0, 6, 0x664},
                              {"mb_oob_input_payload_20", 0, 6, 0x668},
                              {"mb_oob_input_payload_21", 0, 6, 0x66c},
                              {"mb_oob_input_payload_22", 0, 6, 0x670},
                              {"mb_oob_input_payload_23", 0, 6, 0x674},
                              {"mb_oob_input_payload_24", 0, 6, 0x678},
                              {"mb_oob_input_payload_25", 0, 6, 0x67c},
                              {"mb_oob_input_payload_26", 0, 6, 0x680},
                              {"mb_oob_input_payload_27", 0, 6, 0x684},
                              {"mb_oob_input_payload_28", 0, 6, 0x688},
                              {"mb_oob_input_payload_29", 0, 6, 0x68c},
                              {"mb_oob_input_payload_30", 0, 6, 0x690},
                              {"mb_oob_input_payload_31", 0, 6, 0x694},
                              {"mb_oob_status", 0, 6, 0x698},
                              {"mb_oob_spare_status", 0, 6, 0x69c},
                              {"mb_oob_output_payload_0", 0, 6, 0x6a0},
                              {"mb_oob_output_payload_1", 0, 6, 0x6a4},
                              {"mb_oob_output_payload_2", 0, 6, 0x6a8},
                              {"mb_oob_output_payload_3", 0, 6, 0x6ac},
                              {"mb_oob_output_payload_4", 0, 6, 0x6b0},
                              {"mb_oob_output_payload_5", 0, 6, 0x6b4},
                              {"mb_oob_output_payload_6", 0, 6, 0x6b8},
                              {"mb_oob_output_payload_7", 0, 6, 0x6bc},
                              {"mb_oob_output_payload_8", 0, 6, 0x6c0},
                              {"mb_oob_output_payload_9", 0, 6, 0x6c4},
                              {"mb_oob_output_payload_10", 0, 6, 0x6c8},
                              {"mb_oob_output_payload_11", 0, 6, 0x6cc},
                              {"mb_oob_output_payload_12", 0, 6, 0x6d0},
                              {"mb_oob_output_payload_13", 0, 6, 0x6d4},
                              {"mb_oob_output_payload_14", 0, 6, 0x6d8},
                              {"mb_oob_output_payload_15", 0, 6, 0x6dc},
                              {"mb_oob_output_payload_16", 0, 6, 0x6e0},
                              {"mb_oob_output_payload_17", 0, 6, 0x6e4},
                              {"mb_oob_output_payload_18", 0, 6, 0x6e8},
                              {"mb_oob_output_payload_19", 0, 6, 0x6ec},
                              {"mb_oob_output_payload_20", 0, 6, 0x6f0},
                              {"mb_oob_output_payload_21", 0, 6, 0x6f4},
                              {"mb_oob_output_payload_22", 0, 6, 0x6f8},
                              {"mb_oob_output_payload_23", 0, 6, 0x6fc},
                              {"mb_oob_output_payload_24", 0, 6, 0x700},
                              {"mb_oob_output_payload_25", 0, 6, 0x704},
                              {"mb_oob_output_payload_26", 0, 6, 0x708},
                              {"mb_oob_output_payload_27", 0, 6, 0x70c},
                              {"mb_oob_output_payload_28", 0, 6, 0x710},
                              {"mb_oob_output_payload_29", 0, 6, 0x714},
                              {"mb_oob_output_payload_30", 0, 6, 0x718},
                              {"mb_oob_output_payload_31", 0, 6, 0x71c},
                              {"mb_oob_abort", 0, 6, 0x720},
                              {"mb_smbus_cmd_spare", 5, 6, 0x000},
                              {"mb_smbus_cmd", 5, 6, 0x004},
                              {"mb_smbus_nonce_0", 5, 6, 0x008},
                              {"mb_smbus_nonce_1", 5, 6, 0x00c},
                              {"mb_smbus_nonce_2", 5, 6, 0x010},
                              {"mb_smbus_nonce_3", 5, 6, 0x014},
                              {"mb_smbus_input_payload_0", 5, 6, 0x018},
                              {"mb_smbus_input_payload_1", 5, 6, 0x01c},
                              {"mb_smbus_input_payload_2", 5, 6, 0x020},
                              {"mb_smbus_input_payload_3", 5, 6, 0x024},
                              {"mb_smbus_input_payload_4", 5, 6, 0x028},
                              {"mb_smbus_input_payload_5", 5, 6, 0x02c},
                              {"mb_smbus_input_payload_6", 5, 6, 0x030},
                              {"mb_smbus_input_payload_7", 5, 6, 0x034},
                              {"mb_smbus_input_payload_8", 5, 6, 0x038},
                              {"mb_smbus_input_payload_9", 5, 6, 0x03c},
                              {"mb_smbus_input_payload_10", 5, 6, 0x040},
                              {"mb_smbus_input_payload_11", 5, 6, 0x044},
                              {"mb_smbus_input_payload_12", 5, 6, 0x048},
                              {"mb_smbus_input_payload_13", 5, 6, 0x04c},
                              {"mb_smbus_input_payload_14", 5, 6, 0x050},
                              {"mb_smbus_input_payload_15", 5, 6, 0x054},
                              {"mb_smbus_input_payload_16", 5, 6, 0x058},
                              {"mb_smbus_input_payload_17", 5, 6, 0x05c},
                              {"mb_smbus_input_payload_18", 5, 6, 0x060},
                              {"mb_smbus_input_payload_19", 5, 6, 0x064},
                              {"mb_smbus_input_payload_20", 5, 6, 0x068},
                              {"mb_smbus_input_payload_21", 5, 6, 0x06c},
                              {"mb_smbus_input_payload_22", 5, 6, 0x070},
                              {"mb_smbus_input_payload_23", 5, 6, 0x074},
                              {"mb_smbus_input_payload_24", 5, 6, 0x078},
                              {"mb_smbus_input_payload_25", 5, 6, 0x07c},
                              {"mb_smbus_input_payload_26", 5, 6, 0x080},
                              {"mb_smbus_input_payload_27", 5, 6, 0x084},
                              {"mb_smbus_input_payload_28", 5, 6, 0x088},
                              {"mb_smbus_input_payload_29", 5, 6, 0x08c},
                              {"mb_smbus_input_payload_30", 5, 6, 0x090},
                              {"mb_smbus_input_payload_31", 5, 6, 0x094},
                              {"mb_smbus_status", 5, 6, 0x098},
                              {"mb_smbus_spare_status", 5, 6, 0x09c},
                              {"mb_smbus_output_payload_0", 5, 6, 0x0a0},
                              {"mb_smbus_output_payload_1", 5, 6, 0x0a4},
                              {"mb_smbus_output_payload_2", 5, 6, 0x0a8},
                              {"mb_smbus_output_payload_3", 5, 6, 0x0ac},
                              {"mb_smbus_output_payload_4", 5, 6, 0x0b0},
                              {"mb_smbus_output_payload_5", 5, 6, 0x0b4},
                              {"mb_smbus_output_payload_6", 5, 6, 0x0b8},
                              {"mb_smbus_output_payload_7", 5, 6, 0x0bc},
                              {"mb_smbus_output_payload_8", 5, 6, 0x0c0},
                              {"mb_smbus_output_payload_9", 5, 6, 0x0c4},
                              {"mb_smbus_output_payload_10", 5, 6, 0x0c8},
                              {"mb_smbus_output_payload_11", 5, 6, 0x0cc},
                              {"mb_smbus_output_payload_12", 5, 6, 0x0d0},
                              {"mb_smbus_output_payload_13", 5, 6, 0x0d4},
                              {"mb_smbus_output_payload_14", 5, 6, 0x0d8},
                              {"mb_smbus_output_payload_15", 5, 6, 0x0dc},
                              {"mb_smbus_output_payload_16", 5, 6, 0x0e0},
                              {"mb_smbus_output_payload_17", 5, 6, 0x0e4},
                              {"mb_smbus_output_payload_18", 5, 6, 0x0e8},
                              {"mb_smbus_output_payload_19", 5, 6, 0x0ec},
                              {"mb_smbus_output_payload_20", 5, 6, 0x0f0},
                              {"mb_smbus_output_payload_21", 5, 6, 0x0f4},
                              {"mb_smbus_output_payload_22", 5, 6, 0x0f8},
                              {"mb_smbus_output_payload_23", 5, 6, 0x0fc},
                              {"mb_smbus_output_payload_24", 5, 6, 0x100},
                              {"mb_smbus_output_payload_25", 5, 6, 0x104},
                              {"mb_smbus_output_payload_26", 5, 6, 0x108},
                              {"mb_smbus_output_payload_27", 5, 6, 0x10c},
                              {"mb_smbus_output_payload_28", 5, 6, 0x110},
                              {"mb_smbus_output_payload_29", 5, 6, 0x114},
                              {"mb_smbus_output_payload_30", 5, 6, 0x118},
                              {"mb_smbus_output_payload_31", 5, 6, 0x11c},
                              {"mb_smbus_abort", 5, 6, 0x120},
                              {"ds_perm_err_log", 5, 6, 0x1d4},
                              {"i3c_perm_err_log", 25, 6, 0x5f8},
                              {"temp_csr", 24, 6, 0x000},
                              {"sgx_a", 24, 6, 0x004},
                              {"vendor_device_id", 24, 6, 0x008},
                              {"revision_mfg_id", 24, 6, 0x00c},
                              {"fnv_rom_ctrl", 24, 6, 0x010},
                              {"fnv_perpartid_l", 24, 6, 0x014},
                              {"fnv_perpartid_h", 24, 6, 0x018},
                              {"fnv_sku_num", 24, 6, 0x01c},
                              {"fnv_fuse_ctrl", 24, 6, 0x020},
                              {"tsod_mirror_csr", 24, 6, 0x024},
                              {"fnv_aep_mfg", 24, 6, 0x028},
                              {"jtag_payload_0", 24, 6, 0x030},
                              {"jtag_payload_1", 24, 6, 0x034},
                              {"jtag_payload_2", 24, 6, 0x038},
                              {"jtag_payload_3", 24, 6, 0x03c},
                              {"jtag_tunnel_ctrl", 24, 6, 0x040},
                              {"fnv_debug_lock", 24, 6, 0x048},
                              {"msc_feature_stat_0", 24, 6, 0x050},
                              {"smbus_controls", 24, 6, 0x058},
                              {"smbus_xtrigger", 24, 6, 0x05c},
                              {"die_temp_csr", 24, 6, 0x060},
                              {"msc_temp_override_csr", 24, 6, 0x064},
                              {"msc_temp_cntrl_csr", 24, 6, 0x068},
                              {"msc_temp_low_csr", 24, 6, 0x06c},
                              {"msc_temp_high_csr", 24, 6, 0x070},
                              {"msc_temp_crit_csr", 24, 6, 0x074},
                              {"msc_temp_cal_csr", 24, 6, 0x078},
                              {"msc_temp_hipcntrl_csr", 24, 6, 0x07c},
                              {"msc_temp_status_csr", 24, 6, 0x080},
                              {"sgx_b", 24, 6, 0x190}};

PECIStatus waitForSMBusFree(const int addr, const RegEntry reg, uint8_t* cc)
{
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    int timeout_count = SMBUS_WAIT_LOOPS;
    while (timeout_count-- > 0)
    {
        peciStatus = regRd(addr, reg, &data, cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            status = PECI_SMB_FAIL;
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: SMB_BUSY check failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }

        // Check SMB_BUSY to see if the bus is free
        if (getFields(data, 0, 0) == 0)
        {
            goto Finish;
        }
    }

    // Timed out without SMB_BUSY getting cleared, so return an error
    status = PECI_SMB_STATUS_CHECK_FAIL;
    DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB_BUSY not cleared: val: (0x%x)\n", data);

Finish:
    return status;
}

PECIStatus waitForReadDataValid(const int addr, const RegEntry reg, uint8_t* cc)
{
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    int timeout_count = SMBUS_WAIT_LOOPS;
    while (timeout_count-- > 0)
    {
        peciStatus = regRd(addr, reg, &data, cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            status = PECI_SMB_FAIL;
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: SMB_RDO check failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }

        // Check SMB_SBE and abort if there are errors
        if (getFields(data, 1, 1) == 1)
        {
            status = PECI_SMB_STATUS_CHECK_FAIL;
            DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB_SBE set: val: (0x%x)\n", data);
            goto Finish;
        }

        // Check SMB_RDO to see if the read data is ready
        if (getFields(data, 2, 2) == 1)
        {
            goto Finish;
        }
    }

    // Timed out without SMB_RDO getting set, so return an error
    status = PECI_SMB_STATUS_CHECK_FAIL;
    DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB_RDO not set: val: (0x%x)\n", data);

Finish:
    return status;
}

PECIStatus waitForWriteOperationDone(const int addr, const RegEntry reg,
                                     uint8_t* cc)
{
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    int timeout_count = SMBUS_WAIT_LOOPS;
    while (timeout_count-- > 0)
    {
        peciStatus = regRd(addr, reg, &data, cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            status = PECI_SMB_FAIL;
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: SMB_WOD check failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }

        // Check SMB_SBE and abort if there are errors
        if (getFields(data, 1, 1) == 1)
        {
            status = PECI_SMB_STATUS_CHECK_FAIL;
            DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB_SBE set: val: (0x%x)\n", data);
            goto Finish;
        }

        // Check SMB_WOD to see if the read data is ready
        if (getFields(data, 3, 3) == 1)
        {
            goto Finish;
        }
    }

    // Timed out without SMB_WOD getting set, so return an error
    status = PECI_SMB_STATUS_CHECK_FAIL;
    DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB_WOD not set: val: (0x%x)\n", data);

Finish:
    return status;
}

PECIStatus checkStatus(const int addr, const RegEntry reg, uint8_t* cc)
{
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = SMBUS_TIMEOUT_INTERVAL_MS * 1000 * 1000;
    int timeout_count = 0;
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;
    do
    {
        nanosleep(&req, NULL);
        timeout_count += SMBUS_TIMEOUT_INTERVAL_MS;
        peciStatus = regRd(addr, reg, &data, cc);
        if (peciStatus != PECI_CC_SUCCESS)
        {
            status = PECI_SMB_FAIL;
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: Status check failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }

        data = getFields(data, 1, 1);
        if ((data == 1) || (timeout_count >= SMBUS_TIMEOUT_MS))
	{
		status = PECI_SMB_STATUS_CHECK_FAIL;
		DEBUG_PRINT(PMEM_ERROR,"ERROR: SMB WR failed status(%d): cc: (0x%x)\n",status, *cc);
		goto Finish;

	}
    } while ((0 == (data & 0x1)) && (timeout_count <= SMBUS_TIMEOUT_MS));

Finish:
    return status;
}

EPECIStatus regRd(int addr, RegEntry reg, uint32_t* val, uint8_t* cc)
{
    return peci_RdEndPointConfigPciLocal(addr, reg.seg, reg.bus, reg.dev,
                                         reg.func, reg.offset, reg.size,
                                         (uint8_t*)val, cc);
}

EPECIStatus regWr(int addr, RegEntry reg, uint32_t val, uint8_t* cc)
{
    return peci_WrEndPointPCIConfigLocal(addr, reg.seg, reg.bus, reg.dev,
                                         reg.func, reg.offset, reg.size, val,
                                         cc);
}

PECIStatus smbusRd(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                   uint8_t bytewidth, uint32_t* val, uint8_t* cc, bool check)
{
    uint8_t smb_cmd_cfg_spd = (bus == 0 ? smb_cmd_cfg_spd0 : smb_cmd_cfg_spd1);
    uint8_t smb_data_cfg_spd =
        (bus == 0 ? smb_data_cfg_spd0 : smb_data_cfg_spd1);
    uint8_t smb_status_cfg_spd =
        (bus == 0 ? smb_status_cfg_spd0 : smb_status_cfg_spd1);
    uint32_t cmd = 0;
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    // Set the command
    // reference: ICX EDS Vol2 17.1
    setFields(&cmd, 19, 19, 1);
    setFields(&cmd, 29, 29, 1);
    setFields(&cmd, 17, 17, (uint32_t)(bytewidth - 1));
    setFields(&cmd, 16, 15, 0);
    setFields(&cmd, 14, 11, (saddr >> 4) & 0xf);
    setFields(&cmd, 10, 8, (saddr >> 1) & 7);
    setFields(&cmd, 7, 0, command);

    status = waitForSMBusFree(addr, regs[smb_status_cfg_spd], cc);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB Busy (%d): cc: (0x %x)\n ", status,
                    *cc);
        goto Finish;
    }

    // Send the read command
    peciStatus = regWr(addr, regs[smb_cmd_cfg_spd], cmd, cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        status = PECI_SMB_FAIL;
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB RD failed status(%d): cc: (0x%x)\n",
                    status, *cc);
        goto Finish;
    }

    status = waitForSMBusFree(addr, regs[smb_status_cfg_spd], cc);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB Busy (%d): cc: (0x %x)\n ", status,
                    *cc);
        goto Finish;
    }

    status = waitForReadDataValid(addr, regs[smb_status_cfg_spd], cc);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: SMB Data not ready (%d): cc: (0x %x)\n ", status,
                    *cc);
        goto Finish;
    }

    // Read the data
    peciStatus = regRd(addr, regs[smb_data_cfg_spd], &data, cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        status = PECI_SMB_FAIL;
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB RD failed status(%d): cc: (0x%x)\n",
                    status, *cc);
        goto Finish;
    }

    // use only on pmem discovery
    if (check)
    {
        status = checkStatus(addr, regs[smb_status_cfg_spd], cc);
    }

    *val = getFields(data, 15, 0);

Finish:
    return status;
}

PECIStatus smbusWr(int addr, uint8_t bus, uint8_t saddr, uint8_t command,
                   uint8_t bytewidth, uint32_t val, uint8_t* cc, bool check)
{
    uint8_t smb_cmd_cfg_spd = (bus == 0 ? smb_cmd_cfg_spd0 : smb_cmd_cfg_spd1);
    uint8_t smb_data_cfg_spd =
        (bus == 0 ? smb_data_cfg_spd0 : smb_data_cfg_spd1);
    uint8_t smb_status_cfg_spd =
        (bus == 0 ? smb_status_cfg_spd0 : smb_status_cfg_spd1);
    uint32_t cmd = 0;
    uint32_t data = 0;
    EPECIStatus peciStatus = PECI_CC_SUCCESS;
    PECIStatus status = PECI_SUCCESS;

    // Set the write data
    setFields(&data, 31, 16, val);

    // Set the command
    // reference: ICX EDS Vol2 17.1
    setFields(&cmd, 19, 19, 1);
    setFields(&cmd, 29, 29, 1);
    setFields(&cmd, 17, 17, (uint32_t)bytewidth - 1);
    setFields(&cmd, 16, 15, 1);
    setFields(&cmd, 14, 11, (saddr >> 4) & 0xf);
    setFields(&cmd, 10, 8, (saddr >> 1) & 7);
    setFields(&cmd, 7, 0, command);

    status = waitForSMBusFree(addr, regs[smb_status_cfg_spd], cc);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB Busy (%d): cc: (0x %x)\n ", status,
                    *cc);
        goto Finish;
    }

    peciStatus = regWr(addr, regs[smb_data_cfg_spd], data, cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        status = PECI_SMB_FAIL;
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB WR failed status(%d): cc: (0x%x)\n",
                    status, *cc);
        goto Finish;
    }

    peciStatus = regWr(addr, regs[smb_cmd_cfg_spd], cmd, cc);
    if (peciStatus != PECI_CC_SUCCESS)
    {
        status = PECI_SMB_FAIL;
        DEBUG_PRINT(PMEM_ERROR, "ERROR: SMB WR failed status(%d): cc: (0x%x)\n",
                    status, *cc);
        goto Finish;
    }

    status = waitForWriteOperationDone(addr, regs[smb_status_cfg_spd], cc);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: SMB write not complete (%d): cc: (0x%x)\n", status,
                    *cc);
        goto Finish;
    }

    // use only on pmem discovery
    if (check)
    {
        status = checkStatus(addr, regs[smb_status_cfg_spd], cc);
    }

Finish:
    return status;
}

static void busAddrToLoc(BusAddrAndLoc* loc)
{
    uint8_t imcs_per_bus = IMCS_PER_SOCKET / BUSES_PER_SOCKET;

    loc->imc_index = loc->bus * imcs_per_bus +
                     (((loc->saddr & 0xf) / MAX_DEVICE_PER_BUS) % imcs_per_bus);

    loc->channel_index =
        ((loc->saddr & 0xf) / IMCS_PER_SOCKET) % CHANNELS_PER_IMC;

    loc->slot_index = (loc->saddr / CHANNELS_PER_IMC) % SLOTS_PER_CHANNEL;
}

PECIStatus csrRd(int addr, uint8_t device, SmBusCsrEntry csr, uint32_t* val,
                 uint8_t* cc, bool check)
{
    PECIStatus status;
    uint8_t bus = device < MAX_DEVICE_PER_BUS ? 0 : 1;
    uint8_t saddr = BASE_ADDR + (device % 8) * 2;
    uint8_t bus_channel;
    uint8_t opcode = 0;
    uint8_t numbytes = 4;
    BusAddrAndLoc loc = {.bus = bus, .saddr = saddr};
    cc = cc;
    check = check;

    busAddrToLoc(&loc);
    bus_channel = (loc.imc_index << 1) + loc.channel_index;
    status = i3cBlockRead(addr, saddr, bus_channel, csr.dev, csr.func,
                          csr.offset, opcode, numbytes, val);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: i3cBlockRead(%d)\n", status);
    }
    return status;
}

PECIStatus csrWr(int addr, uint8_t device, SmBusCsrEntry csr, uint32_t val,
                 uint8_t* cc, bool check)
{
    PECIStatus status;
    uint8_t bus = device < MAX_DEVICE_PER_BUS ? 0 : 1;
    uint8_t saddr = BASE_ADDR + (device % 8) * 2;
    uint8_t channel;
    uint8_t opcode = 0;
    uint8_t numbytes = 4;
    BusAddrAndLoc loc = {.bus = bus, .saddr = saddr};
    cc = cc;
    check = check;

    busAddrToLoc(&loc);
    channel = (loc.imc_index << 1) + loc.channel_index;

    status = i3cBlockWrite(addr, saddr, channel, csr.dev, csr.func, csr.offset,
                           opcode, numbytes, val);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: i3c_block_write(%d)\n", status);
    }
    return status;
}

PECIStatus api(const int addr, const uint8_t device, const ApiParams params,
               uint32_t* payload, const uint32_t payloadSize, uint8_t* cc)
{
    uint32_t val = 0, cmdOpcode = 0, cmdSubOpcode = 0, cmdDoorbell = 0,  cmd = 0, sequenceBit = 0;
    int startIdx = 0;
    int count = 0;
    struct timespec req = {0};
    PECIStatus status = PECI_SUCCESS;
    cc = cc;
    status =
        csrRd(addr, device, csrs[mb_smbus_cmd], &val, cc, false);
    
     if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI API failed status(%d): cc: (0x%x)\n", status,
                    *cc);
        goto Finish;
    }

    // Checking that doorbell bit was not set
    if (getFields(DOORBELL_BIT, DOORBELL_BIT, val) == 1)
    {
        if (status != PECI_SUCCESS)
        {
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: PECI API failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }
    }

    // Set all the input registers
    startIdx = mb_smbus_input_payload_0;
    for (int i = 0; i < params.indwSize; i++)
    {
        status =
            csrWr(addr, device, csrs[startIdx++], params.indw[i], cc, false);
        if (status != PECI_SUCCESS)
        {
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: PECI API failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }
    }
    cmdOpcode = bitField(OPCODE_BIT, 8, (uint32_t)params.opcode);
    cmdSubOpcode = bitField(SUBOPCODE_BIT, 8, (uint32_t)params.subopcode);
    cmdDoorbell = bitField(DOORBELL_BIT, 1, 1);
    cmd = cmdOpcode | cmdSubOpcode;
    if (getFields(SEQUENCE_BIT, SEQUENCE_BIT, val) == 0)
    {
        sequenceBit = bitField(SEQUENCE_BIT, 1, 1);
    }
    else
    {
        sequenceBit = bitField(SEQUENCE_BIT, 1, 0);
    }
    status = csrWr(addr, device, csrs[mb_smbus_cmd], cmd, cc, false);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI API failed status(%d): cc: (0x%x)\n", status,
                    *cc);
        goto Finish;
    }
    status =
        csrWr(addr, device, csrs[mb_smbus_cmd], (cmd | cmdDoorbell | sequenceBit), cc, false);
    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI API failed status(%d): cc: (0x%x)\n", status,
                    *cc);
        goto Finish;
    }
    req.tv_nsec = API_CHECK_INTERVAL_US * 10;

    while (count < API_MAX_RETRIES)
    {
        status = csrRd(addr, device, csrs[mb_smbus_status], &val, cc, false);

        // if completed bit set and ratelimiting bit not set
        if (CHECK_BIT(val, 0) && !CHECK_BIT(val, 3))
        {
            break;
        }

        count++;
        nanosleep(&req, NULL);
    }

    if (count == API_MAX_RETRIES)
    {
        status = PECI_SMB_FAIL;
        DEBUG_PRINT(PMEM_ERROR, "ERROR: PECI API Timeout status(%d):\n",
                    status);
        goto Finish;
    }

    // operation not completed after retries
    if (!CHECK_BIT(val, 0))
    {
        DEBUG_PRINT(PMEM_WARNING,
                    "WARNING: PECI API unexpected status register value"
                    "0x%x\n",
                    status);
    }

    startIdx = mb_smbus_output_payload_0;
    for (uint32_t i = 0; i < payloadSize; i++)
    {
        status = csrRd(addr, device, csrs[startIdx++], &payload[i], cc, false);
        if (status != PECI_SUCCESS)
        {
            DEBUG_PRINT(PMEM_ERROR,
                        "ERROR: PECI API failed status(%d): cc: (0x%x)\n",
                        status, *cc);
            goto Finish;
        }
    }

Finish:
    return status;
}

RETURN_STATUS peci_pass_thru(peci_device_addr_t device_addr, fw_cmd_t* fw_cmd)
{
    ApiParams params = {};
    uint8_t cc = 0;
    uint32_t payloadSize = 0;
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint32_t* payload = NULL;
    const int addr = device_addr.peci_addr;
    // Method to calculate dimm number exclusively. iMC designates two bits.
    // There are 4 iMCs per socket, 2 channels per iMC, and 2 slots per channel.
    const uint8_t device =
            device_addr.imc << 2 | device_addr.channel << 1 | device_addr.slot;
    
    if (fw_cmd == NULL)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: peci pass thru invalid input \n");
        return_code = PECI_FAIL;
        goto Finish;
    }

    params.opcode = fw_cmd->opcode;
    params.subopcode = fw_cmd->subOpcode;
    params.indwSize = fw_cmd->inputPayloadSize / sizeof(uint32_t);
    params.indw = (uint32_t*)fw_cmd->inputPayload;

    payload = (uint32_t*)fw_cmd->outPayload;
    payloadSize = fw_cmd->outputPayloadSize / sizeof(uint32_t);   

    if (addr == 0)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR: no valid PECI bus provided \n");
        return_code = PECI_FAIL;
        goto Finish;
    }

    return_code = api(addr, device, params, payload, payloadSize, &cc);

Finish:
    return return_code;
}

uint16_t discovery(const int addr, uint16_t* mask)
{
    uint8_t cc = 0;
    uint16_t failMask = 0;
    uint32_t devid = 0;
    static const uint8_t maxNumOfDevices = 16;
    int ret = 0;

    if (mask == NULL)
    {
        DEBUG_PRINT(PMEM_ERROR, "ERROR : invalid parameters!\n");
        ret = PECI_FAIL;
        return ret;
    }

    for (int i = 0; i < maxNumOfDevices; i++)
    {
        ret = csrRd(addr, i, csrs[vendor_device_id], &devid, &cc, true);
        if ((ret != PECI_SUCCESS) && PECI_CC_UA(cc))
        {
            SET_BIT(failMask, i);
        }
        else
        {
            DEBUG_PRINT(PMEM_SUCCESS, "SUCCESS: idx-%d devid-0x%x\n", i, devid);
        }

        switch (devid)
        {
            case ekv:
            case bwv:
            case cwv:
                SET_BIT(*mask, i);
		break;
	    default:
		break;
        }
    }

    return failMask;
}

RETURN_STATUS
peci_get_BSR(peci_device_addr_t* device_addr, device_bsr_t* bsr)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    PECIStatus status = PECI_SUCCESS;
    uint32_t BSR_L = 0;
    uint32_t BSR_H = 0;
    uint8_t cc = 0;

    if (device_addr == NULL || bsr == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    status = csrRd(device_addr->peci_addr, device_addr->slot,
                   csrs[d_fw_status_h], &BSR_H, &cc, false);

    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI read BSR high failed status(%d): cc: (0x%x)\n",
                    status, cc);
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

    status = csrRd(device_addr->peci_addr, device_addr->slot, csrs[d_fw_status],
                   &BSR_L, &cc, false);

    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI read BSR low failed status(%d): cc: (0x%x)\n",
                    status, cc);
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

    bsr->AsUint64 = ((uint64_t)BSR_H << 32) | (uint64_t)BSR_L;

Finish:
    return return_code;
}

RETURN_STATUS peci_get_Db_cmd_register(peci_device_addr_t* device_addr,
                                       uint64_t* pDb)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint32_t command_reg = 0;
    uint8_t cc = 0;
    PECIStatus status = PECI_SUCCESS;

    if (device_addr == NULL || pDb == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    status = csrRd(device_addr->peci_addr, device_addr->slot,
                              csrs[mb_smbus_cmd], &command_reg, &cc, false);

    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI read doorbell failed status(%d): cc: (0x%x)\n",
                    status, cc);
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

    *pDb = (uint64_t)(command_reg & (1 << (DB_SHIFT_32)));

Finish:
    return return_code;
}

RETURN_STATUS peci_get_Mb_status_register(peci_device_addr_t* device_addr,
                                          uint32_t* mbStatus)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    PECIStatus status = PECI_SUCCESS;
    uint8_t cc = 0;

    if (device_addr == NULL || mbStatus == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    status = csrRd(device_addr->peci_addr, device_addr->slot,
                   csrs[mb_smbus_status], mbStatus, &cc, false);

    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "ERROR: PECI read mailbox status failed status(%d): "
                    "cc: (0x%x)\n",
                    status, cc);
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

Finish:
    return return_code;
}

RETURN_STATUS peci_send_activation(peci_device_addr_t* device_addr)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint8_t cc = 0;
    uint32_t command = 0;
    PECIStatus status = PECI_SUCCESS;

    if (device_addr == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    // opcode = 0x09, subOpcode = 0x03
    command = ((uint32_t)0x09 << OP_SHIFT_32) |
                       ((uint32_t)0x03 << SUB_OP_SHIFT_32) |
                       ((uint32_t)1 << DB_SHIFT_32);

    status = csrWr(device_addr->peci_addr, device_addr->slot,
                              csrs[mb_smbus_cmd], command, &cc, false);

    if (status != PECI_SUCCESS)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "ERROR: PECI send activation failed status(%d): cc: (0x%x)\n",
            status, cc);
        return_code = RETURN_DEVICE_ERROR;
        goto Finish;
    }

Finish:
    return return_code;
}
