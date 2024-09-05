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

from lib.Region import Region

import warnings


class OutFileCompare(Region):
    def processRequest(self, request, report):
        if "compare" in request["regions"]:
            __specialCompSections = ["big_core", "pm_info", "tor"]
            missCompares = {}
            nDiffs = {}
            foundSpecialCompSections = []
            ignoreList = request["ignoreList"]

            for key in request["sections"]:
                if key == "metadata":
                    if key in request["compare"].sections:
                        metadataObj = request["sections"]["metadata"]
                        metadataCompObj = request["compare"].sections["metadata"]
                        if hasattr(metadataObj, 'getCompareInfo'):
                            missCompares["metadata"] = metadataObj.getCompareInfo(metadataCompObj)
                            nDiff = metadataObj.getDiffNum()
                            if nDiff > 0:
                                nDiffs["metadata"] = metadataObj.getDiffNum()
                    else:
                        warnings.warn(
                            f"{key} section not found in file processed")
                if key.startswith("cpu"):
                    # Check if CPU is in other file
                    if key in request["compare"].sections:
                        missCompares[key] = {}
                        nDiffs[key] = {}
                        # Go through each section from CPU
                        for section in request["sections"][key]:
                            sectInfo = request["sections"][key][section]
                            if section in __specialCompSections and sectInfo:
                                foundSpecialCompSections.append(section)

                            # Check section exists in file
                            if section in request["compare"].sections[key]:
                                sectionObj = request["sections"][key][section]
                                sectionCompObj = request["compare"].sections[key][section]
                                if hasattr(sectionObj, 'getCompareInfo'):
                                    compareInfoRes = sectionObj.getCompareInfo(
                                        sectionCompObj, ignoreList)
                                    missCompares[key][section] = compareInfoRes
                                    nDiff = sectionObj.getDiffNum()
                                    if nDiff > 0:
                                        nDiffs[key][section] = nDiff
                            else:
                                warnings.warn(
                                    f"{section} section not found in {key} file processed")
                    # CPU not in file processed
                    else:
                        warnings.warn(
                            f"{key} not found in file processed")
            report["missCompares"] = missCompares
            report["nDiffs"] = nDiffs
            report["specialCompSections"] = list(set(foundSpecialCompSections))

            return True
