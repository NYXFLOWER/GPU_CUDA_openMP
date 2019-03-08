//
// Created by Hao Xu on 2019-03-07.
//

#ifndef PPM_H
#define PPM_H

#include <sys/types.h>

typedef enum PPM_TYPE { PPM_BINARY, PPM_PLAIN_TEXT } TYPE;

typedef struct Img
{
    unsigned int width;
    unsigned int height;
    unsigned int color_value;
    unsigned int num_pixel;

    unsigned char * r, * g, * b;
} Img;

//TYPE ppm_type = PPM_BINARY;

Img * read_ppm(const char * img_path);


#endif /* PPM_H */