/*
 * wifi_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "wifi_test.h"

void wifi_test(void* pdata){
	uint8_t err;
	while (1){
		  //do stuff
		OSSemPend(BUTTON_SEM, 0, &err);
		printf("Hello from wifi\n");
	}
	//OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
}

void button_interrupt(void * context){
	printf("button\n");
	OSTimeDlyHMSM(0, 0, 0, 400);

	//clear interrupt
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BUTTON_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_BUTTON_BASE, 0xF);

	//post semaphore for camera
	OSSemPost(BUTTON_SEM);
}
