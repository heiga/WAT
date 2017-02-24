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
#include <inttypes.h>
#include <io.h>
#include "includes.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    motor_test_stk[TASK_STACKSIZE];
OS_STK    camera_test_stk[TASK_STACKSIZE];
OS_STK    wifi_test_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define MOTOR_TEST_PRIORITY     1
#define CAMERA_TEST_PRIORITY    2
#define WIFI_TEST_PRIORITY		3

/* Definition of camera commands */
#define CAM_WRITE_OFFSET        2
#define CAM_READ_OFFSET         2
#define CAM_LENGTH      	    6
static const CAM_START[CAM_LENGTH] = {0xAA, 0x01, 0x00, 0x07, 0x09, 0x03};
static const CAM_ACK[CAM_LENGTH] = {0xAA, 0x0E, 0x00, 0x00, 0x00, 0x00}; //NEEDS REVISION

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

	//add sync commands

	for (uint8_t q=0; q< CAM_LENGTH;q++){
		IOWR(CAM_UART_BASE,CAM_WRITE_OFFSET,CAM_START[q]);
	}

	OSTimeDlyHMSM(0, 0, 3, 0);

	for (uint8_t q=0; q< CAM_LENGTH;q++){
		if (IORD(CAM_UART_BASE,CAM_WRITE_OFFSET) != CAM_ACK[q]){
			printf("Camera start failure");
			return;
		}
	}

	while (1){
		//wait for button push
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
