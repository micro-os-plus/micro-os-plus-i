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

#if true

// Usb Device Descriptor
typedef struct
{
  unsigned char bLength; //!< Size of this descriptor in bytes
  unsigned char bDescriptorType; //!< DEVICE descriptor type
  //unsigned short bscUSB; //!< Binay Coded Decimal Spec. release
  unsigned char bscUSB_low; //!< Binay Coded Decimal Spec. release
  unsigned char bscUSB_high; //!< Binay Coded Decimal Spec. release
  unsigned char bDeviceClass; //!< Class code assigned by the USB
  unsigned char bDeviceSubClass; //!< Sub-class code assigned by the USB
  unsigned char bDeviceProtocol; //!< Protocol code assigned by the USB
  unsigned char bMaxPacketSize0; //!< Max packet size for EP0
  //unsigned short idVendor; //!< Vendor ID. ATMEL = 0x03EB
  unsigned char idVendor_low; //!< Vendor ID. ATMEL = 0x03EB
  unsigned char idVendor_high; //!< Vendor ID. ATMEL = 0x03EB
  //unsigned short idProduct; //!< Product ID assigned by the manufacturer
  unsigned char idProduct_low; //!< Product ID assigned by the manufacturer
  unsigned char idProduct_high; //!< Product ID assigned by the manufacturer
  //unsigned short bcdDevice; //!< Device release number
  unsigned char bcdDevice_low; //!< Device release number
  unsigned char bcdDevice_high; //!< Device release number
  unsigned char iManufacturer; //!< Index of manu. string descriptor
  unsigned char iProduct; //!< Index of prod. string descriptor
  unsigned char iSerialNumber; //!< Index of S.N.  string descriptor
  unsigned char bNumConfigurations; //!< Number of possible configurations
} S_usb_device_descriptor;

// Usb Configuration Descriptor
typedef struct
{
  unsigned char bLength; //!< size of this descriptor in bytes
  unsigned char bDescriptorType; //!< CONFIGURATION descriptor type
  //unsigned short wTotalLength; //!< total length of data returned
  unsigned char wTotalLength_low; //!< total length of data returned
  unsigned char wTotalLength_high; //!< total length of data returned
  unsigned char bNumInterfaces; //!< number of interfaces for this conf.
  unsigned char bConfigurationValue; //!< value for SetConfiguration resquest
  unsigned char iConfiguration; //!< index of string descriptor
  unsigned char bmAttibutes; //!< Configuration characteristics
  unsigned char MaxPower; //!< maximum power consumption
} S_usb_configuration_descriptor;

// Usb Interface Descriptor
typedef struct
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
} S_usb_interface_descriptor;

// Usb Endpoint Descriptor
typedef struct
{
  unsigned char bLength; //!< Size of this descriptor in bytes
  unsigned char bDescriptorType; //!< ENDPOINT descriptor type
  unsigned char bEndpointAddress; //!< Address of the endpoint
  unsigned char bmAttributes; //!< Endpoint's attributes
  //unsigned short wMaxPacketSize; //!< Maximum packet size for this EP
  unsigned char wMaxPacketSize_low; //!< Maximum packet size for this EP
  unsigned char wMaxPacketSize_high; //!< Maximum packet size for this EP
  unsigned char bInterval; //!< Interval for polling EP in ms
} S_usb_endpoint_descriptor;

// Usb Device Qualifier Descriptor
typedef struct
{
  unsigned char bLength; //!< Size of this descriptor in bytes
  unsigned char bDescriptorType; //!< Device Qualifier descriptor type
  //unsigned short bscUSB; //!< Binay Coded Decimal Spec. release
  unsigned char bscUSB_low; //!< Binay Coded Decimal Spec. release
  unsigned char bscUSB_high; //!< Binay Coded Decimal Spec. release
  unsigned char bDeviceClass; //!< Class code assigned by the USB
  unsigned char bDeviceSubClass; //!< Sub-class code assigned by the USB
  unsigned char bDeviceProtocol; //!< Protocol code assigned by the USB
  unsigned char bMaxPacketSize0; //!< Max packet size for EP0
  unsigned char bNumConfigurations; //!< Number of possible configurations
  unsigned char bReserved; //!< Reserved for future use, must be zero
} S_usb_device_qualifier_descriptor;

// Usb Language Descriptor
typedef struct
{
  unsigned char bLength; //!< size of this descriptor in bytes
  unsigned char bDescriptorType; //!< STRING descriptor type
  //unsigned short wlangid; //!< language id
  unsigned char wlangid_low; //!< language id
  unsigned char wlangid_high; //!< language id
} S_usb_language_id;

/*_____ U S B   C D C  D E S C R I P T O R __________________________________*/

typedef struct
{
  S_usb_configuration_descriptor cfg;

  S_usb_interface_descriptor ifc0;
  unsigned char CS_INTERFACE[19];
  S_usb_endpoint_descriptor ep3;
  S_usb_interface_descriptor ifc1;
  S_usb_endpoint_descriptor ep1;
  S_usb_endpoint_descriptor ep2;
} S_usb_user_configuration_descriptor;

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
  unsigned char CS_INTERFACEb[19];
  S_usb_endpoint_descriptor ep3b;
#endif
  S_usb_interface_descriptor ifc1b;
  S_usb_endpoint_descriptor ep1b;
  S_usb_endpoint_descriptor ep2b;
#endif
} S_usb_user_configuration2_descriptor;

//_____ U S B   M A N U F A C T U R E R   D E S C R I P T O R _______________


//struct usb_st_manufacturer
typedef struct
{
  unsigned char bLength; // size of this descriptor in bytes
  unsigned char bDescriptorType; // STRING descriptor type
  unsigned short wstring[USB_MN_LENGTH]; // unicode characters
} S_usb_manufacturer_string_descriptor;

//_____ U S B   P R O D U C T   D E S C R I P T O R _________________________


//struct usb_st_product
typedef struct
{
  unsigned char bLength; // size of this descriptor in bytes
  unsigned char bDescriptorType; // STRING descriptor type
  unsigned short wstring[USB_PN_LENGTH]; // unicode characters
} S_usb_product_string_descriptor;

//_____ U S B   S E R I A L   N U M B E R   D E S C R I P T O R _____________

#if !defined(OS_EXCLUDE_USBSERIALNUMBER)
//struct usb_st_serial_number
typedef struct
{
  unsigned char bLength; // size of this descriptor in bytes
  unsigned char bDescriptorType; // STRING descriptor type
  unsigned short wstring[USB_SN_LENGTH]; // unicode characters
} S_usb_serial_number;

#endif

#endif

// ----------------------------------------------------------------------------

#if true

extern PROGMEM S_usb_device_descriptor usb_dev_desc;
extern PROGMEM S_usb_user_configuration2_descriptor usb_conf2_desc;
extern PROGMEM S_usb_language_id usb_user_language_id;
extern PROGMEM S_usb_manufacturer_string_descriptor
    usb_user_manufacturer_string_descriptor;
extern PROGMEM S_usb_product_string_descriptor
    usb_user_product_string_descriptor;

#if !defined(OS_EXCLUDE_USBSERIALNUMBER)
extern PROGMEM S_usb_serial_number usb_user_serial_number;
#endif

#endif

#endif /* HAL_OSUSBDEVICEDESCRIPTORS_H_ */
