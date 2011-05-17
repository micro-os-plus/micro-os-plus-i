/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_TEMPLATE)

#if !defined(OS_EXCLUDE_MULTITASKING)

OSStack_t* 
OSSchedulerImpl::stackInitialise(OSStack_t* pStackTop, void
(*pCode)(void*), void* pParams, unsigned char id)
{
  /* The value on the right is the offset from the thread stack pointer */

  /* Place a few bytes of known values on the bottom of the stack.
   This is just useful for debugging. */
#define STACK_WORD_COUNT   (17+1)

  *pStackTop-- = (0x12345600 | id); /* - +17*4=68 */

  /* Simulate how the stack would look after a call to yield()*/

  *pStackTop-- = 0x08; /* R8          +16*4=64 */
  *pStackTop-- = 0x09; /* R9          +15*4=60 */
  *pStackTop-- = 0x10; /* R10         +14*4=56 */
  *pStackTop-- = 0x11; /* R11         +13*4=52 */

  *pStackTop-- = (OSStack_t) pParams; /* R12  +12*4=48 */
  *pStackTop-- = 0xDEADBEEF;   /* R14/LR      +11*4=44 */

#define STACK_PCL_WORD_OFFSET   (10)

  *pStackTop-- = (OSStack_t) pCode; /* R15/PC +10*4=40 */
  *pStackTop-- = 0x00400000; /* SR    +9*4=36 */
  /* Thread starts in system mode, with all interrupts enabled */

#define STACK_R0_WORD_OFFSET    (8)

  *pStackTop-- = 0xFF0000FF; /* R0    +8*4=32 */
  *pStackTop-- = 0x01; /* R1          +7*4=24 */
  *pStackTop-- = 0x02; /* R2          +6*4=20 */
  *pStackTop-- = 0x03; /* R3          +5*4=16 */
  *pStackTop-- = 0x04; /* R4          +4*4=12 */
  *pStackTop-- = 0x05; /* R5          +3*4=8 */
  *pStackTop-- = 0x06; /* R6          +2*4=4 */
  *pStackTop-- = 0x07; /* R7          +1*4=0 */

  *pStackTop-- = 0x0; /* criticalSectionNesting +0*4=0 */

  ++pStackTop;

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_STACKINITIALIZE)
    {
      OSDeviceDebug::putString("Stack Init SP=");
      OSDeviceDebug::putPtr(pStackTop);
      OSDeviceDebug::putString(" PC=");
      OSDeviceDebug::putPtr((void*) pCode);
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_DUMPSTACK)
      OSStack_t* p;
      int i;
      for (p = pStackTop, i = 0; i < STACK_WORD_COUNT; p++, i++)
        {
          OSDeviceDebug::putNewLine();
          OSDeviceDebug::putDec(i);
          OSDeviceDebug::putChar(' ');
          OSDeviceDebug::putHex(*p);
        }
      OSDeviceDebug::putChar(' ');
      OSDeviceDebug::putPtr(--p);
#endif /* OS_DEBUG_OSSCHEDULER_DUMPSTACK */
      OSDeviceDebug::putNewLine();
    }
#endif /* OS_DEBUG_OSSCHEDULER_STACKINITIALIZE */

  return pStackTop;
}

void
OSSchedulerImpl::start(void)
{
  // be sure interrupts remain disabled before restoring first context
  //OS::interruptsClearMask();
  //OS::interruptsEnable();

  OSSchedulerImpl::FirstThread_contextRestore();

  for (;;)
    ; // noreturn
}

#if defined(DEBUG)

void
OSSchedulerImpl::dumpContextInfo(OSThread* pThread)
{
  OSDeviceDebug::putChar('\'');
  OSDeviceDebug::putString(pThread->getName());

  OSStack_t* pStack;
  pStack = pThread->getStack();
  OSDeviceDebug::putString("' SP=");
  OSDeviceDebug::putPtr(pStack);
  OSDeviceDebug::putString(" PC=");
  OSDeviceDebug::putHex(pStack[STACK_PCL_WORD_OFFSET]);

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_DUMPSTACK)

  const char*  sRegNames[] =
    { "R7", "R6", "R5", "R4", "R3", "R2", "R1", "R0", "SR", "PC", "LR",
      "R12", "R11", "R10", "R9", "R8", "X"};

  int i;
  for (i = 0; i < STACK_WORD_COUNT; ++i, ++pStack)
    {
      OSDeviceDebug::putNewLine();
      OSDeviceDebug::putDec(i);
      OSDeviceDebug::putChar(' ');
      OSDeviceDebug::putString(sRegNames[i]);
      OSDeviceDebug::putChar('=');
      OSDeviceDebug::putHex(*pStack);
      if (i==0 || i==(STACK_WORD_COUNT-1))
        {
          OSDeviceDebug::putChar(' ');
          OSDeviceDebug::putPtr(pStack);
        }
    }

#endif /* OS_DEBUG_OSSCHEDULER_DUMPSTACK */

  OSDeviceDebug::putNewLine();
}

#endif /* DEBUG */

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */

#endif /* defined(OS_CONFIG_ARCH_TEMPLATE) */

