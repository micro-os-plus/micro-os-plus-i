/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

//#include "portable/kernel/include/OS_Defines.h"
#include "portable/kernel/include/OS_Defines.h"

#if !defined(OS_EXCLUDE_MULTITASKING)

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

void OSTimerTicks::interruptServiceRoutine(void)
  {
    OSScheduler::criticalEnter();
      {
        interruptTick();
        incrementTicks();

        OSScheduler::interruptTick();

#if defined(OS_INCLUDE_SDI12SENSOR)
        SDI12Sensor::interruptTick();
#endif
      }
    OSScheduler::criticalExit();

  }

#endif /* !defined(OS_EXCLUDE_MULTITASKING) */
