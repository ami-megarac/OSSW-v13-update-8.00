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

union Version
{
    struct
    {
        uint32_t revision : 8, reserved0 : 4, cpu_id : 12, crash_record_type : 5, reserved1 : 3;
    };
    uint32_t version;
};

const std::map<uint16_t, const char*> decodedCpuId = {
    {0x2A, "SKX"}, {0x1A, "ICX"}, {0x1B, "ICX"},     {0x34, "CPX"}, {0x2C, "CLX"},
    {0x1C, "SPR"}, {0x33, "SPR"}, {0x6f, "SPR-HBM"}, {0x79, "EMR"}, {0x2F, "GNR"},
    {0x80, "GNR"}, {0x84, "GNR"}, {0x82, "SRF"}, {0x83, "SRF"},
};

const std::map<uint16_t, const char*> decodedCpuFullName = {
    {0x2A, "Sky Lake SP"},     {0x1A, "Icelake SP"},         {0x1B, "Icelake D"},         {0x34, "Cooper Lake"},
    {0x2C, "Cascade Lake SP"}, {0x1C, "Sapphire Rapids SP"}, {0x33, "Sapphire Rapids D"}, {0x6F, "Sapphire Rapids HBM"},
    {0x79, "Emerald Rapids"},  {0x2F, "Granite Rapids"},     {0x80, "Granite Rapids"},    {0x84, "Granite Rapids D"},
    {0x82, "Sierra Forest"},   {0x83, "Sierra Forest"},
};
