//
//  SNMPOID.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef SNMPOID_hpp
#define SNMPOID_hpp

#include <stdio.h>
#include <string>
using namespace std;

class SNMPOID {
public:
    string oidValue;
    bool isValid = true;
    
    SNMPOID(string oid)
    {
        this->oidValue = oid;
    }
    
    SNMPOID(u_int8_t *raw, int packageLength);
    
    u_int8_t * rawValuePtr(int &length);
};


#endif
