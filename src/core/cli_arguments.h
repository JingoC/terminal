#ifndef _TERMINAL_ARGUMENTS_H
#define _TERMINAL_ARGUMENTS_H

#include "cli_types.h"

uint32_t CLI_GetArgDec(uint8_t index);
uint32_t CLI_GetArgHex(uint8_t index);
bool CLI_GetArgDecByFlag(const char* flag, uint32_t* outValue);
bool CLI_GetArgHexByFlag(const char* flag, uint32_t* outValue);

bool CLI_IsArgFlag(const char* flag);

void CLI_ArgDestroy(Params_s* src);

#endif // ARGUMENTS
