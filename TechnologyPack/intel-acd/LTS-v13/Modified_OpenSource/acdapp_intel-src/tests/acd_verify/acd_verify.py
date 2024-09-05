###############################################################################
# INTEL CONFIDENTIAL                                                          #
#                                                                             #
# Copyright 2021 Intel Corporation.                                           #
#                                                                             #
# This software and the related documents are Intel copyrighted materials,    #
# and your use of them is governed by the express license under which they    #
# were provided to you ("License"). Unless the License provides otherwise,    #
# you may not use, modify, copy, publish, distribute, disclose or transmit    #
# this software or the related documents without Intel's prior written        #
# permission.                                                                 #
#                                                                             #
# This software and the related documents are provided as is, with no express #
# or implied warranties, other than those that are expressly stated in the    #
# License.                                                                    #
###############################################################################

import ast
from lib.Preprocessor import Preprocessor
from lib.Processor import Processor
from lib.Postprocessor import Postprocessor

import json
import os
import argparse
import sys
import warnings


def processJsonFile(rProcess, fVerbose=False, fCompare=False, rCompare='',
                    ignoreList=None,
                    checks=None, checksScope=None):
    '''
    Process the given file with acd_verify.
    This function is now the entry point to acd_verify.

    Parameters:
        rProcess (str): The file to process.
        fVerbose (bool): Flag that triggers verbosed report.
        fCompare (bool): Flag that triggers compare mode.
        rCompare (str): Path to the compare file.
        fCheck3strike (bool): Flag that triggers 3strike check.
        ignoreList

    Returns:
        results (dic): A dictionary containing:
            reportSaved (bool): Indicates if report was able to be saved.
            check3strike: Result of 3strike check when check was performed
                          [True|False], or None.

    [Example] How to import and run processJsonFile:
        From /crashdump/myscript.py do:
                >>> import sys
                >>> sys.path.append('tests/acd_verify')
                >>> from acd_verify import processJsonFile
                >>> rProcess = "JsonFileIWant2Process.json"
                >>> processJsonFile(rProcess)
    '''
    print("Processing file: ", rProcess)
    reportSaved = False
    processor = None
    try:

        # Check file is valid
        rProcessIsFile = os.path.isfile(rProcess)
        rProcessIsJsonFile = rProcess.endswith('.json')
        isProcessFileValid = rProcessIsFile and rProcessIsJsonFile

        results = {
                "reportSaved": reportSaved,
                "check3strike": None
            }

        if (not isProcessFileValid):
            warnings.warn(f"The path given is not a json file {rProcess}")
            return results

        # Check flags
        if (fCompare and ("check3strike" in checks)):
            warnings.warn("Error: --compare and -check3strike given. " +
                          "Only use one at a time")
            return results

        if "checkMCAValueMask" in checks:
            for element in checks["checkMCAValueMask"]:
                if type(element) == list:
                    for x in element:
                        if type(x) == str:
                            # convert to number
                            element[element.index(x)] = int(x, 16)
                        elif type(x) == int:
                            pass
                        else:
                            warnings.warn("Error: --checkMCAValueMask values" +
                                          "are not in the correct format")
                            return results
                else:
                    warnings.warn("Error: --checkMCAValueMask values are not" +
                                  "in the correct format")
                    return results

        # Flags valid and file to process is valid
        f = open(rProcess)
        jOutput = json.load(f)
        acdData = jOutput["crash_data"]

        # Compare
        compareFileName = None
        preprocessorCompare = None
        if fCompare:
            compareIsFile = os.path.isfile(rCompare)
            compareIsJsonFile = rCompare.endswith('.json')
            if compareIsFile and compareIsJsonFile:
                print("Comparing vs file: ", rCompare)
                f = open(rCompare)
                jCompare = json.load(f)
                compareData = jCompare["crash_data"]
                compareFileName = os.path.splitext(
                        os.path.basename(rCompare))[0]
                preprocessorCompare = Preprocessor(compareData)
            else:
                print(f"Compare path given is not valid: {rCompare}")

        preprocessor = Preprocessor(acdData, checks)
        processor = Processor(preprocessor.sections,
                              preprocessorCompare, ignoreList,
                              checks=checks, checksScope=checksScope)
        postprocessor = Postprocessor(
            processor.report, rProcess,
            "txt", fVerbose, compareFileName)

        reportSaved = postprocessor.saveFile()
    except Exception as e:
        warnings.warn(
            f"An error ocurred during acd_verify execution:\n{e}"
        )

    results = {
        "reportSaved": reportSaved,
        "check3strike": processor.getCheck3strikeResult()
    }
    return results


def main():
    try:
        parser = argparse.ArgumentParser(
            formatter_class=argparse.RawTextHelpFormatter
        )
        parser.add_argument("--verbose",
                            action = 'store_true',
                            help = "Verbose details of report")
        parser.add_argument("--path",
                            type = str,
                            help = "Path to where the json output file(s) are located")
        parser.add_argument("--compare",
                            type = str,
                            help = "Path to the json output file for comparison")
        parser.add_argument("--ignoreList",
                            action = 'store_true',
                            help = "Use ignore list")
        parser.add_argument("--check3strike",
                            action = 'store_true',
                            help = "Do checks for 3strike")
        parser.add_argument("--check_cpu",
                            type = str,
                            help = "Do Checks only in this CPU")
        parser.add_argument("--check_core",
                            type = str,
                            help = "Do Checks only in this core")
        parser.add_argument("--check_thread",
                            type = str,
                            help = "Do Checks only in this thread")
        parser.add_argument("--check_mca_mask_match",
                            type = str,
                            help = \
                            "Search for registers that match with the " +
                            "value specified.\n" +
                            "Format: [[mask0, match0], [mask1, match1]]")
        args = parser.parse_args()

        rootPath = ""

        if args.path:
            rootPath = args.path
            print("Path given: ", rootPath)
        if not args.path:
            rootPath = os.getcwd()

        fCompare = True if args.compare else False
        rCompare = args.compare if fCompare else ""

        checksScope = ""
        if args.check_cpu:
            checksScope += f"{args.check_cpu}."
        if args.check_core:
            checksScope += f"{args.check_core}."
        if args.check_thread:
            checksScope += f"{args.check_thread}."

        checks = {}
        if args.check3strike:
            checks["check3strike"] = None

        # list of lists of [mask, match]
        if args.check_mca_mask_match:
            checks["checkMCAValueMask"] =\
                ast.literal_eval(args.check_mca_mask_match)

        # Path given is a json file
        if os.path.isfile(rootPath) and rootPath.endswith('.json'):
            processJsonFile(rootPath, args.verbose, fCompare,
                            rCompare, args.ignoreList,
                            checks, checksScope)
        # Path given is a directory
        elif os.path.isdir(rootPath):
            jsonFiles = \
                [f for f in os.listdir(rootPath) if f.endswith('.json')]

            for jsonFile in jsonFiles:
                processJsonFile(os.path.join(rootPath, jsonFile), args.verbose,
                                fCompare, rCompare, args.ignoreList,
                                checks, checksScope)
        else:
            print("The path given is not a json file" +
                  f" nor a directory: {rootPath}")
            raise Exception(f"Invalid path given: {rootPath}")

    except Exception as e:
        print(f"An error ocurred during acd_verify execution")
        raise e


if __name__ == "__main__":
    main()
