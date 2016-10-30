/*
 * kernel.h
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 */

#ifndef APP_RTOS_INCLUDE_OS_H_
#define APP_RTOS_INCLUDE_OS_H_


#define STATE_INVALID    0 // no task
#define STATE_READY      1 // ready to run
#define STATE_BLOCKED    2 // has run, but now blocked by semaphore
#define STATE_DELAYED    3 // has run, but now awaiting timer

// function pointer
typedef void (*_fn)();

// Process stack, each task is allowed with 256 Bytes in RAM at the time of creation
extern uint32_t stack[MAX_TASKS][256];

// semaphore
extern struct semaphore *s, keyPressed, keyReleased, flashReq;

#define MAX_TIMERS          3
#define TIMER_COUNT(_timer)  (_timer.count + 0)
#define TIMER_RESET(_timer)  _timer.load = 0 ;      \
                             _timer.count = 0       \
                             _timer.running = false
#define TIMER_STOP(_timer)   _timer.running = false
#define TIMER_START(_timer)  _timer.running = true;
#define TIMER_LOAD(_timer,_load) _timer.load = _load;  \
                                 _timer.count = _load
#define TIMER_STATE(_timer)  _timer.running

//extern static struct timers *tmr1, *tmr2, *tmr3;

void rtosInit(int mode);
int rtosScheduler();
bool createProcess(_fn fn, int priority);
void destroyProcess(_fn fn);
void rtosStart();
void init(void* p, int count);
void OS_yield();
void OS_sleep(uint32_t tick);
void OS_wait(void* pSemaphore);
void OS_signal(void* pSemaphore);


#endif /* APP_RTOS_INCLUDE_OS_H_ */
