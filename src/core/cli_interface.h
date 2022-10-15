#ifndef _CLI_INTERFACE_H_
#define _CLI_INTERFACE_H_

#include <stdint.h>

/// \brief Terminal Error Result Execute
typedef enum { TE_OK = 0, TE_Err, TE_NotFound, TE_ArgErr, TE_ExecErr, TE_WorkInt, } TE_Result_e;

/// \brief Terminal Add Command Result
typedef enum { TA_OK = 0, TA_MaxCmd, TA_FcnNull, TA_EmptyName, TA_RetryName, } TA_Result_e;

/// \brief Terminal Char Append Result
typedef enum { TC_OK = 0, TC_Enter, TC_BufFull, TC_Reset, TC_Ignore, } TC_Result_e;

typedef struct {
    // Start work terminal
	bool (*Execute)();
    // Add new command in terminal
	TA_Result_e (*AddCmd)(const char* name, uint8_t (*fcn)(), uint8_t argc, uint16_t mode, const char* descr);
    // Enter new char into terminal
	TC_Result_e (*EnterChar)(char c);
} ITerminal;

typedef struct {
    // Check exists argument flag
	bool (*IsArgFlag)(const char* flag);

    // Get argument dec value by flag
	bool (*GetArgDecByFlag)(const char* flag, uint32_t* outValue);
    // Get argument hex value by flag
	bool (*GetArgHexByFlag)(const char* flag, uint32_t* outValue);

    // Get argument dec value by index
	uint32_t (*GetArgDec)(uint8_t index);
    // Get argument hex value by index
	uint32_t (*GetArgHex)(uint8_t index);
	
	bool (*GetIntState)();
}ITerminalCommand;

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

typedef struct
{
	char* (*GetBuffer)(InputBufferType_e type);
	void (*SetBuffer)(InputBufferType_e type, char* buffer, uint32_t len);

	bool (*IsEmpty)();
	bool (*IsFull)();

	void (*RemChar)();
	void (*AddChar)(char c);

	void (*Cache)();
	void (*Reset)();

	char (*GetLastChar)();

	void (*Refresh)(const char* newCmd);

	InputValue_s (*PutChar)(char c);

	void (*Delete)();
	void (*Backspace)();

	void (*CursorToHome)();
	void (*CursorToEnd)();
	void (*CursorTo)(uint16_t pos);
	void (*CursorToLeft)();
	void (*CursorToRight)();

	void (*CursorShift)(int16_t shift);
}IInput;

#endif // _CLI_INTERFACE_H_