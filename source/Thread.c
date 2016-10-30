/*
 * task.c
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 */

#ifndef _STDINT_H_
#include <stdint.h>
#endif
#ifndef _STDBOOL
#include <stdbool.h>
#endif
#ifndef __TM4C123GH6PM_H__
#include <tm4c123gh6pm.h>
#endif
#ifndef APP_RTOS_INCLUDE_CONFIG_H_
#include <config.h>
#endif
#ifndef APP_RTOS_INCLUDE_HAL_INIT_H_
#include <HAL.h>
#endif
#ifndef APP_RTOS_INCLUDE_KERNEL_H_
#include <OS.h>
#endif
#ifndef APP_RTOS_INCLUDE_TASK_H_
#include <Thread.h>
#endif

//-----------------------------------------------------------------------------
// 		  	 Task Variables
//-----------------------------------------------------------------------------
static struct timers
{
  uint32_t load;
  uint32_t count;
  bool running;
}T[MAX_TIMERS];

//timers tmr1,tmr2,tmr3;

//TIMER_START(tmr1);
//TIMER_START(tmr2);
//TIMER_START(tmr3);
//
//tmr1.load = 1000;
//tmr2.load = 2000;
//tmr3.load = 500;
//
//tmr1.count = tmr1.load;
//tmr2.count = tmr2.load;
//tmr3.count = tmr3.load;
//-----------------------------------------------------------------------------
// 			 Task functions
//------------------------------------------------------------------------------
// one task must be ready at all times or the scheduler will fail
// the idle task is implemented for this purpose


void Timer_Init()
{
	TIMER_START(T[0]);
	TIMER_START(T[1]);
	TIMER_START(T[2]);

	TIMER_LOAD(T[0],1000);
	TIMER_LOAD(T[1],2000);
	TIMER_LOAD(T[2],500);
}
// REQUIRED: add code to return a value from 0-15 indicating which of 4 PBs are pressed
uint8_t readPbs()
{
//	return ((8*(!PB4))+(4*(!PB3))+(2*(!PB2))+(!PB1));
	return ((8*(!HAL_LPAD_SW1_STATUS))+(4*(!HAL_LPAD_SW2_STATUS)));
}

void idle()
{
  while(true)
  {
	  if (TIMER_COUNT(T[2]) <= 0)
	  {
		  BLUE_LED ^= 1;
		  TIMER_LOAD(T[2],500);
	  }
    //delayMicrosecond(1000);
//    BLUE_LED = 0;
    //yield();
  }
}
void idle_2()
{
	while (true)
	{
		//BLUE_LED = 0;
	}
}
void red_on()
{
	while(true)
	{
		RED_LED ^= 1;
		OS_sleep(1000);
//		if (TIMER_COUNT(T[0]) <= 0)
//		{
//			RED_LED ^= 1;
//			TIMER_LOAD(T[0], 1000);
//		}
	  //delayMicrosecond(100000);
	  //RED_LED = 0;s
	  //yield();
	}
}
void red_off()
{
	while(true)
	{
	  RED_LED = 0;
	  //delayMicrosecond(100000);
	  //BLUE_LED = 1;
	  //yield();
	}
}
void flash4Hz()
{
  while(true)
  {
	  GREEN_LED ^= 1;
	  OS_sleep(2000);
//	  if (TIMER_COUNT(T[1]) <= 0)
//	  {
//		  GREEN_LED ^= 1;
//		  TIMER_LOAD(T[1],2000);
//	  }

//    sleep(125);
  }
}

void oneshot()
{
  while(true)
  {
//    wait(&flashReq);
//    YELLOW_LED = 1;
//    sleep(1000);
	  GREEN_LED = 0;
  }
}

void partOfLengthyFn()
{
  // represent some lengthy operation
	delayMicrosecond(1000);
  // give another process a chance
	OS_yield();
}

void lengthyFn()
{
  uint16_t i;
  while(true)
  {
    for (i = 0; i < 4000; i++)
    {
      partOfLengthyFn();
    }
    RED_LED ^= 1;
  }
}

void readKeys()
{
  uint8_t buttons;
  while(true)
  {
	  OS_wait(&keyReleased);
    buttons = 0;
    while (buttons == 0)
    {
      buttons = readPbs();
      OS_yield();
    }
    OS_signal(&keyPressed);
    if ((buttons & 1) != 0)
    {
//      YELLOW_LED ^= 1;
      RED_LED = 1;
    }
    if ((buttons & 2) != 0)
    {
    	OS_signal(&flashReq);
      RED_LED = 0;
    }
    if ((buttons & 4) != 0)
    {
      createProcess(flash4Hz, 0);
    }
    if ((buttons & 8) != 0)
    {
      destroyProcess(flash4Hz);
	}

    OS_yield();
  }
}

void debounce()
{
  uint8_t count;
  while(true)
  {
	  OS_wait(&keyPressed);
    count = 10;
    while (count != 0)
    {
    	OS_sleep(10);
      if (readPbs() == 0)
        count--;
      else
        count = 10;
    }
    OS_signal(&keyReleased);
  }
}

void uncooperative()
{
  while(true)
  {
    while (readPbs() == 8)
    {
    }
    OS_yield();
  }
}

void Timer_routine()
{
	if((TIMER_STATE(T[0]) == true) && (T[0].count>0))
		T[0].count--;

	if((TIMER_STATE(T[1]) == true) && (T[1].count>0))
		T[1].count--;

	if((TIMER_STATE(T[2]) == true) && (T[2].count>0))
		T[2].count--;

}
