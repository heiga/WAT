/*
 * motor_test.h
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#ifndef MOTOR_TEST_H_
#define MOTOR_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "altera_avalon_pio_regs.h"
#include "includes.h"

void motor_test(void* pdata);
void sensor_interrupt(void * context);

/* Definition of Queue info */
#define SEM_INIT_VALUE 0
OS_EVENT *SENSOR_SEM;

#endif /* MOTOR_TEST_H_ */
