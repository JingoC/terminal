#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "terminal_config.h"
#include "core/cli_types.h"

ITerminal* CLI_CreateInstanse(TypeDefaultCmd_e defCmd);

#define CLI_CheckAbort(cli)			{ if (cli->GetIntState()){return TE_WorkInt;}}

#endif // _TERMINAL_H
