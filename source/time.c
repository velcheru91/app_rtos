/*
 * time.c
 *
 *  Created on: Sep 8, 2016
 *      Author: Venugopal Velcheru
 */


#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>
#include "hal_init.h"
#include "time.h"
#include "config.h"
// Approximate busy waiting (in units of microseconds), given a 40 MHz system clock
void delayMicrosecond(uint32_t us)
{
	                                            // Approx clocks per us
	__asm("WMS_LOOP0:   MOV  R1, #6");          // 1
    __asm("WMS_LOOP1:   SUB  R1, #1");          // 6
    __asm("             CBZ  R1, WMS_DONE1");   // 5+1*3
    __asm("             NOP");                  // 5
    __asm("             B    WMS_LOOP1");       // 5*3
    __asm("WMS_DONE1:   SUB  R0, #1");          // 1
    __asm("             CBZ  R0, WMS_DONE0");   // 1
    __asm("             B    WMS_LOOP0");       // 1*3
    __asm("WMS_DONE0:");                        // ---
                                                // 40 clocks/us + error
}
