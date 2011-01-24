/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_AVR32)

/* init value for the stack pointer. defined in linker script */
extern unsigned long __stack_end;

/* start address for the initialization values of the .data section.
 defined in linker script */
extern unsigned long __data_load_start;

/* start address for the .data section. defined in linker script */
extern unsigned long __data_start;

/* end address for the .data section. defined in linker script */
extern unsigned long __data_end;

/* start address for the .bss section. defined in linker script */
extern unsigned long __bss_start;

/* end address for the .bss section. defined in linker script */
extern unsigned long __bss_end;

void os_init_data_and_bss(void)
  {
    unsigned long *pSrc, *pDest;

    /* Copy the data segment initializers from flash to SRAM */
    pSrc = &__data_load_start;
    pDest = &__data_start;
    for (; pDest < &__data_end;)
      {
        *(pDest++) = *(pSrc++);
      }
    /* Zero fill the bss segment. */
    for (pDest = &__bss_start; pDest < &__bss_end;)
      {
        *(pDest++) = 0;
      }
  }

/* start address for the constructors section. defined in linker script */
extern unsigned long __ctors_array_start;

/* end address for the constructors section. defined in linker script */
extern unsigned long __ctors_array_end;

typedef void (*pFunc_t)(void);

void os_init_static_constructors(void)
  {
    unsigned long *p;
    void (*pFunc)(void);

    p = &__ctors_array_start;
    for (; p < &__ctors_array_end; p++)
      {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS) && defined(OS_DEBUG_CONSTRUCTORS_INIT)
        OSDeviceDebug::putString("INIT=");
        OSDeviceDebug::putHex(*p);
        OSDeviceDebug::putNewLine();
#endif
        pFunc = (pFunc_t) (*p);
        (*pFunc)();
      }
}


#if false
extern "C" void __dso_handle(void);
void __dso_handle(void)
  {
    ;
  }
#endif

#endif /* OS_CONFIG_ARCH_AVR32 */

