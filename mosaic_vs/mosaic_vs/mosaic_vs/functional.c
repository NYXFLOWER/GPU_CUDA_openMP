//
// Created by Hao Xu on 2019-03-07.
//

#include <ctype.h>
#include <omp.h>
#include "mosaic.h"

#define BUFFER_SIZE 100

int index_main(int cell, int row, Img * image, Mosaic* mos);

int index_column_edge(int cell, int row, Img * image, Mosaic * mos);

int index_row_edge(int cell, int row, Img * image, Mosaic * mos);

int index_end_case(int cell, int row, Img * image, Mosaic * mos);

void process_mosaic_section_cpu(Img * image, Mosaic * mos, int limits[3], Func fff, int pixcel_num, int * total);
void process_mosaic_section_openmp(Img * image, Mosaic * mos, int limits[3], Func fff, int pixcel_num, int * total);

/************************ Various Utility Functions ****************************/

/* print error message and exit */
void
error(char *message) {
	fprintf(stderr, "ppm: %s\n", message);
	exit(1);
}

/* give the index of data corresponding to the jth row of ith cell*/

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
	FILE *fp = fopen(img_path, "rb");
	if (!fp) error("Image file cannot be accessed");

	// alloc image
	Img *image = (Img *)malloc(sizeof(Img));
	if (!image) error("New image cannot be allocated on memory");

	// local variables
	unsigned int length;
	char buffer[BUFFER_SIZE];

	// get ppm type
	if (!fgets(buffer, BUFFER_SIZE, fp)) error("...");
	PPM ppm_type = (buffer[1] == '6') ? PPM_BINARY : PPM_PLAIN_TEXT;

	// get comment and other three features
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
		image->data = (unsigned char *)malloc(length);
		if (!image->data) error("data cannot be allocated on memory");

		if (fread((void *)image->data, 1, (size_t)length, fp) != length)
			error("cannot read image data from file");
		//            printf("\n");
		//
		//            for(int i = 0; i<100; i++)
		//                printf("%hhu ", image->data[i]);
		break;
	}
	case (PPM_PLAIN_TEXT): {
		int temp_int;
		int i = 0;
		unsigned char * temp_data = image->data = (unsigned char *)malloc(length);
		int data_index = 0;
		while (fscanf(fp, "%d", &temp_int) == 1) {
			temp_data[data_index++] = (unsigned char)temp_int;
			printf("%hhu %d \n", temp_data[data_index - 1], i++);
		}
		break;
	}
	}

	fclose(fp);
	return image;
}



/* read either p3 or p6, success will return 1 */
void
write_ppm_binary(Img * image, char * file_name) {
	FILE *fp = fopen(file_name, "wb");
	if (!fp) error("file cannot be created or opened");

	// write header
	fprintf(fp, "P6\n%d\n%d\n%d\n", image->width, image->height, image->color_value);

	// write pixel data
	unsigned int length = 3 * image->width * image->height;
	if (fwrite((void *)image->data, 1, (size_t)length, fp) != length)
		error("data cannot be written to file");
}

/************************ Mosaic Function ****************************/
Mosaic *
compute_mosaic_info(unsigned int cell_size, Img * image) {
	Mosaic * mos = (Mosaic *)malloc(sizeof(Mosaic));
	mos->cell_size = cell_size;

	// the numbers of mosaic cell in each row and column, as well as their remain pixels
	mos->cell_num_height = image->height / cell_size;
	mos->cell_remain_height = image->height % cell_size;
	mos->cell_num_weight = image->width / cell_size;
	mos->cell_remain_weight = image->width % cell_size;

	// calculate the total number of mosaic cell
	mos->cell_main_num = mos->cell_num_height * mos->cell_num_weight;
	mos->cell_num = mos->cell_main_num;
	if (mos->cell_remain_weight != 0) mos->cell_num += mos->cell_num_height;
	if (mos->cell_remain_height != 0) mos->cell_num += mos->cell_num_weight + 1;

	// calculate the number of pixel in each mosaic cell
	mos->pixcel_num = cell_size * cell_size;

	return mos;
}

void
run(Img * image, Mosaic * mos, Process ppp) {
	/****** variables declaraction ******/
	unsigned int * limits = (unsigned int) malloc(3);
	int pixcel_num;
	int * total = (int) malloc(3);
	total[0] = 0; total[1] = 0; total[2] = 0;

	

	/****** process main mosaic section ******/
	limits[0] = mos->cell_main_num;
	limits[1] = mos->cell_size;
	limits[2] = mos->cell_size;

	pixcel_num = mos->cell_size * mos->cell_size;
	ppp(image, mos, limits, index_main, pixcel_num, total);

	/****** process column-edge mosaic section ******/
	if (mos->cell_remain_weight != 0)
	{
		limits[0] = mos->cell_num_height;
		limits[1] = mos->cell_size;
		limits[2] = mos->cell_remain_weight;

		pixcel_num = mos->cell_size * mos->cell_remain_weight;

		ppp(image, mos, limits, &index_column_edge, pixcel_num, total);
	}

	/****** process row-edge mosaic section ******/
	if (mos->cell_remain_weight != 0)
	{
		limits[0] = mos->cell_num_weight;
		limits[1] = mos->cell_remain_height;
		limits[2] = mos->cell_size;

		pixcel_num = mos->cell_size * mos->cell_remain_height;
		ppp(image, mos, limits, &index_row_edge, pixcel_num, total);

		/****** process end-corner mosaic section ******/
		limits[0] = 1;
		limits[1] = mos->cell_remain_height;
		limits[2] = mos->cell_remain_weight;

		pixcel_num = mos->cell_remain_height * mos->cell_remain_weight;

		ppp(image, mos, limits, &index_end_case, pixcel_num, total);

	}

	// Output the average colour value for the image
	printf("CPU Average image colour red = %d, green = %d, blue = %d \n",
		total[0] / mos->cell_num, total[1] / mos->cell_num, total[2] / mos->cell_num);

	
}

void 
process_mosaic_section_cpu(Img * image, Mosaic * mos, int limits[3], Func fff, int pixcel_num, int * total) {
	/** limits[0]: the number of mosaic in this mosaic section 
		limits[1]: the number of pixcel rows in the current mosaic
		limits[2]: the number of pixcel columns in the current mosaic*/
	
	for (int i = 0; i < limits[0]; i++)	// the ith cell
	{
		/****** variables declaraction ******/
		unsigned char r, g, b;
		int r_sum = 0, g_sum = 0, b_sum = 0;
		int index;

		for (int j = 0; j < limits[1]; j++)	// the jth row of ith cell
		{
			// get the index in photo data that strar the jth row of the ith cell
			index = fff(i, j, image, mos);

			for (int k = 0; k < limits[2]; k++)	// the kth element of jth row
			{
				r_sum += (int) image->data[index + k * 3];
				g_sum += (int) image->data[index + k * 3 + 1];
				b_sum += (int) image->data[index + k * 3 + 2];
			}
		}

		// calculate the average
		r = (unsigned char)(r_sum / pixcel_num);
		g = (unsigned char)(g_sum / pixcel_num);
		b = (unsigned char)(b_sum / pixcel_num);

		// add to total
		total[0] += r; total[1] += g; total[2] += b;

		// mosaic the original
		for (int j = 0; j < limits[1]; ++j) {      // the jth row of ith cell
			// index in data that start the l row in ith cell
			index = index = fff(i, j, image, mos);

			for (int k = 0; k < limits[2]; ++k) {       // the kth element of jth row
				image->data[index + k * 3] = r;
				image->data[index + k * 3 + 1] = g;
				image->data[index + k * 3 + 2] = b;
			}
		}
		
	}
}

void
process_mosaic_section_openmp(Img * image, Mosaic * mos, int limits[3], Func fff, int pixcel_num, int * total) {
	/** limits[0]: the number of mosaic in this mosaic section
		limits[1]: the number of pixcel rows in the current mosaic
		limits[2]: the number of pixcel columns in the current mosaic*/

	for (int i = 0; i < limits[0]; i++)	// the ith cell
	{
		/****** variables declaraction ******/
		unsigned char r, g, b;
		int r_sum = 0, g_sum = 0, b_sum = 0;
		int index;

		for (int j = 0; j < limits[1]; j++)	// the jth row of ith cell
		{
			// get the index in photo data that strar the jth row of the ith cell
			index = fff(i, j, image, mos);

			for (int k = 0; k < limits[2]; k++)	// the kth element of jth row
			{
				r_sum += (int)image->data[index + k * 3];
				g_sum += (int)image->data[index + k * 3 + 1];
				b_sum += (int)image->data[index + k * 3 + 2];
			}
		}

		// calculate the average
		r = (unsigned char)(r_sum / pixcel_num);
		g = (unsigned char)(g_sum / pixcel_num);
		b = (unsigned char)(b_sum / pixcel_num);

		// add to total
		total[0] += r; total[1] += g; total[2] += b;

		// mosaic the original
		for (int j = 0; j < limits[1]; ++j) {      // the jth row of ith cell
			// index in data that start the l row in ith cell
			index = index = fff(i, j, image, mos);

			for (int k = 0; k < limits[2]; ++k) {       // the kth element of jth row
				image->data[index + k * 3] = r;
				image->data[index + k * 3 + 1] = g;
				image->data[index + k * 3 + 2] = b;
			}
		}

	}
}


int
index_main(int cell, int row, Img * image, Mosaic* mos) {
	return ((cell / mos->cell_num_weight * mos->cell_size + row) * image->width + (cell % mos->cell_num_weight) * mos->cell_size) * 3;
}

int
index_column_edge(int cell, int row, Img * image, Mosaic * mos) {
	return (cell * image->width * mos->cell_size + row * image->width + mos->cell_num_weight * mos->cell_size) * 3;
}

int
index_row_edge(int cell, int row, Img * image, Mosaic * mos) {
	return (image->width * (mos->cell_num_height * mos->cell_size + row) + cell * mos->cell_size) * 3;
}

int
index_end_case(int cell, int row, Img * image, Mosaic * mos) {
	return ((mos->cell_num_height * mos->cell_size + row) * image->width + mos->cell_num_weight * mos->cell_size) * 3;
}
