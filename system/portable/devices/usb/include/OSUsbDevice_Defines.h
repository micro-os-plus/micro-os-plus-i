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
#define DEVICE_TYPE                     0x00

#define REQUEST_DEVICE_STATUS           0x80
#define REQUEST_INTERFACE_STATUS        0x81
#define REQUEST_ENDPOINT_STATUS         0x82

#define USB_DT_CS_INTERFACE             0x24

#define USB_CDC_HEADER_TYPE             0x00
#define USB_CDC_CALL_MANAGEMENT_TYPE    0x01
#define USB_ACM_TYPE                    0x02
#define USB_CDC_UNION_TYPE              0x06

                  // Test Mode Selectors
#define TEST_J                                0x01
#define TEST_K                                0x02
#define TEST_SE0_NAK                          0x03
#define TEST_PACKET                           0x04
#define TEST_FORCE_ENABLE                     0x05

// ----- Features -------------------------------------------------------------

#define FEATURE_DEVICE_REMOTE_WAKEUP          0x01
#define FEATURE_ENDPOINT_HALT                 0x00
#define FEATURE_TEST_MODE                     0x02

// ----------------------------------------------------------------------------

#define CDC_COMM_CLASS                        0x02
#define CDC_DATA_CLASS                        0x0A

            // Interface 0 descriptor
#define INTERFACE_NB_0              0                  //! The number of this interface
#define ALTERNATE_0                 0                  //! The alt setting nb of this interface
#define NB_ENDPOINT_0               1                  //! The number of endpoints this interface has
#define INTERFACE_CLASS_0           CDC_COMM_CLASS     //! CDC ACR Com Class
#define INTERFACE_SUB_CLASS_0       0x02
#define INTERFACE_PROTOCOL_0        0x01
#define INTERFACE_INDEX_0           0

            // Interface 1 descriptor
#define INTERFACE_NB_1              1                  //! The number of this interface
#define ALTERNATE_1                 0                  //! The alt setting nb of this interface
#define NB_ENDPOINT_1               2                  //! The number of endpoints this interface has
#define INTERFACE_CLASS_1           CDC_DATA_CLASS     //! CDC ACR Data Class
#define INTERFACE_SUB_CLASS_1       0
#define INTERFACE_PROTOCOL_1        0
#define INTERFACE_INDEX_1           0

             // USB Endpoint 3 descriptor
             // Interrupt IN
#define ENDPOINT_NB_3       ( INT_EP | MSK_EP_DIR )
#define EP_IN_LENGTH_3      0x20
#define EP_INTERVAL_3       0xFF          //! Interrupt polling interval from host

             // USB Endpoint 1 descriptor
             // Bulk IN
#define ENDPOINT_NB_1       ( TX_EP | MSK_EP_DIR )
#define EP_IN_LENGTH_1_FS   0x40
#define EP_IN_LENGTH_1_HS   512
#define EP_SIZE_1_FS        EP_IN_LENGTH_1_FS
#define EP_SIZE_1_HS        EP_IN_LENGTH_1_HS
#define EP_INTERVAL_1       0x00          //! Interrupt polling interval from host

             // USB Endpoint 2 descriptor
             // Bulk OUT
#define ENDPOINT_NB_2       RX_EP
#define EP_IN_LENGTH_2_FS   0x40
#define EP_IN_LENGTH_2_HS   512
#define EP_SIZE_2_FS        EP_IN_LENGTH_2_FS
#define EP_SIZE_2_HS        EP_IN_LENGTH_2_HS
#define EP_INTERVAL_2       0x00          //! Interrupt polling interval from host


#endif /* OSUSBDEVICE_DEFINES_H_ */
