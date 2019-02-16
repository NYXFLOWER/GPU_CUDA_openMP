//
// Created by Hao Xu on 2019-02-14.
//
#include "random.h"
#include <stdio.h>

#define NUM_VALUES 250

int values[NUM_VALUES];

int main() {
    unsigned int sum = 0, average = 0;
    unsigned char i = 0;
    int temp = 0, max = 0, min = 0;

    init_random();

    for (i = 0; i < NUM_VALUES; ++i) {
        temp = random_ushort();
        values[i] = temp;
//        printf("i: %d, value[%d]: %d\n", i, i, value[i]);
        sum += temp;
    }

    average = sum / NUM_VALUES;

    for (i = 0; i < NUM_VALUES; ++i) {
        values[i] -= average;
        min = (values[i] < min) ? values[i] : min;
        max = (values[i] > max) ? values[i] : max;
    }

    printf("Sum = %u\n", sum);
    printf("Average = %u\n", average);
    printf("Min = %d\n", min);
    printf("Max = %d\n", max);
};


