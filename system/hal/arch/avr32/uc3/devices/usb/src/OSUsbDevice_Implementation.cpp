/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AVR32UC3)

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbDevice.h"
#include "portable/devices/character/include/DeviceCharacterUsb.h"
#include "hal/arch/avr32/lib/include/compiler.h"
#include "hal/arch/avr32/uc3/lib/include/pm.h"


extern void
usb_process_request(void);
extern void
interruptGenServiceRoutine(void);

extern void
USB_contextHandler(void) __attribute__((interrupt));

void
USB_contextHandler(void)
  {
    OSScheduler::ledActiveOn();

    OSUsbDevice::interruptGenServiceRoutine();
  }



/* USB Endpoint/Pipe Interrupt Communication Request */

void
OSUsbDevice::interruptComServiceRoutine(void)
  {
#if TO_BE_PORTED
    if (UEINT & _BV(EP_CONTROL))
#endif
  }

// @brief USB interrupt subroutine
//
// This function is called each time a USB interrupt occurs.
// The following USB DEVICE events are taken in charge:
// - VBus On / Off
// - Start Of Frame
// - Suspend
// - Wake-Up
// - Resume
// - Reset
// - Start of frame
//
// Note: Only interrupts events that are enabled are processed

//extern void interruptGenServiceRoutine(void);

void
OSUsbDevice::interruptGenServiceRoutine(void)
  {
    OSUsbLed::toggle();

    // ---------- DEVICE events management -----------------------------------
    //- VBUS state detection
    if (OSUsbDevice::Is_usb_vbus_transition()
        && OSUsbDevice::Is_usb_vbus_interrupt_enabled())
      {
        OSDeviceDebug::putString("uv");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_ack_vbus_transition();
        if (OSUsbDevice::Is_usb_vbus_high())
          {
            usb_connected = true;
            OSUsbDevice::Usb_vbus_on_action();
            OSUsbDevice::Usb_send_event(EVT_USB_POWERED);
            OSUsbDevice::Usb_enable_reset_interrupt();
            OSUsbDevice::usb_start_device();
            OSUsbDevice::Usb_attach();
          }
        else
          {
            OSUsbDevice::Usb_vbus_off_action();
            usb_connected = false;
            OSUsbDevice::Usb_send_event(EVT_USB_UNPOWERED);
          }
      }
    // - Device start of frame received
    if (OSUsbDevice::Is_usb_sof() && OSUsbDevice::Is_sof_interrupt_enabled())
      {
        OSUsbDevice::Usb_ack_sof();
        //debug_put_string("uf\n");
        OSUsbDevice::Usb_sof_action();
      }
    // - Device Suspend event(no more USB activity detected)
    if (OSUsbDevice::Is_usb_suspend()
        && OSUsbDevice::Is_suspend_interrupt_enabled())
      {
        OSDeviceDebug::putString("us");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_ack_suspend();
        OSUsbDevice::Usb_enable_wake_up_interrupt();
        OSUsbDevice::Usb_ack_wake_up(); // clear wake up to detect next event
        OSUsbDevice::Usb_freeze_clock();
        OSUsbDevice::Usb_send_event(EVT_USB_SUSPEND);
        OSUsbDevice::Usb_suspend_action();
      }
    // - Wake up event(USB activity detected): Used to resume
    if (OSUsbDevice::Is_usb_wake_up()
        && OSUsbDevice::Is_swake_up_interrupt_enabled())
      {
        OSDeviceDebug::putString("uw");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_unfreeze_clock();
        OSUsbDevice::Usb_ack_wake_up();
        OSUsbDevice::Usb_disable_wake_up_interrupt();
        OSUsbDevice::Usb_wake_up_action();
        OSUsbDevice::Usb_send_event(EVT_USB_WAKE_UP);
      }
    // - Resume state bus detection
    if (OSUsbDevice::Is_usb_resume()
        && OSUsbDevice::Is_resume_interrupt_enabled())
      {
        OSDeviceDebug::putString("ue");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_disable_wake_up_interrupt();
        OSUsbDevice::Usb_ack_resume();
        OSUsbDevice::Usb_disable_resume_interrupt();
        OSUsbDevice::Usb_resume_action();
        OSUsbDevice::Usb_send_event(EVT_USB_RESUME);
      }
    // - USB bus reset detection
    if (OSUsbDevice::Is_usb_reset() && OSUsbDevice::Is_reset_interrupt_enabled())
      {
        OSDeviceDebug::putString("ur");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_ack_reset();
        OSUsbDevice::usb_init_device();
        OSUsbDevice::Usb_reset_action();
        OSUsbDevice::Usb_send_event(EVT_USB_RESET);
      }

    if (OSUsbDevice::isInterruptReceiveSetup()
        && OSUsbDevice::isInterruptReceiveSetupEnabled())
      {
        OSDeviceDebug::putString("Vs ");
        //OSDeviceDebug::putNewLine();

        OSUsbDevice::endpointSelect(EP_CONTROL);

        OSUsbDevice::standardProcessRequest();

        DeviceCharacterUsb::specificCdcInterruptServiceRoutine();
      }

    OSUsbLed::toggle();
  }

// usb_init_device.
//
// This function initialises the USB device controller and
// configures the Default Control Endpoint.
//
//
// @param none
//
// @return status
//
unsigned char
OSUsbDevice::usb_init_device(void)
  {
    OSUsbDevice::Usb_select_device();
    if (OSUsbDevice::Is_usb_id_device())
      {
        OSUsbDevice::endpointSelect(EP_CONTROL);
        if (!OSUsbDevice::Is_usb_endpoint_enabled())
          {
            int r;
            r = OSUsbDevice::usb_configure_endpoint(EP_CONTROL, TYPE_CONTROL,
                DIRECTION_OUT, SIZE_64, ONE_BANK, NYET_DISABLED);
            if (r)
              {
                OSUsbDevice::interruptReceiveSetupEnable();
              }
            return r;
          }
      }
    return false;
  }

bool
OSUsbDevice::usb_configure_endpoint(unsigned char num, unsigned char type,
    unsigned char dir, unsigned char size, unsigned char bank,
    unsigned char nyet)
  {
    OSUsbDevice::endpointSelect(num);
    return OSUsbDevice::usb_config_ep((type << 6) | (nyet << 1) | (dir), (size
            << 4) | (bank << 2));
  }

// usb_configure_endpoint.
//
//  This function configures an endpoint with the selected type.
//
//
// @param config0
// @param config1
//
// @return Is_endpoint_configured.
//
bool
OSUsbDevice::usb_config_ep(unsigned char config0, unsigned char config1)
  {
    OSUsbDevice::Usb_enable_endpoint();

#if TO_BE_PORTED
    UECFG0X = config0;
    UECFG1X = (UECFG1X & _BV(ALLOC)) | config1;
#else
    config0 = config0;
    config1 = config1;
#endif

    OSUsbDevice::Usb_allocate_memory();
    return (OSUsbDevice::Is_endpoint_configured());
  }

// @brief This function initialises the USB device controller
//
// This function enables the USB controller and init the USB interrupts.
// The aim is to allow the USB connection detection in order to send
// the appropriate USB event to the operating mode manager.
// Start device function is executed once VBUS connection has been detected
// either by the VBUS change interrupt either by the VBUS high level
//
// @param none
//
// @return none
//
void
OSUsbDevice::usb_start_device(void)
  {
  //PORTD |= _BV(PORTD7);

  ////////////////////
  // Start USB clock.
  ////////////////////

  // Use 16MHz from OSC0 and generate 96 MHz
  pm_pll_setup(&AVR32_PM, 0,  // pll.
   2,   // mul.
   0,   // div.
   0,   // osc.
   16); // lockcount.

  pm_pll_set_option(&AVR32_PM, 0, // pll.
   1,  // pll_freq: choose the range 80-180MHz.
   0,  // pll_div2.
   0); // pll_wbwdisable.

  // start PLL1 and wait forl lock
  pm_pll_enable(&AVR32_PM, 0);

  // Wait for PLL1 locked.
  pm_wait_for_pll0_locked(&AVR32_PM);

  //setup the USB generic clock
  pm_gc_setup(&AVR32_PM, AVR32_PM_GCLK_USBB,  // gc
               1,                            // osc_or_pll: use Osc (if 0) or PLL (if 1)
               0,                          // pll_osc: select Osc0/PLL0 or Osc1/PLL1
               1,                   // diven
               3);                     // div
  // Enable USB GCLK.
  pm_gc_enable(&AVR32_PM, AVR32_PM_GCLK_USBB);

//    OSUsbDevice::Pll_start_auto();
//    while (!OSUsbDevice::Is_pll_ready())
//    ;
  OSUsbDevice::Usb_unfreeze_clock();
  OSUsbDevice::Usb_enable_suspend_interrupt();
  OSUsbDevice::Usb_enable_reset_interrupt();
  OSUsbDevice::usb_init_device(); // configure the USB controller EP0
  OSUsbDevice::Usb_attach();
  usb_connected = false;

  //PORTD &= ~_BV(PORTD7);

  }

UnionVPtr pep_fifo[AVR32_USBB_EPT_NUM];
//! usb_read_ep_rxpacket
//!
//!  This function reads the selected endpoint FIFO to the buffer pointed to by
//!  rxbuf, using as few accesses as possible.
//!
//! @param ep           Number of the addressed endpoint
//! @param rxbuf        Address of buffer to write
//! @param data_length  Number of bytes to read
//! @param prxbuf       NULL or pointer to the buffer address to update
//!
//! @return             Number of non-read bytes
//!
//! @note The selected endpoint FIFO may be read in several steps by calling
//! usb_read_ep_rxpacket several times.
//!
//! @warning Invoke Usb_reset_endpoint_fifo_access before this function when at
//! FIFO beginning whether or not the FIFO is to be read in several steps.
//!
//! @warning Do not mix calls to this function with calls to indexed macros.
//!
U32 usb_read_ep_rxpacket(U8 ep, void *rxbuf, U32 data_length, void **prxbuf)
{
  // Use aggregated pointers to have several alignments available for a same address
  UnionCVPtr  ep_fifo;
  UnionPtr    rxbuf_cur;
#if (!defined __OPTIMIZE_SIZE__) || !__OPTIMIZE_SIZE__  // Auto-generated when GCC's -Os command option is used
  StructCPtr  rxbuf_end;
#else
  StructCPtr  rxbuf_end;
#endif  // !__OPTIMIZE_SIZE__

  // Initialize pointers for copy loops and limit the number of bytes to copy
  ep_fifo.u8ptr = pep_fifo[ep].u8ptr;
  rxbuf_cur.u8ptr = (U8*)rxbuf;
  rxbuf_end.u8ptr = rxbuf_cur.u8ptr + min(data_length, (AVR32_usb_byte_count(ep)));
#if (!defined __OPTIMIZE_SIZE__) || !__OPTIMIZE_SIZE__  // Auto-generated when GCC's -Os command option is used
  rxbuf_end.u16ptr = (U16 *)Align_down((U32)rxbuf_end.u8ptr, sizeof(U16));
  rxbuf_end.u32ptr = (U32 *)Align_down((U32)rxbuf_end.u16ptr, sizeof(U32));
  rxbuf_end.u64ptr = (U64 *)Align_down((U32)rxbuf_end.u32ptr, sizeof(U64));

  // If all addresses are aligned the same way with respect to 16-bit boundaries
  if (Get_align((U32)rxbuf_cur.u8ptr, sizeof(U16)) == Get_align((U32)ep_fifo.u8ptr, sizeof(U16)))
  {
    // If pointer to reception buffer is not 16-bit aligned
    if (!Test_align((U32)rxbuf_cur.u8ptr, sizeof(U16)))
    {
      // Copy 8-bit data to reach 16-bit alignment
      if (rxbuf_cur.u8ptr < rxbuf_end.u8ptr)
      {
        // 8-bit accesses to FIFO data registers do require pointer post-increment
        *rxbuf_cur.u8ptr++ = *ep_fifo.u8ptr++;
      }
    }

    // If all addresses are aligned the same way with respect to 32-bit boundaries
    if (Get_align((U32)rxbuf_cur.u16ptr, sizeof(U32)) == Get_align((U32)ep_fifo.u16ptr, sizeof(U32)))
    {
      // If pointer to reception buffer is not 32-bit aligned
      if (!Test_align((U32)rxbuf_cur.u16ptr, sizeof(U32)))
      {
        // Copy 16-bit data to reach 32-bit alignment
        if (rxbuf_cur.u16ptr < rxbuf_end.u16ptr)
        {
          // 16-bit accesses to FIFO data registers do require pointer post-increment
          *rxbuf_cur.u16ptr++ = *ep_fifo.u16ptr++;
        }
      }

      // If pointer to reception buffer is not 64-bit aligned
      if (!Test_align((U32)rxbuf_cur.u32ptr, sizeof(U64)))
      {
        // Copy 32-bit data to reach 64-bit alignment
        if (rxbuf_cur.u32ptr < rxbuf_end.u32ptr)
        {
          // 32-bit accesses to FIFO data registers do not require pointer post-increment
          *rxbuf_cur.u32ptr++ = *ep_fifo.u32ptr;
        }
      }

      // Copy 64-bit-aligned data
      while (rxbuf_cur.u64ptr < rxbuf_end.u64ptr)
      {
        // 64-bit accesses to FIFO data registers do not require pointer post-increment
        *rxbuf_cur.u64ptr++ = *ep_fifo.u64ptr;
      }

      // Copy 32-bit-aligned data
      if (rxbuf_cur.u32ptr < rxbuf_end.u32ptr)
      {
        // 32-bit accesses to FIFO data registers do not require pointer post-increment
        *rxbuf_cur.u32ptr++ = *ep_fifo.u32ptr;
      }
    }

    // Copy remaining 16-bit data if some
    while (rxbuf_cur.u16ptr < rxbuf_end.u16ptr)
    {
      // 16-bit accesses to FIFO data registers do require pointer post-increment
      *rxbuf_cur.u16ptr++ = *ep_fifo.u16ptr++;
    }
  }

#endif  // !__OPTIMIZE_SIZE__

  // Copy remaining 8-bit data if some
  while (rxbuf_cur.u8ptr < rxbuf_end.u8ptr)
  {
    // 8-bit accesses to FIFO data registers do require pointer post-increment
    *rxbuf_cur.u8ptr++ = *ep_fifo.u8ptr++;
  }

  // Save current position in FIFO data register
  pep_fifo[ep].u8ptr = (volatile U8 *)ep_fifo.u8ptr;

  // Return the updated buffer address and the number of non-copied bytes
  if (prxbuf) *prxbuf = rxbuf_cur.u8ptr;
  return data_length - (rxbuf_cur.u8ptr - (U8 *)rxbuf);
}

//! usb_write_ep_txpacket
//!
//!  This function writes the buffer pointed to by txbuf to the selected
//!  endpoint FIFO, using as few accesses as possible.
//!
//! @param ep           Number of the addressed endpoint
//! @param txbuf        Address of buffer to read
//! @param data_length  Number of bytes to write
//! @param ptxbuf       NULL or pointer to the buffer address to update
//!
//! @return             Number of non-written bytes
//!
//! @note The selected endpoint FIFO may be written in several steps by calling
//! usb_write_ep_txpacket several times.
//!
//! @warning Invoke Usb_reset_endpoint_fifo_access before this function when at
//! FIFO beginning whether or not the FIFO is to be written in several steps.
//!
//! @warning Do not mix calls to this function with calls to indexed macros.
//!
U32 usb_write_ep_txpacket(U8 ep, const void *txbuf, U32 data_length, const void **ptxbuf)
{
  // Use aggregated pointers to have several alignments available for a same address
  UnionVPtr   ep_fifo;
  UnionCPtr   txbuf_cur;
#if (!defined __OPTIMIZE_SIZE__) || !__OPTIMIZE_SIZE__  // Auto-generated when GCC's -Os command option is used
  StructCPtr  txbuf_end;
#else
  UnionCPtr   txbuf_end;
#endif  // !__OPTIMIZE_SIZE__

  // Initialize pointers for copy loops and limit the number of bytes to copy
  ep_fifo.u8ptr = pep_fifo[ep].u8ptr;
  txbuf_cur.u8ptr = (const U8*)txbuf;
  txbuf_end.u8ptr = txbuf_cur.u8ptr +
                    min(data_length, AVR32_usb_get_endpoint_size(ep) - AVR32_usb_byte_count(ep));
#if (!defined __OPTIMIZE_SIZE__) || !__OPTIMIZE_SIZE__  // Auto-generated when GCC's -Os command option is used
  txbuf_end.u16ptr = (U16 *)Align_down((U32)txbuf_end.u8ptr, sizeof(U16));
  txbuf_end.u32ptr = (U32 *)Align_down((U32)txbuf_end.u16ptr, sizeof(U32));
  txbuf_end.u64ptr = (U64 *)Align_down((U32)txbuf_end.u32ptr, sizeof(U64));

  // If all addresses are aligned the same way with respect to 16-bit boundaries
  if (Get_align((U32)txbuf_cur.u8ptr, sizeof(U16)) == Get_align((U32)ep_fifo.u8ptr, sizeof(U16)))
  {
    // If pointer to transmission buffer is not 16-bit aligned
    if (!Test_align((U32)txbuf_cur.u8ptr, sizeof(U16)))
    {
      // Copy 8-bit data to reach 16-bit alignment
      if (txbuf_cur.u8ptr < txbuf_end.u8ptr)
      {
        // 8-bit accesses to FIFO data registers do require pointer post-increment
        *ep_fifo.u8ptr++ = *txbuf_cur.u8ptr++;
      }
    }

    // If all addresses are aligned the same way with respect to 32-bit boundaries
    if (Get_align((U32)txbuf_cur.u16ptr, sizeof(U32)) == Get_align((U32)ep_fifo.u16ptr, sizeof(U32)))
    {
      // If pointer to transmission buffer is not 32-bit aligned
      if (!Test_align((U32)txbuf_cur.u16ptr, sizeof(U32)))
      {
        // Copy 16-bit data to reach 32-bit alignment
        if (txbuf_cur.u16ptr < txbuf_end.u16ptr)
        {
          // 16-bit accesses to FIFO data registers do require pointer post-increment
          *ep_fifo.u16ptr++ = *txbuf_cur.u16ptr++;
        }
      }

      // If pointer to transmission buffer is not 64-bit aligned
      if (!Test_align((U32)txbuf_cur.u32ptr, sizeof(U64)))
      {
        // Copy 32-bit data to reach 64-bit alignment
        if (txbuf_cur.u32ptr < txbuf_end.u32ptr)
        {
          // 32-bit accesses to FIFO data registers do not require pointer post-increment
          *ep_fifo.u32ptr = *txbuf_cur.u32ptr++;
        }
      }

      // Copy 64-bit-aligned data
      while (txbuf_cur.u64ptr < txbuf_end.u64ptr)
      {
        // 64-bit accesses to FIFO data registers do not require pointer post-increment
        *ep_fifo.u64ptr = *txbuf_cur.u64ptr++;
      }

      // Copy 32-bit-aligned data
      if (txbuf_cur.u32ptr < txbuf_end.u32ptr)
      {
        // 32-bit accesses to FIFO data registers do not require pointer post-increment
        *ep_fifo.u32ptr = *txbuf_cur.u32ptr++;
      }
    }

    // Copy remaining 16-bit data if some
    while (txbuf_cur.u16ptr < txbuf_end.u16ptr)
    {
      // 16-bit accesses to FIFO data registers do require pointer post-increment
      *ep_fifo.u16ptr++ = *txbuf_cur.u16ptr++;
    }
  }

#endif  // !__OPTIMIZE_SIZE__

  // Copy remaining 8-bit data if some
  while (txbuf_cur.u8ptr < txbuf_end.u8ptr)
  {
    // 8-bit accesses to FIFO data registers do require pointer post-increment
    *ep_fifo.u8ptr++ = *txbuf_cur.u8ptr++;
  }

  // Save current position in FIFO data register
  pep_fifo[ep].u8ptr = ep_fifo.u8ptr;

  // Return the updated buffer address and the number of non-copied bytes
  if (ptxbuf) *ptxbuf = txbuf_cur.u8ptr;
  return data_length - (txbuf_cur.u8ptr - (U8 *)txbuf);
}

#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

