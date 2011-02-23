/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERBUFFEREDUSART1_H_
#define DEVICECHARACTERBUFFEREDUSART1_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_ARCH_AVR8)

#include "hal/arch/avr8/devices/character/include/DeviceCharacterBufferedUsart1.h"

#elif defined(OS_CONFIG_ARCH_AVR32)

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#include "hal/arch/avr32/uc3/devices/character/include/DeviceCharacterBufferedUsart1.h"

#else

#error "Missing OS_CONFIG_FAMILY_* definition"

#endif /* OS_CONFIG_FAMILY_* */

#else

#error "Missing OS_CONFIG_ARCH_* definition"

#endif /* OS_CONFIG_ARCH_* */

#endif /* DEVICECHARACTERBUFFEREDUSART1_H_ */