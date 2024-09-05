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


class Address_map(Section):
    def __init__(self, jOutput):
        Section.__init__(self, jOutput, "address_map")

        self.verifySection()
        self.rootNodes = ""

    @classmethod
    def createAdress_map(cls, jOutput):
        if "address_map" in jOutput:
            return cls(jOutput)
        else:
            warnings.warn(
                f"Address_map section was not found in this file"
            )
            return None
