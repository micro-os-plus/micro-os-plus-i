/*
 *      Copyright (C) 2009-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_FAMILY_OS_DEFS_H_
#define HAL_FAMILY_OS_DEFS_H_

#include "hal/arch/arm_cortex_m3/stm32f10x/lib/include/stm32f10x.h"
#include "hal/arch/arm_cortex_m3/stm32f10x/lib/include/stm32f10x_defs.h"

#define OS_BIT8(_BIT)           (((_BIT) > 7) ? ((_BIT)-8) : (_BIT))

#define OS_S1B(_BIT)            (0x1<<(_BIT))

#define OS_SVB(_BIT,_VALUE)     (((_VALUE) & 0xF) << ((OS_BIT8(_BIT)) * 4))
#define OS_CFV(_MODE,_SPEED)    ((((_MODE) & 0x3) << 2) | ((_SPEED) & 0x03))

// Select CRL or CRH based on bit balue
#define OS_CFGP(_PORT, _BIT)    (((_BIT)<=7) ? ((_PORT)->CRL) : ((_PORT)->CRH))

// Clear the 4 bits and set new value
#define OS_GPIO_PIN_CONFIG(_PORT, _BIT, _VALUE) (OS_CFGP(_PORT, _BIT) = (OS_CFGP(_PORT, _BIT) & ~(OS_SVB(_BIT, OS_CFV(0x3, 0x3)))) | (OS_SVB(_BIT, _VALUE)))

// 0b0011 = general purpose output push-pull, output with max speed
#define OS_GPIO_PIN_CONFIG_OUTPUT(_PORT, _BIT) (OS_GPIO_PIN_CONFIG(_PORT, _BIT, 0x3))
// 0b0100 = floating input
#define OS_GPIO_PIN_CONFIG_INPUT(_PORT, _BIT) (OS_GPIO_PIN_CONFIG(_PORT, _BIT, 0x4))

// Enable sets the port in input mode
#define OS_GPIO_PIN_CONFIG_ENABLE(_PORT, _BIT) (OS_GPIO_PIN_CONFIG_INPUT(_PORT, _BIT))

// limit mask to 16 bits, since all ports are 16 bits
#define OS_S1W(_BIT)            (0x1<<((_BIT)&0xF))

#define OS_GPIO_PIN_HIGH(_PORT, _BIT) ((_PORT)->BSRR |= OS_S1W(_BIT))
#define OS_GPIO_PIN_LOW(_PORT, _BIT) ((_PORT)->BRR |= OS_S1W(_BIT))

#define OS_GPIO_PIN_TOGGLE(_PORT, _BIT) (((_PORT)->ODR) ^= OS_S1W(_BIT))

#define OS_GPIO_PIN_ISHIGH(_PORT, _BIT) (((((_PORT)->IDR) & OS_S1W(_BIT)) != 0) ? true : false)
#define OS_GPIO_PIN_ISLOW(_PORT, _BIT)  (((((_PORT)->IDR) & OS_S1W(_BIT)) == 0) ? true : false)

#define OS_CFGLONG_CPU_FREQUENCY_HZ    (OS_CFGLONG_OSCILLATOR_HZ/OS_CFGINT_CLOCK_PRESCALER)

/* External Interrupts */
void WWDG_IRQHandler(void) __attribute__ ((interrupt));
void PVD_IRQHandler(void) __attribute__ ((interrupt));
void TAMPER_IRQHandler(void) __attribute__ ((interrupt));
void RTC_IRQHandler(void) __attribute__ ((interrupt));
void FLASH_IRQHandler(void) __attribute__ ((interrupt));
void RCC_IRQHandler(void) __attribute__ ((interrupt));
void EXTI0_IRQHandler(void) __attribute__ ((interrupt));
void EXTI1_IRQHandler(void) __attribute__ ((interrupt));
void EXTI2_IRQHandler(void) __attribute__ ((interrupt));
void EXTI3_IRQHandler(void) __attribute__ ((interrupt));
void EXTI4_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel1_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel2_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel3_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel4_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel5_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel6_IRQHandler(void) __attribute__ ((interrupt));
void DMA1_Channel7_IRQHandler(void) __attribute__ ((interrupt));
void ADC1_2_IRQHandler(void) __attribute__ ((interrupt));
void USB_HP_CAN1_TX_IRQHandler(void) __attribute__ ((interrupt));
void USB_LP_CAN1_RX0_IRQHandler(void) __attribute__ ((interrupt));
void CAN1_RX1_IRQHandler(void) __attribute__ ((interrupt));
void CAN1_SCE_IRQHandler(void) __attribute__ ((interrupt));
void EXTI9_5_IRQHandler(void) __attribute__ ((interrupt));
void TIM1_BRK_IRQHandler(void) __attribute__ ((interrupt));
void TIM1_UP_IRQHandler(void) __attribute__ ((interrupt));
void TIM1_TRG_COM_IRQHandler(void) __attribute__ ((interrupt));
void TIM1_CC_IRQHandler(void) __attribute__ ((interrupt));
void TIM2_IRQHandler(void) __attribute__ ((interrupt));
void TIM3_IRQHandler(void) __attribute__ ((interrupt));
#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
void TIM4_IRQHandler(void) __attribute__ ((interrupt));
#endif
void I2C1_EV_IRQHandler(void) __attribute__ ((interrupt));
void I2C1_ER_IRQHandler(void) __attribute__ ((interrupt));
#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
void I2C2_EV_IRQHandler(void) __attribute__ ((interrupt));
void I2C2_ER_IRQHandler(void) __attribute__ ((interrupt));
#endif
void SPI1_IRQHandler(void) __attribute__ ((interrupt));
void SPI2_IRQHandler(void) __attribute__ ((interrupt));
void USART1_IRQHandler(void) __attribute__ ((interrupt));
void USART2_IRQHandler(void) __attribute__ ((interrupt));
#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
void USART3_IRQHandler(void) __attribute__ ((interrupt));
#endif
void EXTI15_10_IRQHandler(void) __attribute__ ((interrupt));
void RTCAlarm_IRQHandler(void) __attribute__ ((interrupt));
void USBWakeUp_IRQHandler(void) __attribute__ ((interrupt));
#if defined(STM32F10X_HD)

void TIM8_BRK_IRQHandler(void) __attribute__ ((interrupt));
void TIM8_UP_IRQHandler(void) __attribute__ ((interrupt));
void TIM8_TRG_COM_IRQHandler(void) __attribute__ ((interrupt));
void TIM8_CC_IRQHandler(void) __attribute__ ((interrupt));
void ADC3_IRQHandler(void) __attribute__ ((interrupt));
void FSMC_IRQHandler(void) __attribute__ ((interrupt));
void SDIO_IRQHandler(void) __attribute__ ((interrupt));
void TIM5_IRQHandler(void) __attribute__ ((interrupt));
void SPI3_IRQHandler(void) __attribute__ ((interrupt));
void UART4_IRQHandler(void) __attribute__ ((interrupt));
void UART5_IRQHandler(void) __attribute__ ((interrupt));
void TIM6_IRQHandler(void) __attribute__ ((interrupt));
void TIM7_IRQHandler(void) __attribute__ ((interrupt));
void DMA2_Channel1_IRQHandler(void) __attribute__ ((interrupt));
void DMA2_Channel2_IRQHandler(void) __attribute__ ((interrupt));
void DMA2_Channel3_IRQHandler(void) __attribute__ ((interrupt));
void DMA2_Channel4_5_IRQHandler(void) __attribute__ ((interrupt));
void SystemInit_ExtMemCtl(void) __attribute__ ((interrupt));
#endif

#if defined(STM32F10X_HD)
void SystemInit_ExtMemCtl_Dummy(void) __attribute__ ((interrupt));
#endif

extern "C" void Default_Handler(void) __attribute__((interrupt));

#endif /* HAL_FAMILY_OS_DEFS_H_ */
