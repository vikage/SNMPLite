//
//  SNMPLiteManager.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/14/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^SNMPLiteManagerGetCompleteHandler)(NSDictionary *result);

@interface SNMPLiteManager : NSObject
+(instancetype) shared;
-(SNMPLiteManagerGetCompleteHandler) getCompleteHandlerForRequestId:(int)requestId;
-(void) removeCompleteHandlerForRequestId:(int)requestId;
-(void) getOid:(NSString *)oidString address:(NSString *)address retry:(int)retry timeout:(int)timeout completeHandler:(SNMPLiteManagerGetCompleteHandler)completeHandler;
@end
