#ifndef _TERMINAL_TYPES_H
#define _TERMINAL_TYPES_H

#include "terminal_config.h"
#include "cli_interface.h"

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

typedef struct{
	uint8_t argc;
	char** argv;
}Params_s;

/// \brief Command settings
typedef struct
{
    uint8_t (*fcn)();			// callback function command
    const char* name;			// name command
	uint8_t argc;				// min count argument
	uint16_t mode;				// mode execute command
	const char* description;	// description command
}TermCmd_s;

/// \brief Terminal State
typedef struct
{
    TermCmd_s cmds[TERM_SIZE_TASK];				// list commands
    uint8_t countCommand;						// count commands
    uint8_t executeState;						// state terminal
    volatile Params_s inputArgs;				// args current execute command
    bool isEntered;								// 
    bool isCreated;
    ITerminal iTerminal;
    ITerminalCommand iTerminalCmdService;
	IInput* iInput;
}Terminal_s;

#endif // _TERMINAL_TYPES_H
