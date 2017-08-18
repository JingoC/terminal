#include "global_inc.h"

#include "lib\uart\uart.h"
#include "lib\spi\spi.h"
#include "lib\timer\timer1.h"

#include "lib\temp_sensor\temperature.h"

#include "test_cmds\test_cmds.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "terminal_config.h"
#include "terminal.h"

char dbgbuffer[TERM_CMD_BUF_SIZE];

void HW_Init()
{
	UART_Init(57600);
	SPI_Init();

	DDRC |= 0x03;

	TMP_Init();
	TMR1_Init();

	sei();
}

void SW_Init()
{
	CLI_Init(TDC_None);
	TestCmds_Init();
}

void MainLoop()
{
	if (UART_IsStatus(1 << 7))
	{
		CLI_EnterChar(UART_GetChar());
	}

	CLI_Execute();
}

int main (void)
{
    HW_Init();
	SW_Init();
    
    while(1)
	{
		MainLoop();
	}
}
