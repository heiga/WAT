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
	uint16_t moveCommand;

	uint8_t leftDirection;
	uint8_t leftTime;
	uint8_t rightDirection;
	uint8_t rightTime;

	bool notFound;

	while (1){

		initCommand = (uint8_t) OSQPend(initCommandQueue, 0, &err);

		if((initCommand & INIT_RCMODE) == INIT_RCMODE){
			rcControl(initCommand);
		}else{ //parse color and time
			notFound = TRUE;
			while(notFound){
				findCommand = 0; //TEST code, red region is defined as 0
				OSQPost(findCommandQueue, (void*) findCommand);
				moveCommand = (uint16_t) OSQPend(moveCommandQueue, 0, &err);
				printf("MOTOR: received %i\n", moveCommand);

				//Straight
				if((moveCommand >= MID_LOWER) && (moveCommand <= MID_UPPER)){
					printf("MOTOR: FORWARD\n");
					motorControl(FORWARD, MED_MOVE);
				//Destination
				}else if(moveCommand == MOVE_DONE){
					printf("MOTOR: DONE\n");
					notFound = FALSE;
				//Left
				}else if((moveCommand < MID_LOWER) && (moveCommand > 0)){
					printf("MOTOR: LEFT\n");
					motorControl(LEFT, SML_MOVE);
					motorControl(FORWARD, SML_MOVE);
				//Right
				}else if(moveCommand > MID_UPPER){
					printf("MOTOR: RIGHT\n");
					motorControl(RIGHT, SML_MOVE);
					motorControl(FORWARD, SML_MOVE);
				//Nothing
				}else{
					printf("MOTOR: NOTHING\n");
					motorControl(RIGHT, MED_MOVE);
				}
			}
		}
	}
}

void motorStop() {
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);
}

void motorForward() {
	//IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
}

void motorReverse() {
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
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
		motorStop();
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
	if(time == INF_MOVE) {
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

// RC Mode stuff run here to reduce overhead
void rcControl(uint8_t input) {
	// Decode the command received from wifi and send it to Motor task
	char direction = STOP;
	int time;

	if((input & STOP) == STOP) {
		direction = STOP;
	}
	if((input & FORWARD) == FORWARD) {
		direction = FORWARD;
	}
	if((input & REVERSE) == REVERSE) {
		direction = REVERSE;
		//IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	}
	if((input & LEFT) == LEFT) {
		direction = LEFT;
	}
	if((input & RIGHT) == RIGHT) {
		direction = RIGHT;
	}
	if((input & i1) == i1) {
		time = SML_MOVE;
	}
	if((input & i2) == i2) {
		time = MED_MOVE;
	}
	if((input & i3) == i3) {
		time = INF_MOVE;
	}
	motorControl(direction, time);
}
