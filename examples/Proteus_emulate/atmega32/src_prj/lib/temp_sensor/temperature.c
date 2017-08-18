#include "temperature.h"

#include "lib\spi\spi.h"

void delay_t(uint16_t cntr)
{
	volatile uint16_t c = cntr;
	while(c-- > 0){}
}

void TMP_Init()
{
	
}

uint8_t TMP_ReadReg(uint8_t reg)
{
	volatile uint8_t data = 0;
	SPI_SetCS();

	SPI_SetData(reg);
	while(!SPI_IsStatus(0x80)){}

	SPI_SetData(0x00);
	while(!SPI_IsStatus(0x80)){}
	data = SPI_GetData();

	SPI_ClrCS();

	return data;
}

void TMP_WriteReg(uint8_t reg, uint8_t value)
{
	SPI_SetCS();

	SPI_SetData(reg);
	while(!SPI_IsStatus(0x80)){}
	
	SPI_SetData(value);
	while(!SPI_IsStatus(0x80)){}

	SPI_ClrCS();
}
