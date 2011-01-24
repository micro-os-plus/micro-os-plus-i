/*
 *	Copyright (C) 2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_
#define HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_

// ---------------------------------------------------------------------------

// emulate i2c master by bitbanging

inline bool OSDeviceDebug::implCanTransmit(void)
  {
    return true;
  }

inline bool OSDeviceDebug::implDevicePresent(void)
  {
    return !masterSdaIsLow();
  }

inline void OSDeviceDebug::implWDReset(void)
  {
    IWDG_ReloadCounter();
  }

// ---

inline void masterInit(void)
  {
    // enable clock for APB2 where GPIOC is located
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

#if false
    // code to blink the debug leds before testing I2C debug
    OS_CONFIG_DEBUG_SCL_PORT->CRH = (OS_CONFIG_DEBUG_SCL_PORT->CRH
        & ~_CBV(12, 0x3, 0x3)) | _CBV(12, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);
    OS_CONFIG_DEBUG_SCL_PORT->CRH = (OS_CONFIG_DEBUG_SCL_PORT->CRH
        & ~_CBV(10, 0x3, 0x3)) | _CBV(10, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

    int i;
    i = 0;
    while (i < 10)
      {
        if ((i & 1) != 0)
          GPIOC->BRR |= _BV(12);
        else
          GPIOC->BSRR |= _BV(12);

        if ((i & 2) != 0)
          GPIOC->BRR |= _BV(10);
        else
          GPIOC->BSRR |= _BV(10);

        int delay;
        delay = 500000;
        while (delay)
          {
            asm volatile( "nop":: );
            delay--;
          }

        i++;
      }
#endif
  }

inline void masterSclInit(void)
  {
    OS_GPIO_PIN_HIGH(OS_CONFIG_DEBUG_SCL_PORT, OS_CONFIG_DEBUG_SCL_BIT);
    OS_GPIO_PIN_CONFIG(OS_CONFIG_DEBUG_SCL_PORT_CONFIG, OS_CONFIG_DEBUG_SCL_BIT, OS_CFV(GPIO_Mode_Out_OD, GPIO_Speed_50MHz));
  }

inline void masterSclHigh(void)
  {
    OS_GPIO_PIN_HIGH(OS_CONFIG_DEBUG_SCL_PORT, OS_CONFIG_DEBUG_SCL_BIT);
  }

inline void masterSclLow(void)
  {
    OS_GPIO_PIN_LOW(OS_CONFIG_DEBUG_SCL_PORT, OS_CONFIG_DEBUG_SCL_BIT);
  }

inline bool masterSclIsLow(void)
  {
    return ((OS_CONFIG_DEBUG_SCL_PORT->IDR & OS_S1B(OS_CONFIG_DEBUG_SCL_BIT)) == 0) ? true
        : false;
  }

inline void masterSdaInit(void)
  {
    OS_GPIO_PIN_HIGH(OS_CONFIG_DEBUG_SDA_PORT, OS_CONFIG_DEBUG_SDA_BIT);
    OS_GPIO_PIN_CONFIG(OS_CONFIG_DEBUG_SDA_PORT_CONFIG, OS_CONFIG_DEBUG_SDA_BIT, OS_CFV(GPIO_Mode_Out_OD, GPIO_Speed_50MHz));
  }

inline void masterSdaHigh(void)
  {
    OS_GPIO_PIN_HIGH(OS_CONFIG_DEBUG_SDA_PORT, OS_CONFIG_DEBUG_SDA_BIT);
  }

inline void masterSdaLow(void)
  {
    OS_GPIO_PIN_LOW(OS_CONFIG_DEBUG_SDA_PORT, OS_CONFIG_DEBUG_SDA_BIT);
  }

inline bool masterSdaIsLow(void)
  {
    return ((OS_CONFIG_DEBUG_SDA_PORT->IDR & OS_S1B(OS_CONFIG_DEBUG_SDA_BIT)) == 0) ? true
        : false;
  }

inline void masterSclSynchronize(void)
  {
    while (masterSclIsLow())
      {
        IWDG_ReloadCounter();
      }
  }

inline void masterSdaSynchronize(void)
  {
    while (masterSdaIsLow())
      {
        IWDG_ReloadCounter();
      }
  }
#endif /*HAL_DEVICEDEBUGI2CEMUMASTER_INLINES_H_*/
