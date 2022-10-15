#include "cli_input.h"

#include "lib/cli_queue.h"
#include "lib/cli_string.h"

extern uint32_t _strlen(const char* strSrc);

#define INPUT_COUNT_BUFFER		2

typedef struct
{
	char Data[TERM_CMD_BUF_SIZE + 1];		// buffer
	int16_t CursorInBuffer;					// cursos position
	int16_t BufferCount;					// count entered symbols
}Buffer_s;

struct
{
	Buffer_s Buffers[INPUT_COUNT_BUFFER];			// buffers commands
	Buffer_s* CurBuffer;
	Queue_s Symbols;								// queue symbols input
	InputBufferType_e CurrentBuffer;				// current proccesing buffer
	IInput InputService;
}Input;

static void _AddChar(char c)
{
	CLI_PutChar(c);
	
	Input.CurBuffer->Data[Input.CurBuffer->CursorInBuffer] = c;
	Input.CurBuffer->BufferCount++;
	Input.CurBuffer->CursorInBuffer++;
	Input.CurBuffer->Data[Input.CurBuffer->BufferCount] = '\0';
}

static void _RemChar()
{
	CLI_PutChar(TERM_KEY_BACKSPACE);
    CLI_PutChar(' ');
    CLI_PutChar(TERM_KEY_BACKSPACE);
    
	Input.CurBuffer->CursorInBuffer--;
	Input.CurBuffer->BufferCount--;
	Input.CurBuffer->Data[Input.CurBuffer->BufferCount] = '\0';
}

void INPUT_Refresh(const char* newCmd)
{
    CLI_PutChar('\r');
	CLI_Printf(STRING_TERM_ARROW);

	if (Input.CurBuffer->Data != newCmd)
	{
		uint32_t lenNewCmd = cli_strlen(newCmd);
		uint32_t lenCurCmd = Input.CurBuffer->BufferCount;
		cli_memcpy(Input.CurBuffer->Data, newCmd, lenNewCmd);
		
		Input.CurBuffer->BufferCount = lenNewCmd;
		Input.CurBuffer->CursorInBuffer = lenNewCmd;

		for(uint8_t i = 0; i < lenNewCmd; i++)
		{
			CLI_PutChar(Input.CurBuffer->Data[i]);
		}

		uint8_t cntSpcChar = 0;
		for(uint8_t i = lenNewCmd; i < lenCurCmd; i++)
		{
			CLI_PutChar(' ');
			cntSpcChar++;
		}
		
		for(uint8_t i = 0; i < cntSpcChar; i++)
        	{CLI_PutChar(TERM_KEY_BACKSPACE);}	
        
#if 0
		CLI_DPrintf("\r\nNewCmd: %s", newCmd);
		CLI_DPrintf("\r\nlenNewCmd: %d", lenNewCmd);
#endif

	}
	else
	{
		for(uint8_t i = 0; i < Input.CurBuffer->BufferCount; i++)
		{
			CLI_PutChar(Input.CurBuffer->Data[i]);
		}
	}
}

bool INPUT_IsEmpty()
{
	return Input.CurBuffer->BufferCount == 0;
}

bool INPUT_IsFull()
{
	return Input.CurBuffer->BufferCount >= TERM_CMD_BUF_SIZE;
}

void INPUT_RemChar()
{
	#if (TERM_LR_KEY_EN == 1)

	if (Input.CurBuffer->CursorInBuffer != Input.CurBuffer->BufferCount)
	{
		// save current position cursor
		uint8_t tmpPos = Input.CurBuffer->CursorInBuffer - 1;

		cli_memcpy(Input.Buffers[TransitBuffer].Data, Input.CurBuffer->Data, tmpPos);
		cli_memcpy(Input.Buffers[TransitBuffer].Data + tmpPos, Input.CurBuffer->Data + tmpPos + 1, Input.CurBuffer->BufferCount - tmpPos);
		
		Input.Buffers[TransitBuffer].Data[Input.CurBuffer->BufferCount - 1] = '\0';
		
		INPUT_Refresh(Input.Buffers[TransitBuffer].Data);

		for(uint8_t pos = 0; pos < Input.CurBuffer->BufferCount - tmpPos; pos++)
		{
            CLI_PutChar(TERM_KEY_LSHIFT);
			Input.CurBuffer->CursorInBuffer--;
		}
	}
	else
	{
        _RemChar();
	}
#else
    _RemChar();
#endif
}

void INPUT_AddChar(char c)
{
#if (TERM_LR_KEY_EN == 1)
	if (Input.CurBuffer->CursorInBuffer != Input.CurBuffer->BufferCount)
	{
		uint8_t tmpPos = Input.CurBuffer->CursorInBuffer;
		cli_memcpy(Input.Buffers[TransitBuffer].Data, Input.CurBuffer->Data, tmpPos);
		cli_memcpy(Input.Buffers[TransitBuffer].Data + tmpPos, &c, 1);
		cli_memcpy(Input.Buffers[TransitBuffer].Data + tmpPos + 1, Input.CurBuffer->Data + tmpPos, Input.CurBuffer->BufferCount - tmpPos);
		Input.Buffers[TransitBuffer].Data[Input.CurBuffer->BufferCount + 1] = '\0';
		
		Input.CurBuffer->BufferCount++;
		Input.CurBuffer->CursorInBuffer++;
		Input.CurBuffer->Data[Input.CurBuffer->BufferCount] = '\0';

		tmpPos++;
		INPUT_Refresh(Input.Buffers[TransitBuffer].Data);

		for(uint8_t pos = 0; pos < Input.CurBuffer->BufferCount - tmpPos; pos++)
		{
            CLI_PutChar(TERM_KEY_LSHIFT);
			Input.CurBuffer->CursorInBuffer--;
		}
	}
	else
	{
        _AddChar(c);
	}

#else
	_AddChar(c);
#endif	
}

uint8_t arr_up[]	= {0x1B, 0x5B, 0x41};
uint8_t arr_down[]	= {0x1B, 0x5B, 0x42};
uint8_t arr_right[] = {0x1B, 0x5B, 0x43};
uint8_t arr_left[]	= {0x1B, 0x5B, 0x44};
uint8_t arr_esc[]	= {0x1B, 0x1B, 0x1B};
uint8_t del[]		= {0x1B, 0x5B, 0x33};
uint8_t home[]		= {0x1B, 0x5B, 0x31};
uint8_t end[]		= {0x1B, 0x5B, 0x34};
	
InputValue_s INPUT_PutChar(char c)
{
	InputValue_s iv;
	
	Q_Push(&Input.Symbols, &c);
	
	if (Q_IsEqual(&Input.Symbols, arr_up, 3))			{c = TERM_KEY_UP;}
	else if (Q_IsEqual(&Input.Symbols, arr_down, 3))	{c = TERM_KEY_DOWN;}
	else if (Q_IsEqual(&Input.Symbols, arr_right, 3))	{c = TERM_KEY_RIGHT;}
	else if (Q_IsEqual(&Input.Symbols, arr_left, 3))	{c = TERM_KEY_LEFT;}
	else if (Q_IsEqual(&Input.Symbols, arr_esc, 3))		{c = TERM_KEY_ESCAPE;}
	else if (Q_IsEqual(&Input.Symbols, del, 3))			{c = TERM_KEY_DEL;}
	else if (Q_IsEqual(&Input.Symbols, home, 3))		{c = TERM_KEY_HOME;}
	else if (Q_IsEqual(&Input.Symbols, end, 3))			{c = TERM_KEY_END;}
	
	iv.isValid = ((Input.CurBuffer->BufferCount < TERM_CMD_BUF_SIZE) ||
						(c == TERM_KEY_BACKSPACE) ||
						(c == TERM_KEY_ENTER)	||
						(c == CHAR_INTERRUPT));
						
	iv.isAlphaBet = (((c >= '0') && (c <= '9')) ||
					((c >= 'a') && (c <= 'z')) ||
					((c >= 'A') && (c <= 'Z')) ||
					(c == 0x20) || (c == '_') || (c == '-'));
					
	iv.keyCode = c;
	return iv;
}

void INPUT_Cache()
{
	Input.CurBuffer->Data[Input.CurBuffer->BufferCount] = '\0';
	cli_memcpy(Input.Buffers[TransitBuffer].Data, Input.CurBuffer->Data, Input.CurBuffer->BufferCount + 1);
}

void INPUT_Reset()
{
	Input.CurBuffer->CursorInBuffer = Input.CurBuffer->BufferCount = 0;
	Input.CurBuffer->Data[Input.CurBuffer->BufferCount] = '\0';
}

char INPUT_GetLastChar()						{ return Input.CurBuffer->Data[Input.CurBuffer->BufferCount - 1];	}

void INPUT_CursorTo(uint16_t pos)				{ Input.CurBuffer->CursorInBuffer = pos; }

void INPUT_CursorShift(int16_t shift)			{ Input.CurBuffer->CursorInBuffer += shift; }

char* INPUT_GetBuffer(InputBufferType_e type)	{ return Input.Buffers[type].Data; }

void INPUT_SetBuffer(InputBufferType_e type, char* buffer, uint32_t len)
{
	cli_memcpy(Input.Buffers[type].Data, buffer, len);
	Input.CurBuffer->BufferCount = Input.CurBuffer->CursorInBuffer = len;
}

void INPUT_CursorToHome()
{
	while(Input.CurBuffer->CursorInBuffer > 0)
	{
		CLI_PutChar(TERM_KEY_LSHIFT);
		INPUT_CursorShift(-1);
	}
}

void INPUT_CursorToEnd()
{
	while(Input.CurBuffer->CursorInBuffer < Input.CurBuffer->BufferCount)
	{
		CLI_PutChar(Input.CurBuffer->Data[Input.CurBuffer->CursorInBuffer]);
		INPUT_CursorShift(1);
	}
}

void INPUT_CursorToLeft()
{
	if (Input.CurBuffer->CursorInBuffer > 0)
	{
		INPUT_CursorShift(-1);
        CLI_PutChar(TERM_KEY_LSHIFT);
	}
}

void INPUT_CursorToRight()
{
	if (Input.CurBuffer->CursorInBuffer < Input.CurBuffer->BufferCount)
	{
        CLI_PutChar(Input.CurBuffer->Data[Input.CurBuffer->CursorInBuffer]);
        INPUT_CursorShift(1);
	}	
}

void INPUT_Delete()
{
	if ((Input.CurBuffer->CursorInBuffer != Input.CurBuffer->BufferCount) && (!INPUT_IsEmpty()))
	{
		INPUT_CursorShift(1);
		if(Input.CurBuffer->CursorInBuffer != Input.CurBuffer->BufferCount)
		{
			CLI_PutChar(Input.CurBuffer->Data[Input.CurBuffer->CursorInBuffer - 1]);
		}
		INPUT_RemChar();
	}	
}

void INPUT_Backspace()
{
	if (!INPUT_IsEmpty() && (Input.CurBuffer->CursorInBuffer > 0))
		INPUT_RemChar();
}

IInput* Input_CreateInstance()
{
	for(uint32_t i = 0; i < INPUT_COUNT_BUFFER; i++)
	{
		Input.Buffers[i].Data[0] = '\0';
		Input.Buffers[i].BufferCount = 0;
		Input.Buffers[i].CursorInBuffer = 0;
	}
    
    Input.CurBuffer = &Input.Buffers[MainBuffer];
    
    Q_Init(&Input.Symbols, 3, sizeof(char), QUEUE_FORCED_PUSH_POP_Msk);
    
    for(uint8_t i = 0; i < 3; i++)
    {
    	char c = 0;
    	Q_Push(&Input.Symbols, &c);
	}

	IInput service = 
	{
		.GetBuffer = INPUT_GetBuffer,
		.SetBuffer = INPUT_SetBuffer,
		.IsEmpty = INPUT_IsEmpty,
		.IsFull = INPUT_IsFull,
		.RemChar = INPUT_RemChar,
		.AddChar = INPUT_AddChar,
		.Cache = INPUT_Cache,
		.Reset = INPUT_Reset,
		.GetLastChar = INPUT_GetLastChar,
		.Refresh = INPUT_Refresh,
		.PutChar = INPUT_PutChar,
		.Delete = INPUT_Delete,
		.Backspace = INPUT_Backspace,
		.CursorToHome = INPUT_CursorToHome,
		.CursorToEnd = INPUT_CursorToEnd,
		.CursorTo = INPUT_CursorTo,
		.CursorToLeft = INPUT_CursorToLeft,
		.CursorToRight = INPUT_CursorToRight,
		.CursorShift = INPUT_CursorShift
	};

	Input.InputService = service;

	return (IInput*) &Input.InputService;
}