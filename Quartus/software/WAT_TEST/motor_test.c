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
		//OSSemPend(SENSOR_SEM, 0, &err);
		//Drive straight for one second
		//printf("Start of loop\n");
		OSTimeDlyHMSM(0, 0, 1, 0);
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_FORWARD);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x1);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
		//Drive left for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		//printf("First of loop\n");
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x0);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
		//Drive right for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		//printf("Second of loop\n");
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x1);
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_STOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);
		//Drive reverse for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		//printf("Third of loop\n");
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, 0x0);
		IOWR_ALTERA_AVALON_PIO_DATA(BUTTON_LED_BASE, 0x0);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);
		//Stop for one second
		OSTimeDlyHMSM(0, 0, 1, 0);
		//printf("Fourth of loop\n");
		IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LEDS_BASE, MOTOR_REVERSE);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_STOP);
		IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_STOP);
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
