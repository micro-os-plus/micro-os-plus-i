/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMCP2510SELECT_INLINES_H_
#define DEVICEMCP2510SELECT_INLINES_H_

inline void DeviceMCP2510Select::implSelectInit(void)
  {
    // set high
    OS_CONFIG_MCP2510_SELECT_PORT |= _BV(OS_CONFIG_MCP2510_SELECT_BIT);
    // output
    OS_CONFIG_MCP2510_SELECT_PORT_INIT |= _BV(OS_CONFIG_MCP2510_SELECT_BIT);
  }

inline void DeviceMCP2510Select::implSelectEnable(void)
  {
    // set low
    OS_CONFIG_MCP2510_SELECT_PORT &= ~_BV(OS_CONFIG_MCP2510_SELECT_BIT);
  }

inline void DeviceMCP2510Select::implSelectDisable(void)
  {
    // set high
    OS_CONFIG_MCP2510_SELECT_PORT |= _BV(OS_CONFIG_MCP2510_SELECT_BIT);
  }

#endif /*DEVICEMCP2510SELECT_INLINES_H_*/
