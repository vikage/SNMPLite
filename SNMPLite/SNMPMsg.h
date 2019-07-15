//
//  SNMPMsg.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef SNMPMsg_hpp
#define SNMPMsg_hpp

#include "SNMPFieldVersion.h"
#include "SNMPPDU.h"
#include "SNMPOID.h"

class SNMPMsg;
typedef void (*SNMPCallback)(SNMPMsg *request, SNMPMsg *response);
class SNMPMsg {
public:
    SNMPFieldVersion *version;
    string community = "public";
    SNMPPDU *pdu;
    bool isValid = true;
    
    SNMPMsg(SNMPVersion version, SNMPPDU *pdu)
    {
        this->version = new SNMPFieldVersion(version);
        this->pdu = pdu;
    }
    
    SNMPMsg(u_int8_t *raw, int packageLength);
    ~SNMPMsg();
    
    u_int8_t * rawValuePtr(int &length);
    void send(char* server, int maxTry, int timeout, SNMPCallback callback);

private:
    u_int8_t * doSend(char* server, int &receivedLength, int timeout);
    void trySend(char* server, int current, int maxTry, int timeout, SNMPCallback callback);
};

#endif
