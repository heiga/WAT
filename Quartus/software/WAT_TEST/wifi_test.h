/*
 * wifi_test.h
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#ifndef WIFI_TEST_H_
#define WIFI_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "altera_avalon_pio_regs.h"
#include "includes.h"

void wifi_test(void* pdata);

/* Definition of Queue info */
#define SEM_INIT_VALUE 0
#define Wifi_PACKAGE_LENGTH     1
#define MOTOR_CW                1
#define MOTOR_CCW               2
#define MOTOR_STOP              3


void* WifiBuffer[Wifi_PACKAGE_LENGTH];

#endif /* WIFI_TEST_H_ */
