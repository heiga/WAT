/*
 * camera_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "camera_test.h"

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
	uint16_t temp;

	uint8_t cam_reply = 0;
	uint16_t packet_count = 0;

	uint8_t sync_delay = CAM_INIT_SYNC_DELAY;
	bool synced = FALSE;

	/*
	* Synchronise with camera
	* Documentation mentions this will take 25-60 attempts
	* Recommends an increasing delay between attempts
	* with an initial time of 5ms
	*/

	useData = FALSE;
	for (q=0; q < CAM_MAX_ATTEMPTS; q++){
		//Assume false until proven otherwise
		synced = FALSE;

		//Send the sync command
		for (z=0; z < CAM_COMMAND_LENGTH; z++){
			while(!(IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
			IOWR_ALTERA_AVALON_UART_TXDATA(CAM_UART_BASE, CAM_SYNC[z]);
		}

		//Wait the recommended sync time as per doc
		OSTimeDlyHMSM(0, 0, 0, sync_delay);

//		OSQQuery(camCommandQueue, camQueueStatus);
//		temp = camQueueStatus->OSMsg;
//		printf("temp = %i\n", temp);
		cam_reply = (uint8_t)OSQAccept(camCommandQueue, &err);
		if (cam_reply == CAM_ACK_SYNC[0]){
			synced = TRUE;
			printf("Receiving %x", cam_reply);
			for (z=1; z < CAM_COMMAND_LENGTH; z++){
				cam_reply = (uint8_t)OSQPend(camCommandQueue, 0, &err);
				if ((cam_reply != CAM_ACK_SYNC[z]) && (z != CAM_ACK_IGNORE)){
					printf("Got %x expected %x\n", cam_reply, CAM_ACK_SYNC[z]);
					synced = FALSE;
					break;
				}else{
					printf(" %x", cam_reply);
				}
			}
			printf("\n");
		}

		if (synced){
			break;
		}else{
			sync_delay++;
		}
	}

	if (synced){
		printf("Cam synced after %i attempts\n", sync_delay);
		*leds = *leds ^ 0xFF;
	}else{
		printf("Cam sync failure after %i attempts\n", sync_delay);
		//return;
	}

	//Send ACK to camera to ACK its SYNC
	for (z=0; z < CAM_COMMAND_LENGTH; z++){
		while(!(IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
		IOWR_ALTERA_AVALON_UART_TXDATA(CAM_UART_BASE, CAM_ACK_SYNC[z]);
	}

	//Flush queue then turn on camera
	OSQFlush(camCommandQueue);
	for (z=0; z < CAM_COMMAND_LENGTH; z++){
		while(!(IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
		IOWR_ALTERA_AVALON_UART_TXDATA(CAM_UART_BASE, CAM_INIT[z]);
	}
	//Wait the recommended second then check for ACK
	OSTimeDlyHMSM(0, 0, 2, 0);
	printf("Done waiting\n");
	//for (z=0; z < CAM_MAX_ATTEMPTS; z++){
		cam_reply = (uint8_t)OSQPend(camCommandQueue, 0, &err);
		if (cam_reply == CAM_ACK_INIT[0]){
			printf("Receiving %x", cam_reply);
			for (z=1; z < CAM_COMMAND_LENGTH; z++){
				cam_reply = (uint8_t)OSQPend(camCommandQueue, 0, &err);
				if ((cam_reply != CAM_ACK_INIT[z]) && (z != CAM_ACK_IGNORE)){
					printf("Got %x expected %x\n", cam_reply, CAM_ACK_INIT[z]);
					//break;
				}else{
					printf(" %x", cam_reply);
				}
			}
			printf("\n");
		}else{
			printf("Got %x expected %x\n", cam_reply, CAM_ACK_INIT[0]);
		}
	//}
	while (1){
		//wait for button push
		//OSSemPend(FREQ_SEM, 0, &err);

		//tell camera the image settings

		//tell camera to take picture

		//Tell camera we are ready to receive the picture

		//Get total packet count

		//Parse total package count

		//Collect image
		//note, package size = 512 -> Data = 506
		//ID = 2, Size = 2, Data = 506, Verifification = 2

		//print to terminal

		//repeat

		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void cam_uart_interrupt(void * context){
	uint8_t read = 0;

	while(!(IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK));
	read = IORD_ALTERA_AVALON_UART_RXDATA(CAM_UART_BASE);

	if (useData){
		OSQPost(camPackageQueue, (void*) read);
	}else{
		OSQPost(camCommandQueue, (void*) read);
	}
}

void button_interrupt(void * context){
	printf("button\n");
	OSTimeDlyHMSM(0, 0, 0, 400);

	//clear interrupt
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTON_BUTTON_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTON_BUTTON_BASE, 0xF);

	//post semaphore for camera
	OSSemPost(BUTTON_SEM);
}