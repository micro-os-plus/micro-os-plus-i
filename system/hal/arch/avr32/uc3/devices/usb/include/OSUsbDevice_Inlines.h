/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICE_INLINES_H_
#define HAL_OSUSBDEVICE_INLINES_H_

//----------------------------------------------------------------------------
inline void
OSUsbDevice::Usb_set_configuration_action(void)
{
  ;
}

inline void
OSUsbDevice::Usb_sof_action(void)
{
  ; //sof_action();
}

inline void
OSUsbDevice::Usb_wake_up_action(void)
{
  ;
}

inline void
OSUsbDevice::Usb_resume_action(void)
{
  ;
}

inline void
OSUsbDevice::Usb_suspend_action(void)
{
  OSDeviceDebug::putString("zzz...");
  OSDeviceDebug::putNewLine();

#if TO_BE_PORTED
  if (false)
    {
      //Enable_interrupt();
      //Enter_power_down_mode();
      cli();
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sei();
      sleep_cpu();
      sleep_disable();
    }
#endif
}

inline void
OSUsbDevice::Usb_reset_action(void)
{
  ;
}

inline void
OSUsbDevice::Usb_vbus_on_action(void)
{
  ;
}

inline void
OSUsbDevice::Usb_vbus_off_action(void)
{
  ;
}

//----------------------------------------------------------------------------


// select the endpoint number to interface with the CPU
inline void
OSUsbDevice::endpointSelect(unsigned char ep)
{
#if TO_BE_PORTED
  UENUM = ep;
#else
  m_selectedEndpoint = ep;
#endif
}

// reset the selected endpoint
inline void
OSUsbDevice::endpointReset(unsigned char ep)
{
#if TO_BE_PORTED
  UERST = _BV(ep);
  UERST = 0;
#else
  AVR32_usb_reset_endpoint(ep);
#endif
}

// test if SETUP interrupt received
inline bool
OSUsbDevice::isInterruptReceiveSetup(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(RXSTPI)) != 0) ? true : false;
#else
  return AVR32_is_usb_setup_received();
#endif
}

// test if SETUP interrupt enabled
inline bool
OSUsbDevice::isInterruptReceiveSetupEnabled(void)
{
#if TO_BE_PORTED
  return ((UEIENX & _BV(RXSTPE)) != 0) ? true : false;
#else
  return AVR32_is_usb_setup_received_interrupt_enabled();
#endif
}

// enable receive SETUP interrupt
inline void
OSUsbDevice::interruptReceiveSetupEnable(void)
{
#if TO_BE_PORTED
  UEIENX |= _BV(RXSTPE);
#else
  AVR32_usb_enable_setup_received_interrupt();
#endif
}

// tests if OUT received on selected endpoint
inline bool
OSUsbDevice::isInterruptReceiveOut(void)
{
#if TO_BE_PORTED
  return (((UEINTX & _BV(RXOUTI))) != 0) ? true : false;
#else
  return AVR32_is_usb_out_received(m_selectedEndpoint);
#endif
}

// test if receive OUT interrupt is enabled on selected endpoint
inline bool
OSUsbDevice::isInterruptReceiveOutEnabled(void)
{
#if TO_BE_PORTED
  return ((UEIENX & _BV(RXOUTE)) != 0) ? true : false;
#else
  return AVR32_is_usb_out_received_interrupt_enabled(m_selectedEndpoint);
#endif
}

// acks OUT received on selected endpoint and frees current bank
inline void
OSUsbDevice::interruptReceiveOutAck(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(RXOUTI);
#else
  AVR32_usb_ack_out_received(m_selectedEndpoint);
#endif
}

// enables OUT received interrupt on selected endpoint
inline void
OSUsbDevice::interruptReceiveOutEnable(void)
{
#if TO_BE_PORTED
  UEIENX |= _BV(RXOUTE);
#else
  AVR32_usb_enable_out_received_interrupt(m_selectedEndpoint);
#endif
}

extern U32 usb_read_ep_rxpacket(U8 ep, void *rxbuf, U32 data_length, void **prxbuf);

// return FIFO byte for current endpoint
inline unsigned char
OSUsbDevice::readByte(void)
{
#if TO_BE_PORTED
  return UEDATX;
#else
  int availableSize;
  int status;
  unsigned char ret;

  // tests if OUT received
  status = AVR32_is_usb_out_received(RX_EP);

  if( !status )
  {
      return 0;
  }

  // returns the byte count
  availableSize = AVR32_usb_byte_count(RX_EP);

  // Reset known position inside FIFO
  // data register of selected endpoint
  AVR32_usb_reset_endpoint_fifo_access(RX_EP);

  // read min bytes into pBuf
  usb_read_ep_rxpacket(RX_EP, &ret, 1, NULL);

  // acknowledge OUT received and frees current bank
  if(availableSize == 1)
    {
      AVR32_usb_ack_out_received_free(RX_EP);
    }

  return ret;
#endif
}

extern U32 usb_write_ep_txpacket(U8 ep, const void *txbuf, U32 data_length, const void **ptxbuf);

// write byte in FIFO for current endpoint
inline void
OSUsbDevice::writeByte(unsigned char b)
{
#if TO_BE_PORTED
  UEDATX = b;
#else
  int status;

  status = AVR32_is_usb_in_ready(TX_EP);

  if( !status )
  {
      return;
  }

  // Reset known position inside FIFO
  // data register of selected endpoint
  AVR32_usb_reset_endpoint_fifo_access(TX_EP);

  // write 1 byte
  usb_write_ep_txpacket(TX_EP, &b, 1, NULL);

  // acknowledge IN ready and sends current bank
  AVR32_usb_ack_in_ready_send(TX_EP);

  // wait for previous data to be sent
  while (!AVR32_is_usb_in_ready(TX_EP)) ;

#endif
}

// enables the STALL handshake for the next transaction
inline void
OSUsbDevice::Usb_enable_stall_handshake(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(STALLRQ);
#else
  AVR32_usb_enable_stall_handshake(m_selectedEndpoint);
#endif
}

// acks receive SETUP interrupt
inline void
OSUsbDevice::Usb_ack_receive_setup(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(RXSTPI);
#else
  AVR32_usb_ack_setup_received_free();
#endif
}

// Enable internal USB pads regulator
inline void
OSUsbDevice::Usb_enable_regulator(void)
{
#if TO_BE_PORTED
  UHWCON |=_BV(UVREGE);
#else

#endif
}

// Disable external UID pin
inline void
OSUsbDevice::Usb_disable_uid_pin(void)
{
#if TO_BE_PORTED
  UHWCON &= ~_BV(UIDE);
#else
  AVR32_usb_disable_id_pin();
#endif
}

// Disable external UID pin and force device mode
inline void
OSUsbDevice::Usb_force_device_mode(void)
{
#if TO_BE_PORTED
  UHWCON |= _BV(UIMOD);
#else
  AVR32_usb_force_device_mode();
#endif
}

// Disable both USB interface and Vbus pad
inline void
OSUsbDevice::Usb_disable(void)
{
#if TO_BE_PORTED
  USBCON &= ~(_BV(USBE) | _BV(OTGPADE));
#else
  AVR32_usb_disable();
#endif
}

// Enable both USB interface and Vbus pad
inline void
OSUsbDevice::Usb_enable(void)
{
#if TO_BE_PORTED
  USBCON |=(_BV(USBE) |_BV(OTGPADE));
#else
  AVR32_usb_enable();
#endif
}

// select device mode
inline void
OSUsbDevice::Usb_select_device(void)
{
#if TO_BE_PORTED
  USBCON &= ~_BV(HOST);
#else

#endif
}

// enable VBUS interrupt
inline void
OSUsbDevice::Usb_enable_vbus_interrupt(void)
{
#if TO_BE_PORTED
  USBCON |= _BV(VBUSTE);
#else
  AVR32_usb_enable_vbus_interrupt();
#endif
}

// test if write endpoint enabled
inline bool
OSUsbDevice::Is_usb_write_enabled(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
#else
  return AVR32_is_usb_write_enabled(TX_EP);
#endif
}

// test if transmit ready (same bit as allowed!)
inline bool
OSUsbDevice::Is_usb_tx_ready(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
#else
  return AVR32_is_usb_in_ready(TX_EP);
#endif
}

// send IN
inline void
OSUsbDevice::Usb_send_in(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(FIFOCON);
#else
  AVR32_usb_send_in(m_selectedEndpoint);
#endif
}

// return number of bytes in FIFO current endpoint (16 bits)
inline unsigned short
OSUsbDevice::Usb_byte_counter(void)
{
#if TO_BE_PORTED
  return UEBCX;
#else
  return AVR32_usb_byte_count(m_selectedEndpoint);
#endif
}

// clear FIFOCON bit
inline void
OSUsbDevice::Usb_ack_fifocon(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(FIFOCON);
#else
  AVR32_usb_ack_fifocon(m_selectedEndpoint);
#endif
}

inline bool
OSUsbDevice::Is_usb_vbus_transition(void)
{
#if TO_BE_PORTED
  return ((USBINT & (1<<VBUSTI)) != 0) ? true : false;
#else
  return AVR32_is_usb_vbus_transition();
#endif
}

inline bool
OSUsbDevice::Is_usb_vbus_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((USBCON & (1<<VBUSTE)) != 0) ? true : false;
#else
  return AVR32_is_usb_vbus_interrupt_enabled();
#endif
}

inline void
OSUsbDevice::Usb_ack_vbus_transition(void)
{
#if TO_BE_PORTED
  USBINT = ~_BV(VBUSTI);
#else
  AVR32_usb_ack_vbus_transition();
#endif
}

inline bool
OSUsbDevice::Is_usb_vbus_high(void)
{
#if TO_BE_PORTED
  return ((USBSTA & (1<<VBUS)) != 0) ? true : false;
#else
  return AVR32_is_usb_vbus_high();
#endif
}

// enable USB reset interrupt
inline void
OSUsbDevice::Usb_enable_reset_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN |= _BV(EORSTE);
#else
  AVR32_usb_enable_reset_interrupt();
#endif
}

// test if read allowed on control endpoint
inline bool
OSUsbDevice::Is_usb_read_control_enabled(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
#else
  return AVR32_is_usb_control_in_ready();
#endif
}

// send IN on control endpoint
inline void
OSUsbDevice::Usb_send_control_in(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(TXINI);
#else
  AVR32_usb_ack_control_in_ready_send();
#endif
}

// ack reveive OUT
inline void
OSUsbDevice::Usb_ack_receive_out(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(RXOUTI);
#endif
  AVR32_usb_ack_out_received(m_selectedEndpoint);
}

// ack IN ready
inline void
OSUsbDevice::Usb_ack_in_ready(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(TXINI);
#endif
  AVR32_usb_ack_in_ready(m_selectedEndpoint);
}

// test if IN ready
inline bool
OSUsbDevice::Is_usb_in_ready(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
#else
  return AVR32_is_usb_in_ready(m_selectedEndpoint);
#endif
}

// test if the current endpoint is enabled
inline bool
OSUsbDevice::Is_usb_endpoint_enabled(void)
{
#if TO_BE_PORTED
  return ((UECONX & _BV(EPEN)) != 0) ? true : false;
#else
  return AVR32_is_usb_endpoint_enabled(m_selectedEndpoint);
#endif
}

// disable the STALL handshake
inline void
OSUsbDevice::Usb_disable_stall_handshake(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(STALLRQC);
#else
  AVR32_usb_disable_stall_handshake(m_selectedEndpoint);
#endif
}

// reset the data toggle sequence
inline void
OSUsbDevice::Usb_reset_data_toggle(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(RSTDT);
#else
  AVR32_usb_reset_data_toggle(m_selectedEndpoint);
#endif
}

// enable USB device address
inline void
OSUsbDevice::Usb_enable_address(void)
{
#if TO_BE_PORTED
  UDADDR |= _BV(ADDEN);
#else
  AVR32_usb_enable_address();
#endif
}

// set the USB device address
inline void
OSUsbDevice::Usb_configure_address(unsigned char addr)
{
#if TO_BE_PORTED
  UDADDR = (UDADDR & _BV(ADDEN)) |(addr & MSK_UADD);
#else
  AVR32_usb_configure_address(addr);
#endif
}

inline void
OSUsbDevice::Usb_send_event(unsigned char event)
{
  g_usb_event |= (1 << event);
}

// attache to USB bus
inline void
OSUsbDevice::Usb_attach(void)
{
#if TO_BE_PORTED
  UDCON &= ~_BV(DETACH);
#else
  AVR32_usb_attach();
#endif
}

// test if Start Of Frame occurs
inline bool
OSUsbDevice::Is_usb_sof(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(SOFI)) != 0) ? true : false;
#else
  return AVR32_is_usb_sof();
#endif
}

inline bool
OSUsbDevice::Is_sof_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(SOFE)) != 0) ? true : false;
#else
  return AVR32_is_usb_sof_interrupt_enabled();
#endif
}

// ack Start Of Frame
inline void
OSUsbDevice::Usb_ack_sof(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(SOFI);
#else
  AVR32_usb_ack_sof();
#endif
}

// test if Suspend state detected
inline bool
OSUsbDevice::Is_usb_suspend(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(SUSPI)) != 0) ? true : false;
#else
  return AVR32_is_usb_suspend();
#endif
}

inline bool
OSUsbDevice::Is_suspend_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(SUSPE)) != 0) ? true : false;
#else
  return AVR32_is_usb_suspend_interrupt_enabled();
#endif
}

// ack Suspend
inline void
OSUsbDevice::Usb_ack_suspend(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(SUSPI);
#else
  AVR32_usb_ack_suspend();
#endif
}

// enable wake-up interrupt
inline void
OSUsbDevice::Usb_enable_wake_up_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN |= _BV(WAKEUPE);
#else
  AVR32_usb_enable_wake_up_interrupt();
#endif
}

// ack wake-up
inline void
OSUsbDevice::Usb_ack_wake_up(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(WAKEUPI);
#else
  AVR32_usb_ack_wake_up();
#endif
}

// Stop internal USB clock in interface(freeze the interface register)
inline void
OSUsbDevice::Usb_freeze_clock(void)
{
#if TO_BE_PORTED
  USBCON |= _BV(FRZCLK);
#else
  AVR32_usb_freeze_clock();
#endif
}

inline void
OSUsbDevice::Usb_unfreeze_clock(void)
{
#if TO_BE_PORTED
  USBCON &= ~_BV(FRZCLK);
#else
  AVR32_usb_unfreeze_clock();
#endif
}

// disable wake-up interrupt
inline void
OSUsbDevice::Usb_disable_wake_up_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN &= ~_BV(WAKEUPE);
#else
  AVR32_usb_disable_wake_up_interrupt();
#endif
}

// ack resume
inline void
OSUsbDevice::Usb_ack_resume(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(EORSMI);
#else
  AVR32_usb_ack_resume();
#endif
}

// disable resume interrupt
inline void
OSUsbDevice::Usb_disable_resume_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN &= ~_BV(EORSME);
#else
  AVR32_usb_disable_resume_interrupt();
#endif
}

// ack USB reset
inline void
OSUsbDevice::Usb_ack_reset(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(EORSTI);
#else
  AVR32_usb_ack_reset();
#endif
}

// test if wake-up occurs
inline bool
OSUsbDevice::Is_usb_wake_up(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(WAKEUPI)) != 0) ? true : false;
#else
  return AVR32_is_usb_wake_up();
#endif
}

inline bool
OSUsbDevice::Is_swake_up_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & (1<<WAKEUPE)) != 0) ? true : true;
#else
  return AVR32_is_usb_wake_up_interrupt_enabled();
#endif
}

// test if resume occurs
inline bool
OSUsbDevice::Is_usb_resume(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(EORSMI)) != 0) ? true : false;
#else
  return AVR32_is_usb_resume();
#endif
}

inline bool
OSUsbDevice::Is_resume_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(EORSME)) != 0) ? true : false;
#else
  return AVR32_is_usb_resume_interrupt_enabled();
#endif
}

// test if USB reset occurs
inline bool
OSUsbDevice::Is_usb_reset(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(EORSTI)) != 0) ? true : false;
#else
  return AVR32_is_usb_reset();
#endif
}

inline bool
OSUsbDevice::Is_reset_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(EORSTE)) != 0) ? true : false;
#else
  return AVR32_is_usb_reset_interrupt_enabled();
#endif
}

inline bool
OSUsbDevice::Is_usb_id_device(void)
{
#if TO_BE_PORTED
  return ((USBSTA & _BV(ID)) != 0) ? true : false;
#else
  return AVR32_is_usb_id_device();
#endif
}

// enable the current endpoint
inline void
OSUsbDevice::Usb_enable_endpoint(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(EPEN);
#else
  AVR32_usb_enable_endpoint(m_selectedEndpoint);
#endif
}

// allocate the current configuration in DPRAM memory
inline void
OSUsbDevice::Usb_allocate_memory(void)
{
#if TO_BE_PORTED
  UECFG1X |= _BV(ALLOC);
#else
  AVR32_usb_allocate_memory(m_selectedEndpoint);
#endif
}

// test if current endpoint is configured
inline bool
OSUsbDevice::Is_endpoint_configured(void)
{
#if TO_BE_PORTED
  return ((UESTA0X & _BV(CFGOK)) != 0) ? true : false;
#else
  return AVR32_is_usb_endpoint_configured(m_selectedEndpoint);
#endif
}

// enable suspend state interrupt
inline void
OSUsbDevice::Usb_enable_suspend_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN |= _BV(SUSPE);
#else
  AVR32_usb_enable_suspend_interrupt();
#endif
}

// These functions allow to control the PLL

#if TO_BE_PORTED

#if(OS_CFGLONG_OSCILLATOR_HZ==2000000L)
// Start the PLL in autofactor mode
// regarding FOSC define
#define PLL_FACTOR                  ((0<<PLLP2)|(0<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==4000000L)
#define PLL_FACTOR                  ((0<<PLLP2)|(0<<PLLP1)|(1<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==6000000L)
#define PLL_FACTOR                  ((0<<PLLP2)|(1<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==8000000L)
// Start the PLL in autofactor mode
// regarding FOSC define
#define PLL_FACTOR                  ((0<<PLLP2)|(1<<PLLP1)|(1<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==12000000L)
#define PLL_FACTOR                  ((1<<PLLP2)|(0<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==16000000L)
#define PLL_FACTOR                  ((1<<PLLP2)|(0<<PLLP1)|(1<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==20000000L)
#define PLL_FACTOR                ((1<<PLLP2)|(1<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==24000000L)
#define PLL_FACTOR                  ((1<<PLLP2)|(1<<PLLP1)|(1<<PLLP0))
#else
#error   "illegal OS_CFGLONG_OSCILLATOR_HZ"
#endif

#endif /* TO_BE_PORTED */

// @brief Start the PLL at only 48 MHz, regarding CPU frequency
// Start the USB PLL with clockfactor
// clockfactor can be PLLx24, PLLx12, PLLx08
// PLLx06, PLLx04, PLLx03
inline void
OSUsbDevice::Pll_start_auto(void)
{
#if TO_BE_PORTED
  PLLCSR = (PLL_FACTOR | _BV(PLLE));
#endif
}

inline bool
OSUsbDevice::Is_pll_ready(void)
{
#if TO_BE_PORTED
  return ((PLLCSR & _BV(PLOCK)) != 0) ? true : false;
#else
  return false;
#endif
}

#endif /* HAL_OSUSBDEVICE_INLINES_H_ */
