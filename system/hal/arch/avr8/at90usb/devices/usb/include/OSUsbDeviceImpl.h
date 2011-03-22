/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICEIMPL_H_
#define HAL_OSUSBDEVICEIMPL_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AT90USB)

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/devices/usb/include/OSUsbLed.h"

#include "portable/devices/usb/include/OSUsbDevice_Defines.h"
#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDevice_Defines.h"

#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDeviceDescriptors.h"

// ----------------------------------------------------------------------------

class OSUsbDeviceImpl
{
public:
  OSUsbDeviceImpl();

  // interrupt service routines
  static void
  interruptComServiceRoutine(void);
  static void
  interruptGenServiceRoutine(void);

  // read/write methods
  static unsigned char
  readByte(void);
  static void
  writeByte(unsigned char b);

  static void
  standardProcessRequest(void);
  static bool
  specificProcessReadRequest(unsigned char type, unsigned char request);
  static bool
  specificGetDescriptor(unsigned char type, unsigned char string);

  static void
  specificEndpointInit(unsigned char conf_nb);

  static bool
  usb_configure_endpoint(unsigned char num, unsigned char type,
      unsigned char dir, unsigned char size, unsigned char bank,
      unsigned char nyet);

  static bool
  usb_config_ep(unsigned char config0, unsigned char config1);

  static void
  usb_get_descriptor(void);
  static void
  usb_get_configuration(void);
  static void
  usb_set_address(void);
  static void
  usb_set_configuration(void);
  static void
  usb_clear_feature(void);
  static void
  usb_set_feature(void);
  static void
  usb_get_status(void);
  static void
  usb_get_interface(void);
  static void
  usb_set_interface(void);
  static void
  usb_enum_var_init(void);
  static void
  usb_set_return(unsigned char len, PGM_VOID_P pbuf);

  static unsigned char
  usb_init_device(void);
  static void
  usb_start_device(void);

  // implementation functions
  static void
  endpointSelect(unsigned char ep);
  static void
  endpointReset(unsigned char ep);

  static bool
  isInterruptReceiveSetup(void);
  static bool
  isInterruptReceiveSetupEnabled(void);
  static void
  interruptReceiveSetupEnable(void);

  static bool
  isInterruptReceiveOut(void);
  static bool
  isInterruptReceiveOutEnabled(void);
  static void
  interruptReceiveOutAck(void);
  static void
  interruptReceiveOutEnable(void);

  static void
  Usb_enable_stall_handshake(void);
  static void
  Usb_ack_receive_setup(void);
  static void
  Usb_enable_regulator(void);
  static void
  Usb_disable_uid_pin(void);
  static void
  Usb_force_device_mode(void);

  static void
  Usb_disable(void);
  static void
  Usb_enable(void);

  static void
  Usb_select_device(void);
  static void
  Usb_enable_vbus_interrupt(void);

  static bool
  Is_usb_write_enabled(void);
  static bool
  Is_usb_tx_ready(void);
  static void
  Usb_send_in(void);
  static unsigned short
  Usb_byte_counter(void);

  static void
  Usb_ack_fifocon(void);

  static bool
  Is_usb_vbus_transition(void);
  static bool
  Is_usb_vbus_interrupt_enabled(void);
  static void
  Usb_ack_vbus_transition(void);
  static bool
  Is_usb_vbus_high(void);

  static void
  Usb_enable_reset_interrupt(void);

  static bool
  Is_usb_read_control_enabled(void);
  static void
  Usb_send_control_in(void);
  static void
  Usb_ack_receive_out(void);
  static void
  Usb_ack_in_ready(void);
  static bool
  Is_usb_in_ready(void);
  static bool
  Is_usb_endpoint_enabled(void);

  static void
  Usb_disable_stall_handshake(void);
  static void
  Usb_reset_data_toggle(void);
  static void
  Usb_enable_address(void);
  static void
  Usb_configure_address(unsigned char addr);

  static void
  Usb_set_configuration_action(void);
  static void
  Usb_sof_action(void);
  static void
  Usb_wake_up_action(void);
  static void
  Usb_resume_action(void);
  static void
  Usb_suspend_action(void);
  static void
  Usb_reset_action(void);
  static void
  Usb_vbus_on_action(void);
  static void
  Usb_vbus_off_action(void);

  static void
  Usb_send_event(unsigned char event);

  static void
  Usb_attach(void);
  static bool
  Is_usb_sof(void);
  static bool
  Is_sof_interrupt_enabled(void);
  static void
  Usb_ack_sof(void);

  static bool
  Is_usb_suspend(void);
  static bool
  Is_suspend_interrupt_enabled(void);

  static void
  Usb_ack_suspend(void);

  static void
  Usb_enable_wake_up_interrupt(void);
  static void
  Usb_ack_wake_up(void);

  static void
  Usb_freeze_clock(void);
  static void
  Usb_unfreeze_clock(void);

  static void
  Usb_disable_wake_up_interrupt(void);
  static void
  Usb_ack_resume(void);

  static void
  Usb_disable_resume_interrupt(void);
  static void
  Usb_ack_reset(void);

  static bool
  Is_usb_wake_up(void);
  static bool
  Is_swake_up_interrupt_enabled(void);

  static bool
  Is_usb_resume(void);
  static bool
  Is_resume_interrupt_enabled(void);

  static bool
  Is_usb_reset(void);
  static bool
  Is_reset_interrupt_enabled(void);

  static bool
  Is_usb_id_device(void);

  static void
  Usb_enable_endpoint(void);
  static void
  Usb_allocate_memory(void);
  static bool
  Is_endpoint_configured(void);

  static void
  Usb_enable_suspend_interrupt(void);
  static void
  Pll_start_auto(void);
  static bool
  Is_pll_ready(void);

  // members
  static unsigned char data_to_transfer;
  static PGM_VOID_P pbuffer;
  static bool zlp;
  static unsigned char bmRequestType;
  static volatile unsigned short g_usb_event;
  static unsigned char endpoint_status[NB_ENDPOINTS];
  static bool usb_connected;
  static unsigned char usb_configuration_nb;

};

// ----------------------------------------------------------------------------

inline void
OSUsbDeviceImpl::Usb_set_configuration_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_sof_action(void)
{
  ; //sof_action();
}

inline void
OSUsbDeviceImpl::Usb_wake_up_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_resume_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_suspend_action(void)
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

inline void
OSUsbDeviceImpl::Usb_reset_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_vbus_on_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_vbus_off_action(void)
{
  ;
}

// ----------------------------------------------------------------------------

// select the endpoint number to interface with the CPU
inline void
OSUsbDeviceImpl::endpointSelect(unsigned char ep)
{
  UENUM = ep;
}

// reset the selected endpoint
inline void
OSUsbDeviceImpl::endpointReset(unsigned char ep)
{
  UERST = _BV(ep);
  UERST = 0;
}

// test if SETUP interrupt received
inline bool
OSUsbDeviceImpl::isInterruptReceiveSetup(void)
{
  return ((UEINTX & _BV(RXSTPI)) != 0) ? true : false;
}

// test if SETUP interrupt enabled
inline bool
OSUsbDeviceImpl::isInterruptReceiveSetupEnabled(void)
{
  return ((UEIENX & _BV(RXSTPE)) != 0) ? true : false;
}

// enable receive SETUP interrupt
inline void
OSUsbDeviceImpl::interruptReceiveSetupEnable(void)
{
  UEIENX |= _BV(RXSTPE);
}

// test if receive OUT interrupt received
inline bool
OSUsbDeviceImpl::isInterruptReceiveOut(void)
{
  return (((UEINTX & _BV(RXOUTI))) != 0) ? true : false;
}

// test if receive OUT interrupt enabled
inline bool
OSUsbDeviceImpl::isInterruptReceiveOutEnabled(void)
{
  return ((UEIENX & _BV(RXOUTE)) != 0) ? true : false;
}

// ack receive OUT interrupt on control endpoint
inline void
OSUsbDeviceImpl::interruptReceiveOutAck(void)
{
  UEINTX &= ~_BV(RXOUTI);
}

// enable receive OUT interrupt
inline void
OSUsbDeviceImpl::interruptReceiveOutEnable(void)
{
  UEIENX |= _BV(RXOUTE);
}

// return FIFO byte for current endpoint
inline unsigned char
OSUsbDeviceImpl::readByte(void)
{
  return UEDATX;
}

// write byte in FIFO for current endpoint
inline void
OSUsbDeviceImpl::writeByte(unsigned char b)
{
  UEDATX = b;
}

// enables the STALL handshake for the next transaction
inline void
OSUsbDeviceImpl::Usb_enable_stall_handshake(void)
{
  UECONX |= _BV(STALLRQ);
}

// acks receive SETUP interrupt
inline void
OSUsbDeviceImpl::Usb_ack_receive_setup(void)
{
  UEINTX &= ~_BV(RXSTPI);
}

// Enable internal USB pads regulator
inline void
OSUsbDeviceImpl::Usb_enable_regulator(void)
{
  UHWCON |= _BV(UVREGE);
}

// Disable external UID pin
inline void
OSUsbDeviceImpl::Usb_disable_uid_pin(void)
{
  UHWCON &= ~_BV(UIDE);
}

// Disable external UID pin and force device mode
inline void
OSUsbDeviceImpl::Usb_force_device_mode(void)
{
  OSUsbDeviceImpl::Usb_disable_uid_pin();
  UHWCON |= _BV(UIMOD);
}

// Disable both USB interface and Vbus pad
inline void
OSUsbDeviceImpl::Usb_disable(void)
{
  USBCON &= ~(_BV(USBE) | _BV(OTGPADE));
}

// Enable both USB interface and Vbus pad
inline void
OSUsbDeviceImpl::Usb_enable(void)
{
  USBCON |= (_BV(USBE) | _BV(OTGPADE));
}

// select device mode
inline void
OSUsbDeviceImpl::Usb_select_device(void)
{
  USBCON &= ~_BV(HOST);
}

// enable VBUS interrupt
inline void
OSUsbDeviceImpl::Usb_enable_vbus_interrupt(void)
{
  USBCON |= _BV(VBUSTE);
}

// test if write endpoint enabled
inline bool
OSUsbDeviceImpl::Is_usb_write_enabled(void)
{
  return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
}

// test if transmit ready (same bit as allowed!)
inline bool
OSUsbDeviceImpl::Is_usb_tx_ready(void)
{
  return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
}

// send IN
inline void
OSUsbDeviceImpl::Usb_send_in(void)
{
  UEINTX &= ~_BV(FIFOCON);
}

// return number of bytes in FIFO current endpoint (16 bits)
inline unsigned short
OSUsbDeviceImpl::Usb_byte_counter(void)
{
  return UEBCX;
}

// clear FIFOCON bit
inline void
OSUsbDeviceImpl::Usb_ack_fifocon(void)
{
  UEINTX &= ~_BV(FIFOCON);
}

inline bool
OSUsbDeviceImpl::Is_usb_vbus_transition(void)
{
  return ((USBINT & (1 << VBUSTI)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_usb_vbus_interrupt_enabled(void)
{
  return ((USBCON & (1 << VBUSTE)) != 0) ? true : false;
}

inline void
OSUsbDeviceImpl::Usb_ack_vbus_transition(void)
{
  USBINT = ~_BV(VBUSTI);
}

inline bool
OSUsbDeviceImpl::Is_usb_vbus_high(void)
{
  return ((USBSTA & (1 << VBUS)) != 0) ? true : false;
}

// enable USB reset interrupt
inline void
OSUsbDeviceImpl::Usb_enable_reset_interrupt(void)
{
  UDIEN |= _BV(EORSTE);
}

// test if read allowed on control endpoint
inline bool
OSUsbDeviceImpl::Is_usb_read_control_enabled(void)
{
  return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
}

// send IN on control endpoint
inline void
OSUsbDeviceImpl::Usb_send_control_in(void)
{
  UEINTX &= ~_BV(TXINI);
}

// ack reveive OUT
inline void
OSUsbDeviceImpl::Usb_ack_receive_out(void)
{
  UEINTX &= ~_BV(RXOUTI);
  OSUsbDeviceImpl::Usb_ack_fifocon();
}

// ack IN ready
inline void
OSUsbDeviceImpl::Usb_ack_in_ready(void)
{
  UEINTX &= ~_BV(TXINI);
  OSUsbDeviceImpl::Usb_ack_fifocon();
}

// test if IN ready
inline bool
OSUsbDeviceImpl::Is_usb_in_ready(void)
{
  return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
}

// test if the current endpoint is enabled
inline bool
OSUsbDeviceImpl::Is_usb_endpoint_enabled(void)
{
  return ((UECONX & _BV(EPEN)) != 0) ? true : false;
}

// disable the STALL handshake
inline void
OSUsbDeviceImpl::Usb_disable_stall_handshake(void)
{
  UECONX |= _BV(STALLRQC);
}

// reset the data toggle sequence
inline void
OSUsbDeviceImpl::Usb_reset_data_toggle(void)
{
  UECONX |= _BV(RSTDT);
}

// enable USB device address
inline void
OSUsbDeviceImpl::Usb_enable_address(void)
{
  UDADDR |= _BV(ADDEN);
}

// set the USB device address
inline void
OSUsbDeviceImpl::Usb_configure_address(unsigned char addr)
{
  UDADDR = (UDADDR & _BV(ADDEN)) | (addr & MSK_UADD);
}

inline void
OSUsbDeviceImpl::Usb_send_event(unsigned char event)
{
  g_usb_event |= (1 << event);
}

// attache to USB bus
inline void
OSUsbDeviceImpl::Usb_attach(void)
{
  UDCON &= ~_BV(DETACH);
}

// test if Start Of Frame occurs
inline bool
OSUsbDeviceImpl::Is_usb_sof(void)
{
  return ((UDINT & _BV(SOFI)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_sof_interrupt_enabled(void)
{
  return ((UDIEN & _BV(SOFE)) != 0) ? true : false;
}

// ack Start Of Frame
inline void
OSUsbDeviceImpl::Usb_ack_sof(void)
{
  UDINT = ~_BV(SOFI);
}

// test if Suspend state detected
inline bool
OSUsbDeviceImpl::Is_usb_suspend(void)
{
  return ((UDINT & _BV(SUSPI)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_suspend_interrupt_enabled(void)
{
  return ((UDIEN & _BV(SUSPE)) != 0) ? true : false;
}

// ack Suspend
inline void
OSUsbDeviceImpl::Usb_ack_suspend(void)
{
  UDINT = ~_BV(SUSPI);
}

// enable wake-up interrupt
inline void
OSUsbDeviceImpl::Usb_enable_wake_up_interrupt(void)
{
  UDIEN |= _BV(WAKEUPE);
}

// ack wake-up
inline void
OSUsbDeviceImpl::Usb_ack_wake_up(void)
{
  UDINT = ~_BV(WAKEUPI);
}

// Stop internal USB clock in interface(freeze the interface register)
inline void
OSUsbDeviceImpl::Usb_freeze_clock(void)
{
  USBCON |= _BV(FRZCLK);
}

inline void
OSUsbDeviceImpl::Usb_unfreeze_clock(void)
{
  USBCON &= ~_BV(FRZCLK);
}

// disable wake-up interrupt
inline void
OSUsbDeviceImpl::Usb_disable_wake_up_interrupt(void)
{
  UDIEN &= ~_BV(WAKEUPE);
}

// ack resume
inline void
OSUsbDeviceImpl::Usb_ack_resume(void)
{
  UDINT = ~_BV(EORSMI);
}

// disable resume interrupt
inline void
OSUsbDeviceImpl::Usb_disable_resume_interrupt(void)
{
  UDIEN &= ~_BV(EORSME);
}

// ack USB reset
inline void
OSUsbDeviceImpl::Usb_ack_reset(void)
{
  UDINT = ~_BV(EORSTI);
}

// test if wake-up occurs
inline bool
OSUsbDeviceImpl::Is_usb_wake_up(void)
{
  return ((UDINT & _BV(WAKEUPI)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_swake_up_interrupt_enabled(void)
{
  return ((UDIEN & (1 << WAKEUPE)) != 0) ? true : true;
}

// test if resume occurs
inline bool
OSUsbDeviceImpl::Is_usb_resume(void)
{
  return ((UDINT & _BV(EORSMI)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_resume_interrupt_enabled(void)
{
  return ((UDIEN & _BV(EORSME)) != 0) ? true : false;
}

// test if USB reset occurs
inline bool
OSUsbDeviceImpl::Is_usb_reset(void)
{
  return ((UDINT & _BV(EORSTI)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_reset_interrupt_enabled(void)
{
  return ((UDIEN & _BV(EORSTE)) != 0) ? true : false;
}

inline bool
OSUsbDeviceImpl::Is_usb_id_device(void)
{
  return ((USBSTA & _BV(ID)) != 0) ? true : false;
}

// enable the current endpoint
inline void
OSUsbDeviceImpl::Usb_enable_endpoint(void)
{
  UECONX |= _BV(EPEN);
}

// allocate the current configuration in DPRAM memory
inline void
OSUsbDeviceImpl::Usb_allocate_memory(void)
{
  UECFG1X |= _BV(ALLOC);
}

// test if current endpoint is configured
inline bool
OSUsbDeviceImpl::Is_endpoint_configured(void)
{
  return ((UESTA0X & _BV(CFGOK)) != 0) ? true : false;
}

// enable suspend state interrupt
inline void
OSUsbDeviceImpl::Usb_enable_suspend_interrupt(void)
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
inline void
OSUsbDeviceImpl::Pll_start_auto(void)
{
  PLLCSR = (PLL_FACTOR | _BV(PLLE));
}

inline bool
OSUsbDeviceImpl::Is_pll_ready(void)
{
  return ((PLLCSR & _BV(PLOCK)) != 0) ? true : false;
}

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

#endif /* HAL_OSUSBDEVICEIMPL_H_ */

