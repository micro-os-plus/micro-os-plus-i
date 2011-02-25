/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEADC_H_
#define DEVICEADC_H_

#include "portable/kernel/include/OS.h"

class DeviceADC
{
public:

  static void
  init(void);

  static void
  enable(void);
  static void
  disable(void);

  static void
  select(unsigned short channel);
  static unsigned short
  measure(void);

#if defined(OS_INCLUDE_DEVICEADC_ISR)
  static volatile bool ms_bCompleted;
#endif
};

#endif /*DEVICEADC_H_*/
