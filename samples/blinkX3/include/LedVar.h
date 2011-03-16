/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef LEDVAR_H_
#define LEDVAR_H_

#include "portable/kernel/include/uOS.h"

class LedVar
{
public:
  LedVar(unsigned char iBit);

  void
  toggle(void);
  void
  init(void);

  unsigned char
  bitNumber(void);

private:
  unsigned char m_iBit;
};

inline
LedVar::LedVar(unsigned char iBit)
{
  m_iBit = iBit;
}

#if !defined(APP_CONFIG_LED_PORT_CONFIG) || !defined(APP_CONFIG_LED_PORT)
#error "APP_CONFIG_LED_PORT_* missing"
#endif

inline void
LedVar::init(void)
{
  os.sched.critical.enter();
    {
      // init led port as output
      OS_GPIO_PIN_CONFIG_OUTPUT(APP_CONFIG_LED_PORT_CONFIG, m_iBit);

#if defined(APP_CONFIG_LED_ISACTIVE_LOW)
      OS_GPIO_PIN_HIGH(APP_CONFIG_LED_PORT_CONFIG, m_iBit);
#else
      OS_GPIO_PIN_LOW(APP_CONFIG_LED_PORT_CONFIG, m_iBit);
#endif
    }
  os.sched.critical.exit();
}

inline void
LedVar::toggle(void)
{
  os.sched.critical.enter();
    {
      // toggle led
      OS_GPIO_PIN_TOGGLE(APP_CONFIG_LED_PORT, m_iBit);
    }
  os.sched.critical.exit();
}

inline unsigned char
LedVar::bitNumber(void)
{
  return m_iBit;
}

#endif /*LEDVAR_H_*/
