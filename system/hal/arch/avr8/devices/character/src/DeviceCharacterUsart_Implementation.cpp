/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECHARACTERUSART) && !defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART) && defined(OS_CONFIG_ARCH_AVR)

#include "portable/devices/character/include/DeviceCharacterUsart.h"

// ===== ISRs ================================================================

#if (OS_CFGBOOL_DEVICECHARACTERUSART_TX_CONTEXT_SWITCH)

extern "C" void USART1_UDRE_vect( void ) __attribute__( ( signal, naked ) );

void
USART1_UDRE_vect( void )
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCharacterUsart::ms_pThis->interruptTxServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#else

extern "C"
void USART1_UDRE_vect( void ) __attribute__( ( signal ) );

void USART1_UDRE_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterUsart::ms_pThis->interruptTxServiceRoutine();
  }

#endif /*OS_CFGBOOL_DEVICECHARACTERUSART_TX_CONTEXT_SWITCH*/

#if (OS_CFGBOOL_DEVICECHARACTERUSART_RX_CONTEXT_SWITCH)

extern "C" void USART1_RX_vect( void ) __attribute__( ( signal, naked ) );

void
USART1_RX_vect( void )
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCharacterUsart::ms_pThis->interruptRxServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#else

extern "C"
void USART1_RX_vect( void ) __attribute__( ( signal ) );

void USART1_RX_vect(void)
  {
    OSScheduler::ledActiveOn();

    DeviceCharacterUsart::ms_pThis->interruptRxServiceRoutine();
  }

#endif /*OS_CFGBOOL_DEVICECHARACTERUSART_RX_CONTEXT_SWITCH*/

#endif
