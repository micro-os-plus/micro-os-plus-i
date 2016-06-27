/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OSSCHEDULER_INLINES_H_
#define HAL_FAMILY_OSSCHEDULER_INLINES_H_

//------------------------------------------------------------------------------------

/*
 * Manage the activity LED
 */
inline void
OSScheduler::ledActiveInit(void)
{
  // enable clock for APB2 where GPIOC is located
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

#if defined(OS_CONFIG_ACTIVE_LED_PORT_WRITE) && defined(OS_CONFIG_ACTIVE_LED_PORT_CONFIG)
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
  OS_GPIO_PIN_CONFIG(OS_CONFIG_ACTIVE_LED_PORT_CONFIG, OS_CONFIG_ACTIVE_LED_BIT, OS_CFV(GPIO_Mode_Out_PP, GPIO_Speed_50MHz));
#else
#error "OS_CONFIG_ACTIVE_LED_* missing"
#endif
}

/* Turn LED on (on interrupts) */
inline void
OSScheduler::ISR_ledActiveOn(void)
{
  OS_GPIO_PIN_LOW(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
}

inline void
OSScheduler::ledActiveOn(void)
{
  OSCriticalSection::enter();
    {
      ISR_ledActiveOn();
    }
  OSCriticalSection::exit();
}

/* Turn LED off (on interrupts) */
inline void
OSScheduler::ISR_ledActiveOff(void)
{
  OS_GPIO_PIN_HIGH(OS_CONFIG_ACTIVE_LED_PORT_WRITE, OS_CONFIG_ACTIVE_LED_BIT);
}

/* Turn LED off (at sleep) */
inline void
OSScheduler::ledActiveOff(void)
{
  OSCriticalSection::enter();
    {
      ISR_ledActiveOff();
    }
  OSCriticalSection::exit();
}

// -----

inline bool
OSSchedulerImpl::isYieldAllowed(void)
{
  //return ((getSR() >> 22) & 0x7) > 1 ? false : true;
  return true;
}

#endif /* HAL_FAMILY_OSSCHEDULER_INLINES_H_ */
