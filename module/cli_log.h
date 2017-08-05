#ifndef _TERMINAL_LOG_H_
#define _TERMINAL_LOG_H_

#include "terminal_config.h"

#include <stdint.h>
#include <stdbool.h>
#include "../lib/cli_queue.h"

void CLI_LogInit();

void CLI_LogCmdPush(const char* cmd);
const char* CLI_LogCmdGet(uint32_t index);

const char* CLI_GetNextCmd();
const char* CLI_GetLastCmd();

void CLI_CurReset();

#endif // _TERMINAL_LOG_H_
