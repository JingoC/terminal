#include <stdio.h>
#include <stdlib.h>

#include "com/com.h"
#include "systimer/systimer.h"
#include "irq/irq.h"

#include "terminal_config.h"
#include "terminal.h"

#include "commands/test_commands.h"

char dbgbuffer[256];
ITerminal* cli;

void IRQ_UartRxHandler()
{
	cli->EnterChar(COM_GetChar());
}

void InitSW()
{
	cli = CLI_CreateInstanse(TDC_Time);
	
	TestCommands_Init(cli);
}

void InitHW()
{
	COM_Init("COM1");
	STMR_Init();
	IRQ_SetVector(0, IRQ_UartRxHandler, COM_IsNotEmpty);
	IRQ_Init();
}

int main(int argc, char *argv[])
{
	InitHW();
	InitSW();
	IRQ_Enable();
		
	while(1)
	{
		cli->Execute();
	}
	
	return 0;
}

