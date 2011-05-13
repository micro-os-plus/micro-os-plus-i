/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_CONFIG_FAMILY_AT90USB)

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/kernel/include/OS.h"

#include "hal/arch/avr8/at90usb/devices/usb/include/OSUsbDeviceImpl.h"
#include "portable/devices/character/include/DeviceCharacterUsb.h"

// ----- Static data ----------------------------------------------------------

volatile unsigned short OSUsbDeviceImpl::g_usb_event = 0;
unsigned char OSUsbDeviceImpl::data_to_transfer;
PGM_VOID_P OSUsbDeviceImpl::pbuffer;

bool OSUsbDeviceImpl::zlp;
unsigned char OSUsbDeviceImpl::bmRequestType;
unsigned char OSUsbDeviceImpl::endpoint_status[NB_ENDPOINTS];

bool OSUsbDeviceImpl::usb_connected;

unsigned char OSUsbDeviceImpl::usb_configuration_nb;

//unsigned char OSUsbDeviceImpl::m_selectedEndpoint;

// ----- Constructors ---------------------------------------------------------

OSUsbDeviceImpl::OSUsbDeviceImpl()
{
  OSDeviceDebug::putConstructor_P(PSTR("OSUsbDeviceImpl"), this);
}

// ----------------------------------------------------------------------------

extern void
usb_process_request(void);
extern void
interruptGenServiceRoutine(void);

extern "C" void
USB_GEN_vect(void) __attribute__( ( signal ) );

void
USB_GEN_vect(void)
{
  OSScheduler::ledActiveOn();

  OSUsbDeviceImpl::interruptGenServiceRoutine();
}

#if (OS_CFGBOOL_OSUSBDEVICE_COM_CONTEXT_SWITCH)

extern "C" void USB_COM_vect( void ) __attribute__( ( signal, naked ) );

void
USB_COM_vect( void )
  {
    OSScheduler::contextSave();
      {
        OSScheduler::ledActiveOn();

        OSUsbDeviceImpl::interruptComServiceRoutine();

        if ( OSScheduler::requireContextSwitch() )
          {
            OSScheduler::contextSwitch( true );
          }
      }
    OSScheduler::contextRestore();
    OS::returnFromInterrupt();
  }

#else

extern "C" void
USB_COM_vect(void) __attribute__( ( signal ) );

void
USB_COM_vect(void)
{
  OSScheduler::ledActiveOn();

  OSUsbDeviceImpl::interruptComServiceRoutine();
}

#endif

/* USB Endpoint/Pipe Interrupt Communication Request */

void
OSUsbDeviceImpl::interruptComServiceRoutine(void)
{
  OSUsbLed::toggle();

  //OSDeviceDebug::putString("!");
  if (UEINT & _BV(EP_CONTROL))
    {
      OSUsbDeviceImpl::endpointSelect(EP_CONTROL);
      if (OSUsbDeviceImpl::isInterruptReceiveSetup()
          && OSUsbDeviceImpl::isInterruptReceiveSetupEnabled())
        {
          OSDeviceDebug::putString("Vs ");
          //OSDeviceDebug::putNewLine();

          OSUsbDeviceImpl::standardProcessRequest();
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

void
OSUsbDeviceImpl::interruptGenServiceRoutine(void)
{
  OSUsbLed::toggle();

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
          usb_connected = true;
          OSUsbDeviceImpl::Usb_vbus_on_action();
          OSUsbDeviceImpl::Usb_send_event(EVT_USB_POWERED);
          OSUsbDeviceImpl::Usb_enable_reset_interrupt();
          OSUsbDeviceImpl::usb_start_device();
          OSUsbDeviceImpl::Usb_attach();
        }
      else
        {
          OSUsbDeviceImpl::Usb_vbus_off_action();
          usb_connected = false;
          OSUsbDeviceImpl::Usb_send_event(EVT_USB_UNPOWERED);
        }
    }
  // - Device start of frame received
  if (OSUsbDeviceImpl::Is_usb_sof()
      && OSUsbDeviceImpl::Is_sof_interrupt_enabled())
    {
      OSUsbDeviceImpl::Usb_ack_sof();
      //debug_put_string("uf\n");
      OSUsbDeviceImpl::Usb_sof_action();
    }
  // - Device Suspend event(no more USB activity detected)
  if (OSUsbDeviceImpl::Is_usb_suspend()
      && OSUsbDeviceImpl::Is_suspend_interrupt_enabled())
    {
      OSDeviceDebug::putString("us");
      OSDeviceDebug::putNewLine();

      OSUsbDeviceImpl::Usb_ack_suspend();
      OSUsbDeviceImpl::Usb_enable_wake_up_interrupt();
      OSUsbDeviceImpl::Usb_ack_wake_up(); // clear wake up to detect next event
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
      OSUsbDeviceImpl::usb_init_device();
      OSUsbDeviceImpl::Usb_reset_action();
      OSUsbDeviceImpl::Usb_send_event(EVT_USB_RESET);
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
OSUsbDeviceImpl::usb_init_device(void)
{
  OSUsbDeviceImpl::Usb_select_device();
  if (OSUsbDeviceImpl::Is_usb_id_device())
    {
      OSUsbDeviceImpl::endpointSelect(EP_CONTROL);
      if (!OSUsbDeviceImpl::Is_usb_endpoint_enabled())
        {
          int r;
          r = OSUsbDeviceImpl::usb_configure_endpoint(EP_CONTROL, TYPE_CONTROL,
              DIRECTION_OUT, SIZE_64, ONE_BANK, NYET_DISABLED);
          if (r)
            {
              OSUsbDeviceImpl::interruptReceiveSetupEnable();
            }
          return r;
        }
    }
  return false;
}

bool
OSUsbDeviceImpl::usb_configure_endpoint(unsigned char num, unsigned char type,
    unsigned char dir, unsigned char size, unsigned char bank,
    unsigned char nyet)
{
  OSUsbDeviceImpl::endpointSelect(num);
  return OSUsbDeviceImpl::usb_config_ep((type << 6) | (nyet << 1) | (dir),
      (size << 4) | (bank << 2));
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
  UECFG0X = config0;
  UECFG1X = (UECFG1X & _BV(ALLOC)) | config1;
  OSUsbDeviceImpl::Usb_allocate_memory();
  return (OSUsbDeviceImpl::Is_endpoint_configured());
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
  //PORTD |= _BV(PORTD7);

  OSUsbDeviceImpl::Pll_start_auto();
  while (!OSUsbDeviceImpl::Is_pll_ready())
    ;
  OSUsbDeviceImpl::Usb_unfreeze_clock();
  OSUsbDeviceImpl::Usb_enable_suspend_interrupt();
  OSUsbDeviceImpl::Usb_enable_reset_interrupt();
  OSUsbDeviceImpl::usb_init_device(); // configure the USB controller EP0
  OSUsbDeviceImpl::Usb_attach();
  usb_connected = false;

  //PORTD &= ~_BV(PORTD7);

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

  bmRequestType = OSUsbDeviceImpl::readByte();
  bmRequest = OSUsbDeviceImpl::readByte();

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
    if (0x80 == bmRequestType)
      {
        OSUsbDeviceImpl::usb_get_descriptor();
        return;
      }
    break;

  case GET_CONFIGURATION:
    if (0x80 == bmRequestType)
      {
        usb_get_configuration();
        return;
      }
    break;

  case SET_ADDRESS:
    if (0x00 == bmRequestType)
      {
        usb_set_address();
        return;
      }
    break;

  case SET_CONFIGURATION:
    if (0x00 == bmRequestType)
      {
        usb_set_configuration();
        return;
      }
    break;

  case CLEAR_FEATURE:
    if (0x02 >= bmRequestType)
      {
        usb_clear_feature();
        return;
      }
    break;

  case SET_FEATURE:
    if (0x02 >= bmRequestType)
      {
        usb_set_feature();
        return;
      }
    break;

  case GET_STATUS:
    if ((0x7F < bmRequestType) & (0x82 >= bmRequestType))
      {
        usb_get_status();
        return;
      }
    break;

  case GET_INTERFACE:
    if (bmRequestType == 0x81)
      {
        usb_get_interface();
        return;
      }
    break;

  case SET_INTERFACE:
    if (bmRequestType == 0x01)
      {
        usb_set_interface();
        return;
      }
    break;

  case SET_DESCRIPTOR:
  case SYNCH_FRAME:
  default:
    ;
    }

  // for un-supported requests call the specific read request
  if (OSUsbDeviceImpl::specificProcessReadRequest(bmRequestType, bmRequest))
    return;

  // if nothing worked, reply with stall
  OSUsbDeviceImpl::Usb_enable_stall_handshake();
  OSUsbDeviceImpl::Usb_ack_receive_setup();
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

  dummy16 = OSUsbDevice::readWord(); // don't care of wIndex field
  wLength = OSUsbDevice::readWord(); // read wLength
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
      if ((data_to_transfer % EP_CONTROL_LENGTH) == 0)
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
      data_to_transfer = (unsigned char) wLength; // send only requested number of data
    }

  while ((data_to_transfer != 0) && (!OSUsbDeviceImpl::isInterruptReceiveOut()))
    {
      while (!OSUsbDeviceImpl::Is_usb_read_control_enabled())
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
          pbuffer = (unsigned char *) pbuffer + 1;

          data_to_transfer--;
        }
      OSUsbDeviceImpl::Usb_send_control_in();
    }

  OSUsbDeviceImpl::Usb_send_control_in();

  if (OSUsbDeviceImpl::isInterruptReceiveOut())
    {
      OSUsbDeviceImpl::Usb_ack_receive_out();
      return;
    } // abort from Host

  if (zlp)
    {
      while (!OSUsbDeviceImpl::Is_usb_read_control_enabled())
        {
          ;
        }
      OSUsbDeviceImpl::Usb_send_control_in();
    }

  while (!OSUsbDeviceImpl::isInterruptReceiveOut())
    {
      ;
    }
  OSUsbDeviceImpl::Usb_ack_receive_out();
}

/*
 * Manage the GET CONFIGURATION request. The current
 * configuration number is returned.
 */
void
OSUsbDeviceImpl::usb_get_configuration(void)
{
  OSUsbDeviceImpl::Usb_ack_receive_setup();

  OSUsbDeviceImpl::writeByte(usb_configuration_nb);
  OSUsbDeviceImpl::Usb_ack_in_ready();

  while (!OSUsbDeviceImpl::isInterruptReceiveOut())
    {
      ;
    }
  OSUsbDeviceImpl::Usb_ack_receive_out();
}

/*
 * Manage the SET ADDRESS request. When complete, the device
 * will filter the requests using the new address.
 */
void
OSUsbDeviceImpl::usb_set_address(void)
{
  unsigned char addr;
  addr = OSUsbDeviceImpl::readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("SET_ADDRESS ");
  OSDeviceDebug::putHex(addr);
  OSDeviceDebug::putNewLine();
#endif

  OSUsbDeviceImpl::Usb_configure_address(addr);

  OSUsbDeviceImpl::Usb_ack_receive_setup();

  OSUsbDeviceImpl::Usb_send_control_in(); //!< send a ZLP for STATUS phase
  while (!OSUsbDeviceImpl::Is_usb_in_ready())
    {
      ;
    } //!< waits for status phase done
  //< before using the new address
  OSUsbDeviceImpl::Usb_enable_address();
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
  unsigned char configuration_number;

  configuration_number = OSUsbDeviceImpl::readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("SET_CONFIGURATION ");
  OSDeviceDebug::putHex(configuration_number);
  OSDeviceDebug::putNewLine();
#endif

  if (configuration_number <= NB_CONFIGURATION)
    {
      OSUsbDeviceImpl::Usb_ack_receive_setup();
      usb_configuration_nb = configuration_number;
    }
  else
    {
      // keep that order(set StallRq/clear RxSetup)or a
      // OUT request following the SETUP may be acknowledged
      OSUsbDeviceImpl::Usb_enable_stall_handshake();
      OSUsbDeviceImpl::Usb_ack_receive_setup();
      return;
    }

  OSUsbDeviceImpl::Usb_send_control_in(); // send a ZLP for STATUS phase

  OSUsbDeviceImpl::specificEndpointInit(usb_configuration_nb); // endpoint configuration
  OSUsbDeviceImpl::Usb_set_configuration_action();
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

  if (bmRequestType == ZERO_TYPE)
    {
      // keep that order(set StallRq/clear RxSetup)or a
      // OUT request following the SETUP may be acknowledged
      OSUsbDeviceImpl::Usb_enable_stall_handshake();
      OSUsbDeviceImpl::Usb_ack_receive_setup();
      return;
    }
  else if (bmRequestType == INTERFACE_TYPE)
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
          wIndex = (OSUsbDeviceImpl::readByte() & MSK_EP_DIR);

          OSUsbDeviceImpl::endpointSelect(wIndex);
          if (OSUsbDeviceImpl::Is_usb_endpoint_enabled())
            {
              if (wIndex != EP_CONTROL)
                {
                  OSUsbDeviceImpl::Usb_disable_stall_handshake();
                  OSUsbDeviceImpl::endpointReset(wIndex);
                  OSUsbDeviceImpl::Usb_reset_data_toggle();
                }
              OSUsbDeviceImpl::endpointSelect(EP_CONTROL);
              endpoint_status[wIndex] = 0x00;
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
}

/*
 * Manage the SET FEATURE request. The USB test modes are
 * supported by this function.
 */
void
OSUsbDeviceImpl::usb_set_feature(void)
{
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
          wIndex = (OSUsbDeviceImpl::readByte() & MSK_EP_DIR);

          if (wIndex == EP_CONTROL)
            {
              OSUsbDeviceImpl::Usb_enable_stall_handshake();
              OSUsbDeviceImpl::Usb_ack_receive_setup();
              return;
            }

          OSDeviceDebug::putString(" se");
          OSDeviceDebug::putDec((unsigned short) wIndex);
          OSDeviceDebug::putChar(' ');

          OSUsbDeviceImpl::endpointSelect(wIndex);
          if (OSUsbDeviceImpl::Is_usb_endpoint_enabled())
            {
              OSUsbDeviceImpl::Usb_enable_stall_handshake();
              OSUsbDeviceImpl::endpointSelect(EP_CONTROL);
              endpoint_status[wIndex] = 0x01;
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

  dummy = OSUsbDeviceImpl::readByte(); // dummy read
  dummy = OSUsbDeviceImpl::readByte(); // dummy read
  wIndex = OSUsbDeviceImpl::readByte();

  switch (bmRequestType)
    {
  case REQUEST_DEVICE_STATUS:
    OSUsbDeviceImpl::Usb_ack_receive_setup();
    OSUsbDeviceImpl::writeByte(DEVICE_STATUS);
    break;

  case REQUEST_INTERFACE_STATUS:
    OSUsbDeviceImpl::Usb_ack_receive_setup();
    OSUsbDeviceImpl::writeByte(INTERFACE_STATUS);
    break;

  case REQUEST_ENDPOINT_STATUS:
    OSUsbDeviceImpl::Usb_ack_receive_setup();
    wIndex = wIndex & MSK_EP_DIR;
    OSUsbDeviceImpl::writeByte(endpoint_status[wIndex]);
    break;

  default:
    OSUsbDeviceImpl::Usb_enable_stall_handshake();
    OSUsbDeviceImpl::Usb_ack_receive_setup();
    return;
    }

  OSUsbDeviceImpl::writeByte(0x00);
  OSUsbDeviceImpl::Usb_send_control_in();

  while (!OSUsbDeviceImpl::isInterruptReceiveOut())
    {
      ;
    }
  OSUsbDeviceImpl::Usb_ack_receive_out();
}

/*
 * Manage the GET_INTERFACE request.
 */
void
OSUsbDeviceImpl::usb_get_interface(void)
{
  OSUsbDeviceImpl::Usb_enable_stall_handshake();
  OSUsbDeviceImpl::Usb_ack_receive_setup();
}

/*
 * Manage the SET_INTERFACE request.
 */
void
OSUsbDeviceImpl::usb_set_interface(void)
{
  OSUsbDeviceImpl::Usb_ack_receive_setup();
  OSUsbDeviceImpl::Usb_send_control_in(); // send a ZLP for STATUS phase
  while (!OSUsbDeviceImpl::Is_usb_in_ready())
    {
      ;
    }
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
#define U_LOW(x) ((x) & 0xFF)
#define U_HIGH(x) ((x) >> 8)
#define U_WORD(x) U_LOW(x),U_HIGH(x)
// usb_user_device_descriptor
PROGMEM S_usb_device_descriptor usb_dev_desc =
  { sizeof(usb_dev_desc), DEVICE_DESCRIPTOR, U_WORD( USB_SPECIFICATION ),
      DEVICE_CLASS, DEVICE_SUB_CLASS, DEVICE_PROTOCOL, EP_CONTROL_LENGTH,
      U_WORD( VENDOR_ID ),
    U_WORD( PRODUCT_ID ),
    U_WORD( RELEASE_NUMBER ),  STRING_INDEX_MAN, STRING_INDEX_PROD,
      STRING_INDEX_SN, NB_CONFIGURATION };
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
    { sizeof(S_usb_configuration_descriptor), CONFIGURATION_DESCRIPTOR,
        U_WORD(sizeof(usb_conf2_desc)),
        NB2_INTERFACE,  CONF2_NB, CONF_INDEX,
        CONF_ATTRIBUTES, MAX_POWER },
    { sizeof(S_usb_interface_descriptor), INTERFACE_DESCRIPTOR, IF0_NB,
        ALTERNATE0, NB_ENDPOINT0, IF0_CLASS, IF0_SUB_CLASS, IF0_PROTOCOL,
        STRING_INDEX_IF0 },
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
      },
      { sizeof(S_usb_endpoint_descriptor), ENDPOINT_DESCRIPTOR, EP_NB_3,
          EP_ATTRIBUTES_3, U_WORD( EP_SIZE_3 ), EP_INTERVAL_3 },
      { sizeof(S_usb_interface_descriptor), INTERFACE_DESCRIPTOR, IF1_NB,
          ALTERNATE1, NB_ENDPOINT1, IF1_CLASS, IF1_SUB_CLASS, IF1_PROTOCOL,
          STRING_INDEX_IF1 },
      { sizeof(S_usb_endpoint_descriptor), ENDPOINT_DESCRIPTOR, EP_NB_1,
          EP_ATTRIBUTES_1, U_WORD( EP_SIZE_1 ), EP_INTERVAL_1 },
      { sizeof(S_usb_endpoint_descriptor), ENDPOINT_DESCRIPTOR, EP_NB_2,
          EP_ATTRIBUTES_2, U_WORD( EP_SIZE_2 ), EP_INTERVAL_2 }

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

}    ;

// usb_user_language_id

PROGMEM S_usb_language_id usb_user_language_id =
  { sizeof(usb_user_language_id), STRING_DESCRIPTOR, U_WORD( LANGUAGE_ID )};

// usb_user_manufacturer_string_descriptor
PROGMEM S_usb_manufacturer_string_descriptor
    usb_user_manufacturer_string_descriptor =
      { sizeof(usb_user_manufacturer_string_descriptor), STRING_DESCRIPTOR,
      USB_MANUFACTURER_NAME };
// usb_user_product_string_descriptor
PROGMEM S_usb_product_string_descriptor usb_user_product_string_descriptor =
  { sizeof(usb_user_product_string_descriptor), STRING_DESCRIPTOR,
  USB_PRODUCT_NAME };
// usb_user_serial_number
PROGMEM S_usb_serial_number usb_user_serial_number =
  { sizeof(usb_user_serial_number), STRING_DESCRIPTOR,
  USB_SERIAL_NUMBER };

// ----------------------------------------------------------------------------

#endif /* defined(OS_INCLUDE_OSUSBDEVICE) */

#endif /* defined(OS_CONFIG_FAMILY_AT90USB) */

