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
#include "summary_field.hpp"

class Summary
{
  public:
    std::string cpuType;
    std::string cpuId;

    std::array<const char*, 68> PORT_ID;
    std::array<const char*, 16> LLCS;
    std::map<uint8_t, uint8_t> FirstErrorCha;
    std::map<uint16_t, const char*> FirstError;
    std::map<uint32_t, const char*> OpCodeDecode;

    MCAPointer mca;
    AerPointer uncAer;
    AerPointer corAer;
    TSCPointer tsc;
    ThermPointer thermStatus;
    TorPointer tor;
    std::map<std::string, std::array<uint64_t, 2>> memoryMap;

    
    Summary(std::string cpuTypeIn, std::string cpuIdIn) : cpuType(cpuTypeIn), cpuId(cpuIdIn)
    {
    /*Reason for False Positive - There are no uninitialized members*/
    /* coverity[uninit_member : FALSE] */
    }
};
