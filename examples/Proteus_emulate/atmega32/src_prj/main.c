#include "global_inc.h"
#include "lib\uart\uart.h"

#include "terminal_config.h"
#include "terminal.h"

char dbgbuffer[TERM_CMD_BUF_SIZE];

uint8_t _t1_cmd()
{
	uint32_t decVal = CLI_GetArgDec(0);
	uint32_t hexVal = 0;

	CLI_GetArgHexByFlag("-h", &hexVal);

	CLI_Printf("\r\nDec: %d\r\nHex: 0x%04X%04X", (int) decVal, 
			(unsigned int) (hexVal >> 16),
			(unsigned int) hexVal);

	if (CLI_IsArgFlag("-f"))
		CLI_Printf("\r\n-f: TRUE")
	else
		CLI_Printf("\r\n-f: FALSE");
	
	return TE_OK;
}

int main (void)
{
    UART_Init(57600);

    CLI_Init(TDC_None);
	CLI_AddCmd("t1", _t1_cmd, 1, 0, "");
    
    while(1)
	{
		if (UART_IsStatus(1 << 7))
		{
			CLI_EnterChar(UART_GetChar());
		}
		CLI_Execute();
	}
}

