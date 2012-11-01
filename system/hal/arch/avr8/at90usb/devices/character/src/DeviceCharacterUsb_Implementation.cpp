/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AT90USB)

#if defined(OS_INCLUDE_DEVICECHARACTERUSB)

#include "portable/devices/character/include/DeviceCharacterUsb.h"
#include "portable/devices/usb/include/OSUsbDevice.h"

// ----- Implementation code --------------------------------------------------

int
DeviceCharacterUsb::implOpen()
{
  OSCriticalSection::enter();
    {
      OSDeviceDebug::putString("DeviceCharacterUsb::open()");
      OSDeviceDebug::putNewLine();
    }
  OSCriticalSection::exit();

  static bool flagShouldNotInit;

  if (!flagShouldNotInit)
    {
#if !defined(USE_USB_PADS_REGULATOR_ENABLE)	// Otherwise assume USB PADs regulator is not used
      OSUsbDevice::Usb_enable_regulator();
#endif

      OSUsbDevice::Usb_force_device_mode();

      //Enable_interrupt();
      OSUsbDevice::Usb_disable();
      OSUsbDevice::Usb_enable();
      OSUsbDevice::Usb_select_device();
      OSUsbDevice::Usb_enable_vbus_interrupt();
      //Enable_interrupt();

      flagShouldNotInit = true;

      OSUsbLed::init();
      //DDRD |= _BV(DDD6);
      //DDRD |= _BV(DDD7);
    }

  m_txCounter = 0;
  m_rxCounter = 0;
  m_opened = false;
  return 0;
}

OSEvent_t
DeviceCharacterUsb::implGetOpenEvent(void)
{
  return (OSEvent_t) &m_opened;
}

bool
DeviceCharacterUsb::implIsConnected()
{
  //OSDeviceDebug::putString("DeviceCharacterUsb::isConnected()");
  //    OSDeviceDebug::putNewLine();
  return m_opened;
}

int
DeviceCharacterUsb::implClose()
{
  OSDeviceDebug::putString("DeviceCharacterUsb::close()");
  OSDeviceDebug::putNewLine();

  return 0;
}

bool
DeviceCharacterUsb::implCanWrite(void)
{
  return true;
}

OSEvent_t
DeviceCharacterUsb::implGetWriteEvent(void)
{
  return (OSEvent_t) &m_txCounter;
}

int
DeviceCharacterUsb::implWriteByte(unsigned char b)
{
  // if closed return -1
  if (!m_opened)
    return OSReturn::OS_DISCONNECTED;

  OSCriticalSection::enter();
    {
      while (!OSUsbDevice::Is_usb_tx_ready())
        {
          //OSDeviceDebug::putChar('>');
          //os.sched.eventWait(&m_txCounter);       // Wait Endpoint ready
          OSUsbDevice::endpointSelect(m_tx_ep);
        }

      OSUsbDevice::endpointSelect(m_tx_ep);
      OSUsbDevice::writeByte(b);
      m_txCounter++;

#if defined(OS_DEBUG_DEVICECHARACTERUSB_WRITE)

      OSDeviceDebug::putChar('}');
      if (b >= ' ')
      OSDeviceDebug::putChar(b);
      else
      OSDeviceDebug::putHex(b);

#endif

      if (!OSUsbDevice::Is_usb_tx_ready()) //If Endpoint full -> flush
        {
          OSUsbDevice::Usb_send_in();
          //OSDeviceDebug::putChar('^');
          m_txCounter = 0;
        }
    }
  OSCriticalSection::exit();
  return b;
}

int
DeviceCharacterUsb::implWriteBytes(const unsigned char* pBuf, int size)
{
#if true

  // if closed return -1
  if (!m_opened)
    return OSReturn::OS_DISCONNECTED;

  if (size == 0)
    {
      return 0;
    }

#if false
  // TODO: implement properly, it does not work
  bool status;
  status = OSUsbDevice::Is_usb_write_enabled();

  if (!status)
    {
      return 0;
    }
#endif

  int i;
  OSCriticalSection::enter();
    {
      while (!OSUsbDevice::Is_usb_tx_ready())
        {
          //OSDeviceDebug::putChar('>');
          //os.sched.eventWait(&m_txCounter);       // Wait Endpoint ready
          OSUsbDevice::endpointSelect(m_tx_ep);
        }

      for (i = 0; i < size; ++i)
        {
          OSUsbDevice::endpointSelect(m_tx_ep);
          OSUsbDevice::writeByte(pBuf[i]);
          m_txCounter++;

#if defined(OS_DEBUG_DEVICECHARACTERUSB_WRITE)

          unsigned char b;
          b = pBuf[i];
          OSDeviceDebug::putChar('}');
          if (b >= ' ')
          OSDeviceDebug::putChar(b);
          else
          OSDeviceDebug::putHex(b);

#endif

          if (!OSUsbDevice::Is_usb_tx_ready()) //If Endpoint full -> flush
            {
              OSUsbDevice::Usb_send_in();
              //OSDeviceDebug::putChar('^');
              m_txCounter = 0;

              // Return when endpoint full
              break;
            }

        }
    }
  OSCriticalSection::exit();

  return i;

#else

  return OSDeviceCharacter::implWriteBytes(pBuf, size);

#endif
}

int
DeviceCharacterUsb::implFlush(void)
{
  // if closed return -1
  if (!m_opened)
    return OSReturn::OS_DISCONNECTED;

  OSCriticalSection::enter();
    {
      if (m_txCounter != 0)
        {
          OSUsbDevice::endpointSelect(m_tx_ep);
          OSUsbDevice::Usb_send_in();
          //OSDeviceDebug::putChar('^');
          m_txCounter = 0;
        }
    }
  OSCriticalSection::exit();
  return 0;
}

bool
DeviceCharacterUsb::implCanRead()
{
  return DeviceCharacterUsb::implAvailableRead() != 0;
}

int
DeviceCharacterUsb::implAvailableRead(void)
{
  OSCriticalSection::enter();
    {
      OSUsbDevice::endpointSelect(m_rx_ep);
      m_rxCounter = OSUsbDevice::Usb_byte_counter();
    }
  OSCriticalSection::exit();
  return m_rxCounter;
}

OSEvent_t
DeviceCharacterUsb::implGetReadEvent(void)
{
  return (OSEvent_t) &m_rxCounter;
}

int
DeviceCharacterUsb::implReadByte(void)
{
  int c;
  OSCriticalSection::enter();
    {
      OSUsbDevice::endpointSelect(m_rx_ep);
      c = OSUsbDevice::readByte();
      m_rxCounter = OSUsbDevice::Usb_byte_counter();
      if (m_rxCounter == 0)
        {
          //OSUsbDevice::Usb_ack_receive_out();
          OSUsbDevice::Usb_ack_fifocon();
        }

#if defined(OS_DEBUG_DEVICECHARACTERUSB_READ)

      OSDeviceDebug::putChar('{');
      if (c >= ' ')
      OSDeviceDebug::putChar(c);
      else
      OSDeviceDebug::putHex((unsigned char) c);

#endif

    }
  OSCriticalSection::exit();
  // OS_CONFIG_USBINT_LED_PORT &= ~_BV(PORTD0);
  return c;
}

int
DeviceCharacterUsb::implReadBytes(unsigned char* pBuf, int size)
{
  // TODO: implement it here
  return OSDeviceCharacter::implReadBytes(pBuf, size);
}

// USB CDC support functions

void
DeviceCharacterUsb::specificCdcInterruptServiceRoutine(void)
{
  if (UEINT & _BV(RX_EP))
    {
#if defined(OS_CONFIG_USBINT_LED_PORT)
      OS_CONFIG_USBINT_LED_PORT |= _BV(OS_CONFIG_USBINT_LED_BIT);
#endif

      OSUsbDevice::endpointSelect(RX_EP);
      if (OSUsbDevice::isInterruptReceiveOut()
          && OSUsbDevice::isInterruptReceiveOutEnabled())
        {
          OSDeviceDebug::putString("Vo");
          OSDeviceDebug::putNewLine();

          OSUsbDevice::interruptReceiveOutAck();
          OSScheduler::eventNotify(g_usb0->getReadEvent());
        }
    }
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
  if (UEINT & _BV(RXb_EP))
    {
#if defined(OS_CONFIG_USBINT_LED_PORT)
      OS_CONFIG_USBINT_LED_PORT |= _BV(OS_CONFIG_USBINT_LED_BIT);
#endif

      OSUsbDevice::endpointSelect(RXb_EP);
      if (OSUsbDevice::isInterruptReceiveOut() && OSUsbDevice::isInterruptReceiveOutEnabled())
        {
          OSDeviceDebug::putString("Vo b");
          OSDeviceDebug::putNewLine();

          OSUsbDevice::interruptReceiveOutAck();
          os.sched.eventNotify(g_usb1->getReadEvent());
        }
    }
#endif
}

/*
 *  This function is called by the standard usb read request function to 
 * process specific requests. Return TRUE when the request is processed. 
 */

bool
DeviceCharacterUsb::specificCdcProcessReadRequest(
    unsigned char __attribute__((unused)) type, unsigned char request)
{
  // TODO: for multi-interface devices, determine pointer
  DeviceCharacterUsb *pDev = g_usb0;

  switch (request)
    {
  case USB_CDC_GET_LINE_CODING:
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
    OSDeviceDebug::putString("GET_LINE_CODING");
#endif
    pDev->cdcGetLineCoding();
    return true;

  case USB_CDC_SET_LINE_CODING:
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
    OSDeviceDebug::putString("SET_LINE_CODING");
#endif
    pDev->cdcSetLineCoding();
    return true;

  case USB_CDC_SET_CONTROL_LINE_STATE:
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
    OSDeviceDebug::putString("SET_CONTROL_LINE_STATE");
#endif
    pDev->cdcSetControlLineState();
    return true;

  default:
    return false;

    }
  return false;
}

/*
 * Set the size and the pointer on a user information structure
 * Return true if t
 */

bool
DeviceCharacterUsb::specificCdcGetDescriptor(unsigned char type,
    unsigned char index)
{
  switch (type)
    {
  case STRING_DESCRIPTOR:
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
    OSDeviceDebug::putString("STRING ");
    OSDeviceDebug::putHex(index);
    //OSDeviceDebug::putNewLine();
#endif
    switch (index)
      {
    case LANG_ID: // zero index, return an array of suported LANGID codes
      OSUsbDevice::usb_set_return(sizeof(usb_user_language_id),
          &usb_user_language_id);
      return true;

    case STRING_INDEX_MAN:
      OSUsbDevice::usb_set_return(
          sizeof(usb_user_manufacturer_string_descriptor),
          &usb_user_manufacturer_string_descriptor);
      return true;

    case STRING_INDEX_PROD:
      OSUsbDevice::usb_set_return(sizeof(usb_user_product_string_descriptor),
          &usb_user_product_string_descriptor);
      return true;

    case STRING_INDEX_SN:
      OSUsbDevice::usb_set_return(sizeof(usb_user_serial_number),
          &usb_user_serial_number);
      return true;

    default:
      return false;
      }

  case DEVICE_QUALIFIER_DESCRIPTOR:
    OSDeviceDebug::putString("DEVICE QUALIFIER - ???");
    OSDeviceDebug::putNewLine();
    return false; // not yet implemented

  default:
    return false;
    }

  return false;
}

/*
 * Get line coding parameters (baudrate...) from the device to the host.
 */

void
DeviceCharacterUsb::cdcGetLineCoding(void)
{
  OSUsbDevice::Usb_ack_receive_setup();

  OSUsbDevice::writeLong(m_lineCoding.baudRate);
  OSUsbDevice::writeByte(m_lineCoding.charFormat);
  OSUsbDevice::writeByte(m_lineCoding.parityType);
  OSUsbDevice::writeByte(m_lineCoding.dataBits);

  OSUsbDevice::Usb_send_control_in();

  while (!(OSUsbDevice::Is_usb_read_control_enabled()))
    {
      ;
    }
  //Usb_clear_tx_complete();

  while (!OSUsbDevice::isInterruptReceiveOut())
    ;
  OSUsbDevice::Usb_ack_receive_out();

  OSDeviceDebug::putNewLine();
}

/*
 * Set the line coding parameters (baudrate...) on the device.
 */

void
DeviceCharacterUsb::cdcSetLineCoding(void)
{
  unsigned short value;

  value = OSUsbDevice::readWord();

#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString(" val=");
  OSDeviceDebug::putHex(value);
  OSDeviceDebug::putString(" ");
  //OSDeviceDebug::putNewLine();
#endif

  unsigned short index;
  index = OSUsbDevice::readWord();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("int=");
  OSDeviceDebug::putHex(index);
  OSDeviceDebug::putString(" ");
#endif

  OSUsbDevice::Usb_ack_receive_setup();
  while (!(OSUsbDevice::isInterruptReceiveOut()))
    {
      ;
    }

  m_lineCoding.baudRate = OSUsbDevice::readLong();
  m_lineCoding.charFormat = OSUsbDevice::readByte();
  m_lineCoding.parityType = OSUsbDevice::readByte();
  m_lineCoding.dataBits = OSUsbDevice::readByte();
  OSUsbDevice::Usb_ack_receive_out();

#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("rate=");
#if defined(OS_INCLUDE_OSDEVICEDEBUG_PUTDEC_LONG)
  OSDeviceDebug::putDec(m_lineCoding.baudRate);
#else
  OSDeviceDebug::putString("0x");
  OSDeviceDebug::putHex((unsigned short) (m_lineCoding.baudRate >> 16));
  OSDeviceDebug::putHex((unsigned short) (m_lineCoding.baudRate & 0xFFFF));
#endif
  OSDeviceDebug::putString(", fmt=");
  OSDeviceDebug::putDec((unsigned short) m_lineCoding.charFormat);
  OSDeviceDebug::putString(", par=");
  OSDeviceDebug::putDec((unsigned short) m_lineCoding.parityType);
  OSDeviceDebug::putString(", bits=");
  OSDeviceDebug::putDec((unsigned short) m_lineCoding.dataBits);
  OSDeviceDebug::putNewLine();
#endif

  OSUsbDevice::Usb_send_control_in(); // send a ZLP for STATUS phase
  while (!(OSUsbDevice::Is_usb_read_control_enabled()))
    {
      ;
    }
  //Uart_set_baudrate(m_lineCoding.baudRate);
}

/*
 * Manage the line state (DTR).
 *
 * TODO: Manage hardware flow control...
 */

void
DeviceCharacterUsb::cdcSetControlLineState()
{
  unsigned short value;

  value = OSUsbDevice::readWord();

#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString(" val=");
  OSDeviceDebug::putHex(value);
  OSDeviceDebug::putString(" ");
  //OSDeviceDebug::putNewLine();
#endif

  unsigned short index;
  index = OSUsbDevice::readWord();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("int=");
  OSDeviceDebug::putHex(index);
  OSDeviceDebug::putString(" ");
#endif

  bool opened;
  opened = (value & 0x0001) ? true : false;

  if (index == IF0_NB)
    {
      g_usb0->m_opened = opened;
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
      OSDeviceDebug::putString("set ");
#endif
    }
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
  if (index == IF0b_NB)
  g_usb1->m_opened = opened;
#endif

  if (opened)
    {
      OSUsbLed::off(); // will be toggled to on() at interrupt prolog
      // Note: interface numbers must start at 0 to match index!!!
      if (index == IF0_NB)
        {
          OSScheduler::eventNotify(g_usb0->getOpenEvent());
#if defined(DEBUG) && false
          OSDeviceDebug::putString("eventNotify(");
          OSDeviceDebug::putHex((unsigned short)(g_usb0->getOpenEvent()));
          OSDeviceDebug::putString(") ");
#endif
        }
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
      if (index == IF0b_NB)
      os.sched.eventNotify(g_usb1->getOpenEvent());
#endif
    }
  else
    {
      OSUsbLed::on(); // will be toggled to off() at interrupt prolog
      if (index == IF0_NB)
        {
          OSScheduler::eventNotify(g_usb0->getReadEvent());
#if defined(DEBUG) && false
          OSDeviceDebug::putString("eventNotify(");
          OSDeviceDebug::putHex((unsigned short)(g_usb0->getReadEvent()));
          OSDeviceDebug::putString(") ");
#endif
        }
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
      if (index == IF0b_NB)
      os.sched.eventNotify(g_usb1->getReadEvent());
#endif
    }

#if defined(DEBUG)
  if (opened)
    {
      OSDeviceDebug::putString("DTR ready");
    }
  else
    {
      OSDeviceDebug::putString("DTR not ready");
    }
  OSDeviceDebug::putNewLine();
#endif

  OSUsbDevice::Usb_ack_receive_setup();
  OSUsbDevice::Usb_send_control_in();
  while (!(OSUsbDevice::Is_usb_read_control_enabled()))
    {
      ;
    }

}

/*
 * Configure all associated endpoints.
 */

void
DeviceCharacterUsb::specificCdcEndpointInit(
    unsigned char __attribute__((unused)) conf_nb)
{
  OSUsbDevice::usb_configure_endpoint(INT_EP, TYPE_INTERRUPT, DIRECTION_IN,
      SIZE_32, ONE_BANK, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(TX_EP, TYPE_BULK, DIRECTION_IN, SIZE_32,
      ONE_BANK, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(RX_EP, TYPE_BULK, DIRECTION_OUT, SIZE_32,
      TWO_BANKS, NYET_ENABLED);

  OSUsbDevice::endpointSelect(RX_EP);
  OSUsbDevice::interruptReceiveOutEnable();

  OSUsbDevice::endpointReset(INT_EP);
  OSUsbDevice::endpointReset(TX_EP);
  OSUsbDevice::endpointReset(RX_EP);

#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
  OSUsbDevice::usb_configure_endpoint(INTb_EP, TYPE_INTERRUPT, DIRECTION_IN, SIZE_32,
      ONE_BANK, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(TXb_EP, TYPE_BULK, DIRECTION_IN, SIZE_32, ONE_BANK,
      NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(RXb_EP, TYPE_BULK, DIRECTION_OUT, SIZE_32, TWO_BANKS,
      NYET_ENABLED);

  OSUsbDevice::endpointSelect(RXb_EP);
  OSUsbDevice::interruptReceiveOutEnable();

  OSUsbDevice::endpointReset(INTb_EP);
  OSUsbDevice::endpointReset(TXb_EP);
  OSUsbDevice::endpointReset(RXb_EP);
#endif
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_DEVICECHARACTERUSB) */

#endif /* defined(OS_CONFIG_FAMILY_AT90USB) */
