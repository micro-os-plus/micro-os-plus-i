/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#include "portable/kernel/include/OSTimerTicks.h"

// static members
OSTimerStruct_t OSTimerTicks::m_array[OS_CFGINT_OSTIMERTICKS_SIZE];
OSTimerTicks_t OSTimerTicks::ms_secondTicks;

OSTimerTicks::OSTimerTicks() :
  OSTimer(&m_array[0], sizeof(m_array) / sizeof(m_array[0]))
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("OSTimerTicks()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    ms_secondTicks = 0;
  }

#if defined(OS_INCLUDE_SDI12SENSOR)
#include "portable/devices/sdi12/include/SDI12Sensor.h"
#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSSCHEDULER_TIMER_MARK_SECONDS)
static int i;
#endif

void
OSTimerTicks::init(void)
{
#if defined(DEBUG)

  OSDeviceDebug::putString("OSTimerTicks::init()");
  OSDeviceDebug::putNewLine();

#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSSCHEDULER_TIMER_MARK_SECONDS)
  i = 0;
#endif

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
#endif /* OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED */

  implInit();
}


void OSTimerTicks::interruptServiceRoutine(void)
  {

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
  OS_GPIO_PIN_HIGH(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
#endif /* OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED */

    OSScheduler::criticalEnter();
      {
        interruptTick();
        incrementTicks();
      }
    OSScheduler::criticalExit();

    OSScheduler::interruptTick();

#if defined(OS_INCLUDE_SDI12SENSOR)
    SDI12Sensor::interruptTick();
#endif

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_SOFT)

  if (++OSTimerSeconds::ms_schedulerTicks == OS_CFGINT_TICK_RATE_HZ)
    {
      OSTimerSeconds::ms_schedulerTicks = 0;

      OSScheduler::timerSeconds.interruptServiceRoutine();
    }

#endif

#if defined(DEBUG) && defined(OS_INCLUDE_OSSCHEDULER_TIMER_MARK_SECONDS)

  if ((i++ % OS_CFGINT_TICK_RATE_HZ) == 0)
    OSDeviceDebug::putChar('!');

#endif

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
  OS_GPIO_PIN_LOW(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
#endif /* OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED */

  }

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
