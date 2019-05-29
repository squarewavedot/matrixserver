/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o erase main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

#include <stdio.h>
#include "../../ftd2xx.h"

#define BUF_SIZE 0x1000

int main(int argc, char *argv[])
{
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	int iport;
	
	if(argc > 1) {
		sscanf(argv[1], "%d", &iport);
	}
	else {
		iport = 0;
	}
	printf("opening port %d\n", iport);
	FT_SetVIDPID(0x0403, 0x6007);
	ftStatus = FT_Open(iport, &ftHandle0);
	if(ftStatus != FT_OK) {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(%d) failed\n", iport);
		return 1;
	}
	
	ftStatus = FT_EraseEE(ftHandle0);
	if(ftStatus != FT_OK) {
		printf("Erase failed\n");
	}
	
	FT_Close(ftHandle0);
	return 0;
}
