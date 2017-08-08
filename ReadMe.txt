/*********************************************************/
/*				Example cli-embedded 				
/*********************************************************

#include <stdio.h>
#include <stdlib.h>

#include "com/com.h"

#include "terminal_config.h"
#include "terminal.h"

void ReceiveControl()
{
	if (COM_Recieve(&c))
	    CLI_EnterChar(c);	// Entered character in Terminal
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
		ReceiveControl();
		CLI_RetInt();
	}
	
	return TE_OK;
}

int main(int argc, char *argv[]) {
	
	if (COM_Init("COM1"))
	{
		// Initialize Terminal
		CLI_Init(TDC_Time);
		
		// Add Terminal Command
		CLI_AddCmd("t1", _t1_cmd, 1, TMC_PrintStartTime | TMC_PrintStopTime, "t1 - description command");
		CLI_AddCmd("t2", _t2_cmd, 0, TMC_PrintDiffTime, "t2 - description command");
	
		char c;
		while(1)
		{
	        ReceiveControl();
			CLI_Execute();		// Execute command
	    }
	}
	
	return 0;
}
**************************************************************/

example entered commands and result views:

>> t1 19 -b 0xA10032
00:xx:xx.xxx
a: 0x00000001
b: 0x00A10032
c: 19
00:xx:xx.xxx

>> t2
LT: 00:xx:xx.xxx
msg: Command abort

/*************************************************************/