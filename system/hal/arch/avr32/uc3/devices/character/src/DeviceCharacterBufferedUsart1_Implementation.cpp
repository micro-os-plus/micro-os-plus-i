/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsart1.h"

#include <avr32/io.h>
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/intc.h"
#include "hal/arch/avr32/uc3/lib/include/usart.h"
#include "hal/arch/avr32/uc3/lib/include/gpio.h"

// ----- inits ----------------------------------------------------------------

int
DeviceCharacterBufferedUsart1::implPortInit(void)
{
  setPortAddress(&AVR32_USART1);

  gpio_enable_module_pin(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_RX_PIN,
      OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_GPIO_FUNCTION);
  gpio_enable_module_pin(OS_CFGPIN_DEVICECHARACTERBUFFEREDUSART1_TX_PIN,
      OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_GPIO_FUNCTION);

  usart_options_t usartConfig;

#if defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE)
  if(m_baudRate == 0)
    m_baudRate = OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE;
  usartConfig.baudrate =m_baudRate;
#else
  usartConfig.baudrate = OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE;
#endif  /* defined(OS_INCLUDE_OSDEVICECHARACTER_SETBAUDRATE) */

  usartConfig.channelmode = USART_NORMAL_CHMODE;
  usartConfig.charlength = 8;
  usartConfig.paritytype = USART_NO_PARITY;
  usartConfig.stopbits = USART_1_STOPBIT;

  usart_init_rs232(&AVR32_USART1, &usartConfig, OS_CFGLONG_PBA_FREQUENCY_HZ);

  INTC_register_interrupt(DeviceCharacterBufferedUsart1::contextHandler,
      AVR32_USART1_IRQ, OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_IRQ_PRIORITY);

  // Enable the TX interrupt
  AVR32_USART1.ier = AVR32_USART_IER_RXRDY_MASK;

  // Do not enable TX interrupt

  return 0;
}

// ===== ISRs =================================================================

void
DeviceCharacterBufferedUsart1::contextHandler(void)
{
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_ISR_PREEMPTION)
  OSScheduler::interruptEnter();
#else
  OSScheduler::ISR_ledActiveOn();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_ISR_PREEMPTION)*/
    {
      ms_pThis->interruptServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_ISR_PREEMPTION)
  OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_ISR_PREEMPTION)*/
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
