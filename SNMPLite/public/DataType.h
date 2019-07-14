//
//  DataType.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef DataType_h
#define DataType_h

typedef enum : u_int8_t
{
    SNMPDataTypeInteger = 0x02,
    SNMPDataTypeOctetString = 0x04,
    SNMPDataTypeNULL = 0x05,
    SNMPDataTypeOID = 0x06,
    SNMPDataTypeSequence = 0x30,
    SNMPDataTypeGetRequest = 0xa0,
    SNMPDataTypeGetResponse = 0xa2,
    SNMPDataTypeSetRequest = 0xa3,
    SNMPDataTypeNoSuchObject = 0x80,
    SNMPDataTypeNoSuchInstance = 0x81,
    
} SNMPDataType;

#define kDataTypeIndex 0
#define kLengthIndex 1
#define kBodyIndex 2
#define kLengthOfDataTypeAndLengthField 2

#endif /* DataType_h */
