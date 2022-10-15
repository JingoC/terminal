#ifndef _CLI_STRING_A_H
#define _CLI_STRING_A_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../core/cli_types.h"

void cli_strcpy(const char* src, uint16_t offsetSrc, char* dst, uint16_t offsetDst, uint16_t length);
char* cli_trim(const char* src);
uint8_t cli_strcmp(const char* str1, const char* str2);
uint8_t cli_strPartCmp(const char* str1, const char* str2);
uint32_t cli_strlen(const char* strSrc);

void cli_split(char* strSrc, const char* separator, Params_s* dst);

#endif // _CLI_STRING_A_H
