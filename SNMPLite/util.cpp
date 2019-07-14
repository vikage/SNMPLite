//
//  util.c
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#include "util.h"
#include <string.h>
#include <cstdlib>

u_int8_t * copyToHeap(u_int8_t *src, int length)
{
    u_int8_t *dst = (u_int8_t *) malloc(length);
    memcpy(dst, src, length);
    return dst;
}

void print_array(u_int8_t *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%.02x, ",array[i]);
    }
    
    printf("\n");
}

u_int8_t *encodeNumber(int number, int &length)
{
    if (number <= 127)
    {
        length = 1;
        u_int8_t *raw = (u_int8_t *)malloc(length);
        raw[0] = (u_int8_t)number;
        
        return raw;
    }
    else
    {
        length = 2;
        u_int8_t *raw = (u_int8_t *)malloc(length);
        
        u_int8_t byteHigh = number/128;
        byteHigh = byteHigh | 0b10000000;
        
        u_int8_t byteLow = number%128;
        
        raw[0] = byteHigh;
        raw[1] = byteLow;
        
        return raw;
    }
}

int decodeNumber(u_int8_t *raw, int &usedLength)
{
    if (raw[0] >> 7)
    {
        usedLength = 2;
        u_int8_t byteHigh = raw[0];
        u_int8_t byteLow = raw[1];
        
        return (byteHigh & 0b0111111) * 128 + byteLow;
    }
    else
    {
        usedLength = 1;
        return raw[0];
    }
}
