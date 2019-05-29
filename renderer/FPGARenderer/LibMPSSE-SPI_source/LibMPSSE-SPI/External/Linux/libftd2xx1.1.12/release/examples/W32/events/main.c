/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o w32_events main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../../ftd2xx.h"
#include <pthread.h>

#define BUF_SIZE 1000

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

void *read_watch(void *pArgs)
{
	DWORD 	dwBytesWritten;
	char 	cBufWrite[BUF_SIZE];
	FT_HANDLE	ftHandle = (FT_HANDLE)pArgs;
		
	sleep(1);
	printf("FT_W32_WriteFile\n");
	FT_W32_WriteFile(ftHandle, cBufWrite, BUF_SIZE, &dwBytesWritten, NULL);
	printf("FT_W32_WriteFile done\n");
}

int main(int argc, char *argv[])
{
	pthread_t thread_id;
	int iTestTXEmpty;
	char 	cBufWrite[BUF_SIZE];
	char * 	pcBufRead = NULL;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead, dwStat;
	FT_STATUS	ftStatus;
	int	iNumDevs = 0;
	int	i, j;
	int	iDevicesOpen;
	DWORD dwMask, dwErrors;
	FTDCB ftDCB, ftnewDCB;
	FTCOMSTAT ftComstat;

	if(argc > 1) {
		sscanf(argv[1], "%d", &iTestTXEmpty);
	}
	else {
		iTestTXEmpty = 0;
	}
	
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
	
		ftHandle[i] = FT_W32_CreateFile(
						cBufLD[i],
						0,		// GENERIC_READ|GENERIC_WRITE ignored
						0,		// ignored
						0, 		// ignored
						0,		// OPEN_EXISTING ignored
						FT_OPEN_BY_SERIAL_NUMBER, // FT_OPEN_BY_SERIAL_NUMBER,
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

		iDevicesOpen++;
		if(FT_W32_GetCommState(ftHandle[i], &ftDCB) == FALSE) {
			ftStatus = FT_W32_GetLastError(ftHandle[i]);
			printf("Error FT_W32_GetCommState = %d)\n", ftStatus);
			return 1;
		}	
			
		ftDCB.BaudRate = 9600;		
		ftDCB.EvtChar = '2';
		
		if(FT_W32_SetCommState(ftHandle[i], &ftDCB) == FALSE) {
			ftStatus = FT_W32_GetLastError(ftHandle[i]);
			printf("Error FT_W32_SetCommState = %d)\n", ftStatus);
			return 1;
		}	
		
		FT_SetRts(ftHandle[i]);	
		
		for(;;) {
			FT_W32_SetCommMask(ftHandle[i], (EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY));  
			
			if(iTestTXEmpty == 1) {
				pthread_create(&thread_id, NULL, read_watch, ftHandle[i]);
			}
			
			printf("Before wait Comm Event\n");
			FT_W32_WaitCommEvent(ftHandle[i], &dwMask, NULL);
			if (dwMask & EV_BREAK) {
				printf("EV_BREAK Event detected\n");
			}
			if (dwMask & EV_CTS) {
				printf("EV_CTS Event detected\n");
				FT_W32_GetCommModemStatus(ftHandle[i], &dwStat);
				if(dwStat & MS_CTS_ON) {
					printf("CTS is on\n");	// OK
				}
				else {
					printf("CTS is off\n");	// OK
				}
			}
			if (dwMask & EV_DSR) {
				printf("EV_DSR Event detected\n");
				FT_W32_GetCommModemStatus(ftHandle[i], &dwStat);
				if(dwStat & MS_DSR_ON) {
					printf("DSR is on\n");	// OK
				}
				else {
					printf("DSR is off\n");	// OK
				}
			}
			if (dwMask & EV_ERR) {	// OK
				FT_W32_ClearCommError(ftHandle[i], &dwErrors, &ftComstat);
				printf("EV_ERR Event detected 0x%X!\n", dwErrors);
				if (dwErrors & CE_BREAK) 	// OK
					printf("BREAK condition detected\n");					
				if (dwErrors & CE_FRAME) 	// OK
					printf("FRAME condition detected\n");
				if (dwErrors & CE_OVERRUN) 
					printf("OVERRUN condition detected\n");
				if (dwErrors & CE_RXPARITY) 
					printf("RXPARITY condition detected\n");
			}
			if (dwMask & EV_RING) {
				printf("EV_RING Event detected\n");
				FT_W32_GetCommModemStatus(ftHandle[i], &dwStat);
				if(dwStat & MS_RING_ON) {
					printf("RING is on\n");	// OK
				}
				else {
					printf("RING is off\n"); // OK
				}
			}
			if (dwMask & EV_RLSD) {
				printf("EV_RLSD Event detected\n");
				FT_W32_GetCommModemStatus(ftHandle[i], &dwStat);
				if(dwStat & MS_RLSD_ON) {
					printf("RLSD is on\n");		// OK
				}
				else {
					printf("RLSD is off\n");	// OK
				}
			}
			if (dwMask & EV_RXCHAR) {
				printf("EV_RXCHAR Event detected\n");	// OK
			}
			if (dwMask & EV_RXFLAG) {
				printf("EV_RXFLAG Event detected\n");	// OK	
			}
			if (dwMask & EV_TXEMPTY) {
				printf("EV_TXEMPTY Event detected\n");	// OK
			}	
		}
	}
	iDevicesOpen = i;
	/* Cleanup */
	for(i = 0; i < iDevicesOpen; i++) {
		if(ftHandle[i] != NULL) {
			FT_W32_CloseHandle(ftHandle[i]);
			printf("Closed device %s\n", cBufLD[i]);
		}
	}

	if(pcBufRead)
		free(pcBufRead);
	return 0;
}
