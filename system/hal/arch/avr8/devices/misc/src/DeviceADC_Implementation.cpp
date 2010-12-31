/*
 *      Copyright (C) 2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR) 
#if defined(OS_INCLUDE_DEVICEADC)

#include "portable/devices/misc/include/DeviceADC.h"

void DeviceADC::init(void)
  {
#if defined(DEBUG)
    OSDeviceDebug::putString_P(PSTR("DeviceADC::init()"));
    OSDeviceDebug::putNewLine();
#endif

    ADMUX = 0; // external AREF, right adjusted

    ADCSRA = 0x07; // divisor=128
  }

void DeviceADC::enable(void)
  {
    ADCSRA |= _BV(ADEN);
  }

void DeviceADC::disable(void)
  {
    ADCSRA &= ~_BV(ADEN);
  }

void DeviceADC::select(unsigned short channel)
  {
    ADMUX &= ~0x1F; // clear channel bits
    ADMUX |= (channel & 0x7); // select channel
  }

unsigned short DeviceADC::measure(void)
  {
#if defined(OS_INCLUDE_DEVICEADC_ISR)

    ADCSRA |= _BV(ADIE);

#if defined(OS_INCLUDE_DEVICEADC_SLEEP)
    for (;;)
      {
        ms_bCompleted = false;

        OSScheduler::ledActiveOff();
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_cpu();
        if (ms_bCompleted)
          {
            break;
          }

        OSDeviceDebug::putChar('@');

        if (false)
          {
            ADCSRA |= _BV(ADSC); // start conversion

            while (!ms_bCompleted)
              {
                ;
              }
          }
      }
#else
    ADCSRA |= _BV(ADSC); // start conversion

    while (!ms_bCompleted)
      {
        ;
      }
#endif

    ADCSRA &= ~_BV(ADIE);

#else

    ADCSRA |= _BV(ADSC); // start conversion

    // wait for conversion to complete
    while (ADCSRA & _BV(ADSC))
      {
        ; // busy wait, it is short anyway
      }
#endif

    return ADC;
  }

#if defined(OS_INCLUDE_DEVICEADC_ISR)

extern "C"
void ADC_vect( void ) __attribute__( ( signal ) );

void ADC_vect(void)
  {
    OSScheduler::ISRledActiveOn();
    DeviceADC::ms_bCompleted = true;
  }

namespace DeviceADC
  {
    bool ms_bCompleted;
  }

#endif

#endif /*OS_INCLUDE_DEVICEADC*/
#endif /*OS_CONFIG_ARCH_AVR*/

