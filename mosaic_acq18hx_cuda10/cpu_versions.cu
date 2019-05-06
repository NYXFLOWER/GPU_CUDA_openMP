//
// Created by Hao Xu on 2019-03-07.
//
#include "mosaic_header.cuh"

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
			index = fff(i, j, image, mos);

			for (int k = 0; k < limits[2]; ++k) {       // the kth element of jth row
				image->data[index + k * 3] = r;
				image->data[index + k * 3 + 1] = g;
				image->data[index + k * 3 + 2] = b;
			}
		}
	}
}

void
process_mosaic_s