//
// Created by Hao Xu on 2019-03-07.
//
#include "mosaic_header.cuh"

#define FAILURE 0
#define SUCCESS !FAILURE

#define USER_NAME "acq18hx"        //replace with your user name

//========================================================
void print_help();
int process_command_line(int argc, char *argv[]);
//========================================================

MODE execution_mode = CPU;
PPM write_type = PPM_BINARY;
char * in_file;
char * out_file;
char out_name[100];
int cell_size = 8;

int main(int argc, char *argv[]) {
	//========================================================
	if (process_command_line(argc, argv) == FAILURE)
		return 1;
	//========================================================

	
	

	
	


	// ------------ read input image file (either binary or plain text ppm)
	// allocate memory
	Img * image = (Img *)malloc(sizeof(Img));
	if (!image) error("New image cannot be allocated on memory");
	Mosaic * mos = (Mosaic *)malloc(sizeof(Mosaic));

	// load image and get mosaic infomation
	read_ppm(in_file, image);
	compute_mosaic_info(cell_size, image, mos);

	// ------------- execute the mosaic filter based on the mode
	switch (execution_mode)
	{
	case CPU:
		break;
	case OPENMP:
		break;
	case GPU:
		//  in device
		Img * dev_image;
		Mosaic * dev_mos;

		// allocate GPU memory
		cudaMalloc((void **)&dev_image, sizeof(Img));
		break;
	case ALL:
		break;
	default:
		break;
	}
	

	// free image and data
	

	return 0;
}

//========================================================
void print_help() {
	printf("mosaic_%s C M -i input_file -o output_file [options]\n", USER_NAME);

	printf("where:\n");
	printf("\tC              Is the mosaic cell size which should be any positive\n"
		"\t               power of 2 number \n");
	printf("\tM              Is the mode with a value of either CPU, OPENMP or\n"
		"\t               ALL. The mode specifies which version of the simulation\n"
		"\t               code should execute. ALL should execute each mode in\n"
		"\t               turn.\n");
	printf("\t-i input_file  Specifies an input image file\n");
	printf("\t-o output_file Specifies an output image file which will be used\n"
		"\t               to write the mosaic image\n");
	printf("[options]:\n");
	printf("\t-f ppm_format  PPM image output format either PPM_BINARY (default) or \n"
		"\t               PPM_PLAIN_TEXT\n ");
}


int process_command_line(int argc, char *argv[]) {
	if (argc < 7) {
		fprintf(stderr, "Error: Missing program arguments. Correct usage is...\n");
		print_help();
		return FAILURE;
	}
	//first argument is always the executable name

	//read in the non optional command line arguments
	cell_size = (unsigned int)atoi(argv[1]);
	if (!is_exp_of_two(cell_size)) {
		error("mosaic size should be exp of 2");
		print_help();
	}


	if (!strcmp(argv[2], "CPU")) { execution_mode = CPU; };
	if (!strcmp(argv[2], "OPENMP")) { execution_mode = OPENMP; };
	if (!strcmp(argv[2], "GPU")) { execution_mode = GPU; };
	if (!strcmp(argv[2], "ALL")) { execution_mode = ALL; };
	//TODO: read in the input image name
	in_file = argv[4];
	//TODO: read in the output image name
	out_file = argv[6];
	if (argc > 7) {
		//TODO: read in any optional part 3 arguments
		if (!strcmp(argv[8], "PPM_BINARY")) { write_type = PPM_BINARY; };
		if (!strcmp(argv[8], "PPM_PLAIN_TEXT")) { write_type = PPM_PLAIN_TEXT; };
	}
	
	return SUCCESS;
}

