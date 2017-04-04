/*
 * motor_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "motor.h"
#include "wifi.h"

int MOTORMOVING = false;
int MOVINGFORWARD = false;

/*
 * Drives FORWARD initially but when the INFRARED SENSOR
 * detects something and it will REVERSE the direction
 */
void motor_task(void* pdata){
	uint8_t err;
	uint8_t initCommand;
	uint8_t findCommand;
	uint8_t lastCommand;
	uint16_t moveCommand;
	uint8_t shiftCounter;

	uint8_t waitTime;

	bool notFound;

	while (1){

		initCommand = (uint8_t) OSQPend(initCommandQueue, 0, &err);

		if((initCommand & INIT_RCMODE) == INIT_RCMODE){
			rcControl(initCommand);
		}else{ //parse color and time
			//Get first colour and time at each destination
			waitTime = initCommand & COM_MASK;
			initCommand = initCommand >> 2;
			shiftCounter = 1;
			findCommand = initCommand & COM_MASK;
			initCommand = initCommand >> 2;
			shiftCounter++;
			notFound = TRUE;
			while(notFound){
				OSQPost(findCommandQueue, (void*) findCommand);
				moveCommand = (uint16_t) OSQPend(moveCommandQueue, 0, &err);
				printf("MOTOR: RECIEVED %i\n", moveCommand);

				/* Check first for edge cases of done or none
				 * (close enough or found nothing of that colour)
				 * Check then for direction by bit shifting and
				 * drive forward a small amount. If you get too
				 * close the buzzer will go off anyways
				 */
				if (moveCommand == MOVE_DONE){
					printf("MOTOR: DONE\n");
					victoryScreech();
					if (shiftCounter == SHIFT_MAX){
						notFound = FALSE;
					}else{
						findCommand = initCommand & COM_MASK;
						initCommand = initCommand >> 2;
						shiftCounter++;
					}
				}else if (moveCommand == MOVE_NONE){
					printf("MOTOR: NONE\n");
					motorControl(RIGHT, SMALL_TURN);
				}else{
					if (moveCommand > MOVE_LEFT){
						printf("MOTOR: LEFT\n");
						moveCommand = moveCommand >> 7;
						motorControl(LEFT, moveCommand);
					}else{
						printf("MOTOR: RIGHT\n");
						moveCommand *= 2;
						motorControl(RIGHT, moveCommand);
					}
					printf("MOTOR: FORWARD\n");
					motorControl(FORWARD, SML_MOVE);
				}
			}
		}
	}
}

void motorStop() {
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_SHORTSTOP);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_SHORTSTOP);
	//MOVINGFORWARD = FALSE;
}

void motorForward() {
	//IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);
	MOVINGFORWARD = TRUE;
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
}

void motorReverse() {
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);
	MOVINGFORWARD = FALSE;
}

void motorLeft() {
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_FORWARD);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_REVERSE);
	MOVINGFORWARD = FALSE;
}

void motorRight() {
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_L_BASE, MOTOR_REVERSE);
	IOWR_ALTERA_AVALON_PIO_DATA(MOTOR_R_BASE, MOTOR_FORWARD);
	MOVINGFORWARD = FALSE;
}

void motorControl(char direction, int time) {
	int isTurn = 0;

	if(MOTORMOVING) {
		motorStop();
		OSTimeDlyHMSM(0, 0, 0, 20);
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
		OSTimeDlyHMSM(0, 0, 0, time);
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
		time = SMALL_TURN;
	}
	if((input & i2) == i2) {
		time = SHARP_TURN;
	}
	if((input & i3) == i3) {
		time = INF_MOVE;
	}
	motorControl(direction, time);
}

void motorEStop() {
	if(MOVINGFORWARD) {
		motorStop();
		IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);
	}
}

void victoryScreech(){
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);
	OSTimeDlyHMSM(0, 0, 1, 0);
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	OSTimeDlyHMSM(0, 0, 0, 500);
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);
	OSTimeDlyHMSM(0, 0, 1, 0);
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
	OSTimeDlyHMSM(0, 0, 0, 500);
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);
	OSTimeDlyHMSM(0, 0, 1, 0);
	IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x0);
}
