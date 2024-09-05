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

union Retry_rd_err_log_prl
{
    struct
    {
        uint32_t valid : 1, uc : 1, over : 1, mode : 4, reserved : 25;
    };
    uint32_t retry_rd_err_log = 0;
};

union Retry_rd_err_log_address1_prl
{
    struct
    {
        uint32_t reserved1 : 6, chip_select : 3, cbit : 3, bank : 4, column : 12, reserved2 : 4;
    };
    uint32_t retry_rd_err_log_address1 = 0;
};

union Retry_rd_err_log_address2_prl
{
    struct
    {
        uint32_t row : 21, reserved : 11;
    };
    uint32_t retry_rd_err_log_address2 = 0;
};

const std::map<std::string, const char*> PrlImcChannel = {
    {"B02_D0a_F3_0x154", "IMC 0, Channel 0"}, {"B02_D0a_F7_0x154", "IMC 0, Channel 1"}, {"B02_D0b_F3_0x154", "IMC 0, Channel 2"},
    {"B02_D0c_F3_0x154", "IMC 1, Channel 0"}, {"B02_D0c_F7_0x154", "IMC 1, Channel 1"}, {"B02_D0d_F3_0x154", "IMC 1, Channel 2"},
};

const std::map<uint8_t, const char*> PrlMode = {{0x0, "sddc 2LM"},     {0x1, "sddc 1LM"},     {0x2, "sddc +1 2LM"},
                                                {0x3, "sddc +1 1LM"},  {0x4, "adddc 2LM"},    {0x5, "adddc 1LM"},
                                                {0x6, "adddc +1 2LM"}, {0x7, "adddc +1 1LM"}, {0x8, "read is from ddrt dimm"},
                                                {0x9, "x8 sddc"},      {0xA, "x8 sddc +1"},   {0xB, "not a valid ecc mode"}};

std::array<std::string, 6> prlImcChannels = {
    "B02_D0a_F3_", "B02_D0a_F7_", "B02_D0b_F3_", "B02_D0c_F3_", "B02_D0c_F7_", "B02_D0d_F3_",
};