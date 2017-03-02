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
void button_interrupt(void * context);

/* Definition of Queue info */
#define SEM_INIT_VALUE 0
OS_EVENT *BUTTON_SEM;

#endif /* WIFI_TEST_H_ */
