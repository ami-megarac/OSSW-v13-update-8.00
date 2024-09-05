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

#ifndef __FWUTILITY_H__
#define __FWUTILITY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define DB_SHIFT 48
#define DB_SHIFT_32                                                            \
    (DB_SHIFT - 32) //!< DB_SHIFT in UINT32 half of command register
#define SUB_OP_SHIFT 40
#define SUB_OP_SHIFT_32                                                        \
    (SUB_OP_SHIFT - 32) //!< SUB_OP_SHIFT in UINT32 half of command register
#define OP_SHIFT 32
#define OP_SHIFT_32                                                            \
    (OP_SHIFT - 32) //!< OP_SHIFT in UINT32 half of command register

#define IN_MB_SIZE (1 << 20)  //!< Size of the OS mailbox large input payload
#define OUT_MB_SIZE (1 << 20) //!< Size of the OS mailbox large output payload
#define IN_PAYLOAD_SIZE (128)
#define OUT_PAYLOAD_SIZE (128)

    typedef union
    {
        struct
        {
            uint8_t digit2;
            uint8_t digit1;
        } byte;
        uint16_t version;
    } fw_api_version_t;

    /**
     * Firmware command
     */
    typedef struct
    {
        uint32_t inputPayloadSize;
        uint32_t largeInputPayloadSize;
        uint32_t outputPayloadSize;
        uint32_t largeOutputPayloadSize;
        uint8_t inputPayload[IN_PAYLOAD_SIZE];
        uint8_t largeInputPayload[IN_MB_SIZE];
        uint8_t outPayload[OUT_PAYLOAD_SIZE];
        uint8_t largeOutputPayload[OUT_MB_SIZE];
        uint32_t deviceID;
        uint8_t opcode;
        uint8_t subOpcode;
        uint8_t status;
    } fw_cmd_t;

#ifdef __cplusplus
}
#endif

#endif
