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

import os
import warnings


class Postprocessor():
    def __init__(self, reportObj, filePath, fileType, verbose=False,
                 compareFileName=None):
        self.fileType = fileType
        baseFileName = os.path.splitext(os.path.basename(filePath))[0]
        self.fileName = f"{baseFileName}_report.{fileType}"
        baseFilePath = os.path.dirname(filePath)
        self.filePath = os.path.join(baseFilePath, self.fileName)
        self.report = reportObj
        self.verbose = verbose
        self.compareFileName = compareFileName

    def formatTXT(self):
        sReport = ""
        for region in self.report:
            if region == "summary":
                sReport = sReport + self.txtSummary(self.report[region])
            elif region == "table":
                sReport = sReport + "\n\n"+self.txtErrorsTable(self.report[region])
            elif region == "tableInfo":
                sReport = sReport + "\n" + \
                    self.txtTableDetails(self.report[region])
            elif region == "timeInfo":
                sReport = sReport + self.txtTimesTable(self.report[region])
            elif region == "selfCheck":
                sReport = sReport + "\n" + \
                    self.txtSelfCheck(self.report[region])
            elif region == "checks":
                sReport = sReport + \
                    self.txtChecks(self.report["checks"])
            elif region == "missCompares":
                sReport = sReport + "\n" + \
                    self.txtCompareInfo(self.report[region])
        try:
            f = open(self.filePath, "w")
            f.write(sReport)
            f.close()
            print(f"Report saved: {self.filePath}\n")
            return True
        except Exception as e:
            eMessage = "An error ocurred while trying to save " +\
                         "the file {self.filePath}"
            warnings.warn(eMessage)
            return False

    def saveFile(self):
        if self.fileType == "txt":
            return self.formatTXT()
        else:
            warnings.warn(
                f"File type not supported"
            )
            return False

    def txtSummary(self, summary):
        title = "Summary of File Contents:"
        metadataSummary = self.txtMetadataSummary(summary["metadata"])
        tableSummary = self.txtSummaryTable(summary)
        sSummary = title + metadataSummary + tableSummary

        return sSummary

    def txtMetadataSummary(self, metadata):
        nCPUs = "\n\tNumber of CPUs: {0}".format(metadata["nCPUs"])
        totalTime = "\n\t_total_time: {0}".format(metadata["totalTime"])
        triggerType = "\n\ttrigger_type: {0}".format(metadata["triggerType"])

        sSummary = nCPUs + totalTime + triggerType
        return sSummary

    def txtErrorsTable(self, table):
        tInfo = {}
        sTable = ""

        # Get the headers
        headers = self.getTableHeaders(table)
        for header in headers:
            tInfo[header] = []

        # Fill the table
        for key in table:
            # metadata
            if key == "metadata":
                keys = table[key].keys()
                for header in headers:
                    if header in keys:
                        tInfo[header].append(str(table[key][header]))
                    else:
                        tInfo[header].append("")
            # Socket
            if "cpu" in key:
                socket = table[key]
                for section in socket:
                    keys = socket[section].keys()
                    for header in headers:
                        if header in keys:
                            if header == "Section":
                                value = f"{key}:{socket[section][header]}"
                                tInfo[header].append(value)
                            else:
                                value = str(socket[section][header])
                                tInfo[header].append(value)
                        else:
                            tInfo[header].append("")

        columns = [tInfo[x] for x in tInfo]
        return self.txtTable(headers, columns)

    def txtTableDetails(self, errorList):
        regsList = "\t"

        if self.verbose:
            regsList += "All errors found"
            fullList = self.getFullErrorList(errorList)
            for reg in fullList:
                regsList = regsList + f"\n\t\t{reg}: {fullList[reg]}"
        elif not self.verbose:
            regsList += "First 3 errors of each record are:" +\
                        "    (use –-verbose for all)"
            for key in errorList:
                if key == "metadata":
                    count = 0
                    for reg in errorList[key]:
                        regValue = errorList[key][reg]
                        if regValue == 'N/A':
                            continue
                        regsList += f"\n\t\t{key}.{reg}: {regValue}"
                        if count == 2:
                            break
                        count += 1
                if "cpu" in key:
                    for section in errorList[key]:
                        count = 0
                        for reg in errorList[key][section]:
                            regValue = errorList[key][section][reg]
                            if regValue == 'N/A':
                                continue
                            regsList += f"\n\t\t{key}.{section}.{reg}:" + \
                                        f" {regValue}"
                            if count == 2:
                                break
                            count += 1

        return regsList

    def getFullErrorList(self, errorList):
        fullList = {}
        for key in errorList:
            if key == "metadata":
                for reg in errorList[key]:
                    regName = f"{key}.{reg}"
                    regValue = errorList[key][reg]
                    if regValue == 'N/A':
                        continue
                    fullList[regName] = regValue
            elif "cpu" in key:
                for section in errorList[key]:
                    for reg in errorList[key][section]:
                        regName = f"{key}.{section}.{reg}"
                        regValue = errorList[key][section][reg]
                        if regValue == 'N/A':
                            continue
                        fullList[regName] = regValue
        return fullList

    def getTableHeaders(self, table):
        headers = ["Section"]
        for key in table:
            # metadata
            if key == "metadata":
                keys = table[key].keys()
                for header in keys:
                    if header not in headers:
                        headers.append(header)
            # Socket
            if "cpu" in key:
                socket = table[key]
                for section in socket:
                    keys = socket[section].keys()
                    for header in keys:
                        if header not in headers:
                            headers.append(header)
        return headers

    def txtSelfCheck(self, errorList):
        sErrors = ""
        errors = 0

        for key in errorList:
            if key == "metadata":
                count = 3
                for error in errorList[key]:
                    if count and not self.verbose:
                        sErrors += f"\n\t\t{key}: {error}"
                        count -= 1
                        errors += 1
                    elif not count and not self.verbose:
                        break
                    else:
                        sErrors += f"\n\t\t{key}: {error}"
                        errors += 1
            if key.startswith("cpu"):
                for section in errorList[key]:
                    count = 3
                    for error in errorList[key][section]:
                        if count and not self.verbose:
                            sErrors += f"\n\t\t{key}.{section}: {error}"
                            count -= 1
                            errors += 1
                        elif not count and not self.verbose:
                            break
                        else:
                            sErrors += f"\n\t\t{key}.{section}: {error}"
                            errors += 1

        sTitle = "\nSelf-check:\n\tHealth check:"
        if errors == 0:
            sTitle += " Pass"
        else:
            sTitle += " Failed"

            if self.verbose:
                sErrors += "All errors found"
            else:
                sErrors += "First 3 errors of each record are:" +\
                            "    (use --verbose for all)"

        return sTitle + sErrors

    def txtCheck3Strike(self, checkList, fPass):
        status = "Pass" if fPass else "Failed"
        sErrors = f"\n\t3-Strike check: {status}"

        for error in checkList:
            sErrors += f"\n\t\t{error}"

        return sErrors

    def txtCheckMCAValueMask(self, checkList, fPass):
        status = "Found" if fPass else "Not found"
        sErrors = f"\n\tMCA regs that match the value specified: {status}"

        if not fPass:
            return sErrors

        regs = []
        vals = []
        masks = []
        matchs = []

        for cpu in checkList:
            regs += [f"{cpu}.{reg}" for reg in checkList[cpu] for occur in checkList[cpu][reg]]
            vals += [str(occur[0]) for reg in checkList[cpu] for occur in checkList[cpu][reg]]
            masks += [hex(occur[1]) for reg in checkList[cpu] for occur in checkList[cpu][reg]]
            matchs += [hex(occur[2]) for reg in checkList[cpu] for occur in checkList[cpu][reg]]

        if not self.verbose:
            regs = regs[:3]
            vals = vals[:3]
            masks = masks[:3]
            matchs = matchs[:3]

        columns = [regs, vals, masks, matchs]
        headers = ["Registers", "Value", "Mask", "Match"]

        sErrors = sErrors + "\n" + self.txtTable(headers, columns, 2)

        return sErrors

    def txtChecks(self, checks):
        sChecks = "\n"
        if "check3strike" in checks:
            sChecks += self.txtCheck3Strike(checks["check3strike"]["list"],
                                            checks["check3strike"]["pass"])
        if "checkMCAValueMask" in checks:
            sChecks += self.txtCheckMCAValueMask(checks["checkMCAValueMask"]["list"],
                                                 checks["checkMCAValueMask"]["pass"])
        return sChecks

    def txtCompareInfo(self, missCompares):
        sErrors = "\nCompare Data:\t"

        nDiffsTable, specialSections = self.txtNDiffsTable()
        if len(specialSections) > 0:
            sErrors += "\n Comments:\n  - Not comparing data for: " +\
                ', '.join([x for x in specialSections]) +\
                " at this time"
        sErrors += "\n\n" + nDiffsTable + "\n"
        if self.verbose:
            sErrors += "All errors found:\n\n"
        else:
            sErrors += "Showing first 3 errors of each record:" +\
                        "    (use –-verbose for all)\n\n"
        sErrors += self.txtDiffsTable(missCompares)

        return sErrors

    def txtTable(self, headers, columns, ident=0):
        identValid = (type(ident) == int) and (ident >= 0)
        if not identValid:
            warnings.warn(
                f"ident parameter of txtTable function has incorrect value {ident}"
            )
            ident = 0
        sTable = ""
        for column in range(len(columns)):
            # Add headers row to columns
            columns[column].insert(0, headers[column])

            # Get longest string in column
            maxWidth = len(max(columns[column], key=len))
            for i in range(len(columns[column])):
                element = columns[column][i]
                columns[column][i] = element.rjust(maxWidth + 1)

        nRows = len(columns[0])

        # Iterate through all columns in the same row
        #  and form the string for each row
        for row in range(nRows):
            sRow = '\t'*ident+'|'
            for column in range(len(columns)):
                sRow += columns[column][row]+'|'
            rowWidth = len(sRow) if (ident == 0) else (len(sRow) - ident)
            # Configure headers
            if row == 0:
                sRow = '\t'*ident + '='*(rowWidth) + '\n' + sRow + '\n' +\
                         '\t'*ident + '='*(rowWidth) + '\n'
            # Configure rest of the rows
            else:
                sRow += '\n' + '\t'*ident + '-'*(rowWidth) + '\n'
            # Append row
            sTable += sRow
        return sTable

    def txtDiffsTable(self, lDiffs):
        lDiff = {
            "Section": [],
            "fProcess": [],
            "fCompare": []
        }

        headers = ["Section", "fProcess", "fCompare"]

        for key in lDiffs:
            if key == "metadata":
                sectionInfo = lDiffs["metadata"]
                if self.verbose:
                    for reg in sectionInfo:
                        lDiff["Section"].append(f"{key}.{reg}")
                        lDiff["fProcess"].append(sectionInfo[reg][0])
                        lDiff["fCompare"].append(sectionInfo[reg][1])
                else:
                    sectionInfo = {k: sectionInfo[k] for k in list(sectionInfo)[:3]}
                    for reg in sectionInfo:
                        lDiff["Section"].append(f"{key}.{reg}")
                        lDiff["fProcess"].append(sectionInfo[reg][0])
                        lDiff["fCompare"].append(sectionInfo[reg][1])
            if key.startswith("cpu"):
                for section in lDiffs[key]:
                    sectionInfo = lDiffs[key][section]
                    if self.verbose:
                        for reg in sectionInfo:
                            lDiff["Section"].append(f"{key}.{section}.{reg}")
                            lDiff["fProcess"].append(sectionInfo[reg][0])
                            lDiff["fCompare"].append(sectionInfo[reg][1])
                    else:
                        sectionInfo = {k: sectionInfo[k] for k in list(sectionInfo)[:3]}
                        for reg in sectionInfo:
                            lDiff["Section"].append(f"{key}.{section}.{reg}")
                            lDiff["fProcess"].append(sectionInfo[reg][0])
                            lDiff["fCompare"].append(sectionInfo[reg][1])

        columns = [lDiff[x] for x in lDiff]

        # Names of files
        fProcessed = os.path.splitext(self.fileName)[0].rstrip("_report")

        tableHeaders = ["Item", fProcessed[:20], self.compareFileName[:20]]

        return self.txtTable(tableHeaders, columns)

    def txtNDiffsTable(self):
        lDiffs = (self.report["nDiffs"])
        lDiff = {
            "Section": [],
            "nDiffs": [],
        }

        foundSpecialSections = []

        for key in lDiffs:
            if key == "metadata":
                lDiff["Section"].append(key)
                lDiff["nDiffs"].append(str(lDiffs[key]))
            if key.startswith("cpu"):
                for section in lDiffs[key]:
                    lDiff["Section"].append(f"{key}.{section}")
                    lDiff["nDiffs"].append(str(lDiffs[key][section]))

        columns = [lDiff[x] for x in lDiff]
        headers = ["Section", "Differences"]

        return self.txtTable(headers, columns), self.report["specialCompSections"]

    def _getChops(self, summary):
        chops = {}
        for key in summary:
            if key.startswith('cpu'):
                chops[key] = None
                if 'uncore' in summary[key]:
                    if 'chop' in summary[key]['uncore']:
                        chops[key] = summary[key]['uncore']['chop']

        return chops

    def _getHeaders(self, headers, obj):
        for key in obj:
            if key not in headers:
                headers.append(key)
        return headers

    def txtSummaryTable(self, summary):
        chops = self._getChops(summary)

        headers = []
        headers = self._getHeaders(headers, chops)
        headers = self._getHeaders(headers, summary['metadata']['crashcoreCounts'])
        headers = self._getHeaders(headers, summary['metadata']['cpuIDs'])

        columns = [['CPUID', 'crashcoreCount', 'Chop']]
        for cpu in range(len(headers)):
            lCpu = []
            if f'cpu{cpu}' in summary['metadata']['cpuIDs']:
                lCpu.append(summary['metadata']['cpuIDs'][f'cpu{cpu}'])
            else:
                lCpu.append('Not present')
                warnings.warn(f'cpu{cpu} cpuid not in metadata')
            if f'cpu{cpu}' in summary['metadata']['crashcoreCounts']:
                lCpu.append(
                    summary['metadata']['crashcoreCounts'][f'cpu{cpu}'])
            else:
                lCpu.append('Not present')
                warnings.warn(f'cpu{cpu} crashcoreCounts not in metadata')
            lCpu.append(str(chops[f'cpu{cpu}']))
            columns.append(lCpu)

        headers.insert(0, 'Data')

        return '\n\n' + self.txtTable(headers, columns)

    def txtTimesTable(self, timeInfo):
        headers = ["Section", "Time Key", "Seconds"]
        columns = [[], [], []]
        for key in timeInfo:
            if key == 'metadata':
                for time in timeInfo[key]:
                    # Section
                    columns[0].append(str(key))
                    # Time Key
                    columns[1].append(str(time))
                    # Seconds
                    columns[2].append(str(timeInfo[key][time]))

            if key.startswith('cpu'):
                for section in timeInfo[key]:
                    for time in timeInfo[key][section]:
                        # Section
                        columns[0].append(f"{key}.{section}")
                        # Time Key
                        columns[1].append(str(time))
                        # Seconds
                        columns[2].append(str(timeInfo[key][section][time]))

        return '\n\n\nTime Table\n' + self.txtTable(headers, columns)
