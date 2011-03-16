/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlinkRealTime.h"

__attribute__((interrupt)) void
RealTimeInterrupt_contextHandler(void);

static TaskBlinkRealTime *pTaskBlinkRealTime;

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlinkRealTime::TaskBlinkRealTime(const char *pName, unsigned char iLed,
    schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlinkRealTime()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_rate = rate;
  m_count = 0;

  pTaskBlinkRealTime = this;
}

/*
 * Task main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
 */

void
TaskBlinkRealTime::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec((unsigned short) m_oLed.bitNumber());
          debug.putString(", divider=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

#if !defined(APP_EXCLUDE_TASKBLINKREALTIME_TASKMAIN_LED)
  // initialise led port as output
  m_oLed.init();
#endif

  interruptInit();

  g_flagNotify = false;

  bool bSecond;
  bSecond = false;
  // task endless loop
  for (;;)
    {
#if true
      os.sched.eventWait(APP_CFGINT_TASKBLINKREALTIME_EVENT);
#else
      os.sched.yield();
#endif

      //os.sched.realTimeCriticalEnter();
      os.rt.critical.enter();
        {
          if (m_count >= m_rate)
            {
              m_count -= m_rate;
              bSecond = true;
            }
        }
      os.rt.critical.exit();
      //os.sched.criticalExit();

      if (bSecond)
        {
          bSecond = false;

          // display something on debug
          debug.putChar(':');

#if !defined(APP_EXCLUDE_TASKBLINKREALTIME_TASKMAIN_LED)
          // finally toggle led
          m_oLed.toggle();
#endif
        }
    }
}

void
TaskBlinkRealTime::interruptInit(void)
{
  volatile avr32_tc_t* tc_reg = &APP_CFGVAR_TASKBLINKREALTIME_TIMER;

  // the clock for TCO is already enabled

  // initialise registers for the Channel

  // disable channel
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CCR.clkdis = 1;

  // clear interrupt flags, by reading SR
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].sr;

  // reset register
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].cmr = 0;

  // set waveform mode as default
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.wave
      = 1;

  // for waveform mode the TIOB must not be input
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.eevt
      = 2;
  //disable all interrupt sources
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].idr = 0xFFFFFFFF;

  //register the interrupt
  INTC_register_interrupt(RealTimeInterrupt_contextHandler,
      APP_CFGINT_TASKBLINKREALTIME_TIMERIRQID,
      APP_CFGINT_TASKBLINKREALTIME_TIMERIRQLEVEL);

  //clock source
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.tcclks
      = APP_CFGINT_TASKBLINKREALTIME_TIMERCLOCKSELECT;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.wavsel
      = 2;
  // no event on TIOA when RC compare
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.acpc
      = 0;
  // no event on TIOB when RC compare
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.bcpc
      = 0;
  // enable channel, as it might be disabled
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CCR.clken = 1;

  // start channel using software trigger
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CCR.swtrg = 1;

#define T_DIVIDER (1 << ((APP_CFGINT_TASKBLINKREALTIME_TIMERCLOCKSELECT-2)*2+1))
#define T_COUNTER (OS_CFGLONG_OSCILLATOR_HZ/APP_CFGINT_TASKBLINKREALTIME_TIMERPRESCALLER/T_DIVIDER/TSKBLKNEST_CFGINT_TICK_RATE_HZ)

  unsigned int nCounter;
  nCounter = (OS_CFGLONG_OSCILLATOR_HZ
      / APP_CFGINT_TASKBLINKREALTIME_TIMERPRESCALLER / T_DIVIDER / m_rate);

#if defined(DEBUG)

  OSDeviceDebug::putDec(
      (unsigned short) APP_CFGINT_TASKBLINKREALTIME_TIMERPRESCALLER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec((unsigned short) T_DIVIDER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec((unsigned short) nCounter);
  OSDeviceDebug::putNewLine();

#endif

  // set RC value
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].RC.rc = nCounter;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].CMR.waveform.wavsel
      = 2;

#if true
  // set interrupt source RC Compare
  tc_reg->channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].IER.cpcs = 1;
#endif

#if defined(APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT)
  OS_GPIO_PIN_CONFIG_ENABLE(APP_CFGREG_TASKBLINKREALTIME_ISRLEDPORTCONFIG, APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT);
  OS_GPIO_PIN_CONFIG_OUTPUT(APP_CFGREG_TASKBLINKREALTIME_ISRLEDPORTCONFIG, APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT);
#endif
}

void
TaskBlinkRealTime::interruptAcknowledge(void)
{
  APP_CFGVAR_TASKBLINKREALTIME_TIMER.channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].sr;
}

void
TaskBlinkRealTime::interruptServiceRoutine(void)
{
#if !defined(APP_EXCLUDE_TASKBLINKREALTIME_ISRACTION)

#if defined(APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT)
  OS_GPIO_PIN_HIGH(APP_CFGREG_TASKBLINKREALTIME_ISRLEDPORTCONFIG, APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT);
#endif

  interruptAcknowledge();

  m_count++; // count ticks

#if defined(APP_INCLUDE_TASKBLINKREALTIME_ISRACTIONBUSYWAIT)
  OS::busyWaitMicros(10);
#else

#if defined(OS_INCLUDE_OSTASK_EVENTNOTIFY_REALTIMECRITICAL)
  // Notify task; some of these will be lost, but the total count
  // will accumulate, and when awaken, will be considered
  OSScheduler::eventNotify(APP_CFGINT_TASKBLINKREALTIME_EVENT);
#else
  OS::busyWaitMicros(100);
  g_flagNotify = true;
#endif

#endif

#if defined(APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT)
  OS_GPIO_PIN_LOW(APP_CFGREG_TASKBLINKREALTIME_ISRLEDPORTCONFIG, APP_CFGINT_TASKBLINKREALTIME_ISRLEDBIT);
#endif

#endif /* APP_EXCLUDE_TASKBLINKREALTIME_ISRACTION */
}

void
RealTimeInterrupt_contextHandler(void)
{
#if !defined(APP_EXCLUDE_TASKBLINKREALTIME_ISRACTION)
  pTaskBlinkRealTime->interruptServiceRoutine();
#else
  APP_CFGVAR_TASKBLINKREALTIME_TIMER.channel[APP_CFGINT_TASKBLINKREALTIME_TIMERCHANNEL].sr;
#endif
}

