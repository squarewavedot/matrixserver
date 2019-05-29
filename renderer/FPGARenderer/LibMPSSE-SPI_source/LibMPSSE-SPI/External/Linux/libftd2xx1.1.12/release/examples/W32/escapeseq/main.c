/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o w32_escapeseq main.c -L. -lftd2xx
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "../../ftd2xx.h"

#define BUF_SIZE 0x11

#define MAX_DEVICES		5

//Globals
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

int main(int argc, char *argv[])
{
	char 	cBufWrite[BUF_SIZE];
	char * 	pcBufRead = NULL;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD	dwRxSize = 0;
	DWORD 	dwBytesWritten, dwBytesRead;
	FT_STATUS	ftStatus;
	int	iNumDevs = 0;
	int	i, j;
	int	iDevicesOpen;
	FTDCB ftDCB;
		
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
		/***************************************************************/		
		printf("About to set DTR press return to set");
		getchar();
	
		if(FT_W32_EscapeCommFunction(ftHandle[i], SETDTR) == FALSE) {
			printf("Error setting DTR\n");
		}
		else {
			printf("Set DTR OK\n");
		}
		
		/***************************************************************/		
		printf("About to clear DTR press return to set");
		getchar();
	
		if(FT_W32_EscapeCommFunction(ftHandle[i], CLRDTR) == FALSE) {
			printf("Error clearing DTR\n");
		}
		else {
			printf("Cleared DTR OK\n");
		}
		
		/***************************************************************/		
		printf("About to set RTS press return to set");
		getchar();
	
		if(FT_W32_EscapeCommFunction(ftHandle[i], SETRTS) == FALSE) {
			printf("Error setting RTS\n");
		}
		else {
			printf("Set RTS OK\n");
		}
		
		/***************************************************************/		
		printf("About to clear RTS press return to set");
		getchar();
	
		if(FT_W32_EscapeCommFunction(ftHandle[i], CLRRTS) == FALSE) {
			printf("Error clearing RTS\n");
		}
		else {
			printf("Cleared RTS OK\n");
		}
		
		/***************************************************************/		
		printf("About to set BREAK press return to set");
		getchar();
	
		if(FT_W32_EscapeCommFunction(ftHandle[i], SETBREAK) == FALSE) {
			printf("Error setting BREAK\n");
		}
		else {
			printf("Set BREAK OK\n");
		}
		
		/***************************************************************/		
		printf("About to clear BREAK press return to set");
		getchar();
	
		if(FT_W32_EscapeCommFunction(ftHandle[i], CLRBREAK) == FALSE) {
			printf("Error clearing BREAK\n");
		}
		else {
			printf("Clear BREAK OK\n");
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
