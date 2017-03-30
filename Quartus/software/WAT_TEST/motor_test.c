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
		//OSSemPend(SENSOR_SEM, 0, &err);

        /*
		//Drive straight for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_FORWARD);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x1);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);

		//Stop for half a second
		OSTimeDlyHMSM(0, 0, 0, 500);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);

		//Drive left for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x0);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);

		//Stop for half a second
		OSTimeDlyHMSM(0, 0, 0, 500);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);

		//Drive right for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x1);
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_SHORTSTOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);

		//Stop for half a second
		OSTimeDlyHMSM(0, 0, 0, 500);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);

		//Drive reverse for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x0);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x0);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);

		//Stop for half a second
		OSTimeDlyHMSM(0, 0, 0, 500);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);

        */
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	}
}

void sensor_interrupt(void * context){
	printf("sensor\n");

	//clear interrupt
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PROX_SENSOR_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PROX_SENSOR_BASE, 0xF);
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);

	//post semaphore for camera
	//OSSemPost(SENSOR_SEM);
}
