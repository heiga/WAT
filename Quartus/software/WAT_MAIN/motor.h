/*
 * motor_test.h
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#ifndef MOTOR_TEST_H_
#define MOTOR_TEST_H_

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "includes.h"
#include "altera_avalon_pio_regs.h"
#include "wat_main.h"

void motor_task(void* pdata);
void motorControl(char direction, int time);
void motorStop();
void rcControl(uint8_t input);
void motorEStop();

/* Redefine stdbool to use proper constant names */
#define TRUE   true
#define FALSE  false

/* Definition of direction info */
//2 1 PWM
#define MOTOR_FORWARD		0b011 //CW
#define MOTOR_REVERSE		0b101 //CCW
#define MOTOR_SHORTSTOP		0b111 //SHORT STOP, low voltage output
#define MOTOR_FULLSTOP	  	0b001 //FULL STOP, high impedance output

/*
#define STOP				"s"
#define AUTOMATA_MODE		"a"
#define RC_MODE				"r"
#define RED					"R"
#define GREEN				"G"
#define BLUE				"B"
#define SEC_5				"5"
#define SEC_10				"1"
#define SEC_30				"3"
*/

/* Time defintions */
#define SML_MOVE  1
#define MED_MOVE  2
#define INF_MOVE  10

// Autonomous Mode definitions
#define RED			0b00
#define GREEN		0b01
#define BLUE		0b10
#define SEC5		0b01
#define SEC10		0b10
#define SEC30		0b11
#define COM_MASK    0b11
#define SHIFT_MAX   4
#define MID_UPPER	160
#define MID_LOWER   80
#define MOVE_DONE   0xFFFF

// RC Mode definitions
#define INIT_RCMODE 0b11000000
#define STOP		0b00000000
#define	FORWARD		0b00000001
#define REVERSE		0b00000010
#define LEFT		0b00000011
#define RIGHT		0b00000100
#define i1			0b00001000
#define i2			0b00010000
#define i3			0b00011000

#define deg45		380
#define deg90		750

#endif /* MOTOR_TEST_H_ */
