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

#include "imagepro.h"

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
		regions[y].cols_traversed = 0;
		regions[y].rows_traversed = 0;
		regions[y].x = 0;
		regions[y].y = 0;
	}

	//Iterate over the image row by row and detect color regions
	y = 0;
	while (cinfo.output_scanline < height) {
		if(y == 100){
			endTime = clock();
		}
		region_found = FALSE;
		temp_row_count = 0;
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

		if (y == 101){
			printf("clock ticks=%i\n", clock() - endTime);
		}

		y++;
	}

	endTime = clock() - startTime;

	int row_mid = regions[0].x - (regions[0].rows_traversed / 2);
	int col_mid = regions[0].y - (regions[0].cols_traversed / 2);

	printf("height_end=%i width_end=%i\n", regions[0].y, regions[0].x);
	printf("height_mid=%i width_mid=%i\n", col_mid, row_mid);
	printf("cols=%i rows=%i\n", regions[0].cols_traversed, regions[0].rows_traversed);
	printf("detected=%i\n", regions[0].pixels_detected);
	printf("clock ticks=%i\n", endTime);

	(void) jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	printf("DONE\n");

	//TODO replace with constants
	if (regions[0].pixels_detected > 0){
		if ((regions[0].cols_traversed > 100) ||
			(regions[0].rows_traversed > 200) ||
			(regions[0].pixels_detected > 10000)){
			return 0xFFFF;
		}else{
			/* Compute distance away from mid point as a percentage
			 * conditional avoids needing to add math.h for one abs call
			 * Direction will be determined by bit shifting with the
			 * upper 8 bits indicating right, lower 4 bits indicating left
			 */

			temp_x = row_mid * 2;
			if (temp_x >= width){
				return (((temp_x) - width) * 100) / width;
			}else{
				temp_x = ((width - temp_x) * 100) / width;
				temp_x = temp_x << 8;
				return temp_x;
			}
		}
	}else{
		//Found nothing
		return 0xAAAA;
	}
}
