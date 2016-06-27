/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERUSB)

#include "portable/devices/usb/include/OSUsbDevice.h"
#include "portable/devices/character/include/DeviceCharacterUsb.h"
#include "hal/arch/avr32/lib/include/compiler.h"

// ----- Implementation code --------------------------------------------------

int
DeviceCharacterUsb::implOpen()
{
  OSDeviceDebug::putString(" DeviceCharacterUsb::implOpen() ");

  m_opened = true;

  m_baudRate = 0;

  //m_connected = false;
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
  return m_connected;
}

int
DeviceCharacterUsb::implClose()
{
  OSDeviceDebug::putString("DeviceCharacterUsb::implClose()");
  OSDeviceDebug::putNewLine();

  m_opened = false;

  return 0;
}

bool
DeviceCharacterUsb::implCanWrite(void)
{
  return OSUsbDevice::Is_usb_write_enabled();
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
  if (!m_connected)
    return OSReturn::OS_DISCONNECTED;

  OSUsbDevice::endpointSelect(m_tx_ep);

  while (!OSUsbDevice::Is_usb_write_enabled())
    {
      //OSDeviceDebug::putChar('>');
      //os.sched.eventWait(&m_txCounter);       // Wait Endpoint ready
      //OSUsbDevice::endpointSelect(m_tx_ep);
    }

  if (m_txCounter == 0)
    {
      OSUsbDevice::Usb_reset_endpoint_fifo_access(m_tx_ep);
    }
  AVR32_usb_write_endpoint_data(m_tx_ep, 8, b);
  m_txCounter++;

#if defined(OS_DEBUG_DEVICECHARACTERUSB_WRITE)

  OSDeviceDebug::putChar('}');
  if (b >= ' ')
  OSDeviceDebug::putChar(b);
  else
  OSDeviceDebug::putHex(b);

#endif

  if (!OSUsbDevice::Is_usb_write_enabled()) //If Endpoint full -> flush

    {
      implFlush();
    }

  return b;
}

int
DeviceCharacterUsb::implWriteBytes(const unsigned char* pBuf, int size)
{
  uint32_t availableSize, min;
  int32_t status;

  if (!m_connected)
    return OSReturn::OS_DISCONNECTED;

  if (size == 0)
    {
      return 0;
    }

  status = OSUsbDevice::Is_usb_write_enabled();

  if (status == 0)
    {
      return 0;
    }

  if (m_txCounter == 0)
    {
      // Reset known position inside FIFO
      // data register of selected endpoint
      AVR32_usb_reset_endpoint_fifo_access(m_tx_ep);
    }

  availableSize = AVR32_usb_get_endpoint_size(m_tx_ep) - m_txCounter;

  // min will hold the number of bytes to be write
  min = min(availableSize, size);

  m_txCounter += min;

  // write min bytes from pBuf
  OSUsbDevice::usbWriteEpTxPacket(m_tx_ep, pBuf, min, NULL);

  //If Endpoint full -> flush
  if (!AVR32_is_usb_write_enabled(m_tx_ep))
    {
      m_txCounter = 0;

      // acknowledge IN ready and sends current bank
      AVR32_usb_send_in(m_tx_ep);

      // check if there is a need for sending a zlp
      if (min == availableSize)
        {
          // wait for previous data to be sent
          while (!AVR32_is_usb_in_ready(m_tx_ep))
            ;
          // send zlp
          AVR32_usb_send_in(m_tx_ep);
        }
    }

  return min;
}

int
DeviceCharacterUsb::implFlush(void)
{
  bool zlp = false;

  // if closed return -1
  if (!m_connected)
    return OSReturn::OS_DISCONNECTED;

  if (m_txCounter != 0)
    {
      if (!AVR32_is_usb_write_enabled(m_tx_ep)) //If Endpoint full, add zlp
        {
          zlp = true;
        }

      AVR32_usb_send_in(m_tx_ep);
      //OSDeviceDebug::putChar('^');

      if (zlp == TRUE)
        {
          while (!AVR32_is_usb_write_enabled(m_tx_ep))
            ; // Wait Endpoint ready...
          AVR32_usb_send_in(m_tx_ep);
        }

      m_txCounter = 0;
    }

  return 0;
}

bool
DeviceCharacterUsb::implCanRead()
{
  return (DeviceCharacterUsb::implAvailableRead() != 0);
}

int
DeviceCharacterUsb::implAvailableRead(void)
{
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

  c = AVR32_usb_read_endpoint_data(m_rx_ep, 8);

  m_rxCounter--;

  if (m_rxCounter == 0)
    {
      AVR32_usb_ack_fifocon(m_rx_ep);
    }

#if defined(OS_DEBUG_DEVICECHARACTERUSB_READ)

  OSDeviceDebug::putChar('{');
  if (c >= ' ')
  OSDeviceDebug::putChar(c);
  else
  OSDeviceDebug::putHex((unsigned char) c);

#endif

  // OS_CONFIG_USBINT_LED_PORT_WRITE &= ~_BV(PORTD0);
  return c;
}

int
DeviceCharacterUsb::implReadBytes(unsigned char* pBuf, int size)
{
  uint32_t min;

  if (size == 0)
    {
      return 0;
    }

  if (m_rxCounter == 0)
    {
      return 0;
    }

  // available Size to read is: m_rxCounter
  // min will hold the number of bytes to be read
  min = min(m_rxCounter, size);

  // read min bytes into pBuf
  OSUsbDevice::usbReadEpRxPacket(m_rx_ep, pBuf, min, NULL);

  m_rxCounter -= min;

  // free current bank
  if (m_rxCounter == 0)
    AVR32_usb_free_out(m_rx_ep);

  return min;
}

// USB CDC support functions

void
DeviceCharacterUsb::specificCdcInterruptServiceRoutine(void)
{

#if TO_BE_PORTED
  if (UEINT & _BV(RX_EP))
#endif
    {
#if defined(OS_CONFIG_USBINT_LED_PORT_WRITE)
      OS_CONFIG_USBINT_LED_PORT_WRITE |= _BV(OS_CONFIG_USBINT_LED_BIT);
#endif

      OSUsbDevice::endpointSelect(RX_EP);
      if (OSUsbDevice::isInterruptReceiveOut()
          && OSUsbDevice::isInterruptReceiveOutEnabled())
        {
#if true
          OSDeviceDebug::putString("Vo");
          OSDeviceDebug::putNewLine();
#endif
          g_usb0->m_rxCounter = OSUsbDevice::Usb_byte_counter();
          if (g_usb0->m_rxCounter == 0) // if we have received a zlp
            {
              // acknowledge OUT received and frees current bank
              AVR32_usb_ack_out_received_free(g_usb0->m_rx_ep);
            }
          else
            {
#if false
              OSDeviceDebug::putString("by");
              OSDeviceDebug::putDec((uint16_t)g_usb0->m_rxCounter);
              OSDeviceDebug::putNewLine();
#endif
              OSUsbDevice::Usb_reset_endpoint_fifo_access(RX_EP);
              OSUsbDevice::Usb_ack_receive_out();
              OSScheduler::eventNotify(g_usb0->getReadEvent());
            }
        }
    }

  if (AVR32_is_usb_in_ready(TX_EP)
      && AVR32_is_usb_in_ready_interrupt_enabled(TX_EP))
    {
      OSUsbDevice::UsbEnableEndpointInterrupt(TX_EP);
      AVR32_usb_ack_in_ready(TX_EP);
      OSScheduler::eventNotify(g_usb0->getWriteEvent());
    }
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
  if (UEINT & _BV(RXb_EP))
    {
#if defined(OS_CONFIG_USBINT_LED_PORT_WRITE)
      OS_CONFIG_USBINT_LED_PORT_WRITE |= _BV(OS_CONFIG_USBINT_LED_BIT);
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

  OSUsbDevice::endpointSelect(EP_CONTROL);

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
  type = type;
  index = index;
  /*
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
   */
  return false;
}

/*
 * Get line coding parameters (baudrate...) from the device to the host.
 */

void
DeviceCharacterUsb::cdcGetLineCoding(void)
{
  OSUsbDevice::Usb_ack_receive_setup();

  OSUsbDevice::Usb_reset_endpoint_fifo_access(EP_CONTROL);
  OSUsbDevice::writeLong(m_lineCoding.baudRate);
  OSUsbDevice::writeByte(m_lineCoding.charFormat);
  OSUsbDevice::writeByte(m_lineCoding.parityType);
  OSUsbDevice::writeByte(m_lineCoding.dataBits);

  OSUsbDevice::Usb_send_control_in();
  while (!OSUsbDevice::Is_usb_read_control_enabled())
    ;

  while (!OSUsbDevice::isInterruptReceiveOut())
    ;
  OSUsbDevice::interruptReceiveOutAck();

  OSUsbDevice::Usb_ack_receive_out();

  OSDeviceDebug::putNewLine();
}

/*
 * Set the line coding parameters (baudrate...) on the device.
 */

void
DeviceCharacterUsb::cdcSetLineCoding(void)
{
  OSUsbDevice::Usb_ack_receive_setup();

  while (!OSUsbDevice::isInterruptReceiveOut())
    ;
  OSUsbDevice::Usb_reset_endpoint_fifo_access(EP_CONTROL);

  m_lineCoding.baudRate = OSUsbDevice::readLong();
  m_lineCoding.charFormat = OSUsbDevice::readByte();
  m_lineCoding.parityType = OSUsbDevice::readByte();
  m_lineCoding.dataBits = OSUsbDevice::readByte();
  OSUsbDevice::interruptReceiveOutAck();

  OSUsbDevice::Usb_ack_in_ready();
  OSUsbDevice::Usb_send_in();
  while (!OSUsbDevice::Is_usb_tx_ready())
    ;

  OSDeviceDebug::putDec(m_lineCoding.baudRate);
  OSDeviceDebug::putChar(' ');

  m_baudRate = m_lineCoding.baudRate;
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

  bool connected;
  connected = (value & 0x0001) ? true : false;
  // TODO: check this
  // connected = (value) ? true : false;

  if (index == IF0_NB)
    {
      g_usb0->m_connected = connected;
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
      OSDeviceDebug::putString("set ");
#endif
    }
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
  if (index == IF0b_NB)
  g_usb1->setConnected(connected);
#endif

  if (connected)
    {
      // TODO: check if it's ok to be here.
      // enable RX interrupt
      OSUsbDevice::endpointSelect(RX_EP);
      AVR32_usb_free_out(RX_EP);
      OSUsbDevice::UsbEnableEndpointInterrupt(RX_EP);
      OSUsbDevice::interruptReceiveOutEnable();
      OSUsbDevice::Usb_reset_endpoint_fifo_access(RX_EP);
      m_rxCounter = 0;

      // enable TX interrupt
      OSUsbDevice::UsbEnableEndpointInterrupt(TX_EP);
      AVR32_usb_enable_in_ready_interrupt(TX_EP);
      OSUsbDevice::Usb_reset_endpoint_fifo_access(TX_EP);
      m_txCounter = 0;

      OSUsbDevice::endpointSelect(EP_CONTROL);

      // OSUsbLed::off(); // will be toggled to on() at interrupt prolog
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
      // OSUsbLed::on(); // will be toggled to off() at interrupt prolog
      if (index == IF0_NB)
        {
          OSScheduler::eventNotify(g_usb0->getReadEvent());
          OSScheduler::eventNotify(g_usb0->getWriteEvent());
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
  if (connected)
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
#if (USB_HIGH_SPEED_SUPPORT==ENABLED)

  OSUsbDevice::usb_configure_endpoint(INT_EP, TYPE_INTERRUPT, DIRECTION_IN,
      SIZE_32, ONE_BANK, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(TX_EP, TYPE_BULK, DIRECTION_IN, EP_SIZE_1_HS,
      TWO_BANKS, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(RX_EP, TYPE_BULK, DIRECTION_OUT, EP_SIZE_2_HS,
      TWO_BANKS, NYET_ENABLED);

#else
  OSUsbDevice::usb_configure_endpoint(INT_EP, TYPE_INTERRUPT, DIRECTION_IN,
      SIZE_32, ONE_BANK, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(TX_EP, TYPE_BULK, DIRECTION_IN,
      EP_SIZE_1_FS, TWO_BANKS, NYET_ENABLED);

  OSUsbDevice::usb_configure_endpoint(RX_EP, TYPE_BULK, DIRECTION_OUT,
      EP_SIZE_2_FS, TWO_BANKS, NYET_ENABLED);
#endif
#if TO_BE_PORTED
  OSUsbDevice::endpointSelect(RX_EP);
  OSUsbDevice::interruptReceiveOutEnable();

  OSUsbDevice::endpointReset(INT_EP);
  OSUsbDevice::endpointReset(TX_EP);
  OSUsbDevice::endpointReset(RX_EP);
#endif
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

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
