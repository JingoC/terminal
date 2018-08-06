#include "com.h"

#include <windows.h>
#include <stdio.h>

HANDLE hSerial;

#define COM_BUFFER_SIZE			256

struct
{
	char comBuffer[COM_BUFFER_SIZE];
	int size;
}COM;

bool COM_Init(const char* comName)
{
	COM.size = 0;
	
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

void COM_UpdateBuffer()
{
	if (COM.size < (COM_BUFFER_SIZE - 1))
	{
		DWORD size = 0;
		char c;
		ReadFile(hSerial, &c, 1, &size, 0);
	
		COM.comBuffer[COM.size] = c;
		COM.size++;
	}
}

bool COM_IsNotEmpty()	
{ 
	COM_UpdateBuffer();
	return COM.size > 0; 
}

char COM_GetChar()
{
	char ret = 0;
	if (COM.size > 0)
	{
		ret = COM.comBuffer[0];
		
		for(int i = 0; i < COM.size - 1; i++)
		{
			COM.comBuffer[i] = COM.comBuffer[i+1];
		}
		COM.size--;
	}
	return ret;
}
