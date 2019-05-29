/*!
 * \file ftdi_infra.h
 *
 * \author FTDI
 * \date 20110127
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
 * This file abstracts everything that is specific to CPU, OS and Platform to ensure portability
 * These abstractions are datatypes, function calls, etc
 *
 * Rivision History:
 * 0.1  - initial version
 * 0.2  - 20110708 - added memory related macros
 * 0.3  - 20111103 - added 64bit linux support, cleaned up
 * 0.41 - 20140903 - fixed compile warnings
 *
 */

#ifndef FTDI_INFRA_H
#define FTDI_INFRA_H

/******************************************************************************/
/*								Macro defines								  */
/******************************************************************************/

#ifdef _WIN32
#include<windows.h>
#endif

#ifdef __linux
#include<dlfcn.h>	/*for dlopen() & dlsym()*/
#include<stdarg.h>	/*for va_start() & va_arg()*/
#include<unistd.h>	/*for Sleep()*/
#endif

#ifndef _MSC_VER
#include<inttypes.h>
#endif // _MSC_VER

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ftd2xx.h"


/******************************************************************************/
/*								Macro defines								  */
/******************************************************************************/

#define FTDI_EXPORTS


#ifdef FTDI_EXPORTS
	#ifdef _WIN32
		#define FTDI_API __declspec(dllexport)
	#else
		#define FTDI_API
	#endif
#else
	#define FTDI_API __declspec(dllimport)
#endif

/* Macro for calling convention, if required */
#ifdef _WIN32
	//#define CAL_CONV
	#define CAL_CONV	__stdcall
#else
	#define CAL_CONV
#endif

/* Uncomment the #define INFRA_DEBUG_ENABLE in makefile to enable debug messages */
#define MSG_EMERG	0 /*Used for emergency messages, usually those that precede a crash*/
#define MSG_ALERT	1 /*A situation requiring immediate action */
#define MSG_CRIT	2 /*Critical conditions, often related to serious hw/sw failures */
#define MSG_ERR		3 /*Used to report error conditions */
#define MSG_WARN	4 /*Warnings about problematic situations that do not, in themselves,
						create serious problems with the system */
#define MSG_NOTICE	5 /*Situations that are normal, but still worthy of note. */
#define MSG_INFO	6 /*Informational messages */
#define MSG_DEBUG	7 /*Used for debugging messages */

#ifdef INFRA_DEBUG_ENABLE
	extern int currentDebugLevel;
	#define DBG(level,...) do { if (level <= currentDebugLevel){ \
		printf("%s:%d:%s():",__FILE__, __LINE__, __FUNCTION__);	\
		printf(__VA_ARGS__); }} while (0)
#else
	#define DBG(level,fmt,...)	;
#endif

/* Macros to provide branch prediction information to compiler */
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

/* Macro to check null expression and exit if true */
#ifndef _MSC_VER
#define CHECK_NULL(exp){if(unlikely(exp==NULL)){printf("%s:%d:%s(): NULL \
expression encountered\n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

/* Macro to check null expression and return if true */
#define CHECK_NULL_RET(exp){if(unlikely(exp==NULL)){printf("%s:%d:%s(): NULL \
expression encountered\n",__FILE__, __LINE__, __FUNCTION__);\
status=FT_INVALID_PARAMETER ; return(status);}else{;}};

/* Macro to check status  code and only print debug message */
#define CHECK_STATUS_NORET(exp) {if(unlikely(exp!=FT_OK)){DBG(MSG_ERR," status \
!= FT_OK\n"); Infra_DbgPrintStatus(exp);}else{;}};

/* Macro to check status  code and return if not FT_OK */
#define CHECK_STATUS(exp) {if(unlikely(exp!=FT_OK)){DBG(MSG_ERR," status \
!= FT_OK\n");Infra_DbgPrintStatus(exp);return(exp);}else{;}};

#else // _MSC_VER

#define CHECK_NULL(exp){if(exp==NULL){printf("%s:%d:%s(): NULL \
expression encountered\n",__FILE__, __LINE__, __FUNCTION__);exit(1);}else{;}};

/* Macro to check null expression and return if true */
#define CHECK_NULL_RET(exp){if(exp==NULL){printf("%s:%d:%s(): NULL \
expression encountered\n",__FILE__, __LINE__, __FUNCTION__);\
status=FT_INVALID_PARAMETER ; return(status);}else{;}};

/* Macro to check status  code and only print debug message */
#define CHECK_STATUS_NORET(exp) {if(unlikely(exp!=FT_OK)){DBG(MSG_ERR," status \
!= FT_OK\n"); Infra_DbgPrintStatus(exp);}else{;}};

/* Macro to check status  code and return if not FT_OK */
#define CHECK_STATUS(exp) {if(exp!=FT_OK){DBG(MSG_ERR," status \
!= FT_OK\n");Infra_DbgPrintStatus(exp);return(exp);}else{;}};
#endif // _MSC_VER

#ifndef __BORLANDC__
	#define FN_ENTER	DBG(MSG_DEBUG,"Entering function\n");
	#define FN_EXIT		DBG(MSG_DEBUG,"Exiting function status=%u\n",(unsigned)status);
	#define FN_PT		DBG(MSG_DEBUG,"Path taken\n");
#else
	#define FN_ENTER	;
	#define FN_EXIT     ;
	#define FN_PT		;
#endif

/* sleep function abstraction */
#ifdef __linux__
	#define INFRA_SLEEP(exp)			usleep((exp)*1000);
#else
	#define INFRA_SLEEP(exp)			Sleep(exp);
#endif

/* Memory allocating, freeing & copying macros -  */
#define INFRA_MALLOC(exp)			malloc(exp); \
	DBG(MSG_DEBUG,"INFRA_MALLOC %ubytes\n",exp);
#define INFRA_FREE(exp)				free(exp); \
	DBG(MSG_DEBUG,"INFRA_FREE 0x%x\n",exp);
#define INFRA_MEMCPY(dest,src,siz)	memcpy(dest,src,siz);\
	DBG(MSG_DEBUG,"INFRA_MEMCPY dest:0x%x src:0x%x size:0x%x\n",dest,src,siz);


/******************************************************************************/
/*								Define platform								  */
/******************************************************************************/


/******************************************************************************/
/*								Type defines								  */
/******************************************************************************/

//#if(CPU == x86)
#if 1
	typedef unsigned char   uint8;
	typedef unsigned short  uint16;
	typedef unsigned long long uint64;

	typedef signed char   int8;
	typedef signed short  int16;
	typedef signed long long int64;

	typedef unsigned char	bool;

	#ifdef __x86_64__
		typedef unsigned int   uint32;
		typedef signed int   int32;
	#else
		typedef unsigned long   uint32;
		typedef signed long   int32;
	#endif
#endif

	typedef FT_STATUS (CAL_CONV *pfunc_FT_GetLibraryVersion)(LPDWORD \
		lpdwVersion);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_GetNumChannel)(LPDWORD \
		lpdwNumDevs);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_GetDeviceInfoList) \
		(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_Open) (int iDevice, \
		FT_HANDLE *ftHandle);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_Close) (FT_HANDLE ftHandle);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_ResetDevice) (FT_HANDLE ftHandle);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_Purge) (FT_HANDLE ftHandle, \
		DWORD dwMask);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_SetUSBParameters) (\
		FT_HANDLE ftHandle, DWORD dwInTransferSize, DWORD dwOutTransferSize);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_SetChars) (FT_HANDLE ftHandle, \
		UCHAR uEventCh, UCHAR uEventChEn, UCHAR uErrorCh, UCHAR uErrorChEn);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_SetTimeouts) (FT_HANDLE ftHandle, \
		DWORD dwReadTimeout, DWORD dwWriteTimeout);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_SetLatencyTimer) (FT_HANDLE ftHandle\
		, UCHAR ucTimer);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_SetBitmode) (FT_HANDLE ftHandle, \
		UCHAR ucMask, UCHAR ucMode);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_GetQueueStatus) (FT_HANDLE ftHandle,\
		LPDWORD lpdwAmountInRxQueue);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_Read) (FT_HANDLE ftHandle, \
		LPVOID lpBuffer, DWORD dwBytesToRead,LPDWORD lpdwBytesReturned);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_Write) (FT_HANDLE ftHandle, LPVOID \
		lpBuffer, DWORD dwBytesToWrite,LPDWORD lpdwBytesWritten);
	typedef FT_STATUS (CAL_CONV *pfunc_FT_GetDeviceInfo)(FT_HANDLE ftHandle, \
		FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR \
		Description, LPVOID Dummy);

typedef struct InfraFunctionPtrLst_t
{
	pfunc_FT_GetLibraryVersion p_FT_GetLibraryVersion;
	pfunc_FT_GetNumChannel p_FT_GetNumChannel;
	pfunc_FT_GetDeviceInfoList p_FT_GetDeviceInfoList;
	pfunc_FT_Open p_FT_Open;
	pfunc_FT_Close p_FT_Close;
	pfunc_FT_ResetDevice p_FT_ResetDevice;
	pfunc_FT_Purge p_FT_Purge;
	pfunc_FT_SetUSBParameters p_FT_SetUSBParameters;
	pfunc_FT_SetChars p_FT_SetChars;
	pfunc_FT_SetTimeouts p_FT_SetTimeouts;
	pfunc_FT_SetLatencyTimer p_FT_SetLatencyTimer;
	pfunc_FT_SetBitmode p_FT_SetBitmode;
	pfunc_FT_GetQueueStatus p_FT_GetQueueStatus;
	pfunc_FT_Read p_FT_Read;
	pfunc_FT_Write p_FT_Write;
	pfunc_FT_GetDeviceInfo p_FT_GetDeviceInfo;
}InfraFunctionPtrLst;


/******************************************************************************/
/*								External variables							  */
/******************************************************************************/
#ifdef _WIN32
	extern HANDLE hdll_d2xx;
#endif

#ifdef __linux__
	extern void *hdll_d2xx;
#endif

extern InfraFunctionPtrLst varFunctionPtrLst;




/******************************************************************************/
/*								Function declarations						  */
/******************************************************************************/
FT_STATUS Infra_DbgPrintStatus(FT_STATUS status);
FT_STATUS Infra_Delay(uint64 delay);



/******************************************************************************/


#endif	/*FTDI_INFRA_H*/


