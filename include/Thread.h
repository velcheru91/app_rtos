/*
 * task.h
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 */

#ifndef APP_RTOS_INCLUDE_THREAD_H_
#define APP_RTOS_INCLUDE_THREAD_H_


//#define MAX_TIMERS          2
//#define TIMER_COUNT(_timer)  (_timer.count + 0)
//#define TIMER_RESET(_timer)  _timer.load = 0        \
//                             _timer.count = 0       \
//                             _timer.running = FALSE
//#define TIMER_STOP(_timer)   _timer.running = FALSE
//#define TIMER_START(_timer)  _timer.running = TRUE
//#define TIMER_LOAD(_timer,_load) _timer.load = _load  \
//                                 _timer.count = _load

//extern struct timers tmr[MAX_TIMERS];
//extern struct timers *tmr1, *tmr2, *tmr3;

void Timer_Init(void);
uint8_t readPbs(void);
void red_on(void);
void red_off(void);
void idle(void);
void idle_2(void);
void flash4Hz(void);
void oneshot(void);
void partOfLengthyFn(void);
void lengthyFn(void);
void readKeys(void);
void debounce(void);
void uncooperative(void);
void Timer_routine(void);
#endif /* APP_RTOS_INCLUDE_THREAD_H_ */
