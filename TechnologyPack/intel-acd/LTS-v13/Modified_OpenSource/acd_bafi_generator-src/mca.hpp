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
#include "mca_cpx.hpp"
#include "mca_icx.hpp"
#include "mca_skx.hpp"
#include "mca_spr.hpp"

#include <memory>
#include <string>

std::shared_ptr<McaDecoder> mcaDecoderFactory(const MCAData& mca, const std::string& cpuType)
{
    // CLX has the same decoding rules as CPX
    if (cpuType == "CPX" || cpuType == "CLX")
    {
        if (mca.type == McaTypeFlag::CBO)
        {
            return std::make_shared<CpxMcaBankCha>(mca);
        }
        switch (mca.bank)
        {
            case 0:
                return std::make_shared<CpxMcaBankIfu>(mca);
            case 1:
                return std::make_shared<CpxMcaBankDcu>(mca);
            case 2:
                return std::make_shared<CpxMcaBankDtlb>(mca);
            case 3:
                return std::make_shared<CpxMcaBankMlc>(mca);
            case 4:
                return std::make_shared<CpxMcaBankPcu>(mca);
            case 5:
                return std::make_shared<CpxMcaBankUpi>(mca);
            case 6:
                return std::make_shared<CpxMcaBankIio>(mca);
            case 7:
                return std::make_shared<CpxMcaBankM2m>(mca);
            case 8:
                return std::make_shared<CpxMcaBankM2m>(mca);
            case 9:
                return std::make_shared<CpxMcaBankCha>(mca);
            case 10:
                return std::make_shared<CpxMcaBankCha>(mca);
            case 11:
                return std::make_shared<CpxMcaBankCha>(mca);
            case 12:
                return std::make_shared<CpxMcaBankUpi>(mca);
            case 13:
                return std::make_shared<CpxMcaBankImc>(mca);
            case 14:
                return std::make_shared<CpxMcaBankImc>(mca);
            case 15:
                return std::make_shared<CpxMcaBankImc>(mca);
            case 16:
                return std::make_shared<CpxMcaBankImc>(mca);
            case 17:
                return std::make_shared<CpxMcaBankImc>(mca);
            case 18:
                return std::make_shared<CpxMcaBankImc>(mca);
            case 19:
                return std::make_shared<CpxMcaBankUpi>(mca);
            case 20:
                return std::make_shared<CpxMcaBankUpi>(mca);
            case 21:
                return std::make_shared<CpxMcaBankUpi>(mca);
            case 22:
                return std::make_shared<CpxMcaBankUpi>(mca);
        }
    }
    else if (cpuType == "ICX")
    {
        if (mca.type == McaTypeFlag::CBO)
        {
            return std::make_shared<IcxMcaBankCbo>(mca);
        }
        switch (mca.bank)
        {
            case 0:
                return std::make_shared<IcxMcaBankIfu>(mca);
            case 1:
                return std::make_shared<IcxMcaBankDcu>(mca);
            case 2:
                return std::make_shared<IcxMcaBankDtlb>(mca);
            case 3:
                return std::make_shared<IcxMcaBankMlc>(mca);
            case 4:
                return std::make_shared<IcxMcaBankPcu>(mca);
            case 6:
                return std::make_shared<IcxMcaBankIio>(mca);
            case 5:
            case 7:
            case 8:
                return std::make_shared<IcxMcaBankUpi>(mca);
            case 9:
            case 10:
            case 11:
                return std::make_shared<IcxMcaBankCha>(mca);
            case 12:
            case 16:
            case 20:
            case 24:
                return std::make_shared<IcxMcaBankM2m>(mca);
            case 13:
            case 14:
            case 17:
            case 18:
            case 21:
            case 22:
            case 25:
            case 26:
                return std::make_shared<IcxMcaBankImc>(mca);
        }
    }
    else if (cpuType == "SKX")
    {
        if (mca.type == McaTypeFlag::CBO)
        {
            return std::make_shared<SkxMcaBankCha>(mca);
        }
        switch (mca.bank)
        {
            case 0:
                return std::make_shared<SkxMcaBankIfu>(mca);
            case 1:
                return std::make_shared<SkxMcaBankDcu>(mca);
            case 3:
                return std::make_shared<SkxMcaBankMlc>(mca);
            case 4:
                return std::make_shared<SkxMcaBankPcu>(mca);
            case 5:
                return std::make_shared<SkxMcaBankUpi>(mca);
            case 6:
                return std::make_shared<SkxMcaBankIio>(mca);
            case 7:
                return std::make_shared<SkxMcaBankM2m>(mca);
            case 8:
                return std::make_shared<SkxMcaBankM2m>(mca);
            case 9:
                return std::make_shared<SkxMcaBankCha>(mca);
            case 10:
                return std::make_shared<SkxMcaBankCha>(mca);
            case 11:
                return std::make_shared<SkxMcaBankCha>(mca);
            case 12:
                return std::make_shared<SkxMcaBankUpi>(mca);
            case 13:
                return std::make_shared<SkxMcaBankImc>(mca);
            case 14:
                return std::make_shared<SkxMcaBankImc>(mca);
            case 15:
                return std::make_shared<SkxMcaBankImc>(mca);
            case 16:
                return std::make_shared<SkxMcaBankImc>(mca);
            case 17:
                return std::make_shared<SkxMcaBankImc>(mca);
            case 18:
                return std::make_shared<SkxMcaBankImc>(mca);
            case 19:
                return std::make_shared<SkxMcaBankUpi>(mca);
        }
    }
    else if (cpuType == "SPR" || cpuType == "EMR")
    {
        if (mca.type == McaTypeFlag::CBO)
        {
            return std::make_shared<SprMcaBankCbo>(mca);
        }
        else if (mca.type == McaTypeFlag::MDF)
        {
            return std::make_shared<SprMcaBankMdf>(mca);
        }
        else if (mca.type == McaTypeFlag::M2M)
        {
            return std::make_shared<SprMcaBankM2m>(mca);
        }
        else if (mca.type == McaTypeFlag::UPI)
        {
            return std::make_shared<SprMcaBankUpi>(mca);
        }
        switch (mca.bank)
        {
            case 0:
                return std::make_shared<SprMcaBankIfu>(mca);
            case 1:
                return std::make_shared<SprMcaBankDcu>(mca);
            case 2:
                return std::make_shared<SprMcaBankDtlb>(mca);
            case 3:
                return std::make_shared<SprMcaBankMlc>(mca);
            case 4:
                return std::make_shared<SprMcaBankPcu>(mca);
            case 6:
                return std::make_shared<SprMcaBankIio>(mca);
            case 9:
                return std::make_shared<SprMcaBankCha>(mca);
            case 10:
                return std::make_shared<SprMcaBankCha>(mca);
            case 11:
                return std::make_shared<SprMcaBankCha>(mca);
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
                return std::make_shared<SprMcaBankImc>(mca);
        }
    }
    else if (cpuType == "SPR-HBM")
    {
        if (mca.type == McaTypeFlag::CBO)
        {
            return std::make_shared<SprMcaBankCbo>(mca);
        }
        else if (mca.type == McaTypeFlag::MDF)
        {
            return std::make_shared<SprMcaBankMdf>(mca);
        }
        else if (mca.type == McaTypeFlag::M2M)
        {
            return std::make_shared<SprHbmMcaBankM2m>(mca);
        }
        else if (mca.type == McaTypeFlag::UPI)
        {
            return std::make_shared<SprMcaBankUpi>(mca);
        }
        else if (mca.type == McaTypeFlag::HBM_M2M)
        {
            return std::make_shared<SprHbmMcaBankM2m>(mca);
        }
        else if (mca.type == McaTypeFlag::HBM_IMC)
        {
            return std::make_shared<SprHbmMcaBankImc>(mca);
        }
        switch (mca.bank)
        {
            case 0:
                return std::make_shared<SprMcaBankIfu>(mca);
            case 1:
                return std::make_shared<SprMcaBankDcu>(mca);
            case 2:
                return std::make_shared<SprMcaBankDtlb>(mca);
            case 3:
                return std::make_shared<SprMcaBankMlc>(mca);
            case 4:
                return std::make_shared<SprMcaBankPcu>(mca);
            case 6:
                return std::make_shared<SprMcaBankIio>(mca);
            case 9:
                return std::make_shared<SprMcaBankCha>(mca);
            case 10:
                return std::make_shared<SprMcaBankCha>(mca);
            case 11:
                return std::make_shared<SprMcaBankCha>(mca);
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
                return std::make_shared<SprMcaBankImc>(mca);
        }
    }
    return nullptr;
};
