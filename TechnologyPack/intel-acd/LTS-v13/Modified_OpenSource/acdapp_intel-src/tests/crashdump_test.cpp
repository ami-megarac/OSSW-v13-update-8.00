/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2019 Intel Corporation.
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

#include "crashdump.hpp"
#include "utils.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// enable crashdump.cpp unit test requires the following:
// 1 - change #ifdef MOCK
// 2 - comment out crashdump.cpp main() function
// 3 - add crashdump.cpp in CMakeList.txt under unit test build
// 4 - remove static keyword from the functions to be unit tested

#ifndef MOCK
using namespace ::testing;
using ::testing::Return;
using namespace crashdump;

namespace crashdump
{
void loadInputFiles(std::vector<CPUInfo>& cpuInfo, InputFileInfo* inputFileInfo,
                    bool isTelemetry);
}

class CrashdumpTestFixture : public Test
{
  public:
    void SetUp() override
    {
        // Build a list of cpus
        info.model = cpu::icx;
        cpus.push_back(info);
        info.model = cpu::icx;
        cpus.push_back(info);
    }

    CPUInfo info = {};
    std::vector<CPUInfo> cpus;
    InputFileInfo inputFileInfo = {
        .unique = true, .filenames = {NULL}, .buffers = {NULL}};
    bool isTelemetry = false;
};

TEST_F(CrashdumpTestFixture, loadInputFiles)
{
    isTelemetry = false;
    loadInputFiles(cpus, &inputFileInfo, isTelemetry);
    EXPECT_EQ(cpus[0].sectionMask, 0xff);
}
TEST_F(CrashdumpTestFixture, loadInputFilesTelemetry)
{
    isTelemetry = true;
    loadInputFiles(cpus, &inputFileInfo, isTelemetry);
    EXPECT_EQ(cpus[0].sectionMask, 0xc4);
}
#endif