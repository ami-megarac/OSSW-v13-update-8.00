/******************************************************************************
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2021 Intel Corporation.
 *
 * This software and the related documents are Intel copyrighted materials,
 * and your use of them is governed by the express license under which they
 * were provided to you ("License"). Unless the License provides otherwise,
 * you may not use, modify, copy, publish, distribute, disclose or transmit
 * this software or the related documents without Intel's prior written
 * permission.
 *
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 *
 ******************************************************************************/

#include "base64Encode.h"

char getEncodedValueFromTable(char bitsteam)
{
    char tmp= 0;
    //-----AMI-start-----
    short value = (short)(bitsteam & 0x00FF);
    //-----AMI-end-----
    if ((sizeof(encode_table) / sizeof(char)) - 1 < bitsteam)
    {
        return '=';
    }
    //-----AMI-start-----
    tmp = encode_table[value];
    return tmp;
    //-----AMI-end-----
}
//-----AMI-start-----
uint64_t base64Encode(uint8_t* src, uint16_t srcSize, char* encodedString)
//-----AMI-end-----
{
    uint8_t* text = src;
    uint8_t fragment = 0;
    uint8_t bitstream = 0;
    //-----AMI-start-----
    uint8_t* stringOut = (uint8_t*) encodedString;
    //-----AMI-end-----
    uint16_t size = srcSize;

    while (2 < size)
    {
        fragment = *text;
        bitstream = (fragment & 0xfc) >> 2;
        *stringOut = getEncodedValueFromTable(bitstream);
        stringOut++;
        bitstream = (fragment & 0x03) << 4;
        text++;
        fragment = *text;
        bitstream = bitstream | (fragment & 0xf0) >> 4;
        *stringOut = getEncodedValueFromTable(bitstream);
        stringOut++;
        bitstream = (fragment & 0x0f) << 2;
        text++;
        fragment = *text;
        bitstream = bitstream | (fragment & 0xc0) >> 6;
        *stringOut = getEncodedValueFromTable(bitstream);
        stringOut++;
        bitstream = fragment & (0x3f);
        *stringOut = getEncodedValueFromTable(bitstream);
        stringOut++;
        text++;
        size = size - 3;
    }

    if (0 != size)
    {
        if (1 == size)
        {
            fragment = *text;
            bitstream = (fragment & 0xfc) >> 2;
            *stringOut = getEncodedValueFromTable(bitstream);
            stringOut++;
            bitstream = (fragment & 0x03) << 4 | 0x00;
            *stringOut = getEncodedValueFromTable(bitstream);
            stringOut++;
            *stringOut = getEncodedValueFromTable(64);
            stringOut++;
            *stringOut = getEncodedValueFromTable(64);
            stringOut++;
            size--;
        }
        else if (2 == size)
        {
            fragment = *text;
            bitstream = (fragment & 0xfc) >> 2;
            *stringOut = getEncodedValueFromTable(bitstream);
            stringOut++;
            bitstream = (fragment & 0x03) << 4;
            text++;
            fragment = *text;
            bitstream = bitstream | (fragment & 0xf0) >> 4;
            *stringOut = getEncodedValueFromTable(bitstream);
            stringOut++;
            bitstream = (fragment & 0x0f) << 2 | 0x00;
            *stringOut = getEncodedValueFromTable(bitstream);
            stringOut++;
            *stringOut = getEncodedValueFromTable(64);
            stringOut++;
            size = size - 2;
        }
    }

    *stringOut = '\0';
    return 0;
}
