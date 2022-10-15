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
		void (*interrupt)();
		bool (*check)();
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
				if ((IRQ.Handlers[i].check != NULL) && (IRQ.Handlers[i].check()))
				{
					IRQ.Handlers[i].interrupt();
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
	IRQ.Handlers[vector].interrupt = handler;
	IRQ.Handlers[vector].check = check;
}

void IRQ_Init()
{
	IRQ_Disable();
	CreateThread(NULL, 0, &threadInterrupts, NULL, 0, NULL);
}
