#include "timer1.h"

#include <avr\interrupt.h>
#include <avr\io.h>

#define TMR_OVF_TOFS		(65536-62439)

volatile uint16_t sec_cntr = 0;

void TMR1_Init()
{
	TCCR1B = 0x04;
    TCNT1 = TMR_OVF_TOFS;
    TIMSK = 0x04;
}

uint16_t TMR1_GetSec()
{
	return sec_cntr;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = TMR_OVF_TOFS;
	sec_cntr++;
}
