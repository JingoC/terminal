#include "spi.h"

#include <avr\io.h>

void SPI_Init()
{
	DDRB = 0xB0;	

	SPCR |= 0x50 | (1 << 2);
}

void SPI_SetData(uint8_t data)
{
	SPDR = data;
}

uint8_t SPI_GetData()
{
	return SPDR;
}

bool SPI_IsStatus(uint8_t status)
{
	return ((SPSR & status) == status);
}

void SPI_SetCS()
{
	PORTB |= 0x10;
}

void SPI_ClrCS()
{
	PORTB &= ~0x10;
}
