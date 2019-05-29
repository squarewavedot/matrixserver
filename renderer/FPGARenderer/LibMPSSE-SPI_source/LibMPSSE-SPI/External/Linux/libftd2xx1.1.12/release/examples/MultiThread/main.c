/*
	A Simple multithreaded application that continually watches the read queue 
	until the application exits.
	
gcc -o multi main.c -lpthread -lftd2xx -Wl,-rpath /usr/local/lib
	
*/
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include "../ftd2xx.h"

FT_HANDLE ftHandle;
int exit_reader = 0;

/*
	This is the main reader thread - continually waits for a read

*/
void *read_watch(void *pArgs)
{
	char buf[20];
	DWORD dwRet;
	FT_STATUS ftStatus;
		
	(void)pArgs; /* Deliberately unused parameter */

	while(exit_reader != 1) {
//		printf("Write\n");
		ftStatus = FT_Write(ftHandle, buf, 20, &dwRet);
		if (ftStatus != FT_OK)
			printf("Error: FT_Write returned %d\n", (int)ftStatus);
//		sleep(1);	
	}
	(void)FT_Close(ftHandle);
	
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t thread_id;
	int i;
	FT_STATUS ftStatus;
	
	(void)argc; /* Deliberately unused parameter */
	(void)argv; /* Deliberately unused parameter */

	exit_reader = 0;
	
	ftStatus = FT_Open(0, &ftHandle);
	pthread_create(&thread_id, NULL, &read_watch, NULL);
	
	for(i = 0; i < 1000; i++) {
		char buf[100];
		DWORD index = 0;
		
		ftStatus = FT_ListDevices((PVOID)(uintptr_t)index, 
		                          (void*)buf,
		                          FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
		if(ftStatus == FT_OK) {
//			printf("FT_LIST_BY_INDEX = %s\n");
		}
		else {
			printf("FT_LIST_BY_INDEX failed(%d)\n", (int)ftStatus);
		}
	}
	
	exit_reader = 1;
	
	/* wait for it to exit */
	pthread_join(thread_id, NULL);
	
	return 0;
}



