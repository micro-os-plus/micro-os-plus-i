/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbDevice.h"
#include "portable/devices/usb/include/OSUsbDeviceDescriptors.h"

#if TO_BE_PORTED

#define U_LOW(x) ((x) & 0xFF)
#define U_HIGH(x) ((x) >> 8)

#define U_WORD(x) U_LOW(x),U_HIGH(x)

// usb_user_device_descriptor
PROGMEM S_usb_device_descriptor usb_dev_desc =
  {
    sizeof( usb_dev_desc ), DEVICE_DESCRIPTOR,
    U_WORD( USB_SPECIFICATION ), DEVICE_CLASS,
    DEVICE_SUB_CLASS, DEVICE_PROTOCOL, EP_CONTROL_LENGTH,
    U_WORD( VENDOR_ID ),
    U_WORD( PRODUCT_ID ),
    U_WORD( RELEASE_NUMBER ), STRING_INDEX_MAN, STRING_INDEX_PROD,
    STRING_INDEX_SN, NB_CONFIGURATION
  };

#if defined(OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION)

// usb_user_configuration_descriptor FS
PROGMEM S_usb_user_configuration_descriptor usb_conf_desc =
  {
      { sizeof( S_usb_configuration_descriptor ), CONFIGURATION_DESCRIPTOR,
        U_WORD(sizeof(usb_conf_desc)),
        NB1_INTERFACE, CONF1_NB, CONF_INDEX, CONF_ATTRIBUTES, MAX_POWER}
    ,
      { sizeof( S_usb_interface_descriptor ), INTERFACE_DESCRIPTOR, IF0_NB,
        ALTERNATE0, NB_ENDPOINT0, IF0_CLASS, IF0_SUB_CLASS,
        IF0_PROTOCOL, STRING_INDEX_IF0}
    ,
    // CS_INTERFACE

      { 0x05, USB_DT_CS_INTERFACE, USB_CDC_HEADER_TYPE, // Header Functional Descriptor
        0x10, 0x01, //  version 10.01

        0x05, USB_DT_CS_INTERFACE, USB_CDC_CALL_MANAGEMENT_TYPE, // Call Management Functional Descriptor
        0x03,
        // D1 = device can send/receive call management over 
        //  a Data Class interface
        // D0 = device handles call management itself 
        IF1_NB, // 0x01 - interface used for call management   

        0x04, USB_DT_CS_INTERFACE, USB_ACM_TYPE, // Abstract Control Management Functional Descriptor
        0x06,
        // D2 = break
        // D1 = Set line coding, control...

        0x05, USB_DT_CS_INTERFACE, USB_CDC_UNION_TYPE, // Union Functional Descriptor
        IF0_NB, // 0x00 = controlling interface
        IF1_NB // 0x01 = slave interface
      }
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_3,
        EP_ATTRIBUTES_3, U_WORD( EP_SIZE_3 ), EP_INTERVAL_3}
    ,
      { sizeof( S_usb_interface_descriptor ), INTERFACE_DESCRIPTOR, IF1_NB,
        ALTERNATE1, NB_ENDPOINT1, IF1_CLASS, IF1_SUB_CLASS,
        IF1_PROTOCOL, STRING_INDEX_IF1}
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_1,
        EP_ATTRIBUTES_1, U_WORD( EP_SIZE_1 ), EP_INTERVAL_1}
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_2,
        EP_ATTRIBUTES_2, U_WORD( EP_SIZE_2 ), EP_INTERVAL_2}
  };
#endif

// second configuration
PROGMEM S_usb_user_configuration2_descriptor usb_conf2_desc =
  {
      { sizeof( S_usb_configuration_descriptor ), CONFIGURATION_DESCRIPTOR,
        U_WORD(sizeof(usb_conf2_desc)),
        NB2_INTERFACE, CONF2_NB, CONF_INDEX, CONF_ATTRIBUTES, MAX_POWER}
    ,
      { sizeof( S_usb_interface_descriptor ), INTERFACE_DESCRIPTOR, IF0_NB,
        ALTERNATE0, NB_ENDPOINT0, IF0_CLASS, IF0_SUB_CLASS,
        IF0_PROTOCOL, STRING_INDEX_IF0}
    ,
    // CS_INTERFACE

      { 0x05, USB_DT_CS_INTERFACE, USB_CDC_HEADER_TYPE, // Header Functional Descriptor
        0x10, 0x01, //  version 10.01

        0x05, USB_DT_CS_INTERFACE, USB_CDC_CALL_MANAGEMENT_TYPE, // Call Management Functional Descriptor
        0x03,
        // D1 = device can send/receive call management over 
        //  a Data Class interface
        // D0 = device handles call management itself 
        IF1_NB, // 0x01 - interface used for call management   

        0x04, USB_DT_CS_INTERFACE, USB_ACM_TYPE, // Abstract Control Management Functional Descriptor
        0x06,
        // D2 = break
        // D1 = Set line coding, control...

        0x05, USB_DT_CS_INTERFACE, USB_CDC_UNION_TYPE, // Union Functional Descriptor
        IF0_NB, // 0x00 = controlling interface
        IF1_NB // 0x01 = slave interface
      }
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_3,
        EP_ATTRIBUTES_3, U_WORD( EP_SIZE_3 ), EP_INTERVAL_3}
    ,
      { sizeof( S_usb_interface_descriptor ), INTERFACE_DESCRIPTOR, IF1_NB,
        ALTERNATE1, NB_ENDPOINT1, IF1_CLASS, IF1_SUB_CLASS,
        IF1_PROTOCOL, STRING_INDEX_IF1}
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_1,
        EP_ATTRIBUTES_1, U_WORD( EP_SIZE_1 ), EP_INTERVAL_1}
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_2,
        EP_ATTRIBUTES_2, U_WORD( EP_SIZE_2 ), EP_INTERVAL_2}

#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
    ,
      { sizeof( S_usb_interface_descriptor ), INTERFACE_DESCRIPTOR, IF0b_NB,
        ALTERNATE0b, NB_ENDPOINT0b, IF0b_CLASS, IF0b_SUB_CLASS,
        IF0b_PROTOCOL, STRING_INDEX_IF0b}
    ,
    // CS_INTERFACE

      { 0x05, USB_DT_CS_INTERFACE, USB_CDC_HEADER_TYPE, // Header Functional Descriptor
        0x10, 0x01, //  10.01

        0x05, USB_DT_CS_INTERFACE, USB_CDC_CALL_MANAGEMENT_TYPE, // Call Management Functional Descriptor
        0x03,
        // D1 = device can send/receive call management over 
        //  a Data Class interface
        // D0 = device handles call management itself 
        IF1b_NB, // 0x01 - interface used for call management   

        0x04, USB_DT_CS_INTERFACE, USB_ACM_TYPE, // Abstract Control Management Functional Descriptor
        0x06,
        // D2 = break
        // D1 = Set line coding, control...

        0x05, USB_DT_CS_INTERFACE, USB_CDC_UNION_TYPE, // Union Functional Descriptor
        IF0b_NB, // 0x02 = controlling interface
        IF1b_NB // 0x03 = slave interface
      }
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_3b,
        EP_ATTRIBUTES_3b, U_WORD( EP_SIZE_3b ), EP_INTERVAL_3b}
    ,
      { sizeof( S_usb_interface_descriptor ), INTERFACE_DESCRIPTOR, IF1b_NB,
        ALTERNATE1b, NB_ENDPOINT1b, IF1b_CLASS, IF1b_SUB_CLASS,
        IF1b_PROTOCOL, STRING_INDEX_IF1b}
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_1b,
        EP_ATTRIBUTES_1b, U_WORD( EP_SIZE_1b ), EP_INTERVAL_1b}
    ,
      { sizeof( S_usb_endpoint_descriptor ), ENDPOINT_DESCRIPTOR, EP_NB_2b,
        EP_ATTRIBUTES_2b, U_WORD( EP_SIZE_2b ), EP_INTERVAL_2b}
#endif

  };

// usb_user_language_id

PROGMEM S_usb_language_id usb_user_language_id =
  {
    sizeof( usb_user_language_id ), STRING_DESCRIPTOR,
    U_WORD( LANGUAGE_ID )
  };

// usb_user_manufacturer_string_descriptor
PROGMEM S_usb_manufacturer_string_descriptor
usb_user_manufacturer_string_descriptor =
  {
    sizeof( usb_user_manufacturer_string_descriptor ), STRING_DESCRIPTOR,
    USB_MANUFACTURER_NAME
  };

// usb_user_product_string_descriptor

PROGMEM S_usb_product_string_descriptor usb_user_product_string_descriptor =
  {
    sizeof( usb_user_product_string_descriptor ), STRING_DESCRIPTOR,
    USB_PRODUCT_NAME
  };

// usb_user_serial_number

PROGMEM S_usb_serial_number usb_user_serial_number =
  {
    sizeof( usb_user_serial_number ), STRING_DESCRIPTOR, USB_SERIAL_NUMBER
  };

#else

//_____ D E F I N I T I O N S ______________________________________________

// usb_user_device_descriptor
const S_usb_device_descriptor usb_dev_desc =
{
  sizeof(S_usb_device_descriptor),
  DEVICE_DESCRIPTOR,
  Usb_format_mcu_to_usb_data(16, USB_SPECIFICATION),
  DEVICE_CLASS,
  DEVICE_SUB_CLASS,
  DEVICE_PROTOCOL,
  EP_CONTROL_LENGTH,
  Usb_format_mcu_to_usb_data(16, VENDOR_ID),
  Usb_format_mcu_to_usb_data(16, PRODUCT_ID),
  Usb_format_mcu_to_usb_data(16, RELEASE_NUMBER),
  STRING_INDEX_MAN, // ???
  STRING_INDEX_PROD, // ???
  STRING_INDEX_SN, // ???
  NB_CONFIGURATION
};


// usb_user_configuration_descriptor FS
const S_usb_user_configuration_descriptor usb_conf_desc_fs =
{
   {  sizeof(S_usb_configuration_descriptor)
   ,  CONFIGURATION_DESCRIPTOR
   ,  Usb_format_mcu_to_usb_data(16, sizeof(S_usb_user_configuration_descriptor))
   ,  NB_INTERFACE
   ,  CONF_NB
   ,  CONF_INDEX
   ,  CONF_ATTRIBUTES
   ,  MAX_POWER
   }
,  {  sizeof(S_usb_interface_descriptor)
   ,  INTERFACE_DESCRIPTOR
   ,  IF0_NB
   ,  ALTERNATE0
   ,  NB_ENDPOINT0
   ,  IF0_CLASS
   ,  IF0_SUB_CLASS
   ,  IF0_PROTOCOL
   ,  STRING_INDEX_IF0
   }
,  { 0x05,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x00,       // Header Functional Descriptor
     0x10, 0x01, // USB Class Definitions for Communication Devices Specification release number in
                 // binary-coded decimal.

     0x05,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x01,       // Call Management Functional Descriptor
     0x03,       // The capabilities that this configuration supports:
                 // - Device handles call management itself.
                 // - Device can send/receive call management information over a Data Class interface.
     0x01,       // Interface number of Data Class interface optionally used for call management.

     0x04,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x02,       // Abstract Control Management Functional Descriptor.
     0x06,       // Abstract Control Management functional descriptor subtype:
                 // - Union Functional descriptor

     0x05,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x06,       // Union Functional descriptor
     0x00,       // The interface number of the Communication or Data Class interface, designated as
                 // the master or controlling interface for the union.
     0x01        // Interface number of first slave or associated interface in the union.
   }
,  {  sizeof(S_usb_endpoint_descriptor)
   ,  ENDPOINT_DESCRIPTOR
   ,  EP_NB_3
   ,  EP_ATTRIBUTES_3
   ,  Usb_format_mcu_to_usb_data(16, EP_SIZE_3)
   ,  EP_INTERVAL_3
   }
,  {  sizeof(S_usb_interface_descriptor)
   ,  INTERFACE_DESCRIPTOR
   ,  IF1_NB
   ,  ALTERNATE1
   ,  NB_ENDPOINT1
   ,  IF1_CLASS
   ,  IF1_SUB_CLASS
   ,  IF1_PROTOCOL
   ,  STRING_INDEX_IF1
   }
,  {  sizeof(S_usb_endpoint_descriptor)
   ,  ENDPOINT_DESCRIPTOR
   ,  EP_NB_1
   ,  EP_ATTRIBUTES_1
   ,  Usb_format_mcu_to_usb_data(16, EP_SIZE_1_FS)
   ,  EP_INTERVAL_1
   }
,  {  sizeof(S_usb_endpoint_descriptor)
   ,  ENDPOINT_DESCRIPTOR
   ,  EP_NB_2
   ,  EP_ATTRIBUTES_2
   ,  Usb_format_mcu_to_usb_data(16, EP_SIZE_2_FS)
   ,  EP_INTERVAL_2
   }
};


#if (USB_HIGH_SPEED_SUPPORT==ENABLED)


// usb_user_configuration_descriptor HS
const S_usb_user_configuration_descriptor usb_conf_desc_hs =
{
   {  sizeof(S_usb_configuration_descriptor)
   ,  CONFIGURATION_DESCRIPTOR
   ,  Usb_format_mcu_to_usb_data(16, sizeof(S_usb_user_configuration_descriptor))
   ,  NB_INTERFACE
   ,  CONF_NB
   ,  CONF_INDEX
   ,  CONF_ATTRIBUTES
   ,  MAX_POWER
   }
,  {  sizeof(S_usb_interface_descriptor)
   ,  INTERFACE_DESCRIPTOR
   ,  INTERFACE_NB_0
   ,  ALTERNATE_0
   ,  NB_ENDPOINT_0
   ,  INTERFACE_CLASS_0
   ,  INTERFACE_SUB_CLASS_0
   ,  INTERFACE_PROTOCOL_0
   ,  INTERFACE_INDEX_0
   }
,  { 0x05,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x00,       // Header Functional Descriptor
     0x10, 0x01, // USB Class Definitions for Communication Devices Specification release number in
                 // binary-coded decimal.

     0x05,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x01,       // Call Management Functional Descriptor
     0x03,       // The capabilities that this configuration supports:
                 // - Device handles call management itself.
                 // - Device can send/receive call management information over a Data Class interface.
     0x01,       // Interface number of Data Class interface optionally used for call management.

     0x04,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x02,       // Abstract Control Management Functional Descriptor.
     0x06,       // Abstract Control Management functional descriptor subtype:
                 // - Union Functional descriptor

     0x05,       // Size of structure
                 // -----------------
     0x24,       // CS_INTERFACE
     0x06,       // Union Functional descriptor
     0x00,       // The interface number of the Communication or Data Class interface, designated as
                 // the master or controlling interface for the union.
     0x01        // Interface number of first slave or associated interface in the union.
   }
,  {  sizeof(S_usb_endpoint_descriptor)
   ,  ENDPOINT_DESCRIPTOR
   ,  ENDPOINT_NB_3
   ,  EP_ATTRIBUTES_3
   ,  Usb_format_mcu_to_usb_data(16, EP_SIZE_3)
   ,  EP_INTERVAL_3
   }
,  {  sizeof(S_usb_interface_descriptor)
   ,  INTERFACE_DESCRIPTOR
   ,  INTERFACE_NB_1
   ,  ALTERNATE_1
   ,  NB_ENDPOINT_1
   ,  INTERFACE_CLASS_1
   ,  INTERFACE_SUB_CLASS_1
   ,  INTERFACE_PROTOCOL_1
   ,  INTERFACE_INDEX_1
   }
,  {  sizeof(S_usb_endpoint_descriptor)
   ,  ENDPOINT_DESCRIPTOR
   ,  ENDPOINT_NB_1
   ,  EP_ATTRIBUTES_1
   ,  Usb_format_mcu_to_usb_data(16, EP_SIZE_1_HS)
   ,  EP_INTERVAL_1
   }
,  {  sizeof(S_usb_endpoint_descriptor)
   ,  ENDPOINT_DESCRIPTOR
   ,  ENDPOINT_NB_2
   ,  EP_ATTRIBUTES_2
   ,  Usb_format_mcu_to_usb_data(16, EP_SIZE_2_HS)
   ,  EP_INTERVAL_2
   }
};


// usb_qualifier_desc FS
const S_usb_device_qualifier_descriptor usb_qualifier_desc =
{
  sizeof(S_usb_device_qualifier_descriptor),
  DEVICE_QUALIFIER_DESCRIPTOR,
  Usb_format_mcu_to_usb_data(16, USB_SPECIFICATION),
  DEVICE_CLASS,
  DEVICE_SUB_CLASS,
  DEVICE_PROTOCOL,
  EP_CONTROL_LENGTH,
  NB_CONFIGURATION,
  0
};
#endif


// usb_user_language_id
const S_usb_language_id usb_user_language_id =
{
  sizeof(S_usb_language_id),
  STRING_DESCRIPTOR,
  Usb_format_mcu_to_usb_data(16, LANGUAGE_ID)
};


// usb_user_manufacturer_string_descriptor
const S_usb_manufacturer_string_descriptor usb_user_manufacturer_string_descriptor =
{
  sizeof(S_usb_manufacturer_string_descriptor),
  STRING_DESCRIPTOR,
  USB_MANUFACTURER_NAME
};


// usb_user_product_string_descriptor
const S_usb_product_string_descriptor usb_user_product_string_descriptor =
{
  sizeof(S_usb_product_string_descriptor),
  STRING_DESCRIPTOR,
  USB_PRODUCT_NAME
};


// usb_user_serial_number
const S_usb_serial_number usb_user_serial_number =
{
  sizeof(S_usb_serial_number),
  STRING_DESCRIPTOR,
  USB_SERIAL_NUMBER
};

#endif

#endif


