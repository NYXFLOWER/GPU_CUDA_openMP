//
// Created by Hao Xu on 2019-03-07.
//
#pragma once

#ifndef PPM_H
#define PPM_H

#include <sys/types.h>
#include <time.h>

typedef enum PPM_TYPE {
	PPM_BINARY, PPM_PLAIN_TEXT
} PPM;

typedef enum MODE {
	CPU, OPENMP, CUDA, ALL
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

typedef int(*func)(int, int, Img *, Mosaic*);

void error(char *message);
Img * read_ppm(const char * img_path);
void write_ppm_binary(Img * img_path, char * file_name);
Mosaic * compute_mosaic_info(unsigned int cell_size, Img * image);
void run_cpu(Img * image, Mosaic * mos);

#endif /* PPM_H */