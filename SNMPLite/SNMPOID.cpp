//
//  SNMPOID.cpp
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "SNMPOID.h"
#include "DataType.h"
#include "util.h"
#include <vector>
#include <string>
#include <string.h>

using namespace std;
vector<string> componentsWithSeparate(char *astring, char separateChar)
{
    vector<string> result;
    int length = (int)strlen(astring);
    string acomponent = "";
    for (int i = 0; i < length; i++)
    {
        if (astring[i] == separateChar)
        {
            if (acomponent.length() != 0)
            {
                result.push_back(acomponent);
                acomponent = "";
            }
            
            continue;
        }
        
        acomponent += astring[i];
    }
    
    if (acomponent.size() != 0)
    {
        result.push_back(acomponent);
    }
    
    return result;
}

SNMPOID::SNMPOID(u_int8_t *raw, int packageLength)
{
    SNMPDataType dataType = (SNMPDataType)raw[kDataTypeIndex];
    u_int8_t oidBodyLength = raw[kLengthIndex];
    u_int8_t *oidRaw = (u_int8_t *)malloc(oidBodyLength);
    memcpy(oidRaw, raw+kBodyIndex, oidBodyLength);
    
    int index = 0;
    if (oidRaw[0] != 0x2b || dataType != SNMPDataTypeOID)
    {
        this->isValid = false;
        return;
    }
    this->oidValue += "1.3";
    index++;
    
    while (index < oidBodyLength)
    {
        int usedLength;
        int number = decodeNumber(oidRaw+index, usedLength);
        index+=usedLength;
        
        this->oidValue += '.';
        this->oidValue += to_string(number);
    }
    
    free(oidRaw);
}

u_int8_t * SNMPOID::rawValuePtr(int &length)
{
    vector<string> components = componentsWithSeparate((char *)this->oidValue.c_str(), '.');
    u_int8_t buffer[500];
    buffer[0] = SNMPDataTypeOID;
    int index = 2;
    
    assert(components[0] == "1");
    assert(components[1] == "3");
    buffer[index++] = 0x2b;
    vector<string>::iterator it = components.begin();
    it++;
    it++;
    
    for (; it != components.end(); it++)
    {
        string val = *it;
        int integerValue = stoi(val);
        
        int numberEncodedLength;
        u_int8_t *numberRaw = encodeNumber(integerValue, numberEncodedLength);
        memcpy(buffer+index, numberRaw, numberEncodedLength);
        free(numberRaw);
        index+=numberEncodedLength;
    }
    
    u_int8_t *raw = (u_int8_t *)malloc(index);
    memcpy(raw, buffer, index);
    length = index;
    
    buffer[kLengthIndex] = length - 2;
    
    return copyToHeap(buffer, length);
}
