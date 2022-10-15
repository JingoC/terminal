#include "com.h"

#include <windows.h>
#include <winbase.h>
#include <stdio.h>

#define COM_BUFFER_SIZE			256

struct
{
	HANDLE handle;
	struct
	{
		char buffer[COM_BUFFER_SIZE];
		int size;
		bool lock;
	}Fifo;
}COM;

static inline void _FIFO_Lock()			{ while(COM.Fifo.lock){} COM.Fifo.lock = true; }
static inline void _FIFO_Unlock()		{ COM.Fifo.lock = false; }
static inline bool _FIFO_IsFull()		{ return COM.Fifo.size == COM_BUFFER_SIZE; }
static inline bool _FIFO_IsEmpty()		{ return COM.Fifo.size == 0; }
static inline char _FIFO_Pop()			
{ 
	_FIFO_Lock();
	char c = COM.Fifo.buffer[0];
	for(int i = 0; i < COM.Fifo.size - 1; i++)
		COM.Fifo.buffer[i] = COM.Fifo.buffer[i + 1];
	COM.Fifo.size--;
	_FIFO_Unlock();
	return c;
}

static inline void _FIFO_Push(char c)
{
	_FIFO_Lock();
	COM.Fifo.buffer[COM.Fifo.size] = c;
	COM.Fifo.size++;
	_FIFO_Unlock();
}

static void _COM_UpdateBuffer()
{
	if (!_FIFO_IsFull())
	{
		DWORD size = 0;
		char c;
		
		ReadFile(COM.handle, &c, 1, &size, 0);
		
		if (size > 0)
			_FIFO_Push(c);
	}
}

static DWORD WINAPI _threadReadComPort(CONST LPVOID lpParam)
{
	while(true)
		_COM_UpdateBuffer();
	
	CloseHandle(COM.handle);
}

inline bool COM_IsNotEmpty()		{ return !_FIFO_IsEmpty(); }

inline char COM_GetChar()
{
	if (!_FIFO_IsEmpty())
		return _FIFO_Pop();
	
	return 0;
}

bool COM_Init(const char* comName)
{
	COM.handle = CreateFile(comName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	
	if(COM.handle != INVALID_HANDLE_VALUE)
	{
		DCB dcbSerialParams = {0};
		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
		
		if (!GetCommState(COM.handle, &dcbSerialParams))
		    return false;
		
		dcbSerialParams.BaudRate = CBR_115200;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;
		
		if(!SetCommState(COM.handle, &dcbSerialParams))
		    return false;
		
		// without timeout mode settings
		COMMTIMEOUTS CommTimeOuts={0xFFFFFFFF,0,0,0,1500};
		if (!SetCommTimeouts(COM.handle, &CommTimeOuts))
			return false;
			
		printf("serial port \"COM1\"-115200-8-1 init\n");
		
		COM.Fifo.size = 0;
		_FIFO_Unlock();

		CreateThread(NULL, 0, &_threadReadComPort, NULL, 0, NULL);

		return true;
	}
	
	return false;
}

void COM_Putc(char c)
{
	DWORD dwBytesWritten;
	WriteFile(COM.handle, &c, 1, &dwBytesWritten, NULL);
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
