/*
 *      Copyright (C) 2009 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS.h"

#if defined(OS_CONFIG_ARCH_ARM_CORTEX_M3)

//void Naked_Reset_Handler(void) __attribute__( ( naked, section( ".init0" ) ) );

#pragma weak NMI_Handler = Sys_Default_Handler
#pragma weak HardFault_Handler = Sys_Default_Handler
#pragma weak MemManage_Handler = Sys_Default_Handler
#pragma weak BusFault_Handler = Sys_Default_Handler
#pragma weak UsageFault_Handler = Sys_Default_Handler
#pragma weak DebugMon_Handler = Sys_Default_Handler
#pragma weak SysTick_Handler = Sys_Default_Handler

/* init value for the stack pointer. defined in linker script */
extern unsigned long __stack_end;

pfnVector_t g_pfnSysVectors[] __attribute__ ((section(".vectors.cm3"))) =
  { (pfnVector_t) (&__stack_end), /* The initial stack pointer */
  (pfnVector_t) Reset_Handler, /* Reset Handler */
  (pfnVector_t) NMI_Handler, /* NMI Handler */
  (pfnVector_t) HardFault_Handler, /* Hard Fault Handler */
  (pfnVector_t) MemManage_Handler, /* MPU Fault Handler */
  (pfnVector_t) BusFault_Handler, /* Bus Fault Handler */
  (pfnVector_t) UsageFault_Handler, /* Usage Fault Handler */
  0, /* Reserved */
  0, /* Reserved */
  0, /* Reserved */
  0, /* Reserved */
  (pfnVector_t) SVC_Handler, /* SVCall Handler */
  (pfnVector_t) DebugMon_Handler, /* Debug Monitor Handler */
  0, /* Reserved */
  (pfnVector_t) PendSV_Handler, /* PendSV Handler */
  (pfnVector_t) SysTick_Handler /* SysTick Handler */
  };

unsigned int __get_IPSR(void);

/*
 * This is the code that gets called when the processor receives an
 * unexpected interrupt.  This simply enters an infinite loop, preserving
 * the system state for examination by a debugger.
 */

void Sys_Default_Handler(void)
  {
#if defined(DEBUG)
    OSDeviceDebug::putString("Sys Default Handler, IPSR=");
    OSDeviceDebug::putHex((unsigned short) __get_IPSR());
    OSDeviceDebug::putNewLine();
#endif
    /* Go into an infinite loop. */
    for (;;)
      ;
  }

/*
 * Switch context to the one pointed by the static g_ppCurrentStack
 * and return to Thread Mode, using Process Stack.
 *
 * Invoked with "svc 1" in OSScheduler::startImpl().
 */
void SVC_Handler(void)
  {
    /* This is a naked function. */

    asm volatile (
        " tst lr, #4            \n"
        " ite eq                \n"
        " mrseq r0, msp         \n"
        " mrsne r0, psp         \n"
        " b SVC_Handler_Impl    \n"
        :::"r0"
    );
  }

extern "C" void SVC_Handler_Impl(unsigned int *svc_args) __attribute__((naked));

void SVC_Handler_Impl(unsigned int *svc_args)
  {
    unsigned int svc_number;
    svc_number = ((char *) svc_args[6])[-2];
    if (svc_number == 0)
      {
        OSScheduler::contextSave();
        OSScheduler::contextSwitch(); /* update g_ppCurrentStack */
        OSScheduler::contextRestore();

        OS::returnFromSubroutine(); /* return to new task */
      }
    else
      {
        //OSDeviceDebug::putChar('^');

        asm volatile (
            " movw r3, #:lower16:g_ppCurrentStack \n"
            " movt r3, #:upper16:g_ppCurrentStack \n"
            " ldr r2, [r3]          \n" /* &OSTask::m_pStack */
            " ldr r0, [r2]          \n" /* OSTask::m_pStack */

            " ldmia r0!, {r4-r11}   \n" /* pop the registers that are not automatically saved on exception entry and the critical nesting count. */
            " msr psp, r0           \n" /* restore the task stack pointer. */
            :::"r0", "r2", "r3"
        );

        OS::interruptsClearMask(); /* enable interrupts */
        OS::returnFromInterrupt(); /* return to Thread Mode, using Process Stack */
      }
  }

void PendSV_Handler(void)
  {
    /* This is a naked function. */

    /* Should have the lowest priority, so it will not be called
     * during other interrupts.
     */
    OSScheduler::contextSave();
    OSScheduler::contextSwitch(); /* update g_ppCurrentStack */
    OSScheduler::contextRestore();

    OS::returnFromSubroutine(); /* return to new task */
  }

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

void __init_data_and_bss(void)
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
extern unsigned long __init_array_start;

/* end address for the constructors section. defined in linker script */
extern unsigned long __init_array_end;

typedef void (*pFunc_t)(void);

void __init_static_constructors(void)
  {
    unsigned long *p;
    void (*pFunc)(void);

    /* run constructors */
    p = &__init_array_start;
    for (; p < &__init_array_end; p++)
      {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
        OSDeviceDebug::putString("INIT=");
        OSDeviceDebug::putHex(*p);
        OSDeviceDebug::putNewLine();
#endif
        pFunc = (pFunc_t) (*p);
        (*pFunc)();
      }
  }


#if true
extern "C" void __dso_handle(void);
void __dso_handle(void)
  {
    ;
  }
#endif

#endif

