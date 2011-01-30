/*
 *      Copyright (C) 2007-2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

#include <string.h>

// C++ style constructor, run taskMain
OSTask::OSTask(const char *pName, const OSStack_t *pStack,
    unsigned short stackSize, OSTaskPriority_t priority)
  {
    OSTask::init(pName, (OSTaskMainPtr_t) &OSTask::staticMain, this, pStack,
        stackSize, priority);
  }

// C style constructor, run any function
OSTask::OSTask(const char *pName, OSTaskMainPtr_t entryPoint,
    void *pParameters, const OSStack_t *pStack, unsigned short stackSize,
    OSTaskPriority_t priority)
  {
    OSTask::init(pName, entryPoint, pParameters, pStack, stackSize, priority);
  }

/* stack size is in multiples of OSStack_t */
void OSTask::init(const char *pName, OSTaskMainPtr_t entryPoint,
    void *pParameters, const OSStack_t *pStack, unsigned short stackSize,
    OSTaskPriority_t priority)
  {
    // no stack, sorry...
    if (pStack == 0 || stackSize == 0)
      return;

    // no entry, sorry...
    if (entryPoint == 0)
      return;

    // no name, sorry...
    if (pName == 0)
      return;

#if defined(DEBUG)
      {
        OSDeviceDebug::putString_P(PSTR("OSTask('"));
        OSDeviceDebug::putString(pName);
        OSDeviceDebug::putString_P(PSTR("',"));
        OSDeviceDebug::putPtr((void*)entryPoint);
        OSDeviceDebug::putChar(',');
        OSDeviceDebug::putPtr(pParameters);
        OSDeviceDebug::putChar(',');
        OSDeviceDebug::putPtr(pStack);
        OSDeviceDebug::putChar(',');
        OSDeviceDebug::putHex(stackSize);
        OSDeviceDebug::putChar(',');
        OSDeviceDebug::putHex(priority);
        OSDeviceDebug::putString_P(PSTR(")="));
        OSDeviceDebug::putPtr(this);
        OSDeviceDebug::putNewLine();
      }
#endif

    m_pName = pName;
    m_entryPoint = entryPoint;
    m_pStackBottom = (unsigned char *) pStack;
    m_stackSize = stackSize;
    m_staticPriority = priority;
    m_pParameters = pParameters;

    m_event = OSEvent::OS_NONE;
    m_eventWaitReturn = OSEventWaitReturn::OS_NONE;
    m_isSuspended = false;
    m_isWaiting = false;
    //m_hasReturnValue = false;

    //m_isPreempted = false;

#if defined(OS_INCLUDE_OSTASK_SLEEP)
    // by default, tasks will enter deep sleep
    m_allowSleep = true;
#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)
    m_WDseconds = 0;
#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)
    m_isInterrupted = false;
#endif
    // register this task to scheduler
    m_id = OSScheduler::taskRegister(this);

    // fill stack with constant pattern(0x5A)
    memset((void *) pStack, STACK_FILL_BYTE, stackSize);

    // initialize stack so that a context restore will be performed
    m_pStack = OSSchedulerImpl::stackInitialize((OSStack_t*) (&pStack[stackSize
        / sizeof(OSStack_t) - 1]), entryPoint, pParameters, m_id);

    // pStack points to first register on arm and
    // to byte before first register on AVR8

#if defined(DEBUG) && false
      {
        OSDeviceDebug::putPtr(m_pStack);
        OSDeviceDebug::putNewLine();
      }
#endif
  }

#if false
OSTask::~OSTask()
  {
#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("~OSTask('"));
    OSDeviceDebug::putString(m_pName);
    OSDeviceDebug::putString("')");
    OSDeviceDebug::putNewLine();
#endif
  }
#endif

// redirect to virtual function
void OSTask::staticMain(OSTask * pt)
  {
    pt->taskMain();

    // normally should not reach here
#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("taskMain loop exit"));
    OSDeviceDebug::putNewLine();
#endif

    OS::SOFTreset();
  }

// overriden by actual implementation
void OSTask::taskMain(void)
  {
    // loop
    for (;;)
      ;
  }

// Suspend and remove from ready list
void OSTask::suspend(void)
  {
    OSScheduler::criticalEnter();
      {
        m_isSuspended = true;
        OSReadyList::remove(this);
      }
    OSScheduler::criticalExit();
  }

// Resume and insert into ready list
void OSTask::resume(void)
  {
    OSScheduler::criticalEnter();
      {
        m_isSuspended = false;
        OSReadyList::insert(this);
      }
    OSScheduler::criticalExit();
  }

char const * OSTask::getName(void)
  {
    return m_pName;
  }

unsigned char * OSTask::getStackBottom(void)
  {
    return m_pStackBottom;
  }

OSStack_t * OSTask::getStack(void)
  {
    return m_pStack;
  }

unsigned short OSTask::getStackSize(void)
  {
    return m_stackSize;
  }

// since all registers are saved on the task stack,
// the 'context' in simply a pointer to the stack (the address below
// the deepest register pushed; for offsets to registers please see
// OSScheduler::stackInitialize()).
void * OSTask::getContext(void)
  {
    return (void *) &m_pStack;
  }

void OSTask::setPriority(OSTaskPriority_t priority)
  {
    m_staticPriority = priority;
  }

void OSTask::setEvent(OSEvent_t event)
  {
    m_event = event;
  }

#if defined(OS_INCLUDE_OSTASK_GETSTACKUSED)

unsigned short OSTask::getStackUsed(void)
  {
    unsigned short i;
    unsigned short r;
    unsigned char *pc;

    r = m_stackSize;
    pc = m_pStackBottom;

    for (i = 0; (i < m_stackSize ) && ( *pc == STACK_FILL_BYTE ); ++i,
        ++pc, --r)
    ;

    return r;
  }

#endif

int OSTask::getID(void)
  {
    return m_id;
  }

#if defined(OS_INCLUDE_OSTASK_GETPROGRAMCOUNTER)

OSProgramPtr_t OSTask::getProgramCounter(void)
  {
    unsigned char * p;
    p = ( unsigned char * ) getStack() + 34;
    unsigned short w;
    w = (p[ 0 ] << 8 ) | p[ 1 ];

    return ( OSProgramPtr_t ) w;
  }

#endif

OSEvent_t OSTask::getEvent(void)
  {
    return m_event;
  }

OSTaskPriority_t OSTask::getPriority(void)
  {
    return m_staticPriority;
  }

bool OSTask::isSuspended(void)
  {
    return m_isSuspended;
  }

bool OSTask::isWaiting(void)
  {
    return m_isWaiting;
  }

#if defined(OS_INCLUDE_OSTASK_SLEEP)

bool OSTask::isAllowDeepSleep(void)
  {
    return m_allowSleep;
  }

void OSTask::setAllowSleep(bool b)
  {
    m_allowSleep = b;
  }

#endif

#if defined(OS_INCLUDE_OSTASK_VIRTUALWATCHDOG)

void OSTask::virtualWatchdogSet(unsigned short seconds)
  {
#if defined(DEBUG) && defined(OS_DEBUG_OSTASK_VIRTUALWATCHDOGSET)
    OSDeviceDebug::putString_P(PSTR("vWD("));
    OSDeviceDebug::putDec(getID());
    OSDeviceDebug::putString(",");
    OSDeviceDebug::putDec(seconds);
    OSDeviceDebug::putString_P(PSTR(") "));
#endif
    m_WDseconds = seconds;
  }

void OSTask::virtualWatchdogCheck(void)
  {
    if (m_WDseconds != 0)
      {
        if (--m_WDseconds == 0)
          {
#if defined(DEBUG)
            OSDeviceDebug::putString_P(PSTR("virtual WD '"));
            OSDeviceDebug::putString(m_pName);
            OSDeviceDebug::putChar('\'');
#if defined(OS_INCLUDE_OSTASK_GETSTACKUSED)
            OSDeviceDebug::putChar(' ');
            OSDeviceDebug::putDec(getStackUsed());
            OSDeviceDebug::putChar('/');
            OSDeviceDebug::putDec(getStackSize());
#endif
            OSDeviceDebug::putNewLine();
#endif

            OS::SOFTreset();
          }
        else
          {
#if defined(DEBUG) && defined(OS_DEBUG_OSTASK_VIRTUALWATCHDOGCHECK)
            OSDeviceDebug::putDec(getID());
            OSDeviceDebug::putChar('=');
#endif
          }
      }
  }

#endif

#if defined(OS_INCLUDE_OSTASK_SCHEDULERTICK)

void
OSTask::schedulerTick( void )
  {
    return;
  }

#endif

#if defined(OS_INCLUDE_OSTASK_INTERRUPTION)

void OSTask::requestInterruption(void)
  {
    setInterruption(true);

    OSScheduler::criticalEnter();
      {
        OSScheduler::ISRcancelTask(this);
      }
    OSScheduler::criticalExit();

#if defined(DEBUG)
    OSDeviceDebug::putChar('>');
#endif
  }

bool
OSTask::isInterrupted(void)
  {
    return m_isInterrupted;
  }

void
OSTask::setInterruption(bool flag)
  {
    m_isInterrupted = flag;
  }

#endif

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
