/*
 * kernel.c
 *
 *  Created on: Sep 9, 2016
 *      Author: Venugopal Velcheru
 *
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <config.h>
#include <kernel.h>
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
//-----------------------------------------------------------------------------
// RTOS Kernel
//-----------------------------------------------------------------------------
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
  // REQUIRED: systick for 1ms system timer
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
      tcb[i].sp = stack[i]; // REQUIRED: + offset as needed for the pre-loaded stack
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
void rtosStart()
{
  // REQUIRED: add code to call the first task to be run, restoring the preloaded context
  _fn fn;
  taskCurrent = rtosScheduler();
  // Add code to initialize the SP with tcb[task_current].sp;
  // Restore the stack to run the first process
}
void init(void* p, int count)
{
  s = p;
  s->count = count;
  s->queueSize = 0;
}
// REQUIRED: modify this function to yield execution back to scheduler
void yield()
{
	// push registers, call scheduler, pop registers, return to new function

}
// REQUIRED: modify this function to support 1ms system timer
// execution yielded back to scheduler until time elapses
void sleep(uint32_t tick)
{
	// push registers, set state to delayed, store timeout, call scheduler, pop registers,
	// return to new function (separate unrun or ready processing)
}
// REQUIRED: modify this function to wait a semaphore with priority inheritance
// return if avail (separate unrun or ready processing), else yield to scheduler
void wait(void* pSemaphore)
{
}
// REQUIRED: modify this function to signal a semaphore is available
void post(void* pSemaphore)
{
}



