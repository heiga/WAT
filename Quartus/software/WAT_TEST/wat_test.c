/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <io.h>
#include "includes.h"

/* Redefine stdbool to use proper constants */
#define TRUE   true
#define FALSE  false

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    motor_test_stk[TASK_STACKSIZE];
OS_STK    camera_test_stk[TASK_STACKSIZE];
OS_STK    wifi_test_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define MOTOR_TEST_PRIORITY     1
#define CAMERA_TEST_PRIORITY    2
#define WIFI_TEST_PRIORITY		3

/* Definition of Queue info */
#define SEM_INIT_VALUE 0
OS_EVENT *FREQ_SEM;

/* Definition of camera constants */
#define CAM_WRITE_OFFSET        0
#define CAM_READ_OFFSET         0
#define CAM_LENGTH      	    6
#define CAM_MAX_SYNC_ATTEMPTS   60
#define CAM_INIT_SYNC_DELAY     5

/* Definition of camera commands */
static const CAM_INIT[CAM_LENGTH] = {0xAA, 0x01, 0x00, 0x07, 0x07, 0x07};
static const CAM_SYNC[CAM_LENGTH] = {0xAA, 0x0E, 0x00, 0x00, 0x00, 0x00};

/* Definition of ACK commands to pair with each relevant command */
static const CAM_ACK_INIT[CAM_LENGTH] = {0xAA, 0x0E, 0x01, 0x00, 0x00, 0x00};
static const CAM_ACK_SYNC[CAM_LENGTH] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};

/*
 * Drives FORWARD initially but when the INFRARED SENSOR
 * detects something and it will REVERSE the direction
 */
void motor_test(void* pdata){
	  while (1){
		  //do stuff
	  }
}

//section 7 is the UART reference
//https://www.altera.com/en_US/pdfs/literature/ug/ug_embedded_ip.pdf
//
//helpful functions for UART interfacing
//IORD(base, offset)
//IOWR(base, offset, data)

/*
 * For this test it is assumed that the operator has minimum
 * two monotone images ready. Upon a BUTTON being pressed the
 * camera will take a picture and output the average color
 * value to the terminal. Operator will switch the image and
 * press the button again. A different average color should
 * appear in the terminal. Repeat as needed.
 */
void camera_test(void* pdata){

	uint8_t q = 0;
	uint8_t err;
	uint8_t sync_delay = CAM_INIT_SYNC_DELAY;
	bool synced = FALSE;


	/*
	* Synchronise with camera
	* Documentation mentions this will take 25-60 attempts
	* Recommends an increasing delay between attempts
	* with an initial time of 5ms
	*/
	for (q=0; q < CAM_MAX_SYNC_ATTEMPTS; q++){
		for (q=0; q < CAM_LENGTH; q++){
			IOWR(CAM_UART_BASE, CAM_WRITE_OFFSET, CAM_SYNC[q]);
		}

		OSTimeDlyHMSM(0, 0, 0, sync_delay);

		for (uint8_t q=0; q< CAM_LENGTH;q++){
			synced = TRUE;
			if (IORD(CAM_UART_BASE, CAM_READ_OFFSET) != CAM_ACK_SYNC[q]){
				synced = FALSE;
			}
		}

		if (synced){
			break;
		}else{
			sync_delay++;
		}
	}

	if (synced){
		printf("Cam synced after %i attempts\n", sync_delay);
	}else{
		printf("Cam sync failure");
		return;
	}

	//Turn on camera
	for (q=0; q < CAM_LENGTH; q++){
		IOWR(CAM_UART_BASE, CAM_WRITE_OFFSET, CAM_INIT[q]);
	}

	//Wait for ACK that camera has turned on
	OSTimeDlyHMSM(0, 0, 1, 0);
	for (uint8_t q=0; q < CAM_LENGTH; q++){
		if (IORD(CAM_UART_BASE, CAM_READ_OFFSET) != CAM_ACK_INIT[q]){
			printf("Cam init failure");
			return;
		}
	}

	while (1){
		//wait for button push
		OSSemPend(FREQ_SEM, 0, &err);

		//tell camera to take picture

		//load image and process

		//print to terminal

		//repeat

	}
}

/*
 * Accept a command from the web interface and
 * active some audio-visual peripheral based on the
 * input from the web interface
 */
void wifi_test(void* pdata){
	  while (1){
		  //do stuff
	  }
}

static void cam_button_interrupt(void * context){
	//clear interrupt
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BUTTON_BASE,0);

	//post semaphore for camera
	OSSemPost(FREQ_SEM);
}

/* The main function creates two task and starts multi-tasking */
int main(void){
  
	OSTaskCreateExt(motor_test,
                  	NULL,
                  	(void *)&motor_test_stk[TASK_STACKSIZE-1],
                  	MOTOR_TEST_PRIORITY,
                  	MOTOR_TEST_PRIORITY,
                  	motor_test_stk,
                  	TASK_STACKSIZE,
                  	NULL,
                  	0);

	OSTaskCreateExt(camera_test,
                  	NULL,
                  	(void *)&camera_test_stk[TASK_STACKSIZE-1],
                  	CAMERA_TEST_PRIORITY,
                  	CAMERA_TEST_PRIORITY,
                  	camera_test_stk,
                  	TASK_STACKSIZE,
                  	NULL,
                  	0);

	OSTaskCreateExt(wifi_test,
                    NULL,
                    (void *)&wifi_test_stk[TASK_STACKSIZE-1],
                    WIFI_TEST_PRIORITY,
                    WIFI_TEST_PRIORITY,
                    wifi_test_stk,
                    TASK_STACKSIZE,
                    NULL,
                    0);

	//Semaphore for button controlling camera
	FREQ_SEM = OSSemCreate(SEM_INIT_VALUE);

	//Interrupt controller for button controlling camera
	alt_ic_isr_register(BUTTON_BUTTON_IRQ_INTERRUPT_CONTROLLER_ID,
					    BUTTON_BUTTON_IRQ,
					    cam_button_interrupt,
					    NULL,
					    NULL);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_BUTTON_BASE, 0x1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BUTTON_BASE, 0x0);

	OSStart();
	return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
