/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

/*
 * Important notice: Starting with 3.0 this file is deprecated 
 * and may not be maintained in future versions. 
 */
#include "portable/kernel/include/uOS.h"

#include "portable/kernel/src/OS.cpp"
//#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/src/OS_Implementation.cpp"
//#else
//#error "Missing OS_CONFIG_ARCH_* definition"
//#endif

#if !defined(OS_EXCLUDE_MULTITASKING)

#include "portable/kernel/src/OSScheduler.cpp"
//#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/src/OSScheduler_Implementation.cpp"
//#else
//#error "Missing OS_CONFIG_ARCH_* definition"
//#endif

#include "portable/kernel/src/OSThread.cpp"

#include "portable/kernel/src/OSThreadIdle.cpp"

#include "portable/kernel/src/OSTimer.cpp"
#include "portable/kernel/src/OSTimerTicks.cpp"
//#if defined(OS_INCLUDE_OSSCHEDULER_TIMERSECONDS)
#include "portable/kernel/src/OSTimerSeconds.cpp"
//#endif
//#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/src/OSTimer_Implementation.cpp"
//#else
//#error "Missing OS_CONFIG_ARCH_* definition"
//#endif

#include "portable/kernel/src/Timer.cpp"

//#if defined(OS_INCLUDE_OSEVENTFLAGS)
#include "portable/kernel/src/OSEventFlags.cpp"
//#endif

//#if defined(OS_INCLUDE_OSMUTEX)
#include "portable/kernel/src/OSMutex.cpp"
//#endif

#endif /* OS_EXCLUDE_MULTITASKING */

//#if defined(OS_INCLUDE_SDI12SENSOR) || defined(OS_INCLUDE_SDI12SENSOR_TINY)
#include "portable/devices/sdi12/src/SDI12Sensor.cpp"
//#if defined(OS_CONFIG_BOARD_M512)
#include "hal/boards/Metrilog/m512/src/SDI12Sensor_Implementation.cpp"
//#else
//#error "Missing OS_CONFIG_BOARD_* definition"
//#endif
//#include "hal/redirect/devices/sdi12/include/SDI12SensorInlines.h"

//#if defined(OS_INCLUDE_SDI12BOOTLOADER)
#include "portable/devices/sdi12/src/SDI12Bootloader.cpp"
//#endif

//#endif

#include "portable/devices/debug/src/OSDeviceDebug.cpp"
//#if defined(DEBUG)

//#if defined(OS_CONFIG_DEBUG_DEVICE_I2C)
#include "portable/devices/debug/src/DeviceDebugI2C.cpp"
//#elif defined(OS_CONFIG_DEBUG_DEVICE_USART)
//#include "portable/devices/debug/src/DeviceDebugUsart.cpp"
//#else
//#error "Missing OS_CONFIG_DEBUG_DEVICE_* definition"
//#endif

//OSDeviceDebug debug;

//#else

// even if code is skipped, the debug reference must remain
//OSDeviceDebug debug;

//#endif

#if !defined(OS_EXCLUDE_MULTITASKING)

//#if defined(OS_INCLUDE_OSUSBDEVICE)
#include "portable/devices/usb/src/OSUsbDevice.cpp"
//#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/usb/src/OSUsbDevice_Implementation.cpp"
//#else
//#error "Unsupported processor family"
//#endif
#include "portable/devices/usb/src/OSUsbDeviceDescriptors.cpp"
//#endif

//#if defined(OS_INCLUDE_CIRCULARBLOCKBUFFER)
#include "portable/misc/src/CircularBlockBuffer.cpp"
//#endif

//#if defined(OS_INCLUDE_OSDEVICECHARACTER)
#include "portable/misc/src/CircularByteBuffer.cpp"
#include "portable/devices/character/src/OSDeviceCharacter.cpp"

//#if defined(OS_INCLUDE_DEVICECHARACTERUSART)
//#if defined(OS_CONFIG_FAMILY_AT90USB)
//#include "hal/arch/avr8/at90usb/devices/character/src/DeviceCharacterUsart_Implementation.cpp"
//#else
//#error "Unsupported processor family"
//#endif
#include "portable/devices/character/src/DeviceCharacterUsart.cpp"
//#endif

//#if defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART1)
#include "portable/devices/character/src/DeviceCharacterMultiUsart1.cpp"
//#if defined(OS_CONFIG_FAMILY_AT90USB)
//#include "hal/arch/avr8/at90usb/devices/character/src/DeviceCharacterMultiUsart1_Implementation.cpp"
//#else
//#error "Unsupported processor family"
//#endif
//#endif

//#if defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART)
//#include "portable/devices/character/src/DeviceCharacterMultiUsart.cpp"
//#endif

//#if defined(OS_INCLUDE_DEVICECHARACTERUSB)
#include "portable/devices/character/src/DeviceCharacterUsb.cpp"
//#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/character/src/DeviceCharacterUsb_Implementation.cpp"
//#else
//#error "Unsupported processor family"
//#endif
//#endif
//#endif

//#if defined(OS_INCLUDE_OSTREAM) || defined(OS_INCLUDE_ISTREAM)
#include "portable/stdlib/src/ios.cpp"
#include "portable/stdlib/src/streambuf.cpp"
//#endif

//#if defined(OS_INCLUDE_OSTREAM)
#include "portable/stdlib/src/ostream.cpp"
//#if defined(OS_INCLUDE_OSTREAM_OSTHREAD)
#include "portable/kernel/src/ostream_OSThread.cpp"
//#endif
//#if defined(OS_INCLUDE_OSTREAM_PROGRAMPTR)
//#if defined(OS_INCLUDE_OSTREAM_PROGRAMPTR)

//#if defined(OS_CONFIG_ARCH_AVR8)
#include "hal/arch/avr8/kernel/src/ostream_ProgramPtr.cpp"
//#else
//#error "Unsupported architecture"
//#endif

//#endif//#endif
//#endif

//#if defined(OS_INCLUDE_ISTREAM)
#include "portable/stdlib/src/istream.cpp"
//#endif

//#if defined(OS_INCLUDE_SIMPLECLI)
#include "portable/misc/src/SimpleCli.cpp"
//#endif

//#if defined(OS_INCLUDE_MEMCARD)
//#include "hal/redirect/devices/block/include/MemCardSelectInlines.h"
#include "portable/devices/block/src/DeviceMemCard.cpp"
#include "portable/devices/block/src/DeviceMemCardSPI.cpp"
//#include "hal/redirect/devices/block/src/MemCardSpiImpl.cpp"
//#endif

//#if defined(OS_INCLUDE_TASKBLINK)
#include "portable/tasks/src/TaskBlink.cpp"
//#endif

//#if defined(OS_INCLUDE_TASKPITPALAC)
#include "portable/tasks/src/TaskPitpalac.cpp"
//#endif

//#if defined(OS_INCLUDE_CLOG)
// debug stream used to display thread messages
//ostream clog( &debug );
//#endif

//#include "portable/misc/src/uOS.cpp"

#endif

