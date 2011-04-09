/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICEDESCRIPTORS_H_
#define HAL_OSUSBDEVICEDESCRIPTORS_H_

#include "portable/kernel/include/OS_Defines.h"

#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDeviceDescriptorsConfig.h"
#include "portable/devices/usb/include/OSUsbDeviceDescriptorsConfig.h"

// ----------------------------------------------------------------------------

extern PROGMEM S_usb_device_descriptor usb_dev_desc;
extern PROGMEM S_usb_user_configuration2_descriptor usb_conf2_desc;
extern PROGMEM S_usb_language_id usb_user_language_id;
extern PROGMEM S_usb_manufacturer_string_descriptor
    usb_user_manufacturer_string_descriptor;
extern PROGMEM S_usb_product_string_descriptor
    usb_user_product_string_descriptor;
extern PROGMEM S_usb_serial_number usb_user_serial_number;

// ----------------------------------------------------------------------------

#endif /* HAL_OSUSBDEVICEDESCRIPTORS_H_ */
