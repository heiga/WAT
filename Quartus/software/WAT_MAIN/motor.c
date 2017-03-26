/*
 * motor_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "motor.h"
#include "wifi.h"

int MOTORMOVING = false;

/*
 * Drives FORWARD initially but when the INFRARED SENSOR
 * detects something and it will REVERSE the direction
 */
void motor_task(void* pdata){
	uint8_t err;
	uint8_t initCommand;
	uint8_t findCommand;
	uint32_t moveCommand;

	uint8_t leftDirection;
	uint8_t leftTime;
	uint8_t rightDirection;
	uint8_t rightTime;

	bool notFound;

	while (1){

		notFound = TRUE;
		initCommand = (uint8_t) OSQPend(initCommandQueue, 0, &err);

		switch(initCommand){
		case INIT_RED:
			findCommand = FIND_RED;
			break;

		case INIT_BLUE:
			findCommand = FIND_BLUE;
			break;

		case INIT_GREEN:
			findCommand = FIND_GREEN;
			break;
		}

		while(notFound){
			//Pass command to camera and await further instructions
			OSQPost(findCommandQueue, (void*) findCommand);
			moveCommand = (uint8_t) OSQPend(moveCommandQueue, 0, &err);

			if (moveCommand == MOVE_STOP){
				notFound = FALSE;
			}else{
				/* Parse the instructions. These are encoded as
				 * four separate 8 bit instructions in a single
				 * 32 bit integer. Currently time is not used
				 * but that may change as the design is refined
				 * so they are being put in as place holders
				 */
				leftDirection = moveCommand & 0xFF;
				moveCommand = moveCommand >> 8;
				leftTime = moveCommand & 0xFF;
				moveCommand = moveCommand >> 8;
				rightDirection = moveCommand & 0xFF;
				moveCommand = moveCommand >> 8;
				rightTime = moveCommand & 0xFF;
				moveCommand = moveCommand >> 8;
			}

			IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, leftDirection);
			IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, rightDirection);
			OSTimeDlyHMSM(0, 0, 1, 0);
			IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
			IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);
		}

	}
}

void motorStop() {
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);
}

void motorForward() {
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
}

void motorReverse() {
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);
}

void motorLeft() {
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);
}

void motorRight() {
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
}

void motorControl(char direction, int time) {
	int isTurn = 0;

	if(MOTORMOVING) {
		motorStop();
	}
	if(direction == STOP) {
		// Cant do stop because delay is broken and espComplete is broken
		//motorStop();
		MOTORMOVING = false;
		return;
	}
	if(direction == FORWARD) {
		motorForward();
	}
	if(direction == REVERSE) {
		motorReverse();
	}
	if(direction == LEFT) {
		isTurn = 1;
		motorLeft();
	}
	if(direction == RIGHT) {
		isTurn = 1;
		motorRight();
	}
	MOTORMOVING = true;
	if(time == 10) {
		return;
	}
	if(isTurn) {
		OSTimeDlyHMSM(0, 0, 0, time * deg45);
	}
	else {
		OSTimeDlyHMSM(0, 0, time, 0);
	}
	motorStop();
}
