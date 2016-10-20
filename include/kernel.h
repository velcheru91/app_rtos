/*
 * kernel.h
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 */

#ifndef APP_RTOS_INCLUDE_KERNEL_H_
#define APP_RTOS_INCLUDE_KERNEL_H_


#define STATE_INVALID    0 // no task
#define STATE_READY      1 // ready to run
#define STATE_BLOCKED    2 // has run, but now blocked by semaphore
#define STATE_DELAYED    3 // has run, but now awaiting timer

// function pointer
typedef void (*_fn)();


void rtosInit(int mode);
int rtosScheduler();
bool createProcess(_fn fn, int priority);
void destroyProcess(_fn fn);
void rtosStart();
void init(void* p, int count);
void yield();
void sleep(uint32_t tick);
void wait(void* pSemaphore);
void post(void* pSemaphore);


#endif /* APP_RTOS_INCLUDE_KERNEL_H_ */
