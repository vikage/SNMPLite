//
//  SNMPPDU.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPPDU.h"
#include "util.h"
#include "DataType.h"
SNMPPDU::SNMPPDU(u_int8_t *raw, int rawLength)
{
    this->dataType = (SNMPDataType)raw[kDataTypeIndex];
    u_int8_t pduBodyLength = raw[kLengthIndex];
    u_int8_t *pduBodyRaw = (u_int8_t *)malloc(pduBodyLength);
    memcpy(pduBodyRaw, raw+kBodyIndex, pduBodyLength);
    
    int index = 0;
    // Parse requestId
    SNMPDataType requestIdDataType = (SNMPDataType)pduBodyRaw[index++];
    u_int8_t requestIdLength = pduBodyRaw[index++];
    u_int8_t *requestIdRaw = (u_int8_t *)malloc(requestIdLength);
    memcpy(requestIdRaw, pduBodyRaw+index, requestIdLength);
    index+=requestIdLength;
    if (requestIdLength != 2 || requestIdDataType != SNMPDataTypeInteger)
    {
        this->isValid = false;
        return;
    }
    
    this->requestId = (requestIdRaw[0] << 8) + requestIdRaw[1];
    free(requestIdRaw);
    
    // Parse error
    SNMPDataType errorDataType = (SNMPDataType)pduBodyRaw[index++];
    u_int8_t errorLength = pduBodyRaw[index++];
    u_int8_t *errorRaw = (u_int8_t *)malloc(errorLength);
    memcpy(errorRaw, pduBodyRaw+index, errorLength);
    index+=errorLength;
    
    if (errorLength != 1 || errorDataType != SNMPDataTypeInteger)
    {
        this->isValid = false;
    }
    
    this->error = errorRaw[0];
    free(errorRaw);
    
    // Parse error index
    SNMPDataType errorIndexDataType = (SNMPDataType)pduBodyRaw[index++];
    u_int8_t errorIndexLength = pduBodyRaw[index++];
    u_int8_t *errorIndexRaw = (u_int8_t *)malloc(errorIndexLength);
    memcpy(errorIndexRaw, pduBodyRaw+index, errorIndexLength);
    index+=errorIndexLength;
    
    if (errorIndexLength != 1 || errorIndexDataType != SNMPDataTypeInteger)
    {
        this->isValid = false;
    }
    
    this->errorIndex = errorRaw[0];
    free(errorIndexRaw);
    
    // Parse varbinds
    SNMPDataType varbindsDataType = (SNMPDataType)pduBodyRaw[index++];
    u_int8_t varbindsLength = pduBodyRaw[index++];
    u_int8_t *varbindsRaw = (u_int8_t *)malloc(varbindsLength);
    memcpy(varbindsRaw, pduBodyRaw+index, varbindsLength);
    index+=varbindsLength;
    
    if (varbindsDataType != SNMPDataTypeSequence || varbindsLength == 0)
    {
        this->isValid = false;
    }
    
    u_int8_t varbindsIndex = 0;
    while (varbindsIndex < varbindsLength - 1)
    {
        u_int8_t varbindBodyLength = varbindsRaw[varbindsIndex+kLengthIndex];
        u_int8_t varbindRawLength = varbindBodyLength + kLengthOfDataTypeAndLengthField;
        u_int8_t *varbindRaw = (u_int8_t *)malloc(varbindRawLength);
        memcpy(varbindRaw, varbindsRaw, varbindRawLength);
        varbindsIndex+=varbindRawLength;
        
        SNMPFieldVarbind *varbind = new SNMPFieldVarbind(varbindRaw, varbindRawLength);
        this->varbinds.push_back(varbind);
        free(varbindRaw);
    }
    
    free(varbindsRaw);
    free(pduBodyRaw);
}

SNMPPDU::~SNMPPDU()
{
    for ( auto varbind : varbinds )
        delete varbind;
    varbinds.clear();
}

u_int8_t * SNMPPDU::rawValuePtr(int &length)
{
    u_int8_t buffer[1000];
    buffer[kDataTypeIndex] = this->dataType;
    
    int currentIndex = kLengthIndex + 1;
    u_int8_t requestIndexByteHigh = requestId >> 8;
    u_int8_t requestIndexByteLow = (u_int8_t)requestId;
    
    // RequestID
    buffer[currentIndex++] = SNMPDataTypeInteger;
    buffer[currentIndex++] = 2;
    buffer[currentIndex++] = requestIndexByteHigh;
    buffer[currentIndex++] = requestIndexByteLow;
    
    // Error
    buffer[currentIndex++] = SNMPDataTypeInteger;
    buffer[currentIndex++] = 1;
    buffer[currentIndex++] = this->error;
    
    // ErrorIndex
    buffer[currentIndex++] = SNMPDataTypeInteger;
    buffer[currentIndex++] = 1;
    buffer[currentIndex++] = this->errorIndex;
    
    // Varbinds
    buffer[currentIndex++] = SNMPDataTypeSequence;
    int varbindFieldLengthIndex = currentIndex;
    currentIndex++; // Increase index for length
    
    int varbindsLength = 0;
    for (vector<SNMPFieldVarbind *>::iterator i = this->varbinds.begin(); i != this->varbinds.end(); i++)
    {
        SNMPFieldVarbind *varbind = *i;
        int varbindRawLength = 0;
        u_int8_t *raw = varbind->rawValuePtr(varbindRawLength);
        
        memcpy(buffer+currentIndex, raw, varbindRawLength);
        varbindsLength+=varbindRawLength;
        currentIndex+=varbindRawLength;
        free(raw);
    }
    
    buffer[varbindFieldLengthIndex] = varbindsLength;
    length = currentIndex;
    buffer[kLengthIndex] = currentIndex - 2;
    return copyToHeap(buffer, length);
}

void SNMPPDU::changeRequestId()
{
    this->requestId = arc4random()%65536;
}
