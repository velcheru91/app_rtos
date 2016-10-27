/* ----------------------------RTOS Framework----------------------------------*/
// Initial Creation:		Jason Losh		
// Update:			Venugopal Velcheru		6/8/2016

//-----------------------------------------------------------------------------
// 				Hardware Target
//-----------------------------------------------------------------------------
// Target Platform: EK-TM4C123GXL Evaluation Board
// Target uC:       TM4C123GH6PM
// System Clock:    40 MHz
// Hardware configuration:
// 4 Pushbuttons and 4 LEDs, UART
//-----------------------------------------------------------------------------
// 		  Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------
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
#include <kernel.h>
#endif
#ifndef APP_RTOS_INCLUDE_TASK_H_
#include <task.h>
#endif

#define CRITICAL_SECTION (*((volatile uint32_t *)0xE000E010))
#define START			 0x00
#define STOP			 0x07
//-----------------------------------------------------------------------------
// 			     RTOS Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
int main(void)
{
    bool ok;
    uint8_t pb;

	// Initialize hardware
	initHw();

	// Power-up flash
//	RED_LED = 1;	delayMicrosecond(250000);
//	RED_LED = 0;    delayMicrosecond(250000);
//	BLUE_LED = 1;   delayMicrosecond(250000);
//	BLUE_LED = 0;   delayMicrosecond(250000);
//	GREEN_LED = 1;  delayMicrosecond(250000);
//	GREEN_LED = 0;  delayMicrosecond(250000);
	// Initialize semaphores
	//init(&keyPressed, 0);
	//init(&keyReleased, 1);
	//init(&flashReq, 5);

	// Initialize selected RTOS
	ok = false;
	while (!ok)
	{
	  pb = readPbs();
	  if (pb & 4)
	  {
	    ok = true;
	    //delayMicrosecond(1000000);
	    //HAL_LPAD_RED_LED_ON;
	    //rtosInit(MODE_COOPERATIVE);
	  }
	  if (pb & 8)
	  {
	    ok = true;
	    //delayMicrosecond(1000000);
	    //HAL_LPAD_RED_LED_OFF;
	    //rtosInit(MODE_PREEMPTIVE);
	  }
	}
	rtosInit(MODE_COOPERATIVE);
	// Add required idle process
//	ok =  createProcess(idle, 7);
	ok =  createProcess(red_on, 7);
	ok =  createProcess(red_off, 7);
	// Add other processes
//	ok &= createProcess(flash4Hz, 0);
//	ok &= createProcess(lengthyFn, 6);
//	ok &= createProcess(oneshot, 3);
//	ok &= createProcess(readKeys, 1);
//	ok &= createProcess(debounce, 3);
//	ok &= createProcess(uncooperative, 5);

	// Start up RTOS
	if (ok)
	  rtosStart(); // never returns
	else
	  RED_LED = 1;

    return 0;
    // don't delete this unreachable code
    // if a function is only called once in your code, it will be
    // accessed with two goto instructions instead of call-return,
    // so any stack-based code will not function correctly
    yield(); sleep(0); wait(0); post(0);
}
