/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_AVR32)

OSStack_t *
OSScheduler::stackInitialize(OSStack_t * pStackTop, void
(*pCode)(void *), void *pParams, unsigned char id)
{
  /* The value on the right is the offset from the task stack pointer */

  /* Place a few bytes of known values on the bottom of the stack.
   This is just useful for debugging. */
#define STACK_WORD_COUNT   (16+1)
  /* -           +16*4=64 */
  *pStackTop-- = (0x12345600 | id);

  /* Simulate how the stack would look after a call to yield()*/

  *pStackTop-- = 0x08; /* R8          +15*4=60 */
  *pStackTop-- = 0x09; /* R9          +14*4=56 */
  *pStackTop-- = 0x10; /* R10         +13*4=52 */
  *pStackTop-- = 0x11; /* R11         +12*4=48 */
  /* R12         +11*4=44 */
  *pStackTop-- = (OSStack_t) pParams;
  /* R14/LR      +10*4=40 */
  *pStackTop-- = 0xDEADBEEF;
#define STACK_PCL_WORD_OFFSET   (9)
  /* R15/PC      +9*4=36 */
  *pStackTop-- = (OSStack_t) pCode;
  /* SR          +8*4=32 */
  *pStackTop-- = 0x00400000;
  /* Task starts with interrupts enabled ??? */

#define STACK_R0_WORD_OFFSET    (7)
  /* R0          +7*4=28 */
  *pStackTop-- = 0xFF0000FF;
  *pStackTop-- = 0x01; /* R1          +6*4=24 */
  *pStackTop-- = 0x02; /* R2          +5*4=20 */
  *pStackTop-- = 0x03; /* R3          +4*4=16 */
  *pStackTop-- = 0x04; /* R4          +3*4=12 */
  *pStackTop-- = 0x05; /* R5          +2*4=8 */
  *pStackTop-- = 0x06; /* R6          +1*4=4 */
  *pStackTop-- = 0x07; /* R7          +0*4=0 */

  ++pStackTop;

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_STACKINITIALIZE)
    {
      OSDeviceDebug::putString("Stack Init SP=");
      OSDeviceDebug::putPtr(pStackTop);
      OSDeviceDebug::putString(" PC=");
      OSDeviceDebug::putPtr((void*) pCode);
#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_DUMPSTACK)
      OSStack_t *p;
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

#if false
void OSScheduler::stackSetReturnedValue(OSStack_t * pStack,
    OSEventWaitReturn_t ret)
  {
#if false && defined(DEBUG)
    OSDeviceDebug::putString(" rv=");
    OSDeviceDebug::putHex((unsigned short)ret);
    OSDeviceDebug::putString(" *");
    OSDeviceDebug::putHex((unsigned short)pStack);
    OSDeviceDebug::putNewLine();
#endif

    pStack[STACK_R0_WORD_OFFSET] = (OSStack_t) ret;
  }
#endif

void
OSScheduler::startImpl(void)
{
  OS::interruptsClearMask();
  OS::interruptsEnable();

  UserMode_contextRestore();

  for (;;)
    ; // noreturn
}

#if defined(DEBUG)

void
OSScheduler::dumpContextInfo(OSTask * pTask)
{
  OSDeviceDebug::putChar('\'');
  OSDeviceDebug::putString(pTask->getName());

  OSStack_t * pStack;
  pStack = pTask->getStack();
  OSDeviceDebug::putString("' SP=");
  OSDeviceDebug::putPtr(pStack);
  OSDeviceDebug::putString(" PC=");
  OSDeviceDebug::putHex(pStack[STACK_PCL_WORD_OFFSET]);

#if defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_DUMPSTACK)

  const char * sRegNames[] =
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

#endif /* OS_CONFIG_ARCH_AVR32 */

