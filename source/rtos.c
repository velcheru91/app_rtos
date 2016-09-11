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
#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>
#include <config.h>
#include <hal_init.h>
#include <sys_clk.h>
#include <kernel.h>
#include <task.h>

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
	    //ok = true;
	    //delayMicrosecond(1000000);
	    HAL_LPAD_RED_LED_ON;
	    //rtosInit(MODE_COOPERATIVE);
	  }
	  if (pb & 8)
	  {
	    //ok = true;
	    //delayMicrosecond(1000000);
	    HAL_LPAD_RED_LED_OFF;
	    //rtosInit(MODE_PREEMPTIVE);
	  }
	}

	// Add required idle process
	ok =  createProcess(idle, 7);

	// Add other processes
	ok &= createProcess(flash4Hz, 0);
	ok &= createProcess(lengthyFn, 6);
	ok &= createProcess(oneshot, 3);
	ok &= createProcess(readKeys, 1);
	ok &= createProcess(debounce, 3);
	ok &= createProcess(uncooperative, 5);

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
