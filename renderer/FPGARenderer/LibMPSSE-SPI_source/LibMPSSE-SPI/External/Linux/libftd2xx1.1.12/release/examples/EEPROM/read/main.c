/*
	To build use the following gcc statement 
	(assuming you have the d2xx library in the /usr/local/lib directory).
	gcc -o read main.c -L. -lftd2xx -Wl,-rpath,/usr/local/lib
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../ftd2xx.h"

int main(int argc, char *argv[])
{
	FT_STATUS	ftStatus;
	FT_HANDLE	ftHandle0;
	int iport;
	static FT_PROGRAM_DATA Data;
	static FT_DEVICE ftDevice;
	DWORD libraryVersion = 0;
	int retCode = 0;

	ftStatus = FT_GetLibraryVersion(&libraryVersion);
	if (ftStatus == FT_OK)
	{
		printf("Library version = 0x%x\n", (unsigned int)libraryVersion);
	}
	else
	{
		printf("Error reading library version.\n");
		return 1;
	}
	
	if(argc > 1) {
		sscanf(argv[1], "%d", &iport);
	}
	else {
		iport = 0;
	}
	printf("Opening port %d\n", iport);
	
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
	
	printf("FT_Open succeeded.  Handle is %p\n", ftHandle0);

	ftStatus = FT_GetDeviceInfo(ftHandle0,
	                            &ftDevice,
	                            NULL,
	                            NULL,
	                            NULL,
	                            NULL); 
	if (ftStatus != FT_OK) 
	{ 
		printf("FT_GetDeviceType FAILED!\n");
		retCode = 1;
		goto exit;
	}  

	printf("FT_GetDeviceInfo succeeded.  Device is type %d.\n", 
	       (int)ftDevice);

	/* MUST set Signature1 and 2 before calling FT_EE_Read */
	Data.Signature1 = 0x00000000;
	Data.Signature2 = 0xffffffff;
	Data.Manufacturer = (char *)malloc(256); /* E.g "FTDI" */
	Data.ManufacturerId = (char *)malloc(256); /* E.g. "FT" */
	Data.Description = (char *)malloc(256); /* E.g. "USB HS Serial Converter" */
	Data.SerialNumber = (char *)malloc(256); /* E.g. "FT000001" if fixed, or NULL */
	if (Data.Manufacturer == NULL ||
	    Data.ManufacturerId == NULL ||
	    Data.Description == NULL ||
	    Data.SerialNumber == NULL)
	{
		printf("Failed to allocate memory.\n");
		retCode = 1;
		goto exit;
	}

	ftStatus = FT_EE_Read(ftHandle0, &Data);
	if(ftStatus != FT_OK) {
		printf("FT_EE_Read failed\n");
		retCode = 1;
		goto exit;
	}

	printf("FT_EE_Read succeeded.\n\n");
		
	printf("Signature1 = %d\n", (int)Data.Signature1);			
	printf("Signature2 = %d\n", (int)Data.Signature2);			
	printf("Version = %d\n", (int)Data.Version);				
								
	printf("VendorId = 0x%04X\n", Data.VendorId);				
	printf("ProductId = 0x%04X\n", Data.ProductId);
	printf("Manufacturer = %s\n", Data.Manufacturer);			
	printf("ManufacturerId = %s\n", Data.ManufacturerId);		
	printf("Description = %s\n", Data.Description);			
	printf("SerialNumber = %s\n", Data.SerialNumber);			
	printf("MaxPower = %d\n", Data.MaxPower);				
	printf("PnP = %d\n", Data.PnP) ;					
	printf("SelfPowered = %d\n", Data.SelfPowered);			
	printf("RemoteWakeup = %d\n", Data.RemoteWakeup);			

	if (ftDevice == FT_DEVICE_BM)
	{
		/* Rev4 (FT232B) extensions */
		printf("BM:\n");
		printf("---\n");
		printf("\tRev4 = 0x%X\n", Data.Rev4);					
		printf("\tIsoIn = 0x%X\n", Data.IsoIn);				
		printf("\tIsoOut = 0x%X\n", Data.IsoOut);				
		printf("\tPullDownEnable = 0x%X\n", Data.PullDownEnable);		
		printf("\tSerNumEnable = 0x%X\n", Data.SerNumEnable);			
		printf("\tUSBVersionEnable = 0x%X\n", Data.USBVersionEnable);		
		printf("\tUSBVersion = 0x%X\n", Data.USBVersion);
	}

	if (ftDevice == FT_DEVICE_2232C) 
	{
		/* Rev 5 (FT2232C) extensions */
		printf("2232RC:\n");
		printf("-------\n");
		printf("\tRev5 = 0x%X\n", Data.Rev5);					
		printf("\tIsoInA = 0x%X\n", Data.IsoInA);				
		printf("\tIsoInB = 0x%X\n", Data.IsoInB);				
		printf("\tIsoOutA = 0x%X\n", Data.IsoOutA);				
		printf("\tIsoOutB = 0x%X\n", Data.IsoOutB);				
		printf("\tPullDownEnable5 = 0x%X\n", Data.PullDownEnable5);		
		printf("\tSerNumEnable5 = 0x%X\n", Data.SerNumEnable5);		
		printf("\tUSBVersionEnable5 = 0x%X\n", Data.USBVersionEnable5);	
		printf("\tUSBVersion5 = 0x%X\n", Data.USBVersion5);			
		printf("\tAIsHighCurrent = 0x%X\n", Data.AIsHighCurrent);		
		printf("\tBIsHighCurrent = 0x%X\n", Data.BIsHighCurrent);		
		printf("\tIFAIsFifo = 0x%X\n", Data.IFAIsFifo);			
		printf("\tIFAIsFifoTar = 0x%X\n", Data.IFAIsFifoTar);			
		printf("\tIFAIsFastSer = 0x%X\n", Data.IFAIsFastSer);			
		printf("\tAIsVCP = 0x%X\n", Data.AIsVCP);				
		printf("\tIFBIsFifo = 0x%X\n", Data.IFBIsFifo);			
		printf("\tIFBIsFifoTar = 0x%X\n", Data.IFBIsFifoTar);			
		printf("\tIFBIsFastSer = 0x%X\n", Data.IFBIsFastSer);			
		printf("\tBIsVCP = 0x%X\n", Data.BIsVCP);
	}

	if (ftDevice == FT_DEVICE_232R)
	{
		/* Rev 6 (FT232R) extensions */
		printf("232R:\n");
		printf("-----\n");
		printf("\tUseExtOsc = 0x%X\n", Data.UseExtOsc);			// Use External Oscillator
		printf("\tHighDriveIOs = 0x%X\n", Data.HighDriveIOs);			// High Drive I/Os
		printf("\tEndpointSize = 0x%X\n", Data.EndpointSize);			// Endpoint size

		printf("\tPullDownEnableR = 0x%X\n", Data.PullDownEnableR);		// non-zero if pull down enabled
		printf("\tSerNumEnableR = 0x%X\n", Data.SerNumEnableR);		// non-zero if serial number to be used

		printf("\tInvertTXD = 0x%X\n", Data.InvertTXD);			// non-zero if invert TXD
		printf("\tInvertRXD = 0x%X\n", Data.InvertRXD);			// non-zero if invert RXD
		printf("\tInvertRTS = 0x%X\n", Data.InvertRTS);			// non-zero if invert RTS
		printf("\tInvertCTS = 0x%X\n", Data.InvertCTS);			// non-zero if invert CTS
		printf("\tInvertDTR = 0x%X\n", Data.InvertDTR);			// non-zero if invert DTR
		printf("\tInvertDSR = 0x%X\n", Data.InvertDSR);			// non-zero if invert DSR
		printf("\tInvertDCD = 0x%X\n", Data.InvertDCD);			// non-zero if invert DCD
		printf("\tInvertRI = 0x%X\n", Data.InvertRI);				// non-zero if invert RI

		printf("\tCbus0 = 0x%X\n", Data.Cbus0);				// Cbus Mux control
		printf("\tCbus1 = 0x%X\n", Data.Cbus1);				// Cbus Mux control
		printf("\tCbus2 = 0x%X\n", Data.Cbus2);				// Cbus Mux control
		printf("\tCbus3 = 0x%X\n", Data.Cbus3);				// Cbus Mux control
		printf("\tCbus4 = 0x%X\n", Data.Cbus4);				// Cbus Mux control

		printf("\tRIsD2XX = 0x%X\n", Data.RIsD2XX); // non-zero if using D2XX
	}	

	
exit:
	free(Data.Manufacturer);
	free(Data.ManufacturerId);
	free(Data.Description);
	free(Data.SerialNumber);
	FT_Close(ftHandle0);
	printf("Returning %d\n", retCode);
	return retCode;
}
