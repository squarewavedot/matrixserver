/*
	Simple example to open a maximum of 4 devices - write some data then read it back.
	Shows one method of using list devices also.
	Assumes the devices have a loopback connector on them and they also have a serial number

	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o simple main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../ftd2xx.h"

#define BUF_SIZE 0x10

#define MAX_DEVICES		5



static void dumpBuffer(unsigned char *buffer, int elements)
{
	int j;

	printf(" [");
	for (j = 0; j < elements; j++)
	{
		if (j > 0)
			printf(", ");
		printf("0x%02X", (unsigned int)buffer[j]);
	}
	printf("]\n");
}



int main()
{
	unsigned char 	cBufWrite[BUF_SIZE];
	unsigned char * pcBufRead = NULL;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead;
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle[MAX_DEVICES];
	int	iNumDevs = 0;
	int	i, j;
	int	iDevicesOpen;	
	
	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	pcBufLD[MAX_DEVICES] = NULL;
	
	ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
	
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices(%d)\n", (int)ftStatus);
		return 1;
	}

	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}

	for(j = 0; j < BUF_SIZE; j++) {
		cBufWrite[j] = j;
	}
	
	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ) ; i++) {
		/* Setup */
		if((ftStatus = FT_OpenEx(cBufLD[i], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle[i])) != FT_OK){
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error FT_OpenEx(%d), device %d\n", (int)ftStatus, i);
			printf("Use lsmod to check if ftdi_sio (and usbserial) are present.\n");
			printf("If so, unload them using rmmod, as they conflict with ftd2xx.\n");
			return 1;
		}
	
		printf("Opened device %s\n", cBufLD[i]);

		iDevicesOpen++;
		if((ftStatus = FT_SetBaudRate(ftHandle[i], 9600)) != FT_OK) {
			printf("Error FT_SetBaudRate(%d), cBufLD[i] = %s\n", (int)ftStatus, cBufLD[i]);
			break;
		}

		printf("Calling FT_Write with this write-buffer:\n");
		dumpBuffer(cBufWrite, BUF_SIZE);
		
		/* Write */
		ftStatus = FT_Write(ftHandle[i], cBufWrite, BUF_SIZE, &dwBytesWritten);
		if (ftStatus != FT_OK) {
			printf("Error FT_Write(%d)\n", (int)ftStatus);
			break;
		}
		if (dwBytesWritten != (DWORD)BUF_SIZE) {
			printf("FT_Write only wrote %d (of %d) bytes\n", 
			       (int)dwBytesWritten, 
			       BUF_SIZE);
			break;
		}
		sleep(1);
		
		/* Read */
		dwRxSize = 0;			
		while ((dwRxSize < BUF_SIZE) && (ftStatus == FT_OK)) {
			ftStatus = FT_GetQueueStatus(ftHandle[i], &dwRxSize);
		}
		if(ftStatus == FT_OK) {
			pcBufRead = realloc(pcBufRead, dwRxSize);
			memset(pcBufRead, 0xFF, dwRxSize);
			printf("Calling FT_Read with this read-buffer:\n");
			dumpBuffer(pcBufRead, dwRxSize);
			ftStatus = FT_Read(ftHandle[i], pcBufRead, dwRxSize, &dwBytesRead);
			if (ftStatus != FT_OK) {
				printf("Error FT_Read(%d)\n", (int)ftStatus);
				break;
			}
			if (dwBytesRead != dwRxSize) {
				printf("FT_Read only read %d (of %d) bytes\n",
				       (int)dwBytesRead,
				       (int)dwRxSize);
				break;
			}
			printf("FT_Read read %d bytes.  Read-buffer is now:\n",
			       (int)dwBytesRead);
			dumpBuffer(pcBufRead, (int)dwBytesRead);
			if (0 != memcmp(cBufWrite, pcBufRead, BUF_SIZE)) {
				printf("Error: read-buffer does not match write-buffer.\n");
				break;
			}
			printf("%s test passed.\n", cBufLD[i]);
		}
		else {
			printf("Error FT_GetQueueStatus(%d)\n", (int)ftStatus);	
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
