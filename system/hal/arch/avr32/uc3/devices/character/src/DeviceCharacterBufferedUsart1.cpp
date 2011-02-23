/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsart1.h"

#if defined(OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_CONSTANT
#endif

#if (OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_DOUBLE_SPEED)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (8)
#else
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER        (16)
#endif

#define CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ          (OS_CFGLONG_OSCILLATOR_HZ / OS_CFGINT_CLOCK_PRESCALER / CFGINT_DEVICECHARACTERBUFFEREDUSART_PRESCALLER)
#define CFGINT_DEVICECHARACTERBUFFEREDUSART_BAUD_CONSTANT     ((CFGINT_DEVICECHARACTERBUFFEREDUSART_CLOCK_HZ / OS_CFGINT_DEVICECHARACTERBUFFEREDUSART1_BAUD_RATE)- 1UL)

// ----- static data ----------------------------------------------------------

DeviceCharacterBufferedUsart1 * DeviceCharacterBufferedUsart1::ms_pThis;

// ----- constructors ---------------------------------------------------------

DeviceCharacterBufferedUsart1::DeviceCharacterBufferedUsart1(void)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart1()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif
}

// set custom high/low water marks
DeviceCharacterBufferedUsart1::DeviceCharacterBufferedUsart1(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM, unsigned char *pRxBuf, unsigned short rxBufSize,
    unsigned short rxHWM, unsigned short rxLWM) :
  OSDeviceCharacterBuffered(pTxBuf, txBufSize, txHWM, txLWM, pRxBuf, rxBufSize,
      rxHWM, rxLWM)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart1()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pThis = this;
}

// use default 3/4 high and 1/4 low water marks
DeviceCharacterBufferedUsart1::DeviceCharacterBufferedUsart1(
    unsigned char *pTxBuf, unsigned short txBufSize, unsigned char *pRxBuf,
    unsigned short rxBufSize) :
  OSDeviceCharacterBuffered(pTxBuf, txBufSize, pRxBuf, rxBufSize)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  OSDeviceDebug::putString("DeviceCharacterBufferedUsart1()=");
  OSDeviceDebug::putPtr(this);
  OSDeviceDebug::putNewLine();
#endif

  ms_pThis = this;
}

// ----- inits ---------------------------------------------------------------

int
DeviceCharacterBufferedUsart1::implPortInit(void)
{
  ; // TODO: implement it
}


// ===== ISRs =================================================================

#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)*/
void
Usart1TX_contextHandler(void);

void
Usart1TX_contextHandler(void)
{
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)
  OSScheduler::interruptEnter();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)*/
    {
      ((DeviceCharacterBufferedUsart1 *) DeviceCharacterBufferedUsart1::ms_pThis)->interruptTxServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)
  OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_TX_ISR)*/
}

#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_RX_ISR)
__attribute__((naked))
#else
__attribute__((interrupt))
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_RX_ISR)*/
void
Usart1RX_contextHandler(void);

void
Usart1RX_contextHandler(void)
{
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_RX_ISR)
  OSScheduler::interruptEnter();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_RX_ISR)*/
    {
      ((DeviceCharacterBufferedUsart1 *) DeviceCharacterBufferedUsart1::ms_pThis)->interruptRxServiceRoutine();
    }
#if !defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_RX_ISR)
  OSScheduler::interruptExit();
#endif /*!defined(OS_EXCLUDE_DEVICECHARACTERBUFFEREDUSART1_NAKED_RX_ISR)*/
}

#endif /* defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSARTBASE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
