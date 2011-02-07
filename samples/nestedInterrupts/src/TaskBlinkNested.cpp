/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "TaskBlinkNested.h"

#define EVENT_NESTEED 0x1234

#if defined(APP_EXCLUDE_PREEMPTION) || defined(OS_EXCLUDE_PREEMPTION)
__attribute__((interrupt))
#else
__attribute__((naked))
#endif
void
NestedInterrupt_contextHandler(void);

static TaskBlinkNested *pTaskBlinkNested;

/*
 * Task constructor.
 * Initialise system task object and store parameters in private members.
 */

TaskBlinkNested::TaskBlinkNested(const char *pName, unsigned char iLed,
    schedTicks_t rate) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_oLed(iLed)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("TaskBlinkNested()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif

  m_rate = rate;
  m_count = 0;

  pTaskBlinkNested = this;
}

/*
 * Task main code.
 * Initialise led and toggle it using the rate.
 *
 * The toggle rate is done with busy wait, the loop being interrupted by yields.
 */

void
TaskBlinkNested::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock();
        {
          debug.putString("Task '");
          debug.putString(getName());
          debug.putString("', led=");
          debug.putDec(m_oLed.bitNumber());
          debug.putString(", divider=");
          debug.putDec(m_rate);
          debug.putNewLine();
        }
      os.sched.unlock();
    }

  // initialise led port as output
  m_oLed.init();

  interruptInit();

  bool bSecond;
  bSecond = false;
  // task endless loop
  for (;;)
    {
#if true
      os.sched.eventWait(EVENT_NESTEED);
#else
      os.sched.yield();
#endif

      os.sched.criticalEnter();
        {
          if (m_count >= m_rate)
            {
              m_count -= m_rate;
              bSecond = true;
            }
        }
      os.sched.criticalExit();

      if (bSecond)
        {
          bSecond = false;

          debug.putChar('.');
          // finally toggle led
          m_oLed.toggle();
        }
    }
}

void
TaskBlinkNested::interruptInit(void)
{
  volatile avr32_tc_t* tc_reg = &TSKBLKNEST_TIMER;

  // the clock for TCO is already enabled

  // initialise registers for the Channel

  // disable channel
  tc_reg->channel[TSKBLKNEST_CHANNEL].CCR.clkdis = 1;

  // clear interrupt flags, by reading SR
  tc_reg->channel[TSKBLKNEST_CHANNEL].sr;

  // reset register
  tc_reg->channel[TSKBLKNEST_CHANNEL].cmr = 0;

  // set waveform mode as default
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.wave = 1;

  // for waveform mode the TIOB must not be input
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.eevt = 2;
  //disable all interrupt sources
  tc_reg->channel[TSKBLKNEST_CHANNEL].idr = 0xFFFFFFFF;

  //register the interrupt
  INTC_register_interrupt(NestedInterrupt_contextHandler,
      TSKBLKNEST_CFGINT_TIMER_IRQ_ID, TSKBLKNEST_CFGINT_TIMER_IRQ_LEVEL);

  //clock source
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.tcclks
      = TSKBLKNEST_CFGINT_TIMER_CLOCK_SELECT;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.wavsel = 2;
  // no event on TIOA when RC compare
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.acpc = 0;
  // no event on TIOB when RC compare
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.bcpc = 0;
  // enable channel, as it might be disabled
  tc_reg->channel[TSKBLKNEST_CHANNEL].CCR.clken = 1;

  // start channel using software trigger
  tc_reg->channel[TSKBLKNEST_CHANNEL].CCR.swtrg = 1;

#define T_DIVIDER (1 << ((TSKBLKNEST_CFGINT_TIMER_CLOCK_SELECT-2)*2+1))
#define T_COUNTER (OS_CFGLONG_OSCILLATOR_HZ/TSKBLKNEST_CFGINT_TIMER_PRESCALLER/T_DIVIDER/TSKBLKNEST_CFGINT_TICK_RATE_HZ)

  unsigned int nCounter;
  nCounter = (OS_CFGLONG_OSCILLATOR_HZ / TSKBLKNEST_CFGINT_TIMER_PRESCALLER
      /T_DIVIDER / m_rate);

#if defined(DEBUG)

  OSDeviceDebug::putDec(TSKBLKNEST_CFGINT_TIMER_PRESCALLER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(T_DIVIDER);
  OSDeviceDebug::putChar(',');
  OSDeviceDebug::putDec(nCounter);
  OSDeviceDebug::putNewLine();

#endif

  // set RC value
  tc_reg->channel[TSKBLKNEST_CHANNEL].RC.rc = nCounter;

  // counter UP mode with automatic trigger on RC Compare
  tc_reg->channel[TSKBLKNEST_CHANNEL].CMR.waveform.wavsel = 2;

#if true
  // set interrupt source RC Compare
  tc_reg->channel[TSKBLKNEST_CHANNEL].IER.cpcs = 1;
#endif

  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, APP_CONFIG_LED3);
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, APP_CONFIG_LED3);

}

void
TaskBlinkNested::interruptAcknowledge(void)
{
  TSKBLKNEST_TIMER.channel[TSKBLKNEST_CHANNEL].sr;
}

void
TaskBlinkNested::interruptServiceRoutine(void)
{
#if !defined(APP_EXCLUDE_ISR_ACTION)

  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, APP_CONFIG_LED3);

  m_count++; // count ticks

#if defined(APP_ISR_ACTION_BUSYWAIT)
  OS::busyWaitMicros(10);
#else

  // Notify task; some of these will be lost, but the total count
  // will accumulate, and when awaken, will be considered
  OSScheduler::eventNotify(EVENT_NESTEED);

#endif

  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, APP_CONFIG_LED3);

#endif

  interruptAcknowledge();
}

void
NestedInterrupt_contextHandler(void)
{
#if !defined(APP_EXCLUDE_PREEMPTION) || defined(OS_EXCLUDE_PREEMPTION)
  OSScheduler::interruptEnter();
#endif
    {
#if !defined(APP_EXCLUDE_ISR_ACTION)
      pTaskBlinkNested->interruptServiceRoutine();
#else
      TSKBLKNEST_TIMER.channel[TSKBLKNEST_CHANNEL].sr;
#endif
    }
#if !defined(APP_EXCLUDE_PREEMPTION) || defined(OS_EXCLUDE_PREEMPTION)
  OSScheduler::interruptExit();
#endif
}

