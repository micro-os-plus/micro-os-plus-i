/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICEDESCRIPTORSCONFIG_H_
#define HAL_OSUSBDEVICEDESCRIPTORSCONFIG_H_

#include "portable/kernel/include/OS.h"

// ----------------------------------------------------------------------------

// English US
#define LANGUAGE_ID           0x0409

#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
#define NB_ENDPOINTS            (1+3+3) //!  number of endpoints in the application including control endpoint
#else
#define NB_ENDPOINTS            (1+3) //!  number of endpoints in the application including control endpoint
#endif
#define TX_EP                   EP_1
#define RX_EP                   EP_2
#define INT_EP                  EP_3

#define TXb_EP                  EP_4
#define RXb_EP                  EP_5
#define INTb_EP                 EP_6

// USB Device descriptor
#define USB_SPECIFICATION       0x0200
// NOTE: device_class 0 does not work on Debian 3.1
#define DEVICE_CLASS            0x02    // CDC class
//#define DEVICE_CLASS          0 // each configuration has its own class
#define DEVICE_SUB_CLASS        0       // each configuration has its own sub-class
#define DEVICE_PROTOCOL         0       // each configuration has its own protocol
#define EP_CONTROL_LENGTH       64
#define VENDOR_ID               0x03EB  // Atmel vendor ID = 03EBh
#if TO_BE_PORTED
#define PRODUCT_ID              0x2018
#else
#define PRODUCT_ID              0x2307
#endif
#define RELEASE_NUMBER          0x1000

#define LANG_ID                 0x00

#define STRING_INDEX_MAN        0x01
#define STRING_INDEX_PROD       0x02
#define STRING_INDEX_SN         0x03

#if defined(OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION)
#define NB_CONFIGURATION        2 // number of configurations
#else
#define NB_CONFIGURATION        1 // number of configurations
#endif

// CDC CONFIGURATION
// configuration numbers must be non zero!
#if defined(OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION)
#define CONF1_NB                1    // configuration number 1
#define CONF2_NB                2    // configuration number 2
#else
#define CONF2_NB                1    // configuration number 1
#endif
#define CONF_NB            1     //! Number of this configuration
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
#define NB2_INTERFACE           4    // number of interfaces
#else
#define NB2_INTERFACE           2
#endif
#define NB1_INTERFACE           2

#define NB_INTERFACE           2

// NOTE: interface numbers must start from 0!

#define CONF_INDEX              0    // configuration string index
#if TO_BE_PORTED
#define CONF_ATTRIBUTES         USB_CONFIG_BUSPOWERED
#else
#define CONF_ATTRIBUTES         USB_CONFIG_SELFPOWERED
#endif
#define MAX_POWER               50      // *2 mA = 100 mA
// Interface 0 descriptor
#define IF0_NB                  0          // interface number 0
#define ALTERNATE0              0
#define NB_ENDPOINT0            1          // one endpoint
#define IF0_CLASS               0x02    // Communication Interface Class
#define IF0_SUB_CLASS           0x02       // Abstract Control Model Subclass
#define IF0_PROTOCOL            0x01       // V.25ter Common AT commands
#define STRING_INDEX_IF0        0          // index of string descriptor
// Interface 1 descriptor
#define IF1_NB                  1          // interface number 1
#define ALTERNATE1              0
#define NB_ENDPOINT1            2          // two endpoints
#define IF1_CLASS               0x0A    // Data Interface Class
#define IF1_SUB_CLASS           0          // no subclass
#define IF1_PROTOCOL            0          // no protocol
#define STRING_INDEX_IF1        0          // index of string descriptor
#define TX_EP_SIZE              0x20

// USB Endpoint 3 descriptor
// Interrupt IN
#define EP_NB_3                 0x80 | INT_EP       // IN
#define EP_ATTRIBUTES_3         0x03    // BULK = 0x02, INTERUPT = 0x03
#define EP_SIZE_3               0x20
#define EP_INTERVAL_3           0xFF    //ms interrupt pooling from host
// USB Endpoint 1 descriptor
// Bulk IN
#define EP_NB_1                 0x80 | TX_EP        // IN
#define EP_ATTRIBUTES_1         0x02    // BULK = 0x02, INTERUPT = 0x03
#define EP_SIZE_1               0x20
#define EP_INTERVAL_1           0x00
#define EP_IN_LENGTH_1_FS   0x40
#define EP_IN_LENGTH_1_HS   512
#define EP_SIZE_1_FS        EP_IN_LENGTH_1_FS
#define EP_SIZE_1_HS        EP_IN_LENGTH_1_HS

// USB Endpoint 2 descriptor
//Bulk OUT  RX endpoint
#define EP_NB_2                 RX_EP               // OUT
#define EP_ATTRIBUTES_2         0x02    // BULK = 0x02, INTERUPT = 0x03
#define EP_SIZE_2               0x20
#define EP_INTERVAL_2           0x00
#define EP_IN_LENGTH_2_FS   0x40
#define EP_IN_LENGTH_2_HS   512
#define EP_SIZE_2_FS        EP_IN_LENGTH_2_FS
#define EP_SIZE_2_HS        EP_IN_LENGTH_2_HS

//#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)

// Interface 0 descriptor
#define IF0b_NB                 2
#define ALTERNATE0b             0
#define NB_ENDPOINT0b           1
#define IF0b_CLASS              0x02       // CDC ACM Com
#define IF0b_SUB_CLASS          0x02
#define IF0b_PROTOCOL           0x01
#define STRING_INDEX_IF0b       0

// Interface 1 descriptor
#define IF1b_NB                 3
#define ALTERNATE1b             0
#define NB_ENDPOINT1b           2
#define IF1b_CLASS              0x0A       // CDC ACM Data
#define IF1b_SUB_CLASS          0
#define IF1b_PROTOCOL           0
#define STRING_INDEX_IF1b       0

// USB Endpoint 3 descriptor
// Interrupt IN
#define EP_NB_3b                0x80 | INTb_EP
#define EP_ATTRIBUTES_3b        0x03        // BULK = 0x02, INTERUPT = 0x03
#define EP_SIZE_3b              TX_EP_SIZE
#define EP_INTERVAL_3b          0xFF        //ms interrupt pooling from host
// USB Endpoint 1 descriptor
// Bulk IN
#define EP_NB_1b                0x80 | TXb_EP
#define EP_ATTRIBUTES_1b        0x02        // BULK = 0x02, INTERUPT = 0x03
#define EP_SIZE_1b              0x20
#define EP_INTERVAL_1b          0x00

// USB Endpoint 2 descriptor
//Bulk OUT  RX endpoint
#define EP_NB_2b                RXb_EP
#define EP_ATTRIBUTES_2b        0x02        // BULK = 0x02, INTERUPT = 0x03
#define EP_SIZE_2b              0x20
#define EP_INTERVAL_2b          0x00
//#endif

#if TO_BE_PORTED
#define DEVICE_STATUS           0x00    // TBD
#else
#define SELF_POWERED          1
#define DEVICE_STATUS         SELF_POWERED
#endif
#define INTERFACE_STATUS      0x00 // TBD
//#define Usb_unicode(a)                  ((unsigned short)(a))

#if TO_BE_PORTED
#define USB_MN_LENGTH           25
#define USB_MANUFACTURER_NAME \
{ Usb_unicode('A') \
  , Usb_unicode('v') \
  , Usb_unicode('i') \
  , Usb_unicode('a') \
  , Usb_unicode('t') \
  , Usb_unicode('i') \
  , Usb_unicode('o') \
  , Usb_unicode('n') \
  , Usb_unicode(' ') \
  , Usb_unicode('I') \
  , Usb_unicode('n') \
  , Usb_unicode('s') \
  , Usb_unicode('t') \
  , Usb_unicode('r') \
  , Usb_unicode('u') \
  , Usb_unicode('m') \
  , Usb_unicode('e') \
  , Usb_unicode('n') \
  , Usb_unicode('t') \
  , Usb_unicode('s') \
  , Usb_unicode(' ') \
  , Usb_unicode('I') \
  , Usb_unicode('n') \
  , Usb_unicode('t') \
  , Usb_unicode('l') \
}

#define USB_PN_LENGTH           20
#define USB_PRODUCT_NAME \
{ Usb_unicode('A') \
  ,Usb_unicode('V') \
  ,Usb_unicode('R') \
  ,Usb_unicode(' ') \
  ,Usb_unicode('U') \
  ,Usb_unicode('S') \
  ,Usb_unicode('B') \
  ,Usb_unicode(' ') \
  ,Usb_unicode('C') \
  ,Usb_unicode('D') \
  ,Usb_unicode('C') \
  ,Usb_unicode('/') \
  ,Usb_unicode('A') \
  ,Usb_unicode('C') \
  ,Usb_unicode('M') \
  ,Usb_unicode(' ') \
  ,Usb_unicode('D') \
  ,Usb_unicode('E') \
  ,Usb_unicode('M') \
  ,Usb_unicode('0') \
}

#define USB_SN_LENGTH         0x05
#define USB_SERIAL_NUMBER \
{Usb_unicode('1') \
 ,Usb_unicode('.') \
 ,Usb_unicode('0') \
 ,Usb_unicode('.') \
 ,Usb_unicode('0') \
}
#else

#define USB_MN_LENGTH         5
#define USB_MANUFACTURER_NAME \
{\
  Usb_unicode('A'),\
  Usb_unicode('T'),\
  Usb_unicode('M'),\
  Usb_unicode('E'),\
  Usb_unicode('L') \
}

#define USB_PN_LENGTH         13
#define USB_PRODUCT_NAME \
{\
  Usb_unicode('A'),\
  Usb_unicode('V'),\
  Usb_unicode('R'),\
  Usb_unicode('3'),\
  Usb_unicode('2'),\
  Usb_unicode(' '),\
  Usb_unicode('U'),\
  Usb_unicode('C'),\
  Usb_unicode('3'),\
  Usb_unicode(' '),\
  Usb_unicode('C'),\
  Usb_unicode('D'),\
  Usb_unicode('C') \
}

#define USB_SN_LENGTH         13
#define USB_SERIAL_NUMBER \
{\
  Usb_unicode('1'),\
  Usb_unicode('.'),\
  Usb_unicode('0'),\
  Usb_unicode('.'),\
  Usb_unicode('0'),\
  Usb_unicode('.'),\
  Usb_unicode('0'),\
  Usb_unicode('.'),\
  Usb_unicode('0'),\
  Usb_unicode('.'),\
  Usb_unicode('0'),\
  Usb_unicode('.'),\
  Usb_unicode('A') \
}

#endif

// ----------------------------------------------------------------------------

#endif /* HAL_OSUSBDEVICEDESCRIPTORSCONFIG_H_ */
