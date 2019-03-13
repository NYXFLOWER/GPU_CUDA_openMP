#include <stdio.h>
#include <stdlib.h>
#include "mosaic.h"

#define FAILURE 0
#define SUCCESS !FAILURE

#define USER_NAME "acq18hx"        //replace with your user name

//========================================================
//void print_help();
//int process_command_line(int argc, char *argv[]);
//========================================================
typedef struct MosaicCell {
    unsigned char *data;
} Cell;

typedef enum MODE {
    CPU, OPENMP, CUDA, ALL
} MODE;
MODE execution_mode = CPU;

int main() {
//========================================================
//	if (process_command_line(argc, argv) == FAILURE)
//		return 1;
//========================================================
    // Sample Input Information
    unsigned int cell_size = 2;     // 2 ** n
//    execution_mode = CPU;
    char input[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/Sheffield16x16.ppm";
//    char input[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/SheffieldPlainText16x16.ppm";
//    char output[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/Sheffield_out.ppm";
    TYPE write_type = PPM_BINARY;

//	TODO: read input image file (either binary or plain text PPM)

    Img *image = read_ppm(input);
//	write_ppm_binary(image, "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/hhh.ppm");


// --------------------------------------------
//    unsigned char d[5 * 7 * 3];
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 7; ++j) {
//            d[(i * 7 + j) * 3] = (unsigned char) j; // where "val" is some value.
//            d[(i * 7 + j) * 3 + 1] = (unsigned char) j;
//            d[(i * 7 + j) * 3 + 2] = (unsigned char) j;
//        }
//    }
//
//    for (int i = 0; i < 5; i++) {
//        for (int j = 0; j < 7; ++j) {
//            printf("%hhu, %hhu, %hhu\t\t", d[(i * 7 + j) * 3], d[(i * 7 + j) * 3 + 1], d[(i * 7 + j) * 3 + 2]);
//        }
//        printf("\n");
//    }
//
//    Img im;
//    im.width = 7;
//    im.height = 5;
//    im.num_pixel = 35;
//    im.data = d;
//    Img *image = &im;
    // --------------------------------------------


//	//TODO: execute the mosaic filter based on the mode
//	switch (execution_mode){
//		case (CPU) : {
//			//TODO: starting timing here
//
    //TODO: calculate the average colour value

    // data
    unsigned char *p_data = image->data;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; ++j) {
            printf("%hhu, %hhu, %hhu\t\t", p_data[(i * 7 + j) * 3], p_data[(i * 7 + j) * 3 + 1], p_data[(i * 7 + j) * 3 + 2]);
        }
        printf("\n");
    }

    // cell size
    unsigned int cell_num_height = image->height / cell_size;
    unsigned int cell_remain_height = image->height % cell_size;

    unsigned int cell_num_weight = image->width / cell_size;
    unsigned int cell_remain_weight = image->width % cell_size;

    // temp r g b for each cell
    unsigned int num_main_call = cell_num_height * cell_num_weight;
    unsigned int num_in_cell = cell_size * cell_size;
    unsigned char r, g, b;

    // set pointer
    unsigned int index;     // the first index for each element start a row in cell
    unsigned int r_sum, g_sum, b_sum;

    // process the main section
    for (unsigned int i = 0; i < num_main_call; ++i) {      // the ith cell
        // initial the sum of r, g, b
        r_sum = 0;
        g_sum = 0;
        b_sum = 0;

        for (unsigned int j = 0; j < cell_size; ++j) {      // the jth row of ith cell
            // index in data that start the l row in ith cell
            index = ((i / cell_num_weight * cell_size + j) * image->width + (i % cell_num_weight) * cell_size) * 3;

            for (int k = 0; k < cell_size; ++k) {           // the kth element of jth row
                r_sum += p_data[index + k * 3];
                g_sum += p_data[index + k * 3 + 1];
                b_sum += p_data[index + k * 3 + 2];
            }
        }

        // calculate the average
        r = (unsigned char) (r_sum / num_in_cell);
        g = (unsigned char) (g_sum / num_in_cell);
        b = (unsigned char) (b_sum / num_in_cell);

//        printf("\n%hhu, %hhu, %hhu\n", r, g, b);

        // mosaic the original
        for (unsigned int j = 0; j < cell_size; ++j) {      // the jth row of ith cell
            // index in data that start the l row in ith cell
            index = ((i / cell_num_weight * cell_size + j) * image->width + (i % cell_num_weight) * cell_size) * 3;

            for (int k = 0; k < cell_size; ++k) {           // the kth element of jth row
                p_data[index + k * 3] = r;
                p_data[index + k * 3 + 1] = g;
                p_data[index + k * 3 + 2] = b;
            }
        }



//        count_line = i % cell_num_weight;
//        if (i % cell_num_weight == 0) {
//            count_line++;
//        }
//
//        index_main = 3 * i;
//        index_data = (i * cell_size + count_line * cell_remain_weight) * 3;
//
//        // calculate the sum of ith cell
//        for (int j = 0; j < cell_size; ++j) {
//            cell_index_row = index_data + 3 * image->width * j;
//            for (int k = 0; k < cell_size; ++k) {
//                r += p_data[cell_index_row++];
//                g += p_data[cell_index_row++];
//                b += p_data[cell_index_row];
//            }
//        }
//
//        // calculate the average of ith cell
//        main_cell[index_main++] = (unsigned char) (r / cell_number);
//        main_cell[index_main++] = (unsigned char) (g / cell_number);
//        main_cell[index_main] = (unsigned char) (b / cell_number);
//        printf("\n%hhu, %hhu, %hhu\n", main_cell[index_main - 2], main_cell[index_main - 1], main_cell[index_data]);
    }

    // edge case: column edge
    if (cell_remain_weight != 0) {
        for (int m = 0; m < cell_num_height; ++m) {
            r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            for (int n = 0; n < cell_size; ++n) {
                index = (m * image->width * cell_size + cell_num_weight * cell_size) * 3;
                for (int l = 0; l < cell_remain_weight; ++l) {
                    r_sum += p_data[index + l * 3];
                    g_sum += p_data[index + l * 3 + 1];
                    b_sum += p_data[index + l * 3 + 2];
                }
            }
            // calculate the average
            num_in_cell = cell_remain_weight * cell_size;
            r = (unsigned char) (r_sum / num_in_cell);
            g = (unsigned char) (g_sum / num_in_cell);
            b = (unsigned char) (b_sum / num_in_cell);

            for (int n = 0; n < cell_size; ++n) {
                index = (m * image->width * (cell_size + n) + cell_num_weight * cell_size) * 3;
                for (int q = 0; q < cell_remain_weight; ++q) {
                    p_data[index + q * 3] = r;
                    p_data[index + q * 3 + 1] = g;
                    p_data[index + q * 3 + 2] = b;
                }
            }
        }
    }

//     edge case: row edge
    if (cell_remain_height != 0) {
        for (int m = 0; m < cell_num_weight; ++m) {
            r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            for (int p = 0; p < cell_remain_height; ++p) {
                index = (image->width * (cell_num_height * cell_size + p) + m * cell_size) * 3;
                for (int n = 0; n < cell_size; ++n) {
                    r_sum += p_data[index + n * 3];
                    g_sum += p_data[index + n * 3 + 1];
                    b_sum += p_data[index + n * 3 + 2];
                }
            }

            // calculate the average
            num_in_cell = cell_size * cell_remain_height;
            r = (unsigned char) (r_sum / num_in_cell);
            g = (unsigned char) (g_sum / num_in_cell);
            b = (unsigned char) (b_sum / num_in_cell);

            for (int p = 0; p < cell_remain_height; ++p) {
                index = (image->width * (cell_num_height * cell_size + p) + m * cell_size) * 3;
                for (int n = 0; n < cell_size; ++n) {
                    p_data[index + n * 3] = r;
                    p_data[index + n * 3 + 1] = g;
                    p_data[index + n * 3 + 2] = b;
                }
            }
        }

        r_sum = 0;
        g_sum = 0;
        b_sum = 0;
        // spacial case of last mosaic cell
        for (int i = 0; i < cell_remain_height; ++i) {
            index = (cell_num_height * image->width + cell_num_weight * cell_size) * 3;
            for (int j = 0; j < cell_remain_weight; ++j) {
                r_sum += p_data[index + j * 3];
                g_sum += p_data[index + j * 3 + 1];
                b_sum += p_data[index + j * 3 + 2];
            }
            // calculate the average
            num_in_cell = cell_remain_height * cell_remain_weight;
            r = (unsigned char) (r_sum / num_in_cell);
            g = (unsigned char) (g_sum / num_in_cell);
            b = (unsigned char) (b_sum / num_in_cell);

            for (int n = 0; n < cell_remain_weight; ++n) {
                p_data[index + n * 3] = r;
                p_data[index + n * 3 + 1] = g;
                p_data[index + n * 3 + 2] = b;
            }
        }
    }

    printf("\n");
//    for (int l = 0; l < 35*3; ++l) {
//        printf("%hhu  ", p_data[l]);
//    }

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; ++j) {
            printf("%hhu, %hhu, %hhu\t\t", p_data[(i * 7 + j) * 3], p_data[(i * 7 + j) * 3 + 1], p_data[(i * 7 + j) * 3 + 2]);
        }
        printf("\n");
    }
//
//			// Output the average colour value for the image
//			printf("CPU Average image colour red = ???, green = ???, blue = ??? \n");
//
//			//TODO: end timing here
//			printf("CPU mode execution time took ??? s and ???ms\n");
//			break;
//		}
//		case (OPENMP) : {
//			//TODO: starting timing here
//
//			//TODO: calculate the average colour value
//
//			// Output the average colour value for the image
//			printf("OPENMP Average image colour red = ???, green = ???, blue = ??? \n");
//
//			//TODO: end timing here
//			printf("OPENMP mode execution time took ??? s and ?? ?ms\n");
//			break;
//		}
//		case (CUDA) : {
//			printf("CUDA Implementation not required for assignment part 1\n");
//			break;
//		}
//		case (ALL) : {
//			//TODO
//			break;
//		}
//	}

    //save the output image file (from last executed mode)
    image->data = p_data;
    write_ppm_binary(image, "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/hhh.ppm");

    return 0;
}
//========================================================
//void print_help(){
//	printf("mosaic_%s C M -i input_file -o output_file [options]\n", USER_NAME);
//
//	printf("where:\n");
//	printf("\tC              Is the mosaic cell size which should be any positive\n"
//		   "\t               power of 2 number \n");
//	printf("\tM              Is the mode with a value of either CPU, OPENMP, CUDA or\n"
//		   "\t               ALL. The mode specifies which version of the simulation\n"
//		   "\t               code should execute. ALL should execute each mode in\n"
//		   "\t               turn.\n");
//	printf("\t-i input_file  Specifies an input image file\n");
//	printf("\t-o output_file Specifies an output image file which will be used\n"
//		   "\t               to write the mosaic image\n");
//	printf("[options]:\n");
//	printf("\t-f ppm_format  PPM image output format either PPM_BINARY (default) or \n"
//		   "\t               PPM_PLAIN_TEXT\n ");
//}
//
//int process_command_line(int argc, char *argv[]){
//	if (argc < 7){
//		fprintf(stderr, "Error: Missing program arguments. Correct usage is...\n");
//		print_help();
//		return FAILURE;
//	}
//
//	//first argument is always the executable name
//
//	//read in the non optional command line arguments
//	c = (unsigned int)atoi(argv[1]);
//
//	//TODO: read in the mode
//
//	//TODO: read in the input image name
//
//	//TODO: read in the output image name
//
//	//TODO: read in any optional part 3 arguments
//
//	return SUCCESS;
//}