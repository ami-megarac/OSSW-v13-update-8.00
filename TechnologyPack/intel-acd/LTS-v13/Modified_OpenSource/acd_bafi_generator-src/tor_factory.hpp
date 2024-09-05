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

#include <memory>
#include <string>

#include "tor_defs.hpp"

class TorFactory
{
  public:
    static TORDataGen *createInstance(std::string cpu_name)
    {
        TORDataGen *instance = nullptr;

        if (cpu_name == "ICX")
            instance = new TORDataIcx;
        // CLX has the same decoding rules as CPX
        else if (cpu_name == "CPX" || cpu_name == "CLX")
            instance = new TORDataCpxSkx;
        else if (cpu_name == "SKX")
            instance = new TORDataCpxSkx;
        else if (cpu_name == "SPR" || cpu_name == "SPR-HBM" || cpu_name == "EMR")
            instance = new TORDataSpr;

        if (instance != nullptr)
            return instance;
        else
            return nullptr;
    }
};
