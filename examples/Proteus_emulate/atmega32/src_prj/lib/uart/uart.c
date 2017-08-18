#include "../../global_inc.h"

#include "uart.h"

#include <avr/io.h>

void UART_Init(uint32_t baudrate)
{
	DDRD = 0x02;
	
	uint16_t v = (FREQ) / (16 * baudrate) - 1;
	UBRRH = (v >> 8) & 0x0F;
	UBRRL = v;
	UCSRB = (1 << 4) | (1 << 3);
	UCSRC = (1 << 7) | (1 << 3) | (3 << 1);
}

void UART_PutChar(char c)
{
	while(!(UCSRA & (1 << 5))){}
	UDR = c;
}

void UART_PutString(const char* str)
{
	while(str != NULL)
	{
		if (*str == '\0')
		break;
		
		UART_PutChar(*str);
		str++;
	}
}

char UART_GetChar()
{
	return UDR;
}

bool UART_IsStatus(uint8_t state)
{
	return ((UCSRA & state)	== state);
}
