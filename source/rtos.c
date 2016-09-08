/* ---------------------------VeRTOS Framework---------------------------------*/
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
#include "tm4c123gh6pm.h"

#define RED_LED      (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
#define BLUE_LED     (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 2*4)))
// REQUIRED: correct these bitbanding references for green and yellow LEDs (temporary to guarantee compilation)
#define GREEN_LED    (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))
#define YELLOW_LED   (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4)))

//-----------------------------------------------------------------------------
// 			RTOS Defines and Kernel Variables
//-----------------------------------------------------------------------------

// function pointer
typedef void (*_fn)();

// semaphore
#define MAX_QUEUE_SIZE 10
struct semaphore
{
  unsigned int count;
  unsigned int queueSize;
  unsigned int processQueue[MAX_QUEUE_SIZE]; // store task index here
} *s, keyPressed, keyReleased, flashReq;

// task
#define STATE_INVALID    0 // no task
#define STATE_READY      1 // ready to run
#define STATE_BLOCKED    2 // has run, but now blocked by semaphore
#define STATE_DELAYED    3 // has run, but now awaiting timer

#define MAX_TASKS 10       // maximum number of valid tasks
uint8_t taskCurrent = 0;   // index of last dispatched task
uint8_t taskCount = 0;     // total number of valid tasks

int rtosMode;              // mode
#define MODE_COOPERATIVE 0
#define MODE_PREEMPTIVE  1

struct _tcb
{
  uint8_t state;                 // see STATE_ values above
  void *pid;                     // used to uniquely identify process
  void *sp;                      // location of stack pointer for process
  uint8_t priority;              // 0=highest, 7=lowest
  uint8_t currentPriority;       // used for priority inheritance
  uint32_t ticks;                // ticks until sleep complete
} tcb[MAX_TASKS];

uint32_t stack[MAX_TASKS][256];

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

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

// Initialize Hardware
void initHw()
{
    // REQUIRED: Add initialization for blue, red, green, and yellow LEDs
	//       4 pushbuttons, and uart
}

// Approximate busy waiting (in units of microseconds), given a 40 MHz system clock
void waitMicrosecond(uint32_t us)
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

// REQUIRED: add code to return a value from 0-15 indicating which of 4 PBs are pressed
uint8_t readPbs()
{
	return 0;
}

// ------------------------------------------------------------------------------
//  Task functions
// ------------------------------------------------------------------------------

// one task must be ready at all times or the scheduler will fail
// the idle task is implemented for this purpose

void idle()
{
  while(true)
  {
    BLUE_LED = 1;
    waitMicrosecond(1000);
    BLUE_LED = 0;
    yield();
  }
}

void flash4Hz()
{
  while(true)
  {
    GREEN_LED ^= 1;
    sleep(125);
  }
}

void oneshot()
{
  while(true)
  {
    wait(&flashReq);
    YELLOW_LED = 1;
    sleep(1000);
    YELLOW_LED = 0;
  }
}

void partOfLengthyFn()
{
  // represent some lengthy operation
  waitMicrosecond(1000);
  // give another process a chance
  yield();
}

void lengthyFn()
{
  uint16_t i;
  while(true)
  {
    for (i = 0; i < 4000; i++)
    {
      partOfLengthyFn();
    }
    RED_LED ^= 1;
  }
}

void readKeys()
{
  uint8_t buttons;
  while(true)
  {
    wait(&keyReleased);
    buttons = 0;
    while (buttons == 0)
    {
      buttons = readPbs();
      yield();
    }
    post(&keyPressed);
    if ((buttons & 1) != 0)
    {
      YELLOW_LED ^= 1;
      RED_LED = 1;
    }
    if ((buttons & 2) != 0)
    {
      post(&flashReq);
      RED_LED = 0;
    }
    if ((buttons & 4) != 0)
    {
      createProcess(flash4Hz, 0);
    }
    if ((buttons & 8) != 0)
    {
      destroyProcess(flash4Hz);
	}

    yield();
  }
}

void debounce()
{
  uint8_t count;
  while(true)
  {
    wait(&keyPressed);
    count = 10;
    while (count != 0)
    {
      sleep(10);
      if (readPbs() == 0)
        count--;
      else
        count = 10;
    }
    post(&keyReleased);
  }
}

void uncooperative()
{
  while(true)
  {
    while (readPbs() == 8)
    {
    }
    yield();
  }
}

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
	RED_LED = 1;
	waitMicrosecond(250000);
	RED_LED = 0;
	waitMicrosecond(250000);

	// Initialize semaphores
	init(&keyPressed, 0);
	init(&keyReleased, 1);
	init(&flashReq, 5);

	// Initialize selected RTOS
	ok = false;
	while (!ok)
	{
	  pb = readPbs();
	  if (pb & 4)
	  {
	    ok = true;
	    waitMicrosecond(1000000);
	    rtosInit(MODE_COOPERATIVE);
	  }
	  if (pb & 8)
	  {
	    ok = true;
	    waitMicrosecond(1000000);
	    rtosInit(MODE_PREEMPTIVE);
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


