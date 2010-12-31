/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBLED_H_
#define OSUSBLED_H_

class OSUsbLed
  {
public:
  OSUsbLed();

  static void toggle(void);
  static void on(void);
  static void off(void);
  static void init(void);

  unsigned char bitNumber(void);

private:
  };

inline OSUsbLed::OSUsbLed()
  {
  }

// TODO: move to hal
inline void OSUsbLed::init(void)
  {
#if defined(OS_CONFIG_USB_LED_PORT_INIT) && defined(OS_CONFIG_USB_LED_BIT)
    // init led port as output
    OS_CONFIG_USB_LED_PORT_INIT |= _BV(OS_CONFIG_USB_LED_BIT);
    // turn bit off
    OS_CONFIG_USB_LED_PORT &= ~_BV(OS_CONFIG_USB_LED_BIT);
#else
#warning "no OS_CONFIG_USB_LED_PORT_INIT defined"
#endif
  }

inline void OSUsbLed::toggle(void)
  {
#if defined(OS_CONFIG_USB_LED_PORT) && defined(OS_CONFIG_USB_LED_BIT)
    // toggle led
    OS_CONFIG_USB_LED_PORT ^= _BV(OS_CONFIG_USB_LED_BIT);
#endif
  }

inline void OSUsbLed::on(void)
  {
#if defined(OS_CONFIG_USB_LED_PORT) && defined(OS_CONFIG_USB_LED_BIT)
    // toggle led
    OS_CONFIG_USB_LED_PORT |= _BV(OS_CONFIG_USB_LED_BIT);
#endif
  }

inline void OSUsbLed::off(void)
  {
#if defined(OS_CONFIG_USB_LED_PORT) && defined(OS_CONFIG_USB_LED_BIT)
    // toggle led
    OS_CONFIG_USB_LED_PORT &= ~_BV(OS_CONFIG_USB_LED_BIT);
#endif
  }

inline unsigned char OSUsbLed::bitNumber(void)
  {
#if defined(OS_CONFIG_USB_PORT_INIT) && defined(OS_CONFIG_USB_LED_BIT)
    return OS_CONFIG_USB_LED_BIT;
#else
    return 0;
#endif
  }

#endif /*OSUSBLED_H_*/
