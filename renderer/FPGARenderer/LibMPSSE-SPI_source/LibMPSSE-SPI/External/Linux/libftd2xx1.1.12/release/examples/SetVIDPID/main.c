/*
	A Simple example on the recommended way to use the FT_SetVIDPID/FT_GetVIDPID pair of
	functions to allow access to custom VID and PID devices.
	
	When using the FT_Open, FT_OpenEx and FT_Listdevices the D2XX library checks all of 
	the USB devices on the system and compares them against a table of predefined VIDs
	and PIDs. If your VID and PID is not included in this table then your device 
	cannot be opened.
	
	One method to allow your device to be used is the FT_SetVIDPID/FT_GetVIDPID pair of
	functions.
	
	This will not affect other applications on your system. However if you are using multiple 
	VID/PIDs in the one application you will need to do something like the following in order to 
	access each device. 
	
	If you experience or foresee problems related to this you can make your own device ID table in a
	separate library. You can use the example in lib_table directory to do this or, if you are not comfortable
	carrying out this proedure you can contact support2@ftdichip.com quoting "Linux SetVIDPID D2XX issue" where 
	we can compile you a separate table to be included on your system. If you want FTDI to perform the process it 
	would be appreciated if you could give as much notice as possible.
	
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o setVIDPID main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib 
*/

#include <stdio.h>
#include <sys/time.h>
#include "../ftd2xx.h"

#define BUF_SIZE 0x1000

#define MAX_DEVICES		4

int CheckConfigFile(DWORD * iVID, DWORD * iPID)
{
	FILE * fp;
	char cVID[5];
	char cPID[5];
	
	fp = fopen("myConfig.txt", "r");
	if(fp == NULL) {
		return 0;
	}
		
	fread(cVID, 1, 5, fp);	
	fread(cPID, 1, 5, fp);
	fclose(fp);
	
	cVID[4] = '\0';	
	cPID[4] = '\0';	
	
	sscanf(cVID, "%X\n", (unsigned int *)iVID);
	sscanf(cPID, "%X\n", (unsigned int *)iPID);
	
	return 1;
}

/*
	A wrapper function to allow safe listing of devices
*/
FT_STATUS my_FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD Flags)
{
	FT_STATUS	ftStatus;
	DWORD iVID, iPID;
	DWORD iOldVID, iOldPID;
	
	CheckConfigFile(&iVID, &iPID);		// get our VID and PID from config file or other source		
	FT_GetVIDPID(&iOldVID, &iOldPID);	// get original VID and PID	
	FT_SetVIDPID(iVID, iPID);							// use our VID and PID
	ftStatus = FT_ListDevices(pArg1, pArg2, Flags);		// Call FTDI function
	FT_SetVIDPID(iOldVID, iOldPID);						// restore original VID and PID
	
	return ftStatus;
}

/*
	A wrapper function to allow safe open of devices
*/
FT_STATUS my_FT_Open(int deviceNumber, FT_HANDLE *pHandle)
{
	FT_STATUS	ftStatus;
	DWORD iVID, iPID;
	DWORD iOldVID, iOldPID;
	
	CheckConfigFile(&iVID, &iPID);		// get our VID and PID from config file or other source
	FT_GetVIDPID(&iOldVID, &iOldPID);	// get original VID and PID
	FT_SetVIDPID(iVID, iPID);							// use our VID and PID
	ftStatus = FT_Open(deviceNumber, pHandle);
	FT_SetVIDPID(iOldVID, iOldPID);						// restore original VID and PID
	
	return ftStatus;	
}

/*
	A wrapper function to allow safe openex of devices
*/
FT_STATUS my_FT_OpenEx(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle)
{
	FT_STATUS	ftStatus;
	DWORD iVID, iPID;
	DWORD iOldVID, iOldPID;
	
	CheckConfigFile(&iVID, &iPID);		// get our VID and PID from config file or other source
	FT_GetVIDPID(&iOldVID, &iOldPID);	// get original VID and PID
	FT_SetVIDPID(iVID, iPID);							// use our VID and PID
	ftStatus = FT_OpenEx(pArg1, Flags, pHandle);
	FT_SetVIDPID(iOldVID, iOldPID);						// restore original VID and PID	
	
	return ftStatus;	
}

int main(int argc, char *argv[])
{
	char 	cBufWrite[BUF_SIZE];
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];
	DWORD 	dwBytesWritten;
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	int	i, iNumDevs;
	
	(void)argc; /* Deliberately unused parameter */
	(void)argv; /* Deliberately unused parameter */

	for(i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}
	pcBufLD[MAX_DEVICES] = NULL;
	
	ftStatus = my_FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION);
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices(%d)\n", (int)ftStatus);
		return 1;
	}
	
	for(i = 0; ( (i <MAX_DEVICES) && (i < iNumDevs) ); i++) {
		printf("Device %d Serial Number - %s\n", i, cBufLD[i]);
	}
	
	ftStatus = my_FT_Open(0, &ftHandle0);
	if(ftStatus != FT_OK) {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(0) failed\n");
		return 1;
	}
	ftStatus = FT_Write(ftHandle0, cBufWrite, BUF_SIZE, &dwBytesWritten);
	if(ftStatus != FT_OK) {
		printf("Error writing to device\n");
		return 1;
	}
	
	ftStatus = FT_Close(ftHandle0);

	return 0;
}
