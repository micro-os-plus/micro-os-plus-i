/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICE_INLINES_H_
#define HAL_OSUSBDEVICE_INLINES_H_

//----------------------------------------------------------------------------
inline void OSUsbDevice::Usb_set_configuration_action(void)
  {
    ;
  }

inline void OSUsbDevice::Usb_sof_action(void)
  {
    ; //sof_action();
  }

inline void OSUsbDevice::Usb_wake_up_action(void)
  {
    ;
  }

inline void OSUsbDevice::Usb_resume_action(void)
  {
    ;
  }

inline void OSUsbDevice::Usb_suspend_action(void)
  {
    OSDeviceDebug::putString("zzz...");
    OSDeviceDebug::putNewLine();

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
  }

inline void OSUsbDevice::Usb_reset_action(void)
  {
    ;
  }

inline void OSUsbDevice::Usb_vbus_on_action(void)
  {
    ;
  }

inline void OSUsbDevice::Usb_vbus_off_action(void)
  {
    ;
  }

//----------------------------------------------------------------------------


// select the endpoint number to interface with the CPU
inline void OSUsbDevice::endpointSelect(unsigned char ep)
  {
    UENUM = ep;
  }

// reset the selected endpoint
inline void OSUsbDevice::endpointReset(unsigned char ep)
  {
    UERST = _BV(ep);
    UERST = 0;
  }

// test if SETUP interrupt received
inline bool OSUsbDevice::isInterruptReceiveSetup(void)
  {
    return ((UEINTX & _BV(RXSTPI)) != 0) ? true : false;
  }

// test if SETUP interrupt enabled
inline bool OSUsbDevice::isInterruptReceiveSetupEnabled(void)
  {
    return ((UEIENX & _BV(RXSTPE)) != 0) ? true : false;
  }

// enable receive SETUP interrupt
inline void OSUsbDevice::interruptReceiveSetupEnable(void)
  {
    UEIENX |= _BV(RXSTPE);
  }

// test if receive OUT interrupt received
inline bool OSUsbDevice::isInterruptReceiveOut(void)
  {
    return (((UEINTX & _BV(RXOUTI))) != 0) ? true : false;
  }

// test if receive OUT interrupt enabled
inline bool OSUsbDevice::isInterruptReceiveOutEnabled(void)
  {
    return ((UEIENX & _BV(RXOUTE)) != 0) ? true : false;
  }

// ack receive OUT interrupt on control endpoint
inline void OSUsbDevice::interruptReceiveOutAck(void)
  {
    UEINTX &= ~_BV(RXOUTI);
  }

// enable receive OUT interrupt
inline void OSUsbDevice::interruptReceiveOutEnable(void)
  {
    UEIENX |= _BV(RXOUTE);
  }

// return FIFO byte for current endpoint
inline unsigned char OSUsbDevice::readByte(void)
  {
    return UEDATX;
  }

// write byte in FIFO for current endpoint
inline void OSUsbDevice::writeByte(unsigned char b)
  {
    UEDATX = b;
  }

// enables the STALL handshake for the next transaction
inline void OSUsbDevice::Usb_enable_stall_handshake(void)
  {
    UECONX |= _BV(STALLRQ);
  }

// acks receive SETUP interrupt
inline void OSUsbDevice::Usb_ack_receive_setup(void)
  {
    UEINTX &= ~_BV(RXSTPI);
  }

// Enable internal USB pads regulator
inline void OSUsbDevice::Usb_enable_regulator(void)
  {
    UHWCON |=_BV(UVREGE);
  }

// Disable external UID pin
inline void OSUsbDevice::Usb_disable_uid_pin(void)
  {
    UHWCON &= ~_BV(UIDE);
  }

// Disable external UID pin and force device mode
inline void OSUsbDevice::Usb_force_device_mode(void)
  {
    OSUsbDevice::Usb_disable_uid_pin();
    UHWCON |= _BV(UIMOD);
  }

// Disable both USB interface and Vbus pad
inline void OSUsbDevice::Usb_disable(void)
  {
    USBCON &= ~(_BV(USBE) | _BV(OTGPADE));
  }

// Enable both USB interface and Vbus pad
inline void OSUsbDevice::Usb_enable(void)
  {
    USBCON |=(_BV(USBE) |_BV(OTGPADE));
  }

// select device mode
inline void OSUsbDevice::Usb_select_device(void)
  {
    USBCON &= ~_BV(HOST);
  }

// enable VBUS interrupt
inline void OSUsbDevice::Usb_enable_vbus_interrupt(void)
  {
    USBCON |= _BV(VBUSTE);
  }

// test if write endpoint enabled
inline bool OSUsbDevice::Is_usb_write_enabled(void)
  {
    return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
  }

// test if transmit ready (same bit as allowed!)
inline bool OSUsbDevice::Is_usb_tx_ready(void)
  {
    return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
  }

// send IN
inline void OSUsbDevice::Usb_send_in(void)
  {
    UEINTX &= ~_BV(FIFOCON);
  }

// return number of bytes in FIFO current endpoint (16 bits)
inline unsigned short OSUsbDevice::Usb_byte_counter(void)
  {
    return UEBCX;
  }

// clear FIFOCON bit
inline void OSUsbDevice::Usb_ack_fifocon(void)
  {
    UEINTX &= ~_BV(FIFOCON);
  }

inline bool OSUsbDevice::Is_usb_vbus_transition(void)
  {
    return ((USBINT & (1<<VBUSTI)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_usb_vbus_interrupt_enabled(void)
  {
    return ((USBCON & (1<<VBUSTE)) != 0) ? true : false;
  }

inline void OSUsbDevice::Usb_ack_vbus_transition(void)
  {
    USBINT = ~_BV(VBUSTI);
  }

inline bool OSUsbDevice::Is_usb_vbus_high(void)
  {
    return ((USBSTA & (1<<VBUS)) != 0) ? true : false;
  }

// enable USB reset interrupt
inline void OSUsbDevice::Usb_enable_reset_interrupt(void)
  {
    UDIEN |= _BV(EORSTE);
  }

// test if read allowed on control endpoint
inline bool OSUsbDevice::Is_usb_read_control_enabled(void)
  {
    return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
  }

// send IN on control endpoint
inline void OSUsbDevice::Usb_send_control_in(void)
  {
    UEINTX &= ~_BV(TXINI);
  }

// ack reveive OUT
inline void OSUsbDevice::Usb_ack_receive_out(void)
  {
    UEINTX &= ~_BV(RXOUTI);
    OSUsbDevice::Usb_ack_fifocon();
  }

// ack IN ready
inline void OSUsbDevice::Usb_ack_in_ready(void)
  {
    UEINTX &= ~_BV(TXINI);
    OSUsbDevice::Usb_ack_fifocon();
  }

// test if IN ready
inline bool OSUsbDevice::Is_usb_in_ready(void)
  {
    return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
  }

// test if the current endpoint is enabled
inline bool OSUsbDevice::Is_usb_endpoint_enabled(void)
  {
    return ((UECONX & _BV(EPEN)) != 0) ? true : false;
  }

// disable the STALL handshake
inline void OSUsbDevice::Usb_disable_stall_handshake(void)
  {
    UECONX |= _BV(STALLRQC);
  }

// reset the data toggle sequence
inline void OSUsbDevice::Usb_reset_data_toggle(void)
  {
    UECONX |= _BV(RSTDT);
  }

// enable USB device address
inline void OSUsbDevice::Usb_enable_address(void)
  {
    UDADDR |= _BV(ADDEN);
  }

// set the USB device address
inline void OSUsbDevice::Usb_configure_address(unsigned char addr)
  {
    UDADDR = (UDADDR & _BV(ADDEN)) |(addr & MSK_UADD);
  }

inline void OSUsbDevice::Usb_send_event(unsigned char event)
  {
    g_usb_event |= (1<<event);
  }

// attache to USB bus
inline void OSUsbDevice::Usb_attach(void)
  {
    UDCON &= ~_BV(DETACH);
  }

// test if Start Of Frame occurs
inline bool OSUsbDevice::Is_usb_sof(void)
  {
    return ((UDINT & _BV(SOFI)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_sof_interrupt_enabled(void)
  {
    return ((UDIEN & _BV(SOFE)) != 0) ? true : false;
  }

// ack Start Of Frame
inline void OSUsbDevice::Usb_ack_sof(void)
  {
    UDINT = ~_BV(SOFI);
  }

// test if Suspend state detected
inline bool OSUsbDevice::Is_usb_suspend(void)
  {
    return ((UDINT & _BV(SUSPI)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_suspend_interrupt_enabled(void)
  {
    return ((UDIEN & _BV(SUSPE)) != 0) ? true : false;
  }

// ack Suspend
inline void OSUsbDevice::Usb_ack_suspend(void)
  {
    UDINT = ~_BV(SUSPI);
  }

// enable wake-up interrupt
inline void OSUsbDevice::Usb_enable_wake_up_interrupt(void)
  {
    UDIEN |= _BV(WAKEUPE);
  }

// ack wake-up
inline void OSUsbDevice::Usb_ack_wake_up(void)
  {
    UDINT = ~_BV(WAKEUPI);
  }

// Stop internal USB clock in interface(freeze the interface register)
inline void OSUsbDevice::Usb_freeze_clock(void)
  {
    USBCON |= _BV(FRZCLK);
  }

inline void OSUsbDevice::Usb_unfreeze_clock(void)
  {
    USBCON &= ~_BV(FRZCLK);
  }

// disable wake-up interrupt
inline void OSUsbDevice::Usb_disable_wake_up_interrupt(void)
  {
    UDIEN &= ~_BV(WAKEUPE);
  }

// ack resume
inline void OSUsbDevice::Usb_ack_resume(void)
  {
    UDINT = ~_BV(EORSMI);
  }

// disable resume interrupt
inline void OSUsbDevice::Usb_disable_resume_interrupt(void)
  {
    UDIEN &= ~_BV(EORSME);
  }

// ack USB reset
inline void OSUsbDevice::Usb_ack_reset(void)
  {
    UDINT = ~_BV(EORSTI);
  }

// test if wake-up occurs
inline bool OSUsbDevice::Is_usb_wake_up(void)
  {
    return ((UDINT & _BV(WAKEUPI)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_swake_up_interrupt_enabled(void)
  {
    return ((UDIEN & (1<<WAKEUPE)) != 0) ? true : true;
  }

// test if resume occurs
inline bool OSUsbDevice::Is_usb_resume(void)
  {
    return ((UDINT & _BV(EORSMI)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_resume_interrupt_enabled(void)
  {
    return ((UDIEN & _BV(EORSME)) != 0) ? true : false;
  }

// test if USB reset occurs
inline bool OSUsbDevice::Is_usb_reset(void)
  {
    return ((UDINT & _BV(EORSTI)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_reset_interrupt_enabled(void)
  {
    return ((UDIEN & _BV(EORSTE)) != 0) ? true : false;
  }

inline bool OSUsbDevice::Is_usb_id_device(void)
  {
    return ((USBSTA & _BV(ID)) != 0) ? true : false;
  }

// enable the current endpoint
inline void OSUsbDevice::Usb_enable_endpoint(void)
  {
    UECONX |= _BV(EPEN);
  }

// allocate the current configuration in DPRAM memory
inline void OSUsbDevice::Usb_allocate_memory(void)
  {
    UECFG1X |= _BV(ALLOC);
  }

// test if current endpoint is configured
inline bool OSUsbDevice::Is_endpoint_configured(void)
  {
    return ((UESTA0X & _BV(CFGOK)) != 0) ? true : false;
  }

// enable suspend state interrupt
inline void OSUsbDevice::Usb_enable_suspend_interrupt(void)
  {
    UDIEN |= _BV(SUSPE);
  }

// These functions allow to control the PLL

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

// @brief Start the PLL at only 48 MHz, regarding CPU frequency
// Start the USB PLL with clockfactor
// clockfactor can be PLLx24, PLLx12, PLLx08
// PLLx06, PLLx04, PLLx03
inline void OSUsbDevice::Pll_start_auto(void)
  {
    PLLCSR = (PLL_FACTOR | _BV(PLLE));
  }

inline bool OSUsbDevice::Is_pll_ready(void)
  {
    return ((PLLCSR & _BV(PLOCK)) != 0) ? true : false;
  }

#endif /*HAL_OSUSBDEVICE_INLINES_H_*/
