/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_AVR8)

OSStack_t * OSSchedulerImpl::stackInitialize(OSStack_t * pStackTop, void(*pCode)(
    void *), void *pParams, unsigned char id)
  {
    /* The value on the right is the offset from the task stack pointer */

    /* Place a few bytes of known values on the bottom of the stack.
     This is just useful for debugging. */
    *pStackTop-- = id; /* -     +39 */
    *pStackTop-- = 0x11; /* -          +38 */
    *pStackTop-- = 0x22; /* -          +37 */
    *pStackTop-- = 0x33; /* -          +36 */

    /* Simulate how the stack would look after a call to yield()*/

    /* The start of the task code will be popped off the stack last, so place
     it on first. */

    /* PCL          +35 */
    *pStackTop-- = (((unsigned short) pCode) & 0xFF);
    /* PCH          +34 */
    *pStackTop-- = ((((unsigned short) pCode) >> 8) & 0xFF);

    /* Next simulate the stack as if after a call to contextSave().
     contextSave()places the flags on the stack immediately after r0
     to ensure the interrupts get disabled as soon as possible, and so ensuring
     the stack use is minimal should a context switch interrupt occur. */
    *pStackTop-- = 0x00; /* R0          +33 */

    /* Task starts with interrupts enabled */
    /* SR          +32 */
    *pStackTop-- = ((unsigned char) (_BV(SREG_I)));

    /* Now the remaining registers.   The compiler expects R1 to be 0. */
    *pStackTop-- = 0x00; /* R1          +31 */

    *pStackTop-- = 0x02; /* R2          +30 */
    *pStackTop-- = 0x03; /* R3          +29 */
    *pStackTop-- = 0x04; /* R4          +28 */
    *pStackTop-- = 0x05; /* R5          +27 */
    *pStackTop-- = 0x06; /* R6          +26 */
    *pStackTop-- = 0x07; /* R7          +25 */
    *pStackTop-- = 0x08; /* R8          +24 */
    *pStackTop-- = 0x09; /* R9          +23 */
    *pStackTop-- = 0x10; /* R10         +22 */
    *pStackTop-- = 0x11; /* R11         +21 */
    *pStackTop-- = 0x12; /* R12         +20 */
    *pStackTop-- = 0x13; /* R13         +19 */
    *pStackTop-- = 0x14; /* R14         +18 */
    *pStackTop-- = 0x15; /* R15         +17 */
    *pStackTop-- = 0x16; /* R16         +16 */
    *pStackTop-- = 0x17; /* R17         +15 */
    *pStackTop-- = 0x18; /* R18         +14 */
    *pStackTop-- = 0x19; /* R19         +13 */
    *pStackTop-- = 0x20; /* R20         +12 */
    *pStackTop-- = 0x21; /* R21         +11 */
    *pStackTop-- = 0x22; /* R22         +10 */
    *pStackTop-- = 0x23; /* R23         +9 */

    /* Place the parameter on the stack in the expected location. */
    /* R24         +8 */
    *pStackTop-- = (((unsigned short) pParams) & 0xFF);
    /* R25         +7 */
    *pStackTop-- = ((((unsigned short) pParams) >> 8) & 0xFF);

    *pStackTop-- = 0x26; /* R26 X       +6 */
    *pStackTop-- = 0x27; /* R27         +5 */
    *pStackTop-- = 0x28; /* R28 Y       +4 */
    *pStackTop-- = 0x29; /* R29         +3 */
    *pStackTop-- = 0x30; /* R30 Z       +2 */
    *pStackTop-- = 0x31; /* R31         +1 */

#if defined(DEBUG)

    if (false)
      {
        OSDeviceDebug::putString("Init S=");
        OSDeviceDebug::putHex((unsigned short) pStackTop);
        OSDeviceDebug::putString(" C=");
        OSDeviceDebug::putHex((unsigned short) pCode);
          {
            unsigned char *p;
            int i;
            for (p = pStackTop, i = 0; i < 40; p++, i++)
              {
                OSDeviceDebug::putChar(' ');
                OSDeviceDebug::putHex((unsigned char) *p);
              }
          }
        OSDeviceDebug::putChar(' ');
        OSDeviceDebug::putHex((unsigned short) pStackTop);
        OSDeviceDebug::putNewLine();
      }
#endif

    return pStackTop;
  }

#if false
void OSSchedulerImpl::stackSetReturnedValue(OSStack_t * pStack,
    OSEventWaitReturn_t ret)
  {
#if false && defined(DEBUG)
    OSDeviceDebug::putString(" rv=");
    OSDeviceDebug::putHex((unsigned short)ret);
    OSDeviceDebug::putString(" *");
    OSDeviceDebug::putHex((unsigned short)pStack);
    OSDeviceDebug::putNewLine();
#endif

    *(pStack + 8) = (((unsigned short) ret) & 0xFF);
    *(pStack + 7) = ((((unsigned short) ret) >> 8) & 0xFF);
  }
#endif

void OSSchedulerImpl::start(void)
  {
    // interrupts enabled when pop-ing flags
    OSScheduler::contextRestore();
    OS::interruptsEnable(); // be sure interrupts are enabled
    OS::returnFromSubroutine();
  }

#if defined(DEBUG)

#if false
void OSScheduler::dumpContextInfo(void *pCurrentContext __attribute__( ( unused ) ) )
  {
    unsigned char * q;
    OSDeviceDebug::putHex(
        (unsigned short) *((unsigned short *) pCurrentContext));
    OSDeviceDebug::putChar('/');
    q = ((unsigned char *) *((unsigned short *) g_pCurrentContext) + 34);
    unsigned short w;
    w = (q[0] << 8) | q[1];
    OSDeviceDebug::putPC(w);
  }
#endif

void OSSchedulerImpl::dumpContextInfo(OSTask * pTask)
  {
    OSDeviceDebug::putChar('\'');
    OSDeviceDebug::putString(pTask->getName());

    OSStack_t * pStack;
    pStack = pTask->getStack();
    OSDeviceDebug::putString("' SP=");
    OSDeviceDebug::putPtr(pStack);
    OSDeviceDebug::putString(" PC=");
    unsigned char * q;
    q = ((unsigned char *) *((unsigned short *) g_ppCurrentStack) + 34);
    unsigned short w;
    w = (q[0] << 8) | q[1];
    OSDeviceDebug::putPC((const char *)w);

    //OSDeviceDebug::putHex(pStack[STACK_PCL_WORD_OFFSET]);

#if false && defined(DEBUG) && defined(OS_DEBUG_OSSCHEDULER_DUMPSTACK)

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

#endif

#endif
