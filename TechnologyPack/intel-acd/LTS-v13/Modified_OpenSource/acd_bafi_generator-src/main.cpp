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

#define MEMORY_MAP_FILENAME "default_memory_map.json"
#define DEVICE_MAP_FILENAME "default_device_map.json"
#define SILKSCREEN_MAP_FILENAME "default_silkscreen_map.json"

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <mca.hpp>
#include <nlohmann/json.hpp>
#include <pcilookup.hpp>
#include <report.hpp>
#include <summary.hpp>
#include <utils.hpp>
#include <cpu_factory.hpp>
#include <triage.hpp>
//AMI define
#define BAFI_SUPPORTED_CPUS  "SKX, CLX, CPX, ICX, SPR, ICX-D, SPR-HBM, EMR"

void saveDumpFile(std::string fileName, nlohmann::ordered_json report,
                  bool oneLinePrint)
{
  std::cout << "saveDumpFile\n" << std::endl;
  std::filesystem::path filePath = std::filesystem::current_path().string();
  filePath /= "/var/crashdump/json/BAFI.json";
        std::cout << "filePath " << filePath << std::endl;         
  std::ofstream file(filePath);
  if (oneLinePrint)
    file << report;
  else
    file << report.dump(4);
  file.close();
}

void collectFilesFromDir(std::vector<std::string> &files)
{
    std::string path = std::filesystem::current_path().string();
    std::regex json_file_lower(".*.json");
    std::regex json_file_capital(".*.JSON");
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        if (std::regex_search(entry.path().string(), json_file_lower) ||
            std::regex_search(entry.path().string(), json_file_capital))
        {
          std::string line;
          std::ifstream file;
          file.open(entry.path().string());
          while (getline(file, line))
          {
              if (line.find("METADATA") != std::string::npos || line.find("metadata") != std::string::npos)
                  files.push_back(entry.path().string());
          }
        }
    }
}

/*Reason for False Positive - execute the code normally*/
/*coverity[UNCAUGHT_EXCEPT : FALSE]*/
int main(int argc, char *argv[]) {
  auto start = std::chrono::steady_clock::now();
  bool oneLinePrint = false;
  bool decodeMultipleDumps = false;
  bool printTriageInfo = false;
  bool deviceMapProvided = false;
  bool silkscreenMapProvided = false;
  json deviceMap = NULL;
  json silkscreenMap = NULL;
  char *triageInfo = NULL;
  size_t triageInfoSize = 0;
  char *fullOutput = NULL;
  size_t fullOutputSize = 0;
  uint8_t minNumberOfParameters = 2;
  uint8_t maxNumberOfParameters = 9;
  uint8_t binaryFileArgumentPosition = 0;
  uint8_t crashdumpFileArgumentPosition = 1;
  std::vector<std::string> files;
try{
  if (argc < minNumberOfParameters || argc > maxNumberOfParameters)
  {
    printHelp();
    return 8;
  }

  if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
  {
    printHelp();
    return 0;
  }

  if (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version")
  {
    std::cout << "BAFI version " << BAFI_VERSION << std::endl;
    std::cout << "CPU release support: \n" << AMI_RELEASED << std::endl;
    std::cout << "CPU engineering support: \n" << AMI_UNRELEASED << std::endl;
    return 0;
  }

  for (int i = 0; i < argc; i++)
  {
    if (std::string(argv[i]) == "-t" || std::string(argv[i]) == "--triage")
    {
      printTriageInfo = true;
      crashdumpFileArgumentPosition++;
    }

    if (std::string(argv[i]) == "-p" || std::string(argv[i]) == "--pcie_names")
    {
      deviceMapProvided = true;
      crashdumpFileArgumentPosition += 2;
      std::ifstream iDev(argv[i + 1]);
      std::filesystem::path file_path(argv[i + 1]);
      if (!iDev.good())
      {
        std::cerr << "File '"
              << "' not found.\n";
        printHelp();
        return 1;
      }

      // Check if file is a symlink
      if (std::filesystem::is_symlink(file_path))
      {
        std::cerr << "File '" << argv[i + 1] << "' is a symlink\n";
        return 6;
      }

      // Check if file is a hard link
      if (std::filesystem::hard_link_count(file_path) > 1)
      {
        std::cerr << "File '" << argv[i + 1] << "' is a hard link\n";
        return 7;
      }

      deviceMap = json::parse(iDev, nullptr, false);
    }

    if (std::string(argv[i]) == "-s" ||
        std::string(argv[i]) == "--silkscreen_names")
    {
      silkscreenMapProvided = true;
      crashdumpFileArgumentPosition += 2;
      std::ifstream iDev(argv[i + 1]);
      std::filesystem::path file_path(argv[i + 1]);
      if (!iDev.good())
      {
        std::cerr << "File '"
              << "' not found.\n";
        printHelp();
        return 1;
      }

      // Check if file is a symlink
      if (std::filesystem::is_symlink(file_path))
      {
        std::cerr << "File '" << argv[i + 1] << "' is a symlink\n";
        return 6;
      }

      // Check if file is a hard link
      if (std::filesystem::hard_link_count(file_path) > 1)
      {
        std::cerr << "File '" << argv[i + 1] << "' is a hard link\n";
        return 7;
      }

      silkscreenMap = json::parse(iDev, nullptr, false);
    }

    if (std::string(argv[i]) == "-d")
    {
      decodeMultipleDumps = true;
      collectFilesFromDir(files);
    }

    if (std::string(argv[i]) == "-m" || std::string(argv[i]) == "--memory-map")
    {
      crashdumpFileArgumentPosition += 2;
      PciBdfLookup PciBdfLookup(argv[i + 1]);
    }
    else
    {
      PciBdfLookup PciBdfLookup(argv[binaryFileArgumentPosition], true,
        MEMORY_MAP_FILENAME);
    }
  }

  if (!decodeMultipleDumps && argv[crashdumpFileArgumentPosition] != NULL)
  {
    files.push_back(std::string(argv[crashdumpFileArgumentPosition]));
  }

  if (!deviceMapProvided)
  {
    PciBdfLookup PciBdfLookup(argv[binaryFileArgumentPosition], true,
                              DEVICE_MAP_FILENAME);
    deviceMap = PciBdfLookup.lookupMap;
  }

  if (!silkscreenMapProvided)
  {
    PciBdfLookup PciBdfLookup(argv[binaryFileArgumentPosition], true,
                              SILKSCREEN_MAP_FILENAME);
    silkscreenMap = PciBdfLookup.lookupMap;
  }

  for (uint32_t it = 0; it < files.size(); it++)
  {
    std::ifstream i(files[it]);
    if (!i.good())
    {
      std::cerr << "File '" << files[it] << "' not found.\n";
      printHelp();
      return 1;
    }
    std::filesystem::path file_path(files[it]);
    // Check if it's a regular file or a directory
    if (std::filesystem::is_directory(file_path))
      continue;

    // Check if file is symlink
    if(std::filesystem::is_symlink(file_path))
    {
      std::cerr << "File '" << files[it] << "' is a symlink\n";
      return 6;
    }

    // Check if file is a hard link
    if (std::filesystem::hard_link_count(file_path) > 1)
    {
      std::cerr << "File '" << files[it] << "' is a hard link\n";
      return 7;
    }

    // Read and check proper JSON format
    json inputData = json::parse(i, nullptr, false);
    if (inputData.is_discarded())
    {
      std::cerr << "File '" << files[it] << "' is not a valid JSON file\n";
      return 9;
    }
    //start-AMI code
    auto input = getProperRootNode(inputData);
    if (!input) {
      std::cerr << "Incorrect JSON root node. Expected crash_data or no root "
                   "node.\n";
      return 9;
    }
    //end-AMI code
    nlohmann::ordered_json fullReport;
    nlohmann::ordered_json triageReport;
    uint32_t fullOutputStatus = 6;
    uint32_t triageInfoStatus = 6;
    std::string jsonDumpStr = inputData.dump();
    const char *jsonDumpChar = jsonDumpStr.c_str();
    char *jsonDump = new char[strlen(jsonDumpChar) + 1];
    //AMI bullseye
    memcpy(jsonDump, jsonDumpChar, strlen(jsonDumpChar) + 1);

    if (printTriageInfo)
    {
      // Generate triage information
      triageInfoStatus = getTriageInformation(inputData, triageInfo, &triageInfoSize, deviceMap, silkscreenMap, true);
    }
    else
    {
      // Generate full output
      fullOutputStatus = getFullOutput(inputData, fullOutput, &fullOutputSize, deviceMap, silkscreenMap, true);
    }

    if (fullOutputStatus == 0)
    {
      fullReport = nlohmann::ordered_json::parse(fullOutput);
    }
    else if (triageInfoStatus == 0)
    {
      triageReport = nlohmann::ordered_json::parse(triageInfo);
    }

    if (fullOutputStatus == 1 || triageInfoStatus == 1)
    {
      std::cerr << "No crashdump file provided \n";
      printHelp();
      delete triageInfo;
      delete fullOutput;
      return 1;
    }
    else if (fullOutputStatus == 3 || triageInfoStatus == 3)
    {
      std::cerr << "Incorrect JSON root node. Expected crash_data or no root "
                 "node.\n";
      return 3;
    }
    else if (fullOutputStatus == 4 || triageInfoStatus == 4)
    {
      std::size_t found = files[it].find_last_of("/\\");
      std::cerr << "Unrecognized CPU type. Supported CPUs: " <<
                  BAFI_SUPPORTED_CPUS << std::endl;
      std::cerr << files[it].substr(found + 1) << std::endl;
      continue;
    }
    else if (fullOutputStatus == 5 || triageInfoStatus == 5)
    {
      std::cerr << "Failed to prepare report" << std::endl;
      return 5;
    }
    //Start-AMI code
    // Create summary object of all data in input JSON
    // Summary summary(getCpuType(*input));
    // Identify CPU type - ICX for Ice Lake, CPX for Cooper Lake, SKX for
    // Sky Lake and SPR for Sapphire Rapids
    //summary.cpuType = getCpuType(*input);
    //auto cpu = CpuFactory::createInstance(summary.cpuType, "");
    /*if (summary.cpuType == "SPR")
    {
      auto cpuId = getCpuId(*input);
      cpu->cpuId = cpuId.value_or("empty");
    }*/
    auto cpu = CpuFactory::createInstance(getCpuType(*input), "");
    if (cpu != nullptr)
    {
      //End-AMI code
      if (oneLinePrint && !decodeMultipleDumps)
      {
        std::cout << fullReport << std::endl;
      }
      else if (decodeMultipleDumps && !printTriageInfo)
      {
        saveDumpFile(files[it], fullReport, oneLinePrint);
      }
      else if (printTriageInfo && !decodeMultipleDumps)
      {
        uint32_t status = getTriageInformation(jsonDump, strlen(jsonDump),
        triageInfo, &triageInfoSize, deviceMap);
        if (status == 0)
	        saveDumpFile(files[it], triageReport, oneLinePrint);//AMI code
        if (triageInfo != NULL)
          delete triageInfo;
      }
      else if (printTriageInfo && decodeMultipleDumps)
      {
        uint32_t status = getTriageInformation(jsonDump, strlen(jsonDump),
        triageInfo, &triageInfoSize, deviceMap);
        if (status == 0)
          saveDumpFile(files[it], triageReport, oneLinePrint);
        if (triageInfo != NULL)
          delete triageInfo;
      }
      else
      {
        // Dump report in 'pretty printed' JSON format
        std::cout << fullReport.dump(4) << std::endl;
      }
    }
    else
    {
	     std::size_t found = files[it].find_last_of("/\\");
	     std::cerr << "Unrecognized CPU type. Supported CPUs:" <<
                  BAFI_SUPPORTED_CPUS << std::endl;
       std::cerr << files[it].substr(found + 1) << std::endl;
    }
    if (!printTriageInfo)
    {
      delete fullOutput;
      delete triageInfo;
    }
    else
    {
      delete fullOutput;
    }
    delete[] jsonDump;
  }
  return 0;
  }
   catch(std::exception &e)
      {
        std::cout << "a json exception happened" << e.what() <<std::endl;
      }
};
