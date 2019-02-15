//
// Created by Hao Xu on 2019-02-14.
//
#include "random.h"
#include <stdio.h>

#define NUM_VALUES 250

float values[NUM_VALUES];

int main() {
    float sum = 0;
    float average = 0;
    unsigned char i = 0;
    float max = 0, min = 0;

    init_random();

    for (i = 0; i < NUM_VALUES; ++i) {
        values[i] = random_float();
//        printf("i: %d, value[%d]: %d\n", i, i, value[i]);
        sum += values[i];
    }

    average = sum / NUM_VALUES;

    for (i = 0; i < NUM_VALUES; ++i) {
        values[i] -= average;
        min = (values[i] < min) ? values[i] : min;
        max = (values[i] > max) ? values[i] : max;
    }

    printf("Sum = %0.0f\n", sum);
    printf("Average = %0.0f\n", average);
    printf("Min = %0.0f\n", min);
    printf("Max = %0.0f\n", max);
};


