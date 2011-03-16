/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR8) 
#if defined(DEBUG)

#include "portable/kernel/include/OS.h"

void OSDeviceDebug::putString_P(const char * PROGMEM pc)
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
                commonPutBytes((const char *)buff, i);
                i = 0;
              }            
          }
        
        if (i > 1)
          {
            commonPutBytes((const char *)buff, i);
          }
      }
    OSCriticalSection::exit();
  }

#endif /*DEBUG*/
#endif /*OS_CONFIG_ARCH_AVR8*/
