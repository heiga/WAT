/*
 * jaltfixes.c
 *
 *  Created on: Mar 22, 2017
 *      Author: Randy Baron
 */

#include "jinclude.h"
#include "jpeglib.h"

/*For whatever reason multiplication causes a crash but addition does not */
EXTERN(long)
mult(long left, long right)
{
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
