/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef HAL_OSUSBDEVICEDEFINES_H_
#define HAL_OSUSBDEVICEDEFINES_H_

#define PGM_VOID_P      void *

// ----------------------------------------------------------------------------

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

///////////////////////
// from AVR32 framework
///////////////////////

extern UnionVPtr pep_fifo[AVR32_USBB_EPT_NUM];

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
          (pep_fifo[(ep)].u64ptr = Usb_get_endpoint_fifo_access(ep, 64))



#endif /* HAL_OSUSBDEVICEDEFINES_H_ */
