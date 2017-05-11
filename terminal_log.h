#ifndef _TERMINAL_LOG_H_
#define _TERMINAL_LOG_H_

#include <stdint.h>
#include <stdbool.h>
#include <queue.h>

#include "terminal_config.h"

void TERM_LogInit();

void TERM_LogCmdPush(const char* cmd);
const char* TERM_LogCmdGet(uint32_t index);

const char* TERM_GetNextCmd();
const char* TERM_GetLastCmd();

void TERM_CurReset();

#endif // _TERMINAL_LOG_H_
