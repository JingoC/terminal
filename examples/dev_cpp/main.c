#include <stdio.h>
#include <stdlib.h>

#include "com/com.h"

#include "terminal_config.h"
#include "terminal.h"

void ReceiveControl()
{
	char c = 0;
	if (COM_Recieve(&c))
	   CLI_EnterChar(c);
}

uint8_t _t1_cmd()
{	
	uint32_t a = 0x01;
	uint32_t b = 0x10;
	uint32_t c = 7;
	
	// be sure arguments
	c = CLI_GetArgDec(0);
	
	// optional arguments
	CLI_GetArgHexByFlag("-a", &a);
	CLI_GetArgHexByFlag("-b", &b);
	
	CLI_Printf("\r\na: 0x%08X\r\nb: 0x%08X\r\nc: %d", a, b, c);
	
	return TE_OK;
}

uint8_t _t2_cmd()
{
	while(1)
	{
		CLI_RetInt();
		ReceiveControl();
	}
	
	return TE_OK;
}

int main(int argc, char *argv[]) {
	
	if (COM_Init("COM1"))
	{	
		CLI_Init(TDC_Time);
		
		CLI_AddCmd("t1", _t1_cmd, 1, TMC_PrintStartTime | TMC_PrintStopTime, "t1 - description command");
		CLI_AddCmd("t2", _t2_cmd, 0, TMC_PrintDiffTime, "t2 - description command");
	
		while(1)
		{
			ReceiveControl();
			CLI_Execute();
	    }
	}
	
	return 0;
}

