/*
 * task.h
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 */

#ifndef APP_RTOS_INCLUDE_TASK_H_
#define APP_RTOS_INCLUDE_TASK_H_

uint8_t readPbs(void);
void idle(void);
void flash4Hz(void);
void oneshot(void);
void partOfLengthyFn(void);
void lengthyFn(void);
void readKeys(void);
void debounce(void);
void uncooperative(void);

#endif /* APP_RTOS_INCLUDE_TASK_H_ */
