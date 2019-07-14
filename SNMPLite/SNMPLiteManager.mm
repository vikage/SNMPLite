//
//  SNMPLiteManager.m
//  SimpleSNMP
//
//  Created by thanhvu on 7/14/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#import "SNMPLiteManager.h"
#include "modules.h"

void SNMPCallbackHandler(SNMPMsg *request, SNMPMsg *response)
{
    SNMPLiteManagerGetCompleteHandler completeHandler = [[SNMPLiteManager shared] getCompleteHandlerForRequestId:request->pdu->requestId];
    
    if (completeHandler)
    {
        NSMutableDictionary *result = [[NSMutableDictionary alloc] init];
        if (response != NULL && response->pdu != NULL && response->isValid && response->pdu->isValid)
        {
            for (vector<SNMPFieldVarbind>::iterator i = response->pdu->varbinds.begin(); i != response->pdu->varbinds.end(); i++)
            {
                SNMPFieldVarbind varbind = *i;
                if (!varbind.isValid)
                {
                    continue;
                }
                
                SNMPOID *oid = varbind.oid;
                SNMPFieldValue *value = varbind.value;
                
                if (!oid->isValid)
                {
                    continue;
                }
                
                NSString *oidValue = [NSString stringWithUTF8String:oid->oidValue.c_str()];
                NSString *valueString = [NSString stringWithUTF8String:value->printableValue()];
                
                [result setObject:valueString forKey:oidValue];
            }
        }
        
        completeHandler(result);
    }
}

@interface SNMPLiteManager()
@property (nonatomic, strong) NSMutableDictionary *requestStorage;
@end

@implementation SNMPLiteManager

+(instancetype)shared
{
    static SNMPLiteManager *sharedInstance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[SNMPLiteManager alloc] init];
    });
    
    return sharedInstance;
}

-(instancetype)init
{
    self = [super init];
    self.requestStorage = [[NSMutableDictionary alloc] init];
    return self;
}


-(SNMPLiteManagerGetCompleteHandler) getCompleteHandlerForRequestId:(int)requestId
{
    return [self.requestStorage objectForKey:@(requestId)];
}

-(void)removeCompleteHandlerForRequestId:(int)requestId
{
    [self.requestStorage removeObjectForKey:@(requestId)];
}

-(void)getOid:(NSString *)oidString address:(NSString *)address retry:(int)retry timeout:(int)timeout completeHandler:(SNMPLiteManagerGetCompleteHandler)completeHandler
{
    SNMPOID *oid = new SNMPOID(oidString.UTF8String);
    SNMPFieldValue *value = new SNMPFieldValue(SNMPDataTypeNULL, NULL, 0);
    SNMPFieldVarbind *varbind = new SNMPFieldVarbind(oid, value);
    
    vector<SNMPFieldVarbind> varbinds;
    varbinds.push_back(*varbind);
    
    SNMPPDU *pdu = new SNMPPDU(SNMPDataTypeGetRequest, varbinds);
    SNMPMsg *msg = new SNMPMsg(SNMPVersionV2, pdu);
    
    [self.requestStorage setObject:completeHandler forKey:@(msg->pdu->requestId)];
    msg->send((char *)address.UTF8String, retry, timeout, SNMPCallbackHandler);
}
@end
