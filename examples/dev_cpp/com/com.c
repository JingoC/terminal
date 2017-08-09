#include "com.h"

#include <windows.h>
#include <stdio.h>

HANDLE hSerial;
char dbgbuffer[256];

bool COM_Init(const char* comName)
{
	hSerial = CreateFile(comName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if(hSerial == INVALID_HANDLE_VALUE)
	{
	   	if(GetLastError() == ERROR_FILE_NOT_FOUND)
	    {
	        printf("serial port does not exist.\n");
	    }
	    else
	    {
	    	printf("some other error occurred.\n");
		}
		
		return false;
	}
	else
	{
		DCB dcbSerialParams = {0};
		dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
		if (!GetCommState(hSerial, &dcbSerialParams))
		{
		    printf("getting state error\n");
		    return false;
		}
		dcbSerialParams.BaudRate=CBR_115200;
		dcbSerialParams.ByteSize=8;
		dcbSerialParams.StopBits=ONESTOPBIT;
		dcbSerialParams.Parity=NOPARITY;
		if(!SetCommState(hSerial, &dcbSerialParams))
		{
		    printf("error setting serial port state\n");
		    return false;
		}
		
		printf("serial port \"COM1\"-115200-8-1 init\n");
	}
	
	return true;
}

void COM_Putc(char c)
{
	DWORD dwBytesWritten;
	WriteFile(hSerial, &c, 1, &dwBytesWritten, NULL);
}

void COM_Print(const char* str)
{
	uint16_t i = 0;
	while(str[i] != '\0')
	{
		COM_Putc(str[i]);
		i++;
	}
}

bool COM_Recieve(char *c)
{
	DWORD size = 0;
	ReadFile(hSerial, c, 1, &size, 0);
	return size > 0;
}
