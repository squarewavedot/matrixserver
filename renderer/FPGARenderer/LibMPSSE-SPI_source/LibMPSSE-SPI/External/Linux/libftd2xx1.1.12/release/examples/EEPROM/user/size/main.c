/*
	UA size sample.

	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o sizeua main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib 
*/

#include <stdio.h>
#include <sys/time.h>
#include "../../../ftd2xx.h"

int main(int argc, char *argv[])
{
	DWORD 	dwUASize;
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
	ftStatus = FT_Open(iport, &ftHandle0);
	if(ftStatus == FT_OK) {
		printf("ftHandle0 = %p\n", (void *)ftHandle0);
	}
	else {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(%d) failed\n", iport);
		return 1;
	}
	
	ftStatus = FT_EE_UASize(ftHandle0, &dwUASize);
	if(ftStatus == FT_OK)
		printf("dwUASize = %d\n", (int)dwUASize);

	ftStatus = FT_Close(ftHandle0);

	return 0;
}
