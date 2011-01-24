/*
 *      Copyright (C) 2009 Liviu Ionescu.
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
#if false
        // enable clock for APB2 where GPIOC is located
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
#endif

#if defined(OS_CONFIG_ACTIVE_LED_PORT) && defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
        OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
        OS_GPIO_PIN_CONFIG(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT, OS_CFV(GPIO_Mode_Out_PP, GPIO_Speed_50MHz));
#else
#error "OS_CONFIG_ACTIVE_LED_* missing"
#endif
  }

/* Turn LED on (on interrupts) */
inline void OSScheduler::ISRledActiveOn(void)
  {
    OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
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
    OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT, OS_CONFIG_ACTIVE_LED_BIT);
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
