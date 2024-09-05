/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2022 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials, and
 * your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise, you may
 * not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in
 * the License.
 *
 ******************************************************************************/

#pragma once
#include <map>
#include <string>

union Retry_rd_err_log_egs
{
    struct
    {
        uint32_t valid : 1, uc : 1, over : 1, mode : 4, reserved1 : 16, system_addr_valid : 1, reserved2 : 8;
    };
    uint32_t retry_rd_err_log = 0;
};

union Retry_rd_err_log_address1_egs
{
    struct
    {
        uint32_t reserved : 6, chip_select : 3, cbit : 5, bank : 6, column : 10, reserved1 : 2;
    };
    uint32_t retry_rd_err_log_address1 = 0;
};

union Retry_rd_err_log_address3_egs
{
    struct
    {
        uint64_t address : 52, reserved : 12;
    };
    uint64_t retry_rd_err_log_address3 = 0;
};

union Retry_rd_err_set2_log_address2_egs
{
    struct
    {
        uint32_t row : 18, reserved5 : 14;
    };
    uint32_t retry_rd_err_set2_log_address2 = 0;
};

union CounterRegister
{
    struct
    {
        uint32_t count : 15, reserved : 17;
    };
    uint32_t counterRegister = 0;
};

std::array<std::string, 4> egsImcChannels = {"B30_D26_F0_", "B30_D27_F0_", "B30_D28_F0_", "B30_D29_F0_"};

std::array<std::string, 4> egsOffsets = {"0x22c60", "0x22e54", "0x2ac60", "0x2ae54"};

const std::map<std::string, const char*> EgsImcChannel = {
    {"B30_D26_F0_0x22c60", "IMC 0, Channel 0"}, {"B30_D26_F0_0x22e54", "IMC 0, Channel 0"},
    {"B30_D26_F0_0x22c58", "IMC 0, Channel 0"}, {"B30_D26_F0_0x22e58", "IMC 0, Channel 0"},
    {"B30_D26_F0_0x2ac60", "IMC 0, Channel 1"}, {"B30_D26_F0_0x2ae54", "IMC 0, Channel 1"},
    {"B30_D26_F0_0x2ac58", "IMC 0, Channel 1"}, {"B30_D26_F0_0x2ae58", "IMC 0, Channel 1"},
    {"B30_D27_F0_0x22c60", "IMC 1, Channel 0"}, {"B30_D27_F0_0x22e54", "IMC 1, Channel 0"},
    {"B30_D27_F0_0x22c58", "IMC 1, Channel 0"}, {"B30_D27_F0_0x22e58", "IMC 1, Channel 0"},
    {"B30_D27_F0_0x2ac60", "IMC 1, Channel 1"}, {"B30_D27_F0_0x2ae54", "IMC 1, Channel 1"},
    {"B30_D27_F0_0x2ac58", "IMC 1, Channel 1"}, {"B30_D27_F0_0x2ae58", "IMC 1, Channel 1"},
    {"B30_D28_F0_0x22c60", "IMC 2, Channel 0"}, {"B30_D28_F0_0x22e54", "IMC 2, Channel 0"},
    {"B30_D28_F0_0x22c58", "IMC 2, Channel 0"}, {"B30_D28_F0_0x22e58", "IMC 2, Channel 0"},
    {"B30_D28_F0_0x2ac60", "IMC 2, Channel 1"}, {"B30_D28_F0_0x2ae54", "IMC 2, Channel 1"},
    {"B30_D28_F0_0x2ac58", "IMC 2, Channel 1"}, {"B30_D28_F0_0x2ae58", "IMC 2, Channel 1"},
    {"B30_D29_F0_0x22c60", "IMC 3, Channel 0"}, {"B30_D29_F0_0x22e54", "IMC 3, Channel 0"},
    {"B30_D29_F0_0x22c58", "IMC 3, Channel 0"}, {"B30_D29_F0_0x22e58", "IMC 3, Channel 0"},
    {"B30_D29_F0_0x2ac60", "IMC 3, Channel 1"}, {"B30_D29_F0_0x2ae54", "IMC 3, Channel 1"},
    {"B30_D29_F0_0x2ac58", "IMC 3, Channel 1"}, {"B30_D29_F0_0x2ae58", "IMC 3, Channel 1"}};

const std::map<uint8_t, const char*> EgsMode = {{0x0, "sddc 2LM"},     {0x1, "sddc 1LM"},     {0x2, "sddc +1 2LM"},
                                                {0x3, "sddc +1 1LM"},  {0x4, "adddc 2LM"},    {0x5, "adddc 1LM"},
                                                {0x6, "adddc +1 2LM"}, {0x7, "adddc +1 1LM"}, {0x8, "read is from ddrt dimm"},
                                                {0x9, "x8 sddc"},      {0xA, "x8 sddc +1"},   {0xB, "not a valid ecc mode"}};