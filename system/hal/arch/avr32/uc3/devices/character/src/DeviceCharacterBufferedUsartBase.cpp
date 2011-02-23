/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE)

#include "portable/devices/character/include/DeviceCharacterUsart.h"

// ----- constructors --------------------------------------------------------

DeviceCharacterBufferedUsartBase::DeviceCharacterBufferedUsartBase(void)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("DeviceCharacterBufferedUsartBase()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }

void DeviceCharacterBufferedUsartBase::setPortAddress(void *pPort)
{
  m_pPort = pPort;
}


virtual int implPortInit(void)
{
  ; // TODO: Implement it
  return 0;
}

virtual int implPortDisable(void)
{
  ; // TODO: Implement it
  return 0;
}

 void
DeviceCharacterBufferedUsartBase::implInterruptTxEnable(void)
{
  ; // TODO: Implement it
}

 void
DeviceCharacterBufferedUsartBase::implInterruptTxDisable(void)
{
  ; // TODO: Implement it
}

 unsigned char
DeviceCharacterBufferedUsartBase::implPortRead(void)
{
  ; // TODO: Implement it
  return 0;
}

 void
DeviceCharacterBufferedUsartBase::implPortWrite(unsigned char b)
{
  ; // TODO: Implement it
}


#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

