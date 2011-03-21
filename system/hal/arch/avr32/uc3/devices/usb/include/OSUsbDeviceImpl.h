/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICEIMPL_H_
#define HAL_OSUSBDEVICEIMPL_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/devices/usb/include/OSUsbLed.h"

#include "portable/devices/usb/include/OSUsbDevice_Defines.h"
#include "hal/arch/avr32/uc3/devices/usb/include/OSUsbDevice_Defines.h"

#include "hal/arch/avr32/uc3/devices/usb/include/OSUsbDeviceDescriptors.h"

// ----------------------------------------------------------------------------

#define PGM_VOID_P      void *

// ----------------------------------------------------------------------------

#define MSK_UADD              0x7F
#define MSK_EP_NBR                            0x0F
#define MSK_EP_DIR                            0x80

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

#define AVR32_USBB_UECFGX_EPBK_SINGLE                       AVR32_USBB_UECFG0_EPBK_SINGLE
#define AVR32_USBB_UECFGX_EPBK_DOUBLE                       AVR32_USBB_UECFG0_EPBK_DOUBLE
#define AVR32_USBB_UECFGX_EPBK_TRIPLE                       AVR32_USBB_UECFG0_EPBK_TRIPLE

#define SINGLE_BANK              AVR32_USBB_UECFGX_EPBK_SINGLE
#define DOUBLE_BANK              AVR32_USBB_UECFGX_EPBK_DOUBLE
#define TRIPLE_BANK              AVR32_USBB_UECFGX_EPBK_TRIPLE

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

///////////////////////
// from AVR32 framework
///////////////////////

//extern UnionVPtr pep_fifo[AVR32_USBB_EPT_NUM];

#define AVR32_USBB_UESTAX_TXINI_MASK                        AVR32_USBB_UESTA0_TXINI_MASK
#define AVR32_USBB_UESTAX_RXSTPI_MASK                       AVR32_USBB_UESTA0_RXSTPI_MASK
#define AVR32_USBB_UECONX_RXSTPE_MASK                       AVR32_USBB_UECON0_RXSTPE_MASK
#define AVR32_USBB_UECONXSET_RXSTPES_MASK                   AVR32_USBB_UECON0SET_RXSTPES_MASK
#define AVR32_USBB_UESTAX_RXOUTI_MASK                       AVR32_USBB_UESTA0_RXOUTI_MASK
#define AVR32_USBB_UECONX_RXOUTE_MASK                       AVR32_USBB_UECON0_RXOUTE_MASK
#define AVR32_USBB_UESTAXCLR_RXOUTIC_MASK                   AVR32_USBB_UESTA0CLR_RXOUTIC_MASK
#define AVR32_USBB_UECONXSET_RXOUTES_MASK                   AVR32_USBB_UECON0SET_RXOUTES_MASK
#define AVR32_USBB_UECONXCLR_FIFOCONC_MASK                  AVR32_USBB_UECON0CLR_FIFOCONC_MASK
#define AVR32_USBB_UESTAXCLR_TXINIC_MASK                    AVR32_USBB_UESTA0CLR_TXINIC_MASK
#define AVR32_USBB_UESTAX_BYCT_MASK                         AVR32_USBB_UESTA0_BYCT_MASK
#define AVR32_USBB_UECFGX_EPSIZE_MASK                       AVR32_USBB_UECFG0_EPSIZE_MASK
#define AVR32_USBB_UESTAX_BYCT_MASK                         AVR32_USBB_UESTA0_BYCT_MASK
#define AVR32_USBB_UESTAX_RWALL_MASK                        AVR32_USBB_UESTA0_RWALL_MASK
#define AVR32_USBB_UECONXSET_STALLRQS_MASK                  AVR32_USBB_UECON0SET_STALLRQS_MASK
#define AVR32_USBB_UESTAXCLR_RXSTPIC_MASK                   AVR32_USBB_UESTA0CLR_RXSTPIC_MASK
#define AVR32_USBB_UECONXSET_RSTDTS_MASK                    AVR32_USBB_UECON0SET_RSTDTS_MASK
#define AVR32_USBB_UECONXCLR_STALLRQC_MASK                  AVR32_USBB_UECON0CLR_STALLRQC_MASK
#define AVR32_USBB_UECFGX_ALLOC_MASK                        AVR32_USBB_UECFG0_ALLOC_MASK
#define AVR32_USBB_UESTAX_CFGOK_MASK                        AVR32_USBB_UESTA0_CFGOK_MASK
#define AVR32_USBB_UECFGX_EPTYPE_MASK                       AVR32_USBB_UECFG0_EPTYPE_MASK
#define AVR32_USBB_UECFGX_EPDIR_MASK                        AVR32_USBB_UECFG0_EPDIR_MASK
#define AVR32_USBB_UECFGX_EPSIZE_MASK                       AVR32_USBB_UECFG0_EPSIZE_MASK
#define AVR32_USBB_UECFGX_EPBK_MASK                         AVR32_USBB_UECFG0_EPBK_MASK
#define AVR32_USBB_UECFGX_EPTYPE_OFFSET                     AVR32_USBB_UECFG0_EPTYPE_OFFSET
#define AVR32_USBB_UECFGX_EPDIR_OFFSET                      AVR32_USBB_UECFG0_EPDIR_OFFSET
#define AVR32_USBB_UECFGX_EPSIZE_OFFSET                     AVR32_USBB_UECFG0_EPSIZE_OFFSET
#define AVR32_USBB_UECFGX_EPBK_OFFSET                       AVR32_USBB_UECFG0_EPBK_OFFSET
#define AVR32_USBB_UESTAXCLR_NAKOUTIC_MASK                  AVR32_USBB_UESTA0CLR_NAKOUTIC_MASK
#define AVR32_USBB_UESTAX_NAKOUTI_MASK                      AVR32_USBB_UESTA0_NAKOUTI_MASK
#define AVR32_USBB_UECONX_STALLRQ_MASK                      AVR32_USBB_UECON0_STALLRQ_MASK
#define AVR32_USBB_UDINTESET_EP0INTES_MASK                  0x00001000

#define AVR32_USBB_uerst                  (AVR32_USBB.uerst)
#define AVR32_USBB_uestax(x)              ((&AVR32_USBB.uesta0)[(x)])
#define AVR32_USBB_ueconx(x)              ((&AVR32_USBB.uecon0)[(x)])
#define AVR32_USBB_ueconxset(x)           ((&AVR32_USBB.uecon0set)[(x)])
#define AVR32_USBB_uestaxclr(x)           ((&AVR32_USBB.uesta0clr)[(x)])
#define AVR32_USBB_ueconxclr(x)           ((&AVR32_USBB.uecon0clr)[(x)])
#define AVR32_USBB_uecfgx(x)              ((&AVR32_USBB.uecfg0)[(x)])
#define AVR32_USBB_usbcon                 (AVR32_USBB.usbcon)
#define AVR32_USBB_usbsta                 (AVR32_USBB.usbsta)
#define AVR32_USBB_udinteset              (AVR32_USBB.udinteset)
#define AVR32_USBB_usbstaclr              (AVR32_USBB.usbstaclr)
#define AVR32_USBB_udcon                  (AVR32_USBB.udcon)
#define AVR32_USBB_udint                  (AVR32_USBB.udint)
#define AVR32_USBB_udinte                 (AVR32_USBB.udinte)
#define AVR32_USBB_udintclr               (AVR32_USBB.udintclr)
#define AVR32_USBB_udinteclr              (AVR32_USBB.udinteclr)

// resets the selected endpoint
#define AVR32_usb_reset_endpoint(ep)                    (Set_bits(AVR32_USBB_uerst, AVR32_USBB_UERST_EPRST0_MASK << (ep)),\
                                                 Clr_bits(AVR32_USBB_uerst, AVR32_USBB_UERST_EPRST0_MASK << (ep)))
// tests if SETUP received
#define AVR32_is_usb_setup_received()                   (Tst_bits(AVR32_USBB_uestax(EP_CONTROL), AVR32_USBB_UESTAX_RXSTPI_MASK))
// tests if SETUP received interrupt is enabled
#define AVR32_is_usb_setup_received_interrupt_enabled() (Tst_bits(AVR32_USBB_ueconx(EP_CONTROL), AVR32_USBB_UECONX_RXSTPE_MASK))
// enables SETUP received interrupt
#define AVR32_usb_enable_setup_received_interrupt()     (AVR32_USBB_ueconxset(EP_CONTROL) = AVR32_USBB_UECONXSET_RXSTPES_MASK)
// tests if OUT received
#define AVR32_is_usb_out_received(ep)                   (Tst_bits(AVR32_USBB_uestax(ep), AVR32_USBB_UESTAX_RXOUTI_MASK))
// tests if OUT received interrupt is enabled
#define AVR32_is_usb_out_received_interrupt_enabled(ep) (Tst_bits(AVR32_USBB_ueconx(ep), AVR32_USBB_UECONX_RXOUTE_MASK))
// acks OUT received
#define AVR32_usb_ack_out_received(ep)                  (AVR32_USBB_uestaxclr(ep) = AVR32_USBB_UESTAXCLR_RXOUTIC_MASK)
// enables OUT received interrupt
#define AVR32_usb_enable_out_received_interrupt(ep)     (AVR32_USBB_ueconxset(ep) = AVR32_USBB_UECONXSET_RXOUTES_MASK)
// returns the byte count
#define AVR32_usb_byte_count(ep)                        (Rd_bitfield(AVR32_USBB_uestax(ep), AVR32_USBB_UESTAX_BYCT_MASK))
// gets the configured selected endpoint size
#define AVR32_usb_get_endpoint_size(ep)                 (8 << Rd_bitfield(AVR32_USBB_uecfgx(ep), AVR32_USBB_UECFGX_EPSIZE_MASK))
// clears FIFOCON bit
#define AVR32_usb_ack_fifocon(ep)                       (AVR32_USBB_ueconxclr(ep) = AVR32_USBB_UECONXCLR_FIFOCONC_MASK)
// frees current bank for OUT endpoint
#define AVR32_usb_free_out(ep)                          (AVR32_usb_ack_fifocon(ep))
// acks OUT received and frees current bank
#define AVR32_usb_ack_out_received_free(ep)             (AVR32_usb_ack_out_received(ep), AVR32_usb_free_out(ep))
// sends current bank for IN endpoint
#define AVR32_usb_send_in(ep)                           (AVR32_usb_ack_fifocon(ep))
// acks IN ready
#define AVR32_usb_ack_in_ready(ep)                      (AVR32_USBB_uestaxclr(ep) = AVR32_USBB_UESTAXCLR_TXINIC_MASK)
// tests if IN ready
#define AVR32_is_usb_in_ready(ep)                       (Tst_bits(AVR32_USBB_uestax(ep), AVR32_USBB_UESTAX_TXINI_MASK))
// acks IN ready and sends current bank
#define AVR32_usb_ack_in_ready_send(ep)                 (AVR32_usb_ack_in_ready(ep), AVR32_usb_send_in(ep))
// enables the STALL handshake
#define AVR32_usb_enable_stall_handshake(ep)            (AVR32_USBB_ueconxset(ep) = AVR32_USBB_UECONXSET_STALLRQS_MASK)
// acks SETUP received
#define AVR32_usb_ack_setup_received_free()             (AVR32_USBB_uestaxclr(EP_CONTROL) = AVR32_USBB_UESTAXCLR_RXSTPIC_MASK)
// Disable external USB_ID pin (ignored by USB)
#define AVR32_usb_disable_id_pin()                      (Clr_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_UIDE_MASK))
// Disable external USB_ID pin (ignored by USB)
#define Usb_disable_id_pin()            (Clr_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_UIDE_MASK))
// Disable external USB_ID pin and force device mode
#define AVR32_usb_force_device_mode()                   (Set_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_UIMOD_MASK), Usb_disable_id_pin())
// Enable USB macro
#define AVR32_usb_enable()                  (Set_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_USBE_MASK))
// Disable USB macro
#define AVR32_usb_disable()                 (Clr_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_USBE_MASK))
// Enable vbus interrupt.
#define AVR32_usb_enable_vbus_interrupt()   (Set_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_VBUSTE_MASK))
// tests if endpoint write allowed
#define AVR32_is_usb_write_enabled(ep)                  (Tst_bits(AVR32_USBB_uestax(ep), AVR32_USBB_UESTAX_RWALL_MASK))
// sends current bank for IN endpoint
#define AVR32_usb_send_in(ep)                           (AVR32_usb_ack_fifocon(ep))
// Check if a vbus transition occurs.
#define AVR32_is_usb_vbus_transition()          (Tst_bits(AVR32_USBB_usbsta, AVR32_USBB_USBSTA_VBUSTI_MASK))
// Check if VBUS interrupt is enabled.
#define AVR32_is_usb_vbus_interrupt_enabled()         (Tst_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_VBUSTE_MASK))
#define AVR32_usb_ack_vbus_transition()     (AVR32_USBB_usbstaclr = AVR32_USBB_USBSTACLR_VBUSTIC_MASK)
#define AVR32_is_usb_vbus_high()            (Tst_bits(AVR32_USBB_usbsta, AVR32_USBB_USBSTA_VBUS_MASK))
// enables USB reset interrupt
#define AVR32_usb_enable_reset_interrupt()              (AVR32_USBB_udinteset = AVR32_USBB_UDINTESET_EORSTES_MASK)
// tests if IN ready on control endpoint
#define AVR32_is_usb_control_in_ready()                 (Tst_bits(AVR32_USBB_uestax(EP_CONTROL), AVR32_USBB_UESTAX_TXINI_MASK))
// acks IN ready on control endpoint and sends current bank
#define AVR32_usb_ack_control_in_ready_send()           (AVR32_USBB_uestaxclr(EP_CONTROL) = AVR32_USBB_UESTAXCLR_TXINIC_MASK)
// acks IN ready
#define AVR32_usb_ack_in_ready(ep)                      (AVR32_USBB_uestaxclr(ep) = AVR32_USBB_UESTAXCLR_TXINIC_MASK)
// tests if the selected endpoint is enabled
#define AVR32_is_usb_endpoint_enabled(ep)               (Tst_bits(AVR32_USBB_uerst, AVR32_USBB_UERST_EPEN0_MASK << (ep)))
//! disables the STALL handshake
#define AVR32_usb_disable_stall_handshake(ep)           (AVR32_USBB_ueconxclr(ep) = AVR32_USBB_UECONXCLR_STALLRQC_MASK)
//! resets the data toggle sequence
#define AVR32_usb_reset_data_toggle(ep)                 (AVR32_USBB_ueconxset(ep) = AVR32_USBB_UECONXSET_RSTDTS_MASK)
//! enables USB device address
#define AVR32_usb_enable_address()                      (Set_bits(AVR32_USBB_udcon, AVR32_USBB_UDCON_ADDEN_MASK))
// configures the USB device address
#define AVR32_usb_configure_address(addr)               (Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_UADD_MASK, addr))
//! attaches to USB bus
#define AVR32_usb_attach()                              (Clr_bits(AVR32_USBB_udcon, AVR32_USBB_UDCON_DETACH_MASK))
//! tests if Start-of-Frame occurs
#define AVR32_is_usb_sof()                              (Tst_bits(AVR32_USBB_udint, AVR32_USBB_UDINT_SOF_MASK))
#define AVR32_is_usb_sof_interrupt_enabled()            (Tst_bits(AVR32_USBB_udinte, AVR32_USBB_UDINTE_SOFE_MASK))
//! acks Start-of-Frame
#define AVR32_usb_ack_sof()                             (AVR32_USBB_udintclr = AVR32_USBB_UDINTCLR_SOFC_MASK)
//! tests if Suspend state detected
#define AVR32_is_usb_suspend()                          (Tst_bits(AVR32_USBB_udint, AVR32_USBB_UDINT_SUSP_MASK))
#define AVR32_is_usb_suspend_interrupt_enabled()        (Tst_bits(AVR32_USBB_udinte, AVR32_USBB_UDINTE_SUSPE_MASK))
//! acks Suspend
#define AVR32_usb_ack_suspend()                         (AVR32_USBB_udintclr = AVR32_USBB_UDINTCLR_SUSPC_MASK)
//! enables wake-up interrupt
#define AVR32_usb_enable_wake_up_interrupt()            (AVR32_USBB_udinteset = AVR32_USBB_UDINTESET_WAKEUPES_MASK)
//! acks wake-up
#define AVR32_usb_ack_wake_up()                         (AVR32_USBB_udintclr = AVR32_USBB_UDINTCLR_WAKEUPC_MASK)
//! Stop (freeze) internal USB clock
#define AVR32_usb_freeze_clock()            (Set_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_FRZCLK_MASK))
#define AVR32_usb_unfreeze_clock()          (Clr_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_FRZCLK_MASK))
#define AVR32_is_usb_clock_frozen()         (Tst_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_FRZCLK_MASK))
//! disables wake-up interrupt
#define AVR32_usb_disable_wake_up_interrupt()           (AVR32_USBB_udinteclr = AVR32_USBB_UDINTECLR_WAKEUPEC_MASK)
//! acks resume
#define AVR32_usb_ack_resume()                          (AVR32_USBB_udintclr = AVR32_USBB_UDINTCLR_EORSMC_MASK)
//! disables resume interrupt
#define AVR32_usb_disable_resume_interrupt()            (AVR32_USBB_udinteclr = AVR32_USBB_UDINTECLR_EORSMEC_MASK)
//! acks USB reset
#define AVR32_usb_ack_reset()                           (AVR32_USBB_udintclr = AVR32_USBB_UDINTCLR_EORSTC_MASK)
//! tests if wake-up occurs
#define AVR32_is_usb_wake_up()                          (Tst_bits(AVR32_USBB_udint, AVR32_USBB_UDINT_WAKEUP_MASK))
#define AVR32_is_usb_wake_up_interrupt_enabled()        (Tst_bits(AVR32_USBB_udinte, AVR32_USBB_UDINTE_WAKEUPE_MASK))
//! tests if resume occurs
#define AVR32_is_usb_resume()                           (Tst_bits(AVR32_USBB_udint, AVR32_USBB_UDINT_EORSM_MASK))
#define AVR32_is_usb_resume_interrupt_enabled()         (Tst_bits(AVR32_USBB_udinte, AVR32_USBB_UDINTE_EORSME_MASK))
//! tests if USB reset occurs
#define AVR32_is_usb_reset()                            (Tst_bits(AVR32_USBB_udint, AVR32_USBB_UDINT_EORST_MASK))
#define AVR32_is_usb_reset_interrupt_enabled()          (Tst_bits(AVR32_USBB_udinte, AVR32_USBB_UDINTE_EORSTE_MASK))
#define AVR32_is_usb_id_device()            (Tst_bits(AVR32_USBB_usbsta, AVR32_USBB_USBSTA_ID_MASK))
//! enables the selected endpoint
#define AVR32_usb_enable_endpoint(ep)                   (Set_bits(AVR32_USBB_uerst, AVR32_USBB_UERST_EPEN0_MASK << (ep)))
//! allocates the configuration x in DPRAM memory
#define AVR32_usb_allocate_memory(ep)                   (Set_bits(AVR32_USBB_uecfgx(ep), AVR32_USBB_UECFGX_ALLOC_MASK))
//! tests if current endpoint is configured
#define AVR32_is_usb_endpoint_configured(ep)            (Tst_bits(AVR32_USBB_uestax(ep), AVR32_USBB_UESTAX_CFGOK_MASK))
//! enables suspend state interrupt
#define AVR32_usb_enable_suspend_interrupt()            (AVR32_USBB_udinteset = AVR32_USBB_UDINTESET_SUSPES_MASK)
#define AVR32_is_usb_enabled()              (Tst_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_USBE_MASK))
//! Disable OTG pad
#define AVR32_usb_disable_otg_pad()         (Clr_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_OTGPADE_MASK))
//! Enable OTG pad
#define AVR32_usb_enable_otg_pad()          (Set_bits(AVR32_USBB_usbcon, AVR32_USBB_USBCON_OTGPADE_MASK))
//! enables Start-of-Frame Interrupt
#define AVR32_usb_enable_sof_interrupt()                (AVR32_USBB_udinteset = AVR32_USBB_UDINTESET_SOFES_MASK)
//! Force device full speed mode (i.e. disable high speed)
#define AVR32_usb_force_full_speed_mode()     (Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_SPDCONF_MASK, 3))
//! Enable dual speed mode (full speed and high speed; default)
#define AVR32_usb_use_dual_speed_mode()       (Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_SPDCONF_MASK, 0))
//! detaches from USB bus
#define AVR32_usb_detach()                              (Set_bits(AVR32_USBB_udcon, AVR32_USBB_UDCON_DETACH_MASK))
//! Bounds given integer size to allowed range and rounds it up to the nearest
//! available greater size, then applies register format of USBB controller
//! for endpoint size bit-field.
#define AVR32_usb_format_endpoint_size(size)            (32 - clz(((U32)min(max(size, 8), 1024) << 1) - 1) - 1 - 3)
//! acks NAK OUT received
#define AVR32_usb_ack_nak_out(ep)                       (AVR32_USBB_uestaxclr(ep) = AVR32_USBB_UESTAXCLR_NAKOUTIC_MASK)
//! tests if NAK OUT received
#define AVR32_is_usb_nak_out(ep)                        (Tst_bits(AVR32_USBB_uestax(ep), AVR32_USBB_UESTAX_NAKOUTI_MASK))
//! tests if STALL handshake request is running
#define AVR32_is_usb_endpoint_stall_requested(ep)       (Tst_bits(AVR32_USBB_ueconx(ep), AVR32_USBB_UECONX_STALLRQ_MASK))
//! enables the selected endpoint interrupt
#define AVR32_usb_enable_endpoint_interrupt(ep)         (AVR32_USBB_udinteset = AVR32_USBB_UDINTESET_EP0INTES_MASK << (ep))

//! configures selected endpoint in one step
#define AVR32_usb_configure_endpoint(ep, type, dir, size, bank) \
(\
AVR32_usb_enable_endpoint(ep),\
Wr_bits(AVR32_USBB_uecfgx(ep), AVR32_USBB_UECFGX_EPTYPE_MASK |\
                               AVR32_USBB_UECFGX_EPDIR_MASK  |\
                               AVR32_USBB_UECFGX_EPSIZE_MASK |\
                               AVR32_USBB_UECFGX_EPBK_MASK,   \
        (((U32)(type) << AVR32_USBB_UECFGX_EPTYPE_OFFSET) & AVR32_USBB_UECFGX_EPTYPE_MASK) |\
        (((U32)(dir ) << AVR32_USBB_UECFGX_EPDIR_OFFSET ) & AVR32_USBB_UECFGX_EPDIR_MASK ) |\
        ( (U32)AVR32_usb_format_endpoint_size(size) << AVR32_USBB_UECFGX_EPSIZE_OFFSET         ) |\
        (((U32)(bank) << AVR32_USBB_UECFGX_EPBK_OFFSET  ) & AVR32_USBB_UECFGX_EPBK_MASK  )),\
AVR32_usb_allocate_memory(ep),\
\
AVR32_is_usb_endpoint_configured(ep)\
)

// Access point to the FIFO data registers of pipes/endpoints
// @param x      Pipe/endpoint of which to access FIFO data register
// @param scale  Data index scale in bits: 64, 32, 16 or 8
// @return       Volatile 64-, 32-, 16- or 8-bit data pointer to FIFO data register
#define AVR32_USBB_FIFOX_DATA(x, scale) \
          (((volatile TPASTE2(U, scale) (*)[0x10000 / ((scale) / 8)])AVR32_USBB_SLAVE)[(x)])
// Get 64-, 32-, 16- or 8-bit access to FIFO data register of selected endpoint.
// @param ep     Endpoint of which to access FIFO data register
// @param scale  Data scale in bits: 64, 32, 16 or 8
// @return       Volatile 64-, 32-, 16- or 8-bit data pointer to FIFO data register
// @warning It is up to the user of this macro to make sure that all accesses
// are aligned with their natural boundaries except 64-bit accesses which
// require only 32-bit alignment.
// @warning It is up to the user of this macro to make sure that used HSB
// addresses are identical to the DPRAM internal pointer modulo 32 bits.
#define Usb_get_endpoint_fifo_access(ep, scale) \
          (AVR32_USBB_FIFOX_DATA(ep, scale))
// Reset known position inside FIFO data register of selected endpoint.
// @param ep     Endpoint of which to reset known position
// @warning Always call this macro before any read/write macro/function
// when at FIFO beginning.
#define AVR32_usb_reset_endpoint_fifo_access(ep) \
          (OSUsbDeviceImpl::pep_fifo[(ep)].u64ptr = Usb_get_endpoint_fifo_access(ep, 64))

//! Post-increment operations associated with 64-, 32-, 16- and 8-bit accesses to
//! the FIFO data registers of pipes/endpoints
//! @note 64- and 32-bit accesses to FIFO data registers do not require pointer
//! post-increment while 16- and 8-bit ones do.
//! @note Only for internal use.
//! @{
#define Pep_fifo_access_64_post_inc()
#define Pep_fifo_access_32_post_inc()
#define Pep_fifo_access_16_post_inc()   ++
#define Pep_fifo_access_8_post_inc()    ++

//! Read 64-, 32-, 16- or 8-bit data from FIFO data register of selected endpoint.
//! @param ep     Endpoint of which to access FIFO data register
//! @param scale  Data scale in bits: 64, 32, 16 or 8
//! @return       64-, 32-, 16- or 8-bit data read
//! @warning It is up to the user of this macro to make sure that all accesses
//! are aligned with their natural boundaries except 64-bit accesses which
//! require only 32-bit alignment.
//! @note This macro assures that used HSB addresses are identical to the
//! DPRAM internal pointer modulo 32 bits.
//! @warning Always call Usb_reset_endpoint_fifo_access before this macro when
//! at FIFO beginning.
//! @warning Do not mix calls to this macro with calls to indexed macros below.
#define AVR32_usb_read_endpoint_data(ep, scale) \
        (*OSUsbDeviceImpl::pep_fifo[(ep)].TPASTE3(u, scale, ptr)\
         TPASTE3(Pep_fifo_access_, scale, _post_inc)())

//! Write 64-, 32-, 16- or 8-bit data to FIFO data register of selected endpoint.
//! @param ep     Endpoint of which to access FIFO data register
//! @param scale  Data scale in bits: 64, 32, 16 or 8
//! @param data   64-, 32-, 16- or 8-bit data to write
//! @return       64-, 32-, 16- or 8-bit data written
//! @warning It is up to the user of this macro to make sure that all accesses
//! are aligned with their natural boundaries except 64-bit accesses which
//! require only 32-bit alignment.
//! @note This macro assures that used HSB addresses are identical to the
//! DPRAM internal pointer modulo 32 bits.
//! @warning Always call Usb_reset_endpoint_fifo_access before this macro when
//! at FIFO beginning.
//! @warning Do not mix calls to this macro with calls to indexed macros below.
#define AVR32_usb_write_endpoint_data(ep, scale, data) \
        (*OSUsbDeviceImpl::pep_fifo[(ep)].TPASTE3(u, scale, ptr)\
         TPASTE3(Pep_fifo_access_, scale, _post_inc)() = (data))

// utils
#if LITTLE_ENDIAN_MCU
#define Usb_format_mcu_to_usb_data(width, data) ((TPASTE2(U, width))(data))
#define Usb_format_usb_to_mcu_data(width, data) ((TPASTE2(U, width))(data))
#define usb_format_mcu_to_usb_data(width, data) ((TPASTE2(U, width))(data))
#define usb_format_usb_to_mcu_data(width, data) ((TPASTE2(U, width))(data))
#else // BIG_ENDIAN_MCU
#define Usb_format_mcu_to_usb_data(width, data) (TPASTE2(Swap, width)(data))
#define Usb_format_usb_to_mcu_data(width, data) (TPASTE2(Swap, width)(data))
#define usb_format_mcu_to_usb_data(width, data) (TPASTE2(swap, width)(data))
#define usb_format_usb_to_mcu_data(width, data) (TPASTE2(swap, width)(data))
#endif

//-----------------------------------------------------------------------------

class OSUsbDeviceImpl
{
public:
  OSUsbDeviceImpl();

  // interrupt service routines
  static void
  interruptComServiceRoutine(void);
  static void
  interruptGenServiceRoutine(void);

  // initialize USB driver
  static bool
  usbDriverInit(void);

  // read/write methods
  static unsigned char
  readByte(void);
  static unsigned short
  readWord(void);
  static unsigned long
  readLong(void);
  static void
  writeByte(unsigned char b);
  static void
  writeWord(unsigned short w);
  static void
  writeLong(unsigned long l);

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static int
  readBuffer(void *buf, int count);
  static int
  writeBuffer(void *buf, int count);
  static void
  Usb_reset_endpoint_fifo_access(unsigned char ep);
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

  static void
  standardProcessRequest(void);
  static bool
  specificProcessReadRequest(unsigned char type, unsigned char request);
  static bool
  specificGetDescriptor(unsigned char type, unsigned char string);

  static void
  specificEndpointInit(unsigned char conf_nb);

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static void
  UsbEnableEndpointInterrupt(unsigned char ep);
  static int
  UsbGetEndpointSize(unsigned char ep);
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

  static bool
  usb_configure_endpoint(unsigned char num, unsigned char type,
      unsigned char dir, unsigned char size, unsigned char bank,
      unsigned char nyet);

  static bool
  usb_config_ep(unsigned char config0, unsigned char config1);

  static void
  usb_get_descriptor(void);
  static void
  usb_get_configuration(void);
  static void
  usb_set_address(void);
  static void
  usb_set_configuration(void);
  static void
  usb_clear_feature(void);
  static void
  usb_set_feature(void);
  static void
  usb_get_status(void);
  static bool
  usb_get_interface(void);
  static void
  usb_set_interface(void);
  static void
  usb_enum_var_init(void);
  static void
  usb_set_return(unsigned char len, PGM_VOID_P pbuf);

  static unsigned char
  usb_init_device(void);
  static void
  usb_start_device(void);

  // implementation functions
  static void
  endpointSelect(unsigned char ep);
  static void
  endpointReset(unsigned char ep);

  static bool
  isInterruptReceiveSetup(void);
  static bool
  isInterruptReceiveSetupEnabled(void);
  static void
  interruptReceiveSetupEnable(void);

  static bool
  isInterruptReceiveOut(void);
  static bool
  isInterruptReceiveOutEnabled(void);
  static void
  interruptReceiveOutAck(void);
  static void
  interruptReceiveOutEnable(void);

  static void
  Usb_enable_stall_handshake(void);
  static void
  Usb_ack_receive_setup(void);
  static void
  Usb_enable_regulator(void);
  static void
  Usb_disable_uid_pin(void);
  static void
  Usb_force_device_mode(void);

  static void
  Usb_disable(void);
  static void
  Usb_enable(void);

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static bool
  isUsbEnabled();

  static void
  disableOtgPad(void);
  static void
  enableOtgPad(void);
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

  static void
  Usb_select_device(void);
  static void
  Usb_enable_vbus_interrupt(void);

  static bool
  Is_usb_write_enabled(void);
  static bool
  Is_usb_tx_ready(void);
  static void
  Usb_send_in(void);
  static unsigned short
  Usb_byte_counter(void);

  static void
  Usb_ack_fifocon(void);

  static bool
  Is_usb_vbus_transition(void);
  static bool
  Is_usb_vbus_interrupt_enabled(void);
  static void
  Usb_ack_vbus_transition(void);
  static bool
  Is_usb_vbus_high(void);

  static void
  Usb_enable_reset_interrupt(void);

  static bool
  Is_usb_read_control_enabled(void);
  static void
  Usb_send_control_in(void);
  static void
  Usb_ack_receive_out(void);
  static void
  Usb_ack_in_ready(void);
  static bool
  Is_usb_in_ready(void);
  static bool
  Is_usb_endpoint_enabled(void);

  static void
  Usb_disable_stall_handshake(void);
  static void
  Usb_reset_data_toggle(void);
  static void
  Usb_enable_address(void);
  static void
  Usb_configure_address(unsigned char addr);

  static void
  Usb_set_configuration_action(void);
  static void
  Usb_sof_action(void);
  static void
  Usb_wake_up_action(void);
  static void
  Usb_resume_action(void);
  static void
  Usb_suspend_action(void);
  static void
  Usb_reset_action(void);
  static void
  Usb_vbus_on_action(void);
  static void
  Usb_vbus_off_action(void);

  static void
  Usb_send_event(unsigned char event);

  static void
  Usb_attach(void);

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static void
  Usb_detach(void);
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

  static bool
  Is_usb_sof(void);
  static bool
  Is_sof_interrupt_enabled(void);
  static void
  Usb_ack_sof(void);

  static bool
  Is_usb_suspend(void);
  static bool
  Is_suspend_interrupt_enabled(void);

  static void
  Usb_ack_suspend(void);

  static void
  Usb_enable_wake_up_interrupt(void);
  static void
  Usb_ack_wake_up(void);

  static void
  Usb_freeze_clock(void);
  static void
  Usb_unfreeze_clock(void);

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static bool
  Is_usb_clock_frozen(void);
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

  static void
  Usb_disable_wake_up_interrupt(void);
  static void
  Usb_ack_resume(void);

  static void
  Usb_disable_resume_interrupt(void);
  static void
  Usb_ack_reset(void);

  static bool
  Is_usb_wake_up(void);
  static bool
  Is_swake_up_interrupt_enabled(void);

  static bool
  Is_usb_resume(void);
  static bool
  Is_resume_interrupt_enabled(void);

  static bool
  Is_usb_reset(void);
  static bool
  Is_reset_interrupt_enabled(void);

  static bool
  Is_usb_id_device(void);

  static void
  Usb_enable_endpoint(void);
  static void
  Usb_allocate_memory(void);
  static bool
  Is_endpoint_configured(void);

  static void
  Usb_enable_suspend_interrupt(void);
  static void
  Pll_start_auto(void);
  static bool
  Is_pll_ready(void);

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static void
  Usb_ack_nak_out(void);
  static bool
  Is_usb_nak_out(void);

  static U8
  Get_desc_ep_nbr(U8);

  static U8
  Is_usb_endpoint_stall_requested(U8 ep);
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

  // members
  static unsigned char data_to_transfer;
  static PGM_VOID_P pbuffer;
  static bool zlp;
  static unsigned char bmRequestType;
  static volatile unsigned short g_usb_event;
  static unsigned char endpoint_status[NB_ENDPOINTS];
  static bool usb_connected;
  static unsigned char usb_configuration_nb;

#if defined(OS_CONFIG_FAMILY_AVR32UC3)
  static unsigned char m_selectedEndpoint;
  static unsigned char usb_interface_status[NB_INTERFACE]; // All interface with default setting
  static UnionVPtr pep_fifo[AVR32_USBB_EPT_NUM];
#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */
};

//-----------------------------------------------------------------------------

inline void
OSUsbDeviceImpl::Usb_set_configuration_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_sof_action(void)
{
  ; //sof_action();
}

inline void
OSUsbDeviceImpl::Usb_wake_up_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_resume_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_suspend_action(void)
{
  OSDeviceDebug::putString("zzz...");
  OSDeviceDebug::putNewLine();

#if TO_BE_PORTED
  if (false)
    {
      //Enable_interrupt();
      //Enter_power_down_mode();
      cli();
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);
      sei();
      sleep_cpu();
      sleep_disable();
    }
#endif
}

inline void
OSUsbDeviceImpl::Usb_reset_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_vbus_on_action(void)
{
  ;
}

inline void
OSUsbDeviceImpl::Usb_vbus_off_action(void)
{
  ;
}

//-----------------------------------------------------------------------------


// select the endpoint number to interface with the CPU
inline void
OSUsbDeviceImpl::endpointSelect(unsigned char ep)
{
#if TO_BE_PORTED
  UENUM = ep;
#else
  m_selectedEndpoint = ep;
#endif
}

// reset the selected endpoint
inline void
OSUsbDeviceImpl::endpointReset(unsigned char ep)
{
#if TO_BE_PORTED
  UERST = _BV(ep);
  UERST = 0;
#else
  AVR32_usb_reset_endpoint(ep);
#endif
}

// test if SETUP interrupt received
inline bool
OSUsbDeviceImpl::isInterruptReceiveSetup(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(RXSTPI)) != 0) ? true : false;
#else
  return AVR32_is_usb_setup_received();
#endif
}

// test if SETUP interrupt enabled
inline bool
OSUsbDeviceImpl::isInterruptReceiveSetupEnabled(void)
{
#if TO_BE_PORTED
  return ((UEIENX & _BV(RXSTPE)) != 0) ? true : false;
#else
  return AVR32_is_usb_setup_received_interrupt_enabled();
#endif
}

// enable receive SETUP interrupt
inline void
OSUsbDeviceImpl::interruptReceiveSetupEnable(void)
{
#if TO_BE_PORTED
  UEIENX |= _BV(RXSTPE);
#else
  AVR32_usb_enable_setup_received_interrupt();
#endif
}

// tests if OUT received on selected endpoint
inline bool
OSUsbDeviceImpl::isInterruptReceiveOut(void)
{
#if TO_BE_PORTED
  return (((UEINTX & _BV(RXOUTI))) != 0) ? true : false;
#else
  return AVR32_is_usb_out_received(m_selectedEndpoint);
#endif
}

// test if receive OUT interrupt is enabled on selected endpoint
inline bool
OSUsbDeviceImpl::isInterruptReceiveOutEnabled(void)
{
#if TO_BE_PORTED
  return ((UEIENX & _BV(RXOUTE)) != 0) ? true : false;
#else
  return AVR32_is_usb_out_received_interrupt_enabled(m_selectedEndpoint);
#endif
}

// acks OUT received on selected endpoint and frees current bank
inline void
OSUsbDeviceImpl::interruptReceiveOutAck(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(RXOUTI);
#else
  AVR32_usb_ack_out_received_free(m_selectedEndpoint);
#endif
}

inline void
OSUsbDeviceImpl::Usb_ack_nak_out(void)
{
  AVR32_usb_ack_nak_out(m_selectedEndpoint);
}

inline bool
OSUsbDeviceImpl::Is_usb_nak_out()
{
  return AVR32_is_usb_nak_out(m_selectedEndpoint);
}

// Extract endpoint number from endpoint address in endpoint descriptor
// param ep_addr U8: Endpoint address
// return U8: Endpoint number

#define MSK_EP_NBR                            0x0F

inline U8
OSUsbDeviceImpl::Get_desc_ep_nbr(U8 ep_addr)
{
  return (Rd_bitfield(ep_addr, MSK_EP_NBR));
}

inline U8
OSUsbDeviceImpl::Is_usb_endpoint_stall_requested(U8 ep)
{
  return AVR32_is_usb_endpoint_stall_requested(ep);
}

// enables OUT received interrupt on selected endpoint
inline void
OSUsbDeviceImpl::interruptReceiveOutEnable(void)
{
#if TO_BE_PORTED
  UEIENX |= _BV(RXOUTE);
#else
  AVR32_usb_enable_out_received_interrupt(m_selectedEndpoint);
#endif
}

// return FIFO byte for current endpoint
inline unsigned char
OSUsbDeviceImpl::readByte(void)
{
#if TO_BE_PORTED
  return UEDATX;
#else
  return AVR32_usb_read_endpoint_data(m_selectedEndpoint, 8);
#endif
}

// write byte in FIFO for current endpoint
inline void
OSUsbDeviceImpl::writeByte(unsigned char b)
{
#if TO_BE_PORTED
  UEDATX = b;
#else

  AVR32_usb_write_endpoint_data(m_selectedEndpoint, 8, b);

#endif
}

// enables the STALL handshake for the next transaction
inline void
OSUsbDeviceImpl::Usb_enable_stall_handshake(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(STALLRQ);
#else
  AVR32_usb_enable_stall_handshake(m_selectedEndpoint);
#endif
}

// acks receive SETUP interrupt
inline void
OSUsbDeviceImpl::Usb_ack_receive_setup(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(RXSTPI);
#else
  AVR32_usb_ack_setup_received_free();
#endif
}

// Enable internal USB pads regulator
inline void
OSUsbDeviceImpl::Usb_enable_regulator(void)
{
#if TO_BE_PORTED
  UHWCON |=_BV(UVREGE);
#else

#endif
}

// Disable external UID pin
inline void
OSUsbDeviceImpl::Usb_disable_uid_pin(void)
{
#if TO_BE_PORTED
  UHWCON &= ~_BV(UIDE);
#else
  AVR32_usb_disable_id_pin();
#endif
}

// Disable external UID pin and force device mode
inline void
OSUsbDeviceImpl::Usb_force_device_mode(void)
{
#if TO_BE_PORTED
  UHWCON |= _BV(UIMOD);
#else
  AVR32_usb_force_device_mode();
#endif
}

// Disable both USB interface and Vbus pad
inline void
OSUsbDeviceImpl::Usb_disable(void)
{
#if TO_BE_PORTED
  USBCON &= ~(_BV(USBE) | _BV(OTGPADE));
#else
  AVR32_usb_disable();
#endif
}

// Enable both USB interface and Vbus pad
inline void
OSUsbDeviceImpl::Usb_enable(void)
{
#if TO_BE_PORTED
  USBCON |=(_BV(USBE) |_BV(OTGPADE));
#else
  AVR32_usb_enable();
#endif
}

// check if usb is enabled
inline bool
OSUsbDeviceImpl::isUsbEnabled(void)
{
  return AVR32_is_usb_enabled();
}

// disable OTG pad
inline void
OSUsbDeviceImpl::disableOtgPad(void)
{
  AVR32_usb_disable_otg_pad();
}

// enable OTG pad
inline void
OSUsbDeviceImpl::enableOtgPad(void)
{
  AVR32_usb_enable_otg_pad();
}

// select device mode
inline void
OSUsbDeviceImpl::Usb_select_device(void)
{
#if TO_BE_PORTED
  USBCON &= ~_BV(HOST);
#else

#endif
}

// enable VBUS interrupt
inline void
OSUsbDeviceImpl::Usb_enable_vbus_interrupt(void)
{
#if TO_BE_PORTED
  USBCON |= _BV(VBUSTE);
#else
  AVR32_usb_enable_vbus_interrupt();
#endif
}

// test if write endpoint enabled
inline bool
OSUsbDeviceImpl::Is_usb_write_enabled(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
#else
  return AVR32_is_usb_write_enabled(TX_EP);
#endif
}

// test if transmit ready (same bit as allowed!)
inline bool
OSUsbDeviceImpl::Is_usb_tx_ready(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(RWAL)) != 0) ? true : false;
#else
  return AVR32_is_usb_in_ready(m_selectedEndpoint);
#endif
}

// send IN
inline void
OSUsbDeviceImpl::Usb_send_in(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(FIFOCON);
#else
  AVR32_usb_send_in(m_selectedEndpoint);
#endif
}

// return number of bytes in FIFO current endpoint (16 bits)
inline unsigned short
OSUsbDeviceImpl::Usb_byte_counter(void)
{
#if TO_BE_PORTED
  return UEBCX;
#else
  return AVR32_usb_byte_count(m_selectedEndpoint);
#endif
}

// clear FIFOCON bit
inline void
OSUsbDeviceImpl::Usb_ack_fifocon(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(FIFOCON);
#else
  AVR32_usb_ack_fifocon(m_selectedEndpoint);
#endif
}

inline bool
OSUsbDeviceImpl::Is_usb_vbus_transition(void)
{
#if TO_BE_PORTED
  return ((USBINT & (1<<VBUSTI)) != 0) ? true : false;
#else
  return AVR32_is_usb_vbus_transition();
#endif
}

inline bool
OSUsbDeviceImpl::Is_usb_vbus_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((USBCON & (1<<VBUSTE)) != 0) ? true : false;
#else
  return AVR32_is_usb_vbus_interrupt_enabled();
#endif
}

inline void
OSUsbDeviceImpl::Usb_ack_vbus_transition(void)
{
#if TO_BE_PORTED
  USBINT = ~_BV(VBUSTI);
#else
  AVR32_usb_ack_vbus_transition();
#endif
}

inline bool
OSUsbDeviceImpl::Is_usb_vbus_high(void)
{
#if TO_BE_PORTED
  return ((USBSTA & (1<<VBUS)) != 0) ? true : false;
#else
  return AVR32_is_usb_vbus_high();
#endif
}

// enable USB reset interrupt
inline void
OSUsbDeviceImpl::Usb_enable_reset_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN |= _BV(EORSTE);
#else
  AVR32_usb_enable_reset_interrupt();
#endif
}

// test if read allowed on control endpoint
inline bool
OSUsbDeviceImpl::Is_usb_read_control_enabled(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
#else
  return AVR32_is_usb_control_in_ready();
#endif
}

// send IN on control endpoint
inline void
OSUsbDeviceImpl::Usb_send_control_in(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(TXINI);
#else
  AVR32_usb_ack_control_in_ready_send();
#endif
}

// ack reveive OUT
inline void
OSUsbDeviceImpl::Usb_ack_receive_out(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(RXOUTI);
#endif
  AVR32_usb_ack_out_received(m_selectedEndpoint);
}

// ack IN ready
inline void
OSUsbDeviceImpl::Usb_ack_in_ready(void)
{
#if TO_BE_PORTED
  UEINTX &= ~_BV(TXINI);
#endif
  AVR32_usb_ack_in_ready(m_selectedEndpoint);
}

// test if IN ready
inline bool
OSUsbDeviceImpl::Is_usb_in_ready(void)
{
#if TO_BE_PORTED
  return ((UEINTX & _BV(TXINI)) != 0) ? true : false;
#else
  return AVR32_is_usb_in_ready(m_selectedEndpoint);
#endif
}

// test if the current endpoint is enabled
inline bool
OSUsbDeviceImpl::Is_usb_endpoint_enabled(void)
{
#if TO_BE_PORTED
  return ((UECONX & _BV(EPEN)) != 0) ? true : false;
#else
  return AVR32_is_usb_endpoint_enabled(m_selectedEndpoint);
#endif
}

// disable the STALL handshake
inline void
OSUsbDeviceImpl::Usb_disable_stall_handshake(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(STALLRQC);
#else
  AVR32_usb_disable_stall_handshake(m_selectedEndpoint);
#endif
}

// reset the data toggle sequence
inline void
OSUsbDeviceImpl::Usb_reset_data_toggle(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(RSTDT);
#else
  AVR32_usb_reset_data_toggle(m_selectedEndpoint);
#endif
}

// enable USB device address
inline void
OSUsbDeviceImpl::Usb_enable_address(void)
{
#if TO_BE_PORTED
  UDADDR |= _BV(ADDEN);
#else
  AVR32_usb_enable_address();
#endif
}

// set the USB device address
inline void
OSUsbDeviceImpl::Usb_configure_address(unsigned char addr)
{
#if TO_BE_PORTED
  UDADDR = (UDADDR & _BV(ADDEN)) |(addr & MSK_UADD);
#else
  AVR32_usb_configure_address(addr);
#endif
}

inline void
OSUsbDeviceImpl::Usb_send_event(unsigned char event)
{
  g_usb_event |= (1 << event);
}

// attache to USB bus
inline void
OSUsbDeviceImpl::Usb_attach(void)
{
#if TO_BE_PORTED
  UDCON &= ~_BV(DETACH);
#else
  AVR32_usb_attach();
#endif
}

// attache to USB bus
inline void
OSUsbDeviceImpl::Usb_detach(void)
{
  AVR32_usb_detach();
}

// test if Start Of Frame occurs
inline bool
OSUsbDeviceImpl::Is_usb_sof(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(SOFI)) != 0) ? true : false;
#else
  return AVR32_is_usb_sof();
#endif
}

inline bool
OSUsbDeviceImpl::Is_sof_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(SOFE)) != 0) ? true : false;
#else
  return AVR32_is_usb_sof_interrupt_enabled();
#endif
}

// ack Start Of Frame
inline void
OSUsbDeviceImpl::Usb_ack_sof(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(SOFI);
#else
  AVR32_usb_ack_sof();
#endif
}

// test if Suspend state detected
inline bool
OSUsbDeviceImpl::Is_usb_suspend(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(SUSPI)) != 0) ? true : false;
#else
  return AVR32_is_usb_suspend();
#endif
}

inline bool
OSUsbDeviceImpl::Is_suspend_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(SUSPE)) != 0) ? true : false;
#else
  return AVR32_is_usb_suspend_interrupt_enabled();
#endif
}

// ack Suspend
inline void
OSUsbDeviceImpl::Usb_ack_suspend(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(SUSPI);
#else
  AVR32_usb_ack_suspend();
#endif
}

// enable wake-up interrupt
inline void
OSUsbDeviceImpl::Usb_enable_wake_up_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN |= _BV(WAKEUPE);
#else
  AVR32_usb_enable_wake_up_interrupt();
#endif
}

// ack wake-up
inline void
OSUsbDeviceImpl::Usb_ack_wake_up(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(WAKEUPI);
#else
  AVR32_usb_ack_wake_up();
#endif
}

// Stop internal USB clock in interface(freeze the interface register)
inline void
OSUsbDeviceImpl::Usb_freeze_clock(void)
{
#if TO_BE_PORTED
  USBCON |= _BV(FRZCLK);
#else
  AVR32_usb_freeze_clock();
#endif
}

// Stop internal USB clock in interface(freeze the interface register)
inline bool
OSUsbDeviceImpl::Is_usb_clock_frozen(void)
{
  return AVR32_is_usb_clock_frozen();
}

inline void
OSUsbDeviceImpl::Usb_unfreeze_clock(void)
{
#if TO_BE_PORTED
  USBCON &= ~_BV(FRZCLK);
#else
  AVR32_usb_unfreeze_clock();
#endif
}

// disable wake-up interrupt
inline void
OSUsbDeviceImpl::Usb_disable_wake_up_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN &= ~_BV(WAKEUPE);
#else
  AVR32_usb_disable_wake_up_interrupt();
#endif
}

// ack resume
inline void
OSUsbDeviceImpl::Usb_ack_resume(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(EORSMI);
#else
  AVR32_usb_ack_resume();
#endif
}

// disable resume interrupt
inline void
OSUsbDeviceImpl::Usb_disable_resume_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN &= ~_BV(EORSME);
#else
  AVR32_usb_disable_resume_interrupt();
#endif
}

// ack USB reset
inline void
OSUsbDeviceImpl::Usb_ack_reset(void)
{
#if TO_BE_PORTED
  UDINT = ~_BV(EORSTI);
#else
  AVR32_usb_ack_reset();
#endif
}

// test if wake-up occurs
inline bool
OSUsbDeviceImpl::Is_usb_wake_up(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(WAKEUPI)) != 0) ? true : false;
#else
  return AVR32_is_usb_wake_up();
#endif
}

inline bool
OSUsbDeviceImpl::Is_swake_up_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & (1<<WAKEUPE)) != 0) ? true : true;
#else
  return AVR32_is_usb_wake_up_interrupt_enabled();
#endif
}

// test if resume occurs
inline bool
OSUsbDeviceImpl::Is_usb_resume(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(EORSMI)) != 0) ? true : false;
#else
  return AVR32_is_usb_resume();
#endif
}

inline bool
OSUsbDeviceImpl::Is_resume_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(EORSME)) != 0) ? true : false;
#else
  return AVR32_is_usb_resume_interrupt_enabled();
#endif
}

// test if USB reset occurs
inline bool
OSUsbDeviceImpl::Is_usb_reset(void)
{
#if TO_BE_PORTED
  return ((UDINT & _BV(EORSTI)) != 0) ? true : false;
#else
  return AVR32_is_usb_reset();
#endif
}

inline bool
OSUsbDeviceImpl::Is_reset_interrupt_enabled(void)
{
#if TO_BE_PORTED
  return ((UDIEN & _BV(EORSTE)) != 0) ? true : false;
#else
  return AVR32_is_usb_reset_interrupt_enabled();
#endif
}

inline bool
OSUsbDeviceImpl::Is_usb_id_device(void)
{
#if TO_BE_PORTED
  return ((USBSTA & _BV(ID)) != 0) ? true : false;
#else
  return AVR32_is_usb_id_device();
#endif
}

// enable the current endpoint
inline void
OSUsbDeviceImpl::Usb_enable_endpoint(void)
{
#if TO_BE_PORTED
  UECONX |= _BV(EPEN);
#else
  AVR32_usb_enable_endpoint(m_selectedEndpoint);
#endif
}

// allocate the current configuration in DPRAM memory
inline void
OSUsbDeviceImpl::Usb_allocate_memory(void)
{
#if TO_BE_PORTED
  UECFG1X |= _BV(ALLOC);
#else
  AVR32_usb_allocate_memory(m_selectedEndpoint);
#endif
}

// test if current endpoint is configured
inline bool
OSUsbDeviceImpl::Is_endpoint_configured(void)
{
#if TO_BE_PORTED
  return ((UESTA0X & _BV(CFGOK)) != 0) ? true : false;
#else
  return AVR32_is_usb_endpoint_configured(m_selectedEndpoint);
#endif
}

// return the size of the specified endpoint
inline int
OSUsbDeviceImpl::UsbGetEndpointSize(unsigned char ep)
{
  return AVR32_usb_get_endpoint_size(ep);
}

inline void
OSUsbDeviceImpl::UsbEnableEndpointInterrupt(unsigned char ep)
{
  AVR32_usb_enable_endpoint_interrupt(ep);
}

// enable suspend state interrupt
inline void
OSUsbDeviceImpl::Usb_enable_suspend_interrupt(void)
{
#if TO_BE_PORTED
  UDIEN |= _BV(SUSPE);
#else
  AVR32_usb_enable_suspend_interrupt();
#endif
}

// These functions allow to control the PLL

#if TO_BE_PORTED

#if(OS_CFGLONG_OSCILLATOR_HZ==2000000L)
// Start the PLL in autofactor mode
// regarding FOSC define
#define PLL_FACTOR                  ((0<<PLLP2)|(0<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==4000000L)
#define PLL_FACTOR                  ((0<<PLLP2)|(0<<PLLP1)|(1<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==6000000L)
#define PLL_FACTOR                  ((0<<PLLP2)|(1<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==8000000L)
// Start the PLL in autofactor mode
// regarding FOSC define
#define PLL_FACTOR                  ((0<<PLLP2)|(1<<PLLP1)|(1<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==12000000L)
#define PLL_FACTOR                  ((1<<PLLP2)|(0<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==16000000L)
#define PLL_FACTOR                  ((1<<PLLP2)|(0<<PLLP1)|(1<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==20000000L)
#define PLL_FACTOR                ((1<<PLLP2)|(1<<PLLP1)|(0<<PLLP0))
#elif(OS_CFGLONG_OSCILLATOR_HZ==24000000L)
#define PLL_FACTOR                  ((1<<PLLP2)|(1<<PLLP1)|(1<<PLLP0))
#else
#error   "illegal OS_CFGLONG_OSCILLATOR_HZ"
#endif

#endif /* TO_BE_PORTED */

// @brief Start the PLL at only 48 MHz, regarding CPU frequency
// Start the USB PLL with clockfactor
// clockfactor can be PLLx24, PLLx12, PLLx08
// PLLx06, PLLx04, PLLx03
inline void
OSUsbDeviceImpl::Pll_start_auto(void)
{
#if TO_BE_PORTED
  PLLCSR = (PLL_FACTOR | _BV(PLLE));
#endif
}

inline bool
OSUsbDeviceImpl::Is_pll_ready(void)
{
#if TO_BE_PORTED
  return ((PLLCSR & _BV(PLOCK)) != 0) ? true : false;
#else
  return false;
#endif
}

//-----------------------------------------------------------------------------


#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

#endif /* HAL_OSUSBDEVICEIMPL_H_ */
