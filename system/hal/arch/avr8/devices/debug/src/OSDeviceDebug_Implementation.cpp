/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR8) 
#if defined(DEBUG)

#include "portable/kernel/include/OS.h"

void
OSDeviceDebug::putString_P(const char* PROGMEM pc)
{
  if (pc == 0)
    return;

  OSCriticalSection::enter();
    {
      unsigned char buff[10];
      unsigned short i;

      i = 0;
      for (; (buff[i++] = pgm_read_byte(pc++)) != '\0';)
        {
          if (i == sizeof(buff))
            {
              commonPutBytes((const char*) buff, i);
              i = 0;
            }
        }

      if (i > 1)
        {
          commonPutBytes((const char*) buff, i);
        }
    }
  OSCriticalSection::exit();
}

#if defined(OS_DEBUG_CONSTRUCTORS)

// display constructor name and object address
void
OSDeviceDebug::putConstructor_P(const char* PROGMEM pc, const void* p)
{
  register OSStack_t mask;

  mask = criticalEnter();
    {
      putString_P(pc);
      putString("() @");
      putPtr(p);
      putNewLine();
    }
  criticalExit(mask);
}

// display constructor name and object address
void
OSDeviceDebug::putConstructorWithIndex_P(const char* PROGMEM pc, uint16_t i,
    const void* p)
{
  register OSStack_t mask;

  mask = criticalEnter();
    {
      putString_P(pc);
      putChar('(');
      putDec(i);
      putString(") @");
      putPtr(p);
      putNewLine();
    }
  criticalExit(mask);
}

// display destructor name and object address
void
OSDeviceDebug::putDestructor_P(const char* PROGMEM pc, const void* p)
{
  register OSStack_t mask;

  mask = criticalEnter();
    {
      putChar('~');
      putString_P(pc);
      putString("() @");
      putPtr(p);
      putNewLine();
    }
  criticalExit(mask);
}

#endif /* defined(OS_DEBUG_CONSTRUCTORS) */

#endif /* defined(DEBUG) */
#endif /* defined(OS_CONFIG_ARCH_AVR8) */
