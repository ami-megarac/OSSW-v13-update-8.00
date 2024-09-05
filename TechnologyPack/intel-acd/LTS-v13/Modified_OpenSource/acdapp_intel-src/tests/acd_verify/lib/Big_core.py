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


class Big_core(Section):
    def __init__(self, jOutput, checks=None):
        Section.__init__(self, jOutput, "big_core")
        self._mc3_status = 0
        self.nCores = 0

        self.ierr = False
        if checks:
            if "check3strike" in checks:
                self.ierr = True
                self.initCheckInfo("check3strike")

        self.verifySection()
        self.rootNodes = f"cores: {self.nCores}"

    @classmethod
    def createBig_core(cls, jOutput, checks):
        if "big_core" in jOutput:
            return cls(jOutput, checks)
        else:
            warnings.warn(
                f"Big_core section was not found in this file"
            )
            return None

    def search(self, key, value):
        valueIsValidType = ((type(value) == str) or (type(value) == bool) or
                            (type(value) == int))
        if type(value) == dict:
            for vKey in value:
                self.search(f"{key}.{vKey}", value[vKey])
        elif valueIsValidType:
            if type(value) != str:
                value = str(value)
            if not value.startswith('_'):
                self.nRegs += 1  # count regs
                lastKey = (key.split(".")[-1]) if ("." in key) else key
                valueIsError = self.eHandler.isError(value)
                if valueIsError:
                    error = self.eHandler.extractError(value)
                    self.eHandler.errors[key] = error
                else:
                    if lastKey == "size":
                        self.checkSize(key, value)
                    elif lastKey == "CORE_CR_APIC_BASE":
                        self.checkApicBase(key, value)
                if self.ierr:
                    if lastKey.endswith("_mc3_status"):
                        self.check3strike(key, value, valueIsError)

    def verifySection(self):
        for key in self.sectionInfo:
            if key.startswith("core"):
                self.nCores += 1
            elif not key.startswith('_'):
                warnings.warn(f"Key {key}, not expected in {self.sectionName}")
            self.search(key, self.sectionInfo[key])
        self.makeSelfCheck()

    def checkSize(self, key, value):
        # ICX
        _icxSize = (value.upper().startswith("0X05F8"))
        _icxSizeWithSq = (value.upper().startswith("0X020005F8"))
        icxValidSize = _icxSize or _icxSizeWithSq
        # SPR
        _sprSizeA = (value.upper().startswith("0X05A8"))
        _sprSizeAWithSq = (value.upper().startswith("0X030005A8"))
        _sprValidSizeA = _sprSizeA or _sprSizeAWithSq
        # SPR C0
        _sprSizeC0 = (value.upper().startswith("0X05B0"))
        _sprSizeC0WithSq = (value.upper().startswith("0X030005B0"))
        _sprValidSizeC0 = _sprSizeC0 or _sprSizeC0WithSq
        sprValidSize = _sprValidSizeA or _sprValidSizeC0

        if (not icxValidSize) and (not sprValidSize):
            sValue = f"{key} has invalid size {value}"
            self.healthCheckErrors["selfCheck"].append(sValue)

    def checkApicBase(self, key, value):
        isValidValA = value.upper().startswith("0XFEE00")
        isValidValB = value.upper().startswith("0XFFFFF")

        if (not isValidValA) and (not isValidValB):
            sValue = f"{key} has invalid value {value}"
            self.healthCheckErrors["selfCheck"].append(sValue)

    def check3strike(self, key, value, valueIsError):
        self._mc3_status += 1
        mask = 0x9000000000800400
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
        if ((self.ierr) and (self._mc3_status == 0)):
            sValue = f"0 *_mc3_status regs were found in Big_core"
            self.checks["check3strike"]["list"].append(sValue)
