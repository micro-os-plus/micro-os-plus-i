/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_FAMILY_STM32F10X)

#if defined(STM32F10X_HD)
extern "C" void SystemInit_ExtMemCtl_Dummy(void) __attribute__ ((interrupt));
#endif

#pragma weak WWDG_IRQHandler = Default_Handler
#pragma weak PVD_IRQHandler = Default_Handler
#pragma weak TAMPER_IRQHandler = Default_Handler
#pragma weak RTC_IRQHandler = Default_Handler
#pragma weak FLASH_IRQHandler = Default_Handler
#pragma weak RCC_IRQHandler = Default_Handler
#pragma weak EXTI0_IRQHandler = Default_Handler
#pragma weak EXTI1_IRQHandler = Default_Handler
#pragma weak EXTI2_IRQHandler = Default_Handler
#pragma weak EXTI3_IRQHandler = Default_Handler
#pragma weak EXTI4_IRQHandler = Default_Handler
#pragma weak DMA1_Channel1_IRQHandler = Default_Handler
#pragma weak DMA1_Channel2_IRQHandler = Default_Handler
#pragma weak DMA1_Channel3_IRQHandler = Default_Handler
#pragma weak DMA1_Channel4_IRQHandler = Default_Handler
#pragma weak DMA1_Channel5_IRQHandler = Default_Handler
#pragma weak DMA1_Channel6_IRQHandler = Default_Handler
#pragma weak DMA1_Channel7_IRQHandler = Default_Handler
#pragma weak ADC1_2_IRQHandler = Default_Handler
#pragma weak USB_HP_CAN1_TX_IRQHandler = Default_Handler
#pragma weak USB_LP_CAN1_RX0_IRQHandler = Default_Handler
#pragma weak CAN1_RX1_IRQHandler = Default_Handler
#pragma weak CAN1_SCE_IRQHandler = Default_Handler
#pragma weak EXTI9_5_IRQHandler = Default_Handler
#pragma weak TIM1_BRK_IRQHandler = Default_Handler
#pragma weak TIM1_UP_IRQHandler = Default_Handler
#pragma weak TIM1_TRG_COM_IRQHandler = Default_Handler
#pragma weak TIM1_CC_IRQHandler = Default_Handler
#pragma weak TIM2_IRQHandler = Default_Handler
#pragma weak TIM3_IRQHandler = Default_Handler

#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
#pragma weak TIM4_IRQHandler = Default_Handler
#endif

#pragma weak I2C1_EV_IRQHandler = Default_Handler
#pragma weak I2C1_ER_IRQHandler = Default_Handler

#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
#pragma weak I2C2_EV_IRQHandler = Default_Handler
#pragma weak I2C2_ER_IRQHandler = Default_Handler
#endif

#pragma weak SPI1_IRQHandler = Default_Handler
#pragma weak SPI2_IRQHandler = Default_Handler
#pragma weak USART1_IRQHandler = Default_Handler
#pragma weak USART2_IRQHandler = Default_Handler

#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
#pragma weak USART3_IRQHandler = Default_Handler
#endif

#pragma weak EXTI15_10_IRQHandler = Default_Handler
#pragma weak RTCAlarm_IRQHandler = Default_Handler
#pragma weak USBWakeUp_IRQHandler = Default_Handler

#if defined(STM32F10X_HD)
#pragma weak TIM8_BRK_IRQHandler = Default_Handler
#pragma weak TIM8_UP_IRQHandler = Default_Handler
#pragma weak TIM8_TRG_COM_IRQHandler = Default_Handler
#pragma weak TIM8_CC_IRQHandler = Default_Handler
#pragma weak ADC3_IRQHandler = Default_Handler
#pragma weak FSMC_IRQHandler = Default_Handler
#pragma weak SDIO_IRQHandler = Default_Handler
#pragma weak TIM5_IRQHandler = Default_Handler
#pragma weak SPI3_IRQHandler = Default_Handler
#pragma weak UART4_IRQHandler = Default_Handler
#pragma weak UART5_IRQHandler = Default_Handler
#pragma weak TIM6_IRQHandler = Default_Handler
#pragma weak TIM7_IRQHandler = Default_Handler
#pragma weak DMA2_Channel1_IRQHandler = Default_Handler
#pragma weak DMA2_Channel2_IRQHandler = Default_Handler
#pragma weak DMA2_Channel3_IRQHandler = Default_Handler
#pragma weak DMA2_Channel4_5_IRQHandler = Default_Handler
#pragma weak SystemInit_ExtMemCtl = SystemInit_ExtMemCtl_Dummy
#endif

pfnVector_t g_pfnVectors[] __attribute__ ((section(".vectors.stm32f"))) =
  {
  /* External Interrupts */
  (pfnVector_t) WWDG_IRQHandler, /* Window Watchdog */
  (pfnVector_t) PVD_IRQHandler, /* PVD through EXTI Line detect */
  (pfnVector_t) TAMPER_IRQHandler, /* Tamper */
  (pfnVector_t) RTC_IRQHandler, /* RTC */
  (pfnVector_t) FLASH_IRQHandler, /* Flash */
  (pfnVector_t) RCC_IRQHandler, /* RCC */
  (pfnVector_t) EXTI0_IRQHandler, /* EXTI Line 0 */
  (pfnVector_t) EXTI1_IRQHandler, /* EXTI Line 1 */
  (pfnVector_t) EXTI2_IRQHandler, /* EXTI Line 2 */
  (pfnVector_t) EXTI3_IRQHandler, /* EXTI Line 3 */
  (pfnVector_t) EXTI4_IRQHandler, /* EXTI Line 4 */
  (pfnVector_t) DMA1_Channel1_IRQHandler, /* DMA1 Channel 1 */
  (pfnVector_t) DMA1_Channel2_IRQHandler, /* DMA1 Channel 2 */
  (pfnVector_t) DMA1_Channel3_IRQHandler, /* DMA1 Channel 3 */
  (pfnVector_t) DMA1_Channel4_IRQHandler, /* DMA1 Channel 4 */
  (pfnVector_t) DMA1_Channel5_IRQHandler, /* DMA1 Channel 5 */
  (pfnVector_t) DMA1_Channel6_IRQHandler, /* DMA1 Channel 6 */
  (pfnVector_t) DMA1_Channel7_IRQHandler, /* DMA1 Channel 7 */
  (pfnVector_t) ADC1_2_IRQHandler, /* ADC1 & ADC2 */
  (pfnVector_t) USB_HP_CAN1_TX_IRQHandler, /* USB High Priority or CAN1 TX */
  (pfnVector_t) USB_LP_CAN1_RX0_IRQHandler, /* USB Low  Priority or CAN1 RX0 */
  (pfnVector_t) CAN1_RX1_IRQHandler, /* CAN1 RX1 */
  (pfnVector_t) CAN1_SCE_IRQHandler, /* CAN1 SCE */
  (pfnVector_t) EXTI9_5_IRQHandler, /* EXTI Line 9..5 */
  (pfnVector_t) TIM1_BRK_IRQHandler, /* TIM1 Break */
  (pfnVector_t) TIM1_UP_IRQHandler, /* TIM1 Update */
  (pfnVector_t) TIM1_TRG_COM_IRQHandler, /* TIM1 Trigger and Commutation */
  (pfnVector_t) TIM1_CC_IRQHandler, /* TIM1 Capture Compare */
  (pfnVector_t) TIM2_IRQHandler, /* TIM2 */
  (pfnVector_t) TIM3_IRQHandler, /* TIM3 */

#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
      (pfnVector_t) TIM4_IRQHandler, /* TIM4 */
#else
      0,
#endif

      (pfnVector_t) I2C1_EV_IRQHandler, /* I2C1 Event */
      (pfnVector_t) I2C1_ER_IRQHandler, /* I2C1 Error */

#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
      (pfnVector_t) I2C2_EV_IRQHandler, /* I2C2 Event */
      (pfnVector_t) I2C2_ER_IRQHandler, /* I2C2 Error */
#else
      0,0,
#endif

      (pfnVector_t) SPI1_IRQHandler, /* SPI1 */
      (pfnVector_t) SPI2_IRQHandler, /* SPI2 */
      (pfnVector_t) USART1_IRQHandler, /* USART1 */
      (pfnVector_t) USART2_IRQHandler, /* USART2 */

#if defined(STM32F10X_MD) || defined(STM32F10X_HD)
      (pfnVector_t) USART3_IRQHandler, /* USART3 */
#else
      0,
#endif

      (pfnVector_t) EXTI15_10_IRQHandler, /* EXTI Line 15..10 */
      (pfnVector_t) RTCAlarm_IRQHandler, /* RTC Alarm through EXTI Line */
      (pfnVector_t) USBWakeUp_IRQHandler, /* USB Wakeup from suspend */

#if defined(STM32F10X_HD)
      (pfnVector_t)TIM8_BRK_IRQHandler, /* TIM8 Break */
      (pfnVector_t)TIM8_UP_IRQHandler, /* TIM8 Update */
      (pfnVector_t)TIM8_TRG_COM_IRQHandler, /* TIM8 Trigger and Commutation */
      (pfnVector_t)TIM8_CC_IRQHandler, /* TIM8 Capture Compare */
      (pfnVector_t)ADC3_IRQHandler, /* ADC3 */
      (pfnVector_t)FSMC_IRQHandler, /* FSMC */
      (pfnVector_t)SDIO_IRQHandler, /* SDIO */
      (pfnVector_t)TIM5_IRQHandler, /* TIM5 */
      (pfnVector_t)SPI3_IRQHandler, /* SPI3 */
      (pfnVector_t)UART4_IRQHandler, /* UART4 */
      (pfnVector_t)UART5_IRQHandler, /* UART5 */
      (pfnVector_t)TIM6_IRQHandler, /* TIM6 */
      (pfnVector_t)TIM7_IRQHandler, /* TIM7 */
      (pfnVector_t)DMA2_Channel1_IRQHandler, /* DMA2 Channel 1 */
      (pfnVector_t)DMA2_Channel2_IRQHandler, /* DMA2 Channel 2 */
      (pfnVector_t)DMA2_Channel3_IRQHandler, /* DMA2 Channel 3 */
      (pfnVector_t)DMA2_Channel4_5_IRQHandler, /* DMA2 Channel 4 and Channel 5 */
      0,0,0,0,0,0,0,0, /* @0x130 */
      0,0,0,0,0,0,0,0, /* @0x150 */
      0,0,0,0,0,0,0,0, /* @0x170 */
      0,0,0,0,0,0,0,0, /* @0x190 */
      0,0,0,0,0,0,0,0, /* @0x1B0 */
      0,0,0,0, /* @0x1D0 */
      (pfnVector_t)0xF1E0F85F /* @0x1E0. This is for boot in RAM mode for
       STM32F10x High Density devices. */
#elif defined(STM32F10X_MD)
      0, 0, 0, 0, 0, 0, 0, (pfnVector_t) 0xF108F85F /* @0x108. This is for boot in RAM mode for
   STM32F10x Medium Density devices. */
#elif defined(STM32F10X_LD)
    0,0,0,0,0,0,0,
    (pfnVector_t)0xF108F85F /* @0x108. This is for boot in RAM mode for
     STM32F10x Low Density devices.*/
#else
#error "Unknown Device Density"
#endif
    };

void __init_data_and_bss(void) __attribute__((noinline));
void __init_static_constructors(void) __attribute__((noinline));

extern "C" int main(void);

void Reset_Handler(void)
  {
    SCB->CCR |= 0x0200; /* Set STKALIGN in NVIC */

#if defined(STM32F10X_HD)
    // TODO: implement properly, copied here from CM3 startup code

    /* FSMC Bank1 NOR/SRAM3 is used for the STM3210E-EVAL, if another Bank is
     required, then adjust the Register Addresses */
    SystemInit_ExtMemCtl();

    /* restore original stack pointer */
    asm(" LDR r0, =_estack");
    asm(" MSR msp, r0");
#endif

    SystemInit();

    // be sure we start with interrupts disabled
    OS::interruptsDisable();

    OSScheduler::ledActiveInit();
    OSScheduler::ISRledActiveOn();

#if false
    wdt_enable(WDTO_2S);

#if !defined(OS_EXCLUDE_MULTITASKING)
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
#endif
#endif

    __init_data_and_bss();

#if defined(DEBUG)
    OSDeviceDebug::earlyInit();
#endif

    OS::earlyInit();

    __init_static_constructors();

    main(); // call standard main()

    for (;;)
      ;
  }

#if defined(STM32F10X_HD)

void SystemInit_ExtMemCtl_Dummy(void)
  {
  }

#endif

unsigned int __get_IPSR(void);

/*
 * This is the code that gets called when the processor receives an
 * unexpected interrupt.  This simply enters an infinite loop, preserving
 * the system state for examination by a debugger.
 */

void Default_Handler(void)
  {
#if defined(DEBUG)
    OSDeviceDebug::putString("Default Handler, IPSR=");
    OSDeviceDebug::putHex((unsigned short) __get_IPSR());
    OSDeviceDebug::putNewLine();
#endif
    /* Go into an infinite loop. */
    for (;;)
      ;
  }

void OS::saveResetBits(void)
  {
    ms_resetBits = -1;
  }

#endif

