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

void functionReadingThread(volatile uint32 * const counter, uint32 threadnum, rwLockObject_t * const lock);
void functionWritingThread(volatile uint32 * const counter, uint32 threadnum, rwLockObject_t * const lock);

#define READER_THREADS 1
#define WRITER_THREADS 1

int32 stack_readers[READER_THREADS][1000], stack_writers[WRITER_THREADS][1000];
threadObject_t threadReader[READER_THREADS], threadWriter[WRITER_THREADS];
uint32 critVal;
rwLockObject_t rwLock;

int main(void)
{
   unsigned int i;

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

   rwLockObjectInit( &rwLock );
   for(i = 0; i < READER_THREADS; ++i)
   {

      threadObjectCreate(&threadReader[i],
                        (void *)functionReadingThread,
                        (int32)&critVal,
                        i,
                        (int32)&rwLock,
                        0,
                        &stack_readers[i][1000],
                        1,
                        INITIAL_CPSR_ARM_FUNCTION,
                        "Rthread");
   }
   for(i = 0; i < WRITER_THREADS; ++i)
   {

      threadObjectCreate(&threadWriter[i],
                        (void *)functionWritingThread,
                        (int32)&critVal,
                        i,
                        (int32)&rwLock,
                        0,
                        &stack_writers[i][1000],
                        1,
                        INITIAL_CPSR_ARM_FUNCTION,
                        "Wthread");
   }

    printf("<Creating Threads\n");

   srand(1);

   timer_init();

   printf("==Scheduler==\n");
   scheduler();            //This function will never return.
}


void functionWritingThread(volatile uint32 * const counter, uint32 threadnum, rwLockObject_t * const lock)
{
   printf( "WriterThread%d: START\n", threadnum );
   while(1)
   {
      rwLockObjectLockWriter( lock );
      ++(*counter);
      printf( "WriterThread%d: %d\n", threadnum, *counter );
      rwLockObjectRelease( lock );
      yield();
   }
}

void functionReadingThread(volatile uint32 * const counter, uint32 threadnum, rwLockObject_t * const lock)
{
   printf( "ReaderThread%d: START\n", threadnum );
   while(1)
   {
      rwLockObjectLockReader( lock );
      printf( "ReaderThread%d: %d\n", threadnum, *counter );
      rwLockObjectRelease( lock );
   }
}

