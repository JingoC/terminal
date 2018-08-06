#ifndef _COM_H_
#define _COM_H_

#include <stdint.h>
#include <stdbool.h>

bool COM_Init(const char* comName);

void COM_Putc(char c);
void COM_Print(const char* str);

bool COM_IsNotEmpty();

char COM_GetChar();

#endif // _COM_H_
