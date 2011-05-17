/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_TEMPLATE)

OSStack_t* OSScheduler::stackInitialise(OSStack_t* pStackTop, void(*pCode)(
    void*), void* pParams, unsigned char id)
  {
    /* The value on the right is the offset from the thread stack pointer */

    /* Place a few bytes of known values on the bottom of the stack.
     This is just useful for debugging. */
#define STACK_WORD_COUNT   (16+1)
    *pStackTop-- = (0x12345600 | id); /* -           +16*4=68 */

    /* Simulate how the stack would look after a call to yield()*/

    /* Thread starts with interrupts enabled ??? */
    /* T bit set */
    *pStackTop-- = 0x01000000; /* xPSR        +15*4=64 */

    /* The start of the thread code will be popped off the stack last,
     so place it on first. */

#define STACK_PCL_WORD_OFFSET   (14)
    *pStackTop-- = (OSStack_t) pCode; /* PCL         +14*4=60 */

    /* Next simulate the stack as if after a call to contextSave(). */
    *pStackTop-- = 0x00; /* LR          +13*4=56 */
    *pStackTop-- = 0x12; /* R12         +12*4=52 */
    *pStackTop-- = 0x03; /* R3          +11*4=48 */
    *pStackTop-- = 0x02; /* R2          +10*4=44 */
    *pStackTop-- = 0x01; /* R1          +9*4=40 */
#define STACK_R0_WORD_OFFSET    (8)
    *pStackTop-- = (OSStack_t) pParams; /* R0          +8*4=36 */
    *pStackTop-- = 0x11; /* R11         +7*4=32 */
    *pStackTop-- = 0x10; /* R10         +6*4=28 */
    *pStackTop-- = 0x09; /* R9          +5*4=24 */
    *pStackTop-- = 0x08; /* R8          +4*4=20 */
    *pStackTop-- = 0x07; /* R7          +3*4=16 */
    *pStackTop-- = 0x06; /* R6          +2*4=12 */
    *pStackTop-- = 0x05; /* R5          +1*4=8 */
    *pStackTop-- = 0x04; /* R4          +0*4=4 */
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

#if false
void OSScheduler::stackSetReturnedValue(OSStack_t* pStack,
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

void OSScheduler::startImpl(void)
  {


    OS::interruptsClearMask();
    OS::interruptsEnable();

    /* System call to start first thread. */
    asm volatile
    (
        " 		\n" /* Use the NVIC offset register to locate the stack. */
        : : :
  );

    for (;;)
      ; // noreturn
  }

void OSScheduler::ISRcontextSwitchRequest(void)
  {

  }

#if defined(DEBUG)

void OSScheduler::dumpContextInfo(OSThread* pThread)
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
      { "R4", "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R0", "R1", "R2",
          "R3", "R12", "LR", "PC", "XPSR", "X" };

    int i;
    for (i = 0; i < STACK_WORD_COUNT; ++i, ++pStack)
      {
        OSDeviceDebug::putNewLine();
        OSDeviceDebug::putDec(i);
        OSDeviceDebug::putChar(' ');
        OSDeviceDebug::putString(sRegNames[i]);
        OSDeviceDebug::putChar('=');
        OSDeviceDebug::putHex(*pStack);
        if (i==0 || i==(STACK_WORD_COUNT-1)){
          OSDeviceDebug::putChar(' ');
          OSDeviceDebug::putPtr(pStack);
        }
      }

#endif /* OS_DEBUG_OSSCHEDULER_DUMPSTACK */

    OSDeviceDebug::putNewLine();
  }

#endif /* DEBUG */

#endif /* OS_CONFIG_ARCH_ARM_CORTEX_M3 */

