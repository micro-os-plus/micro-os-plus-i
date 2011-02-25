/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICEMEMCARDSELECT_INLINES_H_
#define HAL_DEVICEMEMCARDSELECT_INLINES_H_

inline void DeviceMemCardSelect::implSelectInit(void)
  {
    OS_CONFIG_DEVICEMEMCARDSELECT_PORT_INIT
        |= _BV(OS_CONFIG_DEVICEMEMCARDSELECT_PORT_BIT);
    OS_CONFIG_DEVICEMEMCARDSELECT_PORT
        |= _BV(OS_CONFIG_DEVICEMEMCARDSELECT_PORT_BIT);
  }

inline void DeviceMemCardSelect::implSelectEnable(void)
  {
    OS_CONFIG_DEVICEMEMCARDSELECT_PORT
        &= ~_BV(OS_CONFIG_DEVICEMEMCARDSELECT_PORT_BIT);
  }

inline void DeviceMemCardSelect::implSelectDisable(void)
  {
    OS_CONFIG_DEVICEMEMCARDSELECT_PORT
        |= _BV(OS_CONFIG_DEVICEMEMCARDSELECT_PORT_BIT);
  }

#endif /*HAL_DEVICEMEMCARDSELECT_INLINES_H_*/
