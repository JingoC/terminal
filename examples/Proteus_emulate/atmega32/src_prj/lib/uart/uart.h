#ifndef UART_H_
#define UART_H_

#include "../../global_inc.h"

void UART_Init(uint32_t baudrate);

void UART_PutChar(char c);
void UART_PutString(const char* str);

char UART_GetChar();

bool UART_IsStatus(uint8_t state);

#endif /* UART_H_ */
