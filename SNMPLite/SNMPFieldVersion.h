//
//  SNMPVersion.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef SNMPVersion_hpp
#define SNMPVersion_hpp
#include <stdio.h>
#include "SNMPVersions.h"
class SNMPFieldVersion {
    SNMPVersion version;
    
public:
    SNMPFieldVersion(SNMPVersion version)
    {
        this->version = version;
    }
    
    SNMPFieldVersion(u_int8_t *raw, int rawLength);
    
    u_int8_t * rawValuePtr(int &length);
};

#endif
