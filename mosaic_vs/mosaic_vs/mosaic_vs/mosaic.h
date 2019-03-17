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

typedef int(*Func)(int, int, Img *, Mosaic*);
typedef void(*Process)(Img *, Mosaic *, int *, Func, int, int *);

void error(char *message);
Img * read_ppm(const char * img_path);
void write_ppm_binary(Img * img_path, char * file_name);
Mosaic * compute_mosaic_info(unsigned int cell_size, Img * image);
void run(Img * image, Mosaic * mos, Process ppp);

void process_mosaic_section_cpu(Img * image, Mosaic * mos, int limits[3], Func fff, int pixcel_num, int * total);

#endif /* PPM_H */