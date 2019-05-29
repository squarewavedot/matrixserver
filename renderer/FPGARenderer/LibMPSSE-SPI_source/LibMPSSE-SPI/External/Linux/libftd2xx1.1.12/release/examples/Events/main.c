/*
	Simple example to show the RX event. This example assumes you have a 
	hardware setup with the receive port using this sample code and another app 
	(such as hyperterm) is sending serial data to the receiving device.
		
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o events main.c -L. -lftd2xx -Wl,-rpath /usr/local/lib
*/

#include <stdio.h>
#include "../ftd2xx.h"

int main(int argc, char *argv[])
{
	DWORD dwBytesInQueue = 0;
	EVENT_HANDLE eh;
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle;
	int iport;
	
	if(argc > 1) {
		sscanf(argv[1], "%d", &iport);
	}
	else {
		iport = 0;
	}
	
	pthread_mutex_init(&eh.eMutex, NULL);
	pthread_cond_init(&eh.eCondVar, NULL);
	
	ftStatus = FT_Open(iport, &ftHandle);
	if(ftStatus != FT_OK) {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(%d) failed\n", iport);
		return 1;
	}
		
	ftStatus = FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
	if(ftStatus != FT_OK) {
		printf("Failed to set flow control\n");	
	}

	ftStatus = FT_SetEventNotification(ftHandle, FT_EVENT_RXCHAR, (PVOID)&eh);
	if(ftStatus != FT_OK) {
		printf("Failed to set events\n");
		return 1;
	}
	
	pthread_mutex_lock(&eh.eMutex);
	pthread_cond_wait(&eh.eCondVar, &eh.eMutex);
	pthread_mutex_unlock(&eh.eMutex);
	
	FT_GetQueueStatus(ftHandle, &dwBytesInQueue);
	printf("Received chars %d bytes in queue\n", (int)dwBytesInQueue);
	
	FT_Close(ftHandle);
	
	return 0;
}
