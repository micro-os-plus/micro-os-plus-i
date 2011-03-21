/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBDEVICE_DEFINES_H_
#define OSUSBDEVICE_DEFINES_H_

#include "portable/kernel/include/OS_Defines.h"

// ----- Endpoints ------------------------------------------------------------

#define MAX_EP_NB                       7

#define EP_CONTROL                      0
#define EP_1                            1
#define EP_2                            2
#define EP_3                            3
#define EP_4                            4
#define EP_5                            5
#define EP_6                            6
#define EP_7                            7

// ----- Standard requests ----------------------------------------------------

#define GET_STATUS                      0x00
#define GET_DEVICE                      0x01
#define CLEAR_FEATURE                   0x01     // see FEATURES below ?
#define GET_STRING                      0x03
#define SET_FEATURE                     0x03     // see FEATURES below ?
#define SET_ADDRESS                     0x05
#define GET_DESCRIPTOR                  0x06
#define SET_DESCRIPTOR                  0x07
#define GET_CONFIGURATION               0x08
#define SET_CONFIGURATION               0x09
#define GET_INTERFACE                   0x0A
#define SET_INTERFACE                   0x0B
#define SYNCH_FRAME                     0x0C

// ----- Descriptor Types -----------------------------------------------------

#define DEVICE_DESCRIPTOR                       0x01
#define CONFIGURATION_DESCRIPTOR                0x02
#define STRING_DESCRIPTOR                       0x03
#define INTERFACE_DESCRIPTOR                    0x04
#define ENDPOINT_DESCRIPTOR                     0x05
#define DEVICE_QUALIFIER_DESCRIPTOR             0x06
#define OTHER_SPEED_CONFIGURATION_DESCRIPTOR    0x07

// ----------------------------------------------------------------------------

#define ZERO_TYPE                       0x00
#define INTERFACE_TYPE                  0x01
#define ENDPOINT_TYPE                   0x02

#define REQUEST_DEVICE_STATUS           0x80
#define REQUEST_INTERFACE_STATUS        0x81
#define REQUEST_ENDPOINT_STATUS         0x82

#define USB_DT_CS_INTERFACE             0x24

#define USB_CDC_HEADER_TYPE             0x00
#define USB_CDC_CALL_MANAGEMENT_TYPE    0x01
#define USB_ACM_TYPE                    0x02
#define USB_CDC_UNION_TYPE              0x06

// ----- Features -------------------------------------------------------------

#define FEATURE_DEVICE_REMOTE_WAKEUP          0x01
#define FEATURE_ENDPOINT_HALT                 0x00
#define FEATURE_TEST_MODE                     0x02

// ----------------------------------------------------------------------------

#endif /* OSUSBDEVICE_DEFINES_H_ */
