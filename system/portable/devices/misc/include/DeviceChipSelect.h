/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHIPSELECT_H_
#define DEVICECHIPSELECT_H_

#include "portable/kernel/include/OS.h"

class DeviceChipSelect
{
public:
  static void
  implSelectInit(void);
  static void
  implSelectEnable(void);
  static void
  implSelectDisable(void);
};

#endif /* DEVICECHIPSELECT_H_ */
