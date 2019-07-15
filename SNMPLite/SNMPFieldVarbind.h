//
//  SNMPFieldVarbind.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef SNMPFieldVarbind_hpp
#define SNMPFieldVarbind_hpp

#include <stdio.h>
#include "SNMPOID.h"
#include "SNMPFieldValue.h"
#include "util.h"

class SNMPFieldVarbind {
public:
    SNMPOID *oid;
    SNMPFieldValue *value;
    bool isValid = true;
    
    SNMPFieldVarbind(SNMPOID *oid, SNMPFieldValue *value)
    {
        printf("Varbind alloc %p\n", this);
        this->oid = oid;
        this->value = value;
    }
    ~SNMPFieldVarbind();
    
    SNMPFieldVarbind(u_int8_t *raw, int packageLength);
    u_int8_t * rawValuePtr(int &length);
};

#endif
