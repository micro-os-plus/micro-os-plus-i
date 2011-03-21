/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICE_DEFINES_H_
#define HAL_OSUSBDEVICE_DEFINES_H_

// ----- Non portable definitions ---------------------------------------------

#define MSK_UADD              0x7F
#define MSK_EP_DIR            0x7F

// Parameters for endpoint configuration
// These define are the values used to enable and configure an endpoint.
#define TYPE_CONTROL             0
#define TYPE_ISOCHRONOUS         1
#define TYPE_BULK                2
#define TYPE_INTERRUPT           3
//typedef enum ep_type {TYPE_CONTROL, TYPE_BULK, TYPE_ISOCHRONOUS, TYPE_INTERRUPT} e_ep_type;

#define DIRECTION_OUT            0
#define DIRECTION_IN             1
//typedef enum ep_dir {DIRECTION_OUT, DIRECTION_IN} e_ep_dir;

#define SIZE_8                   0
#define SIZE_16                  1
#define SIZE_32                  2
#define SIZE_64                  3
#define SIZE_128                 4
#define SIZE_256                 5
#define SIZE_512                 6
#define SIZE_1024                7
//typedef enum ep_size {SIZE_8,   SIZE_16,  SIZE_32,  SIZE_64,
//                      SIZE_128, SIZE_256, SIZE_512, SIZE_1024} e_ep_size;

#define ONE_BANK                 0
#define TWO_BANKS                1
//typedef enum ep_bank {ONE_BANK, TWO_BANKS} e_ep_bank;

#define NYET_ENABLED             0
#define NYET_DISABLED            1

// TODO: check if this is architecture or usb
#define EVT_USB_POWERED               1 // USB plugged
#define EVT_USB_UNPOWERED             2 // USB un-plugged
#define EVT_USB_DEVICE_FUNCTION       3 // USB in device
#define EVT_USB_HOST_FUNCTION         4 // USB in host
#define EVT_USB_SUSPEND               5 // USB suspend
#define EVT_USB_WAKE_UP               6 // USB wake up
#define EVT_USB_RESUME                7 // USB resume
#define EVT_USB_RESET                 8 // USB reset
#define EVT_HOST_SOF                  9 // Host start of frame sent
#define EVT_HOST_HWUP                 10        // Host wakeup detected
#define EVT_HOST_DISCONNECTION        11        // The target device is disconencted
#define USB_CONFIG_ATTRIBUTES_RESERVED  0x80
#define USB_CONFIG_BUSPOWERED           (USB_CONFIG_ATTRIBUTES_RESERVED | 0x00)
#define USB_CONFIG_SELFPOWERED          (USB_CONFIG_ATTRIBUTES_RESERVED | 0x40)
#define USB_CONFIG_REMOTEWAKEUP         (USB_CONFIG_ATTRIBUTES_RESERVED | 0x20)

// ----- Non portable definitions ---------------------------------------------

#endif /* HAL_OSUSBDEVICE_DEFINES_H_ */
