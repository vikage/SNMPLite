//
//  SNMPFieldValue.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef SNMPFieldValue_hpp
#define SNMPFieldValue_hpp
#include <stdio.h>
#include "DataType.h"
#include "util.h"
class SNMPFieldValue {
    SNMPDataType type;
    u_int8_t *data;
    int dataLength;
    
public:
    
    SNMPFieldValue(SNMPDataType type, u_int8_t *data, int dataLength)
    {
        this->type = type;
        this->data = data;
        this->dataLength = dataLength;
    }
    
    SNMPFieldValue(u_int8_t *raw, int packageLength);
    u_int8_t * rawValuePtr(int &length);
    char * printableValue();
};

#endif
