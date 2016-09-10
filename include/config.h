/*
 * config.h
 *
 *  Created on: Sep 9, 2016
 *      Author: code_it
 */

#ifndef APP_RTOS_INCLUDE_CONFIG_H_
#define APP_RTOS_INCLUDE_CONFIG_H_


#define MAX_QUEUE_SIZE 10
#define MAX_TASKS 10       // maximum number of valid tasks
#define MODE_COOPERATIVE 0 // no priority
#define MODE_PREEMPTIVE  1 // based on task priority

// Process stack, each task is allowed with 256 Bytes in RAM at the time of creation
uint32_t stack[MAX_TASKS][256];
#endif /* APP_RTOS_INCLUDE_CONFIG_H_ */