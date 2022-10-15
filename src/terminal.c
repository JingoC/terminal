#include "terminal.h"

#include "core/cli_interface.h"
#include "core/cli_arguments.h"
#include "core/cli_commands.h"
#include "lib/cli_queue.h"
#include "lib/cli_string.h"
#include "module/cli_time.h"
#include "module/cli_log.h"
#include "core/cli_input.h"

#define printArrow()			{CLI_Printf(STRING_TERM_ENTER);CLI_Printf(STRING_TERM_ARROW);}	// Output of input line
#define printArrowWithoutN()	{CLI_Printf(STRING_TERM_ARROW);}

void _PrintResultExec(uint8_t code);
void _PrintResultAddCmd(uint8_t code);
void _PrintTime(CLI_Time_s* time);

void CLI_PrintTime();
void CLI_PrintTimeWithoutRN();

// ************************** method declare ******************************

static bool CLI_Execute();
static TA_Result_e CLI_AddCmd(const char* name, uint8_t (*fcn)(), uint8_t argc, uint16_t mode, const char* descr);
static TC_Result_e CLI_EnterChar(char ch);

static bool CLI_GetIntState();

// ************************* interrupt function ***************************

static bool _interrupt_operation = false;

/// \brief Checking the status of the start of the operation (return, stator-on)
inline bool CLI_GetIntState(){

	bool res = _interrupt_operation;
	_interrupt_operation = false;
	return res;
}

// *********************** Terminal structs *******************************

/// \brief Terminal State
volatile Terminal_s Terminal = {
	.isEntered = false,
	.executeState = 0,
	.countCommand = 0,
	.isCreated = false,
	.iTerminal = {
		.Execute = CLI_Execute,
		.AddCmd = CLI_AddCmd,
		.EnterChar = CLI_EnterChar
	},
	.iTerminalCmdService = {
		.IsArgFlag = CLI_IsArgFlag,
		.GetArgDecByFlag = CLI_GetArgDecByFlag,
		.GetArgHexByFlag = CLI_GetArgHexByFlag,
		.GetArgDec = CLI_GetArgDec,
		.GetArgHex = CLI_GetArgHex,
		.GetIntState = CLI_GetIntState
	}
};

TermCmd_s* _findTermCmd(const char* cmdName);

// ************************* static methods *******************************

/// \brief Terminal initialize
ITerminal* CLI_CreateInstanse(TypeDefaultCmd_e defCmd) {

	if (Terminal.isCreated)
		return (ITerminal*)&Terminal.iTerminal;
	
    CLI_Printf("\r\n****************************************************");
    CLI_Printf("\r\n|                                                  |");
    CLI_Printf("\r\n|                   Terminal "); CLI_Printf(_TERM_VER_); CLI_Printf("                  |");
    CLI_Printf("\r\n|           sw ver.: "); CLI_Printf(__DATE__); CLI_Printf(" ");CLI_Printf(__TIME__);CLI_Printf("          |");
    CLI_Printf("\r\n|                                                  |");
    CLI_Printf("\r\n****************************************************");
    CLI_Printf("\r\n");

	Terminal.iInput = Input_CreateInstance();

    CLI_CommandsInit(defCmd);

    CLI_Printf("\r\nCount base command: %d", Terminal.countCommand);
    CLI_Printf("\r\nMax command: %d", TERM_SIZE_TASK);
    CLI_Printf("\r\n");

    Terminal.inputArgs.argv = (char**) cli_malloc(sizeof(char*) * TERM_ARGS_BUF_SIZE);
    for(uint8_t i = 0; i < TERM_ARGS_BUF_SIZE; i++)
        Terminal.inputArgs.argv[i] = cli_malloc(sizeof(char) * (TERM_ARG_SIZE + 1));

#if (TERM_CMD_LOG_EN == 1)
    CLI_LogInit();
#endif

    printArrow();
    
    Terminal.isCreated = true;
    return(ITerminal*)&Terminal.iTerminal;
}

/// \brief Execute command
/// \param argv argument strings
/// \param argc count argument strings
/// \return result execute command
TE_Result_e _Execute(char** argv, uint8_t argc)
{
    if (argc < 1)
        return TE_ArgErr;

    TermCmd_s* cmd = _findTermCmd(argv[0]);

    if (cmd != NULL){
		
		if ((argc - 1) < cmd->argc)
			return TE_ArgErr;
		
    	Terminal.executeState = 1;
		
		if (cmd->mode & TMC_PrintStartTime)
			CLI_PrintTime();
    	
		uint32_t startMs = CLI_GetMs();
		TE_Result_e result = cmd->fcn((ITerminalCommand*)&Terminal.iTerminalCmdService);
		uint32_t stopMs = CLI_GetMs();
		
		if (cmd->mode & TMC_PrintStopTime)
			CLI_PrintTime();
		
		if (cmd->mode & TMC_PrintDiffTime)
		{
			CLI_Time_s t = CLI_GetFormatTimeByMs(stopMs - startMs);
			_PrintTime(&t);
		}
	
    	Terminal.executeState = 0;
    	
		return result;
    }

    return TE_NotFound;
}

/// \brief Execute command
/// \param str command const string include arguments
/// \return result execute command
TE_Result_e _ExecuteString(const char* str)
{
	cli_split((char*)str, " ", (Params_s*) &Terminal.inputArgs);

#if 0
	CLI_DPrintf("\r\nCMD: ");
	CLI_DPrintf(str);
#endif

#if 0
	for(uint8_t i = 0; i < Terminal.input_args.argc;i++)
        CLI_DPrintf("\r\n: %s", Terminal.input_args.argv[i]);
#endif

    TE_Result_e result = _Execute(Terminal.inputArgs.argv, Terminal.inputArgs.argc);

	CLI_ArgDestroy((Params_s*)&Terminal.inputArgs);

#if (TERM_PRINT_ERROR_EXEC_EN == 1)
	_PrintResultExec(result);
#endif

	printArrow();
	
	return result;
}

/// \brief Execute command
bool CLI_Execute()
{
	if (Terminal.isEntered == true)
	{
		_ExecuteString((const char*) Terminal.iInput->GetBuffer(TransitBuffer));
		Terminal.isEntered = false;
		
		return true;
	}
	
	return false;
}

/// \brief Add command
/// \param name - input name
/// \param fcn - callback function
/// \param argc - min count arguments
/// \param mode - execute mode
/// \param descr - description
/// \return result append command
TA_Result_e CLI_AddCmd(const char* name, uint8_t (*fcn)(), uint8_t argc, uint16_t mode, const char* descr)
{
    if (Terminal.countCommand >= TERM_SIZE_TASK)
	{
		_PrintResultAddCmd(TA_MaxCmd);
        return TA_MaxCmd;
	}

    if (fcn == NULL)
	{
		_PrintResultAddCmd(TA_FcnNull);
		return TA_FcnNull;
	}

    if (cli_strlen((char*)name) == 0)
	{
		_PrintResultAddCmd(TA_EmptyName);
		return TA_EmptyName;
	}

    uint8_t i = 0;
    for(; i < Terminal.countCommand; i++)
        if (cli_strcmp((char*) Terminal.cmds[i].name, (char*) name))
		{
			_PrintResultAddCmd(TA_RetryName);
			return TA_RetryName;
		}

    uint8_t countCmd = Terminal.countCommand;
    Terminal.cmds[countCmd].fcn = fcn;
    Terminal.cmds[countCmd].name = name;
    Terminal.cmds[countCmd].argc = argc;
    Terminal.cmds[countCmd].mode = mode;
    Terminal.cmds[countCmd].description = descr;
    Terminal.countCommand++;

    return TA_OK;
}

/// \brief Print result execute command
/// \param code - result code
/// \return none
void _PrintResultExec(uint8_t code)
{
	switch(code)
	{
        case TE_NotFound:	CLI_Printf("\n\rerr: Command not found");break;
        case TE_ArgErr: 	CLI_Printf("\n\rerr: Fault argument or Fault number arguments");break;
        case TE_ExecErr:	CLI_Printf("\n\rerr: Execute functions");break;
        case TE_WorkInt:	CLI_Printf("\n\rmsg: Command abort");break;
		default:break;
	}
}

/// \brief Print result add command action
/// \param code - result code
/// \return none
void _PrintResultAddCmd(uint8_t code)
{
#if (TERM_PRINT_ERROR_ADD_CMD_EN == 1)
	switch(code)
	{
        case TA_MaxCmd:		CLI_Printf("\n\radd cmd err: Memory is full");break;
        case TA_FcnNull: 	CLI_Printf("\n\radd cmd err: Function callback is NULL");break;
        case TA_EmptyName:	CLI_Printf("\n\radd cmd err: Empty name command");break;
        case TA_RetryName:	CLI_Printf("\n\radd cmd err: Retry name command");break;
		default:break;
	}
#endif
}

/// \brief Search command by name
/// \return Command pointer or NULL
TermCmd_s* _findTermCmd(const char* cmdName)
{
    uint8_t i = 0;
    for(; i < Terminal.countCommand; i++)
	{
    	char* name1 = (char*) Terminal.cmds[i].name;
    	char* name2 = (char*) cmdName;

    	int res = cli_strcmp(name1, name2);

        if (res)
            return (TermCmd_s*)&Terminal.cmds[i];
    }

    return NULL;
}

/// \brief Search command by name
/// \return Command pointer or NULL
TermCmd_s* _findPartTermCmd(const char* cmdName)
{
	TermCmd_s* result = NULL;
	
	uint8_t i = 0;
    for(; i < Terminal.countCommand; i++)
	{
    	char* name1 = (char*) Terminal.cmds[i].name;
    	char* name2 = (char*) cmdName;
	
    	int res = cli_strPartCmp(name1, name2);

        if (res)
		{
        	if (result != NULL)
        		return NULL;
			
            result = (TermCmd_s*)&Terminal.cmds[i];
        }
    }

    return result;
}

// ************************************************************************

void _PrintTime(CLI_Time_s* t)
{
	CLI_Printf("\r\n%02d:%02d:%02d.%03d", (int) t->hour, (int) t->minute, (int) t->second, (int) t->msec);
}

/// \brief ����� �������� ������� � ��������
/// \return none
void CLI_PrintTime()
{
#if (TERM_TIMELEFT_EN == 1)
	uint32_t ms = CLI_GetMs();
    CLI_Time_s t = CLI_GetFormatLastTimeByMs(ms);
    _PrintTime(&t);
#endif
}

/// \brief ����� �������� ������� � �������� ��� �������� ������
/// \return none
void CLI_PrintTimeWithoutRN()
{
#if (TERM_TIMELEFT_EN == 1)
	uint32_t ms = CLI_GetMs();
    CLI_Time_s t = CLI_GetFormatLastTimeByMs(ms);
	_PrintTime(&t);
#endif
}

/// \brief Append new symbols
TC_Result_e CLI_EnterChar(char ch)
{
	static bool rstUnlock = false;
	if(rstUnlock && (ch != TERM_KEY_RESET))
		rstUnlock = false;
		
	InputValue_s iv = Terminal.iInput->PutChar(ch);
	char c = iv.keyCode;

	if (Terminal.isEntered)
	{
		if (!((ch == CHAR_INTERRUPT) || (ch == TERM_KEY_RESET)))
			return TC_Ignore;
	}
	
#if 0
	CLI_DPrintf("\r\n");
	for(uint8_t i = 0; i < 4; i++)
	{
		CLI_DPrintf("0x%02X ", (char) *((char*)(Terminal.symbols.ptrObj + i)));
	}
    CLI_DPrintf("\r\nKey Code: 0x%02X", (uint8_t) ch);
#endif
	if (iv.isValid)
	{
		switch(c)
		{
			case TERM_KEY_ENTER:
			{
				if (Terminal.iInput->IsEmpty())
				{
					printArrow();
					return TC_Ignore;
				}
				
				Terminal.isEntered = true;
				
				Terminal.iInput->Cache();

#if (TERM_CMD_LOG_EN == 1)
				CLI_LogCmdPush(Terminal.iInput->GetBuffer(MainBuffer));
				CLI_CurReset();
#endif
				
				Terminal.iInput->Reset();

				return TC_Enter;
			}break;
			case CHAR_INTERRUPT:
			{
				_interrupt_operation = true;
				CLI_Printf("\r\nKey ESC");
			}break;
			case TERM_KEY_BACKSPACE:	Terminal.iInput->Backspace(); break;
			case TERM_KEY_RESET:
			{
				if(rstUnlock)
				{
					RESET_FCN();
					return TC_Reset;
				}
				else
					rstUnlock = true;
			}break;
			case TERM_KEY_TAB:
			{
#if (TERM_CMD_AUTOCMPLT_EN == 1)
				if ((!Terminal.iInput->IsEmpty()) && (Terminal.iInput->GetLastChar() != ' '))
				{
					char* buf = Terminal.iInput->GetBuffer(MainBuffer);
					TermCmd_s* cmd = _findPartTermCmd(buf);
					
					if (cmd != NULL)
					{
						uint8_t len = cli_strlen(cmd->name);
						
						Terminal.iInput->SetBuffer(MainBuffer, (char*)cmd->name, len + 1);
						Terminal.iInput->Refresh(buf);
					}
				}
#endif
			}break;
			case TERM_KEY_DOWN:
			{
#if (TERM_CMD_LOG_EN == 1)
				const char* ptrCmd = CLI_GetNextCmd();
				if (ptrCmd != NULL)
					Terminal.iInput->Refresh(ptrCmd);
#endif
			}break;
			case TERM_KEY_UP:
			{
#if (TERM_CMD_LOG_EN == 1)
				const char* ptrCmd = CLI_GetLastCmd();
				if (ptrCmd != NULL)
					Terminal.iInput->Refresh(ptrCmd);
#endif
			}break;
#if (TERM_LR_KEY_EN == 1)
			case TERM_KEY_LEFT:		Terminal.iInput->CursorToLeft(); break;
#endif
#if (TERM_LR_KEY_EN == 1)
			case TERM_KEY_RIGHT:	Terminal.iInput->CursorToRight(); break;
#endif
			case TERM_KEY_DEL:		Terminal.iInput->Delete(); break;
			case TERM_KEY_HOME:		Terminal.iInput->CursorToHome(); break;
			case TERM_KEY_END:		Terminal.iInput->CursorToEnd(); break;
			default:
			{
				if (iv.isAlphaBet && !Terminal.iInput->IsFull())
					Terminal.iInput->AddChar(c);
			}break;
		}
	}
	else
	{
		return TC_BufFull;
	}

	return TC_OK;
}
