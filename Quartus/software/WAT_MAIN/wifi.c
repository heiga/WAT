/*
 * wifi_test.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Randy Baron
 */

#include "wifi.h"

void wifi_task(void* pdata){
	uint8_t err;

    uint8_t wifiReceive;

	while (1){
		  //do stuff
		// OSSemPend(BUTTON_SEM, 0, &err);
		printf("Hello from wifi\n");

        wifiReceive = (uint8_t) OSQPend(wifiQueue, 0, &err);
        printf("Received form Wifi: %d \n", wifiReceive);
	}
	//OSTimeDlyHMSM(0, 0, 0, CAM_WAIT_MS);
}

void wifi_uart_interrupt(void * context){
    uint8_t read = 0;
    printf("Hello from wifi interrupt\n");
    while(!(IORD_ALTERA_AVALON_UART_STATUS(WIFI_UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK));
    read = IORD_ALTERA_AVALON_UART_RXDATA(WIFI_UART_BASE);
    
    OSQPost(wifiQueue, (void*) read);
}
