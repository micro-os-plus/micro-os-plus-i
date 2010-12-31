/*
 *      Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSUSBDEVICE) && defined(OS_CONFIG_FAMILY_AT90USB)

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbDevice.h"
#include "portable/devices/character/include/DeviceCharacterUsb.h"

extern void usb_process_request(void);
extern void interruptGenServiceRoutine(void);

extern "C"
void  USB_GEN_vect( void ) __attribute__( ( signal ) );

void USB_GEN_vect(void)
  {
    OSScheduler::ledActiveOn();

    OSUsbDevice::interruptGenServiceRoutine();
  }

#if (OS_CFGBOOL_OSUSBDEVICE_COM_CONTEXT_SWITCH)

extern "C" void USB_COM_vect( void ) __attribute__( ( signal, naked ) );

void
USB_COM_vect( void )
  {
    OSScheduler::contextSave();
      {
        OSScheduler::ledActiveOn();

        OSUsbDevice::interruptComServiceRoutine();

        if ( OSScheduler::requireContextSwitch() )
          {
            OSScheduler::contextSwitch( true );
          }
      }
    OSScheduler::contextRestore();
    OS::returnFromInterrupt();
  }

#else

extern "C"
void  USB_COM_vect( void ) __attribute__( ( signal ) );

void USB_COM_vect(void)
  {
    OSScheduler::ledActiveOn();

    OSUsbDevice::interruptComServiceRoutine();
  }

#endif

/* USB Endpoint/Pipe Interrupt Communication Request */

void OSUsbDevice::interruptComServiceRoutine(void)
  {
    OSUsbLed::toggle();

    //OSDeviceDebug::putString("!");
    if (UEINT & _BV(EP_CONTROL))
      {
        OSUsbDevice::endpointSelect(EP_CONTROL);
        if (OSUsbDevice::isInterruptReceiveSetup()
            && OSUsbDevice::isInterruptReceiveSetupEnabled())
          {
            OSDeviceDebug::putString("Vs ");
            //OSDeviceDebug::putNewLine();

            OSUsbDevice::standardProcessRequest();
          }
      }
    DeviceCharacterUsb::specificCdcInterruptServiceRoutine();

    OSUsbLed::toggle();
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

void OSUsbDevice::interruptGenServiceRoutine(void)
  {
    OSUsbLed::toggle();

    // ---------- DEVICE events management -----------------------------------
    //- VBUS state detection
    if (OSUsbDevice::Is_usb_vbus_transition()
        && OSUsbDevice::Is_usb_vbus_interrupt_enabled() )
      {
        OSDeviceDebug::putString("uv");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_ack_vbus_transition();
        if (OSUsbDevice::Is_usb_vbus_high() )
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
    if (OSUsbDevice::Is_usb_sof() && OSUsbDevice::Is_sof_interrupt_enabled() )
      {
        OSUsbDevice::Usb_ack_sof();
        //debug_put_string("uf\n");
        OSUsbDevice::Usb_sof_action();
      }
    // - Device Suspend event(no more USB activity detected)
    if (OSUsbDevice::Is_usb_suspend()
        && OSUsbDevice::Is_suspend_interrupt_enabled() )
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
        && OSUsbDevice::Is_swake_up_interrupt_enabled() )
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
        && OSUsbDevice::Is_resume_interrupt_enabled() )
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
    if (OSUsbDevice::Is_usb_reset()
        && OSUsbDevice::Is_reset_interrupt_enabled() )
      {
        OSDeviceDebug::putString("ur");
        OSDeviceDebug::putNewLine();

        OSUsbDevice::Usb_ack_reset();
        OSUsbDevice::usb_init_device();
        OSUsbDevice::Usb_reset_action();
        OSUsbDevice::Usb_send_event(EVT_USB_RESET);
      }

    OSUsbLed::toggle();
  }

// usb_init_device.
//
// This function initializes the USB device controller and
// configures the Default Control Endpoint.
//
//
// @param none
//
// @return status
//
unsigned char OSUsbDevice::usb_init_device(void)
  {
    OSUsbDevice::Usb_select_device();
    if (OSUsbDevice::Is_usb_id_device() )
      {
        OSUsbDevice::endpointSelect(EP_CONTROL);
        if ( !OSUsbDevice::Is_usb_endpoint_enabled() )
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

bool OSUsbDevice::usb_configure_endpoint(unsigned char num, unsigned char type,
    unsigned char dir, unsigned char size, unsigned char bank,
    unsigned char nyet)
  {
    OSUsbDevice::endpointSelect(num);
    return OSUsbDevice::usb_config_ep((type<<6) |(nyet<<1) |(dir), (size<<4)
        | (bank<<2));
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
bool OSUsbDevice::usb_config_ep(unsigned char config0, unsigned char config1)
  {
    OSUsbDevice::Usb_enable_endpoint();
    UECFG0X = config0;
    UECFG1X = (UECFG1X & _BV(ALLOC)) | config1;
    OSUsbDevice::Usb_allocate_memory();
    return (OSUsbDevice::Is_endpoint_configured() );
  }

// @brief This function initializes the USB device controller
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
void OSUsbDevice::usb_start_device(void)
  {
    //PORTD |= _BV(PORTD7);

    OSUsbDevice::Pll_start_auto();
    while (!OSUsbDevice::Is_pll_ready())
      ;
    OSUsbDevice::Usb_unfreeze_clock();
    OSUsbDevice::Usb_enable_suspend_interrupt();
    OSUsbDevice::Usb_enable_reset_interrupt();
    OSUsbDevice::usb_init_device(); // configure the USB controller EP0
    OSUsbDevice::Usb_attach();
    usb_connected = false;

    //PORTD &= ~_BV(PORTD7);

  }

#endif
