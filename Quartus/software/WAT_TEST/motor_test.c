/*
 * motor_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "motor_test.h"

/*
 * Drives FORWARD initially but when the INFRARED SENSOR
 * detects something and it will REVERSE the direction
 */
void motor_test(void* pdata){
	uint8_t err;
	while (1){
		//do stuff
		OSSemPend(SENSOR_SEM, 0, &err);
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PROX_SENSOR_BASE, 0xF);
		printf("Hello from motor\n");
	}
}

void sensor_interrupt(void * context){
	printf("sensor\n");


	//clear interrupt
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PROX_SENSOR_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PROX_SENSOR_BASE, 0xF);
	//post semaphore for camera
	OSSemPost(SENSOR_SEM);
}
