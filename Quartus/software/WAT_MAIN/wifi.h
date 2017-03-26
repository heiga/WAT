/*
 * wifi_test.h
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#ifndef WIFI_TEST_H_
#define WIFI_TEST_H_

#include <stdlib.h>
#include <inttypes.h>

#include "includes.h"
#include "altera_avalon_uart_regs.h"
#include "wat_main.h"

void wifi_task(void* pdata);
void wifi_uart_interrupt(void * context);

// RC Mode definitions
#define RCMODE		0b11000000
#define STOP		0b00000000
#define	FORWARD		0b00000001
#define REVERSE		0b00000010
#define LEFT		0b00000011
#define RIGHT		0b00000100
#define i1			0b00001000
#define i2			0b00010000
#define i3			0b00011000

#endif /* WIFI_TEST_H_ */
