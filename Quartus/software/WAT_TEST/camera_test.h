/*
 * camera_test.h
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#ifndef CAMERA_TEST_H_
#define CAMERA_TEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "includes.h"
#include "altera_avalon_uart_regs.h"

void camera_test(void* pdata);

/* Redefine stdbool to use proper constant names */
#define TRUE   true
#define FALSE  false

/* Definition of camera constants */
#define CAM_WRITE_OFFSET        0
#define CAM_READ_OFFSET         0
#define CAM_LENGTH      	    6
#define CAM_SERIAL_LEN			4
#define CAM_DATA_LENGTH	       	3
#define CAM_MAX_ATTEMPTS   		60
#define CAM_INIT_SYNC_DELAY     5
#define ONE_COMMAND				1
#define CAM_WAIT_SEC			1
#define CAM_WAIT_MS				5
#define CAM_LAST_PACKAGE		"F0FO"
#define CAM_PACKAGE_SIZE		512

/* Definition of camera commands */
static const uint8_t CAM_SYNC[CAM_LENGTH] = {0xAA, 0x0D, 0x00, 0x00, 0x00,0x00};
//static const uint8_t CAM_SYNC[CAM_LENGTH] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
static const char *CAM_INIT = "AA0100070707";
static const char *CAM_SIZE = "AA0608000200"; //512 bytes
static const char *CAM_SNAP = "AA0500000000";
static const char *CAM_GRAB = "AA0401000000";
static const char *CAM_DATA = "AA0A01XXXXXX"; //NOTE sent by camera, last six bits are the package size

/* Definition of ACK commands to pair with each relevant command */
static const uint8_t CAM_ACK_SYNC[CAM_LENGTH] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};
static const char *CAM_ACK_INIT = "AA0E01000000";
static const char *CAM_ACK_SIZE = "AA0E06000000";
static const char *CAM_ACK_SNAP = "AA0E05000000";
static const char *CAM_ACK_GRAB = "AA0E04000000";
static const char *CAM_ACK_DEND = "AA0E0000F0F0";

/* Defintion of the ACK commands for data processing
 * NOTE there are two more commands consisting of the last packet ID
 */
#define CAM_DACK_ONE 0xAA
#define CAM_DACK_TWO 0x0E
#define CAM_DACK_TRE 0x00
#define CAM_DACK_FOR 0x00

#endif /* CAMERA_TEST_H_ */
