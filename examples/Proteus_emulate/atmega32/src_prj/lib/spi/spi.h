#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
#include <stdbool.h>

void SPI_Init();

void SPI_SetData(uint8_t data);
uint8_t SPI_GetData();

bool SPI_IsStatus(uint8_t status);

void SPI_SetCS();
void SPI_ClrCS();

#endif // _SPI_H_
