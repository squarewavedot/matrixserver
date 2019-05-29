/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o write main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

#include <stdio.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

int main(int argc, char *argv[])
{
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	int iport;
	FT_PROGRAM_DATA Data;
	
	if(argc > 1) {
		sscanf(argv[1], "%d", &iport);
	}
	else {
		iport = 0;
	}
	printf("opening port %d\n", iport);
	FT_SetVIDPID(0x0403, 0x6011);
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

	printf("ftHandle0 = %p\n", ftHandle0);

#ifndef BM_DEVICE
	Data.Signature1 = 0x00000000;
	Data.Signature2 = 0xffffffff;
	Data.VendorId = 0x0403;				
	Data.ProductId = 0x6001;
	Data.Manufacturer =  "FTDI";
	Data.ManufacturerId = "FT";
	Data.Description = "USB <-> Serial";
	Data.SerialNumber = "FT000001";		// if fixed, or NULL
	
	Data.MaxPower = 44;
	Data.PnP = 1;
	Data.SelfPowered = 0;
	Data.RemoteWakeup = 1;
	Data.Rev4 = 1;
	Data.IsoIn = 0;
	Data.IsoOut = 0;
	Data.PullDownEnable = 1;
	Data.SerNumEnable = 1;
	Data.USBVersionEnable = 0;
	Data.USBVersion = 0x110;
#else // If 2232C	

	Data.Signature1 = 0x00000000;
	Data.Signature2 = 0xffffffff;
	Data.VendorId = 0x0403;				
	Data.ProductId = 0x6010;
	Data.Manufacturer =  "FTDI";
	Data.ManufacturerId = "FT";
	Data.Description = "SPI";
	Data.SerialNumber = "FT123452";		// if fixed, or NULL
	
	Data.MaxPower = 200;
	Data.PnP = 1;
	Data.SelfPowered = 0;
	Data.RemoteWakeup = 0;
	Data.Rev4 = 0;
	Data.IsoIn = 0;
	Data.IsoOut = 0;
	Data.PullDownEnable = 0;
	Data.SerNumEnable = 0;
	Data.USBVersionEnable = 0;
	Data.USBVersion = 0;

	Data.Rev5 = 1;					// non-zero if Rev5 chip, zero otherwise
	Data.IsoInA = 0;				// non-zero if in endpoint is isochronous
	Data.IsoInB = 0;				// non-zero if in endpoint is isochronous
	Data.IsoOutA = 0;				// non-zero if out endpoint is isochronous
	Data.IsoOutB = 0;				// non-zero if out endpoint is isochronous
	Data.PullDownEnable5 = 0;		// non-zero if pull down enabled
	Data.SerNumEnable5 = 1;			// non-zero if serial number to be used
	Data.USBVersionEnable5 = 0;		// non-zero if chip uses USBVersion
	Data.USBVersion5 = 0x0200;		// BCD (0x0200 => USB2)
	Data.AIsHighCurrent = 0;		// non-zero if interface is high current
	Data.BIsHighCurrent = 0;		// non-zero if interface is high current
	Data.IFAIsFifo = 1;				// non-zero if interface is 245 FIFO
	Data.IFAIsFifoTar = 0;			// non-zero if interface is 245 FIFO CPU target
	Data.IFAIsFastSer = 0;			// non-zero if interface is Fast serial
	Data.AIsVCP = 0;				// non-zero if interface is to use VCP drivers
	Data.IFBIsFifo = 1;				// non-zero if interface is 245 FIFO
	Data.IFBIsFifoTar = 0;			// non-zero if interface is 245 FIFO CPU target
	Data.IFBIsFastSer = 0;			// non-zero if interface is Fast serial
	Data.BIsVCP = 0;				// non-zero if interface is to use VCP drivers

#endif								

	ftStatus = FT_EE_Program(ftHandle0, &Data);
	if(ftStatus != FT_OK) {
		printf("FT_EE_Program failed (%d)\n", (int)ftStatus);
		FT_Close(ftHandle0);
	}
	FT_Close(ftHandle0);
	return 0;
}
