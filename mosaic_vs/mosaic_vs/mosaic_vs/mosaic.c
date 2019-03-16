#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "mosaic.h"

#define FAILURE 0
#define SUCCESS !FAILURE

#define USER_NAME "acq18hx"        //replace with your user name

//========================================================
//void print_help();
//int process_command_line(int argc, char *argv[]);
//========================================================


MODE execution_mode = OPENMP;
PPM write_type = PPM_BINARY;

unsigned int r_average_sum = 0, g_average_sum = 0, b_average_sum = 0;

int main() {
	//========================================================
	//	if (process_command_line(argc, argv) == FAILURE)
	//		return 1;
	//========================================================

	unsigned int cell_size = 64;     // 2 ** n

	char input[] = "G:\\github\\GPU_CUDA_openMP\\photo_mosaic\\Dog2048x2048.ppm";
	//    char input[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/SheffieldPlainText16x16.ppm";
	//    char output[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/Sheffield_out.ppm";
	

	//	todo: read input image file (either binary or plain text ppm)

	Img *image = read_ppm(input);


	// -------------------------- pro-process --------------------------------
	// the numbers of mosaic cell in each row and column, as well as their remain pixels
	unsigned int cell_num_height = image->height / cell_size;
	unsigned int cell_remain_height = image->height % cell_size;
	unsigned int cell_num_weight = image->width / cell_size;
	unsigned int cell_remain_weight = image->width % cell_size;

	// calculate the total number of mosaic cell
	int num_average = cell_num_height * cell_num_weight;
	if (cell_remain_weight != 0) num_average += cell_num_height;
	if (cell_remain_height != 0) num_average += cell_num_weight + 1;

	// temp average r g b for each cell
	unsigned int num_main_call = cell_num_height * cell_num_weight;
	unsigned int num_in_cell = cell_size * cell_size;
	unsigned char r, g, b;

	// set pointer
	unsigned int index = 0;     // the first index for each element start a row in cell
	unsigned int r_sum = 0, g_sum = 0, b_sum = 0;

	//	//TODO: execute the mosaic filter based on the mode
	switch (execution_mode) {
	case CPU: {
		//starting timing here
		clock_t timer = clock();

		// process the main section
		for (unsigned int i = 0; i < num_main_call; ++i) {      // the ith cell
			// initial the sum of r, g, b
			r_sum = 0;
			g_sum = 0;
			b_sum = 0;

			for (unsigned int j = 0; j < cell_size; ++j) {      // the jth row of ith cell
				// index in data that start the l row in ith cell
				index = ((i / cell_num_weight * cell_size + j) * image->width + (i % cell_num_weight) * cell_size) *
					3;

				for (unsigned int k = 0; k < cell_size; ++k) {           // the kth element of jth row
					r_sum += image->data[index + k * 3];
					g_sum += image->data[index + k * 3 + 1];
					b_sum += image->data[index + k * 3 + 2];
				}
			}

			// calculate the average
			r = (unsigned char)(r_sum / num_in_cell);
			g = (unsigned char)(g_sum / num_in_cell);
			b = (unsigned char)(b_sum / num_in_cell);

			// update average sum
			r_average_sum += r;
			g_average_sum += g;
			b_average_sum += b;


			// mosaic the original
			for (unsigned int j = 0; j < cell_size; ++j) {      // the jth row of ith cell
				// index in data that start the l row in ith cell
				index = ((i / cell_num_weight * cell_size + j) * image->width + (i % cell_num_weight) * cell_size) *
					3;

				for (unsigned int k = 0; k < cell_size; ++k) {           // the kth element of jth row
					image->data[index + k * 3] = r;
					image->data[index + k * 3 + 1] = g;
					image->data[index + k * 3 + 2] = b;
				}
			}
		}

		// edge case: column edge
		if (cell_remain_weight != 0) {
			for (unsigned int m = 0; m < cell_num_height; ++m) {
				r_sum = 0;
				g_sum = 0;
				b_sum = 0;
				for (unsigned int n = 0; n < cell_size; ++n) {
					index = (m * image->width * cell_size + n * image->width + cell_num_weight * cell_size) * 3;
					for (unsigned int l = 0; l < cell_remain_weight; ++l) {
						r_sum += image->data[index + l * 3];
						g_sum += image->data[index + l * 3 + 1];
						b_sum += image->data[index + l * 3 + 2];
					}
				}
				// calculate the average
				num_in_cell = cell_remain_weight * cell_size;
				r = (unsigned char)(r_sum / num_in_cell);
				g = (unsigned char)(g_sum / num_in_cell);
				b = (unsigned char)(b_sum / num_in_cell);

				for (unsigned int n = 0; n < cell_size; ++n) {
					index = (m * image->width * cell_size + n * image->width + cell_num_weight * cell_size) * 3;
					for (unsigned int q = 0; q < cell_remain_weight; ++q) {
						image->data[index + q * 3] = r;
						image->data[index + q * 3 + 1] = g;
						image->data[index + q * 3 + 2] = b;
					}
				}
			}
		}

		//     edge case: row edge
		if (cell_remain_height != 0) {
			for (unsigned int m = 0; m < cell_num_weight; ++m) {
				r_sum = 0;
				g_sum = 0;
				b_sum = 0;
				for (unsigned int p = 0; p < cell_remain_height; ++p) {
					index = (image->width * (cell_num_height * cell_size + p) + m * cell_size) * 3;
					for (unsigned int n = 0; n < cell_size; ++n) {
						r_sum += image->data[index + n * 3];
						g_sum += image->data[index + n * 3 + 1];
						b_sum += image->data[index + n * 3 + 2];
					}
				}

				// calculate the average
				num_in_cell = cell_size * cell_remain_height;
				r = (unsigned char)(r_sum / num_in_cell);
				g = (unsigned char)(g_sum / num_in_cell);
				b = (unsigned char)(b_sum / num_in_cell);

				for (unsigned int p = 0; p < cell_remain_height; ++p) {
					index = (image->width * (cell_num_height * cell_size + p) + m * cell_size) * 3;
					for (unsigned int n = 0; n < cell_size; ++n) {
						image->data[index + n * 3] = r;
						image->data[index + n * 3 + 1] = g;
						image->data[index + n * 3 + 2] = b;
					}
				}
			}


			r_sum = 0;
			g_sum = 0;
			b_sum = 0;
			// spacial case of last mosaic cell
			for (unsigned int i = 0; i < cell_remain_height; ++i) {
				index = ((cell_num_height * cell_size + i) * image->width + cell_num_weight * cell_size) * 3;
				for (unsigned int j = 0; j < cell_remain_weight; ++j) {
					r_sum += image->data[index + j * 3];
					g_sum += image->data[index + j * 3 + 1];
					b_sum += image->data[index + j * 3 + 2];
				}
			}
			// calculate the average
			num_in_cell = cell_remain_height * cell_remain_weight;
			r = (unsigned char)(r_sum / num_in_cell);
			g = (unsigned char)(g_sum / num_in_cell);
			b = (unsigned char)(b_sum / num_in_cell);

			for (unsigned int i = 0; i < cell_remain_height; ++i) {
				index = ((cell_num_height * cell_size + i) * image->width + cell_num_weight * cell_size) * 3;
				for (unsigned int j = 0; j < cell_remain_weight; ++j) {
					image->data[index + j * 3] = r;
					image->data[index + j * 3 + 1] = g;
					image->data[index + j * 3 + 2] = b;
				}
			}
		}

		// Output the average colour value for the image
		printf("CPU Average image colour red = %d, green = %d, blue = %d \n",
			r_average_sum / num_average, g_average_sum / num_average, b_average_sum / num_average);
		
		//end timing here
		clock_t timer_end = clock();
		printf("constant CLOCKS_PER_SEC is: %lf", (double)(timer_end - timer));
		double cost = (double)(timer_end - timer) / CLOCKS_PER_SEC;
		printf("constant CLOCKS_PER_SEC is: %d, time cost is: %lf secs", CLOCKS_PER_SEC, cost);

		printf("CPU mode execution time took ??? s and ???ms\n");
		break;
	}
	case OPENMP: {
		//TODO: starting timing here
		double time_begin = omp_get_wtime();
		// #pragma omp parallel for schedule(static, 1) private(r_sum, g_sum, b_sum) reduction(+:r_average_sum, +:g_average_sum, +:b_average_sum)
		printf("Hello!");

		//TODO: calculate the average colour value
		// process the main section
		// process the main section
#pragma omp parallel for private(r_sum, g_sum, b_sum, index, r, g, b, r_average_sum, g_average_sum, b_average_sum) reduction(+:r_average_sum, +:g_average_sum, +:b_average_sum) schedule(dynamic, 1)
		for (unsigned int i = 0; i < num_main_call; ++i) {      // the ith cell
			// initial the sum of r, g, b
			r_sum = 0;
			g_sum = 0;
			b_sum = 0;

			for (unsigned int j = 0; j < cell_size; ++j) {      // the jth row of ith cell
				// index in data that start the l row in ith cell
				index = ((i / cell_num_weight * cell_size + j) * image->width + (i % cell_num_weight) * cell_size) *
					3;

				for (unsigned int k = 0; k < cell_size; ++k) {           // the kth element of jth row
					r_sum += image->data[index + k * 3];
					g_sum += image->data[index + k * 3 + 1];
					b_sum += image->data[index + k * 3 + 2];
				}
			}

			// calculate the average
			r = (unsigned char)(r_sum / num_in_cell);
			g = (unsigned char)(g_sum / num_in_cell);
			b = (unsigned char)(b_sum / num_in_cell);

			// update average sum
			r_average_sum += r;
			g_average_sum += g;
			b_average_sum += b;


			// mosaic the original
			for (unsigned int j = 0; j < cell_size; ++j) {      // the jth row of ith cell
				// index in data that start the l row in ith cell
				index = ((i / cell_num_weight * cell_size + j) * image->width + (i % cell_num_weight) * cell_size) *
					3;

				for (unsigned int k = 0; k < cell_size; ++k) {           // the kth element of jth row
					image->data[index + k * 3] = r;
					image->data[index + k * 3 + 1] = g;
					image->data[index + k * 3 + 2] = b;
				}
			}
		}

//			// Output the average colour value for the image
		int num_average = cell_num_height * cell_num_weight;
		if (cell_remain_weight != 0) num_average += cell_num_height;
		if (cell_remain_height != 0) num_average += cell_num_weight + 1;
		printf("CPU Average image colour red = %d, green = %d, blue = %d \n",
			r_average_sum / num_average, g_average_sum / num_average, b_average_sum / num_average);

		//TODO: end timing here
		double time_end = omp_get_wtime();
		printf("constant CLOCKS_PER_SEC is: %lf\n", (double)(time_end - time_begin));
		//            printf("OPENMP mode execution time took ??? s and ?? ?ms\n");
		break;
	}

	case (CUDA): {
		printf("CUDA Implementation not required for assignment part 1\n");
		break;
	}

	case (ALL): {
		//TODOhttps://www.jetbrains.com/help/pycharm/configuring-remote-interpreters-via-ssh.html
		break;
	}
	}


	//save the output image file (from last executed mode)
	write_ppm_binary(image, "G:\\github\\GPU_CUDA_openMP\\photo_mosaic\\hhhhh\\h.ppm");

	return 0;
}
//========================================================
//void print_help() {
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
//}#include <stdio.h>
