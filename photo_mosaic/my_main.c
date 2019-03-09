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
skip_comment(FILE *fp) {
    int c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n');
        c = getc(fp);
    }
    ungetc(c, fp);
}

/* read either p3 or p6, the output pixel data is a list of unsigned char */
Img *
read_ppm(const char *img_path) {
    // open file
    FILE *fp = fopen(img_path, "r");
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
    TYPE ppm_type = (buffer[1] == '6') ? PPM_BINARY :PPM_PLAIN_TEXT;

    // get commend and other three features
    skip_comment(fp);
    if (!fscanf(fp, "%d\n", &image->width)) error("fail to scan width");
    skip_comment(fp);
    if (!fscanf(fp, "%d\n", &image->height)) error("fail to scan height");
    skip_comment(fp);
    if (!fscanf(fp, "%d\n", &image->color_value)) error("fail to scan color range");


    image->num_pixel = image->width * image->height;
    length = 3 * image->num_pixel;

    // get pixel data
    switch (ppm_type) {
        case (PPM_BINARY): {
            image->data = (unsigned char *) malloc(length);
            if (!image->data) error("data cannot be allocated on memory");

            if (fread((void *) image->data, 1, (size_t) length, fp) != length)
                error("cannot read image data from file");
//            printf("\n");
//
//            for(int i = 0; i<100; i++)
//                printf("%hhu ", image->data[i]);
        }
        case (PPM_PLAIN_TEXT): {
            int temp_int;
            unsigned char * temp_data = image->data = (unsigned char *) malloc(length);
            int data_index = 0;
            while (fscanf(fp, "%d", &temp_int) == 1) {
                temp_data[data_index++] = (unsigned char) temp_int;
//                printf("%hhu \n", temp_data[data_index-1]);
            }
        }
    }

    fclose(fp);
    return image;
}



/* read either p3 or p6, success will return 1 */
void
write_ppm_binary(Img * image, char * file_name) {
    FILE *fp = fopen(file_name, "w");
    if (!fp) error("file cannot be created or opened");

    // write header
    fprintf(fp, "P6\n%d\n%d\n%d\n", image->width, image->height, image->color_value);

    // write pixel data
    unsigned int length = 3 * image->width * image->height;
    if (fwrite((void *) image->data, 1, (size_t) length, fp) != length)
        error("data cannot be written to file");
}