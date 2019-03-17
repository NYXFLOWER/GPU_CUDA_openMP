#include "mosaic.h"

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
int cell_size = 8;

int main(int argc, char *argv[]) {
	//========================================================
	if (process_command_line(argc, argv) == FAILURE)
		return 1;
	//========================================================



	// read input image file (either binary or plain text ppm)
	Img *image = read_ppm(in_file);

	Mosaic * mos = compute_mosaic_info(cell_size, image);

	// ------------- execute the mosaic filter based on the mode -------------
	switch (execution_mode) {
	case CPU: {
		/****** starting timing ******/
		clock_t timer = clock();

		run(image, mos, &process_mosaic_section_cpu);

		//end timing here
		double cost = (double)(clock() - timer) / CLOCKS_PER_SEC;
		printf("CPU mode execution time took %d s and %dms\n", (int)cost, (int)((cost - (int)cost) * 1000));
		break;
	}
	case OPENMP: {
		/****** starting timing ******/
		double time_begin = omp_get_wtime();

		run(image, mos, &process_mosaic_section_openmp);

		//end timing here
		double time_end = omp_get_wtime();
		double cost = (double)(time_end - time_begin) / CLOCKS_PER_SEC;
		printf("CPU mode execution time took %d s and %dms\n", (int)cost, (int)((cost - (int)cost) * 1000));

		break;
	}

	case (ALL): {
		/****** CPU ******/
		clock_t timer = clock();

		run(image, mos, &process_mosaic_section_cpu);

		double cost = (double)(clock() - timer) / CLOCKS_PER_SEC;
		printf("CPU mode execution time took %d s and %dms\n", (int)cost, (int)((cost - (int)cost) * 1000));

		/****** OPENMP ******/
		double time_begin = omp_get_wtime();

		run(image, mos, &process_mosaic_section_openmp);

		double time_end = omp_get_wtime();
		cost = (double)(time_end - time_begin) / CLOCKS_PER_SEC;
		printf("CPU mode execution time took %d s and %dms\n", (int)cost, (int)((cost - (int)cost) * 1000));

		
		break;
	}
	}


	//save the output image file (from last executed mode)
	write_ppm_binary(image, out_file);

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
	if (!strcmp(argv[2], "ALL")) { execution_mode = ALL; };
	//TODO: read in the input image name
	in_file = argv[4];
	//TODO: read in the output image name
	out_file = argv[6];
	//TODO: read in any optional part 3 arguments
	if (!strcmp(argv[8], "PPM_BINARY")) { write_type = PPM_BINARY; };
	if (!strcmp(argv[8], "PPM_PLAIN_TEXT")) { write_type = PPM_PLAIN_TEXT; };

	return SUCCESS;
}


