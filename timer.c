#include "timer.h"
#include <LPC23xx.h>

void irq_interrupt_handler(void);

void timer_init(void)
{
  /* Enable and setup timer interrupt, start timer                            */
  T0MR0         = 11999;                       /* 1msec = 12000-1 at 12.0 MHz */
  T0MCR         = 3;                           /* Interrupt and Reset on MR0  */
  T0TCR         = 1;                           /* Timer0 Enable               */
  VICVectAddr4  = (unsigned long)irq_interrupt_handler;/* Set Interrupt Vector        */
  VICVectPriority4  = 15;                      /* use it for Timer0 Interrupt */
  VICIntEnable  = (1  << 4);                   /* Enable Timer0 Interrupt     */
}
