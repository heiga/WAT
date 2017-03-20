/*
 * imagepro.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Randy Baron
 */

#include <stdio.h>
#include <stdlib.h>
#include "jpeglib.h"
#include <time.h>
#include <math.h>
#include <inttypes.h>

int find_region(FILE* picture, uint8_t reg_r, uint8_t reg_g, uint8_t reg_b);

//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\sample50.jpg"
//static const char* JPEGNAME = "/mnt/host/test.jpg";
static const char* JPEGNAME = "/mnt/host/tiny.jpg";

#define REGION_COUNT 10

#define REDREG_RED 200
#define REDREG_BLU 20
#define REDREG_GRN 20

struct color_region {
	uint16_t confidence;
	uint16_t pixels_detected;
	uint16_t rows_traversed;
	uint16_t cols_traversed;
	uint16_t x;
	uint16_t y;
	uint16_t last_r;
	uint16_t last_g;
	uint16_t last_b;
} color_region;
