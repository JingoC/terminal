#include "terminal.h"

#include "lib/cli_queue.h"
#include "lib/cli_string.h"
#include "module/cli_time.h"
#include "module/cli_log.h"

#define printArrow()			{CLI_Printf(STRING_TERM_ENTER);CLI_Printf(STRING_TERM_ARROW);}	// Output of input line
#define printArrowWithoutN()	{CLI_Printf(STRING_TERM_ARROW);}

void _PrintResultExec(uint8_t code);
void _PrintResultAddCmd(uint8_t code);
void _PrintTime(CLI_Time_s* time);
int8_t _IndexOfFlag(const char* flag);

void CLI_PrintTime();
void CLI_PrintTimeWithoutRN();

extern uint8_t _strcmp(const char* str1, const char* str2);
extern uint32_t _strlen(const char* strSrc);

static uint8_t _help_cmd();
static uint8_t _reset_cpu();
static uint8_t _settime_cmd();
static uint8_t _gettime_cmd();
static uint8_t _cpur_cmd();
static uint8_t _cpuw_cmd();
static uint8_t _cpurb_cmd();

extern CLI_Time_s _def_time;

// ************************* interrupt function ***************************

static bool _interrupt_operation = false;

/// \brief Checking the status of the start of the operation (return, stator-on)
inline bool CLI_GetIntState(){

	bool res = _interrupt_operation;
	_interrupt_operation = false;
	return res;
}

// ************************************************************************

// *********************** Terminal fcns **********************************

/// \brief Command settings
typedef struct{
    uint8_t (*fcn)();			// callback function command
    const char* name;			// name command
	uint8_t argc;				// min count argument
	uint16_t mode;				// mode execute command
	const char* description;	// description command
}TermCmd_s;

/// \brief Terminal State
struct{
	Queue_s symbols;							// queue symbols input
	char buf_transit[TERM_CMD_BUF_SIZE + 1];	// transit buffer
	char buf_enter[TERM_CMD_BUF_SIZE + 1];		// input buffer
	char buf_enter_exec[TERM_CMD_BUF_SIZE + 1];	// input buffer in execute command
	int16_t buf_curPos;							// current cursos position
	int16_t buf_cntr;							// current count input symbols
	int16_t buf_cntr_exec;						// current count input symbols in execute command
    TermCmd_s cmds[TERM_SIZE_TASK];				// list commands
    uint8_t countCommand;						// count commands
    uint8_t executeState;						// state terminal
    volatile args input_args;					// args current execute command
    bool isEntered;								// 
}Terminal;		// Терминал

TermCmd_s* _findTermCmd(const char* cmdName);

/// \brief Terminal initialize
void CLI_Init(TypeDefaultCmd_e defCmd){

    CLI_Printf("\r\n****************************************************");
    CLI_Printf("\r\n|                                                  |");
    CLI_Printf("\r\n|                   Terminal "); CLI_Printf(_TERM_VER_); CLI_Printf("                  |");
    CLI_Printf("\r\n|           sw ver.: "); CLI_Printf(__DATE__); CLI_Printf(" ");CLI_Printf(__TIME__);CLI_Printf("          |");
    CLI_Printf("\r\n|                                                  |");
    CLI_Printf("\r\n****************************************************");
    CLI_Printf("\r\n");

    Terminal.countCommand = 0;
    Terminal.buf_curPos = 0;
    Terminal.buf_cntr = 0;
    Terminal.buf_cntr_exec = 0;
    Terminal.buf_transit[Terminal.buf_cntr] = '\0';
    Terminal.buf_transit[TERM_CMD_BUF_SIZE] = '\0';
    Terminal.buf_enter[Terminal.buf_cntr] = '\0';
    Terminal.buf_enter[TERM_CMD_BUF_SIZE] = '\0';
    Terminal.buf_enter_exec[Terminal.buf_cntr_exec] = '\0';
    Terminal.buf_enter_exec[TERM_CMD_BUF_SIZE] = '\0';
    Terminal.executeState = 0;
    Terminal.isEntered = false;
    

    CLI_SetTime(&_def_time, 0, 0, 0);

    CLI_AddCmd("help", 		_help_cmd, 		0, TMC_None, "help by terminal command");
    CLI_AddCmd("~", 		_reset_cpu, 	0, TMC_None, "reset cpu");

    if (defCmd & TDC_Time)
    {
        CLI_AddCmd("settime", 	_settime_cmd, 	3, TMC_None, "set current time\n\r\tsettime [h] [m] [s]");
        CLI_AddCmd("gettime", 	_gettime_cmd, 	0, TMC_None, "print current time");
    }

    if (defCmd & TDC_CPU)
    {
        CLI_AddCmd("cpur", 		_cpur_cmd, 		1, TMC_None, "read from CPU reg\n\r\tcpur [addr]");
        CLI_AddCmd("cpuw", 		_cpuw_cmd, 		2, TMC_None, "write in CPU reg\n\r\tcpuw [addr] [data]");
        CLI_AddCmd("cpurb", 	_cpurb_cmd,		2, TMC_None, "read block from CPU\n\r\tcpurb [addr] [length]");
    }

    CLI_Printf("\r\nCount base command: %d", Terminal.countCommand);
    CLI_Printf("\r\nMax command: %d", TERM_SIZE_TASK);
    CLI_Printf("\r\n");

    Terminal.input_args.argv = (char**) cli_malloc(sizeof(char*) * TERM_ARGS_BUF_SIZE);
    for(uint8_t i = 0; i < TERM_ARGS_BUF_SIZE; i++)
        Terminal.input_args.argv[i] = cli_malloc(sizeof(char) * (TERM_ARG_SIZE + 1));

    Q_Init(&Terminal.symbols, 3, sizeof(char), QUEUE_FORCED_PUSH_POP_Msk);

#if (TERM_CMD_LOG_EN == 1)
    CLI_LogInit();
#endif

    printArrow();
}

#define CLI_GetDecString(str)        ((uint32_t) strtoll((const char*)str, NULL, 10))
#define CLI_GetHexString(str)        ((uint32_t) strtoll((const char*)str, NULL, 16))

inline uint32_t CLI_GetArgDec(uint8_t index)
{
	return CLI_GetDecString(Terminal.input_args.argv[index + 1]);
}

inline uint32_t CLI_GetArgHex(uint8_t index)
{
	return CLI_GetHexString(Terminal.input_args.argv[index + 1]);
}

bool CLI_GetArgDecByFlag(const char* flag, uint32_t* outValue)
{
	int8_t w = _IndexOfFlag(flag);
	if (w != 0)
	{
		*outValue = CLI_GetArgDec(w);
		return true;
	}
	
	return false;
}

bool CLI_GetArgHexByFlag(const char* flag, uint32_t* outValue)
{
	int8_t w = _IndexOfFlag(flag);
	if (w != 0)
	{
		*outValue = CLI_GetArgHex(w);
		return true;
	}
	
	return false;
}

bool CLI_IsArgFlag(const char* flag)
{
	return _IndexOfFlag(flag) >= 0;
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
    	
		uint32_t startMs = Terminal_GetMs();
		TE_Result_e result = cmd->fcn(argv, argc);
		uint32_t stopMs = Terminal_GetMs();
		
		if (cmd->mode & TMC_PrintStopTime)
			CLI_PrintTime();
		
		if (cmd->mode & TMC_PrintDiffTime)
		{
			CLI_Time_s t = CLI_GenerateTimeMSec(stopMs - startMs);
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
	split((char*)str, " ", (args*) &Terminal.input_args);

#if 0
	for(uint8_t i = 0; i < Terminal.input_args.argc;i++)
        CLI_DPrintf("\r\n: %s", Terminal.input_args.argv[i]);
#endif

    TE_Result_e result = _Execute(Terminal.input_args.argv, Terminal.input_args.argc);

	ArgDestroy((args*)&Terminal.input_args);

#if (TERM_PRINT_ERROR_EXEC_EN == 1)
	_PrintResultExec(result);
#endif

	return result;
}

/// \brief Execute command
bool CLI_Execute()
{
	if (Terminal.isEntered == true)
	{
		_ExecuteString((const char*)Terminal.buf_transit);
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

    if (_strlen((char*)name) == 0)
	{
		_PrintResultAddCmd(TA_EmptyName);
		return TA_EmptyName;
	}

    uint8_t i = 0;
    for(; i < Terminal.countCommand; i++)
        if (_strcmp((char*) Terminal.cmds[i].name, (char*) name))
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
	switch(code){
        case TE_NotFound:	CLI_Printf("\n\rerr: Command not found");break;
        case TE_ArgErr: 	CLI_Printf("\n\rerr: Fault argument or Fault number arguments");break;
        case TE_ExecErr:	CLI_Printf("\n\rerr: Execute functions");break;
        case TE_WorkInt:	CLI_Printf("\n\rmsg: Command abort");break;
		default:break;
	}

	printArrow();
}

/// \brief Print result add command action
/// \param code - result code
/// \return none
void _PrintResultAddCmd(uint8_t code)
{
#if (TERM_PRINT_ERROR_ADD_CMD_EN == 1)
	switch(code){
        case TA_MaxCmd:		CLI_Printf("\n\radd cmd err: Memory is full");break;
        case TA_FcnNull: 	CLI_Printf("\n\radd cmd err: Function callback is NULL");break;
        case TA_EmptyName:	CLI_Printf("\n\radd cmd err: Empty name command");break;
        case TA_RetryName:	CLI_Printf("\n\radd cmd err: Retry name command");break;
		default:break;
	}
#endif
}

/// \brief Get index coinciding args string or -1
/// \param flag - searched string
int8_t _IndexOfFlag(const char* flag)
{
	for(uint8_t i = 0; i < Terminal.input_args.argc; i++)
	{
		if (_strcmp(Terminal.input_args.argv[i], flag))
		{
			if ((Terminal.input_args.argc > (i + 1)) || (flag[0] != '-'))
				{return i;}
			else
				{return -1;}
		}
	}

	return -1;
}

/// \brief Search command by name
/// \return Command pointer or NULL
TermCmd_s* _findTermCmd(const char* cmdName)
{
    uint8_t i = 0;
    for(; i < Terminal.countCommand; i++){

    	char* name1 = (char*) Terminal.cmds[i].name;
    	char* name2 = (char*) cmdName;

    	int res = _strcmp(name1, name2);

        if (res){
            return &Terminal.cmds[i];
        }
    }

    return NULL;
}

// ************************************************************************

// ************************* sys cmd terminal *****************************

uint8_t _help_cmd()
{
    CLI_Printf("\r\nCount command: %d", (int) Terminal.countCommand);
    CLI_Printf("\r\n[] - mandatory argument\r\n<> - optional argument\r\n| - choice between arguments");
	uint16_t i = 1;
	for(; i < Terminal.countCommand; i++){
        CLI_Printf("\r\n\n%-10s - %s", Terminal.cmds[i].name, Terminal.cmds[i].description);
        CLI_Printf("\r\n-----------------------------------------------------------------");
	}

	return TE_OK;
}

uint8_t _reset_cpu()
{
	RESET_FCN();

	return TE_OK;
}

uint8_t _settime_cmd()
{
    uint32_t h = CLI_GetArgDec(0);
    uint8_t m = CLI_GetArgDec(1);
    uint8_t s = CLI_GetArgDec(2);

	SysTimeReset();

    CLI_SetTime(&_def_time, h, m, s);

	return TE_OK;
}

uint8_t _gettime_cmd()
{
    CLI_Printf("\r\nTime: ");
    CLI_PrintTime();

	return TE_OK;
}

uint8_t _cpur_cmd()
{
    uint32_t addr = CLI_GetArgHex(0);
    uint32_t* v = (uint32_t*)(intptr_t) addr;

    CLI_Printf("\r\n0x%08X: 0x%08X", (int)(intptr_t) v, (int) *v);

	return TE_OK;
}

uint8_t _cpuw_cmd()
{
    uint32_t* v = (uint32_t*)(intptr_t) CLI_GetArgHex(0);
    uint32_t d = CLI_GetArgHex(1);
	*v = d;

	return TE_OK;
}

uint8_t _cpurb_cmd()
{
    uint32_t* v = (uint32_t*)(intptr_t) CLI_GetArgHex(0);
    uint32_t c = CLI_GetArgDec(1);

	for(uint32_t i = 0; i < c; i++)
	{
        CLI_Printf("\r\n0x%08X: 0x%08X", (int)(intptr_t) v, (int) *v);
		v++;
	}


	return TE_OK;
}

// ************************************************************************

void _PrintTime(CLI_Time_s* t)
{
	CLI_Printf("\r\n%02d:%02d:%02d.%03d", (int) t->hour, (int) t->minute, (int) t->second, (int) t->msec);
}

/// \brief Вывод текущего времени в терминал
/// \return none
void CLI_PrintTime()
{
	uint32_t ms = Terminal_GetMs();
    CLI_Time_s t = CLI_GenerateTimeMSec(CLI_GetTimeMSec(&_def_time) + ms);
    _PrintTime(&t);
}

/// \brief Вывод текущего времени в терминал без перевода строки
/// \return none
void CLI_PrintTimeWithoutRN()
{
	uint32_t ms = Terminal_GetMs();
    CLI_Time_s t = CLI_GenerateTimeMSec(CLI_GetTimeMSec(&_def_time) + ms);
	_PrintTime(&t);
}

static void _UpdateCmd(const char* newCmd)
{
    CLI_PutChar('\r');
	printArrowWithoutN();

	uint32_t lenNewCmd = _strlen(newCmd) + 1;
	uint32_t lenCurCmd = Terminal.buf_cntr;
	cli_memcpy(Terminal.buf_enter, newCmd, lenNewCmd);
	
	Terminal.buf_cntr = lenNewCmd;
	Terminal.buf_curPos = lenNewCmd;

	for(uint8_t i = 0; i < lenNewCmd; i++)
	{
		CLI_PutChar(Terminal.buf_enter[i]);
	}

	uint8_t cntSpcChar = 0;
	for(uint8_t i = lenNewCmd; i < lenCurCmd; i++)
	{
		CLI_PutChar(' ');
		cntSpcChar++;
	}

	for(uint8_t i = 0; i < cntSpcChar; i++)
        {CLI_PutChar(0x08);}
}

static void _AddChar(char c)
{
#if (TERM_LR_KEY_EN == 1)
	if (Terminal.buf_curPos != Terminal.buf_cntr)
	{
		for(uint8_t i = Terminal.buf_cntr + 1; i > Terminal.buf_curPos; i--)
		{
			Terminal.buf_enter[i] = Terminal.buf_enter[i-1];
		}

		Terminal.buf_enter[Terminal.buf_curPos] = c;
		Terminal.buf_cntr++;
		Terminal.buf_curPos++;
		Terminal.buf_enter[Terminal.buf_cntr] = '\0';

		uint8_t tmpPos = Terminal.buf_curPos;
		_UpdateCmd(Terminal.buf_enter);

		for(uint8_t pos = 0; pos < Terminal.buf_cntr - tmpPos; pos++)
		{
            CLI_PutChar(0x08);
			Terminal.buf_curPos--;
		}
	}
	else
	{
        CLI_PutChar(c);
		Terminal.buf_enter[Terminal.buf_curPos] = c;
		Terminal.buf_cntr++;
		Terminal.buf_curPos++;
		Terminal.buf_enter[Terminal.buf_cntr] = '\0';
	}

#else
    CLI_PutChar(c);
	Terminal.buf_enter[Terminal.buf_curPos] = c;
	Terminal.buf_cntr++;
	Terminal.buf_curPos++;
	Terminal.buf_enter[Terminal.buf_cntr] = '\0';
#endif
}

static void _RemChar()
{
#if (TERM_LR_KEY_EN == 1)

	if (Terminal.buf_curPos != Terminal.buf_cntr)
	{
        CLI_PutChar(0x08);
        CLI_PutChar(' ');
        CLI_PutChar(0x08);

		// save current position cursor
		uint8_t tmpPos = Terminal.buf_curPos - 1;

		// shift all last symbols
		for(uint8_t i = tmpPos; i < Terminal.buf_cntr; i++)
		{
			Terminal.buf_enter[i] = Terminal.buf_enter[i+1];
		}

		Terminal.buf_curPos--;
		Terminal.buf_cntr--;
		Terminal.buf_enter[Terminal.buf_cntr] = '\0';

		_UpdateCmd(Terminal.buf_enter);

		for(uint8_t pos = 0; pos < Terminal.buf_cntr - tmpPos; pos++)
		{
            CLI_PutChar(0x08);
			Terminal.buf_curPos--;
		}
	}
	else
	{
        CLI_PutChar(0x08);
        CLI_PutChar(' ');
        CLI_PutChar(0x08);

		Terminal.buf_curPos--;
		Terminal.buf_cntr--;
		Terminal.buf_enter[Terminal.buf_cntr] = '\0';
	}
#else
    CLI_PutChar(0x08);
    CLI_PutChar(' ');
    CLI_PutChar(0x08);

	Terminal.buf_curPos--;
	Terminal.buf_cntr--;
	Terminal.buf_enter[Terminal.buf_cntr] = '\0';
#endif
}

/// \brief Append new symbols
TC_Result_e CLI_EnterChar(char c)
{
	static bool rstUnlock = false;
	if(rstUnlock && (c != TERM_KEY_RESET))
		rstUnlock = false;
	
	/*
	uint8_t* write_buffer = (uint8_t*)Terminal.buf_enter;
	int16_t* write_cntr = &Terminal.buf_cntr;

	if (Terminal.executeState == 1)
	{
		write_buffer = &Terminal.buf_cntr_exec;
		write_cntr = &Terminal.buf_cntr_exec;
	}
	 */

	Q_Push(&Terminal.symbols, &c);
	/*
	TerminalTx("\r\nPrintQ: ");
	for(uint8_t i = 0; i < Terminal.symbols._cntr; i++)
	{TerminalTx("%02X ",*((char*) (Terminal.symbols.ptrObj + i)));}
	 */

	uint8_t arr_up[]	= {0x1B, 0x5B, 0x41};
	uint8_t arr_down[]	= {0x1B, 0x5B, 0x42};
	uint8_t arr_right[] = {0x1B, 0x5B, 0x43};
	uint8_t arr_left[]	= {0x1B, 0x5B, 0x44};
	uint8_t arr_esc[]	= {0x1B, 0x1B, 0x1B};
	uint8_t del[]		= {0x1B, 0x5B, 0x33};
	
	if (Q_IsEqual(&Terminal.symbols, arr_up, 3))
		{c = TERM_KEY_UP;}
	else if (Q_IsEqual(&Terminal.symbols, arr_down, 3))
		{c = TERM_KEY_DOWN;}
	else if (Q_IsEqual(&Terminal.symbols, arr_right, 3))
		{c = TERM_KEY_RIGHT;}
	else if (Q_IsEqual(&Terminal.symbols, arr_left, 3))
		{c = TERM_KEY_LEFT;}
	else if (Q_IsEqual(&Terminal.symbols, arr_esc, 3))
		{c = TERM_KEY_ESCAPE;}
	else if (Q_IsEqual(&Terminal.symbols, del, 3))
		{c = TERM_KEY_DEL;}
	
	bool isValidKey = ((Terminal.buf_cntr < TERM_CMD_BUF_SIZE) ||
						(c == TERM_KEY_BACKSPACE) ||
						(c == TERM_KEY_ENTER)	||
						(c == CHAR_INTERRUPT));

	bool isAlphaBet = (((c > 0x2F) && (c < 0x3A)) ||
						((c > 0x60) && (c < 0x7B)) ||
						((c > 0x40) && (c < 0x5B)) ||
						(c == 0x20) || (c == '_') || (c == '-'));

	if (Terminal.isEntered)
	{
		if (!((c == CHAR_INTERRUPT) || (c == TERM_KEY_RESET)))
			return TC_Ingore;
	}
	
#if 0
    CLI_DPrintf("\r\nKey Code: 0x%02X", c);
#endif
	if (isValidKey)
	{
		switch(c)
		{
			case TERM_KEY_ENTER:
			{
				Terminal.isEntered = true;
				
				Terminal.buf_enter[Terminal.buf_cntr] = '\0';
				cli_memcpy(Terminal.buf_transit, Terminal.buf_enter, Terminal.buf_cntr + 1);

#if (TERM_CMD_LOG_EN == 1)
				CLI_LogCmdPush(Terminal.buf_enter);
				CLI_CurReset();
#endif
				Terminal.buf_curPos = 0;
				Terminal.buf_cntr = 0;
				Terminal.buf_enter[Terminal.buf_cntr] = '\0';

				return TC_Enter;
			}break;
			case CHAR_INTERRUPT:		{_interrupt_operation = true;CLI_Printf("\r\nKey ESC");}break;
			case TERM_KEY_BACKSPACE:	{if (Terminal.buf_curPos > 0){_RemChar();}}break;
			case TERM_KEY_RESET:
			{
				if(rstUnlock)
				{
					RESET_FCN();
					return TC_Reset;
				}
				else
				{
					rstUnlock = true;
				}
			}break;
			case TERM_KEY_DOWN:
			{
#if (TERM_CMD_LOG_EN == 1)
				const char* ptrCmd = CLI_GetNextCmd();
				if (ptrCmd != NULL)
					{_UpdateCmd(ptrCmd);}
#endif
			}break;
			case TERM_KEY_UP:
			{
#if (TERM_CMD_LOG_EN == 1)
				const char* ptrCmd = CLI_GetLastCmd();
				if (ptrCmd != NULL)
					{_UpdateCmd(ptrCmd);}
#endif
			}break;
			case TERM_KEY_LEFT:
			{
#if (TERM_LR_KEY_EN == 1)
				if (Terminal.buf_curPos > 0)
				{
					Terminal.buf_curPos--;
                    CLI_PutChar(0x08);
				}
#endif
			}break;
			case TERM_KEY_RIGHT:
			{
#if (TERM_LR_KEY_EN == 1)
				if (Terminal.buf_curPos < Terminal.buf_cntr)
				{
                    CLI_PutChar(Terminal.buf_enter[Terminal.buf_curPos]);
					Terminal.buf_curPos++;
				}
#endif
			}break;
			case TERM_KEY_DEL:
			{
				if ((Terminal.buf_curPos != Terminal.buf_cntr) && (Terminal.buf_cntr != 0))
				{
					Terminal.buf_curPos++;
					if(Terminal.buf_curPos == Terminal.buf_cntr)
					{
						CLI_PutChar(Terminal.buf_enter[Terminal.buf_curPos - 1]);
					}
					_RemChar();
				}
			}break;
			default:
			{
				if (isAlphaBet)
				{
					if (Terminal.buf_cntr < TERM_CMD_BUF_SIZE)
						{_AddChar(c);}
				}
			}break;
		}
	}
	else {
		return TC_BufFull;
	}

	return TC_OK;
}
