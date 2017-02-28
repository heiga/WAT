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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

#define MOTOR_TEST_PRIORITY     2
#define CAMERA_TEST_PRIORITY    1
#define WIFI_TEST_PRIORITY		3

/* Definition of Queue info */
#define SEM_INIT_VALUE 0
OS_EVENT *FREQ_SEM;

/* Definition of camera constants */
#define CAM_WRITE_OFFSET        0
#define CAM_READ_OFFSET         0
#define CAM_LENGTH      	    24
#define CAM_SERIAL_LEN			4
#define CAM_DATA_LENGTH	       	3
#define CAM_MAX_ATTEMPTS   		100
#define CAM_INIT_SYNC_DELAY     5
#define ONE_COMMAND				1
#define CAM_WAIT_SEC			1
#define CAM_WAIT_MS				5
#define CAM_LAST_PACKAGE		"F0FO"
#define CAM_PACKAGE_SIZE		512

/* Definition of camera commands */
static const char *CAM_SYNC = "AA\n0E\n00\n00\n00\n00\n";
#define CAM_INIT "AA0100070707"
#define CAM_SIZE "AA0608000200" //512 bytes
#define CAM_SNAP "AA0500000000"
#define CAM_GRAB "AA0401000000"
#define CAM_DATA "AA0A01XXXXXX" //NOTE sent by camera, last six bits are the package size

/* Definition of ACK commands to pair with each relevant command */
static const char *CAM_ACK_SYNC = "AA0E0D000000";
#define CAM_ACK_INIT "AA0E01000000"
#define CAM_ACK_SIZE "AA0E06000000"
#define CAM_ACK_SNAP "AA0E05000000"
#define CAM_ACK_GRAB "AA0E04000000"
#define CAM_ACK_DEND "AA0E0000F0F0"

/* Defintion of the ACK commands for data processing
 * NOTE there are two more commands consisting of the last packet ID
 */
#define CAM_DACK_ONE 0xAA
#define CAM_DACK_TWO 0x0E
#define CAM_DACK_TRE 0x00
#define CAM_DACK_FOR 0x00


/*
 * Drives FORWARD initially but when the INFRARED SENSOR
 * detects something and it will REVERSE the direction
 */
void motor_test(void* pdata){
	  //while (1){
		  //do stuff
	  //}
	OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
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

	uint32_t *leds = GREEN_LEDS_BASE;

	uint8_t q = 0;
	uint8_t z = 0;
	uint8_t err;
	uint8_t sync_delay = CAM_INIT_SYNC_DELAY;
	uint16_t packet_count = 0;
	bool synced = FALSE;
	char *packet_count_string;
	char cam_reply = 0;
	char *end;
	char *package_ID = "0000";
	char *buffer;
	FILE* cam;
	FILE* picture;

	//Open the serial connection
	cam = fopen(CAM_UART_NAME,"r+");
	if(!cam){
		printf("Error opening connection to camera");
		return;
	}else{
		printf("Found cam\n");
		*leds = *leds ^ 0xFF;
	}
	/*
	* Synchronise with camera
	* Documentation mentions this will take 25-60 attempts
	* Recommends an increasing delay between attempts
	* with an initial time of 5ms
	*/
/*
	while(1){
		fwrite("FF", 4, 1, cam);
	}
*/

	for (q = 0; q < CAM_MAX_ATTEMPTS; q++){
		printf("Attempting sync %i\n", q);
		fwrite(CAM_SYNC, CAM_LENGTH, CAM_SERIAL_LEN, cam);

		OSTimeDlyHMSM(0, 0, 0, sync_delay);
		synced = TRUE;
		printf("After attempt %i\n", q);
		//for (z = 0; z < CAM_LENGTH; z++){
			//fgets(buffer, CAM_SERIAL_LEN, cam);

			cam_reply = fgetc(cam);
			if (strcmp(cam_reply, &CAM_ACK_SYNC) != 0){
				synced = FALSE;
				printf("Failed with %s\n", buffer);
			}
		//}

		if (synced){
			fwrite(CAM_ACK_SYNC, CAM_LENGTH, ONE_COMMAND, cam);
			break;
		}else{
			sync_delay++;
			printf("Failed sync %i\n", q);
		}
	}

	if (synced){
		printf("Cam synced after %i attempts\n", sync_delay);
		*leds = *leds ^ 0xFF;
	}else{
		printf("Cam sync failure");
		return;
	}

	//Turn on camera
	fwrite(CAM_INIT, CAM_LENGTH, ONE_COMMAND, cam);
	OSTimeDlyHMSM(0, 0, 1, 500);
	for (z = 0; z < CAM_LENGTH; z++){
		cam_reply = fgetc(cam);
		if (strcmp(&cam_reply, &CAM_ACK_INIT[z]) != 0){
			printf("Cam init failure");
			return;
		}
	}

	//Wait the recommended second then check for ACK

	while (1){
		//wait for button push
		//OSSemPend(FREQ_SEM, 0, &err);

		//tell camera to take picture
		fwrite(CAM_SIZE, CAM_LENGTH, ONE_COMMAND, cam);
		OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
		for (z = 0; z < CAM_LENGTH; z++){
			cam_reply = fgetc(cam);
			if (strcmp(&cam_reply, &CAM_ACK_SIZE[z]) != 0){
				printf("Cam size failure");
				return;
			}
		}

		//tell camera to take picture
		fwrite(CAM_SNAP, CAM_LENGTH, ONE_COMMAND, cam);
		OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
		for (z = 0; z < CAM_LENGTH; z++){
			cam_reply = fgetc(cam);
			if (strcmp(&cam_reply, &CAM_ACK_SNAP[z]) != 0){
				printf("Cam snap failure");
				return;
			}
		}

		//Tell camera we are reacy to recieve the picture
		fwrite(CAM_GRAB, CAM_LENGTH, ONE_COMMAND, cam);
		OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
		for (z = 0; z < CAM_LENGTH; z++){
			cam_reply = fgetc(cam);
			if (strcmp(&cam_reply, &CAM_ACK_GRAB[z]) != 0){
				printf("Cam grab failure");
				return;
			}
		}

		//Get total packet count
		OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
		for (z = 0; z < CAM_DATA_LENGTH; z++){
			cam_reply = fgetc(cam);
			if (strcmp(&cam_reply, &CAM_DATA[z]) != 0){
				printf("Cam get data count failure");
				return;
			}
		}
		//get last characters, turn into an int
		fgets(packet_count_string, CAM_LENGTH - CAM_DATA_LENGTH, cam);
		packet_count = strtoull(packet_count_string, &end, 16);

		//Collect image
		//note, package size = 512 -> Data = 506
		//ID = 2, Size = 2, Data = 506, Verifification = 2

		//TODO fix these constants
		while(strcmp(package_ID, CAM_LAST_PACKAGE) != 0){
			fgets(buffer, CAM_PACKAGE_SIZE, cam);
			strncpy(package_ID, buffer, 4);

			fseek(picture, 0, SEEK_END);
			for (z = 5; z < CAM_PACKAGE_SIZE-2; z++){
				fwrite(&buffer[z], sizeof(char), ONE_COMMAND, picture);
			}
		}


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
	  //while (1){
		  //do stuff
	  //}
	OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
}

static void cam_button_interrupt(void * context){
	//clear interrupt
	//IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BUTTON_BASE,0);

	//post semaphore for camera
	//OSSemPost(FREQ_SEM);
}

/* The main function creates two task and starts multi-tasking */
int main(void){
  
	printf("START OF MAIN\n");


	OSTaskCreateExt(motor_test,
                  	NULL,
                  	(void *)&motor_test_stk[TASK_STACKSIZE-1],
                  	MOTOR_TEST_PRIORITY,
                  	MOTOR_TEST_PRIORITY,
                  	motor_test_stk,
                  	TASK_STACKSIZE,
                  	NULL,
                  	0);

	printf("START OF MOTOR\n");

	OSTaskCreateExt(camera_test,
                  	NULL,
                  	(void *)&camera_test_stk[TASK_STACKSIZE-1],
                  	CAMERA_TEST_PRIORITY,
                  	CAMERA_TEST_PRIORITY,
                  	camera_test_stk,
                  	TASK_STACKSIZE,
                  	NULL,
                  	0);

	printf("START OF CAM\n");

	OSTaskCreateExt(wifi_test,
                    NULL,
                    (void *)&wifi_test_stk[TASK_STACKSIZE-1],
                    WIFI_TEST_PRIORITY,
                    WIFI_TEST_PRIORITY,
                    wifi_test_stk,
                    TASK_STACKSIZE,
                    NULL,
                    0);

	printf("START OF WIFI\n");

	//Semaphore for button controlling camera
	//FREQ_SEM = OSSemCreate(SEM_INIT_VALUE);

	//Interrupt controller for button controlling camera
	//alt_ic_isr_register(BUTTON_BUTTON_IRQ_INTERRUPT_CONTROLLER_ID,
	//				    BUTTON_BUTTON_IRQ,
	//				    cam_button_interrupt,
	//				    NULL,
	//				    NULL);
	//IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_BUTTON_BASE, 0x1);
	//IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BUTTON_BASE, 0x0);

	OSStart();

	printf("END OF MAIN\n");

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
