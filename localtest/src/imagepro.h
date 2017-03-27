/*
 * imagepro.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Randy Baron
 */

#include <inttypes.h>

uint16_t find_region(FILE* picture, uint8_t region);
long mult(long left, long right);

//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\sample50.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\BluEdgeTest.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\ExtremeAll.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\ExtremeRedExtremeBlu.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\FarGrnFarBluMedRed.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\FarRedNearEdgeGrn.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\OverlapAll.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\PhilRed.jpg"
#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\PhilSmallRedTinyGrn.jpg"

#define REGION_COUNT 10

#define REGION_RED 0
#define REGION_GRN 1
#define REGION_BLU 2

#define REDREG_RED 220
#define REDREG_GRN 90
#define REDREG_BLU 90

#define GRNREG_RED 100
#define GRNREG_GRN 110
#define GRNREG_BLU 40

#define BLUREG_RED 30
#define BLUREG_GRN 30
#define BLUREG_BLU 40

#define TURN_STRAIGHT    1
#define TURN_SHORT_RIGHT 2
#define TURN_LONG_RIGHT  3
#define TURN_SHORT_LEFT  4
#define TURN_LONG_LEFT   5

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
