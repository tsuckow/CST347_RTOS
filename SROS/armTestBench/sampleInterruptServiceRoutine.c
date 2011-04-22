#include "rtos.h"
#include <LPC23xx.H>
#include <stdio.h>

void irq_interrupt_service_routine(void)
{
	if( T0IR & 0x01 ) //If Timer 0 Match
	{	
		T0IR = 1;//Reset Interrupt
		
		timerTick();//Tell SROS about the timer tick.
	}
	
	VICVectAddr /*VICAddress*/ = 0; /*Ack the interrupt*/
    return;
}
