/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef OSUSBDEVICE_H_
#define OSUSBDEVICE_H_

#include "portable/devices/usb/include/OSUsbLed.h"
#include "portable/devices/usb/include/OSUsbDeviceDescriptors.h"

#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDevice_Defines.h"
#else
#error "Unsupported processor family"
#endif

#define MAX_EP_NB                       7

#define EP_CONTROL                      0
#define EP_1                            1
#define EP_2                            2
#define EP_3                            3
#define EP_4                            4
#define EP_5                            5
#define EP_6                            6
#define EP_7                            7

// USB device standard  requests decoding module
#define GET_STATUS                      0x00
#define GET_DEVICE                      0x01
#define CLEAR_FEATURE                   0x01     // see FEATURES below
#define GET_STRING                      0x03
#define SET_FEATURE                     0x03     // see FEATURES below
#define SET_ADDRESS                     0x05
#define GET_DESCRIPTOR                  0x06
#define SET_DESCRIPTOR                  0x07
#define GET_CONFIGURATION               0x08
#define SET_CONFIGURATION               0x09
#define GET_INTERFACE                   0x0A
#define SET_INTERFACE                   0x0B
#define SYNCH_FRAME                     0x0C

// Descriptor Types
#define DEVICE_DESCRIPTOR                       0x01
#define CONFIGURATION_DESCRIPTOR                0x02
#define STRING_DESCRIPTOR                       0x03
#define INTERFACE_DESCRIPTOR                    0x04
#define ENDPOINT_DESCRIPTOR                     0x05
#define DEVICE_QUALIFIER_DESCRIPTOR             0x06
#define OTHER_SPEED_CONFIGURATION_DESCRIPTOR    0x07

#define ZERO_TYPE                       0x00
#define INTERFACE_TYPE                  0x01
#define ENDPOINT_TYPE                   0x02

#define FEATURE_ENDPOINT_HALT           0x00

#define REQUEST_DEVICE_STATUS           0x80
#define REQUEST_INTERFACE_STATUS        0x81
#define REQUEST_ENDPOINT_STATUS         0x82

#define USB_DT_CS_INTERFACE             0x24

#define USB_CDC_HEADER_TYPE             0x00
#define USB_CDC_CALL_MANAGEMENT_TYPE    0x01
#define USB_ACM_TYPE                    0x02
#define USB_CDC_UNION_TYPE              0x06

class OSUsbDevice
  {
public:
  OSUsbDevice();

  // interrupt service routines
  static void interruptComServiceRoutine(void);
  static void interruptGenServiceRoutine(void);

  // read/write methods
  static unsigned char readByte(void);
  static unsigned short readWord(void);
  static unsigned long readLong(void);
  static void writeByte(unsigned char b);
  static void writeWord(unsigned short w);
  static void writeLong(unsigned long l);

  static void standardProcessRequest(void);
  static bool specificProcessReadRequest(unsigned char type,
      unsigned char request);
  static bool specificGetDescriptor(unsigned char type, unsigned char string);

  static void specificEndpointInit(unsigned char conf_nb);

  static bool usb_configure_endpoint(unsigned char num, unsigned char type,
      unsigned char dir, unsigned char size, unsigned char bank,
      unsigned char nyet);

  static bool usb_config_ep(unsigned char config0, unsigned char config1);

  static void usb_get_descriptor(void);
  static void usb_get_configuration(void);
  static void usb_set_address(void);
  static void usb_set_configuration(void);
  static void usb_clear_feature(void);
  static void usb_set_feature(void);
  static void usb_get_status(void);
  static void usb_get_interface(void);
  static void usb_set_interface(void);
  static void usb_enum_var_init(void);
  static void usb_set_return(unsigned char len, PGM_VOID_P pbuf);

  static unsigned char usb_init_device(void);
  static void usb_start_device(void);

  // implementation functions
  static void endpointSelect(unsigned char ep);
  static void endpointReset(unsigned char ep);

  static bool isInterruptReceiveSetup(void);
  static bool isInterruptReceiveSetupEnabled(void);
  static void interruptReceiveSetupEnable(void);

  static bool isInterruptReceiveOut(void);
  static bool isInterruptReceiveOutEnabled(void);
  static void interruptReceiveOutAck(void);
  static void interruptReceiveOutEnable(void);

  static void Usb_enable_stall_handshake(void);
  static void Usb_ack_receive_setup(void);
  static void Usb_enable_regulator(void);
  static void Usb_disable_uid_pin(void);
  static void Usb_force_device_mode(void);

  static void Usb_disable(void);
  static void Usb_enable(void);

  static void Usb_select_device(void);
  static void Usb_enable_vbus_interrupt(void);

  static bool Is_usb_write_enabled(void);
  static bool Is_usb_tx_ready(void);
  static void Usb_send_in(void);
  static unsigned short Usb_byte_counter(void);

  static void Usb_ack_fifocon(void);

  static bool Is_usb_vbus_transition(void);
  static bool Is_usb_vbus_interrupt_enabled(void);
  static void Usb_ack_vbus_transition(void);
  static bool Is_usb_vbus_high(void);

  static void Usb_enable_reset_interrupt(void);

  static bool Is_usb_read_control_enabled(void);
  static void Usb_send_control_in(void);
  static void Usb_ack_receive_out(void);
  static void Usb_ack_in_ready(void);
  static bool Is_usb_in_ready(void);
  static bool Is_usb_endpoint_enabled(void);

  static void Usb_disable_stall_handshake(void);
  static void Usb_reset_data_toggle(void);
  static void Usb_enable_address(void);
  static void Usb_configure_address(unsigned char addr);

  static void Usb_set_configuration_action(void);
  static void Usb_sof_action(void);
  static void Usb_wake_up_action(void);
  static void Usb_resume_action(void);
  static void Usb_suspend_action(void);
  static void Usb_reset_action(void);
  static void Usb_vbus_on_action(void);
  static void Usb_vbus_off_action(void);

  static void Usb_send_event(unsigned char event);

  static void Usb_attach(void);
  static bool Is_usb_sof(void);
  static bool Is_sof_interrupt_enabled(void);
  static void Usb_ack_sof(void);

  static bool Is_usb_suspend(void);
  static bool Is_suspend_interrupt_enabled(void);

  static void Usb_ack_suspend(void);

  static void Usb_enable_wake_up_interrupt(void);
  static void Usb_ack_wake_up(void);

  static void Usb_freeze_clock(void);
  static void Usb_unfreeze_clock(void);

  static void Usb_disable_wake_up_interrupt(void);
  static void Usb_ack_resume(void);

  static void Usb_disable_resume_interrupt(void);
  static void Usb_ack_reset(void);

  static bool Is_usb_wake_up(void);
  static bool Is_swake_up_interrupt_enabled(void);

  static bool Is_usb_resume(void);
  static bool Is_resume_interrupt_enabled(void);

  static bool Is_usb_reset(void);
  static bool Is_reset_interrupt_enabled(void);

  static bool Is_usb_id_device(void);

  static void Usb_enable_endpoint(void);
  static void Usb_allocate_memory(void);
  static bool Is_endpoint_configured(void);

  static void Usb_enable_suspend_interrupt(void);
  static void Pll_start_auto(void);
  static bool Is_pll_ready(void);

  // members
  static unsigned char data_to_transfer;
  static PGM_VOID_P pbuffer;
  static bool zlp;
  static unsigned char bmRequestType;
  static volatile unsigned short g_usb_event;
  static unsigned char endpoint_status[NB_ENDPOINTS];
  static bool usb_connected;
  static unsigned char usb_configuration_nb;

  };

#if defined(OS_CONFIG_FAMILY_AT90USB)
#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDevice_Inlines.h"
#else
#error "Unsupported processor family"
#endif

#endif /*OSUSBDEVICE_H_*/
