/*
 * interrupt_peripherals.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Randy Baron
 */

#include "motor.h"
#include "interrupt_peripherals.h"

void sensor_interrupt(void * context){
	//printf("sensor\n");

	motorEStop();
	//Moved to motor.c motorEstop()
	//IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);

	//clear interrupt
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PROX_SENSOR_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PROX_SENSOR_BASE, 0xF);
}
