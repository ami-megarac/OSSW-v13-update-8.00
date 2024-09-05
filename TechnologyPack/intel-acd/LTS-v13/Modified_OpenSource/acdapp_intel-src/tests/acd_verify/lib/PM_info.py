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

from lib.Section import Section

import warnings
import json
import os


class PM_info(Section):
    def __init__(self, jOutput):
        Section.__init__(self, jOutput, "PM_info")
        self.nCores = 0
        self.verifySection()
        self.rootNodes = f"cores: {self.nCores}"

    @classmethod
    def createPM_info(cls, jOutput):
        if "PM_info" in jOutput:
            return cls(jOutput)
        else:
            warnings.warn(
                f"PM_info section was not found in this file"
            )
            return None

    def verifySection(self):
        for key in self.sectionInfo:
            if key.startswith("core"):
                self.nCores += 1
            self.search(key, self.sectionInfo[key])

        if self.nCores == 0:
            warnings.warn("No cores found in PM_info")

        self.makeSelfCheck()

    def getCompareInfo(self, compareSections, ignoreList=False):
        # Load Ignore list
        __location__ = os.path.realpath(
            os.path.join(os.getcwd(), os.path.dirname(__file__)))
        f = open(os.path.join(__location__, 'compareIgnore.json'))
        jCompare = json.load(f)
        self.ignoreList = jCompare[self.sectionName]

        # compareSections != None
        if compareSections is None:
            for key in self.sectionInfo:
                self.diffList[key] = ["", "Not present"]
        else:
            # sectionInfo is equal in both files
            if compareSections.sectionInfo == self.sectionInfo:
                pass
            # Something is diff
            else:
                for key in compareSections.sectionInfo:
                    # First check key(reg) exists in both files
                    if key not in self.sectionInfo:
                        # [file process, file compare]
                        self.diffList[key] = ["Not present", ""]
        return self.diffList
