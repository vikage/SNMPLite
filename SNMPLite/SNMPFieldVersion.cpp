//
//  SNMPVersion.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPFieldVersion.h"
#include "util.h"
#include "DataType.h"

SNMPFieldVersion::SNMPFieldVersion(u_int8_t *raw, int rawLength)
{
    __unused SNMPDataType dataType = (SNMPDataType)raw[kDataTypeIndex];
    __unused u_int8_t length = raw[kLengthIndex];
    this->version = (SNMPVersion)raw[2];
}

u_int8_t* SNMPFieldVersion::rawValuePtr(int &length)
{
    u_int8_t buffer[3];
    buffer[kDataTypeIndex] = SNMPDataTypeInteger;
    buffer[kLengthIndex] = 1;
    buffer[2] = this->version;
    
    length = 3;
    return copyToHeap(buffer, length);
}
