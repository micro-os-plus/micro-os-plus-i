/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER)

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSTimerTicks.h"

// static members
OSTimerStruct_t volatile OSTimerTicks::m_array[OS_CFGINT_OSTIMERTICKS_SIZE];

// TODO: delete if not needed
// OSTimerTicks_t OSTimerTicks::ms_secondTicks;

OSTimerTicks::OSTimerTicks() :
  OSTimer((OSTimerStruct_t*) &m_array[0], sizeof(m_array) / sizeof(m_array[0]))
{
  OSDeviceDebug::putConstructor_P(PSTR("OSTimerTicks"), this);
}

#if defined(OS_INCLUDE_SDI12SENSOR)
#include "portable/devices/sdi12/include/SDI12Sensor.h"
#endif

#if defined(DEBUG) && defined(OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS)
static int i;
static int s;
#endif

void
OSTimerTicks::init(void)
{
#if defined(DEBUG)

  OSDeviceDebug::putString_P(PSTR("OSTimerTicks::init()"));
  OSDeviceDebug::putNewLine();

#endif

#if defined(DEBUG) && defined(OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS)
  i = 0;
  s = 0;
#endif

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
  OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
  OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
#endif /* OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED */

  implInit();
}

void
OSTimerTicks::interruptServiceRoutine(void)
{

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
  OS_GPIO_PIN_HIGH(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
#endif /* OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED */

  implAcknowledgeInterrupt();

  interruptTick();

  OSScheduler::interruptTick();

#if defined(OS_INCLUDE_SDI12SENSOR)
  SDI12Sensor::interruptTick();
#endif /* defined(OS_INCLUDE_SDI12SENSOR) */

#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_SOFT)

  if (++OSTimerSeconds::ms_schedulerTicks == OS_CFGINT_TICK_RATE_HZ)
    {
      OSTimerSeconds::ms_schedulerTicks = 0;

      OSScheduler::timerSeconds.interruptServiceRoutine();

#if defined(OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS)

      if (++s == 60)
        {
          s = 0;
          // Mark every minute by a new line
          OSDeviceDebug::putNewLine();
        }

#endif
    }

#endif /* defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS_SOFT) */

#if defined(DEBUG) && defined(OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS)

  if ((i++ % OS_CFGINT_TICK_RATE_HZ) == 0)
    {
      OSDeviceDebug::putChar('!');
      if (OSSchedulerLock::isSet())
        OSDeviceDebug::putChar('L');
    }

#endif /* defined(DEBUG) && defined(OS_DEBUG_OSTIMERTICKS_ISR_MARK_SECONDS) */

#if defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED)
  OS_GPIO_PIN_LOW(OS_CONFIG_OSTIMERTICKS_LED_PORT_CONFIG, OS_CONFIG_OSTIMERTICKS_LED_BIT);
#endif /* defined(OS_INCLUDE_OSTIMERTICKS_ISR_DEBUGLED) */

}

#endif /* !defined(OS_EXCLUDE_MULTITASKING) && !defined(OS_EXCLUDE_OSTIMER) */
