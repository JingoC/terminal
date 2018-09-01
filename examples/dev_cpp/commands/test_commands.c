#include "test_commands.h"

#include "terminal.h"

uint8_t _to_cmd()
{
	uint32_t count = 1;

	// be sure arguments
	count = CLI_GetArgDec(0);

	for(uint32_t i = 0; i < count; i++)
	{
		CLI_Printf("\r\n|  Test%d  |  OK  |", (int) i);
	}

	return TE_OK;
}

uint8_t _tf_cmd()
{
	// be sure arguments
	uint32_t count = CLI_GetArgDec(0);

	for(uint32_t i = 0; i < count; i++)
	{
		if (i < 2) {
			CLI_Printf("\r\n|  Test%d  |  OK  |", (int) i);
		}
		else {
			CLI_Printf("\r\n|  Test%d  | FAIL |", (int) i);
		}
	}
	return TE_OK;
}

void TestCommands_Init()
{
	CLI_AddCmd("test_ok", _to_cmd, 1, TMC_PrintDiffTime, "test ok");
	CLI_AddCmd("test_fail", _tf_cmd, 1, TMC_PrintDiffTime, "test fail");
}
