//
//  SNMPFieldVarbind.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPFieldVarbind.h"
#include "DataType.h"

SNMPFieldVarbind::SNMPFieldVarbind(u_int8_t *raw, int packageLength)
{
    SNMPDataType dataType = (SNMPDataType)raw[kDataTypeIndex];
    u_int8_t varbindBodyLength = raw[kLengthIndex];
    u_int8_t *varbindRaw = (u_int8_t *)malloc(varbindBodyLength);
    memcpy(varbindRaw, raw+kBodyIndex, varbindBodyLength);
    
    if (dataType != SNMPDataTypeSequence)
    {
        this->isValid = false;
        return;
    }
    
    int index = 0;
    // Parse oid
    u_int8_t oidBodyLength = varbindRaw[kLengthIndex];
    u_int8_t oidRawLength = oidBodyLength + kLengthOfDataTypeAndLengthField;
    u_int8_t *oidRaw = (u_int8_t *)malloc(oidRawLength);
    memcpy(oidRaw, varbindRaw+index, oidRawLength);
    index+=oidRawLength;
    
    this->oid = new SNMPOID(oidRaw, oidRawLength);
    free(oidRaw);
    
    // Parse value
    u_int8_t valueBodyLength = varbindRaw[index+kLengthIndex];
    u_int8_t valueRawLength = valueBodyLength + kLengthOfDataTypeAndLengthField;
    u_int8_t *valueRaw = (u_int8_t *)malloc(valueRawLength);
    memcpy(valueRaw, varbindRaw+index, valueRawLength);
    index+=valueRawLength;
    
    this->value = new SNMPFieldValue(valueRaw, valueRawLength);
    free(valueRaw);
    free(varbindRaw);
}

SNMPFieldVarbind::~SNMPFieldVarbind()
{
    delete oid;
    delete value;
}

u_int8_t * SNMPFieldVarbind::rawValuePtr(int &length)
{
    u_int8_t buffer[1000];
    buffer[0] = SNMPDataTypeSequence;
    
    int oidLength;
    u_int8_t *oidRaw = this->oid->rawValuePtr(oidLength);
    
    int valueLength;
    u_int8_t *valueRaw = this->value->rawValuePtr(valueLength);
    
    memcpy(buffer+2, oidRaw, oidLength);
    memcpy(buffer+2+oidLength, valueRaw, valueLength);
    length = 2 + oidLength + valueLength;
    buffer[1] = oidLength + valueLength;
    
    free(oidRaw);
    free(valueRaw);
    
    return copyToHeap(buffer, length);
}
