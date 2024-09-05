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

#include "utils.hpp"
#include <cstdint>
#include <map>
#include <vector>

struct AerData
{
    uint32_t error_status;
    std::string address;
};

std::map<std::vector<uint8_t>, const char*> decodeCorAer{
    {{15}, "header_log_overflow_error"},
    {{14}, "correctable_internal_error"},
    {{13}, "advisory_non_fatal_error"},
    {{12}, "replay_timer_timeout"},
    {{8}, "replay_num_rollover"},
    {{7}, "bad_dllp"},
    {{6}, "bad_tlp"},
    {{0}, "receiver_error"},
};

std::map<std::vector<uint8_t>, const char*> decodeUncAer{
    {{30}, "pcrc_check_failed"},
    {{29}, "misrouted_ide"},
    {{28}, "ide_check_failed"},
    {{27}, "dmwr_blocked"},
    {{26}, "poisoned_tlp_egress_blocked"},
    {{25}, "tlp_prefix_blocked"},
    {{24}, "atomic_egress_blocked"},
    {{23}, "mc_blocked_tlp"},
    {{22}, "uncorrectable_internal_error"},
    {{21}, "acs_violation"},
    {{20}, "received_an_unsupported_request"},
    {{19}, "ecrc_error"},
    {{18}, "malformed_tlp"},
    {{17}, "receiver_buffer_overflow"},
    {{16}, "unexpected_completion"},
    {{15}, "completer_abort"},
    {{14}, "completion_timeout"},
    {{13}, "flow_control_protocol_error"},
    {{12}, "poisoned_tlp"},
    {{5}, "surprise_down_error"},
    {{4}, "data_link_protocol_error"},
};

std::map<std::string, uint32_t> parseAer(const json& dieSection, BdfRegisterSets registers)
{

    std::map<std::string, uint32_t> ret;
    uint32_t temp;

    if (!dieSection.contains("uncore"))
    {
        return ret;
    }

    for (const auto& [pciKey, pciVal] : dieSection.at("uncore").items())
    {
        if (pciVal == false || checkInproperValue(pciVal) || pciVal == "0x0")
        {
            continue;
        }

        if (!registers.contains(pciKey))
        {
            continue;
        }

        if (!str2uint(pciVal, temp))
        {
            continue;
        }

        ret[pciKey] = temp;
    }

    return ret;
}