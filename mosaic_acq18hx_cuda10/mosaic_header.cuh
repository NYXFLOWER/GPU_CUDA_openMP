//
// Created by Hao Xu on 2019-03-07.
//
#pragma once

#ifndef PPM_H
#define PPM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <omp.h>

/** *********** Type Define *********** **/
typedef enum PPM_TYPE {
	PPM_BINARY, PPM_PLAIN_TEXT
} PPM;

typedef enum MODE {
	CPU, OPENMP, GPU, ALL
} MODE;

typedef struct Img {
	unsigned int width;
	unsigned int height;
	unsigned int color_value;
	unsigned int num_pixel;

	unsigned char * data;
} Img;

typedef struct Mosaic {
	unsigned int cell_size, cell_num, cell_main_num;
	unsigned int cell_num_height, cell_remain_height;
	unsigned int cell_num_weight, cell_remain_weight;
	unsigned int pixcel_num;
} Mosaic;


/** *********** Utility Functions *********** **/
void error(char *message);
int is_exp_of_two(unsigned int x);


/** *********** Pre-Process Data *********** **/
void read_ppm(const char * img_path, Img * image);
void write_ppm_binary(Img * img_path, char * file_name);

void compute_mosaic_info(unsigned int cell_size, Img * image, Mosaic * mosic);


/** *********** Implementation *********** **/

#endif /* PPM_H */