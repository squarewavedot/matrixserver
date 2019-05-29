/*
	Simple example to open a maximum of 4 devices - write some data then read it back.
	Shows one method of using list devices also.
	Assumes the devices have a loopback connector on them and they also have a serial number

	To build use the following gcc statement 
	(assuming you have the static d2xx library in the /usr/local/lib directory
	and you have created a symbolic link to it in the current dir).
	gcc -o static_link main.c -ldl -lpthread libftd2xx.a 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../ftd2xx.h"

#define BUF_SIZE 0x10

#define MAX_DEVICES		5

int main()
{
	char 	cBufWrite[BUF_SIZE];
	char * 	pcBufRead = NULL;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead;
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle[MAX_DEVICES];
	int	iNumDevs = 0;
	int	i;
	int	iDevicesOpen;	
	
	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	pcBufLD[MAX_DEVICES] = NULL;
	
	ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
	
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices returned %d\n", (int)ftStatus);
		return 1;
	}
	
	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}
	
	
	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ) ; i++) {
		/* Setup */
		if((ftStatus = FT_OpenEx(cBufLD[i], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle[i])) != FT_OK){
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error: FT_OpenEx returned %d for device %d\n", (int)ftStatus, i);
			return 1;
		}
	
		printf("Opened device %s\n", cBufLD[i]);

		if((ftStatus = FT_SetBaudRate(ftHandle[i], 9600)) != FT_OK) {
			printf("Error: FT_SetBaudRate returned %d, cBufLD[i] = %s\n", (int)ftStatus, cBufLD[i]);
			break;
		}
		
		/* Write */
		if((ftStatus = FT_Write(ftHandle[i], cBufWrite, BUF_SIZE, &dwBytesWritten)) != FT_OK) {
			printf("Error: FT_Write returned %d\n", (int)ftStatus);
			break;
		}
		sleep(1);
		
		/* Read */
		dwRxSize = 0;			
		while ((dwRxSize < BUF_SIZE) && (ftStatus == FT_OK)) {
			ftStatus = FT_GetQueueStatus(ftHandle[i], &dwRxSize);
		}
		if(ftStatus == FT_OK) {
			pcBufRead = (char*)realloc(pcBufRead, dwRxSize);
			if((ftStatus = FT_Read(ftHandle[i], pcBufRead, dwRxSize, &dwBytesRead)) != FT_OK) {
				printf("Error: FT_Read returned %d\n", (int)ftStatus);
			}
			else {
				printf("FT_Read read %d bytes\n", (int)dwBytesRead);
			}
		}
		else {
			printf("Error: FT_GetQueueStatus returned %d\n", (int)ftStatus);	
		}
	}

	iDevicesOpen = i;
	/* Cleanup */
	for(i = 0; i < iDevicesOpen; i++) {
		FT_Close(ftHandle[i]);
		printf("Closed device %s\n", cBufLD[i]);
	}
	
	if(pcBufRead)
		free(pcBufRead);
	return 0;
}
