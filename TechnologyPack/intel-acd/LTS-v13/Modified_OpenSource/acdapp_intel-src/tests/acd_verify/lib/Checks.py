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


class Checks(Region):
    def getCheckInfo(self, sectionObj, sectionName, checkName):
        checkList = None
        checkPass = False
        sectionHasCheck = hasattr(sectionObj, checkName)
        if sectionHasCheck:
            checkList = sectionObj.checks[checkName]["list"]
            if type(checkList) == dict:
                checkList = {f"{sectionName}.{k}": v for k, v in checkList.items()}
                checkPass = sectionObj.checks[checkName]["pass"]
                return checkList, checkPass
            checkList = [f"{sectionName}.{error}" for error in checkList]
            checkPass = sectionObj.checks[checkName]["pass"]
        return checkList, checkPass

    def doCheck(self, checkName, sections):
        checkList = [] if (checkName != "checkMCAValueMask") else {}
        checkPass = False
        for key in sections:
            if key == "metadata":
                metadataObj = sections["metadata"]

                metaList, metaPass =\
                    self.getCheckInfo(metadataObj, "metadata", checkName)
                if metaList:
                    if type(checkList) == dict:
                        checkList.update(metaList)
                    elif type(checkList) == list:
                        checkList += metaList
                checkPass = metaPass or checkPass

            elif "cpu" in key:
                for section in sections[key]:
                    sectionObj = sections[key][section]
                    sectionList, sectionPass =\
                        self.getCheckInfo(sectionObj, f"{key}.{section}",
                                          checkName)
                    if sectionList:
                        if type(checkList) == dict:
                            checkList[key] = sectionList
                        elif type(checkList) == list:
                            checkList += sectionList
                    checkPass = sectionPass or checkPass

        return checkList, checkPass

    def processRequest(self, request, report):
        if "checks" in request["regions"]:
            sScope = request["checksScope"]
            scopes = sScope.split(".")
            errorList = {}
            for check in request["checks"]:
                checkList, checkPass = self.doCheck(check, request["sections"])
                errorList[check] = {}
                errorList[check]["list"] = checkList
                errorList[check]["pass"] = checkPass

            # Trimm according with scope
            if (len(scopes) > 1):
                for scope in scopes:
                    for check in errorList:
                        checkList = errorList[check]["list"]
                        if type(checkList) == dict:
                            errorList[check]["list"] = \
                                {err: v for err, v in checkList.items() if (scope in err)}
                        elif type(checkList) == list:
                            errorList[check]["list"] = \
                                [error for error in checkList if (scope in error)]

            report["checks"] = errorList
            return True
