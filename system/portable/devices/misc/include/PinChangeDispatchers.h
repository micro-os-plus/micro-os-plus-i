/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef PINCHANGEDISPATCHERS_H_
#define PINCHANGEDISPATCHERS_H_

#include "portable/kernel/include/OS.h"

#if !defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC16) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC17) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC18) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC19) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC20) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC21) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC22) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC23) 
#define OS_INCLUDE_PINCHANGEDISPATCHERS_REG2    (1)
#endif
#endif

#if !defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG3)
#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC24) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC25) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC26) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC27) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC28) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC29) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC30) || defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC31) 
#define OS_INCLUDE_PINCHANGEDISPATCHERS_REG3    (1)
#endif
#endif

class PinChangeDispatchers
{
public:

  static void
  init(void);

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC21)
  static void interruptPinChange21enable(void);
  static void interruptPinChange21disable(void);
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC24)
  static void interruptPinChange24enable(void);
  static void interruptPinChange24disable(void);
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC29)
  static void interruptPinChange29enable(void);
  static void interruptPinChange29disable(void);
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
  static interruptPinChange2ServiceRoutine(unsigned char crt, unsigned char prev);

  static void setPrevValue2(unsigned char c);
  static unsigned char getPrevValue2(void);
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG3)
  static interruptPinChange3ServiceRoutine(unsigned char crt, unsigned char prev);

  static void setPrevValue3(unsigned char c);
  static unsigned char getPrevValue3(void);
#endif

protected:
#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
  static unsigned char ms_prevValue2;
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG3)
  static unsigned char ms_prevValue3;
#endif
};

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG2)
inline void PinChangeDispatchers::setPrevValue2(unsigned char c)
  {
    ms_prevValue2 = c;
  }

inline unsigned char PinChangeDispatchers::getPrevValue2(void)
  {
    return ms_prevValue2;
  }
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_REG3)
inline void PinChangeDispatchers::setPrevValue3(unsigned char c)
  {
    ms_prevValue3 = c;
  }

inline unsigned char PinChangeDispatchers::getPrevValue3(void)
  {
    return ms_prevValue3;
  }
#endif

// implementation methods
#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC21)
inline void PinChangeDispatchers::interruptPinChange21enable()
  {
    PCMSK2 |= _BV(PCINT21);
  }

inline void PinChangeDispatchers::interruptPinChange21disable()
  {
    PCMSK2 &= ~_BV(PCINT21);
  }
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC24)
inline void PinChangeDispatchers::interruptPinChange24enable()
  {
    PCMSK3 |= _BV(PCINT24);
  }

inline void PinChangeDispatchers::interruptPinChange24disable()
  {
    PCMSK3 &= ~_BV(PCINT24);
  }
#endif

#if defined(OS_INCLUDE_PINCHANGEDISPATCHERS_PINC29)
inline void PinChangeDispatchers::interruptPinChange29enable()
  {
    PCMSK3 |= _BV(PCINT29);
  }

inline void PinChangeDispatchers::interruptPinChange29disable()
  {
    PCMSK3 &= ~_BV(PCINT29);
  }
#endif

#endif /* PINCHANGEDISPATCHERS_H_ */
