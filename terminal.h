#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "terminal_config.h"

/// \brief Terminal Error Result Execute
typedef enum{ TE_OK = 0, TE_Err, TE_NotFound, TE_ArgErr, TE_ExecErr, TE_WorkInt, }TE_Result_e;

/// \brief Terminal Add Command Result
typedef enum{ TA_OK = 0, TA_MaxCmd, TA_FcnNull, TA_EmptyName, TA_RetryName, }TA_Result_e;

/// \brief Terminal Char Append Result
typedef enum{ TC_OK = 0, TC_Enter, TC_BufFull, TC_Reset,}TC_Result_e; 

/// \brief Terminal Default Init Command
typedef enum{
	TDC_None = 0x0000,
	TDC_Time = 0x0001,
	TDC_CPU = 0x0002,
	TDC_All = 0xFFFF
}TypeDefaultCmd_e;

/// \brief Terminal Mode Execute Command
typedef enum{
	TMC_None = 0x0000,
	TMC_PrintStartTime = 0x0001,
	TMC_PrintStopTime = 0x0002,
	TMC_PrintDiffTime = 0x0004,
	TMC_All = 0xFFFF,
}TypeModeCmd_e;

bool CLI_GetIntState();
#define CLI_RetInt()			{ if (CLI_GetIntState()){return TE_WorkInt;}}

void CLI_Init(TypeDefaultCmd_e defCmd);
TA_Result_e CLI_AddCmd(const char* name, 
						uint8_t (*fcn)(), 
						uint8_t argc, 
						uint16_t mode, 
						const char* descr);

TC_Result_e CLI_EnterChar(char c);
bool CLI_Execute();

bool CLI_GetArgDecByFlag(const char* flag, uint32_t* outValue);
bool CLI_GetArgHexByFlag(const char* flag, uint32_t* outValue);

uint32_t CLI_GetArgDec(uint8_t index);
uint32_t CLI_GetArgHex(uint8_t index);

#endif // _TERMINAL_H
