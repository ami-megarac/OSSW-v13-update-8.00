##################################################################################
#
# INTEL CONFIDENTIAL
#
# Copyright 2022 Intel Corporation.
#
# This software and the related documents are Intel copyrighted materials, and
# your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise, you may
# not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express
# or implied warranties, other than those that are expressly stated in
# the License.
#
##################################################################################

import json
import sys
import re
from argparse import ArgumentParser

PCI = 0
LSPCI = 1

def print_help():
    print("Usage:")
    print("python3 devices_log_converter.py <device_log_file_format> [-o output_path]\n\n")


def transform_devices_map(devices_map_file):
    devices_map = {"devicesMap": []}
    with open(devices_map_file) as fh:
        for line in fh.readlines():
            line_data, pattern = parse_line(line)
            if line_data:
                extracted_data = extract_data(line_data, pattern)
                if extracted_data:
                    devices_map["devicesMap"].append(extracted_data)

    return devices_map


def parse_line(line):
    pattern_pci_resource_check = \
        re.compile('^\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)'
        '\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)'
        '\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)'
        '\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)'
        '\|([0-9a-fA-F\.]+)\|([0-9a-fA-F\.]+)\|([0-9a-zA-Z\.]+)\|(.+)\|')
    result_pci_resource_check = pattern_pci_resource_check.match(line)
    pattern_lspci_resource_check = \
        re.compile('^([0-9a-fA-F]+)\:([0-9a-fA-F]+)\.([0-9a-fA-F]+)(.+)')
    result_lspci_resource_check = pattern_lspci_resource_check.match(line)
    if result_pci_resource_check:
        return list(result_pci_resource_check.groups()), PCI
    if result_lspci_resource_check:
        return list(result_lspci_resource_check.groups()), LSPCI
    else:
        return None, None


def extract_data(data, pattern):
    single_entry = {}
    if not data:
        pass
    elif pattern == PCI:
        single_entry["description"] = data[14].strip()
        single_entry["bus"] = hex(int(data[1], 16))
        single_entry["device"] = hex(int(data[2], 16))
        single_entry["function"] = hex(int(data[3], 16))
    elif pattern == LSPCI:
        single_entry["description"] = data[3].strip()
        single_entry["bus"] = hex(int(data[0], 16))
        single_entry["device"] = hex(int(data[1], 16))
        single_entry["function"] = hex(int(data[2], 16))

    return single_entry


def get_arguments():
    parser = ArgumentParser(
        epilog="Example usage: python3 devices_log_converter.py C:\\temp\\device_map_file.log "
               "-o C:\\temp\\output_data.json"
    )
    parser.add_argument("-o", "--output_file", type=str,
                        help="output device file path, default - default_device_map.json", default="default_device_map.json")
    parser.add_argument("device_map_file", type=str,
                        help="device map file that will be converted to json format", default=None)

    return parser.parse_args()


def save_to_file(device_map, file_path):
    with open(file_path, "w") as fh:
        fh.write(json.dumps(device_map, sort_keys=False, indent=4))


def main():
    args = get_arguments()
    device_map = transform_devices_map(args.device_map_file)
    save_to_file(device_map, args.output_file)
    return 0


if __name__ == "__main__":
    main()
