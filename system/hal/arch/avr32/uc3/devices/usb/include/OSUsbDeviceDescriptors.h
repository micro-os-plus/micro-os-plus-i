/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICEDESCRIPTORS_H_
#define HAL_OSUSBDEVICEDESCRIPTORS_H_

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr32/uc3/devices/usb/include/OSUsbDeviceDescriptorsConfig.h"

#include "hal/arch/avr32/lib/include/compiler.h"

// ----------------------------------------------------------------------------

#if TO_BE_PORTED

extern PROGMEM S_usb_device_descriptor usb_dev_desc;
extern PROGMEM S_usb_user_configuration2_descriptor usb_conf2_desc;
extern PROGMEM S_usb_language_id usb_user_language_id;
extern PROGMEM S_usb_manufacturer_string_descriptor
usb_user_manufacturer_string_descriptor;
extern PROGMEM S_usb_product_string_descriptor
usb_user_product_string_descriptor;
extern PROGMEM S_usb_serial_number usb_user_serial_number;

#endif

// miscellaneous


//_____ M A C R O S ________________________________________________________

#define Usb_unicode(c)                    (Usb_format_mcu_to_usb_data(16, (U16)(c)))
#define Usb_get_dev_desc_pointer()        (&(usb_dev_desc.bLength))
#define Usb_get_dev_desc_length()         (sizeof(usb_dev_desc))
#define Usb_get_conf_desc_pointer()       (&(usb_conf_desc_fs.cfg.bLength))
#define Usb_get_conf_desc_length()        (sizeof(usb_conf_desc_fs))
#define Usb_get_conf_desc_hs_pointer()    (&(usb_conf_desc_hs.cfg.bLength))
#define Usb_get_conf_desc_hs_length()     (sizeof(usb_conf_desc_hs))
#define Usb_get_conf_desc_fs_pointer()    (&(usb_conf_desc_fs.cfg.bLength))
#define Usb_get_conf_desc_fs_length()     (sizeof(usb_conf_desc_fs))
#define Usb_get_qualifier_desc_pointer()  (&(usb_qualifier_desc.bLength))
#define Usb_get_qualifier_desc_length()   (sizeof(usb_qualifier_desc))

//! USB Request
typedef struct
__attribute__((__packed__))
  {
    U8 bmRequestType; //!< Characteristics of the request
    U8 bRequest; //!< Specific request
    U16 wValue; //!< Field that varies according to request
    U16 wIndex; //!< Field that varies according to request
    U16 wLength; //!< Number of bytes to transfer if Data
  }
S_UsbRequest;

//! USB Device Descriptor
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; //!< Size of this descriptor in bytes
    unsigned char bDescriptorType; //!< DEVICE descriptor type
    unsigned short bscUSB; //!< Binay Coded Decimal Spec. release
#if TO_BE_PORTED
    unsigned char bscUSB_low; //!< Binay Coded Decimal Spec. release
    unsigned char bscUSB_high; //!< Binay Coded Decimal Spec. release
#endif
    unsigned char bDeviceClass; //!< Class code assigned by the USB
    unsigned char bDeviceSubClass; //!< Sub-class code assigned by the USB
    unsigned char bDeviceProtocol; //!< Protocol code assigned by the USB
    unsigned char bMaxPacketSize0; //!< Max packet size for EP0
    unsigned short idVendor; //!< Vendor ID. ATMEL = 0x03EB
#if TO_BE_PORTED
    unsigned char idVendor_low; //!< Vendor ID. ATMEL = 0x03EB
    unsigned char idVendor_high; //!< Vendor ID. ATMEL = 0x03EB
#endif
    unsigned short idProduct; //!< Product ID assigned by the manufacturer
#if TO_BE_PORTED
    unsigned char idProduct_low; //!< Product ID assigned by the manufacturer
    unsigned char idProduct_high; //!< Product ID assigned by the manufacturer
#endif
    unsigned short bcdDevice; //!< Device release number
#if TO_BE_PORTED
    unsigned char bcdDevice_low; //!< Device release number
    unsigned char bcdDevice_high; //!< Device release number
#endif
    unsigned char iManufacturer; //!< Index of manu. string descriptor
    unsigned char iProduct; //!< Index of prod. string descriptor
    unsigned char iSerialNumber; //!< Index of S.N.  string descriptor
    unsigned char bNumConfigurations; //!< Number of possible configurations
  }
S_usb_device_descriptor;

//! USB Configuration Descriptor
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; //!< size of this descriptor in bytes
    unsigned char bDescriptorType; //!< CONFIGURATION descriptor type
    unsigned short wTotalLength; //!< total length of data returned
#if TO_BE_PORTED
    unsigned char wTotalLength_low; //!< total length of data returned
    unsigned char wTotalLength_high; //!< total length of data returned
#endif
    unsigned char bNumInterfaces; //!< number of interfaces for this conf.
    unsigned char bConfigurationValue; //!< value for SetConfiguration resquest
    unsigned char iConfiguration; //!< index of string descriptor
    unsigned char bmAttibutes; //!< Configuration characteristics
    unsigned char MaxPower; //!< maximum power consumption
  } S_usb_configuration_descriptor;

//! USB Interface Descriptor
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; //!< size of this descriptor in bytes
    unsigned char bDescriptorType; //!< INTERFACE descriptor type
    unsigned char bInterfaceNumber; //!< Number of interface
    unsigned char bAlternateSetting; //!< value to select alternate setting
    unsigned char bNumEndpoints; //!< Number of EP except EP 0
    unsigned char bInterfaceClass; //!< Class code assigned by the USB
    unsigned char bInterfaceSubClass; //!< Sub-class code assigned by the USB
    unsigned char bInterfaceProtocol; //!< Protocol code assigned by the USB
    unsigned char iInterface; //!< Index of string descriptor
  }
S_usb_interface_descriptor;

//! USB Endpoint Descriptor
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; //!< Size of this descriptor in bytes
    unsigned char bDescriptorType; //!< ENDPOINT descriptor type
    unsigned char bEndpointAddress; //!< Address of the endpoint
    unsigned char bmAttributes; //!< Endpoint's attributes
    unsigned short wMaxPacketSize; //!< Maximum packet size for this EP
#if TO_BE_PORTED
    unsigned char wMaxPacketSize_low; //!< Maximum packet size for this EP
    unsigned char wMaxPacketSize_high; //!< Maximum packet size for this EP
#endif
    unsigned char bInterval; //!< Interval for polling EP in ms
  }
S_usb_endpoint_descriptor;

//! USB Device Qualifier Descriptor
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; //!< Size of this descriptor in bytes
    unsigned char bDescriptorType; //!< Device Qualifier descriptor type
    unsigned short bscUSB; //!< Binay Coded Decimal Spec. release
#ifdef TO_BE_PORTED
    unsigned char bscUSB_low; //!< Binay Coded Decimal Spec. release
    unsigned char bscUSB_high; //!< Binay Coded Decimal Spec. release
#endif
    unsigned char bDeviceClass; //!< Class code assigned by the USB
    unsigned char bDeviceSubClass; //!< Sub-class code assigned by the USB
    unsigned char bDeviceProtocol; //!< Protocol code assigned by the USB
    unsigned char bMaxPacketSize0; //!< Max packet size for EP0
    unsigned char bNumConfigurations; //!< Number of possible configurations
    unsigned char bReserved; //!< Reserved for future use, must be zero
  }
S_usb_device_qualifier_descriptor;

//! USB Language Descriptor
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; //!< size of this descriptor in bytes
    unsigned char bDescriptorType; //!< STRING descriptor type
    unsigned short wlangid; //!< language id
#ifdef TO_BE_PORTED
    unsigned char wlangid_low; //!< language id
    unsigned char wlangid_high; //!< language id
#endif
  }
S_usb_language_id;

//_____ U S B   M A N U F A C T U R E R   D E S C R I P T O R _______________

//! struct usb_st_manufacturer
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; // size of this descriptor in bytes
    unsigned char bDescriptorType; // STRING descriptor type
    unsigned short wstring[USB_MN_LENGTH]; // unicode characters
  }S_usb_manufacturer_string_descriptor;

//_____ U S B   P R O D U C T   D E S C R I P T O R _________________________

//! struct usb_st_product
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; // size of this descriptor in bytes
    unsigned char bDescriptorType; // STRING descriptor type
    unsigned short wstring[USB_PN_LENGTH]; // unicode characters
  }S_usb_product_string_descriptor;

//_____ U S B   S E R I A L   N U M B E R   D E S C R I P T O R _____________

//! struct usb_st_serial_number
typedef struct
__attribute__((__packed__))
  {
    unsigned char bLength; // size of this descriptor in bytes
    unsigned char bDescriptorType; // STRING descriptor type
    unsigned short wstring[USB_SN_LENGTH]; // unicode characters
  }S_usb_serial_number;

//_____ U S B   D E V I C E   C D C   D E S C R I P T O R ___________________

typedef struct
__attribute__((__packed__))
  {
    S_usb_configuration_descriptor cfg;

    S_usb_interface_descriptor ifc0;
    unsigned char CS_INTERFACE[19];
    S_usb_endpoint_descriptor ep3;
    S_usb_interface_descriptor ifc1;
    S_usb_endpoint_descriptor ep1;
    S_usb_endpoint_descriptor ep2;
  }S_usb_user_configuration_descriptor;

typedef struct
{
  S_usb_configuration_descriptor cfg;

  S_usb_interface_descriptor ifc0;
  unsigned char CS_INTERFACE[19];
  S_usb_endpoint_descriptor ep3;
  S_usb_interface_descriptor ifc1;
  S_usb_endpoint_descriptor ep1;
  S_usb_endpoint_descriptor ep2;

#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
#if true
  S_usb_interface_descriptor ifc0b;
  unsigned char CS_INTERFACEb[ 19 ];
  S_usb_endpoint_descriptor ep3b;
#endif
  S_usb_interface_descriptor ifc1b;
  S_usb_endpoint_descriptor ep1b;
  S_usb_endpoint_descriptor ep2b;
#endif
} S_usb_user_configuration2_descriptor;

// ----------------------------------------------------------------------------

// DECLARATIONS
extern const S_usb_device_descriptor usb_dev_desc;
extern const S_usb_user_configuration_descriptor usb_conf_desc;
extern const S_usb_device_qualifier_descriptor usb_qualifier_desc;
extern const S_usb_user_configuration_descriptor usb_conf_desc_fs;
extern const S_usb_user_configuration_descriptor usb_conf_desc_hs;
extern const S_usb_manufacturer_string_descriptor
    usb_user_manufacturer_string_descriptor;
extern const S_usb_product_string_descriptor usb_user_product_string_descriptor;
extern const S_usb_serial_number usb_user_serial_number;
extern const S_usb_language_id usb_user_language_id;

// ----------------------------------------------------------------------------

#endif /* HAL_OSUSBDEVICEDESCRIPTORS_H_ */
