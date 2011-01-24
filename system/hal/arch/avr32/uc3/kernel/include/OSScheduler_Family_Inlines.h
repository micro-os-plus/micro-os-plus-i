/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OSSCHEDULER_INLINES_H_
#define HAL_FAMILY_OSSCHEDULER_INLINES_H_

//------------------------------------------------------------------------------------

/*
 * Manage the activity LED
 */
inline void OSScheduler::ledActiveInit(void)
  {
#if defined(OS_CONFIG_ACTIVE_LED_PORT) && defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
        OS_GPIO_PORT_CONFIG_ENABLE(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
        OS_GPIO_PORT_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
        OS_GPIO_PORT_CONFIG_OUTPUT(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
#else
#error "OS_CONFIG_ACTIVE_LED_* missing"
#endif
  }

/* Turn LED on (on interrupts) */
inline void OSScheduler::ISRledActiveOn(void)
  {
    OS_GPIO_PORT_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
  }

inline void OSScheduler::ledActiveOn(void)
  {
    OSScheduler::criticalEnter();
      {
        ISRledActiveOn();
      }
    OSScheduler::criticalExit();
  }

/* Turn LED off (on interrupts) */
inline void OSScheduler::ISRledActiveOff(void)
  {
    OS_GPIO_PORT_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
  }

/* Turn LED off (at sleep) */
inline void OSScheduler::ledActiveOff(void)
  {
    OSScheduler::criticalEnter();
      {
        ISRledActiveOff();
      }
    OSScheduler::criticalExit();
    }

#endif /*HAL_FAMILY_OSSCHEDULER_INLINES_H_*/
