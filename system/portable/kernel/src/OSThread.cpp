/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/include/OS.h"

#include <string.h>

// C++ style constructor, run threadMain
OSThread::OSThread(const char* pName, const OSStack_t* pStack,
    unsigned short stackSize, OSThreadPriority_t priority)
{
  OSThread::init(pName, (OSThreadMainPtr_t) &OSThread::staticMain, this, pStack,
      stackSize, priority);
}

// C style constructor, run any function
OSThread::OSThread(const char* pName, OSThreadMainPtr_t entryPoint,
    void* pParameters, const OSStack_t* pStack, unsigned short stackSize,
    OSThreadPriority_t priority)
{
  OSThread::init(pName, entryPoint, pParameters, pStack, stackSize, priority);
}

// The stack size is in multiples of OSStack_t
void
OSThread::init(const char *pName, OSThreadMainPtr_t entryPoint, void *pParameters,
    const OSStack_t *pStack, unsigned short stackSize,
    OSThreadPriority_t priority)
{
  // no stack, sorry...
  if (pStack == 0 || stackSize == 0)
    return;

  // no entry point, sorry...
  if (entryPoint == 0)
    return;

  // no name, sorry...
  if (pName == 0)
    return;

#if defined(DEBUG)
    {
      OSDeviceDebug::putString_P(PSTR("OSThread('"));
      OSDeviceDebug::putString(pName);
      OSDeviceDebug::putString_P(PSTR("',"));
      OSDeviceDebug::putPtr((void*) entryPoint);
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
  m_pStackBottom = (unsigned char*) pStack;
  m_stackSize = stackSize;
  m_staticPriority = priority;
  m_pParameters = pParameters;

  m_event = OSEvent::OS_NONE;
  m_eventWaitReturn = OSEventWaitReturn::OS_NONE;
  m_isSuspended = false;
  m_isWaiting = false;

#if defined(OS_INCLUDE_OSTHREAD_SLEEP)
  // by default, threads will enter deep sleep
  m_allowSleep = true;
#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)
  m_WDseconds = 0;
#endif

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)
  m_isInterrupted = false;
#endif

  // Register this thread to the scheduler.
  // The scheduler is already initialised at earlyInit() for this to work.
  m_id = OSScheduler::threadRegister(this);

  // Fill the stack with constant pattern (0x5A)
  memset((void*) pStack, STACK_FILL_BYTE, stackSize);

  // Initialise the stack so that a context restore will be performed
  m_pStack = OSSchedulerImpl::stackInitialise(
      (OSStack_t*) (&pStack[stackSize / sizeof(OSStack_t) - 1]), entryPoint,
      pParameters, m_id);

  // pStack points to the first register on arm and
  // to the byte before the first register on AVR8

#if defined(DEBUG) && false
    {
      OSDeviceDebug::putPtr(m_pStack);
      OSDeviceDebug::putNewLine();
    }
#endif
}

// Redirect to virtual function
void
OSThread::staticMain(OSThread* pt)
{
  pt->threadMain();

  // normally should not reach here
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("threadMain returned, reset"));
  OSDeviceDebug::putNewLine();
#endif

  OSCPU::softReset();
}

void
OSThread::yield()
{
  OSSchedulerImpl::yield();
}

// Should be overridden by actual implementation
void
OSThread::threadMain(void)
{
  // loop
  for (;;)
    ;
}

// Suspend the thread and remove from the ready list.
void
OSThread::suspend(void)
{
  OSCriticalSection::enter();
    {
      m_isSuspended = true;
      OSActiveThreads::remove(this);
    }
  OSCriticalSection::exit();
}

// Resume the thread and insert into the ready list.
void
OSThread::resume(void)
{
  OSCriticalSection::enter();
    {
      m_isSuspended = false;
      OSActiveThreads::insert(this);
    }
  OSCriticalSection::exit();
}

#if defined(OS_INCLUDE_OSTHREAD_GETSTACKUSED)

unsigned short
OSThread::getStackUsed(void)
{
  unsigned short i;
  unsigned short r;
  unsigned char *pc;

  r = m_stackSize;
  pc = m_pStackBottom;

  for (i = 0; (i < m_stackSize) && (*pc == STACK_FILL_BYTE); ++i, ++pc, --r)
    ;

  return r;
}

#endif /* defined(OS_INCLUDE_OSTHREAD_GETSTACKUSED) */

#if defined(OS_INCLUDE_OSTHREAD_GETPROGRAMCOUNTER)

#if defined(OS_CONFIG_ARCH_AVR8)

OSProgramPtr_t OSThread::getProgramCounter(void)
  {
    unsigned char * p;
    p = ( unsigned char * ) getStack() + 34;
    unsigned short w;
    w = (p[ 0 ] << 8 ) | p[ 1 ];

    return ( OSProgramPtr_t ) w;
  }

#else
#error "implement it in a portable way, this is avr8 only"
#endif /* defined(OS_CONFIG_ARCH_AVR8) */

#endif

#if defined(OS_INCLUDE_OSTHREAD_VIRTUALWATCHDOG)

void OSThread::virtualWatchdogSet(unsigned short seconds)
  {
#if defined(DEBUG) && defined(OS_DEBUG_OSTHREAD_VIRTUALWATCHDOGSET)
    OSDeviceDebug::putString_P(PSTR("vWD("));
    OSDeviceDebug::putDec(getID());
    OSDeviceDebug::putString(",");
    OSDeviceDebug::putDec(seconds);
    OSDeviceDebug::putString_P(PSTR(") "));
#endif
    m_WDseconds = seconds;
  }

void OSThread::virtualWatchdogCheck(void)
  {
    if (m_WDseconds != 0)
      {
        if (--m_WDseconds == 0)
          {
#if defined(DEBUG)
            OSDeviceDebug::putString_P(PSTR("virtual WD '"));
            OSDeviceDebug::putString(m_pName);
            OSDeviceDebug::putChar('\'');
#if defined(OS_INCLUDE_OSTHREAD_GETSTACKUSED)
            OSDeviceDebug::putChar(' ');
            OSDeviceDebug::putDec(getStackUsed());
            OSDeviceDebug::putChar('/');
            OSDeviceDebug::putDec(getStackSize());
#endif /* defined(OS_INCLUDE_OSTHREAD_GETSTACKUSED) */
            OSDeviceDebug::putNewLine();
#endif

            OS::softReset();
          }
        else
          {
#if defined(DEBUG) && defined(OS_DEBUG_OSTHREAD_VIRTUALWATCHDOGCHECK)
            OSDeviceDebug::putDec(getID());
            OSDeviceDebug::putChar('=');
#endif
          }
      }
  }

#endif

#if defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK)

void
OSThread::schedulerTick( void )
  {
    return;
  }

#endif /* defined(OS_INCLUDE_OSTHREAD_SCHEDULERTICK) */

#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

void OSThread::requestInterruption(void)
  {
    setInterruption(true);

    OSCriticalSection::enter();
      {
        OSScheduler::ISRcancelThread(this);
      }
    OSCriticalSection::exit();

#if defined(DEBUG)
    OSDeviceDebug::putChar('>');
#endif
  }

#endif /* defined(OS_INCLUDE_OSTHREAD_INTERRUPTION) */

int
OSThread::eventNotify(OSEvent_t event, OSEventWaitReturn_t retVal)
{
  if (event == OSEvent::OS_NONE)
    return 0;

  int ret;
  ret = 0;

#if defined(OS_INCLUDE_OSTHREAD_NOTIFY_MEASURE)
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_APP_CONFIG_LED3);
#endif /* defined(OS_INCLUDE_OSTHREAD_NOTIFY_MEASURE) */

#if defined(OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL)
  OSRealTimeCriticalSection::enter();
#else /* !defined(OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL) */
  OSCriticalSection::enter();
#endif /* defined(OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL) */
    {
      // not-suspended waiting threads are notified
      if ((m_isWaiting) && (!m_isSuspended))
        {
          OSEvent_t ev;
          ev = m_event;

          // wakeup threads
          // - waiting for event
          // - waiting for OS_ALL
          // - notified with OS_ALL
          if (ev == OSEvent::OS_ALL || ev == event || event == OSEvent::OS_ALL)
            {
              eventWaitClear();

              m_eventWaitReturn = retVal;
#if defined(DEBUG)
              if (retVal == OSEventWaitReturn::OS_NONE)
                OSDeviceDebug::putChar('^');
#endif
              OSActiveThreads::insert(this);
              ret = 1;
            }
        }
    }
#if defined(OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL)
  OSRealTimeCriticalSection::exit();
#else /* !defined(OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL) */
  OSCriticalSection::exit();
#endif /* defined(OS_INCLUDE_OSTHREAD_EVENTNOTIFY_REALTIMECRITICAL) */

#if defined(OS_INCLUDE_OSTHREAD_NOTIFY_MEASURE)
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_APP_CONFIG_LED3);
#endif /* defined(OS_INCLUDE_OSTHREAD_NOTIFY_MEASURE) */

  // return the number of notified threads
  return ret;
}

// Runs in a critical section
bool
OSThread::eventWaitPrepare(OSEvent_t event)
{
#if defined(OS_INCLUDE_OSTHREAD_INTERRUPTION)

  // Do not wait if the thread is interrupted
  if (m_isInterrupted)
    {
      setEventWaitReturn(OSEventWaitReturn::OS_CANCELED);
      return false;
    }

#endif

  // Do not wait if the scheduler is locked
  if (OSSchedulerLock::isSet())
    {
      setEventWaitReturn(OSEventWaitReturn::OS_LOCKED);
      return false;
    }

  // Do not wait if the event is NONE
  if (event == OSEvent::OS_NONE)
    {
      // if no event, return NONE
      setEventWaitReturn(OSEventWaitReturn::OS_NONE);
      return false;
    }

  // Mark that the thread is waiting on the given event
  m_event = event;
  m_isWaiting = true;

  // Later this response will allow yield()
  return true;
}

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
