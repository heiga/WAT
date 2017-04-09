/*
 ============================================================================
 Name        : test.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

//http://stackoverflow.com/questions/694080/how-do-i-read-jpeg-and-png-pixels-in-c-on-linux

#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <time.h>
#include <math.h>
#include "imagepro.h"

int main(void) {
//	uint16_t width = 480;
//	uint16_t row_mid = 230;
//	uint16_t temp;
//	if ((row_mid * 2) >= width){
//		temp = (((row_mid * 2) - width) * 100) / width;
//	}else{
//		temp = ((width - (row_mid * 2)) * 100) / width;
//	}
//	printf("%i", temp);

	FILE * infile;        /* source file */

	if ((infile = fopen(JPEGNAME, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", JPEGNAME);
		return 0;
	}

	find_region(infile, REGION_RED);
//	find_region(infile, REGION_GRN);
//	find_region(infile, REGION_BLU);

	fclose(infile);

//	long q;
//	long z;
//	long k;
//
//	q = 0;
//	z = 0;
//	k = mult(q, z);
//	printf("%li expect 0\n", k);
//
//	q = 2;
//	z = 0;
//	k = mult(q, z);
//	printf("%liexpect 0\n", k);
//
//	q = 0;
//	z = 2;
//	k = mult(q, z);
//	printf("%li expect 0\n", k);
//
//	q = 2;
//	z = 2;
//	k = mult(q, z);
//	printf("%li expect 4\n", k);
//
//	q = -3;
//	z = 2;
//	k = mult(q, z);
//	printf("%li expect -6\n", k);
//
//	q = 3;
//	z = -2;
//	k = mult(q, z);
//	printf("%li expect -6\n", k);
//
//	q = -2;
//	z = -3;
//	k = mult(q, z);
//	printf("%li expect 6\n", k);
}

long mult(long left, long right){
	long result = 0;
	long big = 0;
	long sml = 0;
	long q = 0;
	boolean negative = FALSE;


	//Simple case of either term being 0
	if((left == 0) || (right == 0)){
		return 0;
	}

	//Determine sign and invert if needed to ensure absolute values
	if(left < 0){
		negative = TRUE;
		left = -left;
	}
	if(right < 0){
		right = -right;
		if(negative){
			negative = FALSE;
		}else{
			negative = TRUE;
		}
	}

	//Choose the least amount of iterations possible
	if (left > right){
		big = left;
		sml = right;
	}else{
		big = right;
		sml = left;
	}

	//Now "mutiply"
	for(q = 0; q < sml; q++){
		result += big;
	}

	//Invert if we had a (+ * -) situation
	if (negative){
		result = -result;
	}
	return result;
}

uint16_t find_region(FILE* picture, uint8_t region){
	uint8_t r;			//Red RGB value, between 0-255
	uint8_t g;			//Green RGB value, between 0-255
	uint8_t b;			//Blue RGB value, between 0-255

	uint16_t width;		//Pixel width of the loaded image
	uint16_t height;	//Pixel height of the loaded image
	uint16_t x;			//Current horizontal position
	uint16_t y;			//Current vertical position
	uint16_t temp_x_count;
	uint16_t temp_x;

	boolean region_found;

	struct color_region regions[REGION_COUNT];
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	uint32_t startTime;
	uint32_t endTime;

	JSAMPARRAY pJpegBuffer;       /* Output row buffer */
	int row_stride;       /* physical row width in output buffer */

	startTime = clock();

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, picture);
	(void) jpeg_read_header(&cinfo, TRUE);
	(void) jpeg_start_decompress(&cinfo);
	width = cinfo.output_width;
	height = cinfo.output_height;

	//Initialize... something. Clearly important
	row_stride = width * cinfo.output_components;
	pJpegBuffer = (*cinfo.mem->alloc_sarray)
	((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	//Initialize the regions struct array
	for (y=0; y < REGION_COUNT; y++){
		regions[y].confidence = 0;
		regions[y].pixels_detected = 0;
		regions[y].x_traversed = 0;
		regions[y].y_traversed = 0;
		regions[y].x = 0;
		regions[y].y = 0;
	}

	//Iterate over the image row by row and detect color regions
	y = 0;
	while (cinfo.output_scanline < height) {
		region_found = FALSE;
		temp_x_count = 0;
		temp_x = 0;
		(void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
		for (x = 0; x < width; x++) {
			r = pJpegBuffer[0][cinfo.output_components * x];
			if (cinfo.output_components > 2) {
				g = pJpegBuffer[0][cinfo.output_components * x + 1];
				b = pJpegBuffer[0][cinfo.output_components * x + 2];
			} else {
				g = r;
				b = r;
			}

			if (region == REGION_RED){
				if((r >= REDREG_DRK_MIN_RED) && (r <= REDREG_DRK_MAX_RED) &&
				   (g >= REDREG_DRK_MIN_GRN) && (g <= REDREG_DRK_MAX_GRN) &&
				   (b >= REDREG_DRK_MIN_BLU) && (b <= REDREG_DRK_MAX_BLU)){
					temp_x_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}else if((r >= REDREG_LGT_MIN_RED) && (r <= REDREG_LGT_MAX_RED) &&
						 (g >= REDREG_LGT_MIN_GRN) && (g <= REDREG_LGT_MAX_GRN) &&
						 (b >= REDREG_LGT_MIN_BLU) && (b <= REDREG_LGT_MAX_BLU)){
					temp_x_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}
			}else if (region == REGION_GRN){
				if((r >= GRNREG_DRK_MIN_RED) && (r <= GRNREG_DRK_MAX_RED) &&
				   (g >= GRNREG_DRK_MIN_GRN) && (g <= GRNREG_DRK_MAX_GRN) &&
				   (b >= GRNREG_DRK_MIN_BLU) && (b <= GRNREG_DRK_MAX_BLU)){
					temp_x_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}else if((r >= GRNREG_LGT_MIN_RED) && (r <= GRNREG_LGT_MAX_RED) &&
						 (g >= GRNREG_LGT_MIN_GRN) && (g <= GRNREG_LGT_MAX_GRN) &&
						 (b >= GRNREG_LGT_MIN_BLU) && (b <= GRNREG_LGT_MAX_BLU)){
					temp_x_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}
			}else if (region == REGION_BLU){
				if((r >= BLUREG_DRK_MIN_RED) && (r <= BLUREG_DRK_MAX_RED) &&
				   (g >= BLUREG_DRK_MIN_GRN) && (g <= BLUREG_DRK_MAX_GRN) &&
				   (b >= BLUREG_DRK_MIN_BLU) && (b <= BLUREG_DRK_MAX_BLU)){
					temp_x_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}else if((r >= BLUREG_LGT_MIN_RED) && (r <= BLUREG_LGT_MAX_RED) &&
						 (g >= BLUREG_LGT_MIN_GRN) && (g <= BLUREG_LGT_MAX_GRN) &&
						 (b >= BLUREG_LGT_MIN_BLU) && (b <= BLUREG_LGT_MAX_BLU)){
					temp_x_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}
			}
		}

		if (region_found){
			regions[0].y_traversed++;
			regions[0].y = y;
			if (temp_x_count > regions[0].x_traversed){
				regions[0].x_traversed = temp_x_count;
				regions[0].x = temp_x;
			}
		}

		y++;
	}

	endTime = clock() - startTime;

	int x_mid = regions[0].x - (regions[0].x_traversed / 2);
	int y_mid = regions[0].y - (regions[0].y_traversed / 2);

	printf("width_end=%i height_end=%i\n", regions[0].x, regions[0].y);
	printf("width_mid=%i height_mid=%i\n", x_mid, y_mid);
	printf("width_tot=%i height_tot=%i\n", regions[0].x_traversed, regions[0].y_traversed);
	printf("detected=%i\n", regions[0].pixels_detected);
	printf("clock ticks=%i\n", endTime);

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	//TODO replace with constants
	if (regions[0].pixels_detected > THRESHOLD_MINIMUM){
		if ((regions[0].x_traversed > THRESHOLD_WIDTH)  ||
			(regions[0].y_traversed > THRESHOLD_HEIGHT) ||
			(regions[0].pixels_detected > THRESHOLD_PIXELS)){
			printf("DONE\n");
			return 0xFFFF;
		}else{
			/* Compute distance away from mid point as a percentage
			 * conditional avoids needing to add math.h for one abs call
			 * Direction will be determined by bit shifting with the
			 * upper 8 bits indicating right, lower 4 bits indicating left
			 */

			temp_x = x_mid * 2;
			if (temp_x >= width){
				printf("RIGHT\n");
				return (((temp_x) - width) * 100) / width;
			}else{
				printf("LEFT\n");
				temp_x = ((width - temp_x) * 100) / width;
				temp_x = temp_x << 8;
				return temp_x;
			}
		}
	}else{
		//Found nothing
		printf("NONE\n");
		return 0xAAAA;
	}
}
