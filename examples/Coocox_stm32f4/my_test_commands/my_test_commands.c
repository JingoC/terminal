#include "my_test_commands.h"

#include "terminal_config.h"
#include "terminal.h"

static uint8_t _t1_cmd();
static uint8_t _t2_cmd();

void MyTestCmds_Init()
{
	CLI_AddCmd("t1", _t1_cmd, 1, TMC_PrintStartTime | TMC_PrintStopTime, "t1 - description command");
	CLI_AddCmd("t2", _t2_cmd, 0, TMC_PrintDiffTime, "t2 - description command");
}

// ***************** implementation commands ****************

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

	CLI_Printf("\r\na: 0x%08X\r\nb: 0x%08X\r\nc: %d", (int) a, (int) b, (int) c);

	return TE_OK;
}

uint8_t _t2_cmd()
{
	CLI_Printf("\r\nPress ESC");
	while(1)
	{
		CLI_RetInt();
	}

	return TE_OK;
}
