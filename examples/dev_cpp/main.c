#include <stdio.h>
#include <stdlib.h>

#include "com/com.h"
#include "irq/irq.h"

#include "terminal_config.h"
#include "terminal.h"

static uint8_t _to_cmd();
static uint8_t _tf_cmd();

char dbgbuffer[256];

void IRQ_UartRxHandler()
{
	CLI_EnterChar(COM_GetChar());
}

void delay_cnt(uint32_t cnt)
{
	while(cnt-- > 0)
	{
		asm("nop");
	}
}

uint8_t _to_cmd()
{
	uint32_t count = 1;

	// be sure arguments
	count = CLI_GetArgDec(0);

	// optional arguments
	//CLI_GetArgHexByFlag("-b", &b);

	for(uint32_t i = 0; i < count; i++)
	{
		CLI_Printf("\r\n|  Test%d  |  OK  |", (int) i);
		delay_cnt(500000000);
	}

	return TE_OK;
}

uint8_t _tf_cmd()
{
	// be sure arguments
	uint32_t count = CLI_GetArgDec(0);

	// optional arguments
	//CLI_GetArgHexByFlag("-b", &b);

	for(uint32_t i = 0; i < count; i++)
	{
		if (i < 2) {
			CLI_Printf("\r\n|  Test%d  |  OK  |", (int) i);
		}
		else {
			CLI_Printf("\r\n|  Test%d  | FAIL |", (int) i);
		}
		delay_cnt(500000000);
	}
	return TE_OK;
}

int main(int argc, char *argv[]) {
	
	IRQ_SetVector(0, IRQ_UartRxHandler, COM_IsNotEmpty);
	IRQ_Init();
	
	if (COM_Init("COM1"))
	{	
		CLI_Init(TDC_Time);
		
		CLI_AddCmd("test_ok", _to_cmd, 1, TMC_PrintDiffTime, "test ok");
		CLI_AddCmd("test_fail", _tf_cmd, 1, TMC_PrintDiffTime, "test fail");
	
		IRQ_Enable();
		while(1)
		{
			CLI_Execute();
	    }
	}
	
	return 0;
}

