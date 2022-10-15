#include "cli_commands.h"

#include "cli_interface.h"
#include "cli_arguments.h"

#include "../module/cli_time.h"

extern volatile Terminal_s Terminal;

extern void CLI_PrintTime();

uint8_t _help_cmd()
{
    CLI_Printf("\r\nCount command: %d", (int) Terminal.countCommand);
    CLI_Printf("\r\n[] - mandatory argument\r\n<> - optional argument\r\n| - choice between arguments");
	
	for(uint16_t i = 1; i < Terminal.countCommand; i++)
	{
        CLI_Printf("\r\n\n%-10s - %s", Terminal.cmds[i].name, Terminal.cmds[i].description);
        CLI_Printf("\r\n-----------------------------------------------------------------");
	}

	return TE_OK;
}

uint8_t _reset_cpu()
{
	RESET_FCN();

	return TE_OK;
}

#if (TERM_TIMELEFT_EN == 1)
uint8_t _settime_cmd(ITerminalCommand* cli)
{
    uint32_t h = cli->GetArgDec(0);
    uint8_t m = cli->GetArgDec(1);
    uint8_t s = cli->GetArgDec(2);

	CLI_SetBaseTimeFromHMS(h, m , s);

	return TE_OK;
}

uint8_t _gettime_cmd()
{
    CLI_Printf("\r\nTime: ");
    CLI_PrintTime();

	return TE_OK;
}
#endif

uint8_t _cpur_cmd(ITerminalCommand* cli)
{
    uint32_t addr = cli->GetArgHex(0);
    uint32_t* v = (uint32_t*)(intptr_t) addr;

    CLI_Printf("\r\n0x%08X: 0x%08X", (int)(intptr_t) v, (int) *v);

	return TE_OK;
}

uint8_t _cpuw_cmd(ITerminalCommand* cli)
{
    uint32_t* v = (uint32_t*)(intptr_t) cli->GetArgHex(0);
    uint32_t d = cli->GetArgHex(1);
	*v = d;

	return TE_OK;
}

uint8_t _cpurb_cmd(ITerminalCommand* cli)
{
    uint32_t* v = (uint32_t*)(intptr_t) cli->GetArgHex(0);
    uint32_t c = cli->GetArgDec(1);

	for(uint32_t i = 0; i < c; i++)
	{
        CLI_Printf("\r\n0x%08X: 0x%08X", (int)(intptr_t) v, (int) *v);
		v++;
	}


	return TE_OK;
}

void CLI_CommandsInit(TypeDefaultCmd_e defCmd)
{
	ITerminal cli = Terminal.iTerminal;
	
	cli.AddCmd("help", 		_help_cmd, 		0, TMC_None, "help by terminal command");
    cli.AddCmd("~", 		_reset_cpu, 	0, TMC_None, "reset cpu");

#if (TERM_TIMELEFT_EN == 1)
    CLI_SetBaseTimeFromMs(0);
    if (defCmd & TDC_Time)
    {
        cli.AddCmd("settime", 	_settime_cmd, 	3, TMC_None, "set current time\n\r\tsettime [h] [m] [s]");
        cli.AddCmd("gettime", 	_gettime_cmd, 	0, TMC_None, "print current time");
    }
#endif

    if (defCmd & TDC_CPU)
    {
        cli.AddCmd("cpur", 		_cpur_cmd, 		1, TMC_None, "read from CPU reg\n\r\tcpur [addr]");
        cli.AddCmd("cpuw", 		_cpuw_cmd, 		2, TMC_None, "write in CPU reg\n\r\tcpuw [addr] [data]");
        cli.AddCmd("cpurb", 	_cpurb_cmd,		2, TMC_None, "read block from CPU\n\r\tcpurb [addr] [length]");
    }
}

