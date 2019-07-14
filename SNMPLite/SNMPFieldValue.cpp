//
//  SNMPValue.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPFieldValue.h"
#include <string>

SNMPFieldValue::SNMPFieldValue(u_int8_t *raw, int packageLength)
{
    this->type = (SNMPDataType)raw[kDataTypeIndex];
    this->dataLength = raw[kLengthIndex];
    this->data = (u_int8_t *)malloc(this->dataLength);
    memcpy(this->data, raw+kBodyIndex, this->dataLength);
}

u_int8_t * SNMPFieldValue::rawValuePtr(int &length)
{
    u_int8_t buffer[1000];
    buffer[0] = this->type;
    memcpy(buffer+2, this->data, this->dataLength);
    length = this->dataLength + 2;
    buffer[1] = this->dataLength;
    
    return copyToHeap(buffer, length);
}

char * SNMPFieldValue::printableValue()
{
    char *s = (char *)malloc(this->dataLength + 1);
    memcpy(s, this->data, this->dataLength);
    s[this->dataLength] = '\0';
    
    return s;
}
