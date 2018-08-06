#include "irq.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#define COUNT_VECTORS		2

static struct
{
	struct
	{
		void (*interrupts)();
		bool (*checks)();
	}Handlers[COUNT_VECTORS];
	bool enable;
}IRQ;

static DWORD WINAPI threadInterrupts(CONST LPVOID lpParam) 
{
	while(true)
	{
		if (IRQ.enable)
		{
			for(int i = 0; i < COUNT_VECTORS; i++)
			{
				if ((IRQ.Handlers[i].checks != NULL) && (IRQ.Handlers[i].checks()))
				{
					IRQ.Handlers[i].interrupts();
				}
			}	
		}
		
		Sleep(10);
	}
}

void IRQ_Enable()	{ IRQ.enable = true; }
void IRQ_Disable()	{ IRQ.enable = false; }

void IRQ_SetVector(int vector, void (*handler)(), bool (*check)())
{
	IRQ.Handlers[vector].interrupts = handler;
	IRQ.Handlers[vector].checks = check;
}

void IRQ_Init()
{
	IRQ_Disable();
	HANDLE threadInterrupt = CreateThread(NULL, 0, &threadInterrupts, NULL, 0, NULL);
}
