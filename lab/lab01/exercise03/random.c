//
// Created by Hao Xu on 2019-02-15.
//
#include "random.h"

unsigned int rseed;

void init_random(){
    srand(rseed);
}

unsigned short random_ushort(){
    return rand();
}

unsigned int random_uint() {
    rseed = RANDOM_A * rseed + RANDOM_C;

    return rseed;
}

float random_float() {
    return (float) random_uint();
}