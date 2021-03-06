/*
 * wat_main.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Randy Baron
 */

#ifndef WAT_MAIN_H_
#define WAT_MAIN_H_

#include "includes.h"
#include "stdbool.h"

/*Definition of internal WiFi queues*/
#define WIFI_PACKAGE_BUFFER_LENGTH     1
OS_EVENT* wifiPackageQueue;
void* wifiPackageBuffer[WIFI_PACKAGE_BUFFER_LENGTH];

/*Definition of internal camera queues*/
#define CAM_COMMAND_BUFFER_LENGTH		12
#define CAM_PACKAGE_BUFFER_LENGTH		512
OS_Q_DATA* camQueueStatus;
OS_EVENT* camCommandQueue;
OS_EVENT* camPackageQueue;
void* camCommandBuffer[CAM_COMMAND_BUFFER_LENGTH];
void* camPackageBuffer[CAM_PACKAGE_BUFFER_LENGTH];

/*Definition of intertask queue, WiFi to motor*/
#define INIT_COMMAND_BUFFER_LENGTH     1
OS_EVENT* initCommandQueue;
void* initCommandBuffer[INIT_COMMAND_BUFFER_LENGTH];

/*Definition of intertask queue, motor to camera*/
#define FIND_COMMAND_BUFFER_LENGTH     1
OS_EVENT* findCommandQueue;
void* findCommandBuffer[FIND_COMMAND_BUFFER_LENGTH];

/*Definition of intertask queue, motor to camera*/
#define MOVE_COMMAND_BUFFER_LENGTH     1
OS_EVENT* moveCommandQueue;
void* moveCommandBuffer[MOVE_COMMAND_BUFFER_LENGTH];

/*Definition of intertask queue, camera to WiFi*/
#define FOUND_COMMAND_BUFFER_LENGTH     1
OS_EVENT* foundCommandQueue;
void* foundCommandBuffer[FOUND_COMMAND_BUFFER_LENGTH];

/*Definition of intertask constants, motor to camera */
#define MOVE_STOP        0xFFFF

/* Defintion of intertask control global, wifi to motor */
bool stopRequested;
#endif /* WAT_MAIN_H_ */
