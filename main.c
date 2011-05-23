#include <LPC23xx.H>                    /* LPC23xx definitions                */

#include <stdio.h>
#include <stdlib.h>
#include <rtos.h>

#include <led/led.h>
#include <serial/serial.h>
#include <lcd/lcd.h>
#include "timer.h"
#include "rwLock.h"

semaphoreObject_t   testSemaphore;

void functionLowThread( mutexObject_t * const lock);
void functionMedThread(void);
void functionHighThread( mutexObject_t * const lock);

int32 stack[3][1000];
threadObject_t threads[3];
mutexObject_t mutex;

int main(void)
{
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

   mutexObjectInit( &mutex, 1);

   threadObjectCreate(&threads[0],
                     (void *)functionLowThread,
                     (int32)&mutex,
                     0,
                     0,
                     0,
                     &stack[0][1000],
                     30,
                     INITIAL_CPSR_ARM_FUNCTION,
                     "Lthread");
   threadObjectCreate(&threads[1],
                     (void *)functionMedThread,
                     0,
                     0,
                     0,
                     0,
                     &stack[1][1000],
                     20,
                     INITIAL_CPSR_ARM_FUNCTION,
                     "Mthread");
   threadObjectCreate(&threads[2],
                     (void *)functionHighThread,
                     (int32)&mutex,
                     0,
                     0,
                     0,
                     &stack[2][1000],
                     10,
                     INITIAL_CPSR_ARM_FUNCTION,
                     "Hthread");

   printf("<Creating Threads\n");

   srand(1);

   timer_init();

   printf("==Scheduler==\n");
   scheduler();            //This function will never return.
}

void functionLowThread( mutexObject_t * const lock)
{
   mutexObjectLock( lock, -1 );
   printf("Low Priority\n");
   sleep( 600 );
   printf("About to release lock\n");
   mutexObjectRelease( lock );
   while(1);
}

void functionMedThread()
{
   sleep( 400 );
   printf("Evil Medium Thread!!!\n");
   while(1);
}

void functionHighThread( mutexObject_t * const lock)
{
   sleep( 600 );
   printf("Try to get lock!!!\n");
   mutexObjectLock( lock, -1 );
   printf("Got the lock!!!\n");
   while(1);
}

