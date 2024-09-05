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


class Table(Region):
    def processRequest(self, request, report):
        if "table" in request["regions"]:
            tableInfo = {}
            errorList = {}

            for key in request["sections"]:
                if key == "metadata":
                    metadataObj = request["sections"]["metadata"]
                    if hasattr(metadataObj, 'getTableInfo'):
                        tableInfo["metadata"] = metadataObj.getTableInfo()
                        errorList["metadata"] = metadataObj.getErrorList()

                if "cpu" in key:
                    tableInfo[key] = {}
                    errorList[key] = {}
                    for section in request["sections"][key]:
                        sectionObj = request["sections"][key][section]
                        if section == "mca":
                            if hasattr(sectionObj, 'getTableInfo'):
                                tables = sectionObj.getTableInfo()
                                errors = sectionObj.getErrorList()

                                tableInfo[key]["mca_core"] = tables[0]
                                tableInfo[key]["mca_uncore"] = tables[1]

                                errorList[key]["mca_core"] = errors[0]
                                errorList[key]["mca_uncore"] = errors[1]
                        elif section == "uncore":
                            if hasattr(sectionObj, 'getTableInfo'):
                                tables = sectionObj.getTableInfo()
                                if type(tables) == list:
                                    for bdf in tables:
                                        bdfName = bdf['Section']
                                        if 'regsWE' in bdf:
                                            bdf.pop('regsWE')
                                        tableInfo[key][bdfName] = bdf
                                errorList[key][section] = sectionObj.getErrorList()
                        else:
                            if hasattr(sectionObj, 'getTableInfo'):
                                tableInfo[key][section] = sectionObj.getTableInfo()
                                errorList[key][section] = sectionObj.getErrorList()

            report["table"] = tableInfo
            report["tableInfo"] = errorList

            return True
