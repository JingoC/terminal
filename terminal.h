#ifndef TERMINAL_H
#define TERMINAL_H

#include "terminal_config.h"

#define printArrow()			{TerminalTx(STRING_TERM_ENTER);TerminalTx(STRING_TERM_ARROW);}	// вывод символов указывающих на ввод
#define printArrowWithoutN()	{TerminalTx(STRING_TERM_ARROW);}

typedef enum{
	TE_OK = 0,				// Все прошло успешно
	TE_Err = 1,				// Неизведанная ошибка
	TE_NotFound = 2,		// Функция не найдена
	TE_ArgErr = 10,			// Ошибки связанная с аргументами, при выполнении
	TE_ExecErr = 11,		// Ошибки в выполнении функции
	TE_WorkInt = 12			// Операция прервана в ручную
}TE_Result_e;	// Исходы операции работы задачи терминала (Terminal Error Result)

typedef enum{
	TA_OK = 0,				// Функция успешно добавлена
	TA_MaxCmd = 1,			// Количество функций терминало превышено
	TA_FcnNull = 2,			// Указатель на функцию NULL
	TA_EmptyName = 3,		// Пустой имя команды
	TA_RetryName = 4		// Имя команды уже существует
}TA_Result_e;	// Исходы операции добавления новой команды (Terminal Add Result)

typedef enum{
	TC_OK = 0,
	TC_Enter = 1,
	TC_BufFull = 2,
	TC_Reset = 3
}TC_Result_e; // Исход операции ввода нового символа (Terminal Char Result)

typedef enum{
	TDC_None = 0x0000,
	TDC_Time = 0x0001,
	TDC_CPU = 0x0002,
	TDC_All = 0xFFFF
}TypeDefaultCmd_e;

#define Terminal_RetInt()			{ if (Termial_GetIntState()){return TE_WorkInt;}}

void TerminalInit(TypeDefaultCmd_e defCmd);

TE_Result_e TerminalExec(char** argv, uint8_t argc);
TE_Result_e TerminalExecString(const char* str);

TA_Result_e TerminalAddCmd(const char* name, uint8_t (*fcn)(char**, uint8_t), const char* descr);

bool Termial_GetIntState();

void TerminalPrintTime(void);
void TerminalPrintTimeWithoutRN(void);
void TerminalPrintResultExec(uint8_t code);
void TerminalViewCommandList(void);

TC_Result_e TerminalPutChar(char c);

void TerminalSetEnterCallback(void (*fcn)(uint8_t* rx_buf, uint16_t length));

int8_t TerminalGetValueByFlag(char** argv, uint8_t argc, const char* flag);

#endif // TERMINAL_H
