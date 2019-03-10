#include <stdio.h>
#include <stdlib.h>
#include "mosaic.h"

#define FAILURE 0
#define SUCCESS !FAILURE

#define USER_NAME "acq18hx"		//replace with your user name

//========================================================
//void print_help();
//int process_command_line(int argc, char *argv[]);
//========================================================
typedef struct MosaicCell{
    unsigned char * data;
} Cell;

typedef enum MODE { CPU, OPENMP, CUDA, ALL } MODE;
MODE execution_mode = CPU;

int main() {
//========================================================
//	if (process_command_line(argc, argv) == FAILURE)
//		return 1;
//========================================================
    // Sample Input Information
    unsigned int cell_size = 4;     // 2 ** n
//    execution_mode = CPU;
//    char input[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/Sheffield16x16.ppm";
    char input[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/SheffieldPlainText16x16.ppm";
    char output[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/Sheffield_out.ppm";
    TYPE write_type = PPM_BINARY;

	//TODO: read input image file (either binary or plain text PPM)

	Img * image = read_ppm(input);
//	write_ppm_binary(image, "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/hhh.ppm");

//	//TODO: execute the mosaic filter based on the mode
	switch (execution_mode){
		case (CPU) : {
//			//TODO: starting timing here
//
			//TODO: calculate the average colour value
            unsigned int cell_num_height = image->height / cell_size;
            unsigned int cell_remain_height = image->height % cell_size;

            unsigned int cell_num_weight = image->width / cell_size;
            unsigned int cell_remain_weight = image->width % cell_size;

            // construct r g b - list of cell pointer
            unsigned int num_main_call = cell_num_height * cell_num_weight;
            unsigned char * main_cell = (unsigned char *) malloc(sizeof(unsigned char) * num_main_call * 3);
            if (!main_cell) error("cannot be allocated on memory");

            // set pointer
            unsigned char * p_data = image->data;
            unsigned int count_line = 0;
            unsigned int cell_number = cell_size * cell_size;
            unsigned int index_main;
            unsigned int index_data;
            unsigned int cell_index_row;
            unsigned int r, g, b;

            for (unsigned int i = 0; i < num_main_call; ++i) {
                count_line = i % cell_num_weight;
                if (i % cell_num_weight == 0) {
                    count_line ++;
                }

                index_main = 3 * i;
                index_data = (i * cell_size + count_line * cell_remain_weight) * 3;
                r = 0;
                g = 0;
                b = 0;

                // calculate the sum of ith cell
                for (int j = 0; j < cell_size; ++j) {
                    cell_index_row = index_data + 3 * image->width * j;
                    for (int k = 0; k < cell_size; ++k) {
                        r += p_data[cell_index_row++];
                        g += p_data[cell_index_row++];
                        b += p_data[cell_index_row++];
                    }
                }

                // calculate the average of ith cell
                main_cell[index_main ++] = (unsigned char) (r / cell_number);
                main_cell[index_main ++] = (unsigned char) (g / cell_number);
                main_cell[index_main] = (unsigned char) (b / cell_number);
            }
//
//			// Output the average colour value for the image
//			printf("CPU Average image colour red = ???, green = ???, blue = ??? \n");
//
//			//TODO: end timing here
//			printf("CPU mode execution time took ??? s and ???ms\n");
//			break;
		}
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
	}

	//save the output image file (from last executed mode)

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