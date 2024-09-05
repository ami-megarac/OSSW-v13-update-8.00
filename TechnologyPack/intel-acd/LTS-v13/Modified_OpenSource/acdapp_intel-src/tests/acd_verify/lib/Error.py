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

import re


class Error():
    def __init__(self):
        self.errors = {}
        self.errorType = None
        self.typesOfErrors = {
            1: "UA:0x[0-9a-fA-F]+",
            2: "0x[0-9a-fA-F]+,CC:0x[0-9a-fA-F]+,RC:0x[0-9a-fA-F]+",
            3: "UA:0x[0-9a-fA-F]+,DF:0x[0-9a-fA-F]+"
        }

    def isError(self, errorValue):
        # Check for N/A
        if errorValue == "N/A":
            return True

        # Check for normal Errors
        # First error
        if self.errorType is None:
            for errorType in self.typesOfErrors:
                exp = re.match(self.typesOfErrors[errorType], errorValue)
                if (exp):
                    self.errorType = self.typesOfErrors[errorType]
                    return True  # First error
            return False    # Not an error yet
        else:
            exp = re.match(self.errorType, errorValue)
            if (exp):
                return True
            return False

    def extractError(self, errorValue):
        if errorValue == "N/A":
            return errorValue
        else:
            error = errorValue
            if self.errorType == self.typesOfErrors[2]:
                returnCode = int(errorValue.split(",")[2].split(':')[1], 16)
                if returnCode != 0:
                    error = errorValue
                else:
                    error = errorValue.split(",")[1]
            elif self.errorType == self.typesOfErrors[3]:
                error = errorValue.split(",")[0]
            return error
