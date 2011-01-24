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
        OS_GPIO_PIN_CONFIG_ENABLE(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
        // Turn off led, i.e. HIGH if active low
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
        OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
        OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
        OS_GPIO_PIN_CONFIG_OUTPUT(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT);
#else
#error "OS_CONFIG_ACTIVE_LED_* missing"
#endif
  }

/* Turn LED on (on interrupts) */
inline void OSScheduler::ISRledActiveOn(void)
  {
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
    OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
    OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
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
#if defined(OS_CONFIG_ACTIVE_LED_ISACTIVE_LOW)
    OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#else
    OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
#endif
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
