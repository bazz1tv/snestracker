#include <stdio.h>
#include <stdlib.h>
#include <ftd2xx.h>
#include "common.h"

#include <signal.h>

static int keeprunning = 1;

void intHandler(int dummy) {
    keeprunning = 0;
}

// ...



int main(int argc, char *argv[])
{
	unsigned char pcBufRead;
	DWORD dwBytesRead;
	
	signal(SIGINT, intHandler);
	
	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;
	
	//printf ("DERP");
	ftStatus = FT_OpenEx((void*)"FT232R USB UART",FT_OPEN_BY_DESCRIPTION,&ftHandle);

	if (ftStatus != FT_OK)
	{
		printf("couldn't open port - error code: %d\n", (int)ftStatus);

		return -1;
	}
	
		//pcBufRead = (char *)malloc(BUF_SIZE);
	FT_ResetDevice(ftHandle);
	FT_Purge(ftHandle, FT_PURGE_TX | FT_PURGE_RX);
	FT_SetBaudRate(ftHandle,USART_BAUDRATE);

	if(FT_SetLatencyTimer(ftHandle, 2) != FT_OK)
	{
		return 1;
	}

	if(FT_SetDataCharacteristics(ftHandle,FT_BITS_8,FT_STOP_BITS_1,FT_PARITY_NONE) != FT_OK) {return 1;}

	// 10000110

	if(FT_SetTimeouts(ftHandle,1000,0) != FT_OK) { return 1; }

	
	
	

	
	
	while (keeprunning)
	{
		//		// Process has done i out of n rounds,
		// and we want a bar of width w and resolution r.
		// static inline void loadBar(int x, int n, int r, int w)
		//loadBar(TotalBytesRead, PAGESIZE*PAGES, (PAGESIZE*PAGES)/2, 50);
		ftStatus = FT_Read(ftHandle,&pcBufRead,1,&dwBytesRead);
		if (ftStatus == FT_OK)
		{
			if (dwBytesRead != 1)
			{
				//printf ("Didn't read correct amount of bytes?? : %d\n", dwBytesRead);
				//return -1;
			}
			else
				printf("0x%x : %d\n", pcBufRead, pcBufRead);
		}
		else return -1;
	}
			

	
	//free(pcBufRead);
	FT_Close(ftHandle);
	
	
	return 0;
}