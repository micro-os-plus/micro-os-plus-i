/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
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

  void toggle(void);
  void init(void);

  unsigned char bitNumber(void);

private:
  unsigned char m_iBit;
  };

inline LedVar::LedVar(unsigned char iBit)
  {
    m_iBit = iBit;
  }

inline void LedVar::init(void)
  {
    // init led port as output
    OS_GPIO_PORT_CONFIG_OUTPUT(APP_CONFIG_LED_PORT_CONFIG, m_iBit);
    OS_GPIO_PORT_HIGH(APP_CONFIG_LED_PORT_CONFIG, m_iBit);
  }

inline void LedVar::toggle(void)
  {
    os.sched.criticalEnter();
      {
        // toggle led
        OS_GPIO_PORT_TOGGLE(APP_CONFIG_LED_PORT, m_iBit);
      }
    os.sched.criticalExit();
  }

inline unsigned char LedVar::bitNumber(void)
  {
    return m_iBit;
  }

#endif /*LEDVAR_H_*/
