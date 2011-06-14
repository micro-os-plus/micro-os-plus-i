/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERUSB)

#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/DeviceCharacterUsb.h"

// ----- Constructors ---------------------------------------------------------

DeviceCharacterUsb::DeviceCharacterUsb(int device)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterUsb(");
  OSDeviceDebug::putDec((unsigned short) device);
  OSDeviceDebug::putString(")=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  m_device = device;
  if (m_device == 0)
    {
      m_tx_ep = TX_EP;
      m_rx_ep = RX_EP;
      m_int_ep = INT_EP;
      g_usb0 = this;
    }
  else
    {
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
      m_tx_ep = TXb_EP;
      m_rx_ep = RXb_EP;
      m_int_ep = INTb_EP;
      g_usb1 = this;
#endif /* defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE) */
    }

#if defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE)

  m_type = OS_DEVICECHARACTER_USB_CDC;
#endif /* defined(OS_INCLUDE_OSDEVICECHARACTER_TYPE) */
}

DeviceCharacterUsb::~DeviceCharacterUsb()
{
  OSDeviceDebug::putDestructor("DeviceCharacterUsb", this);
}

void
DeviceCharacterUsb::setOpened(bool flag)
{
  m_opened = flag;
}

DeviceCharacterUsb *g_usb0;
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
DeviceCharacterUsb *g_usb1;
#endif /* defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE) */

#endif /* OS_INCLUDE_DEVICECHARACTERUSB */
