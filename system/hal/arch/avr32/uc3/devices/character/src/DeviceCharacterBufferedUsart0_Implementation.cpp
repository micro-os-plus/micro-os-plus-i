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

#if defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_BAUD_CONSTANT)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT
#endif

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (8)
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (16)
#endif

#define CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     ((CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ / OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE)- 1UL)

#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART0_NAKED_TX_ISR)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)*/
void
Usart0_contextHandler(void);

// ----- inits ---------------------------------------------------------------

int
DeviceCharacterBufferedUsart0::implPortInit(void)
{
  volatile avr32_usart_t* port;
  port = &AVR32_USART0;

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

  INTC_register_interrupt(Usart0_contextHandler, AVR32_USART0_IRQ,
      OS_CFGINT_DEVICECHARACTERBUFFEREDUSART0_IRQ_PRIORITY);

  //enable the rx interrupt
  port->ier = AVR32_USART_IER_RXRDY_MASK;

  return 0;
}

// ===== ISRs =================================================================


void
Usart0_contextHandler(void)
{
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)
  OSScheduler::interruptEnter();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)*/
    {
      ((DeviceCharacterBufferedUsart0 *) DeviceCharacterBufferedUsart0::ms_pThis)->interruptServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)
  OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)*/
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
