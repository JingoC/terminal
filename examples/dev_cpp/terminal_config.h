/*************************************************************************
 *
 *			Terminal configure file
 *
 *	---------------------------------------------------------------------
 *
 *	Examples in Readme.h file.
 *
 ************************************************************************/

#ifndef _TERMIANL_CONFIG_H_
#define _TERMIANL_CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>				// strtol, itoa

// ****************************** Code keys ********************************

#define _KEY_INIT(c)					((char)c)

#define TERM_KEY_ENTER					(_KEY_INIT(0x0D))		// Enter command symbol
#define TERM_KEY_BACKSPACE				(_KEY_INIT(0x7F))		// Delete character before cursor position
#define TERM_KEY_ESCAPE					(_KEY_INIT(0xF0))		// Exception execute command symbol
#define TERM_KEY_UP						(_KEY_INIT(0xF1))		// KeyUp symbol
#define TERM_KEY_RIGHT					(_KEY_INIT(0xF2))		// KeyRight symbol
#define TERM_KEY_DOWN					(_KEY_INIT(0xF3))		// KeyDown symbol
#define TERM_KEY_LEFT					(_KEY_INIT(0xF4))		// KeyLeft symbol
#define TERM_KEY_DEL					(_KEY_INIT(0xF5))		// Delete character after cursor position
#define TERM_KEY_RESET					'~'						// Reset CPU

// **************************************************************************

// ********************** Terminal Settings *********************************

#define _TERM_VER_				"v1.4"				// Terminal version
#define TERM_SIZE_TASK          (80)				// Max number of commands
#define TERM_CMD_BUF_SIZE		(80)				// Max number of character buffer string command
#define TERM_CMD_LOG_SIZE		(10)				// Max number of loging command
#define TERM_ARGS_BUF_SIZE		(20)				// Max number of arguments in one command
#define TERM_ARG_SIZE			(15)				// Max number character of one arguments
#define CHAR_INTERRUPT			TERM_KEY_ESCAPE 	// Abort execute command key-code symbol
#define STRING_TERM_ENTER		"\n\r"				// String new line
#define STRING_TERM_ARROW		">> "				// String arrow enter
#define RESET_FCN()									// Reset CPU Function

#define TERM_TIMELEFT_EN				(0)			// Calculate time
#define TERM_TX_RX_EN					(1)			// Terminal Printf (without this don,t work)
#define TERM_TX_RX_DEB_EN				(1)			// Addition debug printf
#define TERM_CMD_LOG_EN					(1)			// Command logging
#define TERM_LR_KEY_EN					(1)			// Move cursor left-rigth
#define TERM_DEFAULT_ALLOC_EN			(1)			// Default Memory Allocate functions
#define TERM_DEFAULT_STRING_EN			(1)			// Default String functions
#define TERM_PRINT_ERROR_EXEC_EN		(1)			// Print error after execute command
#define TERM_PRINT_ERROR_ADD_CMD_EN		(1)			// Print error after added command
#define ECHO_EN							(1)			// Enter echo enable

// **************************************************************************

// ************************* IO Terminal Settings ***************************

#if (TERM_TX_RX_EN == 1)
#include <stdio.h>
extern void COM_Print(const char* str);
extern char dbgbuffer[256];
#define COM_Printf(...)		{sprintf(dbgbuffer,__VA_ARGS__);COM_Print(dbgbuffer);}
#define CLI_Printf			COM_Printf
extern void COM_Putc(char c);
#if (ECHO_EN == 1)
#define CLI_PutChar		COM_Putc
#else	// ECHO_EN != 1
#define CLI_PutChar
#endif	// ECHO_EN == 1

#else	// TERM_TX_RX_EN != 1
#define CLI_Printf
#define CLI_PutChar
#endif	// TERM_TX_RX_EN == 1
// **************************************************************************

// *********************** IO Debug Terminal Settings ***********************

#if (TERM_TX_RX_DEB_EN == 1)
#define CLI_DPrintf		printf
#else	// TERM_TX_RX_DEB_EN != 1
#define CLI_DPrintf
#endif	// TERM_TX_RX_DEB_EN == 1

// **************************************************************************

// ************************ Time calculate Settings *************************

#if (TERM_TIMELEFT_EN == 1)

	// yout implementation
extern volatile uint64_t SysTickCtr;							// Переменная содержит счетчик тактов

#define Terminal_GetUs()			((float)SysTickCtr * 10)	// Функция возвращающая системное время в us
#define Terminal_GetFastUs()		(SysTickCtr << 3)			// Функция возвращающая системное время в us (не точное)
#define Terminal_GetMs()			((float)SysTickCtr / 100)	// Функция возвращающая системное время в ms
#define Terminal_GetFastMs()		(SysTickCtr >> 7)			// Функция возвращающая системное время в ms (не точное)
#define SysTimeReset()				{SysTickCtr = 0;}			// Функция сбрасывающая системное время
#define delay_ms(ms)				{uint32_t ms_cntr_67 = Terminal_GetMs(); while(((uint32_t)Terminal_GetMs() - ms_cntr_67) < (ms)) {}}
#define delay_us(us)				{uint32_t us_cntr_67 = Terminal_GetUs(); while(((uint32_t)Terminal_GetUs() - us_cntr_67) < (us)) {}}

#else	// TERM_TIMELEFT_EN != 1

#define Terminal_GetUs()			(0)
#define Terminal_GetMs()			(0)
#define SysTimeReset()				{}
#define delay_ms(ms)				{}

#endif	// TERM_TIMELEFT_EN == 1

// **************************************************************************

// ********************** memory allocate functions *************************

#if (TERM_DEFAULT_ALLOC_EN == 1)
#include "lib/cli_malloc.h"
#define cli_malloc		_malloc
#define cli_free		_free
#else
#define cli_malloc		// your implementation
#define cli_free		// your implementation
#endif

// **************************************************************************

// *************************** string functions *****************************

#if (TERM_DEFAULT_STRING_EN == 1)
#include <string.h>
#define cli_memcpy		memcpy
#else
#define cli_memcpy		// your implementation
#endif

// **************************************************************************

#endif // _TERMIANL_CONFIG_H_
