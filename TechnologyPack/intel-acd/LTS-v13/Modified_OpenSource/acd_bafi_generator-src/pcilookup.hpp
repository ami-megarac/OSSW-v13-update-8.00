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
#include <filesystem>
#include <fstream>
#include <tuple>
#include <vector>

#ifdef _WIN32
#define OS_SEP "\\"
#define BMC_PATH "C:\\temp\\bafi\\"
#else
#define OS_SEP "/"
#define BMC_PATH "/conf/crashdump/input/" //AMI
#endif

const std::string LOCAL_PATH = ".";

using json = nlohmann::json;

class PciBdfLookup
{
  public:
    [[nodiscard]] bool static from_json(json &memoryMapJson)
    {
        for (const auto &[key, value] : memoryMapJson["memoryMap"].items())
        {
            mmioItem singleMapEntry;
            if (!str2uint(std::string(value["addressBase"]), singleMapEntry.base))
            {
                return false;
            }

            if (!str2uint(std::string(value["addressLimit"]), singleMapEntry.limit))
            {
                return false;
            }

            if (!str2uint(std::string(value["bus"]), singleMapEntry.bus))
            {
                return false;
            }

            if (!str2uint(std::string(value["device"]), singleMapEntry.dev))
            {
                return false;
            }

            if (!str2uint(std::string(value["function"]), singleMapEntry.func))
            {
                return false;
            }

            memorymap.push_back(singleMapEntry);
        }
        return true;
    }

    [[nodiscard]] static std::tuple<uint8_t, uint8_t, uint8_t> lookup(const uint64_t address)
    {
        for (const auto &iter : memorymap)
        {
            if (address >= iter.base && address <= iter.limit)
            {
                return std::make_tuple(iter.bus, iter.dev, iter.func);
            }
        }
        return std::make_tuple(0, 0, 0);
    }

    PciBdfLookup(char *mapFileLocation)
    {
        std::ifstream i(mapFileLocation);
        std::filesystem::path file_path(mapFileLocation);
        if (!i.good())
        {
            std::cerr << "File '" << mapFileLocation << "' not found.\n";
            return;
        }

        if (std::filesystem::is_symlink(file_path))
        {
            std::cerr << "File '" << *mapFileLocation << "' is a symlink\n";
            return;
        }

        if (std::filesystem::hard_link_count(file_path) > 1)
        {
            std::cerr << "File '" << *mapFileLocation << "' is a hard link\n";
            return;
        }

        json memoryMap = json::parse(i, nullptr, false);

        if (memoryMap.is_discarded())
        {
            std::cerr << "File '" << mapFileLocation << "' is not a valid JSON file\n";
            return;
        }

        if (!PciBdfLookup::from_json(memoryMap))
        {
            std::cerr << "Failed to load " << mapFileLocation << " file" << std::endl;
            return;
        }
        memorymapExistence = true;
    }

    PciBdfLookup(char *binaryLocation, bool, std::string fileName)
    {
        std::ifstream i;

        i.open((BMC_PATH + fileName).c_str());
        if (i.good())
        {
            lookupMap = json::parse(i, nullptr, false);
            if (!lookupMap.is_discarded() && from_json(lookupMap))
            {
                memorymapExistence = true;
            }
        }
        else
        {
            std::string pathToBinaryLocation = binaryLocation;
            std::string pathWithoutBinary = pathToBinaryLocation.substr(0, pathToBinaryLocation.find_last_of(OS_SEP));
            if (pathWithoutBinary != LOCAL_PATH && pathToBinaryLocation != pathWithoutBinary)
            {
                std::string mapPath = pathWithoutBinary + OS_SEP + fileName;
                i.open(mapPath);
                if (i.good())
                {
                    lookupMap = json::parse(i, nullptr, false);
                    if (!lookupMap.is_discarded() && from_json(lookupMap))
                    {
                        memorymapExistence = true;
                    }
                }
            }
            else
            {
                i.open(fileName.c_str());
                if (i.good())
                {
                    lookupMap = json::parse(i, nullptr, false);
                    if (!lookupMap.is_discarded() && from_json(lookupMap))
                    {
                        memorymapExistence = true;
                    }
                }
            }
        }

        if (memorymapExistence == false && lookupMap == NULL)
        {
            lookupMap = {
                {0x00000001, 0x00000002, 0x1, 0x0, 0x0},
                /*
                Examples of other memory map entries
                {0xCB300000, 0xCB3FFFFF, 0x65, 0, 0},
                {0xC6000000, 0xCB2FFFFF, 0x65, 0, 0},
                {0xE1000000, 0xE10FFFFF, 0xb3, 0, 0},
                {0xC000, 0xCFFF, 0xb3, 0, 0}
                */
            };
            memorymapExistence = true;
        }
    }

    struct mmioItem
    {
        uint64_t base;
        uint64_t limit;
        uint8_t bus;
        uint8_t dev;
        uint8_t func;
    };
    json lookupMap;
    static std::vector<mmioItem> memorymap;
    static bool memorymapExistence;
};

json lookupMap = NULL;
std::vector<PciBdfLookup::mmioItem> PciBdfLookup::memorymap = {};
bool PciBdfLookup::memorymapExistence = false;
