/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UOS_H_
#define UOS_H_

#if defined OS_CONFIG_BOARD_
#error "missing BOARD definition in Eclipse"
#endif

#include "portable/kernel/include/OS.h"

#if 1
#if defined(OS_INCLUDE_OSMUTEX)
#include "portable/kernel/include/OSMutex.h"
#endif

#if defined(OS_INCLUDE_OSEVENTFLAGS)
#include "portable/kernel/include/OSEventFlags.h"
#endif

#if defined(OS_INCLUDE_OSLOGGER)
#include "portable/misc/include/OSLogger.h"
#endif /* defined(OS_INCLUDE_OSLOGGER) */

#if defined(OS_INCLUDE_SDI12SENSOR) || defined(OS_INCLUDE_SDI12SENSOR_TINY)
// temporary solution to SDI12 timers
#include "portable/devices/sdi12/include/SDI12Sensor.h"
#endif

#if defined(OS_INCLUDE_SDI12BOOTLOADER)
#include "portable/devices/sdi12/include/SDI12BootLoader.h"
#endif

#if defined(OS_INCLUDE_OSDEVICECHARACTER)
#include "portable/devices/character/include/OSDeviceCharacter.h"
#endif

#if false
#if defined(OS_INCLUDE_DEVICECHARACTERUSART)
#include "portable/devices/character/include/DeviceCharacterUsart.h"
#endif

#if defined(OS_INCLUDE_DEVICECHARACTERMULTIUSART1)
#include "portable/devices/character/include/DeviceCharacterMultiUsart1.h"
#endif
#endif

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART0)
#include "portable/devices/character/include/DeviceCharacterBufferedUsart0.h"
#endif

#if defined(OS_INCLUDE_DEVICECHARACTERBUFFEREDUSART1)
#include "portable/devices/character/include/DeviceCharacterBufferedUsart1.h"
#endif

#if defined(OS_INCLUDE_DEVICECHARACTERUSB)
#include "portable/devices/character/include/DeviceCharacterUsb.h"
#endif

#if defined(OS_INCLUDE_CIRCULARBYTEBUFFER)
#include "portable/misc/include/CircularByteBuffer.h"
#endif

#if defined(OS_INCLUDE_CIRCULARBLOCKBUFFER)
#include "portable/misc/include/CircularBlockBuffer.h"
#endif

#if defined(OS_INCLUDE_SIMPLECLI)
#include "portable/misc/include/SimpleCli.h"
#endif

#if defined(OS_INCLUDE_COMMANDLINEINTERFACE)
#include "portable/misc/include/CommandLineInterface.h"
#endif

#if defined(OS_INCLUDE_PARSER)
#include "portable/misc/include/Parser.h"
#endif

#if defined(OS_INCLUDE_MEMCARD)
#include "portable/devices/block/include/DeviceMemCard.h"
#endif

#if defined(OS_INCLUDE_DEVICESPIMASTER)
#include "portable/devices/misc/include/DeviceSPIMaster.h"
#endif

// -----

#if defined(OS_INCLUDE_OSDEVICEADDRESSABLE)
#include "portable/devices/block/include/OSDeviceAddressable.h"
#endif

#if defined(OS_INCLUDE_OSDEVICEADDRESSABLESYNCHRONISED)
#include "portable/devices/block/include/OSDeviceAddressableSynchronised.h"
#endif

#if defined(OS_INCLUDE_OSDEVICEADDRESSABLEPARTITION)
#include "portable/devices/block/include/OSDeviceAddressablePartition.h"
#endif

#if defined(OS_INCLUDE_LARGECIRCULARSTORAGE)
#include "portable/misc/include/LargeCircularStorage.h"
#endif

// -----

#if defined(OS_INCLUDE_DEVICECHIPSELECT)
#include "portable/devices/misc/include/DeviceChipSelect.h"
#endif

#if defined(OS_INCLUDE_OSDEVICECAN)
#include "portable/devices/block/include/OSDeviceCan.h"
#endif

#if defined(OS_INCLUDE_DEVICELM74)
#include "portable/devices/misc/include/DeviceLM74.h"
#endif

#if defined(OS_INCLUDE_DEVICERTC4574)
#include "portable/devices/misc/include/DeviceRTC4574.h"
#endif

#if defined(OS_INCLUDE_DEVICEADC)
#include "portable/devices/misc/include/DeviceADC.h"
#endif

#if defined(OS_INCLUDE_DEVICECAN_AT90CAN)
#include "hal/arch/avr8/at90can/devices/block/include/DeviceCAN_AT90CAN.h"
#endif

#if defined(OS_INCLUDE_ISTREAM)
#include "portable/stdlib/include/istream"
#endif

#if defined(OS_INCLUDE_OSTREAM)
#include "portable/stdlib/include/ostream"
#endif

#if defined(OS_INCLUDE_OSTREAM_OSTHREAD)
#include "portable/kernel/include/ostream_OSThread.h"
#endif

#if defined(OS_INCLUDE_TASKPITPALAC)
#include "portable/tasks/include/TaskPitpalac.h"
#endif

#if defined(OS_INCLUDE_TASKBLINK)
#include "portable/tasks/include/TaskBlink.h"
#endif

#if defined(OS_INCLUDE_CANLEDS)
#include "portable/tasks/include/CANLeds.h"
#endif

#if defined(OS_INCLUDE_EEPROM)
#include "portable/devices/memory/include/EEPROM.h"
#endif

#if defined(OS_INCLUDE_FLASH)
#include "portable/devices/memory/include/Flash.h"
#endif

#endif

// ----------------------------------------------------------------------------

class OSDeviceDebug;
extern OSDeviceDebug debug;

class OS;
extern OS os;

#if defined(OS_INCLUDE_CLOG)
extern std::ostream clog;
#endif

extern const char greeting[];
extern const char buildDateTime[];

// leave it last
#include "App_Defines.h"

// ----------------------------------------------------------------------------

// Application versioning and greeting definitions

// Stringify numeric definitions and concatenate more strings

#if defined(APP_CFGINT_VERSION_MAJOR) && defined(APP_CFGINT_VERSION_MINOR) && defined(APP_CFGINT_VERSION_REVISION)

// Strings
#define APP_CFGSTR_VERSION_MAJOR        OS_CFGSTR_STRINGIFY(APP_CFGINT_VERSION_MAJOR)
#define APP_CFGSTR_VERSION_MINOR        OS_CFGSTR_STRINGIFY(APP_CFGINT_VERSION_MINOR)
#define APP_CFGSTR_VERSION_REVISION     OS_CFGSTR_STRINGIFY(APP_CFGINT_VERSION_REVISION)

#define APP_CFGSTR_VERSION      APP_CFGSTR_VERSION_MAJOR "." APP_CFGSTR_VERSION_MINOR "." APP_CFGSTR_VERSION_REVISION

#if defined(APP_CFGSTR_APPLICATION_NAME) && defined(APP_CFGSTR_APPLICATION_MANUFACTURER)

#define APP_CFGSTR_GREETING     APP_CFGSTR_APPLICATION_MANUFACTURER \
  " / " APP_CFGSTR_APPLICATION_NAME " v" APP_CFGSTR_VERSION ", " __DATE__ " " __TIME__

#endif /* defined(APP_CFGSTR_APPLICATION_NAME) && .. */

#endif /* defined(APP_CFGINT_VERSION_MAJOR) && .. */

#if !defined(APP_CFGSTR_GREETING)

// Default greeting message, will be used in OSDeviceDebug.cpp
#define APP_CFGSTR_GREETING "uOS++ app"

#endif /* !defined(APP_CFGSTR_GREETING) */

// ----------------------------------------------------------------------------

#if false

// Deprecated, replace with above definitions
#if defined(APP_CFGINT_MAJOR) && defined(APP_CFGINT_MINOR) && defined(APP_CFGINT_REVISION)

// Strings
#define APP_CONFIG_MAJOR                xstr(APP_CFGINT_MAJOR)
#define APP_CONFIG_MINOR                xstr(APP_CFGINT_MINOR)
#define APP_CONFIG_REVISION             xstr(APP_CFGINT_REVISION)

#define APP_CONFIG_VERSION      APP_CONFIG_MAJOR "." APP_CONFIG_MINOR "." APP_CONFIG_REVISION

#endif /* defined(APP_CFGINT_MAJOR) && .. */

#endif

// ----------------------------------------------------------------------------

#endif /* UOS_H_ */
