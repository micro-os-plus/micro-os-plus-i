/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERUSB)

#include "portable/devices/character/include/DeviceCharacterUsb.h"

// ----- constructors --------------------------------------------------------

DeviceCharacterUsb::DeviceCharacterUsb(int device)
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("DeviceCharacterUsb(");
    OSDeviceDebug::putDec((unsigned short)device);
    OSDeviceDebug::putString(")=");
    OSDeviceDebug::putHex( ( unsigned short ) this );
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
#endif
      }

#if defined(OS_INCLUDE_DEVICECHARACTER_TYPE)

    m_type = OS_DEVICECHARACTER_USB_CDC;
#endif
  }

DeviceCharacterUsb::~DeviceCharacterUsb()
  {
    ;
  }

#if true
void DeviceCharacterUsb::setOpened(bool flag)
  {
    m_opened = flag;
  }
#endif

DeviceCharacterUsb *g_usb0;
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
DeviceCharacterUsb *g_usb1;
#endif

#endif /*OS_INCLUDE_DEVICECHARACTERUSB*/
