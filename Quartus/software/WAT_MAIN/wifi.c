/*
 * wifi_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "wifi.h"
#include "motor.h"

void wifi_task(void* pdata){
	uint8_t err;
    uint8_t wifiReceive;

    //IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);

	while (1){

        wifiReceive = (uint8_t) OSQPend(wifiPackageQueue, 0, &err);
        printf("Received form Wifi: %d \n", wifiReceive);

        if((wifiReceive & RCMODE) != RCMODE) {

        	OSQPost(initCommandQueue, (void*) wifiReceive);
        }
        else {
        	//printf("IN RC MODE\n");
        	rcControl(wifiReceive);
        }
	}
}

void wifi_uart_interrupt(void * context){
    uint8_t read = 0;

    while(!(IORD_ALTERA_AVALON_UART_STATUS(WIFI_UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK));
    read = IORD_ALTERA_AVALON_UART_RXDATA(WIFI_UART_BASE);
    
    OSQPost(wifiPackageQueue, (void*) read);

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
		time = 1;
	}
	if((input & i2) == i2) {
		time = 2;
	}
	if((input & i3) == i3) {
		// 10 = INFINITE TIME
		time = 10;
	}
	motorControl(direction, time);
}

