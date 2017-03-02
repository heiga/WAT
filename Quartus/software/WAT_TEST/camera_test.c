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
	uint8_t sync_delay = CAM_INIT_SYNC_DELAY;
	uint16_t packet_count = 0;
	bool synced = FALSE;
	char *packet_count_string;
	uint8_t cam_reply = 0;
	char *end;
	char *package_ID = "0000";
	char *buffer;
	char temp;
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
/*
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
*/

		for (q=0; q < CAM_MAX_ATTEMPTS; q++){
	 		for (z=0; z < CAM_LENGTH; z++){
	 			while(!(IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_TRDY_MSK));
	 			IOWR_ALTERA_AVALON_UART_TXDATA(CAM_UART_BASE, CAM_SYNC[z]);
	 		}

	 		OSTimeDlyHMSM(0, 0, 0, sync_delay);

	 		if((IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK)){
	 			printf("got ack on %i\n", q);
	 			synced = TRUE;
	 			for (z=0; z< CAM_LENGTH;z++){
	 				while(!(IORD_ALTERA_AVALON_UART_STATUS(CAM_UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK));
		 			cam_reply = IORD_ALTERA_AVALON_UART_RXDATA(CAM_UART_BASE);
		 			printf("%i",cam_reply);
		 			if (cam_reply != CAM_ACK_SYNC[z]){
		 				synced = FALSE;
		 				//break;
		 			}
		 		}
	 			printf("\n");
	 		}else{
	 			synced = FALSE;
	 		}

	 		if (synced){
	 			break;
	 		}else{

	 			sync_delay++;
	 		}
	 	}
		IORD_ALTERA_AVALON_UART_RXDATA(CAM_UART_BASE);
	if (synced){
		printf("Cam synced after %i attempts\n", sync_delay);
		*leds = *leds ^ 0xFF;
	}else{
		printf("Cam sync failure after %i attempts\n", sync_delay);
		return;
	}

	//Turn on camera
	fwrite(CAM_INIT, CAM_LENGTH, ONE_COMMAND, cam);
	OSTimeDlyHMSM(0, 0, 1, 500);
	for (z = 0; z < CAM_LENGTH; z++){
		printf("Waiting where I think we are...");
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
