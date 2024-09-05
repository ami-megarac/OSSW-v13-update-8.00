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

#ifndef __FIS_H__
#define __FIS_H__

#include "basetype.h"
#include "fwutility.h"
#include "smbus_direct.h"
#include "smbus_peci.h"

#include <assert.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#pragma pack(push)
#pragma pack(1)

#define PT_LONG_TIMEOUT_INTERVAL 0xF4240

#define IN_MB_SIZE (1 << 20)  //!< Size of the OS mailbox large input payload
#define OUT_MB_SIZE (1 << 20) //!< Size of the OS mailbox large output payload
#define IN_PAYLOAD_SIZE (128)
#define OUT_PAYLOAD_SIZE (128)


    typedef enum
    {
        interface_i2c,  //!< i2c=0
        interface_peci, //!< peci=1
    } interface;

    /**
     * Interface to use for pmem communication
     */
    typedef struct _interface_t
    {
        interface iface;  //!< 0=i2c, 1=peci
        uint32_t timeout; //!< timeout for given interface
    } interface_t;

    /**
     * PMEM device address information
     */
    typedef struct _device_addr_t
    {
        bool present;
        uint8_t cpu;
        uint8_t imc;
        uint8_t channel;
        uint8_t slot;
        uint8_t deviceID;
        interface_t interface;
        smbus_device_addr_t smbus_address; //!< SMBUS address
        peci_device_addr_t peci_address;   //!< PECI address
    } device_addr_t;

    /**
     * Temperature structure
     */
    typedef union
    {
        uint16_t as_uint16;
        struct
        {
            uint16_t temperature_value : 15; //< Unsigned, integer temperature
                                             // in Celsius
            uint16_t sign : 1; //< Sign Bit (1 - negative, 0 - positive)
        } separated;
    } temperature_t;

    /**
      This struct holds information about DIMM capabilities and features
    **/
    typedef struct
    {
        uint32_t memory_mode_enabled : 1;
        uint32_t storage_mode_enabled : 1;
        uint32_t app_direct_mode_enabled : 1;
        uint32_t package_sparing_capable : 1;
        uint32_t : 12; //!< Reserved
        uint32_t soft_programable_sku : 1;
        uint32_t encryption_enabled : 1;
        uint32_t : 14; //!< Reserved
    } sku_information;
    /**
     * FIS Passthrough Payload for Indetify PMEM
     * Opcode: 0x01h (Identify PMEM)
     * Sub-opcode: 0x00h (Identify)
     */
    typedef struct
    {
        uint16_t vid;          //!< 1-0   : PMEM vendor id
        uint16_t did;          //!< 3-2   : Device ID
        uint16_t rid;          //!< 5-4   : Revision ID
        uint16_t ifc;          //!< 7-6   : Interface format code (0x301)
        uint8_t fwr[5];        //!< 12-8  : BCD formated firmware revision
        uint8_t reserved0;     //!< 13    : Reserved
        uint8_t fswr;          //!< 14    : Feature SW Required Mask
        uint8_t reserved1;     //!< 15    : Reserved
        uint8_t reserved2[16]; //!< 31-16 : Reserved
        uint32_t rc;           //!< 35-32 : Raw capacity
        uint16_t mf;           //!< 37-36 : Manufacturer ID (Deprecated)
        uint32_t sn;           //!< 41-38 : Serial Number ID (Deprecated)
        char pn[20];           //!< 61-42 : ASCII Part Number
        uint32_t sku;          //!< 65-62 : Device SKU
        uint8_t reserved3[2];  //!< 66-67 : Reserved
        uint16_t apiVer;       //!< 69-68 : API Version
        uint8_t uid[9];        //!< 78-70 : Unique ID (UID)
        uint8_t reserved4[49]; //!< 127-70: Reserved
    } fis_id_device_payload_t;

    /**
     * FIS Passthrough Payload for Indetify PMEM
     * Opcode: 0x01h (Identify PMEM)
     * Sub-opcode: 0x01h (Device Characteristics)
     */
    typedef struct
    {
        temperature_t controller_shutdown_threshold;
        temperature_t media_shutdown_threshold;
        temperature_t media_throttling_start_threshold;
        temperature_t media_throttling_stop_threshold;
        temperature_t controller_throttling_start_threshold;
        temperature_t controller_throttling_stop_threshold;
        uint16_t max_average_power_limit;
        uint16_t max_memory_bandwidth_boost_max_power_limit;
        uint32_t max_memory_bandwidth_boost_average_power_time_constant;
        uint32_t memory_bandwidth_boost_average_power_time_constant_step;
        uint32_t max_average_power_reporting_time_constant;
        uint32_t average_reporting_time_constant_step;
        uint8_t reserved[96];
    } pt_device_characteristics_payload_2_1_t;
    /**
     * Fis Device characterstics payload
     */
    typedef struct
    {
        uint8_t fis_major;
        uint8_t fis_minor;
        union
        {
            pt_device_characteristics_payload_2_1_t fis_2_01;
            uint8_t data[0];
        } payload;
    } fis_device_characteristics_payload_t;

    /**
     * Fis Alaram Thresholds payload
     */
    typedef struct
    {
	uint8_t enable;
	uint8_t percentage_remaining;
	uint16_t media_temparature;
	uint16_t core_temparature;
	uint8_t reserved[122];
    } fis_get_alaram_thresholds_payload_t;

    /**
     * FIS Passthrough Payload for partition info
     * Opcode:     0x06h (Get Admin Features)
     * Sub-Opcode: 0x02h (DIMM Partition Info)
     */
    typedef struct
    {
        uint32_t volatile_capacity;
        uint32_t reserved;
        uint64_t volatile_start;
        uint32_t persistent_capacity;
        uint32_t reserved2;
        uint64_t persistent_start;
        uint32_t raw_capacity;
        uint8_t reserved3[92];
    } fis_pmem_partition_info_payload_t;

    /**
     * FIS Passthrough Payload for partition info
     * Opcode:     0x06h (Get Admin Features)
     * Sub-Opcode: 0x05h (Get Config Lockdown)
     */
    typedef struct
    {
	uint8_t locked;
        uint8_t reserved[127];
    } fis_pmem_config_lockdown_info_payload_t;

    /**
     * FIS Passthrough Payload for partition info
     * Opcode:     0x06h (Get Admin Features)
     * Sub-Opcode: 0x09h (Get Latch System Shutdown)
     */
    typedef struct
    {
	uint8_t latch_sys_shutdown_state;
	uint8_t prv_pwr_cyc_latch_shutdown_state;
        uint8_t reserved[126];
    } fis_pmem_latch_sys_shutdown_info_payload_t;

    /**
     * FIS Passthrough Payload for partition info
     * Opcode:     0x06h (Get Admin Features)
     * Sub-Opcode: 0x0Ah (Get Viral Policy)
     */
    typedef struct
    {
        uint8_t enable;
        uint8_t status;
        uint8_t reserved[126];
    } fis_pmem_viral_policy_info_payload_t;

    /**
     * FIS Passthrough Payload for partition info
     * Opcode:     0x06h (Get Admin Features)
     * Sub-Opcode: 0xEAh (Get Extend ADR)
     */
    typedef struct
    {
        uint8_t status;
        uint8_t prv_pwr_cyc_status;
	uint8_t reserved[126];
    } fis_pmem_extend_adr_info_payload_t;

    /**
     * FIS Passthrough Payload for smart health:
     * Opcode: 0x08h (Get Log Page)
     * Sub-Opcode: 0x00h (SMART & Health Info)
     */

    /**
     * Last shutdown status struct
     */
    typedef union
    {
        uint8_t all_flags;
        struct
        {
            uint8_t pm_adr : 1; //!< PM ADR Command received
            uint8_t pm_s3 : 1;  //!< PM Se received
            uint8_t pm_s5 : 1;  //!< PM S5 received
            uint8_t
                ddrt_power_failure : 1;    //!< DDRT Power Fail Command received
            uint8_t pmic_power_loss : 1;   //!< PMIC Power Loss
            uint8_t pm_warm_reset : 1;     //!< PM Warm Reset received
            uint8_t thermal_shutdown : 1;  //!< Thermal Shutdown received
            uint8_t fwf_lush_complete : 1; //!< Flush Complete
        } separated;
    } last_shutdown_status_datails_t;

    /**
     * Last shutdown status extended struct
     */
    typedef union
    {
        uint8_t raw[3];
        struct
        {
            uint16_t viral_interrupt : 1; //!< Viral interrupt received
            uint16_t surprise_clock_stop_interrupt : 1; //!< Surprise clock stop
                                                        //!< interrupt received
            uint16_t
                write_data_flush_complete : 1; //!< Write Data Flush Complete
            uint16_t s4_power_state : 1;       //!< S4 Power State received
            uint16_t pm_idle : 1;              //!< PM Idle received
            uint16_t ddrt_surprise_reset : 1;  //!< Surprise Reset received
            uint16_t enhanced_adr_flush_status : 4; //!< eADR Flush Status
        } separated;
    } last_shutdown_status_datails_extended_t;

    /**
     * smart validation flags
     */
    typedef union _smart_validation_flags
    {
        uint32_t all_flags;
        struct
        {
            uint32_t health_status : 1;
            uint32_t percentage_remaining : 1;
            uint32_t : 1; //!< Reserved
            uint32_t media_temperature : 1;
            uint32_t controller_temperature : 1;
            uint32_t latched_dirty_shutdown_count : 1;
            uint32_t AITDRAM_status : 1;
            uint32_t health_status_reason : 1;
            uint32_t : 1; //!< Reserved
            uint32_t alarm_trips : 1;
            uint32_t latched_last_shutdown_status : 1;
            uint32_t size_of_vendor_specific_data_valid : 1;
            uint32_t : 20; //!< Reserved
        } separated;
    } smart_validation_flags_t;

    /**
     * Smart vendor specific data
     */
    typedef struct
    {
        //uint32_t power_cycles;  //!< Number of PMEM power cycles
        //uint32_t power_on_time; //!< Lifetime hours the PMEM has been powered on
        uint64_t power_cycles;  //!< Number of PMEM power cycles
        uint64_t power_on_time; //!< Lifetime hours the PMEM has been powered on
                                //!< (represented in seconds)
        //uint32_t
	uint64_t
            up_time; //!< Current uptime of the PMEM for the current power cycle
        uint32_t unlatched_dirty_shutdown_count; //!< This is the # of times
                                                 //!< that the FW received an
                                                 //!< unexpected power loss

        /**
         * Display the status of the last shutdown that occurred
         * Bit 0: PM ADR Command (0 - Not Received, 1 - Received)
         * Bit 1: PM S3 (0 - Not Received, 1 - Received)
         * Bit 2: PM S5 (0 - Not Received, 1 - Received)
         * Bit 3: DDRT Power Fail Command Received (0 - Not Received, 1 -
         * Received) Bit 4: PMIC Power Loss (0 - Not Received, 1 - PMIC Power
         * Loss) Bit 5: PM Warm Reset (0 - Not Received, 1 - Received) Bit 6:
         * Thermal Shutdown Received (0 - Did not occur, 1 Thermal Shutdown
         * Triggered) Bit 7: Controller Flush Complete (0 - Did not occur, 1 -
         * Completed)
         */
        last_shutdown_status_datails_t latched_last_shutdown_details;

        //uint32_t last_shutdown_time;
        uint64_t last_shutdown_time;

        /**
         * Display extended details of the last shutdown that occured
         * Bit 0: Viral Interrupt Command (0 - Not Received, 1 - Received)
         * Bit 1: Surprise Clock Stop Interrupt (0 - Not Received, 1 - Received)
         * Bit 2: Write Data Flush Complete (0 - Not Completed, 1 - Completed)
         * Bit 3: S4 Power State (0 - Not Received, 1 - Received)
         * Bit 4: PM Idle (0 - Not Received, 1 - Received)
         * Bit 5: Surprise Reset (0 - Not Received, 1 - Received)
         * Bit 6-23: Reserved
         */
        last_shutdown_status_datails_extended_t
            latched_last_shutdown_extended_details;

        uint8_t reserved[2];

        /**
         * Display the status of the last shutdown that occurred
         * Bit 0: PM ADR Command (0 - Not Received, 1 - Received)
         * Bit 1: PM S3 (0 - Not Received, 1 - Received)
         * Bit 2: PM S5 (0 - Not Received, 1 - Received)
         * Bit 3: DDRT Power Fail Command Received (0 - Not Received, 1 -
         * Received) Bit 4: PMIC Power Loss (0 - Not Received, 1 - PMIC Power
         * Loss) Bit 5: PM Warm Reset (0 - Not Received, 1 - Received) Bit 6:
         * Thermal Shutdown Received (0 - Did not occur, 1 Thermal Shutdown
         * Triggered) Bit 7: Controller Flush Complete (0 - Did not occur, 1 -
         * Completed)
         */
        last_shutdown_status_datails_t unlatched_last_shutdown_details;

        /**
         * Display extended details of the last shutdown that occured
         * Bit 0: Viral Interrupt Command (0 - Not Received, 1 - Received)
         * Bit 1: Surprise Clock Stop Interrupt (0 - Not Received, 1 - Received)
         * Bit 2: Write Data Flush Complete (0 - Not Completed, 1 - Completed)
         * Bit 3: S4 Power State (0 - Not Received, 1 - Received)
         * Bit 4: PM Idle (0 - Not Received, 1 - Received)
         * Bit 5: Surprise Reset (0 - Not Received, 1 - Received)
         * Bit 6-23: Reserved
         */
        last_shutdown_status_datails_extended_t
            unlatched_last_shutdown_extended_details;

        temperature_t max_media_temperature; //!< The highest die temperature
                                             //!< reported in degrees Celsius.
        temperature_t max_controller_temperature; //!< The highest controller
                                                  //!< temperature repored in
                                                  //!< degrees Celsius.

        uint8_t thermal_throttle_performance_loss_percent; //!< The average loss
                                                           //!< % due to thermal
                                                           //!< throttling since
                                                           //!< last read in
                                                           //!< current boot
        uint8_t reserved1[41];
    } smart_intel_specific_data_t;

    /**
     * FIS Smart and Health Info
     */
    typedef struct
    {
        /** If validation flag is not set it indicates that corresponding field
         * is not valid**/
        smart_validation_flags_t validation_flags;

        uint32_t reserved;
        /**
         * Overall health summary
         * Bit 0: Normal (no issues detected)
         * Bit 1: Noncritical (maintenance required)
         * Bit 2: Critical (features or performance degraded due to failure)
         * Bit 3: Fatal (data loss has occurred or is imminent)
         * Bits 7-4 Reserved
         */
        uint8_t health_status;

        uint8_t
            percentage_remaining; //!< remaining percentage remaining as a
                                  //!< percentage of factory configured spare

        uint8_t reserved2;
        /**
         * Bits to signify whether or not values has tripped their respective
         * thresholds. Bit 0: Spare Blocks trips (0 - not tripped, 1 - tripped)
         * Bit 1: Media Temperature trip (0 - not tripped, 1 - tripped) Bit 2:
         * Controller Temperature trip (0 - not tripped, 1 - tripped)
         */
        union
        {
            uint8_t all_flags;
            struct
            {
                uint8_t percentage_remaining : 1;
                uint8_t media_temperature : 1;
                uint8_t controller_temperature : 1;
            } separated;
        } alarm_trips;

        temperature_t
            media_temperature; //!< Current temperature in Celcius. This is
                               //!< the highest die temperature reported.
        temperature_t
            controller_temperature; //!< Current temperature in Celcius.
                                    //!< This is the temperature of the
                                    //!< controller.

        uint32_t latched_dirty_shutdown_count; //!< Number of times the PMEM
                                               //!< Last Shutdown State (LSS)
                                               //!< was non-zero.
        uint8_t AITDRAMStatus; //!< The current state of the AIT DRAM (0 -
                               //!< failure occurred, 1 - loaded)
        uint16_t health_status_reason; //!<  Indicates why the module is in the
                                       //!<  current Health State
        uint8_t reserved3[8];

        /**
         * 00h:       Clean Shutdown
         * 01h - FFh: Not Clean Shutdown
         */
        uint8_t latched_last_shutdown_status;
        uint32_t
            vendor_specific_data_size; //!< Size of Vendor specific structure

        smart_intel_specific_data_t vendor_specific_data;
    } fis_smart_and_health_payload_t;

    /**
     * Passthrough Input Payload:
     * Opcode:      0x08h (Get Log Page)
     * Sub-Opcode:  0x03h (Memory Info)
     */
    typedef struct
    {
        uint8_t memory_page; //!< Page of the memory information to retrieve
        uint8_t reserved[127];
    } fis_memory_health_info_input_payload;

#ifndef __SIZEOF_INT128__
    typedef struct
    {
        uint64_t uint64;
        uint64_t uint64_1;
    } __uint128_t;
#endif

    /**
     * Passthrough Output Payload:
     * Opcode:      0x08h (Get Log Page)
     * Sub-Opcode:  0x03h (Memory Info)
     * Page: 0 (Current Boot Info)
     */
    typedef struct
    {
        __uint128_t media_reads;  //!< Number of 64 byte reads from media on the
                                  //!< PMEM since last AC cycle
        __uint128_t media_writes; //!< Number of 64 byte writes to media on the
                                  //!< PMEM since last AC cycle
        __uint128_t read_requests;  //!< Number of DDRT read transactions the
                                    //!< PMEM has serviced since last AC cycle
        __uint128_t write_requests; //!< Number of DDRT write transactions the
                                    //!< PMEM has serviced since last AC cycle
        uint8_t reserved[64];       //!< Reserved
    } fis_memory_health_info_output_payload;

    /**
     * Passthrough Payload for Power management policy:
     * Opcode:    0x04h (Get Features)
     * Sub-Opcode:  0x02h (Power Managment Policy)
     */
    typedef struct
    {
        uint8_t reserved1[3];
        /**
         * Power limit in mW used for averaged power.
         * Valid range for power limit 10000 - 18000 mW.
         */
        uint16_t average_power_limit;

        uint8_t reserved2;
        /**
         * Returns if the Turbo Mode is currently enabled or not.
         */
        uint8_t memory_bandwidth_boost_feature;
        /**
         * Power limit [mW] used for limiting the Turbo Mode power consumption.
         * Valid range for Turbo Power Limit starts from 15000 - X mW, where X
         * represents the value returned from Get Device Characteristics
         * command's Max Turbo Mode Power Consumption field.
         */
        uint16_t memory_bandwidth_boost_max_power_limit;
        /**
         * The value used as a base time window for power usage measurements
         * [ms].
         */
        uint32_t memory_bandwidth_boost_average_power_time_constant;

        uint8_t reserved3[115];
    } pt_payload_power_management_policy_2_1_t;
    /**
     * Fis Power Management Policy payload
     */
    typedef struct
    {
        uint8_t fis_major;
        uint8_t fis_minor;
        union
        {
            pt_payload_power_management_policy_2_1_t fis_2_01;
            uint8_t data[0];
        } payload;
    } fis_power_management_policy_payload_t;
    /**
      Passthrough Payload:
        Opcode:    0x04h (Get Features)
        Sub-Opcode:  0x03h (Package Sparing Policy)
    **/
    typedef struct
    {
        uint8_t enable;    //!< Reflects whether the package sparing policy is
                           //!< enabled or disabled (0x00 = Disabled).
        uint8_t reserved1; //!< Reserved
        uint8_t supported; //!< Designates whether or not the DIMM still
                           //!< supports package sparing.
        uint8_t reserved[125]; //!< 127-3 : Reserved
    } fis_get_package_sparing_policy_payload_t;

   /**
      Passthrough Payload:
        Opcode:    0x04h (Get Features)
        Sub-Opcode:  0x04h (Address Range Scrub)
    **/
    typedef struct
    {
	uint8_t enable;
	uint8_t reserved[3];
	uint64_t dpa_start_address;
	uint64_t dpa_end_address;
	uint64_t dpa_current_address;
	uint8_t reserved2[100];
    } fis_get_address_range_scrub_payload_t;

    /**
      Passthrough Payload:
        Opcode:     0x02h (Get Security Info)
        Sub-Opcode: 0x00h (Get Security State)
    **/
    typedef struct
    {
        union
        {
            struct
            {
                uint32_t reserved1 : 1;
                uint32_t security_enabled : 1;
                uint32_t security_locked : 1;
                uint32_t security_frozen : 1;
                uint32_t user_security_count_expired : 1;
                uint32_t
                    security_not_supported : 1; //!< This SKU does not support
                                                //!< Security Feature Set
                uint32_t BIOS_security_nonce_set : 1;
                uint32_t reserved2 : 1;
                uint32_t master_passphrase_enabled : 1;
                uint32_t master_security_count_expired : 1;
                uint32_t reserved3 : 22;
            } separated;
            uint32_t as_uint32;
        } security_status;

        union
        {
            struct
            {
                uint32_t
                    security_erase_policy : 1; //!< 0 - Never been set, 1 -
                                               //!< Secure Erase Policy opted in
                uint32_t reserved : 31;
            } separated;
            uint32_t as_uint32;
        } opt_in_status;

        uint8_t Reserved[120];
    } fis_get_security_info_payload_t;

    /**
      Passthrough Payload:
        Opcode:     0x02h (Get Security Info)
        Sub-Opcode: 0x02h (Get Security Opt-In)
    **/
    typedef struct
    {
	uint8_t oic;	//Opt-in code
	uint8_t reserved[127];
    } fis_get_security_optin_payload_t;

    /**
     * Update Firmware Payload:
     * Opcode: 0x09h (Update Firmware)
     * Sub-Opcode: 0x0h (Update Firmware)
     */
    typedef struct
    {
        uint16_t phase : 2;
        uint16_t packetNumber : 14;
        uint16_t payloadSelector : 1;
        uint16_t reserved : 15;
        uint8_t data[64];
        uint8_t reserved2[60];
    } fis_update_firmware_payload_t;

    /**
     * Phases for Firmware Payload
     */
    enum UpdatePhases
    {
        start = 0,
        running = 1,
        finish = 2
    };

    /**
     * Firmware Image Info:
     * Opcode: 0x08h (Get Log Page)
     * Sub-Opcode: 0x01h (Firmware Image Info)
     */
    typedef struct
    {
        uint16_t fwBuildVersion;
        uint8_t fwSecurityVersion;
        uint8_t fwRevisionVersion;
        uint8_t fwProductVersion;
        uint8_t reserved;
        uint16_t maxFwImageSize;
        uint8_t reserved2[8];
        uint16_t stagedBuildVersion;
        uint8_t stagedSecurityVersion;
        uint8_t stagedRevisionVersion;
        uint8_t stagedProductVersion;
        uint8_t reserved3;
        uint8_t lastFwUpdateStatus;
        uint8_t reserved4[105];
    } fis_firmware_info_payload_t;

    /**
     * Long Operation Status:
     * Opcode: 0x08h (Get Log Page)
     * Sub-Opcode: 0x04h (Long Operation Status)
     */
    typedef struct
    {
        uint8_t opCode;
        uint8_t subOpCode;
        uint16_t percentComplete;
        uint32_t estimatedTimeLeft;
        uint8_t statusCode;
        uint8_t commandData[119];
    } fis_long_op_status_payload_t;

#define FIS_LONG_OP_DEVICE_BUSY 5

    /**
     * Firmware command long operation status.
     * Execute a FW command to get long op status.
     *
     * @param[in] pmem: The Intel pmem device to retrieve info on
     * @param[out] payload Area to place the info returned from FW
     * @retval RETURN_SUCCESS: Success
     */
    RETURN_STATUS fw_cmd_long_op_status(device_addr_t* device,
                                        fis_long_op_status_payload_t* payload);

    /**
     * Firmware command firmware image info.
     * Execute a FW command to get information firmare image.
     *
     * @param[in] pmem: The Intel pmem device to retrieve info on
     * @param[out] payload Area to place the info returned from FW
     * @retval RETURN_SUCCESS: Success
     */
    RETURN_STATUS
    fw_cmd_firmware_image_info(device_addr_t* device , void* payload);

    /**
     * Passthrough Input Payload:
     * Opcode:      0x08h (Get Log Page)
     * Sub-Opcode:  0x05h (Error logs)
     */
    typedef struct
    {
        union
        {
            uint8_t as_uint8;
            struct
            {
                uint8_t
                    log_level : 1;    //!< Specifies which error log to retrieve
                uint8_t log_type : 1; //!< Specifies which log type to access
                uint8_t log_info : 1; //!< Specifies which log type data to
                                      //!< return (entries / log info)
                uint8_t
                    log_entries_payload_return : 1; //!< Specifies which payload
                                                    //!< return log entries
                                                    //!< (small / large)
                uint8_t : 4;                        //!< Reserved
            } separated;
        } log_parameters;
        uint16_t sequence_number; //!< Log entries with sequence number equal or
                                  //!< higher than the provided will be returned
        uint16_t request_count;   //!< Max number of log entries requested for
                                  //!< this access
        uint8_t reserved[123];
    } fis_error_logs_input_payload_t;

    /**
     * Passthrough Output Payload:
     * Opcode:      0x08h (Get Log Page)
     * Sub-Opcode:  0x05h (Error logs)
     */
    typedef struct
    {
        uint16_t return_count;    //!< Number of log entries returned
        uint8_t log_entries[126]; //!< Media log entry table
    } fis_error_logs_output_payload_t;
    /**
     *  Fw error log priority level
     */
    enum get_error_log_level
    {
        error_log_priority_low = 0x00,
        error_log_priority_high = 0x01,
        error_log_priority_invalid = 0x02,
    };
    /**
     *  Fw error log type
     */
    enum get_error_log_type
    {
        error_log_type_media = 0x00,
        error_log_type_thermal = 0x01,
        error_log_type_invalid = 0x02,
    };
    /**
     *  Fw error log info type
     */
    enum get_error_log_info
    {
        error_log_info_entries = 0x00,
        error_log_info_data = 0x01,
        error_log_info_invalid = 0x02,
    };
    /**
     *  Fw error log payload size
     */
    enum get_error_log_payload_return
    {
        error_log_small_payload = 0x00,
        error_log_large_payload = 0x01,
        error_log_invalid_payload = 0x02,
    };
    /**
     * FW error log info data return
     */
    typedef struct
    {
        uint16_t max_log_entries;
        uint16_t current_sequence_num;
        uint16_t oldest_sequence_num;
        uint64_t oldest_log_entry_timestamp;
        uint64_t newest_log_entry_timestamp;
        uint8_t additional_log_status;
        uint8_t reserved[105];
    } fis_error_log_info_data_return;

    /**
     * Output payload for Thermal Log Entry Format
     */
    typedef struct
    {
        uint64_t system_time_stamp; //!< Unix epoch time of log entry
        union
        {
            uint32_t as_uint32;
            struct
            {
                uint32_t temperature : 15; //!< In celsius
                uint32_t sign : 1;         //!< Positive or negative
                uint32_t reported : 3;     //!< Temperature being reported
                uint32_t type : 2;         //!< Controller or media temperature
                uint32_t : 11;             //!< Reserved
            } separated;
        } host_reported_temp_data;
        uint16_t sequence_num;
        uint8_t reserved[2];
    } fis_error_logs_output_payload_thermal_entry;

    /**
     * Output payload for Media Log Entry Format
     */
    typedef struct
    {
        uint64_t system_time_stamp; //!< Unix epoch time of log entry
        uint64_t dpa;               //!< Specifies DPA address of error
        uint64_t pda;               //!< Specifies PDA address of the failure
        uint8_t range; //!< Specifies the length in address space of this error.
                       //!< Ranges will be encoded as power of 2.
        uint8_t error_type; //!< Indicates what kind of error was logged.
        union
        {
            uint8_t as_uint8; //!< Indicates error flags for this entry.
            struct
            {
                uint8_t pda_valid : 1; //!< Indicates the PDA address is valid.
                uint8_t dpa_valid : 1; //!< Indicates the DPA address is valid.
                uint8_t interrupt : 1; //!< Indicates this error generated an
                                       //!< interrupt packet
                uint8_t : 1;           //!< Reserved
                uint8_t
                    viral : 1; //!< Indicates Viral was signaled for this error
                uint8_t : 3;   //!< Reserved
            } spearated;
        } error_flags;
        uint8_t
            transaction_type; //!< Indicates what transaction caused the error
        uint16_t sequence_num;
        uint8_t reserved[2];
    } fis_error_logs_output_payload_media_entry;

    /**
     * Firmware command update firmware block.
     * Execute a FW command to update firmware.
     *
     * @param[in] device: The Intel pmem module to update
     * @param[in] payload: The image to update the memory to
     * @param[in] size: The size of the image
     * @retval RETURN_SUCCESS: Success
     */
    RETURN_STATUS fw_cmd_update_firmware_block(device_addr_t* device,
                                               const uint8_t* payload,
                                               size_t size, size_t block);

    /**
     * Firmware command Identify PMEM.
     * Execute a FW command to get information about PMEM.
     *
     * @param[in] pmem: The Intel pmem module to retrieve identify info on
     * @param[out] payload: Area to place the identity info returned from FW
     * @retval RETURN_SUCCESS: Success
     */

    RETURN_STATUS
    fw_cmd_device_id(device_addr_t* device, void* payload);


    /**
    * Execute Firmware command to Get PMEM Config Lockdown
    *
    * @param[in] device The Intel PMEM to retrieve configuration lockdown status
    * @param[out] payload Area to place returned info from FW
    *
    * @retval RETURN_SUCCESS : Success
    */
    RETURN_STATUS
    fw_cmd_get_pmem_config_lockdown(device_addr_t* device, void* payload);

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
    fw_cmd_get_pmem_latch_shutdown_state(device_addr_t* device, void* payload);

    /**
    * Execute Firmware command to Get PMEM Viral Policy
    *
    * @param[in] device The Intel PMEM to retrieve viral policy status
    * @param[out] payload Area to place returned info from FW
    *
    * @retval RETURN_SUCCESS : Success
    */
    RETURN_STATUS
    fw_cmd_get_pmem_viral_policy(device_addr_t* device, void* payload);

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
    fw_cmd_get_pmem_extend_adr(device_addr_t* device, void* payload);


    /**
     * Firmware command to get SMART and Health Info
     *
     * @param[in] pmem: The Intel pmem module to retrieve SMART and Health Info
     * @param[out] payloadSmartAndHealth: Area to place SMART and Health
     *  Info data
     * The caller is responsible to free the allocated memory with the
     *  FreePool function.
     *
     * @retval RETURN_SUCCESS Success
     * @retval RETURN_INVALID_PARAMETER pmem or payloadSmartAndHealth
     *  is NULL
     */

     RETURN_STATUS
    fw_cmd_get_smart_and_health(device_addr_t* device , void* payloadSmartAndHealth);

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
                             fis_memory_health_info_output_payload* payload);

    /**
     * Firmware command firmware activation.
     * Execute a FW command to activate a firmware update.
     *
     * @param[in] pmem: The Intel pmem device address
     * @retval RETURN_SUCCESS: Success
     */
    RETURN_STATUS
    fw_cmd_fw_activation(device_addr_t* device);

    /**
     * Firmware command to get Power Management Policy Info (for FIS 2.3+)
     * Execute fw command to get power management policy for pmem device
     *
     * @param[in] device: The Intel PMem device address to retrieve Power
     * Management Policy Info
     * @param[out] payload: Area to place Power Management
     * Policy Info data The caller is responsible to free the allocated memory
     * with the FreePool function.
     *
     * @retval RETURN_SUCCESS Success
     * @retval RETURN_INVALID_PARAMETER device or payload is NULL
     */
    RETURN_STATUS
    fw_cmd_get_power_management_policy(device_addr_t* device , void* payload);



	/**
	 * Firmware command to get Alaram Thresholds
	 *
	 * @param[in] device The Intel PMEM to retrieve alaram thresholds info for
	 * @param[out] payload Area to place returned info from FW
	 *
	 * @retval RETURN_SUCCESS            Success
	 */

        RETURN_STATUS
        fw_cmd_get_alarmthresholds(device_addr_t* device , void* payload);

	/**
	 * Execute Firmware command to get address range scrub
	 *
	 * @param[in] device The Intel PMEM to retrieve address range scrub
	 * @param[out] payload Area to place returned info from FW
	 *
	 * @retval RETURN_SUCCESS : Success
	 */
      RETURN_STATUS
        fw_cmd_get_address_range_scrub(device_addr_t* device , void* payload);


    /**
     * Firmware command to get Device Characteristics
     *
     * @param[in] device The Intel PMEM to retrieve device characteristics info
     * for
     * @param[out] payload Area to place returned info from FW
     *
     * @retval RETURN_SUCCESS            Success
     * @retval RETURN_INVALID_PARAMETER  One or more input parameters are NULL
     */
    RETURN_STATUS
    fw_cmd_device_characteristics(device_addr_t* device , void* payload);

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
    fw_cmd_get_pmem_partition_info(device_addr_t* device , void* payload);

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
        device_addr_t* device,
        fis_get_package_sparing_policy_payload_t* payload);
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
    fw_cmd_get_security_info(device_addr_t* device , void* payload);

    /**
    * Execute Firmware command to get security opt-in
    *
    * @param[in] device The Intel PMEM to retrieve Security Opt-in
    * @param[out] payload Area to place returned info from FW
    *
    * @retval RETURN_SUCCESS : Success
    */
    RETURN_STATUS
    fw_cmd_get_security_optin(device_addr_t* device,
                         fis_get_security_optin_payload_t* payload);

    /**
     * Firmware command to get Error logs
     *
     * @param[in] pmem: The Intel pmem module to retrieve
     * @param[in] input_payload - filled input payload
     * @param[out] output_payload - small payload result data of get error log
     * operation
     * @param[in] output_payload_size - size of small payload
     * @param[out] large_output_payload - large ouput payload.
     * @param[in] larger_ouput_payload_size - size of large payload.
     *
     * @retval RETURN_STATUS.
     */
    RETURN_STATUS
    fw_cmd_get_error_log(device_addr_t* device,
                         fis_error_logs_input_payload_t* input_payload,
                         void* output_payload, uint32_t output_payload_size,
                         void* large_output_payload,
                         uint32_t large_ouput_payload_size);

    RETURN_STATUS fis_pass_thru(device_addr_t* device, fw_cmd_t* fw_cmd,
                                uint64_t timeout);

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
