#include "terminal.h"

#include "lib/cli_queue.h"
#include "lib/cli_string.h"
#include "module/cli_time.h"
#include "module/cli_log.h"

#define printArrow()			{CLI_Printf(STRING_TERM_ENTER);CLI_Printf(STRING_TERM_ARROW);}	// ����� �������� ����������� �� ����
#define printArrowWithoutN()	{CLI_Printf(STRING_TERM_ARROW);}

void _PrintResultExec(uint8_t code);
void _PrintResultAddCmd(uint8_t code);
void _PrintTime(CLI_Time_s* time);
int8_t _IndexOfFlag(const char* flag);

extern uint8_t _strcmp(const char* str1, const char* str2);
extern uint32_t _strlen(const char* strSrc);

static uint8_t _help_cmd(char** argv, uint8_t argc);
static uint8_t _reset_cpu(char** argv, uint8_t argc);
static uint8_t _settime_cmd(char** argv, uint8_t argc);
static uint8_t _gettime_cmd(char** argv, uint8_t argc);
static uint8_t _cpur_cmd(char** argv, uint8_t argc);
static uint8_t _cpuw_cmd(char** argv, uint8_t argc);
static uint8_t _cpurb_cmd(char** argv, uint8_t argc);

extern CLI_Time_s _def_time;

static void (*enter_callback)(uint8_t* rx_buf, uint16_t length);

// ************************* interrupt function ***************************

static bool _interrupt_operation = false;

/// \brief �������� ������� ���������� �������� (����� ������, ������ ���������)
/// \return {bool} - true: ��������� �������� ��������
inline bool CLI_GetIntState(){

	bool res = _interrupt_operation;
	_interrupt_operation = false;
	return res;
}

// ************************************************************************

// *********************** Terminal fcns **********************************

typedef struct{
    uint8_t (*fcn)();			// ������� ���������� ��������
    const char* name;			// ��� �������, �� ������� ���������� �������
	uint8_t argc;				// 
	uint16_t mode;				// 
	const char* description;	// �������� ������� (���������, �������� �������� � �.�.)
}TermCmd;		// ��������� ��������� ������� ���������

struct{
	Queue_s symbols;							// ������� �������� ��������� ���������������
	char buf_transit[TERM_CMD_BUF_SIZE + 1];	//
	char buf_enter[TERM_CMD_BUF_SIZE + 1];		// ����� �����
	char buf_enter_exec[TERM_CMD_BUF_SIZE + 1];	// ����� �����, ��� ���������� ������
	int16_t buf_curPos;							// ������� ������� �������
	int16_t buf_cntr;							// �������� ������ �����
	int16_t buf_cntr_exec;						// �������� ������ ����� ��� ���������� ������
    TermCmd cmds[TERM_SIZE_TASK];				// ������� ���������
    uint8_t countCommand;						// ���������� ������
    uint8_t executeState;						// ��������� ��������� (�������� ������ ��� ���)
    volatile args input_args;
    bool isEntered;								// 
}Terminal;		// ��������

TermCmd* _findTermCmd(const char* cmdName);

/// \brief ������������� ���������
/// \return none
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

    CLI_AddCmd("help", 		_help_cmd, 		"������ �� �������� ���������");
    CLI_AddCmd("~", 		_reset_cpu, 	"reset cpu");

    if (defCmd & TDC_Time)
    {
        CLI_AddCmd("settime", 	_settime_cmd, 	"������ ������� �����\n\r\tsettime [h] [m] [s]");
        CLI_AddCmd("gettime", 	_gettime_cmd, 	"������� ������� �����");
    }

    if (defCmd & TDC_CPU)
    {
        CLI_AddCmd("cpur", 		_cpur_cmd, 		"��������� �� ������ ����������\n\r\tcpur [addr]\n\r\t\taddr - 32-��������� �������� ������, �� �������� ������");
        CLI_AddCmd("cpuw", 		_cpuw_cmd, 		"�������� �� ������ ����������\n\r\tcpuw [addr] [data]\n\r\t\taddr - 32-��������� �������� ������, �� �������� ������\n\r\t\tdata - 32-��������� �������� ������������ ������");
        CLI_AddCmd("cpurb", 	_cpurb_cmd,		"��������� ���� �� ���������� ������� � ������\n\r\tcpurb [addr] [length]\n\r\t\taddr - 32-��������� �������� ������, �� �������� �������� ������\n\r\t\tlength - ���������� ��������� ���������");
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

uint32_t CLI_GetArgDec(uint8_t index)
{
	return CLI_GetDecString(Terminal.input_args.argv[index + 1]);
}

uint32_t CLI_GetArgHex(uint8_t index)
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

/// \brief ��������� �������
/// \param {char*} argv - ��������� (�������� � ���������)
/// \param {uint8_t} argc - ���������� ����������
/// \return {TE_Result_e} - ��������� ���������� �������
TE_Result_e _Execute(char** argv, uint8_t argc)
{
    if (argc < 1)
        return TE_ArgErr;

    TermCmd* cmd = _findTermCmd(argv[0]);

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

/// \brief ��������� �������
/// \param {const char*} str - ������ � �������� � ������� ���������� � ���
/// \return {TE_Result_e} - ��������� ���������� �������
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

/// \brief ������� ���������� ������� � ��������
/// \param {const char*} name - ��� �������, �� ������� ��� ����� ����������� � ���������
/// \param {uint8_t (*)(char**, uint8_t)} fcn - ������� ���������� �������
/// \param {const char*} descr - �������� ������� (��� ������� help)
/// \return {TA_Result_e} - ��������� ������� �������� �������
TA_Result_e CLI_AddCmd(const char* name, uint8_t (*fcn)(char**, uint8_t), const char* descr)
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
    Terminal.cmds[countCmd].description = descr;
    Terminal.countCommand++;

    return TA_OK;
}

/// \brief ������� ��������� ���������� ������� � ��������
/// \param {uint8_t} code - ��� ���������� ���������� �������
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

/// \brief ������� ��������� ���������� ������� � ��������
/// \param {uint8_t} code - ��� ���������� ���������� �������
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

/// \brief �������� ����������� ����� ������ �������
/// \param {const char*} flag - ������� ���� (������������� 2 ���� ������, � ������ ('-') ����� ������ ������������� ������� �������� ����� �����, ������ ������������� ����, ������������� ��������� ��������.
/// \return int8_t - � ������ ������� �����, ���������� ��� ������ ����� ����������, � ���� ������ �������� -1.
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

/// \brief ����� ������� ����� ��������� � ���������
/// \param {const char*} cmdName - ��� ������� �������
/// \return {TermCmd*} - ��������� ������ (NULL, ���� ������� �� �������)
TermCmd* _findTermCmd(const char* cmdName)
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

uint8_t _help_cmd(char** argv, uint8_t argc)
{
    CLI_Printf("\r\nCount command: %d", (int) Terminal.countCommand);
    CLI_Printf("\r\n[] - ������������ ��������\r\n<> - �� ������������ ��������\r\n| - ����� ����� �����������");
	uint16_t i = 1;
	for(; i < Terminal.countCommand; i++){
        CLI_Printf("\r\n\n%-10s - %s", Terminal.cmds[i].name, Terminal.cmds[i].description);
        CLI_Printf("\r\n-----------------------------------------------------------------");
	}

	return TE_OK;
}

uint8_t _reset_cpu(char** argv, uint8_t argc)
{
	RESET_FCN();

	return TE_OK;
}

uint8_t _settime_cmd(char** argv, uint8_t argc)
{
	if (argc < 4)
		return TE_ArgErr;

    uint32_t h = CLI_GetDecString(argv[1]);
    uint8_t m = CLI_GetDecString(argv[2]);
    uint8_t s = CLI_GetDecString(argv[3]);

	SysTimeReset();

    CLI_SetTime(&_def_time, h, m, s);

	return TE_OK;
}

uint8_t _gettime_cmd(char** argv, uint8_t argc)
{
    CLI_Printf("\r\nTime: ");
    CLI_PrintTime();

	return TE_OK;
}

uint8_t _cpur_cmd(char** argv, uint8_t argc)
{
	if (argc < 2)
		return TE_ArgErr;

    uint32_t* v = (uint32_t*) CLI_GetHexString(argv[1]);

    CLI_Printf("\r\n0x%08X: 0x%08X", (uint32_t) v, (uint32_t) *v);

	return TE_OK;
}

uint8_t _cpuw_cmd(char** argv, uint8_t argc)
{
	if (argc < 3)
		return TE_ArgErr;


    uint32_t* v = (uint32_t*) (uint32_t) CLI_GetHexString(argv[1]);
    uint32_t d = CLI_GetHexString(argv[2]);
	*v = d;

    CLI_Printf("\r\n0x%08X: 0x%08X", (int) v, (int) *v);

	return TE_OK;
}

uint8_t _cpurb_cmd(char** argv, uint8_t argc)
{
	if (argc < 3)
		return TE_ArgErr;

    uint32_t* v = (uint32_t*) CLI_GetHexString(argv[1]);
    uint32_t c = CLI_GetDecString(argv[2]);

	for(uint32_t i = 0; i < c; i++)
	{
        CLI_Printf("\r\n0x%08X: 0x%08X", (int32_t) v, (int32_t) *v);
		v++;
	}


	return TE_OK;
}

// ************************************************************************

void _PrintTime(CLI_Time_s* time)
{
	CLI_Printf("\r\n%02d:%02d:%02d.%03d", (int) t.hour, (int) t.minute, (int) t.second, (int) t.msec);
}

/// \brief ����� �������� ������� � ��������
/// \return none
void CLI_PrintTime()
{
	uint32_t ms = Terminal_GetMs();
    CLI_Time_s t = CLI_GenerateTimeMSec(CLI_GetTimeMSec(&_def_time) + ms);
    _PrintTime(&t)
}

/// \brief ����� �������� ������� � �������� ��� �������� ������
/// \return none
void CLI_PrintTimeWithoutRN()
{
	uint32_t ms = Terminal_GetMs();
    CLI_Time_s t = CLI_GenerateTimeMSec(CLI_GetTimeMSec(&_def_time) + ms);
	_PrintTime(&t)
}

static void _UpdateCmd(const char* newCmd)
{
    CLI_PutChar('\r');
	printArrowWithoutN();

	cli_memcpy(Terminal.buf_enter, newCmd, TERM_CMD_BUF_SIZE);
	Terminal.buf_cntr = TERM_CMD_BUF_SIZE;


	uint8_t remCntr = 0;
	for(uint8_t i = 0; i < TERM_CMD_BUF_SIZE; i++)
	{
		if ((Terminal.buf_enter[i] == '\0') && (Terminal.buf_cntr == TERM_CMD_BUF_SIZE))
		{
			Terminal.buf_cntr = i;
		}

		if (i >= Terminal.buf_cntr)
		{
			remCntr++;
            CLI_PutChar(' ');
		}
		else
            {CLI_PutChar(Terminal.buf_enter[i]);}
	}

	Terminal.buf_curPos = Terminal.buf_cntr;
	for(uint8_t i = 0; i < remCntr; i++)
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

		// ��������� ��������� �������
		uint8_t tmpPos = Terminal.buf_curPos - 1;

		// ������� ��� ����������� ������� �����
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

/// \brief �������� ����� ������ � ����� ���������
/// \param {char} c - ����������� ������
/// \return {TC_Result_e} - ��������� �������� ���������� �������
TC_Result_e CLI_EnterChar(char c)
{
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

	uint8_t arr_up[] = {0x1B, 0x5B, 0x41};
	uint8_t arr_down[] = {0x1B, 0x5B, 0x42};
	uint8_t arr_right[] = {0x1B, 0x5B, 0x43};
	uint8_t arr_left[] = {0x1B, 0x5B, 0x44};
	uint8_t arr_esc[] = {0x1B, 0x1B, 0x1B};

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

	bool isValidKey = ((Terminal.buf_cntr < TERM_CMD_BUF_SIZE) ||
						(c == TERM_KEY_BACKSPACE) ||
						(c == TERM_KEY_ENTER)	||
						(c == CHAR_INTERRUPT));

	bool isAlphaBet = (((c > 0x2F) && (c < 0x3A)) ||
						((c > 0x60) && (c < 0x7B)) ||
						((c > 0x40) && (c < 0x5B)) ||
						(c == 0x20) || (c == '_') || (c == '-'));

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
				
				if (enter_callback != NULL)
				{
					enter_callback((uint8_t*)Terminal.buf_enter, (uint16_t)Terminal.buf_cntr + 1);
				}	

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
			case TERM_KEY_RESET:		{return TC_Reset;}break;
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

/// \brief ������ ������� ���������� ����� �������
/// \param {(*)(uint8_t, uint16_t)} - ������� ����������
/// \return none
void CLI_SetEnterCallback(void (*fcn)(uint8_t* rx_buf, uint16_t length))
{
	enter_callback = fcn;
}