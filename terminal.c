#include "terminal.h"

#include "lib/cli_queue.h"
#include "lib/cli_string.h"
#include "module/cli_time.h"
#include "module/cli_log.h"


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

/// \brief Проверка статуса прерывания операции (после чтения, статус очищается)
/// \return {bool} - true: требуется прервать операцию
inline bool CLI_GetIntState(){

	bool res = _interrupt_operation;
	_interrupt_operation = false;
	return res;
}


// ************************************************************************

// *********************** Terminal fcns **********************************

typedef struct{
    uint8_t (*fcn)(char** argv, uint8_t argc);			// Функция обработчик комманды
    const char* name;									// Имя команды, по которой вызывается функция
    const char* description;							// Описание команды (аргументы, описание действия и т.п.)
}TermCmd;		// Структура описывает команду терминала

struct{
	Queue_s symbols;							// очередь символов введенных последовательно
	char buf_enter[TERM_CMD_BUF_SIZE + 1];		// Буфер ввода
	char buf_enter_exec[TERM_CMD_BUF_SIZE + 1];	// Буфер ввода, при запущенной задачи
	int16_t buf_curPos;							// Текущая позиция курсора
	int16_t buf_cntr;							// Счетчика буфера ввода
	int16_t buf_cntr_exec;						// Счетчика буфера ввода при запущенной задаче
    TermCmd cmds[TERM_SIZE_TASK];				// Команды терминала
    uint8_t countCommand;						// Количество команд
    uint8_t executeState;						// Состояние терминала (запущена задача или нет)
}Terminal;		// Терминал

TermCmd* _findTermCmd(const char* cmdName);

static volatile args _input_args;

/// \brief Инициализация терминала
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
    Terminal.buf_enter[Terminal.buf_cntr] = '\0';
    Terminal.buf_enter[TERM_CMD_BUF_SIZE] = '\0';
    Terminal.buf_enter_exec[Terminal.buf_cntr_exec] = '\0';
    Terminal.buf_enter_exec[TERM_CMD_BUF_SIZE] = '\0';
    Terminal.executeState = 0;

    CLI_SetTime(&_def_time, 0, 0, 0);

    CLI_AddCmd("help", 		_help_cmd, 		"помощь по командам терминала");
    CLI_AddCmd("~", 		_reset_cpu, 	"reset cpu");

    if (defCmd & TDC_Time)
    {
        CLI_AddCmd("settime", 	_settime_cmd, 	"задать текущее время\n\r\tsettime [h] [m] [s]");
        CLI_AddCmd("gettime", 	_gettime_cmd, 	"вывести текущее время");
    }

    if (defCmd & TDC_CPU)
    {
        CLI_AddCmd("cpur", 		_cpur_cmd, 		"прочитать из адреса процессора\n\r\tcpur [addr]\n\r\t\taddr - 32-разрядное значение адреса, из которого читать");
        CLI_AddCmd("cpuw", 		_cpuw_cmd, 		"записать по адресу процессора\n\r\tcpuw [addr] [data]\n\r\t\taddr - 32-разрядное значение адреса, из которого читать\n\r\t\tdata - 32-разрядное значение записываемых данных");
        CLI_AddCmd("cpurb", 	_cpurb_cmd,		"прочитать блок из процессора начиная с адреса\n\r\tcpurb [addr] [length]\n\r\t\taddr - 32-разрядное значение адреса, из которого начинать чтение\n\r\t\tlength - количество читаемыхх регистров");
    }

    CLI_Printf("\r\nCount base command: %d", Terminal.countCommand);
    CLI_Printf("\r\nMax command: %d", TERM_SIZE_TASK);
    CLI_Printf("\r\n");

    _input_args.argv = (char**) cli_malloc(sizeof(char*) * TERM_ARGS_BUF_SIZE);
    for(uint8_t i = 0; i < TERM_ARGS_BUF_SIZE; i++)
        _input_args.argv[i] = cli_malloc(sizeof(char) * (TERM_ARG_SIZE + 1));

    Q_Init(&Terminal.symbols, 3, sizeof(char), QUEUE_FORCED_PUSH_POP_Msk);

#if (TERM_CMD_LOG_EN == 1)
    TERM_LogInit();
#endif

    printArrow();
}

/// \brief Выполнить команду
/// \param {const char*} str - строка с командой и набором аттрибутов к ней
/// \return {TE_Result_e} - Результат выполнения команды
TE_Result_e CLI_ExecuteString(const char* str)
{
	split((char*)str, " ", (args*) &_input_args);

	for(uint8_t i = 0; i < _input_args.argc;i++)
        CLI_DPrintf("\r\n: %s", _input_args.argv[i]);

    CLI_PrintTime();
    TE_Result_e result = CLI_Execute(_input_args.argv, _input_args.argc);
    CLI_PrintTime();

	ArgDestroy((args*)&_input_args);

	return result;
}

/// \brief Выполнить команду
/// \param {char*} argv - аргументы (комманда и аттрибуты)
/// \param {uint8_t} argc - количество аргументов
/// \return {TE_Result_e} - Результат выполнения команды
TE_Result_e CLI_Execute(char** argv, uint8_t argc)
{
    if (argc < 1)
        return TE_ArgErr;

    TermCmd* cmd = _findTermCmd(argv[0]);

    if (cmd != NULL){
    	Terminal.executeState = 1;
    	TE_Result_e result = cmd->fcn(argv, argc);
    	Terminal.executeState = 0;
    	return result;
    }

    return TE_NotFound;
}

/// \brief Функция добавления команды в терминал
/// \param {const char*} name - имя команды, по которой она будет выполняться в терминале
/// \param {uint8_t (*)(char**, uint8_t)} fcn - функция обработчик команды
/// \param {const char*} descr - описание команды (для команды help)
/// \return {TA_Result_e} - результат попытки добавить команду
TA_Result_e CLI_AddCmd(const char* name, uint8_t (*fcn)(char**, uint8_t), const char* descr)
{
    if (Terminal.countCommand >= TERM_SIZE_TASK)
        return TA_MaxCmd;

    if (fcn == NULL)
        return TA_FcnNull;

    if (_strlen((char*)name) == 0)
        return TA_EmptyName;

    uint8_t i = 0;
    for(; i < Terminal.countCommand; i++)
        if (_strcmp((char*) Terminal.cmds[i].name, (char*) name))
            return TA_RetryName;

    uint8_t countCmd = Terminal.countCommand;
    Terminal.cmds[countCmd].fcn = fcn;
    Terminal.cmds[countCmd].name = name;
    Terminal.cmds[countCmd].description = descr;
    Terminal.countCommand++;

    return TA_OK;
}

/// \brief Вывести результат выполнения команды в терминал
/// \param {uint8_t} code - код результата выполнения команды
/// \return none
void CLI_PrintResultExec(uint8_t code)
{
	switch(code){
        case TE_NotFound:	CLI_Printf("\n\rerr: Command not found");break;
        case TE_ArgErr: 	CLI_Printf("\n\rerr: Fault argument or Fault number arguments");break;
        case TE_ExecErr:	CLI_Printf("\n\rerr: Execute functions");break;
        case TE_WorkInt:	CLI_Printf("\n\rerr: Command abort");break;
		default:break;
	}

	printArrow();
}

/// \brief Проверка присутствия влага внутри команды
/// \param {char**} argv - набор параметров
/// \param {uint8_t} argc - количество параметров
/// \param {const char*} flag - искомый флаг (предусмотрено 2 типа флагов, с чертой ('-') перед флагом подразумевает наличие значения после флага, просто символический флаг, подразумевает осутствие значения.
/// \return int8_t - в случае наличия флага, возвращает его индекс среди параметров, в ином случае вернется -1.
int8_t CLI_GetValueByFlag(char** argv, uint8_t argc, const char* flag)
{
	for(uint8_t i = 0; i < argc; i++)
	{
		if (_strcmp(argv[i], flag))
		{
			if ((argc > (i + 1)) || (flag[0] != '-'))
				{return i;}
			else
				{return -1;}
		}
	}

	return -1;
}

/// \brief Вывод списка команд в отладочный терминал
/// \return none
void CLI_ViewCommandList(){
    CLI_DPrintf("\nList commands:");
    uint16_t i = 0;
    for(; i < Terminal.countCommand; i++){
        CLI_DPrintf("\n%-10s: 0x%X", Terminal.cmds[i].name, Terminal.cmds[i].fcn);
    }
}

/// \brief Поиск команды среди имеющихся в терминале
/// \param {const char*} cmdName - имя искомой команды
/// \return {TermCmd*} - Результат поиска (NULL, если команда не найдена)
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
    CLI_Printf("\r\n[] - обязательный аргумент\r\n<> - не обязательный аргумент\r\n| - выбор между аргументами");
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

    uint32_t* v = (uint32_t*) (uint32_t) CLI_GetHexString(argv[1]);

    CLI_Printf("\r\n0x%08X: 0x%08X", (int) v, (int) *v);

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

    uint32_t* v = (uint32_t*) (uint32_t) CLI_GetHexString(argv[1]);
    uint32_t c = CLI_GetDecString(argv[2]);

	for(uint32_t i = 0; i < c; i++)
	{
        CLI_Printf("\r\n0x%08X: 0x%08X", (int) v, (int)*v);
		v++;
	}


	return TE_OK;
}

// ************************************************************************

/// \brief Вывод текущего времени в терминал
/// \return none
void CLI_PrintTime()
{
	uint32_t ms = Terminal_GetMs();
    CLI_Time_s t = CLI_GenerateTimeMSec(CLI_GetTimeMSec(&_def_time) + ms);
    CLI_Printf("\r\n%02d:%02d:%02d.%03d", (int) t.hour, (int) t.minute, (int) t.second, (int) t.msec);
}

/// \brief Вывод текущего времени в терминал без перевода строки
/// \return none
void CLI_PrintTimeWithoutRN()
{
	uint32_t ms = Terminal_GetMs();
    CLI_Time_s t = CLI_GenerateTimeMSec(CLI_GetTimeMSec(&_def_time) + ms);
    CLI_Printf("%02d:%02d:%02d.%03d", (int) t.hour, (int) t.minute, (int) t.second, (int) t.msec);
}

static void _UpdateCmd(const char* newCmd)
{
    CLI_PutChar('\r');
	printArrowWithoutN();

	memcpy(Terminal.buf_enter, newCmd, TERM_CMD_BUF_SIZE);
	Terminal.buf_cntr = TERM_CMD_BUF_SIZE;


	uint8_t remCntr = 0;
	for(uint8_t i = 0; i < TERM_CMD_BUF_SIZE; i++)
	{
		if ((Terminal.buf_enter[i] == '\0') && (Terminal.buf_cntr == TERM_CMD_BUF_SIZE))
		{
			Terminal.buf_cntr = i;
		}

		if (i > Terminal.buf_cntr)
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

		// сохраняем положение курсора
		uint8_t tmpPos = Terminal.buf_curPos - 1;

		// смещаем все последующие символы влево
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

/// \brief Добавить новый символ в буфер терминала
/// \param {char} c - добавляемый символ
/// \return {TC_Result_e} - результат операции добавления символа
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
    CLI_Printf("\r\nKey Code: 0x%02X", c);
#endif
	if (isValidKey)
	{
		switch(c)
		{
			case TERM_KEY_ENTER:
			{
				Terminal.buf_enter[Terminal.buf_cntr] = '\0';

				if (enter_callback != NULL)
					enter_callback((uint8_t*)Terminal.buf_enter, (uint16_t)Terminal.buf_cntr + 1);

#if (TERM_CMD_LOG_EN == 1)
				TERM_LogCmdPush(Terminal.buf_enter);
				TERM_CurReset();
#endif
				Terminal.buf_curPos = 0;
				Terminal.buf_cntr = 0;
				Terminal.buf_enter[Terminal.buf_cntr] = '\0';

				return TC_Enter;
			}break;
			case CHAR_INTERRUPT:		{_interrupt_operation = true;TerminalTx("\r\nKey ESC");}break;
			case TERM_KEY_BACKSPACE:	{if (Terminal.buf_curPos > 0){_RemChar();}}break;
			case TERM_KEY_RESET:		{return TC_Reset;}break;
			case TERM_KEY_DOWN:
			{
#if (TERM_CMD_LOG_EN == 1)
				const char* ptrCmd = TERM_GetNextCmd();
				if (ptrCmd != NULL)
					{_UpdateCmd(ptrCmd);}
#endif
			}break;
			case TERM_KEY_UP:
			{
#if (TERM_CMD_LOG_EN == 1)
				const char* ptrCmd = TERM_GetLastCmd();
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

/// \brief Задать функцию обработчик новой команды
/// \param {(*)(uint8_t, uint16_t)} - фукнция обработчик
/// \return none
void CLI_SetEnterCallback(void (*fcn)(uint8_t* rx_buf, uint16_t length))
{
	enter_callback = fcn;
}
