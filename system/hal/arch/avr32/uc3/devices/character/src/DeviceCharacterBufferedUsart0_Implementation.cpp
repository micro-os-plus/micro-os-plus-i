/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0)

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsart0.h"

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/usart.h"
#include "hal/arch/avr32/uc3/lib/include/gpio.h"

// ----- inits ---------------------------------------------------------------

int
DeviceCharacterBufferedUsart0::implPortInit(void)
{
  setPortAddress(&AVR32_USART0);

  gpio_enable_module_pin(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART0_RX_PIN,
      OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_GPIO_FUNCTION);
  gpio_enable_module_pin(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART0_TX_PIN,
      OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_GPIO_FUNCTION);

  usart_options_t usartConfig;

  usartConfig.baudrate = OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_BAUD_RATE;
  usartConfig.channelmode = USART_NORMAL_CHMODE;
  usartConfig.charlength = 8;
  usartConfig.paritytype = USART_NO_PARITY;
  usartConfig.stopbits = USART_1_STOPBIT;

  usart_init_rs232(&AVR32_USART0, &usartConfig, OS_CFGLONG_PBA_FREQUENCY_HZ);

  INTC_register_interrupt(DeviceCharacterBufferedUsart0::contextHandler,
      AVR32_USART0_IRQ, OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_IRQ_PRIORITY);

  // Enable the RX interrupt
  AVR32_USART0.ier = AVR32_USART_IER_RXRDY_MASK;

  // Do not enable TX interrupts

  return 0;
}

// ===== ISRs =================================================================


void
DeviceCharacterBufferedUsart0::contextHandler(void)
{
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART0_ISR_PREEMPTION)
  OSScheduler::interruptEnter();
#else
  OSScheduler::ISR_ledActiveOn();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART0_ISR_PREEMPTION)*/
    {
      ms_pThis->interruptServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART0_ISR_PREEMPTION)
  OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART0_ISR_PREEMPTION)*/
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
