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
#include "summary.hpp"

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class CpuGeneric
{
  public:
  CpuGeneric(std::string cpuNameIn, std::string cpuIdIn): cpuName(cpuNameIn), cpuId(cpuIdIn) {}
  virtual Summary prepareSummary(const json& input) = 0;
  std::string cpuName;
  std::string cpuId;
  virtual ~CpuGeneric() {}; //AMI change for coverity
};
