/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_
#define HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_

// ---------------------------------------------------------------------------

// emulate i2c master by bitbanging

inline bool
OSDeviceDebug::implCanTransmit(void)
{
  return true;
}

inline bool
OSDeviceDebug::implDevicePresent(void)
{
  return !masterSdaIsLow();
}

inline void
OSDeviceDebug::implWDReset(void)
{
#if false
  IWDG_ReloadCounter();
#endif
}

// ---

inline void
masterInit(void)
{
  ;
}

inline void
masterSclInit(void)
{
  OS_GPIO_PORT_LOW(OS_CONFIG_DEBUG_SCL_PORT, OS_CONFIG_DEBUG_SCL_BIT);
  OS_GPIO_PORT_CONFIG_INPUT(OS_CONFIG_DEBUG_SCL_PORT_CONFIG,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline void
masterSclHigh(void)
{
  // The pull-up will do the job
  OS_GPIO_PORT_CONFIG_INPUT(OS_CONFIG_DEBUG_SCL_PORT_CONFIG,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline void
masterSclLow(void)
{
  OS_GPIO_PORT_CONFIG_OUTPUT(OS_CONFIG_DEBUG_SCL_PORT_CONFIG,
      OS_CONFIG_DEBUG_SCL_BIT);
}

inline bool
masterSclIsLow(void)
{
  return OS_GPIO_PORT_ISLOW(OS_CONFIG_DEBUG_SCL_PORT, OS_CONFIG_DEBUG_SCL_BIT) ? true
      : false;
}

inline void
masterSdaInit(void)
{
  OS_GPIO_PORT_LOW(OS_CONFIG_DEBUG_SDA_PORT, OS_CONFIG_DEBUG_SDA_BIT);
  OS_GPIO_PORT_CONFIG_INPUT(OS_CONFIG_DEBUG_SDA_PORT_CONFIG,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline void
masterSdaHigh(void)
{
  // The pull-up will do the job
  OS_GPIO_PORT_CONFIG_INPUT(OS_CONFIG_DEBUG_SDA_PORT_CONFIG,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline void
masterSdaLow(void)
{
  OS_GPIO_PORT_CONFIG_OUTPUT(OS_CONFIG_DEBUG_SDA_PORT_CONFIG,
      OS_CONFIG_DEBUG_SDA_BIT);
}

inline bool
masterSdaIsLow(void)
{
  return OS_GPIO_PORT_ISLOW(OS_CONFIG_DEBUG_SDA_PORT, OS_CONFIG_DEBUG_SDA_BIT) ? true
      : false;
}

inline void
masterSclSynchronize(void)
{
  while (masterSclIsLow())
    {
#if false
      IWDG_ReloadCounter();
#endif
    }
}

inline void
masterSdaSynchronize(void)
{
  while (masterSdaIsLow())
    {
#if false
      IWDG_ReloadCounter();
#endif
    }
}
#endif /*HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_*/
