#include "test_commands.h"

uint8_t _to_cmd(ITerminalCommand* tcmd)
{
	uint32_t count = 1;

	// be sure arguments
	count = tcmd->GetArgDec(0);

	for(uint32_t i = 0; i < count; i++)
	{
		CLI_Printf("\r\n|  Test%d  |  OK  |", (int) i);
	}

	return TE_OK;
}

uint8_t _tf_cmd(ITerminalCommand* tcmd)
{
	// be sure arguments
	uint32_t count = tcmd->GetArgDec(0);

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

uint8_t _delay_cmd(ITerminalCommand* tcmd)
{
	uint32_t delayMs = (float) tcmd->GetArgDec(0);

	float ms = CLI_GetFastMs();
	while((CLI_GetFastMs() - ms) < delayMs) {}

	return TE_OK;
}

void TestCommands_Init(ITerminal* cli)
{
	cli->AddCmd("test_ok", _to_cmd, 1, TMC_PrintDiffTime, "test ok");
	cli->AddCmd("test_fail", _tf_cmd, 1, TMC_PrintDiffTime, "test fail");
	cli->AddCmd("delay", _delay_cmd, 1, TMC_PrintDiffTime|TMC_PrintStartTime|TMC_PrintStopTime, "delay in ms");
}
