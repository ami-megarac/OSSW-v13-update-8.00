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

"""

This python script translates Intel cscript pci memory map into BAFI memory map JSON format.
File default_memory_map.json is created in same directory as script.

Usage: python3 csript_map_converter.py <cscript_memory_map_file_format>

Cscript memory map format, created using pcie_resource_check():
|  0 |  0 |  1 | 0 |    0    |     |     |     |         bar0         | 0x0000200ffee50000 - 0x0000200ffee53fff |   |

will be translated into:
{
    "addressBase": "0x200ffee50000",
    "addressLimit": "0x200ffee53fff",
    "bus": "0x0",
    "device": "0x1",
    "function": "0x0"
}
"""

INCORRECT_DATA = ["00000fff", "00000000ffffffff"]
BARS_TO_REMOVE = ["IO_BAR", "MEM_BAR"]
ITP = 0
IOMEM = 1


def print_help():
    print("Usage:")
    print("python3 csript_map_converter.py <cscript_memory_map_file_format> [-o output_path]\n\n")


def transform_mem_map(memory_map_file):
    memory_map = {"memoryMap": []}
    with open(memory_map_file) as fh:
        for line in fh.readlines():
            line_data, pattern = parse_line(line)
            if line_data:
                extracted_data = extract_data(line_data, pattern)
                if extracted_data:
                    memory_map["memoryMap"].append(extracted_data)

    return memory_map


def parse_line(line):
    line = line.replace(" ", "")
    pattern_pci_resource_check = \
        re.compile('^\|([0-9a-fA-F]+)\|([0-9a-fA-F]+)\|([0-9a-fA-F]+)\|([0-9a-fA-F]+)\|.*\|(\w*)\|.*0x([0-9a-fA-F]+)-0x([0-9a-fA-F]+)\|')
    result_pci_resource_check = pattern_pci_resource_check.match(line)
    pattern_iomem_check = \
        re.compile('([0-9a-fA-F]+)\-([0-9a-fA-F]+)\:([0-9]+)\:([0-9a-fA-F]+)\:([0-9a-fA-F]+)\.([0-9a-fA-F]+)')
    result_iomem_resource_check = pattern_iomem_check.match(line)
    if result_pci_resource_check:
        return list(result_pci_resource_check.groups()), ITP
    elif result_iomem_resource_check:
        return list(result_iomem_resource_check.groups()), IOMEM
    else:
        return None, None


def extract_data(line_data, pattern):
    single_entry = {}
    # workaround for not valid entries in input file
    # - remove IO_BAR and MEM_BAR related entries
    # - remove entries with incorrect addresses
    data = remove_incorrect_entries(line_data)
    if not data:
        pass
    elif pattern == ITP:
        single_entry["bus"] = hex(int(data[1], 16))
        single_entry["device"] = hex(int(data[2], 16))
        single_entry["function"] = hex(int(data[3], 16))
        single_entry["addressBase"] = hex(int(data[5], 16))
        single_entry["addressLimit"] = hex(int(data[6], 16))
    elif pattern == IOMEM:
        single_entry["bus"] = hex(int(data[3], 16))
        single_entry["device"] = hex(int(data[4], 16))
        single_entry["function"] = hex(int(data[5], 16))
        single_entry["addressBase"] = hex(int(data[0], 16))
        single_entry["addressLimit"] = hex(int(data[1], 16))

    return single_entry


def remove_incorrect_entries(data):
    if data[-1] in INCORRECT_DATA:
        return None
    elif data[4] in BARS_TO_REMOVE:
        return None
    else:
        return data


def get_arguments():
    parser = ArgumentParser(
        epilog="Example usage: python3 cscript_map_converter.py C:\\temp\\memory_map_file.txt "
               "-o C:\\temp\\output_data.json"
    )
    parser.add_argument("-o", "--output_file", type=str,
                        help="output map file path, default - default_memory_map.json", default="default_memory_map.json")
    parser.add_argument("memory_map_file", type=str,
                        help="memory map file that will be converted to json format", default=None)

    return parser.parse_args()


def save_to_file(memory_map, file_path):
    with open(file_path, "w") as fh:
        fh.write(json.dumps(memory_map, sort_keys=True, indent=4))


def main():
    args = get_arguments()
    memory_map = transform_mem_map(args.memory_map_file)
    save_to_file(memory_map, args.output_file)
    return 0


if __name__ == "__main__":
    main()
