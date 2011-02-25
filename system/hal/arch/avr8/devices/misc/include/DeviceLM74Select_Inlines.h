/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICELM74SELECT_INLINES_H_
#define DEVICELM74SELECT_INLINES_H_

inline void DeviceLM74Select::implSelectInit(void)
  {
    OS_CONFIG_LM74_SELECT_PORT |= _BV(OS_CONFIG_LM74_SELECT_BIT);
    OS_CONFIG_LM74_SELECT_PORT_INIT |= _BV(OS_CONFIG_LM74_SELECT_BIT);
  }

inline void DeviceLM74Select::implSelectEnable(void)
  {
    OS_CONFIG_LM74_SELECT_PORT &= ~_BV(OS_CONFIG_LM74_SELECT_BIT);
  }

inline void DeviceLM74Select::implSelectDisable(void)
  {
    OS_CONFIG_LM74_SELECT_PORT |= _BV(OS_CONFIG_LM74_SELECT_BIT);
  }

#endif /*DEVICELM74SELECT_INLINES_H_*/
