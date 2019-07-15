//
//  SNMPValue.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPFieldValue.h"
#include <string>
#include <string.h>

using namespace std;

SNMPFieldValue::SNMPFieldValue(u_int8_t *raw, int packageLength)
{
    this->type = (SNMPDataType)raw[kDataTypeIndex];
    this->dataLength = raw[kLengthIndex];
    this->data = (u_int8_t *)malloc(this->dataLength);
    memcpy(this->data, raw+kBodyIndex, this->dataLength);
}

SNMPFieldValue::~SNMPFieldValue()
{
    delete data;
    this->dataLength = 0;
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
    if (this->type == SNMPDataTypeOctetString || this->type == SNMPDataTypeSequence)
    {
        char *s = (char *)malloc(this->dataLength + 1);
        memcpy(s, this->data, this->dataLength);
        s[this->dataLength] = '\0';
        
        return s;
    }
    
    if (this->type == SNMPDataTypeInteger)
    {
        int number = this->data[0];
        string s = std::to_string(number);
        
        return (char *)copyToHeap((u_int8_t *)s.c_str(), (int)s.length());
    }
    
    if (this->type == SNMPDataTypeIPAddress && this->dataLength == 4)
    {
        string ip = "";
        u_int8_t oct1 = this->data[0];
        u_int8_t oct2 = this->data[1];
        u_int8_t oct3 = this->data[2];
        u_int8_t oct4 = this->data[3];
        
        ip+=to_string(oct1);
        ip+=".";
        ip+=to_string(oct2);
        ip+=".";
        ip+=to_string(oct3);
        ip+=".";
        ip+=to_string(oct4);
        
        return (char *)copyToHeap((u_int8_t *)ip.c_str(), (int)ip.length());
    }
    
    char *s = (char *)malloc(this->dataLength + 1);
    memcpy(s, this->data, this->dataLength);
    s[this->dataLength] = '\0';
    
    return s;
}
