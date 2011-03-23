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
#include "hal/arch/avr32/uc3/lib/include/intc.h"

// ----- Static data ----------------------------------------------------------

volatile unsigned short OSUsbDeviceImpl::g_usb_event = 0;
unsigned char OSUsbDeviceImpl::data_to_transfer;
PGM_VOID_P OSUsbDeviceImpl::pbuffer;

bool OSUsbDeviceImpl::zlp;
unsigned char OSUsbDeviceImpl::bmRequestType;
unsigned char OSUsbDeviceImpl::endpoint_status[NB_ENDPOINTS];

bool OSUsbDeviceImpl::usb_connected;

unsigned char OSUsbDeviceImpl::usb_configuration_nb;

unsigned char OSUsbDeviceImpl::m_selectedEndpoint;

// ----------------------------------------------------------------------------

extern void
interruptGenServiceRoutine(void);

extern void
USB_contextHandler(void) __attribute__((interrupt));

void
USB_contextHandler(void)
{
  OSScheduler::ledActiveOn();

  OSUsbLed::toggle();

  // AVR32 does not have separate interrupts, so we call both
  // routines here
  OSUsbDeviceImpl::interruptGenServiceRoutine();
  OSUsbDeviceImpl::interruptComServiceRoutine();

  OSUsbLed::toggle();
}

#if true
/* USB Endpoint/Pipe Interrupt Communication Request */

void
OSUsbDeviceImpl::interruptComServiceRoutine(void)
{
  //OSUsbLed::toggle();
  //OSDeviceDebug::putString("!");
#if TO_BE_PORTED
  if (UEINT & _BV(EP_CONTROL))
#endif
    {
      OSUsbDeviceImpl::endpointSelect(EP_CONTROL);
      if (OSUsbDeviceImpl::isInterruptReceiveSetup()
          && OSUsbDeviceImpl::isInterruptReceiveSetupEnabled())
        {
          OSDeviceDebug::putString("Vs ");
          //OSDeviceDebug::putNewLine();
          //usb_process_request();
          OSUsbDeviceImpl::standardProcessRequest();
        }
    }
  DeviceCharacterUsb::specificCdcInterruptServiceRoutine();

  //OSUsbLed::toggle();
}
#endif

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
OSUsbDeviceImpl::interruptGenServiceRoutine(void)
{
  //OSUsbLed::toggle();

  // ---------- DEVICE events management -----------------------------------
  //- VBUS state detection
  if (OSUsbDeviceImpl::Is_usb_vbus_transition()
      && OSUsbDeviceImpl::Is_usb_vbus_interrupt_enabled())
    {
      OSDeviceDebug::putString("uv");
      OSDeviceDebug::putNewLine();

      OSUsbDeviceImpl::Usb_ack_vbus_transition();
      if (OSUsbDeviceImpl::Is_usb_vbus_high())
        {
          usb_start_device();

          OSUsbDeviceImpl::Usb_vbus_on_action();
          OSUsbDeviceImpl::Usb_send_event(EVT_USB_POWERED);
        }
      else
        {
          Usb_unfreeze_clock();
          Usb_detach();
          usb_connected = false;
          usb_configuration_nb = 0;
          Usb_send_event(EVT_USB_UNPOWERED);
          Usb_vbus_off_action();

          // freeze clocks in order to save power
          Usb_freeze_clock();
          (void) Is_usb_clock_frozen();
        }
    }
  // - Device Suspend event(no more USB activity detected)
  if (OSUsbDeviceImpl::Is_usb_suspend()
      && OSUsbDeviceImpl::Is_suspend_interrupt_enabled())
    {
      OSDeviceDebug::putString("us");
      OSDeviceDebug::putNewLine();

      OSUsbDeviceImpl::Usb_ack_suspend();
      OSUsbDeviceImpl::Usb_enable_wake_up_interrupt();
      (void) OSUsbDeviceImpl::Is_swake_up_interrupt_enabled();
      OSUsbDeviceImpl::Usb_freeze_clock();
      OSUsbDeviceImpl::Usb_send_event(EVT_USB_SUSPEND);
      OSUsbDeviceImpl::Usb_suspend_action();
    }
  // - Wake up event(USB activity detected): Used to resume
  if (OSUsbDeviceImpl::Is_usb_wake_up()
      && OSUsbDeviceImpl::Is_swake_up_interrupt_enabled())
    {
      OSDeviceDebug::putString("uw");
      OSDeviceDebug::putNewLine();

      OSUsbDeviceImpl::Usb_unfreeze_clock();
      (void) Is_usb_clock_frozen();
      OSUsbDeviceImpl::Usb_ack_wake_up();
      OSUsbDeviceImpl::Usb_disable_wake_up_interrupt();
      OSUsbDeviceImpl::Usb_wake_up_action();
      OSUsbDeviceImpl::Usb_send_event(EVT_USB_WAKE_UP);
    }
  // - Resume state bus detection
  if (OSUsbDeviceImpl::Is_usb_resume()
      && OSUsbDeviceImpl::Is_resume_interrupt_enabled())
    {
      OSDeviceDebug::putString("ue");
      OSDeviceDebug::putNewLine();

      OSUsbDeviceImpl::Usb_disable_wake_up_interrupt();
      OSUsbDeviceImpl::Usb_ack_resume();
      OSUsbDeviceImpl::Usb_disable_resume_interrupt();
      OSUsbDeviceImpl::Usb_resume_action();
      OSUsbDeviceImpl::Usb_send_event(EVT_USB_RESUME);
    }
  // - USB bus reset detection
  if (OSUsbDeviceImpl::Is_usb_reset()
      && OSUsbDeviceImpl::Is_reset_interrupt_enabled())
    {
      OSDeviceDebug::putString("ur");
      OSDeviceDebug::putNewLine();

      OSUsbDeviceImpl::Usb_ack_reset();
      usb_init_device();

      endpointReset(EP_CONTROL);

      // now the device will be enumerated
      // enable SETUP received interrupt
      // must be done here because the Usb_reset_endpoint
      // clear the SETUP received interrupt
      interruptReceiveSetupEnable();

      OSUsbDeviceImpl::Usb_reset_action();
      OSUsbDeviceImpl::Usb_send_event(EVT_USB_RESET);
    }
  // - Device start of frame received
  if (OSUsbDeviceImpl::Is_usb_sof()
      && OSUsbDeviceImpl::Is_sof_interrupt_enabled())
    {
      OSUsbDeviceImpl::Usb_ack_sof();
      //debug_put_string("uf\n");
      OSUsbDeviceImpl::Usb_sof_action();
    }

  //OSUsbLed::toggle();
}

// usbDriverInit.
//
// This function initialises the USB driver
//
//
// @param none
//
// @return status
bool
OSUsbDeviceImpl::usbDriverInit(void)
{
  OSScheduler::criticalEnter();

  ////////////////////
  // Start USB clock.
  ////////////////////

  // Use 16MHz from OSC0 and generate 96 MHz
  pm_pll_setup(&AVR32_PM, 0, // pll.
      2, // mul.
      0, // div.
      0, // osc.
      16); // lockcount.

  pm_pll_set_option(&AVR32_PM, 0, // pll.
      1, // pll_freq: choose the range 80-180MHz.
      0, // pll_div2.
      0); // pll_wbwdisable.

  // start PLL1 and wait forl lock
  pm_pll_enable(&AVR32_PM, 0);

  // Wait for PLL1 locked.
  pm_wait_for_pll0_locked(&AVR32_PM);

  //setup the USB generic clock
  pm_gc_setup(&AVR32_PM, AVR32_PM_GCLK_USBB, // gc
      1, // osc_or_pll: use Osc (if 0) or PLL (if 1)
      0, // pll_osc: select Osc0/PLL0 or Osc1/PLL1
      1, // diven
      3); // div
  // Enable USB GCLK.
  pm_gc_enable(&AVR32_PM, AVR32_PM_GCLK_USBB);

  ////////////////////
  // configure USB module.
  ////////////////////

  // register interrupt
  INTC_register_interrupt(USB_contextHandler, AVR32_USBB_IRQ,
      OS_CFGINT_OSUSBDEVICE_IRQ_PRIORITY);

  // force USB module into device mode
  OSUsbDeviceImpl::Usb_force_device_mode();

  /////////////////////
  // initializes the USB device controller.
  ////////////////////
  usb_connected = FALSE;
  usb_configuration_nb = 0;

  OSUsbDeviceImpl::Usb_disable();
  (void) OSUsbDeviceImpl::isUsbEnabled();
  OSUsbDeviceImpl::disableOtgPad();
  OSUsbDeviceImpl::enableOtgPad();
  OSUsbDeviceImpl::Usb_enable();
  OSUsbDeviceImpl::Usb_unfreeze_clock();
  (void) Is_usb_clock_frozen();

  OSUsbDeviceImpl::Usb_enable_vbus_interrupt();

  OSUsbDeviceImpl::Usb_ack_suspend(); // A suspend condition may be detected right after enabling the USB macro
  (void) OSUsbDeviceImpl::Is_usb_suspend();

  // freeze clocks in order to save power
  OSUsbDeviceImpl::Usb_freeze_clock();
  //(void)Is_usb_clock_frozen();

  OSScheduler::criticalExit();

  return 0;
}

bool
OSUsbDeviceImpl::usb_configure_endpoint(unsigned char num, unsigned char type,
    unsigned char dir, unsigned char size, unsigned char bank,
    unsigned char nyet)
{
  nyet = nyet;
  //OSUsbDeviceImpl::endpointSelect(num);
  return AVR32_usb_configure_endpoint(num, type, dir, size, bank);
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
OSUsbDeviceImpl::usb_config_ep(unsigned char config0, unsigned char config1)
{
  OSUsbDeviceImpl::Usb_enable_endpoint();

#if TO_BE_PORTED
  UECFG0X = config0;
  UECFG1X = (UECFG1X & _BV(ALLOC)) | config1;
#else
  // do nothing
  config0 = config0;
  config1 = config1;
#endif

  OSUsbDeviceImpl::Usb_allocate_memory();
  return (OSUsbDeviceImpl::Is_endpoint_configured());
}

//! usb_init_device
//!
//!  This function initializes the USB device controller and
//!  configures the Default Control Endpoint.
//!
//! @return Status
//!
unsigned char
OSUsbDeviceImpl::usb_init_device(void)
{
  return AVR32_is_usb_id_device() && !AVR32_is_usb_endpoint_enabled(EP_CONTROL)
      && AVR32_usb_configure_endpoint(EP_CONTROL,
          TYPE_CONTROL,
          DIRECTION_OUT,
          EP_CONTROL_LENGTH,
          SINGLE_BANK);
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
OSUsbDeviceImpl::usb_start_device(void)
{
  Usb_unfreeze_clock();
  (void) Is_usb_clock_frozen();
  // enable Start of Frame Interrupt
  AVR32_usb_enable_sof_interrupt();
  // enable End of Reset Interrupt
  Usb_enable_reset_interrupt();
  // enable suspend state interrupt
  Usb_enable_suspend_interrupt();

#ifndef HIGH_SPEED_CAPABLE
  AVR32_usb_force_full_speed_mode(); // if we want to use full-speed
#else
  //dual mode needed because the peripheral starts in full-speed mode and performs a
  //high-speed reset to switch to the high-speed mode if the host is high-speed capable
  AVR32_usb_use_dual_speed_mode();
#endif
  // attaches to USB bus
  Usb_attach();

  usb_connected = true;
}

void
OSUsbDeviceImpl::Usb_reset_endpoint_fifo_access(unsigned char ep)
{
  pep_fifo[(ep)].u64ptr = Usb_get_endpoint_fifo_access(ep, 64);
}

#if 0

int
OSUsbDeviceImpl::writeBuffer(void *pBuf, int bufMaxSize)
  {
    unsigned int availableSize, min;
    int status;

    if(bufMaxSize == 0)
      {
        return 0;
      }

    status = AVR32_is_usb_in_ready(m_selectedEndpoint);

    if( !status )
      {
        return 0;
      }

    availableSize = AVR32_usb_get_endpoint_size(m_selectedEndpoint);

    // min will hold the number of bytes to be write
    min = min(availableSize, bufMaxSize);

    // Reset known position inside FIFO
    // data register of selected endpoint
    Usb_reset_endpoint_fifo_access(m_selectedEndpoint);

    // write min bytes from pBuf
    usb_write_ep_txpacket(m_selectedEndpoint, pBuf, min, NULL);

    // acknowledge IN ready and sends current bank
    AVR32_usb_ack_in_ready_send(m_selectedEndpoint);

    // check if there is a need for sending a zlp
    if(min == availableSize)
      {
        // wait for previous data to be sent
        while (!AVR32_is_usb_in_ready(m_selectedEndpoint));
        // send zlp
        AVR32_usb_ack_in_ready_send(m_selectedEndpoint);
      }

    return min;
  }

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
    UnionCVPtr ep_fifo;
    UnionPtr rxbuf_cur;
#if (!defined __OPTIMIZE_SIZE__) || !__OPTIMIZE_SIZE__  // Auto-generated when GCC's -Os command option is used
    StructCPtr rxbuf_end;
#else
    StructCPtr rxbuf_end;
#endif  // !__OPTIMIZE_SIZE__
    // Initialize pointers for copy loops and limit the number of bytes to copy
    ep_fifo.u8ptr = OSUsbDeviceImpl::pep_fifo[ep].u8ptr;
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
    OSUsbDeviceImpl::pep_fifo[ep].u8ptr = (volatile U8 *)ep_fifo.u8ptr;

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
    UnionVPtr ep_fifo;
    UnionCPtr txbuf_cur;
#if (!defined __OPTIMIZE_SIZE__) || !__OPTIMIZE_SIZE__  // Auto-generated when GCC's -Os command option is used
    StructCPtr txbuf_end;
#else
    UnionCPtr txbuf_end;
#endif  // !__OPTIMIZE_SIZE__
    // Initialize pointers for copy loops and limit the number of bytes to copy
    ep_fifo.u8ptr = OSUsbDeviceImpl::pep_fifo[ep].u8ptr;
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
    OSUsbDeviceImpl::pep_fifo[ep].u8ptr = ep_fifo.u8ptr;

    // Return the updated buffer address and the number of non-copied bytes
    if (ptxbuf) *ptxbuf = txbuf_cur.u8ptr;
    return data_length - (txbuf_cur.u8ptr - (U8 *)txbuf);
  }
#endif

// ----------------------------------------------------------------------------

UnionVPtr OSUsbDeviceImpl::pep_fifo[AVR32_USBB_EPT_NUM];
unsigned char OSUsbDeviceImpl::usb_interface_status[NB_INTERFACE];

// ----------------------------------------------------------------------------

/*
 * Read USB Word, LSB | MSB
 */
unsigned short
OSUsbDeviceImpl::readWord(void)
{
  unsigned short w;

  w = readByte();
  w |= (readByte() << 8);

  return w;
}

/*
 * Read USB Long, LSB | MSB
 */
unsigned long
OSUsbDeviceImpl::readLong(void)
{
  unsigned long l;

  l = readByte();
  l |= (((unsigned long) readByte()) << 8);
  l |= (((unsigned long) readByte()) << 16);
  l |= (((unsigned long) readByte()) << 24);

  return l;
}

/*
 * Write USB Word, LSB | MSB
 */
void
OSUsbDeviceImpl::writeWord(unsigned short w)
{
  writeByte((unsigned char) w & 0xFF);
  w >>= 8;
  writeByte((unsigned char) w & 0xFF);
}

/*
 * Write USB Long, LSB | MSB
 */
void
OSUsbDeviceImpl::writeLong(unsigned long l)
{
  writeByte((unsigned char) l & 0xFF);
  l >>= 8;
  writeByte((unsigned char) l & 0xFF);
  l >>= 8;
  writeByte((unsigned char) l & 0xFF);
  l >>= 8;
  writeByte((unsigned char) l & 0xFF);
}

/*
 * Read the SETUP request sent to the default control endpoint
 * and call the appropriate function. When exiting this
 * function, the device is ready to manage the next request.
 *
 * Supported requests:
 * GET_DESCRIPTOR
 * GET_CONFIGURATION
 * SET_ADDRESS
 * SET_CONFIGURATION
 * CLEAR_FEATURE
 * SET_FEATURE
 * GET_STATUS
 */

void
OSUsbDeviceImpl::standardProcessRequest(void)
{
  unsigned char bmRequest;

  endpointSelect(EP_CONTROL);

  Usb_reset_endpoint_fifo_access(EP_CONTROL);
  bmRequestType = readByte();
  bmRequest = readByte();

#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("type/req=");
  OSDeviceDebug::putHex(bmRequestType);
  OSDeviceDebug::putString("/");
  OSDeviceDebug::putHex(bmRequest);
  OSDeviceDebug::putString(" ");
  //OSDeviceDebug::putNewLine();
#endif

  switch (bmRequest)
    {
  case GET_DESCRIPTOR:
    if (bmRequestType == 0x80)
      {
        OSUsbDeviceImpl::usb_get_descriptor();
      }
    else
      goto unsupported_request;
    break;

  case GET_CONFIGURATION:
    if (bmRequestType == 0x80)
      {
        usb_get_configuration();
      }
    else
      goto unsupported_request;
    break;

  case SET_ADDRESS:
    if (bmRequestType == 0x00)
      {
        usb_set_address();
      }
    else
      goto unsupported_request;
    break;

  case SET_CONFIGURATION:
    if (bmRequestType == 0x00)
      {
        usb_set_configuration();
      }
    else
      goto unsupported_request;
    break;

  case CLEAR_FEATURE:
    if (bmRequestType <= 0x02)
      {
        usb_clear_feature();
      }
    else
      goto unsupported_request;
    break;

  case SET_FEATURE:
    if (bmRequestType <= 0x02)
      {
        usb_set_feature();
      }
    else
      goto unsupported_request;
    break;

  case GET_STATUS:
    if (0x7F < bmRequestType && bmRequestType <= 0x82)
      {
        usb_get_status();
      }
    else
      goto unsupported_request;
    break;

  case GET_INTERFACE:
    if (bmRequestType == 0x81)
      {
        if (!usb_get_interface())
          {
            Usb_enable_stall_handshake();
            Usb_ack_receive_setup();
          }
      }
    else
      goto unsupported_request;
    break;

  case SET_INTERFACE:
    if (bmRequestType == 0x01)
      usb_set_interface();
    else
      goto unsupported_request;
    break;

  case SET_DESCRIPTOR:
  case SYNCH_FRAME:
  default: //!< unsupported request => call to user read request
    unsupported_request: if (!DeviceCharacterUsb::specificCdcProcessReadRequest(
        bmRequestType, bmRequest))
      {
        Usb_enable_stall_handshake();
        Usb_ack_receive_setup();
      }
    break;

    }
}

/*
 * Redirector to specific read request
 */
bool
OSUsbDeviceImpl::specificProcessReadRequest(unsigned char type,
    unsigned char request)
{
  if (DeviceCharacterUsb::specificCdcProcessReadRequest(type, request))
    return true;

  return false;
}

/*
 * Redirector to specific endpoint init
 */
void
OSUsbDeviceImpl::specificEndpointInit(unsigned char conf_nb)
{
  DeviceCharacterUsb::specificCdcEndpointInit(conf_nb);
  return;
}

/*
 * Redirector to specific get descriptor
 */
bool
OSUsbDeviceImpl::specificGetDescriptor(unsigned char type, unsigned char string)
{
  if (DeviceCharacterUsb::specificCdcGetDescriptor(type, string))
    return true;

  return false;
}

/*
 * Manage the GET DESCRIPTOR request. The device descriptor,
 * the configuration descriptor and the device qualifier are supported. All
 * other descriptors must be supported by the specificGetDescriptor
 * function.
 * Only 1 configuration is supported.
 */
void
OSUsbDeviceImpl::usb_get_descriptor(void)
{
#if TO_BE_PORTED
  unsigned short wLength;
  unsigned char descriptor_type;
  unsigned char descriptor_index;
  //U8 dummy;
  unsigned short dummy16;
  unsigned char nb_byte;

  zlp = false; /* no zero length packet */
  descriptor_index = OSUsbDeviceImpl::readByte(); /* read LSB of wValue    */
  descriptor_type = OSUsbDeviceImpl::readByte(); /* read MSB of wValue    */

#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("GET_DESCRIPTOR");
  OSDeviceDebug::putString(" ty/ix=");
  OSDeviceDebug::putHex(descriptor_type);
  OSDeviceDebug::putString("/");
  OSDeviceDebug::putHex(descriptor_index);
  OSDeviceDebug::putString(" ");
#endif

  switch (descriptor_type)
    {
      case DEVICE_DESCRIPTOR: // 0x01
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
      OSDeviceDebug::putString("DEVICE");
#endif
      //OSDeviceDebug::putNewLine();
      data_to_transfer = sizeof(usb_dev_desc); //!< sizeof(usb_user_device_descriptor);
      pbuffer = &(usb_dev_desc);
      break;

      case CONFIGURATION_DESCRIPTOR: // 0x02
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
      OSDeviceDebug::putString("CONFIGURATION");
#endif
      //OSDeviceDebug::putNewLine();
#if defined(OS_INCLUDE_USB_CDC_DUAL_CONFIGURATION)

      if (descriptor_index == 0)
        {
          data_to_transfer = sizeof(usb_conf_desc);
          pbuffer = &usb_conf_desc;
        }
      else if (descriptor_index == 1)
        {
          data_to_transfer = sizeof(usb_conf2_desc);
          pbuffer = &usb_conf2_desc;
        }
      else
        {
          OSUsbDeviceImpl::Usb_enable_stall_handshake();
          OSUsbDeviceImpl::Usb_ack_receive_setup();
          return;
        }
#else
      data_to_transfer = sizeof(usb_conf2_desc);
      pbuffer = &usb_conf2_desc;
#endif
      break;

      default:
      if (specificGetDescriptor(descriptor_type, descriptor_index))
      break;

      OSUsbDeviceImpl::Usb_enable_stall_handshake();
      OSUsbDeviceImpl::Usb_ack_receive_setup();
      return;
    }

  dummy16 = OSUsbDeviceImpl::readWord(); // don't care of wIndex field
  wLength = OSUsbDeviceImpl::readWord(); // read wLength
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString(" ix=");
  OSDeviceDebug::putHex(dummy16);
  OSDeviceDebug::putString(" len=");
  OSDeviceDebug::putHex(wLength);
  OSDeviceDebug::putNewLine();
#endif

  OSUsbDeviceImpl::Usb_ack_receive_setup(); // clear the receive setup flag

  if (wLength > data_to_transfer)
    {
      if ( (data_to_transfer % EP_CONTROL_LENGTH ) == 0)
        {
          zlp = true;
        }
      else
        {
          zlp = false;
        } // no need of zero length packet
    }
  else
    {
      data_to_transfer = ( unsigned char ) wLength; // send only requested number of data
    }

  while ( (data_to_transfer != 0)
      && ( !OSUsbDeviceImpl::isInterruptReceiveOut() ))
    {
      while ( !OSUsbDeviceImpl::Is_usb_read_control_enabled() )
        {
          ;
        }

      nb_byte = 0;
      while (data_to_transfer != 0) // Send data until necessary

        {
          if (nb_byte++ == EP_CONTROL_LENGTH) // Check endpoint 0 size

            {
              break;
            }
#if defined(OS_INCLUDE_SEPARATE_PROGMEM)
          // assumes devices descriptors are stored in the lower 64Kbytes of on-chip flash memory
          OSUsbDeviceImpl::writeByte(PROGMEM_READ_BYTE(pbuffer));
          //OSUsbDeviceImpl::writeByte(pgm_read_byte_near( ( unsigned int ) pbuffer ));
#else
          OSUsbDeviceImpl::writeByte( *(( unsigned char * ) pbuffer) );
#endif
          pbuffer = ( unsigned char * ) pbuffer + 1;

          data_to_transfer--;
        }
      OSUsbDeviceImpl::Usb_send_control_in();
    }

  OSUsbDeviceImpl::Usb_send_control_in();

  if (OSUsbDeviceImpl::isInterruptReceiveOut() )
    {
      OSUsbDeviceImpl::Usb_ack_receive_out();
      return;
    } // abort from Host

  if (zlp)
    {
      while ( !OSUsbDeviceImpl::Is_usb_read_control_enabled() )
        {
          ;
        }
      OSUsbDeviceImpl::Usb_send_control_in();
    }

  while ( !OSUsbDeviceImpl::isInterruptReceiveOut() )
    {
      ;
    }
  OSUsbDeviceImpl::Usb_ack_receive_out();
#else
  Bool zlp;
  U16 wLength, counter;
  U8 descriptor_type;
  U8 string_type;
  unsigned short dummy16;
#if (USB_HIGH_SPEED_SUPPORT==ENABLED)
  Bool b_first_data = TRUE;
#endif

  zlp = FALSE; /* no zero length packet */
  string_type = readByte(); /* read LSB of wValue    */
  descriptor_type = readByte(); /* read MSB of wValue    */

  switch (descriptor_type)
    {
  case DEVICE_DESCRIPTOR:
    //push_in_my_debug_table('1');
    data_to_transfer = Usb_get_dev_desc_length(); //!< sizeof(AVR32_usb_dev_desc);
    pbuffer = (PGM_VOID_P) Usb_get_dev_desc_pointer();
    break;

#if (USB_HIGH_SPEED_SUPPORT==DISABLED)
  case CONFIGURATION_DESCRIPTOR:
    //push_in_my_debug_table('2');
    data_to_transfer = Usb_get_conf_desc_length(); //!< sizeof(usb_conf_desc);
    //push_in_my_debug_table(data_to_transfer);
    pbuffer = (PGM_VOID_P) Usb_get_conf_desc_pointer();
    break;

#else
    case CONFIGURATION_DESCRIPTOR:
    if( Is_usb_full_speed_mode() )
      {
        data_to_transfer = Usb_get_conf_desc_fs_length(); //!< sizeof(usb_conf_desc_fs);
        pbuffer = Usb_get_conf_desc_fs_pointer();
      }
    else
      {
        data_to_transfer = Usb_get_conf_desc_hs_length(); //!< sizeof(usb_conf_desc_hs);
        pbuffer = Usb_get_conf_desc_hs_pointer();
      }
    break;

    case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
    if( !Is_usb_full_speed_mode() )
      {
        data_to_transfer = Usb_get_conf_desc_fs_length(); //!< sizeof(usb_conf_desc_fs);
        pbuffer = Usb_get_conf_desc_fs_pointer();
      }
    else
      {
        data_to_transfer = Usb_get_conf_desc_hs_length(); //!< sizeof(usb_conf_desc_hs);
        pbuffer = Usb_get_conf_desc_hs_pointer();
      }
    break;

    case DEVICE_QUALIFIER_DESCRIPTOR:
    data_to_transfer = Usb_get_qualifier_desc_length(); //!< sizeof(usb_qualifier_desc);
    pbuffer = Usb_get_qualifier_desc_pointer();
    break;

#endif

  default:
    //push_in_my_debug_table('3');
    if (!specificGetDescriptor(descriptor_type, string_type))
      {
        Usb_enable_stall_handshake();
        Usb_ack_receive_setup();
        return;
      }
    break;
    }

  dummy16 = OSUsbDeviceImpl::readWord(); // don't care of wIndex field
  wLength = OSUsbDeviceImpl::readWord(); // read wLength
  Usb_ack_receive_setup(); //!< clear the setup received flag

  if (wLength > data_to_transfer)
    {
      zlp = !(data_to_transfer % EP_CONTROL_LENGTH); //!< zero length packet condition
    }
  else
    {
      // No need to test ZLP sending since we send the exact number of bytes as
      // expected by the host.
      data_to_transfer = wLength; //!< send only requested number of data bytes
    }

  Usb_ack_nak_out();

  while (data_to_transfer && !Is_usb_nak_out())
    {
      while (!Is_usb_tx_ready() && !Is_usb_nak_out())
        ;

      if (Is_usb_nak_out())
        break; // don't clear the flag now, it will be cleared after

      Usb_reset_endpoint_fifo_access(EP_CONTROL);

#if (USB_HIGH_SPEED_SUPPORT==ENABLED) // To support other descriptors like OTHER_SPEED_CONFIGURATION_DESCRIPTOR
      if( b_first_data )
        {
          b_first_data = FALSE;
          if( 0!= data_to_transfer )
            {
              usb_write_ep_txpacket(EP_CONTROL, pbuffer, 1, &pbuffer);
              data_to_transfer--;
            }
          if( 0!= data_to_transfer )
            {
              usb_write_ep_txpacket(EP_CONTROL, &descriptor_type, 1, NULL);
              pbuffer = ((const U8*)pbuffer)+1;
              data_to_transfer--;
            }
        }
#endif
      if (0 != data_to_transfer)
        {
          //         data_to_transfer = usb_write_ep_txpacket(EP_CONTROL, pbuffer,
          //                                                  data_to_transfer, &pbuffer);
          //TODO: replace this with the code above.
          while (!OSUsbDeviceImpl::Is_usb_tx_ready())
            ; // Wait Endpoint ready...
          for (counter = 0; counter < UsbGetEndpointSize(EP_CONTROL); counter++)
            {
              writeByte(*((char*) pbuffer));

              pbuffer = (char*) pbuffer + 1;
              data_to_transfer--;
              if (data_to_transfer == 0)
                break;
            }
          Usb_send_control_in();
          if (data_to_transfer == 0 && counter
              == UsbGetEndpointSize(EP_CONTROL))// ZLP
            {
              while (!OSUsbDeviceImpl::Is_usb_tx_ready())
                ; // Wait Endpoint ready...
              OSUsbDeviceImpl::Usb_send_in(); // ...and Send ZLP
            }
        }
      if (Is_usb_nak_out())
        break;
      Usb_send_control_in(); //!< Send data until necessary
    }

  if (zlp && !Is_usb_nak_out())
    {
      while (!Is_usb_read_control_enabled())
        ;
      Usb_send_control_in();
    }

  while (!Is_usb_nak_out())
    ;
  Usb_ack_nak_out();
  while (!isInterruptReceiveOut())
    ;
  interruptReceiveOutAck();
#endif
}

/*
 * Manage the GET CONFIGURATION request. The current
 * configuration number is returned.
 */
void
OSUsbDeviceImpl::usb_get_configuration(void)
{
  Usb_ack_receive_setup();

  Usb_reset_endpoint_fifo_access(EP_CONTROL);
  writeByte(usb_configuration_nb);
  Usb_send_control_in();

  while (!isInterruptReceiveOut())
    ;
  interruptReceiveOutAck();
}

/*
 * Manage the SET ADDRESS request. When complete, the device
 * will filter the requests using the new address.
 */
void
OSUsbDeviceImpl::usb_set_address(void)
{
  unsigned char addr;
  addr = readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("SET_ADDRESS ");
  OSDeviceDebug::putHex(addr);
  OSDeviceDebug::putNewLine();
#endif

  Usb_configure_address(addr);

  Usb_ack_receive_setup();

  Usb_send_control_in(); //!< send a ZLP for STATUS phase
  while (!OSUsbDeviceImpl::Is_usb_in_ready())
    {
      ;
    } //!< waits for status phase done
  //< before using the new address
  Usb_enable_address();
}

/*
 * Manage the SET CONFIGURATION request. If the selected
 * configuration is valid, this function call the specificEndpointInit()
 * function that will configure the endpoints following the configuration
 * number.
 */
void
OSUsbDeviceImpl::usb_set_configuration(void)
{
  unsigned char configuration_number, u8_i;

  configuration_number = OSUsbDeviceImpl::readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("SET_CONFIGURATION ");
  OSDeviceDebug::putHex(configuration_number);
  OSDeviceDebug::putNewLine();
#endif

  if (configuration_number <= NB_CONFIGURATION)
    {
      Usb_ack_receive_setup();
      usb_configuration_nb = configuration_number;
      for (u8_i = 0; u8_i < NB_INTERFACE; u8_i++)
        usb_interface_status[u8_i] = 0;

      DeviceCharacterUsb::specificCdcEndpointInit(usb_configuration_nb); //!< endpoint configuration
      Usb_set_configuration_action();

      Usb_send_control_in(); //!< send a ZLP for STATUS phase
    }
  else
    {
      //!< keep that order (set StallRq/clear RxSetup) or a
      //!< OUT request following the SETUP may be acknowledged
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
    }
}

/*
 * Manage the SET FEATURE request.
 */
void
OSUsbDeviceImpl::usb_clear_feature(void)
{
  unsigned char wValue;
  unsigned char wIndex;
  unsigned char dummy;

  endpointSelect(EP_CONTROL);

  if (bmRequestType == ZERO_TYPE || bmRequestType == INTERFACE_TYPE)
    {
      //!< keep that order (set StallRq/clear RxSetup) or a
      //!< OUT request following the SETUP may be acknowledged
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
    }
  else if (bmRequestType == ENDPOINT_TYPE)
    {
      wValue = OSUsbDeviceImpl::readByte();
      dummy = OSUsbDeviceImpl::readByte(); // dummy read

      if (wValue == FEATURE_ENDPOINT_HALT)
        {
          wIndex = (OSUsbDeviceImpl::readByte() & MSK_EP_DIR);

          OSUsbDeviceImpl::endpointSelect(wIndex);
          if (OSUsbDeviceImpl::Is_usb_endpoint_enabled())
            {
              if (wIndex != EP_CONTROL)
                {
                  Usb_disable_stall_handshake();
                  endpointReset(wIndex);
                  Usb_reset_data_toggle();

                  if (wIndex == RX_EP)
                    {
                      // enable RX interrupt
                      endpointSelect(RX_EP);
                      UsbEnableEndpointInterrupt(RX_EP);
                      interruptReceiveOutEnable();
                      Usb_reset_endpoint_fifo_access(RX_EP);
                      endpointSelect(EP_CONTROL);
                    }
                }
              Usb_ack_receive_setup();
              Usb_send_control_in();
            }
          else
            {
              endpointSelect(EP_CONTROL);
              Usb_enable_stall_handshake();
              Usb_ack_receive_setup();
            }
        }
      else
        {
          endpointSelect(EP_CONTROL);
          Usb_enable_stall_handshake();
          Usb_ack_receive_setup();
        }
    }
}

/*
 * Manage the SET FEATURE request. The USB test modes are
 * supported by this function.
 */
void
OSUsbDeviceImpl::usb_set_feature(void)
{
#if TO_BE_PORTED
  unsigned char wValue;
  unsigned char wIndex;
  unsigned char dummy;

  if (bmRequestType == INTERFACE_TYPE)
    {
      // keep that order(set StallRq/clear RxSetup)or a
      // OUT request following the SETUP may be acknowledged
      OSUsbDeviceImpl::Usb_enable_stall_handshake();
      OSUsbDeviceImpl::Usb_ack_receive_setup();
      return;
    }
  else if (bmRequestType == ENDPOINT_TYPE)
    {
      wValue = OSUsbDeviceImpl::readByte();
      dummy = OSUsbDeviceImpl::readByte(); // dummy read

      if (wValue == FEATURE_ENDPOINT_HALT)
        {
          wIndex = (OSUsbDeviceImpl::readByte() & MSK_EP_DIR );

          if (wIndex == EP_CONTROL)
            {
              OSUsbDeviceImpl::Usb_enable_stall_handshake();
              OSUsbDeviceImpl::Usb_ack_receive_setup();
              return;
            }

          OSDeviceDebug::putString(" se");
          OSDeviceDebug::putDec((unsigned short)wIndex);
          OSDeviceDebug::putChar(' ');

          OSUsbDeviceImpl::endpointSelect(wIndex);
          if (OSUsbDeviceImpl::Is_usb_endpoint_enabled() )
            {
              OSUsbDeviceImpl::Usb_enable_stall_handshake();
              OSUsbDeviceImpl::endpointSelect(EP_CONTROL);
              endpoint_status[ wIndex ] = 0x01;
              OSUsbDeviceImpl::Usb_ack_receive_setup();
              OSUsbDeviceImpl::Usb_send_control_in();
            }
          else
            {
              OSUsbDeviceImpl::Usb_enable_stall_handshake();
              OSUsbDeviceImpl::Usb_ack_receive_setup();
              return;
            }
        }
      else
        {
          OSUsbDeviceImpl::Usb_enable_stall_handshake();
          OSUsbDeviceImpl::Usb_ack_receive_setup();
          return;
        }
    }
#else
  U16 wValue = readWord();
  U16 wIndex = readWord();
  U16 wLength = readWord();

  if (wLength)
    goto unsupported_request;

  switch (wValue)
    {
  case FEATURE_ENDPOINT_HALT:
    wIndex = Get_desc_ep_nbr(wIndex); // clear direction flag

    endpointSelect(wIndex);

    if (bmRequestType != ENDPOINT_TYPE || wIndex == EP_CONTROL
        || !Is_usb_endpoint_enabled())
      goto unsupported_request;

    Usb_enable_stall_handshake();
    Usb_ack_receive_setup();

    endpointSelect(EP_CONTROL);
    Usb_send_control_in();
    break;

#if (USB_HIGH_SPEED_SUPPORT==ENABLED)
    case FEATURE_TEST_MODE:
    if (bmRequestType != DEVICE_TYPE ||
        wIndex & 0x00FF)
    goto unsupported_request;

    switch (wIndex >> 8)
      {
        case TEST_J:
        Usb_ack_setup_received_free();
        Usb_ack_control_in_ready_send();
        while (!Is_usb_control_in_ready());
        Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_SPDCONF_MASK, 2);
        Set_bits(AVR32_USBB_udcon, AVR32_USBB_UDCON_TSTJ_MASK);
        break;

        case TEST_K:
        Usb_ack_setup_received_free();
        Usb_ack_control_in_ready_send();
        while (!Is_usb_control_in_ready());
        Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_SPDCONF_MASK, 2);
        Set_bits(AVR32_USBB_udcon, AVR32_USBB_UDCON_TSTK_MASK);
        break;

        case TEST_SE0_NAK:
        Usb_ack_setup_received_free();
        Usb_ack_control_in_ready_send();
        while (!Is_usb_control_in_ready());
        Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_SPDCONF_MASK, 2);
        break;

        case TEST_PACKET:
          {
            static const U8 test_packet[] =
              {
                // 00000000 * 9
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                // 01010101 * 8
                0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                // 01110111 * 8
                0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
                // 0, {111111S * 15}, 111111
                0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                // S, 111111S, {0111111S * 7}
                0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD,
                // 00111111, {S0111111 * 9}, S0
                0xFC, 0x7E, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0x7E
              };

            Usb_ack_setup_received_free();
            Usb_ack_control_in_ready_send();
            while (!Is_usb_control_in_ready());
            Wr_bitfield(AVR32_USBB_udcon, AVR32_USBB_UDCON_SPDCONF_MASK, 2);
            Usb_disable_endpoint(EP_CONTROL);
            Usb_unallocate_memory(EP_CONTROL);
            (void)Usb_configure_endpoint(EP_CONTROL,
                TYPE_BULK,
                DIRECTION_IN,
                64,
                SINGLE_BANK);
            Usb_reset_endpoint(EP_CONTROL);
            Set_bits(AVR32_USBB_udcon, AVR32_USBB_UDCON_TSTPCKT_MASK);
            usb_write_ep_txpacket(EP_CONTROL, &test_packet, sizeof(test_packet), NULL);
            Usb_send_in(EP_CONTROL);
          }
        break;

        case TEST_FORCE_ENABLE: // Only for downstream facing hub ports
        default:
        goto unsupported_request;
      }
    break;
#endif

  case FEATURE_DEVICE_REMOTE_WAKEUP:
  default:
    goto unsupported_request;
    }

  return;

  unsupported_request: endpointSelect(EP_CONTROL);
  Usb_enable_stall_handshake();
  Usb_ack_receive_setup();
#endif
}

/*
 * Manage the GET STATUS request. The device, interface or
 * endpoint status is returned.
 */
void
OSUsbDeviceImpl::usb_get_status(void)
{
  unsigned char wIndex;
  unsigned char dummy;

  dummy = readWord(); //!< dummy read (wValue)
  wIndex = readByte();
  wIndex = wIndex & MSK_EP_DIR;

  switch (bmRequestType)
    {
  case REQUEST_DEVICE_STATUS:
    Usb_ack_receive_setup();
    Usb_reset_endpoint_fifo_access(EP_CONTROL);
    writeByte(DEVICE_STATUS);
    break;

  case REQUEST_INTERFACE_STATUS:
    Usb_ack_receive_setup();
    Usb_reset_endpoint_fifo_access(EP_CONTROL);
    writeByte(INTERFACE_STATUS);
    break;

  case REQUEST_ENDPOINT_STATUS:
#if TO_BE_PORTED
    OSUsbDeviceImpl::Usb_ack_receive_setup();
    OSUsbDeviceImpl::writeByte(endpoint_status[ wIndex ]);
#else
    Usb_ack_receive_setup();
    Usb_reset_endpoint_fifo_access(EP_CONTROL);
    writeByte(Is_usb_endpoint_stall_requested(wIndex));
#endif
    break;

  default:
    Usb_enable_stall_handshake();
    Usb_ack_receive_setup();
    return;
    }

  writeByte(0x00);
  Usb_send_control_in();

  while (!isInterruptReceiveOut())
    ;
#if TO_BE_PORTED
  OSUsbDeviceImpl::Usb_ack_receive_out();
#else
  interruptReceiveOutAck();
#endif
}

/*
 * Manage the GET_INTERFACE request.
 */
bool
OSUsbDeviceImpl::usb_get_interface(void)
{
#if TO_BE_PORTED
  OSUsbDeviceImpl::Usb_enable_stall_handshake();
  OSUsbDeviceImpl::Usb_ack_receive_setup();
#else
  U16 wInterface;
  U16 wValue;

  // Read wValue
  wValue =  readWord();
  // wValue = Alternate Setting
  // wIndex = Interface
  wInterface = readWord();
  if (0 != wValue)
    return FALSE;
  Usb_ack_receive_setup();

  Usb_reset_endpoint_fifo_access(EP_CONTROL);
  writeByte(usb_interface_status[wInterface]);
  Usb_send_control_in();

  while (!isInterruptReceiveOut())
    ;
  interruptReceiveOutAck();
  return TRUE;
#endif
}

/*
 * Manage the SET_INTERFACE request.
 */
void
OSUsbDeviceImpl::usb_set_interface(void)
{
#if TO_BE_PORTED
  OSUsbDeviceImpl::Usb_ack_receive_setup();
  OSUsbDeviceImpl::Usb_send_control_in(); // send a ZLP for STATUS phase
  while ( !OSUsbDeviceImpl::Is_usb_in_ready() )
    {
      ;
    }
#else
  U8 u8_i;

  endpointSelect(EP_CONTROL);

  // wValue = Alternate Setting
  // wIndex = Interface
  U16 wValue = readWord();
  U16 wIndex = readWord();
  Usb_ack_receive_setup();

  // Get descriptor
#if (USB_HIGH_SPEED_SUPPORT==ENABLED)
  if( Is_usb_full_speed_mode() )
    {
      data_to_transfer = Usb_get_conf_desc_fs_length(); //!< sizeof(usb_conf_desc_fs);
      pbuffer = Usb_get_conf_desc_fs_pointer();
    }
  else
    {
      data_to_transfer = Usb_get_conf_desc_hs_length(); //!< sizeof(usb_conf_desc_hs);
      pbuffer = Usb_get_conf_desc_hs_pointer();
    }
#else
  data_to_transfer = Usb_get_conf_desc_length(); //!< sizeof(usb_conf_desc);
  pbuffer = (PGM_VOID_P) Usb_get_conf_desc_pointer();
#endif

  //** Scan descriptor

  //* Find configuration selected
  if (usb_configuration_nb == 0)
    {
      // No configuration selected then no interface enable
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
    }
  u8_i = usb_configuration_nb;
  while (u8_i != 0)
    {
      if (CONFIGURATION_DESCRIPTOR
          != ((S_usb_configuration_descriptor*) pbuffer)->bDescriptorType)
        {
          data_to_transfer
              -= ((S_usb_configuration_descriptor*) pbuffer)->bLength;
          pbuffer = (U8*) pbuffer
              + ((S_usb_configuration_descriptor*) pbuffer)->bLength;
          continue;
        }
      u8_i--;
      if (u8_i != 0)
        {
          data_to_transfer
              -= ((S_usb_configuration_descriptor*) pbuffer)->wTotalLength;
          pbuffer = (U8*) pbuffer
              + ((S_usb_configuration_descriptor*) pbuffer)->wTotalLength;
        }
    }

  // Find interface selected
  if (wIndex >= ((S_usb_configuration_descriptor*) pbuffer)->bNumInterfaces)
    {
      // Interface number unknow
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
    }
  while (1)
    {
      if (data_to_transfer <= ((S_usb_interface_descriptor*) pbuffer)->bLength)
        {
          // Interface unknow
          Usb_enable_stall_handshake();
          Usb_ack_receive_setup();
          return;
        }
      data_to_transfer -= ((S_usb_interface_descriptor*) pbuffer)->bLength;
      pbuffer = (U8*) pbuffer
          + ((S_usb_interface_descriptor*) pbuffer)->bLength;
      if (INTERFACE_DESCRIPTOR
          != ((S_usb_interface_descriptor*) pbuffer)->bDescriptorType)
        continue;
      if (wIndex != ((S_usb_interface_descriptor*) pbuffer)->bInterfaceNumber)
        continue;
      if (wValue != ((S_usb_interface_descriptor*) pbuffer)->bAlternateSetting)
        continue;
      usb_interface_status[wIndex] = wValue;
      break;
    }

  //* Find endpoints of interface and reset it
  while (1)
    {
      if (data_to_transfer <= ((S_usb_endpoint_descriptor*) pbuffer)->bLength)
        break; // End of interface
      data_to_transfer -= ((S_usb_endpoint_descriptor*) pbuffer)->bLength;
      pbuffer = (U8*) pbuffer + ((S_usb_endpoint_descriptor*) pbuffer)->bLength;
      if (INTERFACE_DESCRIPTOR
          == ((S_usb_endpoint_descriptor*) pbuffer)->bDescriptorType)
        break; // End of interface
      if (ENDPOINT_DESCRIPTOR
          == ((S_usb_endpoint_descriptor*) pbuffer)->bDescriptorType)
        {
          // Reset endpoint
          u8_i = ((S_usb_endpoint_descriptor*) pbuffer)->bEndpointAddress
              & (~MSK_EP_DIR);
          endpointSelect(u8_i);
          Usb_disable_stall_handshake();
          endpointReset(u8_i);
          Usb_reset_data_toggle();
        }
    }

  endpointSelect(EP_CONTROL);
  // send a ZLP for STATUS phase
  Usb_send_control_in();
  while (!Is_usb_read_control_enabled())
    ;
#endif
}

/*
 * Initialise the main usb variables:
 * - endpoint status
 * - connection status
 */
void
OSUsbDeviceImpl::usb_enum_var_init(void)
{
  unsigned char ep_num;

  for (ep_num = 0; ep_num < NB_ENDPOINTS; ep_num++)
    {
      endpoint_status[ep_num] = 0;
    }
  usb_connected = false; // USB is not connected
  usb_configuration_nb = 0; // Default configuration number is 0
}

/*
 * Set the length and address of data to return
 */
void
OSUsbDeviceImpl::usb_set_return(unsigned char len, PGM_VOID_P pbuf)
{
  data_to_transfer = len;
  pbuffer = pbuf;
}

// ----------------------------------------------------------------------------

//_____ D E F I N I T I O N S ______________________________________________

// usb_user_device_descriptor
const S_usb_device_descriptor usb_dev_desc =
  { sizeof(S_usb_device_descriptor), DEVICE_DESCRIPTOR,
      Usb_format_mcu_to_usb_data(16, USB_SPECIFICATION), DEVICE_CLASS,
      DEVICE_SUB_CLASS, DEVICE_PROTOCOL, EP_CONTROL_LENGTH,
      Usb_format_mcu_to_usb_data(16, VENDOR_ID),
      Usb_format_mcu_to_usb_data(16, PRODUCT_ID),
      Usb_format_mcu_to_usb_data(16, RELEASE_NUMBER), STRING_INDEX_MAN, // ???
      STRING_INDEX_PROD, // ???
      STRING_INDEX_SN, // ???
      NB_CONFIGURATION };

// usb_user_configuration_descriptor FS
const S_usb_user_configuration_descriptor
    usb_conf_desc_fs =
          {
                {
                    sizeof(S_usb_configuration_descriptor),
                    CONFIGURATION_DESCRIPTOR,
                    Usb_format_mcu_to_usb_data(16, sizeof(S_usb_user_configuration_descriptor)),
                    NB_INTERFACE, CONF_NB, CONF_INDEX, CONF_ATTRIBUTES,
                    MAX_POWER },
                { sizeof(S_usb_interface_descriptor), INTERFACE_DESCRIPTOR,
                    IF0_NB, ALTERNATE0, NB_ENDPOINT0, IF0_CLASS, IF0_SUB_CLASS,
                    IF0_PROTOCOL, STRING_INDEX_IF0 },
                { 0x05, // Size of structure
                    // -----------------
                    0x24, // CS_INTERFACE
                    0x00, // Header Functional Descriptor
                    0x10, 0x01, // USB Class Definitions for Communication Devices Specification release number in
                    // binary-coded decimal.

                    0x05, // Size of structure
                    // -----------------
                    0x24, // CS_INTERFACE
                    0x01, // Call Management Functional Descriptor
                    0x03, // The capabilities that this configuration supports:
                    // - Device handles call management itself.
                    // - Device can send/receive call management information over a Data Class interface.
                    0x01, // Interface number of Data Class interface optionally used for call management.

                    0x04, // Size of structure
                    // -----------------
                    0x24, // CS_INTERFACE
                    0x02, // Abstract Control Management Functional Descriptor.
                    0x06, // Abstract Control Management functional descriptor subtype:
                    // - Union Functional descriptor

                    0x05, // Size of structure
                    // -----------------
                    0x24, // CS_INTERFACE
                    0x06, // Union Functional descriptor
                    0x00, // The interface number of the Communication or Data Class interface, designated as
                    // the master or controlling interface for the union.
                    0x01 // Interface number of first slave or associated interface in the union.
                },
                { sizeof(S_usb_endpoint_descriptor), ENDPOINT_DESCRIPTOR,
                    EP_NB_3, EP_ATTRIBUTES_3,
                    Usb_format_mcu_to_usb_data(16, EP_SIZE_3), EP_INTERVAL_3 },
                { sizeof(S_usb_interface_descriptor), INTERFACE_DESCRIPTOR,
                    IF1_NB, ALTERNATE1, NB_ENDPOINT1, IF1_CLASS, IF1_SUB_CLASS,
                    IF1_PROTOCOL, STRING_INDEX_IF1 },
                { sizeof(S_usb_endpoint_descriptor), ENDPOINT_DESCRIPTOR,
                    EP_NB_1, EP_ATTRIBUTES_1,
                    Usb_format_mcu_to_usb_data(16, EP_SIZE_1_FS), EP_INTERVAL_1 },
                { sizeof(S_usb_endpoint_descriptor), ENDPOINT_DESCRIPTOR,
                    EP_NB_2, EP_ATTRIBUTES_2,
                    Usb_format_mcu_to_usb_data(16, EP_SIZE_2_FS), EP_INTERVAL_2 } };

#if (USB_HIGH_SPEED_SUPPORT==ENABLED)

// usb_user_configuration_descriptor HS
const S_usb_user_configuration_descriptor usb_conf_desc_hs =
  {
      { sizeof(S_usb_configuration_descriptor)
        , CONFIGURATION_DESCRIPTOR
        , Usb_format_mcu_to_usb_data(16, sizeof(S_usb_user_configuration_descriptor))
        , NB_INTERFACE
        , CONF_NB
        , CONF_INDEX
        , CONF_ATTRIBUTES
        , MAX_POWER
      }
    ,
      { sizeof(S_usb_interface_descriptor)
        , INTERFACE_DESCRIPTOR
        , INTERFACE_NB_0
        , ALTERNATE_0
        , NB_ENDPOINT_0
        , INTERFACE_CLASS_0
        , INTERFACE_SUB_CLASS_0
        , INTERFACE_PROTOCOL_0
        , INTERFACE_INDEX_0
      }
    ,
      { 0x05, // Size of structure
        // -----------------
        0x24, // CS_INTERFACE
        0x00, // Header Functional Descriptor
        0x10, 0x01, // USB Class Definitions for Communication Devices Specification release number in
        // binary-coded decimal.

        0x05, // Size of structure
        // -----------------
        0x24, // CS_INTERFACE
        0x01, // Call Management Functional Descriptor
        0x03, // The capabilities that this configuration supports:
        // - Device handles call management itself.
        // - Device can send/receive call management information over a Data Class interface.
        0x01, // Interface number of Data Class interface optionally used for call management.

        0x04, // Size of structure
        // -----------------
        0x24, // CS_INTERFACE
        0x02, // Abstract Control Management Functional Descriptor.
        0x06, // Abstract Control Management functional descriptor subtype:
        // - Union Functional descriptor

        0x05, // Size of structure
        // -----------------
        0x24, // CS_INTERFACE
        0x06, // Union Functional descriptor
        0x00, // The interface number of the Communication or Data Class interface, designated as
        // the master or controlling interface for the union.
        0x01 // Interface number of first slave or associated interface in the union.
      }
    ,
      { sizeof(S_usb_endpoint_descriptor)
        , ENDPOINT_DESCRIPTOR
        , ENDPOINT_NB_3
        , EP_ATTRIBUTES_3
        , Usb_format_mcu_to_usb_data(16, EP_SIZE_3)
        , EP_INTERVAL_3
      }
    ,
      { sizeof(S_usb_interface_descriptor)
        , INTERFACE_DESCRIPTOR
        , INTERFACE_NB_1
        , ALTERNATE_1
        , NB_ENDPOINT_1
        , INTERFACE_CLASS_1
        , INTERFACE_SUB_CLASS_1
        , INTERFACE_PROTOCOL_1
        , INTERFACE_INDEX_1
      }
    ,
      { sizeof(S_usb_endpoint_descriptor)
        , ENDPOINT_DESCRIPTOR
        , ENDPOINT_NB_1
        , EP_ATTRIBUTES_1
        , Usb_format_mcu_to_usb_data(16, EP_SIZE_1_HS)
        , EP_INTERVAL_1
      }
    ,
      { sizeof(S_usb_endpoint_descriptor)
        , ENDPOINT_DESCRIPTOR
        , ENDPOINT_NB_2
        , EP_ATTRIBUTES_2
        , Usb_format_mcu_to_usb_data(16, EP_SIZE_2_HS)
        , EP_INTERVAL_2
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
  { sizeof(S_usb_language_id), STRING_DESCRIPTOR,
      Usb_format_mcu_to_usb_data(16, LANGUAGE_ID) };

// usb_user_manufacturer_string_descriptor
const S_usb_manufacturer_string_descriptor
    usb_user_manufacturer_string_descriptor =
      { sizeof(S_usb_manufacturer_string_descriptor), STRING_DESCRIPTOR,
      USB_MANUFACTURER_NAME };

// usb_user_product_string_descriptor
const S_usb_product_string_descriptor usb_user_product_string_descriptor =
  { sizeof(S_usb_product_string_descriptor), STRING_DESCRIPTOR,
  USB_PRODUCT_NAME };

// usb_user_serial_number
const S_usb_serial_number usb_user_serial_number =
  { sizeof(S_usb_serial_number), STRING_DESCRIPTOR,
  USB_SERIAL_NUMBER };

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */

#endif /* defined(OS_CONFIG_FAMILY_AVR32UC3) */

