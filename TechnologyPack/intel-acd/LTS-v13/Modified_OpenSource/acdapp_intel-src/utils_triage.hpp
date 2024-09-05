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

#pragma once

#include <stdio.h>

#include <string>

#define JSON_STR_LEN 32
#define BAFI_RC "RC:0x%x"
#define TRIAGE_STR_LEN 32
#define TRIAGE_RC "RC:0x%x"
#define TRIAGE_KEY "triage"
#define TRIAGE_ERR "Error appending triage section"
#define SUMMARY_KEY "summary"
#define SUMMARY_ERR "Error appending summary section"

void appendTriageSection(std::string& storedLogContents);
void appendSummarySection(std::string& storedLogContents);
