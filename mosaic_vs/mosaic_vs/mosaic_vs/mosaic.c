#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include "mosaic.h"

#define FAILURE 0
#define SUCCESS !FAILURE

#define USER_NAME "acq18hx"        //replace with your user name

//========================================================
//void print_help();
//int process_command_line(int argc, char *argv[]);
//========================================================


MODE execution_mode = CPU;
PPM write_type = PPM_BINARY;

int main() {
	//========================================================
	//	if (process_command_line(argc, argv) == FAILURE)
	//		return 1;
	//========================================================

	unsigned int cell_size = 32;     // 2 ** n

	char input[] = "G:\\github\\GPU_CUDA_openMP\\photo_mosaic\\Dog2048x2048.ppm";
	//    char input[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/SheffieldPlainText16x16.ppm";
	//    char output[] = "/Users/nyxfer/Documents/GitHub/gpu/photo_mosaic/Sheffield_out.ppm";
	

	// read input image file (either binary or plain text ppm)

	Img *image = read_ppm(input);

	Mosaic * mos = compute_mosaic_info(cell_size, image);
	
	// ------------- execute the mosaic filter based on the mode -------------
	switch (execution_mode) {
	case CPU: {
		run_cpu(image, mos);

		break;
	}
	case OPENMP: {
		
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
