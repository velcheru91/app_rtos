/*
 * task.c
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 */

#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>
#include <config.h>
#include <hal_init.h>
#include <sys_clk.h>
#include <kernel.h>
#include <task.h>
// ------------------------------------------------------------------------------
//  Task functions
// ------------------------------------------------------------------------------
// one task must be ready at all times or the scheduler will fail
// the idle task is implemented for this purpose

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
    BLUE_LED = 1;
    delayMicrosecond(1000);
    BLUE_LED = 0;
    yield();
  }
}
void red_on()
{
	while(true)
	{
	  RED_LED = 1;
	  delayMicrosecond(100000);
	  //RED_LED = 0;
	  yield();
	}
}
void red_off()
{
	while(true)
	{
	  RED_LED = 0;
	  delayMicrosecond(100000);
	  //BLUE_LED = 1;
	  yield();
	}
}
void flash4Hz()
{
  while(true)
  {
    GREEN_LED ^= 1;
    sleep(125);
  }
}

void oneshot()
{
  while(true)
  {
    wait(&flashReq);
//    YELLOW_LED = 1;
    sleep(1000);
//    YELLOW_LED = 0;
  }
}

void partOfLengthyFn()
{
  // represent some lengthy operation
	delayMicrosecond(1000);
  // give another process a chance
  yield();
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
    wait(&keyReleased);
    buttons = 0;
    while (buttons == 0)
    {
      buttons = readPbs();
      yield();
    }
    post(&keyPressed);
    if ((buttons & 1) != 0)
    {
//      YELLOW_LED ^= 1;
      RED_LED = 1;
    }
    if ((buttons & 2) != 0)
    {
      post(&flashReq);
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

    yield();
  }
}

void debounce()
{
  uint8_t count;
  while(true)
  {
    wait(&keyPressed);
    count = 10;
    while (count != 0)
    {
      sleep(10);
      if (readPbs() == 0)
        count--;
      else
        count = 10;
    }
    post(&keyReleased);
  }
}

void uncooperative()
{
  while(true)
  {
    while (readPbs() == 8)
    {
    }
    yield();
  }
}
