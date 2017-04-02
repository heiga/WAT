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

	FILE * infile;        /* source file */

	if ((infile = fopen(JPEGNAME, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", JPEGNAME);
		return 0;
	}
	printf("LOADED\n");

	find_region(infile, REGION_RED);
	//find_region(infile, REGION_GRN);
	//find_region(infile, REGION_BLU);

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
		uint16_t temp_row_count;
		uint16_t temp_x;

		boolean region_found;

		struct color_region regions[REGION_COUNT];
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;

		uint32_t startTime;
		uint32_t endTime;

		JSAMPARRAY pJpegBuffer;       /* Output row buffer */
		int row_stride;       /* physical row width in output buffer */

		printf("START\n");

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		printf("decompress\n");
		jpeg_stdio_src(&cinfo, picture);
		printf("load\n");
		(void) jpeg_read_header(&cinfo, TRUE);
		printf("header\n");
		(void) jpeg_start_decompress(&cinfo);
		printf("decompress\n");
		width = cinfo.output_width;
		height = cinfo.output_height;

		printf("INFO\n");

		//Initialize... something. Clearly important
		row_stride = width * cinfo.output_components;
		pJpegBuffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

		printf("BUFFER\n");

		//Initialize the regions struct array
		for (y=0; y < REGION_COUNT; y++){
			regions[y].confidence = 0;
			regions[y].pixels_detected = 0;
			regions[y].cols_traversed = 0;
			regions[y].rows_traversed = 0;
			regions[y].x = 0;
			regions[y].y = 0;
		}

		//Iterate over the image row by row and detect color regions
		startTime = clock();
		y = 0;
		while (cinfo.output_scanline < height) {
			region_found = FALSE;
			temp_row_count = 0;
			temp_x = 0;
			if(y == 100){
				endTime = clock();
				(void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
				printf("clock ticks=%li\n", endTime - clock());
			}else{
				(void) jpeg_read_scanlines(&cinfo, pJpegBuffer, 1);
			}
			for (x = 0; x < width; x++) {
				r = pJpegBuffer[0][cinfo.output_components * x];
				if (cinfo.output_components > 2) {
					g = pJpegBuffer[0][cinfo.output_components * x + 1];
					b = pJpegBuffer[0][cinfo.output_components * x + 2];
				} else {
					g = r;
					b = r;
				}

				if ((region == REGION_RED) && (r > REDREG_RED) &&
						(g < REDREG_GRN) && (b < REDREG_BLU)){
					temp_row_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}else if ((region == REGION_GRN) && (r < GRNREG_RED) &&
						(g > GRNREG_GRN) && (b < GRNREG_BLU)){
					temp_row_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}else if ((region == REGION_BLU) && (r < BLUREG_RED) &&
						(g < BLUREG_GRN) && (b > BLUREG_BLU)){
					temp_row_count++;
					temp_x = x;
					region_found = TRUE;
					regions[0].pixels_detected++;
				}
			}

			if (region_found){
				regions[0].cols_traversed++;
				regions[0].y = y;
				if (temp_row_count > regions[0].rows_traversed){
					regions[0].rows_traversed = temp_row_count;
					regions[0].x = temp_x;
				}
			}
			y++;
		}

		endTime = clock() - startTime;

		int row_mid = regions[0].x - (regions[0].rows_traversed / 2);
		int col_mid = regions[0].y - (regions[0].cols_traversed / 2);

		printf("height_end=%i width_end=%i\n", regions[0].y, regions[0].x);
		printf("height_mid=%i width_mid=%i\n", col_mid, row_mid);
		printf("cols=%i rows=%i\n", regions[0].cols_traversed, regions[0].rows_traversed);
		printf("confidence=%i\n", regions[0].confidence);
		printf("clock ticks=%li\n", endTime);

		(void) jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		printf("DONE\n");

		//TODO replace with constants
		if (regions[0].pixels_detected > 0){
			if (regions[0].pixels_detected > 50000){
				return 0xFFFF;
			}else{
				//+1 deals with fringe case of row_mid = 0
				return row_mid + 1;
			}
		}else{
			//Found nothing
			return 0;
		}
}
