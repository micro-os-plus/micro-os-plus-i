/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHIPSELECT_H_
#define DEVICECHIPSELECT_H_

class DeviceChipSelect
  {
public:
  static void implSelectInit(void);
  static void implSelectEnable(void);
  static void implSelectDisable(void);
  };

#endif /*DEVICECHIPSELECT_H_*/
