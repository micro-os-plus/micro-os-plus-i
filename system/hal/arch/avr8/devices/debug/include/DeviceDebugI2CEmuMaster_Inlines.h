/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_
#define HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_

#include "avr/io.h"
#include "avr/wdt.h"

// ---------------------------------------------------------------------------

// emulate i2c master by bitbanging

inline void masterInit(void)
  {
    ;
  }

inline bool OSDeviceDebug::implCanTransmit(void)
  {
    return true;
  }

inline bool OSDeviceDebug::implDevicePresent(void)
  {
    return !masterSdaIsLow();
  }

inline void OSDeviceDebug::implWDReset(void)
  {
    wdt_reset()
;}

// ---

inline void masterSclInit(void)
  {
    OS_CONFIG_DEBUG_SCL_PORT &= ~_BV( OS_CONFIG_DEBUG_SCL_BIT );
    OS_CONFIG_DEBUG_SCL_PORT_INIT &= ~_BV( OS_CONFIG_DEBUG_SCL_BIT );
  }

inline void masterSclHigh(void)
  {
    OS_CONFIG_DEBUG_SCL_PORT_INIT &= ~_BV( OS_CONFIG_DEBUG_SCL_BIT );
  }

inline void masterSclLow(void)
  {
    OS_CONFIG_DEBUG_SCL_PORT_INIT |= _BV( OS_CONFIG_DEBUG_SCL_BIT );
  }

inline bool masterSclIsLow(void)
  {
    return ( (OS_CONFIG_DEBUG_SCL_PORT_READ & _BV( OS_CONFIG_DEBUG_SCL_BIT ) ) == 0 ) ? true : false;
  }

inline void masterSdaInit(void)
  {
    OS_CONFIG_DEBUG_SDA_PORT &= ~_BV( OS_CONFIG_DEBUG_SDA_BIT );
    OS_CONFIG_DEBUG_SDA_PORT_INIT &= ~_BV( OS_CONFIG_DEBUG_SDA_BIT );
  }

inline void masterSdaHigh(void)
  {
    OS_CONFIG_DEBUG_SDA_PORT_INIT &= ~_BV( OS_CONFIG_DEBUG_SDA_BIT );
  }

inline void masterSdaLow(void)
  {
    OS_CONFIG_DEBUG_SDA_PORT_INIT |= _BV( OS_CONFIG_DEBUG_SDA_BIT );
  }

inline bool masterSdaIsLow(void)
  {
    return ( (OS_CONFIG_DEBUG_SDA_PORT_READ & _BV( OS_CONFIG_DEBUG_SDA_BIT ) ) == 0 ) ? true : false;
  }

inline void masterSclSynchronize(void)
  {
    while (masterSclIsLow() )
      {
        wdt_reset()
;    }
}

inline void masterSdaSynchronize(void)
  {
    while (masterSdaIsLow() )
      {
        wdt_reset()
;    }
}
#endif /*HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_*/
