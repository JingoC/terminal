#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include <stdint.h>

void TMP_Init();

uint8_t TMP_ReadReg(uint8_t reg);
void TMP_WriteReg(uint8_t reg, uint8_t value);

#endif // _TEMPERATURE_H_
