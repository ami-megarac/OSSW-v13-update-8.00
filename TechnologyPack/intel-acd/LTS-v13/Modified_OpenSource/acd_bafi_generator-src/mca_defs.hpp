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
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
enum class McaTypeFlag
{
    NO_TYPE,
    CBO,
    MDF,
    UPI,
    IMC,
    M2M,
    HBM_M2M,
    HBM_IMC,
    MSE,
    MCHN,
    B2CMI,
    LLC,
    PUNIT,
};
struct MCAData
{
    union
    {
        struct
        {
            uint64_t reserved0 : 63, valid : 1;
        };
        uint64_t mc_status;
    };

    uint64_t address = 0;
    uint64_t misc = 0;
    uint64_t ctl = 0;
    uint32_t core = 0;
    uint32_t thread = 0;
    uint32_t bank = 0;
    int32_t hbm_bank = 0;
    uint32_t imc_num = 0;
    uint32_t channel = 0;
    uint32_t module = 0;
    McaTypeFlag type = McaTypeFlag::NO_TYPE;
};

class McaDecoder
{
  public:
    McaDecoder(const MCAData& mca) : mca(mca){};

    virtual json decode() = 0;

    virtual json decode_status() = 0;

    const MCAData& mca;
};
