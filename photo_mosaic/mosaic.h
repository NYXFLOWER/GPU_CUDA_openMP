//
// Created by Hao Xu on 2019-03-07.
//
#ifndef PPM_H
#define PPM_H

#include <sys/types.h>

typedef enum PPM_TYPE { PPM_BINARY, PPM_PLAIN_TEXT } PPM;

typedef struct Img
{
    unsigned int width;
    unsigned int height;
    unsigned int color_value;
    unsigned int num_pixel;

    unsigned char * data;
} Img;

void error(char *message);
Img * read_ppm(const char * img_path);
void write_ppm_binary(Img * img_path, char * file_name);


#endif /* PPM_H */