//
//  SNMPPDU.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef SNMPPDU_hpp
#define SNMPPDU_hpp
#include <stdio.h>
#include "SNMPFieldVarbind.h"
#include <vector>

using namespace std;
class SNMPPDU {
public:
    uint16_t requestId;
    u_int8_t error = 0;
    u_int8_t errorIndex = 0;
    
    SNMPDataType dataType;
    vector<SNMPFieldVarbind> varbinds;
    bool isValid = true;
    SNMPPDU(SNMPDataType type, vector<SNMPFieldVarbind> varbinds)
    {
        this->dataType = type;
        this->varbinds = varbinds;
        this->changeRequestId();
    }
    
    SNMPPDU(u_int8_t *raw, int rawLength);
    
    void changeRequestId();
    u_int8_t * rawValuePtr(int &length);
};

#endif
