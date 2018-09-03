#ifndef _CLI_INPUT_H_
#define _CLI_INPUT_H_

#include "terminal_config.h"

typedef struct
{
	char keyCode;
	bool isValid;
	bool isAlphaBet;
}InputValue_s;

typedef enum
{
	MainBuffer		= 0x00,
	TransitBuffer	= 0x01
}InputBufferType_e;

void INPUT_Init();

char* INPUT_GetBuffer(InputBufferType_e type);
void INPUT_SetBuffer(InputBufferType_e type, char* buffer, uint32_t len);

bool INPUT_IsEmpty();
bool INPUT_IsFull();

void INPUT_RemChar();
void INPUT_AddChar(char c);

void INPUT_Cache();
void INPUT_Reset();

char INPUT_GetLastChar();

void INPUT_Refresh(const char* newCmd);

InputValue_s INPUT_PutChar(char c);

void INPUT_Delete();
void INPUT_Backspace();

void INPUT_CursorToHome();
void INPUT_CursorToEnd();
void INPUT_CursorTo(uint16_t pos);
void INPUT_CursorToLeft();
void INPUT_CursorToRight();

void INPUT_CursorShift(int16_t shift);

#endif // _CLI_INPUT_H_
