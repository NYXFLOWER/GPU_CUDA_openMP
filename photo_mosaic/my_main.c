//
// Created by Hao Xu on 2019-03-07.
//

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <mach/boolean.h>
#include "mosaic.h"

#define BUFFER_SIZE 100

/************************ Various Utility Functions ****************************/

/* print error message and exit */
void
error(char *message) {
    fprintf(stderr, "ppm: %s\n", message);
    exit(1);
}

/************************ PPM I/O Functions ****************************/
/* check if this line is commend, skip all commend lines */
static void
skip_comment(FILE * fp) {
    int c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n');
        c = getc(fp);
    }
    ungetc(c, fp);
}

/* print error message and exit */
Img *
read_ppm(const char * img_path) {
    // open file
    FILE * fp = fopen(img_path, "r");
    if (!fp) error("Image file cannot be accessed");

    // alloc image
    Img *image = (Img *) malloc(sizeof(Img));
    if (!image) error("New image cannot be allocated on memory");

    // local variables
    unsigned int width, height, color_range;
    unsigned int length;
    char buffer[BUFFER_SIZE];

    // get ppm type
    if (!fgets(buffer, BUFFER_SIZE, fp)) error("...");
    TYPE ppm_type = buffer[1] == 6 ? PPM_BINARY : PPM_PLAIN_TEXT;

    // get commend and other three features
    skip_comment(fp);
    if (!fscanf(fp, "%d", &image->width)) error("fail to scan width");
    skip_comment(fp);
    if (!fscanf(fp, "%d", &image->height)) error("fail to scan height");
    skip_comment(fp);
    if (!fscanf(fp, "%d", &image->color_value)) error("fail to scan color range");

    // get pixel data
    image->num_pixel = image->width * image->height;
    length = 3 * image->num_pixel;

    switch (ppm_type) {
        case (PPM_BINARY): {
            image->r = (unsigned char *) malloc(image->num_pixel);
            image->g = (unsigned char *) malloc(image->num_pixel);
            image->b = (unsigned char *) malloc(image->num_pixel);

            for (int i = 0; i < image->num_pixel; i++) {
                fscanf(fp, "%hhu", &image->r[i]);
                fscanf(fp, "%hhu", &image->g[i]);
                fscanf(fp, "%hhu", &image->b[i]);
            }
        }
    }


    fclose(fp);
    return image;
}