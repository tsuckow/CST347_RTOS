#include <LPC23xx.H>                    /* LPC23xx definitions                */

#include <stdio.h>
#include <stdlib.h>
#include <rtos.h>

#include <led/led.h>
#include <serial/serial.h>
#include <lcd/lcd.h>
#include "timer.h"

semaphoreObject_t   testSemaphore;

threadObject_t thread1, thread2;
void function1(volatile uint8 * const counter);
void function2(volatile uint8 * const counter);
int32 stack1[1000], stack2[1000];

int main(void)
{
   uint8 mycounter = 0;
   
	LED_Init();
	LED_Out(0x55);

	init_serial();

  lcd_init();
  lcd_clear();
  lcd_print ("   SROS  DEMO   ");
  set_cursor (0, 1);
  lcd_print (">>>>>>>><<<<<<<<");

	printf("\n\n\n\n==MAIN==\n");
	
	printf(">Init\n");
    rtosInit();
	printf("<Init\n");

    printf(">Creating Threads\n");
	
    threadObjectCreate(&thread1,
                        (void *)function1,
                        (int32)&mycounter,
                        0,
                        0,
                        0,
                        &stack1[1000],
                        2,
                        INITIAL_CPSR_ARM_FUNCTION,
                        "thread1");
                        
    threadObjectCreate(&thread2,
                        (void *)function2,
                        (int32)&mycounter,
                        0,
                        0,
                        0,
                        &stack2[1000],
                        2,
                        INITIAL_CPSR_ARM_FUNCTION,
                        "thread2");
    printf("<Creating Threads\n");
	        
   srand(1);
   
   timer_init();

   printf("==Scheduler==\n");
   scheduler();            //This function will never return.
}                       
                        
                        
void function1(volatile uint8 * const counter)
{
    while(1)
    {
		++(*counter);
		LED_Out(*counter);
    }
}
                    
void function2(volatile uint8 * const counter)
{
	while(1)
	{
    	printf( "Count: %d\n", *counter );
	}
}

void DoDebug(void)
{
	printf("!!!DoDebug\n");
}
