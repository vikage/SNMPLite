//
//  util.h
//  SimpleSNMP
//
//  Created by thanhvu on 7/13/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#ifndef util_hpp
#define util_hpp
#include <stdio.h>

extern u_int8_t* copyToHeap(u_int8_t *src, int length);

void print_array(u_int8_t *array, int length);

#endif
