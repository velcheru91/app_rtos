
/*
 * kernel.c
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 *
 *
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
#ifndef APP_RTOS_INCLUDE_THREAD_H_
#include <Thread.h>
#endif

//-----------------------------------------------------------------------------
// 		  	 Declerations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 		  	 RTOS  Kernel Variables
//-----------------------------------------------------------------------------
// variables
int rtosMode;              // mode
uint8_t taskCurrent = 0;   // index of last dispatched task
uint8_t taskCount = 0;     // total number of valid tasks


// task control block
struct _tcb
{
  uint8_t state;                 // see STATE_ values above
  void *pid;                     // used to uniquely identify process
  void *sp;                      // location of stack pointer for process
  uint8_t priority;              // 0=highest, 7=lowest
  uint8_t currentPriority;       // used for priority inheritance
  uint32_t ticks;                // ticks until sleep complete
} tcb[MAX_TASKS];

// semaphore
struct semaphore
{
  unsigned int count;
  unsigned int queueSize;
  unsigned int processQueue[MAX_QUEUE_SIZE]; // store task index here
} *s, keyPressed, keyReleased, flashReq;

// Process stack, each task is allowed with 256 Bytes in RAM at the time of creation
uint32_t stack[MAX_TASKS][256];
//-----------------------------------------------------------------------------
// RTOS Kernel
//-----------------------------------------------------------------------------
uint32_t read_sp(void)
{
	__asm("		MOV R0, SP");
	__asm("		BX  LR");
	return 0;
}
void write_sp(uint32_t p)
{
	__asm("		MOV SP, R0");
	__asm("		BX  LR");
}
void rtosInit(int mode)
{
  uint8_t i;
  rtosMode = mode;
  // no tasks running
  taskCount = 0;
  // clear out tcb records
  for (i = 0; i < MAX_TASKS; i++)
  {
    tcb[i].state = STATE_INVALID;
    tcb[i].pid = 0;
  }
  SysTick_Init();
  Timer_Init();
}
void rtosStart()
{
  // REQUIRED: add code to call the first task to be run, restoring the preloaded context
  _fn fn;
  taskCurrent = rtosScheduler();
  write_sp((uint32_t) (tcb[taskCurrent].sp));
  fn = (_fn) tcb[taskCurrent].pid;
  fn();
  // Add code to initialize the SP with tcb[task_current].sp;
  // Restore the stack to run the first process
}
int rtosScheduler()
{
  // REQUIRED: Implement prioritization to 8 levels
  bool ok;
  static uint8_t task = 0xFF;
  ok = false;
  while (!ok)
  {
    task++;
    if (task >= MAX_TASKS)
      task = 0;
    ok = (tcb[task].state == STATE_READY);
  }
  return task;
}
bool createProcess(_fn fn, int priority)
{
  bool ok = false;
  uint8_t i = 0;
  bool found = false;
  // REQUIRED: take steps to ensure a task switch cannot occur
  // save starting address if room in task list
  if (taskCount < MAX_TASKS)
  {
    // make sure fn not already in list (prevent reentrancy)
    while (!found && (i < MAX_TASKS))
    {
      found = (tcb[i++].pid ==  fn);
    }
    if (!found)
    {
      // find first available tcb record
      i = 0;
      while (tcb[i].state != STATE_INVALID) {i++;}
      tcb[i].state = STATE_READY;
      tcb[i].pid = fn;
      // REQUIRED: preload stack to look like the task had run before
      stack[i][137] = (uint32_t)tcb[i].pid;
      stack[i][139] = 0xFFFFFFF9;
      stack[i][143] = 0xFF000101;
      stack[i][145] = (uint32_t)tcb[i].pid;
      stack[i][146] = (uint32_t)tcb[i].pid;
      stack[i][147] = 0x01000000;
      tcb[i].sp = &stack[i][128]; // REQUIRED: + offset as needed for the pre-loaded stack
      tcb[i].priority = priority;
      tcb[i].currentPriority = priority;
      // increment task count
      taskCount++;
      ok = true;
    }
  }
  // REQUIRED: allow tasks switches again
  return ok;
}
// REQUIRED: modify this function to destroy a process
void destroyProcess(_fn fn)
{
}

void init(void* p, int count)
{
  s = p;
  s->count = count;
  s->queueSize = 0;
}
// REQUIRED: modify this function to yield execution back to scheduler
void OS_yield()
{
	// push registers, call scheduler, pop registers, return to new function
	__asm("		PUSH  {LR}");
	__asm(" 	PUSH  {R4, R5, R6, R7, R8, R9, R10, R11, R14}");
	__asm("   	PUSH  {R13}");
	tcb[taskCurrent].sp=(void *)read_sp();	// saving stack pointer
	taskCurrent = rtosScheduler();
	write_sp((uint32_t) (tcb[taskCurrent].sp));// restoring stack pointer
	__asm(" 	POP   {R13}");
	__asm(" 	POP   {R4, R5, R6, R7, R8, R9, R10, R11, R14}");
	__asm(" 	POP   {LR}");
}
// REQUIRED: modify this function to support 1ms system timer
// execution yielded back to scheduler until time elapses
void OS_sleep(uint32_t tick)
{
	__asm("     CPSID I");
	// push registers, call scheduler, pop registers, return to new function
	tcb[taskCurrent].state = STATE_DELAYED;
	tcb[taskCurrent].ticks = tick;
	__asm("     POP   {R3, LR}");
	__asm("     MOV   R6, #0x0000");
	__asm("     MOVT  R6, #0x0100");
	__asm("     PUSH  {R6}");
	__asm("     MOV   R6, LR");
	__asm("     SUB   R6, #0x01");
	__asm("     PUSH  {R6}");
	__asm("     PUSH  {LR, R12, R3, R2, R1, R0}");
	__asm("     MOV   R6, #0xFFF9");
	__asm("     MOVT  R6, #0xFFFF");
	__asm("     PUSH  {R6}");
	__asm("     PUSH  {R3}");
	__asm("		PUSH  {LR}");
	__asm(" 	PUSH  {R4, R5, R6, R7, R8, R9, R10, R11, R14}");
//	__asm("   	PUSH  {R13}");
	tcb[taskCurrent].sp=(void *)read_sp();	// saving stack pointer
	taskCurrent = rtosScheduler();
	write_sp((uint32_t) (tcb[taskCurrent].sp));// restoring stack pointer
//	__asm(" 	POP   {R13}");
	__asm(" 	POP   {R4, R5, R6, R7, R8, R9, R10, R11, R14}");
	__asm(" 	POP   {LR}");
	__asm("     POP   {R3, LR}");
	__asm("     POP   {R0, R1, R2, R3, R12, LR}");
	__asm("     POP   {R6}");
	__asm("     POP   {R5}");
	__asm("     MOV   R5, #0x00");
	__asm("     MSR   IPSR, R5");
	__asm("     CPSIE I");
	__asm("     MOV   PC, R6");
	// push registers, set state to delayed, store timeout, call scheduler, pop registers,
	// return to new function (separate unrun or ready processing)
}
// REQUIRED: modify this function to wait a semaphore with priority inheritance
// return if avail (separate unrun or ready processing), else yield to scheduler
void OS_wait(void* pSemaphore)
{
}
// REQUIRED: modify this function to signal a semaphore is available
void OS_signal(void* pSemaphore)
{
}

void SysTick_interrupt(void)
{
	int i;
	for (i = 0; i < MAX_TASKS; i++)
			{
				if (tcb[i].state == STATE_DELAYED)
				{
					tcb[i].ticks --;
					if (tcb[i].ticks == 0)
					{
		    		tcb[i].state = STATE_READY;
		    		//tcb[i].currentPriority = tcb[i].priority;
					}
				}
			}
	Timer_routine();
//	// push registers, call scheduler, pop registers, return to new function
	__asm("		PUSH  {LR}");
	__asm(" 	PUSH  {R4, R5, R6, R7, R8, R9, R10, R11, R14}");
//	__asm("   	PUSH  {R13}");
	tcb[taskCurrent].sp=(void *)read_sp();	// saving stack pointer
	taskCurrent = rtosScheduler();
	write_sp((uint32_t) (tcb[taskCurrent].sp));// restoring stack pointer
//	__asm(" 	POP   {R13}");
	__asm(" 	POP   {R4, R5, R6, R7, R8, R9, R10, R11, R14}");
	__asm(" 	POP   {LR}");
//	__asm("     MOV   R6, #0x0F");
//	__asm("     MSR   IPSR, R6");
//	__asm("     ADD   SP, #0x08");
//	__asm("     MOV   R3, #0x0201");
//	__asm("     MOVT  R3, #0xFF00");
//	__asm("     MOV   R6, #0xFFF9");
//	__asm("     MOVT  R6, #0xFFFF");
//	__asm("     MOV   PC, R6");
//	__asm(" 	POP   {R3, LR}");
}

