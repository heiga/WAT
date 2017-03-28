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
    uint8_t motorProgress = 0xFF;

    FILE* log;

    //IOWR_ALTERA_AVALON_PIO_DATA(SPEAKER_BASE, 0x1);

	while (1){


        wifiReceive = (uint8_t) OSQPend(wifiPackageQueue, 0, &err);
        printf("Received form Wifi: %d \n", wifiReceive);

//		if ((log = fopen(LOG, "rb")) == NULL) {
			//no file, wait for command
			wifiReceive = (uint8_t) OSQPend(wifiPackageQueue, 0, &err);
//		}else{
//			//found file, parse so we can recover from the crash
//			wifiReceive = parseLog(log);
//		}

        //printf("Received form Wifi: %d \n", wifiReceive);
        OSQPost(initCommandQueue, (void*) wifiReceive);
	}
}

void wifi_uart_interrupt(void * context){
    uint8_t read = 0;

    while(!(IORD_ALTERA_AVALON_UART_STATUS(WIFI_UART_BASE) & ALTERA_AVALON_UART_STATUS_RRDY_MSK));
    read = IORD_ALTERA_AVALON_UART_RXDATA(WIFI_UART_BASE);
    
    OSQPost(wifiPackageQueue, (void*) read);

}

uint8_t parseLog(FILE* log){
	uint8_t recovCommand = 0;

	return recovCommand;
}

