/*************************************************************************
 *
 * 				Конфигурационный файл терминала
 * 	Для использования данного терминала, требуется скопировать все
 * 	исходники терминала в проект, и произвести конфигурацию параметров
 * 	согласно используемому проекту.
 *
 * 	Основные параматры:
 *
 * 	TERM_SIZE_TASK 		- максимальное количество команд терминала
 * 	TERM_CMD_BUF_SIZE	- максимальная длина 1 вводимой команды
 *	ECHO_EN				- добавлиять эхо при приходе нового символа
 *
 *
 *	Для выдачи результата терминалом требуется заполнить макросы:
 *	TerminalTx			- функция реализующая вывод в ваш интерфейс
 *							должна иметь сигнатуру fcn(const char*, ...) (аналог printf)
 *	PutChar				- функция реализующая вывод 1 символа в ваш интерфейс
 *							должна иметь сигнатуру fcn(char) [требуется для echo]
 *
 *	Настройка времени:
 *	Временные отсчеты помещать в переменную с данным именем uint32_t SysTickCtr;
 *
 *	Переопределить макрос "Terminal_GetMs", чтобы он возвращал отсчитанное
 *	время в милисекундах.
 *
 *	---------------------------------------------------------------------
 *
 *	Пример работы терминала в файле Readme.h
 *
 ************************************************************************/

#ifndef _TERMIANL_CONFIG_H_
#define _TERMIANL_CONFIG_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>				// strtol, itoa

#define TERM_TIMELEFT_EN				(1)			// Расчет времени
#define TERM_TX_RX_EN					(1)			// Интерфейс терминала
#define TERM_TX_RX_DEB_EN				(0)			// Дополнительный отладочный интерфейс терминала
#define TERM_CMD_LOG_EN					(1)			// Лог введеных команд
#define TERM_LR_KEY_EN					(1)			// Перемещение каретки вправо-влево

// ******************************** keys ************************************

#define TERM_KEY_ENTER					(0x0D)		// Символ ввода команды
#define TERM_KEY_BACKSPACE				(0x08)		// Символ удаление последнего введеного символа
#define TERM_KEY_ESCAPE					(0xF0)		// Символ прерывание операции
#define TERM_KEY_UP						(0xF1)		//
#define TERM_KEY_RIGHT					(0xF2)		//
#define TERM_KEY_DOWN					(0xF3)		//
#define TERM_KEY_LEFT					(0xF4)		//
#define TERM_KEY_RESET					'~'			// Символ сброса cpu	(shift+Ё)

// **************************************************************************

// ********************** Настройки терминала *******************************

#define _TERM_VER_				"v1.3"				// Версия терминала
#define TERM_SIZE_TASK          (80)				// Максимальное количество задач
#define TERM_CMD_BUF_SIZE		(80)				// Максимальное количество символов в буффере вводимых команд
#define TERM_CMD_LOG_SIZE		(10)				// Лог последних введенных команд
#define TERM_ARGS_BUF_SIZE		(20)				// Максимальное количество аргументов к команде
#define TERM_ARG_SIZE			(15)				// Максиимальная длина одного аргумента
#define CHAR_INTERRUPT			TERM_KEY_ESCAPE 	// Символ прерывания операции
#define STRING_TERM_ENTER		"\n\r"				// Перенос строки
#define STRING_TERM_ARROW		">> "				// Указатель ввода ввода
#define ECHO_EN					(1)					// Включить echo
#define RESET_FCN				NVIC_SystemReset	// Функция сброса процессора

// **************************************************************************

// ********************** Настройки вывода в терминал ***********************

#if (TERM_TX_RX_EN == 1)

//#include "RS232_PC/rs232.h"			// HW настройки rs232
#include "debug.h"

//#define TerminalTx					RS232_TXDataStr			// void fcn(const char*)			// фукнция вывода в терминал
#define TerminalTx					DBG					// void fcn(const char*)			// фукнция вывода в терминал
#if (ECHO_EN == 1)
#define PutChar			            RS232_PutChar		// Короткая быстрая функция складывающая в fifo интерфейса символ
#else	// ECHO_EN != 1
#define PutChar
#endif	// ECHO_EN == 1

#else	// TERM_TX_RX_EN != 1
#define TerminalTx
#define PutChar
#endif	// TERM_TX_RX_EN == 1
// **************************************************************************

// ************** Настройки дополнительного отладочного вывода **************

#if (TERM_TX_RX_DEB_EN == 1)

#include "debug.h"					// здесь дебажный вывод

#define TerminalTxDebug			rtt_printf				// void fcn(str, ...)		// функция отладочного вывода при работе с терминалом

#else	// TERM_TX_RX_DEB_EN != 1

#define TerminalTxDebug

#endif	// TERM_TX_RX_DEB_EN == 1

// **************************************************************************

// ************************ Настройки вывода времени ************************

#if (TERM_TIMELEFT_EN == 1)

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

#define atoiDec(str)        strtoll((const char*)str, NULL, 10)
#define atoiHex(str)        strtoll((const char*)str, NULL, 16)

#endif // _TERMIANL_CONFIG_H_
