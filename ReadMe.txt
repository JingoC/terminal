/*********************************************************/
/*				Example cli-embedded 				
/*********************************************************

#include <stdio.h>
#include <stdlib.h>

#include "com/com.h"

#include "terminal_config.h"
#include "terminal.h"

uint8_t _t1_cmd(char** argv, uint8_t argc)
{
	if (argc < 2)
		return TE_ArgErr;
	
	// variables arguments
	int8_t w = CLI_IndexOfFlag("-b");
	if (w >= 0)
		CLI_Printf("\r\nB: 0x%08X", CLI_GetHexString(argv[w+1]));
	
	
	uint32_t v = CLI_GetDecString(argv[1]);
	CLI_Printf("\r\nARG: %d", v);
	
	return TE_OK;
}

int main(int argc, char *argv[]) {
	
	if (COM_Init("COM1"))
	{
		// Initialize Terminal
		CLI_Init(TDC_Time);
		
		// Add Terminal Command
		CLI_AddCmd("t1", _t1_cmd, "t1 - descr");
	
		char c;
		while(1)
		{
	        if (COM_Recieve(&c))
	        	CLI_EnterChar(c);	// Entered character in Terminal
			
			CLI_Execute();	// Execute command
	    }
	}
	
	return 0;
}
**************************************************************/