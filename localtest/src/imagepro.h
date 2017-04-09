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
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\PhilSmallRedTinyGrn.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\PhilPlacingToolbox.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn1.jpg" //SHOULD BE NONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn2.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn3.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn4.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn5.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn6.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn7.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\grn8.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu1.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu2.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu3.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu4.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu5.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu6.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu7.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu8.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu9.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\blu10.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\red1.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\red2.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\red3.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\red4.jpg" //SHOULD NOT BE DONE
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\red5.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\RedDone.jpg"
//#define JPEGNAME "C:\\Users\\Randy Baron\\Pictures\\test\\BenchTheFirst.jpg"

#define REGION_COUNT 10

#define REGION_RED 0
#define REGION_GRN 1
#define REGION_BLU 2

#define REDREG_DRK_MIN_RED 200
#define REDREG_DRK_MAX_RED 255
#define REDREG_DRK_MIN_GRN 0
#define REDREG_DRK_MAX_GRN 60
#define REDREG_DRK_MIN_BLU 0
#define REDREG_DRK_MAX_BLU 80

#define REDREG_LGT_MIN_RED 200
#define REDREG_LGT_MAX_RED 255
#define REDREG_LGT_MIN_GRN 150
#define REDREG_LGT_MAX_GRN 200
#define REDREG_LGT_MIN_BLU 150
#define REDREG_LGT_MAX_BLU 200

#define GRNREG_DRK_MIN_RED 40
#define GRNREG_DRK_MAX_RED 100
#define GRNREG_DRK_MIN_GRN 100
#define GRNREG_DRK_MAX_GRN 180
#define GRNREG_DRK_MIN_BLU 0
#define GRNREG_DRK_MAX_BLU 40

#define GRNREG_LGT_MIN_RED 100
#define GRNREG_LGT_MAX_RED 200
#define GRNREG_LGT_MIN_GRN 175
#define GRNREG_LGT_MAX_GRN 255
#define GRNREG_LGT_MIN_BLU 0
#define GRNREG_LGT_MAX_BLU 40

#define BLUREG_DRK_MIN_RED 0
#define BLUREG_DRK_MAX_RED 40
#define BLUREG_DRK_MIN_GRN 0
#define BLUREG_DRK_MAX_GRN 40
#define BLUREG_DRK_MIN_BLU 40
#define BLUREG_DRK_MAX_BLU 80

#define BLUREG_LGT_MIN_RED 40
#define BLUREG_LGT_MAX_RED 100
#define BLUREG_LGT_MIN_GRN 80
#define BLUREG_LGT_MAX_GRN 160
#define BLUREG_LGT_MIN_BLU 150
#define BLUREG_LGT_MAX_BLU 255

#define THRESHOLD_MINIMUM 	200
#define THRESHOLD_WIDTH  	200
#define THRESHOLD_HEIGHT 	180
#define THRESHOLD_PIXELS 	10000

struct color_region {
	uint16_t confidence;
	uint16_t pixels_detected;
	uint16_t x_traversed;
	uint16_t y_traversed;
	uint16_t x;
	uint16_t y;
	uint16_t last_r;
	uint16_t last_g;
	uint16_t last_b;
} color_region;
