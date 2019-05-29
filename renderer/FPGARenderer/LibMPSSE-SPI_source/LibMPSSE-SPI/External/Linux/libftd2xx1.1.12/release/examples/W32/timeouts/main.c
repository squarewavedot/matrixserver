/*
	Simple example to open demonstrate the W32 FT_W32_Set/GetCommTimouts functions

	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o w32_timouts main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include "../../ftd2xx.h"

#define BUF_SIZE 0x11

#define MAX_DEVICES		5

// Globals
FT_HANDLE ftHandle[MAX_DEVICES];

void quit()
{  
	int i;

	for(i = 0; i < MAX_DEVICES; i++) {
		if(ftHandle[i] != NULL) {
			FT_W32_CloseHandle(ftHandle[i]);
			ftHandle[i] = NULL;
			printf("Closed device\n");
		}
	}

	exit(1);
}

int main()
{
	char 	cBufWrite[BUF_SIZE];
	char * 	pcBufRead = NULL;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead, dwErrors;
	FT_STATUS	ftStatus;
	int	iNumDevs = 0;
	int	i, j;
	int	iDevicesOpen;
	FTDCB ftDCB, ftnewDCB;
	FTCOMSTAT ftComStat;
	FTTIMEOUTS ftTimouts, ftnewTimouts;
	struct timeval tv;
	struct timezone	tz;
	
	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
		ftHandle[i] = NULL;
	}
	pcBufLD[MAX_DEVICES] = NULL;
	
	ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
	
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices(%d)\n", ftStatus);
		return 1;
	}
	for(j = 0; j < BUF_SIZE; j++) {
		cBufWrite[j] = j;
	}
	
	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}

    signal(SIGINT, quit);		// trap ctrl-c call quit fn 
		
	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ) ; i++) {
	
		memset(&ftDCB, 0, sizeof(FTDCB));
		
		ftHandle[i] = FT_W32_CreateFile(
						cBufLD[i],
						0,		// GENERIC_READ|GENERIC_WRITE ignored
						0,		// ignored
						0, 		// ignored
						0,		// OPEN_EXISTING ignored
						FT_OPEN_BY_SERIAL_NUMBER, // ignored FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FT_OPEN_BY_SERIAL_NUMBER,
						0		// ignored
						);
		/* Setup */
		if(ftHandle[i] == (FT_HANDLE)INVALID_HANDLE_VALUE){
			/* 
				This can fail if the ftdi_sio driver is loaded
		 		use lsmod to check this and rmmod ftdi_sio to remove
				also rmmod usbserial
		 	*/
			printf("Error FT_W32_CreateFile(%d), device\n", i);
			return 1;
		}
	
		printf("Opened device %s\n", cBufLD[i]);
		
		ftDCB.BaudRate = 9600;		
		
		if(FT_W32_SetCommState(ftHandle[i], &ftDCB) == FALSE) {
			ftStatus = FT_W32_GetLastError(ftHandle[i]);
			printf("Error FT_W32_SetCommState = %d)\n", ftStatus);
			return 1;
		}
		
		if(FT_W32_GetCommState(ftHandle[i], &ftnewDCB) == FALSE) {
			ftStatus = FT_W32_GetLastError(ftHandle[i]);
			printf("Error FT_W32_GetCommState = %d)\n", ftStatus);
			return 1;
		}
		
		if(ftnewDCB.BaudRate != ftDCB.BaudRate) {
			printf("New baud rate does not match baud rate set\n");
		}

		ftTimouts.ReadIntervalTimeout = 0;			// ignored by library
		ftTimouts.ReadTotalTimeoutMultiplier = 0;	// ignored by library
		ftTimouts.ReadTotalTimeoutConstant = 1000;	// 1 second
		ftTimouts.WriteTotalTimeoutMultiplier = 0;	// ignored by library
		ftTimouts.WriteTotalTimeoutConstant = 1000;	// 1 second
		
		// 1 second for both read and write		
		if(FT_W32_SetCommTimeouts(ftHandle[i], &ftTimouts) == FALSE) {
			ftStatus = FT_W32_GetLastError(ftHandle[i]);
			printf("Error FT_W32_GetCommState = %d)\n", ftStatus);
			return 1;
		}
		
		if(FT_W32_GetCommTimeouts(ftHandle[i], &ftnewTimouts) == FALSE) {
			ftStatus = FT_W32_GetLastError(ftHandle[i]);
			printf("Error FT_W32_GetCommState = %d)\n", ftStatus);
			return 1;
		}
		else {
			printf("ftTimouts.ReadIntervalTimeout = %d\n", ftTimouts.ReadIntervalTimeout);
			printf("ftTimouts.ReadTotalTimeoutMultiplier = %d\n", ftTimouts.ReadTotalTimeoutMultiplier);
			printf("ftTimouts.ReadTotalTimeoutConstant = %d\n", ftTimouts.ReadTotalTimeoutConstant);
			printf("ftTimouts.WriteTotalTimeoutMultiplier = %d\n", ftTimouts.WriteTotalTimeoutMultiplier);
			printf("ftTimouts.WriteTotalTimeoutConstant = %d\n", ftTimouts.WriteTotalTimeoutConstant);
		}
	
		
		for(j = 0 ; j < 10; j++) {	
			if(FT_W32_ReadFile(ftHandle[i], cBufWrite, BUF_SIZE, &dwBytesRead, NULL) == FALSE) {
				ftStatus = FT_W32_GetLastError(ftHandle[i]);
				gettimeofday(&tv, &tz);
				printf("%d secs, %d usecs\n", tv.tv_sec, tv.tv_usec);
//				printf("Error FT_W32_ReadFile = %d)\n", ftStatus);
//				return 1;
			}
			else if(dwBytesRead != BUF_SIZE) {
				gettimeofday(&tv, &tz);
				printf("%d secs, %d usecs\n", tv.tv_sec, tv.tv_usec);
			}
		}
		
		printf("dwBytesRead = %d\n", dwBytesRead);

	}
	iDevicesOpen = i;
	/* Cleanup */
	for(i = 0; i < iDevicesOpen; i++) {
		if(ftHandle[i] != NULL) {
			if(FT_W32_CloseHandle(ftHandle[i]) == FALSE) {
				ftStatus = FT_W32_GetLastError(ftHandle[i]);
				printf("Error FT_W32_CloseHandle = %d, device = %d)\n", ftStatus, i);
			}
		}
		printf("Closed device %s\n", cBufLD[i]);
	}

	if(pcBufRead)
		free(pcBufRead);
	return 0;
}
