#include "global_inc.h"

#include "lib/cmsis/cmsis_boot/LPC12xx.h"
#include "cmsis_lib/lpc12xx_iocon.h"		// AF
#include "cmsis_lib/lpc12xx_sysctrl.h"		// CLK
#include "cmsis_lib/lpc12xx_uart.h"			// UART

#include "core_cm0.h"
#include "core_cmFunc.h"

#include "terminal_config.h"
#include "terminal.h"

#include "my_test_commands/my_test_commands.h"

char dbgbuffer[256];

// GPIO0_8 , GPIO0_9
#define TERMINAL_USART			LPC_UART1

void _reset_fcn()
{
	NVIC_SystemReset();
}

inline void TUSART_PutChar(char c)
{
	while(!(TERMINAL_USART->LSR & (1 << 5))){};
	TERMINAL_USART->THR = c;
}

void TUSART_Print(const char* str)
{
	uint16_t co = 0;

	while(((str+co) != NULL) && ((*(str+co) != '\0')))
	{
		TUSART_PutChar(str[co]);
		co++;
	}
}

void _InitHW()
{
	SystemCoreClockUpdate();

	LPC_WDT->MOD &= ~(1 << 0);	// wdt disable
	SysTick->CTRL = 3;			// start systick interrupt

	SysTick_Config(360);		// [max 0x00ffffff], div for systick

	// uart init
	LPC_IOCON->PIO0_8 |= 0x2;		// AF RXD1
	LPC_IOCON->PIO0_9 |= 0x2;		// AF TXD1

	LPC_SYSCON->UART1CLKDIV = 1;

	LPC_SYSCON->SYSAHBCLKCTRL |= SYS_AHBCLKCTRL_UART1 | SYS_AHBCLKCTRL_GPIO0;

	UART_CFG_Type cfg;
	cfg.baudrate = 115200;
	cfg.databits = UART_CFG_DATABIT_8;
	cfg.stopbits = UART_CFG_STOPBIT_1;
	cfg.parity = UART_CFG_PARITY_NONE;
	cfg.fifodma = UART_CFG_DMAMODE_DISABLE;
	cfg.txdbreak = UART_CFG_TXDBREAK_DISABLE;
	cfg.fifolevel = UART_CFG_FIFOTRG_4;

	UART_SetConfig(TERMINAL_USART, &cfg);

	TERMINAL_USART->TER = 0x80;
	TERMINAL_USART->FCR |= 0x7;

	NVIC_EnableIRQ(UART1_IRQn);
	TERMINAL_USART->IER |= (UART_INTCFG_RBR);


	LPC_SYSCON->SYSAHBCLKCTRL |= SYS_AHBCLKCTRL_IOCON;

	__enable_irq();
}

void _InitSW()
{
	CLI_Init(TDC_Time);

	MyTestCmds_Init();
}

int main(void)
{
	_InitHW();
	_InitSW();

    while(1)
    {
    	CLI_Execute();
    }
}

void UART1_IRQHandler()
{
	TC_Result_e result = CLI_EnterChar(TERMINAL_USART->RBR);
	if (result == TC_Reset)
	{
		RESET_FCN();
	}
}

volatile uint64_t SysTickCtr = 0;
void SysTick_Handler()
{
	SysTickCtr++;
}
