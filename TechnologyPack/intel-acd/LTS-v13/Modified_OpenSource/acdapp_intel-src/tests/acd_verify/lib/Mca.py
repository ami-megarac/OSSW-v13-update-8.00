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

import collections
import warnings
import json
import os


class Mca(Section):
    def __init__(self, jOutput, checks=None):
        Section.__init__(self, jOutput, "MCA")

        self.nCores = 0
        self.nRegsCores = 0
        self.nRegsUncore = 0

        self._cbo5_status = 0
        self.ierr = False
        self.checkValueMask = False
        if checks:
            if "check3strike" in checks:
                self.ierr = True
                self.initCheckInfo("check3strike")

            if "checkMCAValueMask" in checks:
                self.checkValueMask = True
                self.initCheckInfo("checkMCAValueMask")
                self.checks["checkMCAValueMask"]["values"] =\
                    checks["checkMCAValueMask"]

        self.verifySection()
        self.rootNodes = f"cores: {self.nCores}"

    @classmethod
    def createMCA(cls, jOutput, checks):
        if "MCA" in jOutput:
            return cls(jOutput, checks)
        else:
            warnings.warn(
                f"MCA section was not found in this file"
            )
            return None

    def searchCore(self, key, value):
        if type(value) == dict:
            for vKey in value:
                self.searchCore(f"{key}.{vKey}", value[vKey])
        elif (type(value) == str) and not value.startswith('_'):
            self.nRegsCores += 1
            valueIsError = self.eHandler.isError(value)
            if valueIsError:
                error = self.eHandler.extractError(value)
                if "core" not in self.eHandler.errors:
                    self.eHandler.errors["core"] = {}
                self.eHandler.errors["core"][key] = error
            self.checkMCAValueMask(key, value, valueIsError)

    def searchUncore(self, key, value):
        if type(value) == dict:
            for vKey in value:
                self.searchUncore(f"{key}.{vKey}", value[vKey])
        elif (type(value) == str) and not value.startswith('_'):
            self.nRegsUncore += 1
            valueIsError = self.eHandler.isError(value)
            if valueIsError:
                error = self.eHandler.extractError(value)
                if "uncore" not in self.eHandler.errors:
                    self.eHandler.errors["uncore"] = {}
                self.eHandler.errors["uncore"][key] = error

            if self.ierr:
                lastKey = (key.split(".")[-1]) if ("." in key) else key
                if lastKey == "cbo5_status":
                    self.check3strike(key, value, valueIsError)

            self.checkMCAValueMask(key, value, valueIsError)

    def verifySection(self):
        for key in self.sectionInfo:
            if key.startswith("core"):
                self.nCores += 1
                self.searchCore(key, self.sectionInfo[key])
            elif key.startswith("uncore"):
                self.searchUncore(key, self.sectionInfo[key])
            # Considering core and uncore as the valid sections
            #  for the MCA region of the output file
            elif not key.startswith('_'):
                warnings.warn(f"Key {key}, not expected in {self.sectionName}")
        self.makeSelfCheck()

    def getTableInfoCore(self):
        tableInfo = {
            "Section": "MCA_Core",
            "rootNodes": self.rootNodes,
            "regs": self.nRegsCores
        }
        if "core" in self.eHandler.errors:
            coreErrors = self.eHandler.errors["core"]
            level = dict(collections.Counter(coreErrors.values()))
            for error in level:
                if error not in tableInfo:
                    tableInfo[error] = level[error]
                elif error in tableInfo:
                    tableInfo[error] = tableInfo[error] + level[error]

        return tableInfo

    def getTableInfoUncore(self):
        tableInfo = {
            "Section": "MCA_Uncore",
            "rootNodes": "",
            "regs": self.nRegsUncore
        }
        if "uncore" in self.eHandler.errors:
            coreErrors = self.eHandler.errors["uncore"]
            level = dict(collections.Counter(coreErrors.values()))
            for error in level:
                if error not in tableInfo:
                    tableInfo[error] = level[error]
                elif error in tableInfo:
                    tableInfo[error] = tableInfo[error] + level[error]

        return tableInfo

    def getTableInfo(self):
        return [self.getTableInfoCore(), self.getTableInfoUncore()]

    def getErrorList(self):
        coreErrors = {}
        uncoreErrors = {}
        if "core" in self.eHandler.errors:
            coreErrors = self.eHandler.errors["core"]

        if "uncore" in self.eHandler.errors:
            uncoreErrors = self.eHandler.errors["uncore"]

        return [coreErrors, uncoreErrors]

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

    def check3strike(self, key, value, valueIsError):
        self._cbo5_status += 1
        mask = 0x90000000000c017a
        if valueIsError:
            sValue = f"{key}: {value}"
            self.checks["check3strike"]["list"].append(sValue)
            self.checks["check3strike"]["pass"] = \
                self.checks["check3strike"]["pass"] or False
        else:
            check3strikeA = self.checkRegValueVsMask(key, value, mask,
                                                     "check3strike")
            self.checks["check3strike"]["pass"] = \
                self.checks["check3strike"]["pass"] or check3strikeA

    def checkMCAValueMask(self, key, value, valueIsError):
        if self.checkValueMask:
            lPairs = self.checks["checkMCAValueMask"]["values"]
            keyAndValValid = key.endswith('_status') and\
                not valueIsError
            if keyAndValValid:
                for pair in lPairs:
                    # pair = [mask, match]
                    valueMatch =\
                        self.__checkIndividualValueMask(key, value,
                                                        pair[0], pair[1])

                self.checks["checkMCAValueMask"]["pass"] = \
                    self.checks["checkMCAValueMask"]["pass"] or valueMatch

    def __checkIndividualValueMask(self, key, value, mask, match):
        try:
            intVal = int(value, 16)
        except ValueError as e:
            warnings.warn(
                f"register {key} has an unnexpected value"
            )
            return False
        valMatches = (intVal & mask) == (mask & match)

        if valMatches:
            # First time in here
            if (type(self.checks["checkMCAValueMask"]["list"]) == list):
                self.checks["checkMCAValueMask"]["list"] = {}

            if key not in self.checks["checkMCAValueMask"]["list"]:
                self.checks["checkMCAValueMask"]["list"][key] = []
                self.checks["checkMCAValueMask"]["list"][key].append(
                    [value, mask, match])
            else:
                self.checks["checkMCAValueMask"]["list"][key].append(
                    [value, mask, match])
            return True
        return False

    def makeSelfCheck(self):
        # Version Check
        if "_version" in self.sectionInfo:
            version = self.sectionInfo["_version"]
            if not self.eHandler.isError(version):
                self.checkVersion(version)
        else:
            errMessage = "_version key not present in " +\
                         self.sectionName
            self.healthCheckErrors["selfCheck"].append(errMessage)
            if self.ierr:
                self.checks["check3strike"]["list"].append(errMessage)

        if ((self.ierr) and (self._cbo5_status == 0)):
            sValue = f"Reg cbo5_status was not found in MCA"
            self.checks["check3strike"]["list"].append(sValue)
