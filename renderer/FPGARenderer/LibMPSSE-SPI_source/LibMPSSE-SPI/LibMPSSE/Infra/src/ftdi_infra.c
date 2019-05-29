/*!
 * \file infra.c
 *
 * \author FTDI
 * \date 20110317
 *
 * Copyright © 2000-2014 Future Technology Devices International Limited
 *
 *
 * THIS SOFTWARE IS PROVIDED BY FUTURE TECHNOLOGY DEVICES INTERNATIONAL LIMITED ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL FUTURE TECHNOLOGY DEVICES INTERNATIONAL LIMITED
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Project: libMPSSE
 * Module: Infra
 *
 * Rivision History:
 * 0.1  - initial version
 * 0.2  - 20110708 - exported Init_libMPSSE & Cleanup_libMPSSE for Microsoft toolchain support
 * 0.3  - 20111103 - commented & cleaned up
 * 0.41 - 20140903 - fixed compile warnings 
 */


/******************************************************************************/
/*								Include files					  			  */
/******************************************************************************/
#include "ftdi_infra.h"		/*portable infrastructure(datatypes, libraries, etc)*/


/******************************************************************************/
/*								Macro defines					  			  */
/******************************************************************************/


#ifdef __linux
	#define GET_FUNC(libHandle,symbol)	dlsym(libHandle,symbol)
	/* Macro to check if dlsym returned a correctly */
	#define CHECK_SYMBOL(exp) {if(dlerror() != NULL)\
		{DBG(MSG_ERR,"Error getting symbol\n");}}
#else
	#define GET_FUNC(libHandle,symbol) GetProcAddress(libHandle,symbol)
	#define CHECK_SYMBOL(exp) CHECK_NULL(exp);
#endif



/******************************************************************************/
/*								Global variables							  */
/******************************************************************************/
#ifdef INFRA_DEBUG_ENABLE
	//int currentDebugLevel = MSG_INFO;
	int currentDebugLevel = MSG_DEBUG;
#endif

/* Handle to D2XX driver */
#ifdef _WIN32
	HANDLE hdll_d2xx;
#endif

#ifdef __linux__
	void *hdll_d2xx;
#endif

InfraFunctionPtrLst varFunctionPtrLst;


/******************************************************************************/
/*								Local function declarations					  */
/******************************************************************************/
#ifdef _MSC_VER
void my_init(void);/*called when lib is loaded*/
void my_exit(void);/*called when lib is unloaded*/
#else // _MSC_VER
void __attribute__ ((constructor))my_init(void);/*called when lib is loaded*/
void __attribute__ ((destructor))my_exit(void);/*called when lib is unloaded*/
#endif // _MSC_VER

/******************************************************************************/
/*						Global function definitions						  */
/******************************************************************************/

/*!
 * \brief Print function return status
 *
 * All the functions return a status code. This function prints a text to the debug terminal
 * that provides the meaning of the status code.
 *
 * \param[in] status Status code returned by functions
 * \return Returns status code of type FT_STATUS(see D2XX Programmer's Guide)
 * \sa
 * \note
 * \warning
 */
FT_STATUS Infra_DbgPrintStatus(FT_STATUS status)
{
	FN_ENTER;

	switch(status)
	{
		case FT_OK:
			DBG(MSG_ERR, "Status: FT_OK\n");
		break;

		case FT_INVALID_HANDLE:
			DBG(MSG_ERR, "Status: FT_INVALID_HANDLE\n");
		break;

		case FT_DEVICE_NOT_FOUND:
			DBG(MSG_ERR, "Status: FT_DEVICE_NOT_FOUND\n");
		break;

		case FT_DEVICE_NOT_OPENED:
			DBG(MSG_ERR, "Status: FT_DEVICE_NOT_OPENED\n");
		break;

		case FT_IO_ERROR:
			DBG(MSG_ERR, "Status: FT_IO_ERROR\n");
		break;

		case FT_INSUFFICIENT_RESOURCES:
			DBG(MSG_ERR, "Status: FT_INSUFFICIENT_RESOURCES\n");
		break;

		case FT_INVALID_PARAMETER:
			DBG(MSG_ERR, "Status: FT_INVALID_PARAMETER\n");
		break;

		case FT_INVALID_BAUD_RATE:
			DBG(MSG_ERR, "Status: FT_INVALID_BAUD_RATE\n");
		break;

		case FT_DEVICE_NOT_OPENED_FOR_ERASE:
			DBG(MSG_ERR, "Status: FT_DEVICE_NOT_OPENED_FOR_ERASE\n");
		break;

		case FT_DEVICE_NOT_OPENED_FOR_WRITE:
			DBG(MSG_ERR, "Status: FT_DEVICE_NOT_OPENED_FOR_WRITE\n");
		break;

		case FT_FAILED_TO_WRITE_DEVICE:
			DBG(MSG_ERR, "Status: FT_FAILED_TO_WRITE_DEVICE\n");
		break;

		case FT_EEPROM_READ_FAILED:
			DBG(MSG_ERR, "Status: FT_EEPROM_READ_FAILED\n");
		break;

		case FT_EEPROM_WRITE_FAILED:
			DBG(MSG_ERR, "Status: FT_EEPROM_WRITE_FAILED\n");
		break;

		case FT_EEPROM_ERASE_FAILED:
			DBG(MSG_ERR, "Status: FT_EEPROM_ERASE_FAILED\n");
		break;

		case FT_EEPROM_NOT_PRESENT:
			DBG(MSG_ERR, "Status: FT_EEPROM_NOT_PRESENT\n");
		break;

		case FT_EEPROM_NOT_PROGRAMMED:
			DBG(MSG_ERR, "Status: FT_EEPROM_NOT_PROGRAMMED\n");
		break;

		case FT_INVALID_ARGS:
			DBG(MSG_ERR, "Status: FT_INVALID_ARGS\n");
		break;

		case FT_NOT_SUPPORTED:
			DBG(MSG_ERR, "Status: FT_NOT_SUPPORTED\n");
		break;

		case FT_OTHER_ERROR:
			DBG(MSG_ERR, "Status: FT_OTHER_ERROR\n");
		break;

#ifndef __linux
/* gives compilation error in linux - not defined in D2XX for linux */
		case FT_DEVICE_LIST_NOT_READY:
			DBG(MSG_ERR, "Status: FT_DEVICE_LIST_NOT_READY\n");
#endif
		break;
			DBG(MSG_ERR, "Status: Unknown Error!\n");
		default:

			;

	}
	FN_EXIT;
	return FT_OK;
}


/******************************************************************************/
/*						Local function definitions						  */
/*!
 * \brief Delay the execution of the thread
 *
 * Delay the execution of the thread
 *
 * \param[in] delay Value of the delay in milliseconds
 * \return Returns status code of type FT_STATUS(see D2XX Programmer's Guide)
 * \sa
 * \note The macro INFRA_SLEEP has a resolution of 1 second
 * \warning
 */
FT_STATUS Infra_Delay(uint64 delay)
{
	FT_STATUS status=FT_OTHER_ERROR;
	FN_ENTER;
#ifdef _WIN32
/*TBD*/
	/*status = FT_OK;*/
#endif

#ifdef _LINUX
/*TBD*/
	/*status = FT_OK;*/
#endif

	FN_EXIT;
	return status;
}

/******************************************************************************/
/*						Local function definitions						  */
/******************************************************************************/

/*!
 * \brief This function initializes libMPSSE
 *
 * This function is called once when the library is loaded. It initializes all the modules in the
 * library. This function initializes all the variables and data structures that are required to be
 * initialized once only during loading.
 *
 * \param[in] none
 * \param[out] none
 * \return none
 * \sa
 * \note May individually call Ftdi_I2C_Module_Init, Ftdi_SPI_Module_Init, Ftdi_Mid_Module_Init,
 * Ftdi_Common_Module_Init, etc if required. This function should be called by the OS specific
 * function(eg: DllMain for windows) that is called by the OS automatically during startup.
 * \warning
 */
FTDI_API void Init_libMPSSE(void)
{
	//FT_STATUS status;
	FN_ENTER;

/* Load D2XX dynamic library */
#ifdef __linux
	hdll_d2xx = dlopen("libftd2xx.so",RTLD_LAZY);
	CHECK_NULL(hdll_d2xx);
#else
	hdll_d2xx = LoadLibraryA("ftd2xx.dll");
	CHECK_NULL(hdll_d2xx);
#endif

	varFunctionPtrLst.p_FT_GetLibraryVersion = (pfunc_FT_GetLibraryVersion)GET_FUNC(hdll_d2xx, "FT_GetLibraryVersion");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_GetLibraryVersion);
	/*FunctionPointer for FT_CreateDeviceInfoList*/
	varFunctionPtrLst.p_FT_GetNumChannel = (pfunc_FT_GetNumChannel)GET_FUNC(hdll_d2xx,"FT_CreateDeviceInfoList");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_GetNumChannel);
	/*function Pointer for FT_GetDeviceInfoList */
	varFunctionPtrLst.p_FT_GetDeviceInfoList = \
		(pfunc_FT_GetDeviceInfoList)GET_FUNC(hdll_d2xx,"FT_GetDeviceInfoList");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_GetDeviceInfoList);
	/*open*/
	varFunctionPtrLst.p_FT_Open = (pfunc_FT_Open)GET_FUNC(hdll_d2xx,"FT_Open");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_Open);
	/*close*/
	varFunctionPtrLst.p_FT_Close = (pfunc_FT_Close)GET_FUNC(hdll_d2xx,"FT_Close");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_Close);
	/*Reset*/
	varFunctionPtrLst.p_FT_ResetDevice = (pfunc_FT_ResetDevice)GET_FUNC(hdll_d2xx, "FT_ResetDevice");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_ResetDevice);
	/*Purge*/
	varFunctionPtrLst.p_FT_Purge = (pfunc_FT_Purge)GET_FUNC(hdll_d2xx,"FT_Purge");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_Purge);
	/*SetUSBParameters*/
	varFunctionPtrLst.p_FT_SetUSBParameters = (pfunc_FT_SetUSBParameters)GET_FUNC(hdll_d2xx,"FT_SetUSBParameters");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_SetUSBParameters);
	/*SetChars*/
	varFunctionPtrLst.p_FT_SetChars = (pfunc_FT_SetChars)GET_FUNC(hdll_d2xx,"FT_SetChars");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_SetChars);
	/*SetTimeouts*/
	varFunctionPtrLst.p_FT_SetTimeouts = (pfunc_FT_SetTimeouts)GET_FUNC(hdll_d2xx,"FT_SetTimeouts");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_SetTimeouts);
	/*SetLatencyTimer*/
    varFunctionPtrLst.p_FT_SetLatencyTimer = (pfunc_FT_SetLatencyTimer)GET_FUNC(hdll_d2xx,"FT_SetLatencyTimer");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_SetLatencyTimer);
	/*SetBitmode*/
	varFunctionPtrLst.p_FT_SetBitmode = (pfunc_FT_SetBitmode)GET_FUNC(hdll_d2xx,"FT_SetBitMode");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_SetBitmode);
	/*FT_GetQueueStatus*/
	varFunctionPtrLst.p_FT_GetQueueStatus = (pfunc_FT_GetQueueStatus)GET_FUNC(hdll_d2xx,"FT_GetQueueStatus");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_GetQueueStatus);
	/*FT_Read*/
	varFunctionPtrLst.p_FT_Read = (pfunc_FT_Read)GET_FUNC(hdll_d2xx,"FT_Read");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_Read);
	/*FT_Write*/
	varFunctionPtrLst.p_FT_Write = (pfunc_FT_Write)GET_FUNC(hdll_d2xx,"FT_Write");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_Write);
	/*FT_GetDeviceInfo*/
	varFunctionPtrLst.p_FT_GetDeviceInfo = (pfunc_FT_GetDeviceInfo)GET_FUNC(hdll_d2xx,"FT_GetDeviceInfo");
	CHECK_SYMBOL(varFunctionPtrLst.p_FT_GetDeviceInfo);

	/*Call module specific initialization functions from here(if at all they are required)
		Example:
			Top_Init();	//This may be a function in ftdi_common.c
						// Inside this function we may have macros (eg: #ifdef(_I2C))

			Mid_Init();
	*/
	FN_EXIT;
}

/*!
 * \brief Cleans up the module before unloading
 *
 * This function frees all the resources that were allocated during initialization. It should be called
 * by the OS to ensure that the module exits gracefully
 *
 * \param[in] none
 * \param[out] none
 * \return none
 * \sa
 * \note
 * \warning
 */
FTDI_API void Cleanup_libMPSSE(void)
{
	//FT_STATUS status=FT_OK;
	FN_ENTER;
#ifdef _WIN32
	if(NULL != hdll_d2xx)
	{
		if(FreeLibrary(hdll_d2xx)>0)
		{
			DBG(MSG_DEBUG, "D2XX unloaded\n");
		}
		else
		{
			DBG(MSG_DEBUG, "failed unloading D2XX\n");
		}
	}
	else
	{
		DBG(MSG_INFO, "handle to D2XX is NULL\n");
	}
#endif

#ifdef __linux
    dlclose(hdll_d2xx);
#endif

	FN_EXIT;
}


#ifdef _WIN32

/*!
 * \brief Module entry point for Windows DLL
 *
 * This function is called by Windows OS when an application loads/unloads libMPSSE as a DLL
 *
 * \param[in] hModule			Handle
 * \param[in] reason_for_call	Reason for being called
 * \param[in] lpReserved		Reserved
 * \return none
 * \sa
 * \note
 * \warning
 */
BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD reason_for_call,LPVOID lpReserved)
{
	//FT_STATUS status=FT_OK;
	FN_ENTER;

    switch (reason_for_call)
  	{
		case DLL_PROCESS_ATTACH:
			DBG(MSG_DEBUG,"reason_for_call = DLL_PROCESS_ATTACH\n");
			Init_libMPSSE();
		break;
		case DLL_THREAD_ATTACH:
			DBG(MSG_DEBUG,"reason_for_call = DLL_THREAD_ATTACH\n");

      	break;

		case DLL_THREAD_DETACH:
			DBG(MSG_DEBUG,"reason_for_call = DLL_THREAD_DETACH\n");

		break;
		case DLL_PROCESS_DETACH:
			DBG(MSG_DEBUG,"reason_for_call = DLL_PROCESS_DETACH\n");
			Cleanup_libMPSSE();
		break;

		default:
			DBG(MSG_WARN,"DllMain was called with an unknown reason\n");
    }

	FN_EXIT;
    return TRUE;
}
#endif /*_WIN32*/



/*!
 * \brief Module entry point for Windows Static Library and Linux
 * Dynamic & Static Libraries
 *
 * This function is the entry point for the module when it is loaded as a
 * static library(libMPSSE.lib or libMPSSE.a) in windows, and when it is loaded
 * as either a static library(libMPSSE.a) or a dynamic library(libMPSSE.so) in
 * linux
 *
 * \param[in] none
 * \param[out] none
 * \return none
 * \sa
 * \note
 * \warning
 */
void my_init(void)
{
	//FT_STATUS status=FT_OK;
	FN_ENTER;
	Init_libMPSSE();
	FN_EXIT;
}

/*!
 * \brief Module exit point for Windows Static Library and Linux
 * Dynamic & Static Libraries
 *
 * This function is the exit point for the module when it is loaded as a
 * static library(libMPSSE.lib or libMPSSE.a) in windows, and when it is loaded
 * as either a static library(libMPSSE.a) or a dynamic library(libMPSSE.so) in
 * linux
 *
 * \param[in] none
 * \param[out] none
 * \return none
 * \sa
 * \note
 * \warning
 */
void my_exit(void)
{
	//FT_STATUS status=FT_OK;
	FN_ENTER;
	Cleanup_libMPSSE();
	FN_EXIT;
}


/******************************************************************************/
/*						Public function definations						  */
/******************************************************************************/

/*!
 * \brief Test Function
 *
 * Returns D2XX version number
 *
 * \param[in]	i	dummy input
 * \param[out] *j	D2XX version number is returned here
 * \return Returns status code of type FT_STATUS(see D2XX Programmer's Guide)
 * \sa
 * \warning
 */
FTDI_API FT_STATUS CAL_CONV Infra_TestFunction(unsigned long i, unsigned long *j)
{
	FT_STATUS status=FT_OK;
	FN_ENTER;
	*j = i+100;
	//CHECK_NULL_RET(varFunctionPtrLst.p_FT_GetLibraryVersion);
	//varFunctionPtrLst.p_FT_GetLibraryVersion(j);

	FN_EXIT;
	return status;
}



