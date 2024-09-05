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
#include "cpu.hpp"
#include "tor_cpx.hpp"
#include "tor_icx.hpp"
#include "tor_skx.hpp"
#include "tor_spr.hpp"

#include <memory>
#include <string>
#include <utility>

class CpuFactory
{
  public:
    static std::shared_ptr<CpuGeneric> createInstance(std::string cpu_name, std::string cpuId)
    {
        if (cpu_name == "ICX")
            return std::make_shared<IcxCpu>(cpu_name, cpuId);
        else if (cpu_name == "CPX" || cpu_name == "CLX")
            return std::make_shared<CpxCpu>(cpu_name, cpuId);
        else if (cpu_name == "SKX")
            return std::make_shared<SkxCpu>(cpu_name, cpuId);
        else if (cpu_name == "SPR" || cpu_name == "SPR-HBM" || cpu_name == "EMR")
            return std::make_shared<SprCpu>(cpu_name, cpuId);

        return nullptr;
    }
};
