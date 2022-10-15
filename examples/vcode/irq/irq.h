#ifndef _IRQ_H_
#define _IRQ_H_

#include <stdbool.h>

void IRQ_Enable();
void IRQ_Disable();

void IRQ_SetVector(int vector, void (*handler)(), bool (*check)());

void IRQ_Init();

#endif // _IRQ_H_
