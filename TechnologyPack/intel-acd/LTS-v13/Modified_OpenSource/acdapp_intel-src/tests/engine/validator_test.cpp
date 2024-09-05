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

#include "../mock/test_crashdump.hpp"

extern "C" {
#include "engine/crashdump.h"
#include "engine/utils.h"
#include "engine/validator.h"
}

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace ::testing;
using ::testing::Return;

TEST(ValidatorTestFixture, IsValidHexString)
{
    EXPECT_EQ(IsValidHexString(""), false);
    EXPECT_EQ(IsValidHexString("0x"), false);
    EXPECT_EQ(IsValidHexString("0"), false);
    EXPECT_EQ(IsValidHexString("123"), false);
    EXPECT_EQ(IsValidHexString("0x1122K"), false);
    EXPECT_EQ(IsValidHexString("0x11AABBCC"), true);
    EXPECT_EQ(IsValidHexString("0x01DDEEFF"), true);
}

TEST(ValidatorTestFixture, IsRdIAMSRParamsValid)
{
    // TODO: placeholder test, update to a real test.
    CPUInfo cpuInfo = {.clientAddr = 48,
                       .model = cd_spr,
                       .coreMask = 0x0000db7e,
                       .crashedCoreMask = 0x0,
                       .chaCount = 0,
                       .initialPeciWake = ON,
                       .inputFile = {},
                       .cpuidRead = {},
                       .chaCountRead = {},
                       .coreMaskRead = {},
                       .dimmMask = 0};

    std::vector<CPUInfo> cpusInfo = {cpuInfo};
    TestCrashdump crashdump(cpusInfo);
    cJSON* root = cJSON_CreateObject();
    ValidatorParams ValidParams;
    ValidParams.validateInput = true;
    EXPECT_EQ(IsRdIAMSRParamsValid(root, &ValidParams), true);
    cJSON_Delete(root);
}
