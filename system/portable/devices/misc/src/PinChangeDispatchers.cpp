/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS)

#include "portable/devices/misc/include/PinChangeDispatchers.h"

void PinChangeDispatchers::init(void)
  {
#if defined(DEBUG)
    OSDeviceDebug::putString("PinChangeDispatchers::init()");
    OSDeviceDebug::putNewLine();
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
    // Enable group 2
    ms_prevValue2 = PINC;
    PCIFR &= ~_BV(PCIF2);
    PCICR |= _BV(PCIE2);
    PCMSK2 = 0;
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG3)
    // Enable group 3
    ms_prevValue3 = PIND;
    PCIFR &= ~_BV(PCIF3);
    PCICR |= _BV(PCIE3);
    PCMSK3 = 0;
#endif

  }

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)

extern "C"
void PCINT2_vect( void ) __attribute__( ( signal, naked ) );

void PCINT2_vect(void)
  {
    OSScheduler::interruptEnter(); // interrupts disabled in here

      {
        unsigned char crt;
        crt = PINC;

        // beware of race conditions when bits change during the ISR
        PinChangeDispatchers::interruptPinChange2ServiceRoutine(crt,
            PinChangeDispatchers::getPrevValue2());

        PinChangeDispatchers::setPrevValue2(crt);
      }
    OSScheduler::interruptExit();
  }
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG3)
extern "C"
void PCINT3_vect( void ) __attribute__( ( signal, naked ) );

void PCINT3_vect(void)
  {
    OSScheduler::interruptEnter(); // interrupts disabled in here

      {
        unsigned char crt;
        crt = PINC;

        // beware of race conditions when bits change during the ISR
        PinChangeDispatchers::interruptPinChange3ServiceRoutine(crt,
            PinChangeDispatchers::getPrevValue3());

        PinChangeDispatchers::setPrevValue3(crt);
      }
    OSScheduler::interruptExit();
  }
#endif

// static members
namespace PinChangeDispatchers
  {
#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
  unsigned char ms_prevValue2;
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
  unsigned char ms_prevValue3;
#endif
  }

#endif
