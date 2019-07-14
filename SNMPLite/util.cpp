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
