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

#include "fis.h"

#include "basetype.h"

#include <stdint.h>
#include <stdio.h>

#define firmwareUpdatePacketSize 64

RETURN_STATUS
fis_pass_thru(device_addr_t* device, fw_cmd_t* fw_cmd, uint64_t timeout)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;

    if (device == NULL || fw_cmd == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        DEBUG_PRINT(PMEM_ERROR, "Error: Invalid parameter.\n");
        return return_code;
    }
    switch (device->interface.iface)
    {
        case interface_i2c:
            return_code =
                smbus_pass_thru(device->smbus_address, fw_cmd, timeout);
            break;

        case interface_peci:
            return_code = peci_pass_thru(device->peci_address, fw_cmd);
            break;

        default:
            DEBUG_PRINT(PMEM_ERROR, "Error: Invalid interface selection. \n");
            return_code = RETURN_INVALID_PARAMETER;
            break;
    }
    return return_code;
}

/**
 * Firmware command long operation status.
 * Execute a FW command to get long op status.
 *
 * @param[in] pmem: The Intel pmem device to retrieve identify info on
 * @param[out] payload Area to place the identity info returned from FW
 * @retval RETURN_SUCCESS: Success
 */
RETURN_STATUS fw_cmd_long_op_status(device_addr_t* device,
                                    fis_long_op_status_payload_t* payload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint8_t* percent = NULL;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x08;
    fw_cmd.subOpcode = 0x4;
    fw_cmd.outputPayloadSize = sizeof(fis_long_op_status_payload_t);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);

    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected when getting long op status %lld \n",
                    return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

     percent = (uint8_t*)(&payload->percentComplete);

    payload->percentComplete =
        (BCD_TO_TWO_DEC(percent[1]) * 100) + BCD_TO_TWO_DEC(percent[0]);

Finish:
    return return_code;
}

/**
 * Firmware command firmware image info.
 * Execute a FW command to get information firmare image.
 *
 * @param[in] pmem: The Intel pmem device to retrieve identify info on
 * @param[out] payload Area to place the identity info returned from FW
 * @retval RETURN_SUCCESS: Success
 */

RETURN_STATUS fw_cmd_firmware_image_info(device_addr_t* device , void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint8_t* version = NULL;
    uint8_t* versionEnd = NULL;
    fis_firmware_info_payload_t *payload = (fis_firmware_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x08;
    fw_cmd.subOpcode = 0x1;
    fw_cmd.outputPayloadSize = sizeof(fis_firmware_info_payload_t);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);

    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected when sending firmware image info %lld \n",
                    return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

     version = &payload->fwSecurityVersion;
     versionEnd = &payload->fwProductVersion;

    for (; version <= versionEnd; version++)
    {
        *version = BCD_TO_TWO_DEC(*version);
    }

    version = (uint8_t*)&payload->fwBuildVersion;
    payload->fwBuildVersion =
        (BCD_TO_TWO_DEC(version[1]) * 100) + BCD_TO_TWO_DEC(version[0]);

    version = &payload->stagedSecurityVersion;
    versionEnd = &payload->stagedProductVersion;

    for (; version <= versionEnd; version++)
    {
        *version = BCD_TO_TWO_DEC(*version);
    }

    version = (uint8_t*)&payload->stagedBuildVersion;
    payload->stagedBuildVersion =
        (BCD_TO_TWO_DEC(version[1]) * 100) + BCD_TO_TWO_DEC(version[0]);

Finish:
    return return_code;
}

/**
 * Firmware command update firmware.
 *
 * @param[in] pmem: The Intel pmem device to retrieve identify info on
 * @param[in] payload Area to place image to upload
 * @param[in] size: size of image
 * @param[in] block: block of image to transfer
 * @retval RETURN_SUCCESS: Success
 */
RETURN_STATUS fw_cmd_update_firmware_block(device_addr_t* device,
                                           const uint8_t* payload, size_t size,
                                           size_t block)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_update_firmware_payload_t* command = NULL;
    if (device == NULL || payload == NULL || size % firmwareUpdatePacketSize ||
        !size)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x09;
    fw_cmd.subOpcode = 0x0;
    fw_cmd.inputPayloadSize = sizeof(fis_update_firmware_payload_t);

     command =
        (fis_update_firmware_payload_t*)fw_cmd.inputPayload;
    command->payloadSelector = 1; // small payload
    command->phase = start;

    command->packetNumber = block;
    memcpy(command->data, &(payload[block * firmwareUpdatePacketSize]),
           sizeof(command->data));

    if (block * firmwareUpdatePacketSize >= (size - firmwareUpdatePacketSize))
    {
        command->phase = finish;
        DEBUG_PRINT(PMEM_INFO, "Ending Transfer \n");
    }
    else if (block)
    {
        command->phase = running;
    }
    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);

    if (return_code != RETURN_SUCCESS)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected when sending Update Firmware command "
                    "%lld, packet %d\n",
                    return_code, command->packetNumber);
        goto Finish;
    }

    if (command->packetNumber % 100 == 0)
    {
        DEBUG_PRINT(PMEM_INFO, "Update %d percent done\n",
                    (firmwareUpdatePacketSize * block * 100) / size);
    }

Finish:
    return return_code;
}

/**
 * Firmware command Identify PMEM.
 * Execute a FW command to get information about PMEM device.
 *
 * @param[in] pmem: The Intel pmem device to retrieve identify info on
 * @param[out] payload Area to place the identity info returned from FW
 * @retval RETURN_SUCCESS: Success
 */
RETURN_STATUS
fw_cmd_device_id(device_addr_t* device, void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_id_device_payload_t *payload = (fis_id_device_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.deviceID = 0xDEADBEEF;
    fw_cmd.opcode = 0x01;
    fw_cmd.subOpcode = 0x0;
    fw_cmd.outputPayloadSize = 128;

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);

    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending Identify Device command %lld \n",
            return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Firmware command to get SMART and Health Info
 *
 * @param[in] pmem: The Intel pmem to retrieve SMART and Health Info
 * @param[out] payloadSmartAndHealth Area to place SMART and Health
 *  Info data
 *
 * @retval RETURN _SUCCESS or error code based on the type of error.
 */
RETURN_STATUS
fw_cmd_get_smart_and_health(device_addr_t* device , void* respayloadSmartAndHealth)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fw_cmd_t fw_cmd = {0};
    fis_smart_and_health_payload_t *payloadSmartAndHealth = (fis_smart_and_health_payload_t*) respayloadSmartAndHealth;
    if (device == NULL || payloadSmartAndHealth == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.deviceID = 0xDEADBEEF;
    fw_cmd.opcode = 0x08;
    fw_cmd.subOpcode = 0x00;
    fw_cmd.outputPayloadSize = sizeof(*payloadSmartAndHealth);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected sending Smart Health command %lld",
                    return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payloadSmartAndHealth) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payloadSmartAndHealth, fw_cmd.outPayload,
           sizeof(*payloadSmartAndHealth));

Finish:
    return return_code;
}

/**
 * Firmware command firmware activation.
 * Execute a FW command to activate a firmware update.
 *
 * @param[in] device_addr_t: Pmem device address.
 * @retval RETURN_SUCCESS: Success
 */
RETURN_STATUS
fw_cmd_fw_activation(device_addr_t* device)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    uint32_t command = 0;

    if (device == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    switch (device->interface.iface)
    {
        case interface_i2c:
            // opcode = 0x09, subOpcode = 0x03
            command = ((uint32_t)0x09 << OP_SHIFT_32) |
                      ((uint32_t)0x03 << SUB_OP_SHIFT_32) |
                      ((uint32_t)1 << DB_SHIFT_32);

            // SMB_CMD_REGISTER = C8000804
            return_code =
                smbus_write_direct(&device->smbus_address, 0xC8000804, command);
            break;

        case interface_peci:
            return_code = peci_send_activation(&device->peci_address);
            break;

        default:
            DEBUG_PRINT(PMEM_ERROR, "Error: Invalid interface selection.\n");
            return_code = RETURN_INVALID_PARAMETER;
            break;
    }

Finish:
    return return_code;
}

/**
 * Firmware command to get Power Management Policy Info (for FIS 2.3+)
 * Execute fw command to get power management policy for pmem device
 *
 * @param[in] device: The Intel PMem device address to retrieve Power Management
 * Policy Info
 * @param[out] payload: Area to place Power Management
 * Policy Info data The caller is responsible to free the allocated memory with
 * the FreePool function.
 *
 * @retval RETURN_SUCCESS Success
 * @retval RETURN_INVALID_PARAMETER pDimm or ppPayloadPowerManagementPolicy is
 * NULL
 */

RETURN_STATUS
fw_cmd_get_power_management_policy(device_addr_t* device, void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_power_management_policy_payload_t *payload = (fis_power_management_policy_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x04;
    fw_cmd.subOpcode = 0x02;
    fw_cmd.outputPayloadSize = sizeof(payload->payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending device characteristic command %lld \n",
            return_code);
        goto Finish;
    }
    _Static_assert(sizeof(payload->payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(payload->payload));

Finish:
    return return_code;
}

/**
 * Firmware command to get Alaram Thresholds
 *
 * @param[in] device The Intel PMEM to retrieve alaram thresholds info for
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS            Success
 * @retval RETURN_INVALID_PARAMETER  One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_alarmthresholds(device_addr_t* device , void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_get_alaram_thresholds_payload_t *payload = (fis_get_alaram_thresholds_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x04;
    fw_cmd.subOpcode = 0x01;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected when sending get device characteristic "
                    "command %lld \n",
                    return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));
Finish:
    return return_code;
}


/**
 * Firmware command to get Device Characteristics
 *
 * @param[in] device The Intel PMEM to retrieve device characteristics info for
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS            Success
 * @retval RETURN_INVALID_PARAMETER  One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_device_characteristics(device_addr_t* device, void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_device_characteristics_payload_t *payload = (fis_device_characteristics_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x01;
    fw_cmd.subOpcode = 0x01;
    fw_cmd.outputPayloadSize = sizeof(payload->payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected when sending get device characteristic "
                    "command %lld \n",
                    return_code);
        goto Finish;
    }
    _Static_assert(sizeof(payload->payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(payload->payload));
Finish:
    return return_code;
}

/**
 * Execute Firmware command to Get DIMM Partition Info
 *
 * @param[in] device The Intel PMEM to retrieve partition info from FW
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_pmem_partition_info(device_addr_t* device , void* respayload)

{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_pmem_partition_info_payload_t *payload = (fis_pmem_partition_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x06;
    fw_cmd.subOpcode = 0x02;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem partition info command %lld \n",
            return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Execute Firmware command to Get PMEM Config Lockdown
 *
 * @param[in] device The Intel PMEM to retrieve configuration lockdown status
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_pmem_config_lockdown(device_addr_t* device , void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_pmem_config_lockdown_info_payload_t *payload = (fis_pmem_config_lockdown_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x06;
    fw_cmd.subOpcode = 0x05;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem partition info command %lld \n",
            return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Execute Firmware command to Get Latch System Shutdown
 *
 * @param[in] device The Intel PMEM to Latch System Shutdown Status
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_pmem_latch_shutdown_state(device_addr_t* device , void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_pmem_latch_sys_shutdown_info_payload_t *payload = (fis_pmem_latch_sys_shutdown_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x06;
    fw_cmd.subOpcode = 0x09;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem partition info command %lld \n",
            return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Execute Firmware command to Get PMEM Viral Policy
 *
 * @param[in] device The Intel PMEM to retrieve viral policy status
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_pmem_viral_policy(device_addr_t* device , void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_pmem_viral_policy_info_payload_t *payload = (fis_pmem_viral_policy_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x06;
    fw_cmd.subOpcode = 0x0A;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem partition info command %lld \n",
            return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Execute Firmware command to Get PMEM Extend ADR
 *
 * @param[in] device The Intel PMEM to retrieve extend adr status
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_pmem_extend_adr(device_addr_t* device , void* respayload)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fis_pmem_extend_adr_info_payload_t *payload = (fis_pmem_extend_adr_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x06;
    fw_cmd.subOpcode = 0xEA;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem partition info command %lld \n",
            return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Execute Firmware command to get package sparing policy
 *
 * @param[in] device The Intel PMEM to retrieve Package Sparing policy
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_package_sparing_policy(
    device_addr_t* device, fis_get_package_sparing_policy_payload_t* payload)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fw_cmd_t fw_cmd = {0};

    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x04;
    fw_cmd.subOpcode = 0x03;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem get pkg sparing policy %lld \n",
            return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));
Finish:
    return return_code;
}

/**
 * Execute Firmware command to get address range scrub
 *
 * @param[in] device The Intel PMEM to retrieve address range scrub
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_address_range_scrub(device_addr_t* device, void* respayload)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fw_cmd_t fw_cmd = {0};
    fis_get_address_range_scrub_payload_t *payload = (fis_get_address_range_scrub_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x04;
    fw_cmd.subOpcode = 0x04;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem get pkg sparing policy %lld \n",
            return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));
Finish:
    return return_code;
}

/**
 * Execute Firmware command to get security state
 *
 * @param[in] device The Intel PMEM to retrieve Security Info
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_security_info(device_addr_t* device , void* respayload)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fw_cmd_t fw_cmd = {0};
    fis_get_security_info_payload_t *payload = (fis_get_security_info_payload_t*) respayload;
    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x02;
    fw_cmd.subOpcode = 0x00;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem get security info %lld \n",
            return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));
Finish:
    return return_code;
}

/**
 * Execute Firmware command to get security opt-in
 *
 * @param[in] device The Intel PMEM to retrieve Security Opt-in
 * @param[out] payload Area to place returned info from FW
 *
 * @retval RETURN_SUCCESS : Success
 * @retval RETURN_INVALID_PARAMETER : One or more input parameters are NULL
 */
RETURN_STATUS
fw_cmd_get_security_optin(device_addr_t* device,
                         fis_get_security_optin_payload_t* payload)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fw_cmd_t fw_cmd = {0};

    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x02;
    fw_cmd.subOpcode = 0x02;
    fw_cmd.outputPayloadSize = sizeof(*payload);

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(
            PMEM_ERROR,
            "Error detected when sending pmem get security info %lld \n",
            return_code);
        goto Finish;
    }

    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));
Finish:
    return return_code;
}


/**
 * Firmware command to get memory Health Info
 *
 * @param[in] pmem: The Intel pmem module to retrieve SMART and Health Info
 * @param[out] payload: Area to place memory Health Info data
 *
 * @retval RETURN_SUCCESS Success
 * @retval RETURN_INVALID_PARAMETER pmem or payload is NULL
 */
RETURN_STATUS
fw_cmd_get_memory_health(device_addr_t* device, const uint8_t page_num,
                         fis_memory_health_info_output_payload* payload)
{
    RETURN_STATUS return_code = RETURN_SUCCESS;
    fw_cmd_t fw_cmd = {0};
    fis_memory_health_info_input_payload input_payload = {0};

    if (device == NULL || payload == NULL)
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.opcode = 0x08;
    fw_cmd.subOpcode = 0x03;
    fw_cmd.outputPayloadSize = sizeof(*payload);
    input_payload.memory_page = page_num;
    _Static_assert(sizeof(input_payload) <= sizeof(fw_cmd.inputPayload),
                   "Error: payload size mismatch");
    memcpy(fw_cmd.inputPayload, &input_payload, sizeof(input_payload));

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected sending Smart Health command %lld",
                    return_code);
        goto Finish;
    }
    _Static_assert(sizeof(*payload) <= sizeof(fw_cmd.outPayload),
                   "Error: payload size mismatch");
    memcpy(payload, fw_cmd.outPayload, sizeof(*payload));

Finish:
    return return_code;
}

/**
 * Firmware command to get Error logs
 *
 * @param[in] pmem: The Intel pmem module to retrieve
 * @param[in] inputPayload - filled input payload
 * @param[out] output_payload - small payload result data of get error log
 * operation
 * @param[in] output_payload_size - size of small payload
 * @param[in] large_output_payload - large ouput payload.
 * @param[in] larger_ouput_payload_size - size of large payload.
 *
 * @retval RETURN_STATUS.
 */
RETURN_STATUS
fw_cmd_get_error_log(device_addr_t* device,
                     fis_error_logs_input_payload_t* input_payload,
                     void* output_payload, uint32_t output_payload_size,
                     void* large_output_payload,
                     uint32_t large_output_payload_size)
{
    fw_cmd_t fw_cmd = {0};
    RETURN_STATUS return_code = RETURN_SUCCESS;

    if (device == NULL || input_payload == NULL ||
        (output_payload == NULL && large_output_payload == NULL))
    {
        return_code = RETURN_INVALID_PARAMETER;
        goto Finish;
    }

    fw_cmd.deviceID = device->deviceID;
    fw_cmd.opcode = 0x08;
    fw_cmd.subOpcode = 0x05;
    fw_cmd.inputPayloadSize = sizeof(*input_payload);
    fw_cmd.outputPayloadSize = output_payload_size;
    fw_cmd.largeOutputPayloadSize = large_output_payload_size;
    _Static_assert(sizeof(*input_payload) <= sizeof(fw_cmd.inputPayload),
                   "Error: payload size mismatch");
    memcpy(fw_cmd.inputPayload, input_payload, sizeof(*input_payload));

    return_code = fis_pass_thru(device, &fw_cmd, PT_LONG_TIMEOUT_INTERVAL);
    if (return_code)
    {
        DEBUG_PRINT(PMEM_ERROR,
                    "Error detected when sending pmem get error log %lld \n",
                    return_code);
        goto Finish;
    }

    if (output_payload != NULL && output_payload_size > 0)
    {
        _Static_assert(sizeof(*output_payload) <= sizeof(fw_cmd.outPayload),
                       "Error: payload size mismatch");
        memcpy(output_payload, fw_cmd.outPayload, output_payload_size);
    }
    if (large_output_payload != NULL && large_output_payload_size > 0)
    {
        _Static_assert(sizeof(*large_output_payload) <=
                           sizeof(fw_cmd.largeOutputPayload),
                       "Error: payload size mismatch");
        memcpy(large_output_payload, fw_cmd.largeOutputPayload,
               large_output_payload_size);
    }

Finish:
    return return_code;
}
