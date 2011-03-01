/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSUSBDEVICE)

#include "portable/kernel/include/OS.h"

#include "portable/devices/usb/include/OSUsbDevice.h"
#include "portable/devices/character/include/DeviceCharacterUsb.h"
#include "portable/devices/usb/include/OSUsbDeviceDescriptors.h"

/*
 * This file contains the USB endpoint 0 management routines corresponding to
 * the USB device specification.
 */

// ----- static data ---------------------------------------------------------
    volatile unsigned short OSUsbDevice::g_usb_event = 0;
    unsigned char OSUsbDevice::data_to_transfer;
    PGM_VOID_P OSUsbDevice::pbuffer;

    bool OSUsbDevice::zlp;
    unsigned char OSUsbDevice::bmRequestType;
    unsigned char OSUsbDevice::endpoint_status[NB_ENDPOINTS];

    bool OSUsbDevice::usb_connected;

    unsigned char OSUsbDevice::usb_configuration_nb;

    unsigned char OSUsbDevice::m_selectedEndpoint;
// ---------------------------------------------------------------------------

OSUsbDevice::OSUsbDevice()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString("OSUsbDevice()=");
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif

    OSUsbLed::init();
  }

/*
 * Read USB Word, LSB | MSB
 */
unsigned short OSUsbDevice::readWord(void)
  {
    unsigned short w;

    w = readByte();
    w |= (readByte() << 8);
    return w;
  }

/*
 * Read USB Long, LSB | MSB
 */
unsigned long OSUsbDevice::readLong(void)
  {
    unsigned long l;

    l = readByte();
    l |= (((unsigned long)readByte()) << 8);
    l |= (((unsigned long)readByte()) << 16);
    l |= (((unsigned long)readByte()) << 24);
    return l;
  }

/*
 * Write USB Word, LSB | MSB
 */
void OSUsbDevice::writeWord(unsigned short w)
  {
    writeByte((unsigned char)w & 0xFF);
    w >>= 8;
    writeByte((unsigned char)w & 0xFF);
  }

/*
 * Write USB Long, LSB | MSB
 */
void OSUsbDevice::writeLong(unsigned long l)
  {
    writeByte((unsigned char)l & 0xFF);
    l >>= 8;
    writeByte((unsigned char)l & 0xFF);
    l >>= 8;
    writeByte((unsigned char)l & 0xFF);
    l >>= 8;
    writeByte((unsigned char)l & 0xFF);
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
void OSUsbDevice::standardProcessRequest(void)
  {
    unsigned char bmRequest;

    bmRequestType = OSUsbDevice::readByte();
    bmRequest = OSUsbDevice::readByte();

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
      if ( 0x80 == bmRequestType)
        {
          OSUsbDevice::usb_get_descriptor();
          return;
        }
      break;

    case GET_CONFIGURATION:
      if ( 0x80 == bmRequestType)
        {
          usb_get_configuration();
          return;
        }
      break;

    case SET_ADDRESS:
      if ( 0x00 == bmRequestType)
        {
          usb_set_address();
          return;
        }
      break;

    case SET_CONFIGURATION:
      if ( 0x00 == bmRequestType)
        {
          usb_set_configuration();
          return;
        }
      break;

    case CLEAR_FEATURE:
      if ( 0x02 >= bmRequestType)
        {
          usb_clear_feature();
          return;
        }
      break;

    case SET_FEATURE:
      if ( 0x02 >= bmRequestType)
        {
          usb_set_feature();
          return;
        }
      break;

    case GET_STATUS:
      if ( ( 0x7F < bmRequestType ) & ( 0x82 >= bmRequestType ))
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
    if (OSUsbDevice::specificProcessReadRequest(bmRequestType, bmRequest))
      return;

    // if nothing worked, reply with stall
    OSUsbDevice::Usb_enable_stall_handshake();
    OSUsbDevice::Usb_ack_receive_setup();
  }

/*
 * Redirector to specific read request
 */
bool OSUsbDevice::specificProcessReadRequest(unsigned char type,
    unsigned char request)
  {
    if (DeviceCharacterUsb::specificCdcProcessReadRequest(type, request))
      return true;

    return false;
  }

/*
 * Redirector to specific endpoint init
 */
void OSUsbDevice::specificEndpointInit(unsigned char conf_nb)
  {
    DeviceCharacterUsb::specificCdcEndpointInit(conf_nb);
    return;
  }

/*
 * Redirector to specific get descriptor
 */
bool OSUsbDevice::specificGetDescriptor(unsigned char type, unsigned char string)
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
void OSUsbDevice::usb_get_descriptor(void)
  {
    unsigned short wLength;
    unsigned char descriptor_type;
    unsigned char descriptor_index;
    //U8 dummy;
    unsigned short dummy16;
    unsigned char nb_byte;

    zlp = false; /* no zero length packet */
    descriptor_index = OSUsbDevice::readByte(); /* read LSB of wValue    */
    descriptor_type = OSUsbDevice::readByte(); /* read MSB of wValue    */

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
          OSUsbDevice::Usb_enable_stall_handshake();
          OSUsbDevice::Usb_ack_receive_setup();
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

      OSUsbDevice::Usb_enable_stall_handshake();
      OSUsbDevice::Usb_ack_receive_setup();
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

    OSUsbDevice::Usb_ack_receive_setup(); // clear the receive setup flag

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
        && ( !OSUsbDevice::isInterruptReceiveOut() ))
      {
        while ( !OSUsbDevice::Is_usb_read_control_enabled() )
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
            OSUsbDevice::writeByte(PROGMEM_READ_BYTE(pbuffer));
            //OSUsbDevice::writeByte(pgm_read_byte_near( ( unsigned int ) pbuffer ));
#else
            OSUsbDevice::writeByte( *(( unsigned char * ) pbuffer) );
#endif
            pbuffer = ( unsigned char * ) pbuffer + 1;

            data_to_transfer--;
          }
        OSUsbDevice::Usb_send_control_in();
      }

    OSUsbDevice::Usb_send_control_in();

    if (OSUsbDevice::isInterruptReceiveOut() )
      {
        OSUsbDevice::Usb_ack_receive_out();
        return;
      } // abort from Host

    if (zlp)
      {
        while ( !OSUsbDevice::Is_usb_read_control_enabled() )
          {
            ;
          }
        OSUsbDevice::Usb_send_control_in();
      }

    while ( !OSUsbDevice::isInterruptReceiveOut() )
      {
        ;
      }
    OSUsbDevice::Usb_ack_receive_out();
  }

/*
 * Manage the GET CONFIGURATION request. The current
 * configuration number is returned.
 */
void OSUsbDevice::usb_get_configuration(void)
  {
    OSUsbDevice::Usb_ack_receive_setup();

    OSUsbDevice::writeByte(usb_configuration_nb);
    OSUsbDevice::Usb_ack_in_ready();

    while ( !OSUsbDevice::isInterruptReceiveOut() )
      {
        ;
      }
    OSUsbDevice::Usb_ack_receive_out();
  }

/*
 * Manage the SET ADDRESS request. When complete, the device
 * will filter the requests using the new address.
 */
void OSUsbDevice::usb_set_address(void)
  {
    unsigned char addr;
    addr = OSUsbDevice::readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
    OSDeviceDebug::putString("SET_ADDRESS ");
    OSDeviceDebug::putHex(addr);
    OSDeviceDebug::putNewLine();
#endif

    OSUsbDevice::Usb_configure_address(addr);

    OSUsbDevice::Usb_ack_receive_setup();

    OSUsbDevice::Usb_send_control_in(); //!< send a ZLP for STATUS phase
    while ( !OSUsbDevice::Is_usb_in_ready() )
      {
        ;
      } //!< waits for status phase done
    //< before using the new address
    OSUsbDevice::Usb_enable_address();
  }

/*
 * Manage the SET CONFIGURATION request. If the selected
 * configuration is valid, this function call the specificEndpointInit()
 * function that will configure the endpoints following the configuration
 * number.
 */
void OSUsbDevice::usb_set_configuration(void)
  {
    unsigned char configuration_number;

    configuration_number = OSUsbDevice::readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
    OSDeviceDebug::putString("SET_CONFIGURATION ");
    OSDeviceDebug::putHex(configuration_number);
    OSDeviceDebug::putNewLine();
#endif

    if (configuration_number <= NB_CONFIGURATION)
      {
        OSUsbDevice::Usb_ack_receive_setup();
        usb_configuration_nb = configuration_number;
      }
    else
      {
        // keep that order(set StallRq/clear RxSetup)or a
        // OUT request following the SETUP may be acknowledged
        OSUsbDevice::Usb_enable_stall_handshake();
        OSUsbDevice::Usb_ack_receive_setup();
        return;
      }

    OSUsbDevice::Usb_send_control_in(); // send a ZLP for STATUS phase

    OSUsbDevice::specificEndpointInit(usb_configuration_nb); // endpoint configuration
    OSUsbDevice::Usb_set_configuration_action();
  }

/*
 * Manage the SET FEATURE request.
 */
void OSUsbDevice::usb_clear_feature(void)
  {
    unsigned char wValue;
    unsigned char wIndex;
    unsigned char dummy;

    if (bmRequestType == ZERO_TYPE)
      {
        // keep that order(set StallRq/clear RxSetup)or a
        // OUT request following the SETUP may be acknowledged
        OSUsbDevice::Usb_enable_stall_handshake();
        OSUsbDevice::Usb_ack_receive_setup();
        return;
      }
    else if (bmRequestType == INTERFACE_TYPE)
      {
        // keep that order(set StallRq/clear RxSetup)or a
        // OUT request following the SETUP may be acknowledged
        OSUsbDevice::Usb_enable_stall_handshake();
        OSUsbDevice::Usb_ack_receive_setup();
        return;
      }
    else if (bmRequestType == ENDPOINT_TYPE)
      {
        wValue = OSUsbDevice::readByte();
        dummy = OSUsbDevice::readByte(); // dummy read

        if (wValue == FEATURE_ENDPOINT_HALT)
          {
            wIndex = (OSUsbDevice::readByte() & MSK_EP_DIR );

            OSUsbDevice::endpointSelect(wIndex);
            if (OSUsbDevice::Is_usb_endpoint_enabled() )
              {
                if (wIndex != EP_CONTROL)
                  {
                    OSUsbDevice::Usb_disable_stall_handshake();
                    OSUsbDevice::endpointReset(wIndex);
                    OSUsbDevice::Usb_reset_data_toggle();
                  }
                OSUsbDevice::endpointSelect(EP_CONTROL);
                endpoint_status[ wIndex ] = 0x00;
                OSUsbDevice::Usb_ack_receive_setup();
                OSUsbDevice::Usb_send_control_in();
              }
            else
              {
                OSUsbDevice::Usb_enable_stall_handshake();
                OSUsbDevice::Usb_ack_receive_setup();
                return;
              }
          }
        else
          {
            OSUsbDevice::Usb_enable_stall_handshake();
            OSUsbDevice::Usb_ack_receive_setup();
            return;
          }
      }
  }

/*
 * Manage the SET FEATURE request. The USB test modes are
 * supported by this function.
 */
void OSUsbDevice::usb_set_feature(void)
  {
    unsigned char wValue;
    unsigned char wIndex;
    unsigned char dummy;

    if (bmRequestType == INTERFACE_TYPE)
      {
        // keep that order(set StallRq/clear RxSetup)or a
        // OUT request following the SETUP may be acknowledged
        OSUsbDevice::Usb_enable_stall_handshake();
        OSUsbDevice::Usb_ack_receive_setup();
        return;
      }
    else if (bmRequestType == ENDPOINT_TYPE)
      {
        wValue = OSUsbDevice::readByte();
        dummy = OSUsbDevice::readByte(); // dummy read

        if (wValue == FEATURE_ENDPOINT_HALT)
          {
            wIndex = (OSUsbDevice::readByte() & MSK_EP_DIR );

            if (wIndex == EP_CONTROL)
              {
                OSUsbDevice::Usb_enable_stall_handshake();
                OSUsbDevice::Usb_ack_receive_setup();
                return;
              }

            OSDeviceDebug::putString(" se");
            OSDeviceDebug::putDec((unsigned short)wIndex);
            OSDeviceDebug::putChar(' ');

            OSUsbDevice::endpointSelect(wIndex);
            if (OSUsbDevice::Is_usb_endpoint_enabled() )
              {
                OSUsbDevice::Usb_enable_stall_handshake();
                OSUsbDevice::endpointSelect(EP_CONTROL);
                endpoint_status[ wIndex ] = 0x01;
                OSUsbDevice::Usb_ack_receive_setup();
                OSUsbDevice::Usb_send_control_in();
              }
            else
              {
                OSUsbDevice::Usb_enable_stall_handshake();
                OSUsbDevice::Usb_ack_receive_setup();
                return;
              }
          }
        else
          {
            OSUsbDevice::Usb_enable_stall_handshake();
            OSUsbDevice::Usb_ack_receive_setup();
            return;
          }
      }
  }

/*
 * Manage the GET STATUS request. The device, interface or
 * endpoint status is returned.
 */
void OSUsbDevice::usb_get_status(void)
  {
    unsigned char wIndex;
    unsigned char dummy;

    dummy = OSUsbDevice::readByte(); // dummy read
    dummy = OSUsbDevice::readByte(); // dummy read
    wIndex = OSUsbDevice::readByte();

    switch (bmRequestType)
      {
    case REQUEST_DEVICE_STATUS:
      OSUsbDevice::Usb_ack_receive_setup();
      OSUsbDevice::writeByte(DEVICE_STATUS);
      break;

    case REQUEST_INTERFACE_STATUS:
      OSUsbDevice::Usb_ack_receive_setup();
      OSUsbDevice::writeByte(INTERFACE_STATUS);
      break;

    case REQUEST_ENDPOINT_STATUS:
      OSUsbDevice::Usb_ack_receive_setup();
      wIndex = wIndex & MSK_EP_DIR;
      OSUsbDevice::writeByte(endpoint_status[ wIndex ]);
      break;

    default:
      OSUsbDevice::Usb_enable_stall_handshake();
      OSUsbDevice::Usb_ack_receive_setup();
      return;
      }

    OSUsbDevice::writeByte(0x00);
    OSUsbDevice::Usb_send_control_in();

    while ( !OSUsbDevice::isInterruptReceiveOut() )
      {
        ;
      }
    OSUsbDevice::Usb_ack_receive_out();
  }

/*
 * Manage the GET_INTERFACE request.
 */
void OSUsbDevice::usb_get_interface(void)
  {
    OSUsbDevice::Usb_enable_stall_handshake();
    OSUsbDevice::Usb_ack_receive_setup();
  }

/*
 * Manage the SET_INTERFACE request.
 */
void OSUsbDevice::usb_set_interface(void)
  {
    OSUsbDevice::Usb_ack_receive_setup();
    OSUsbDevice::Usb_send_control_in(); // send a ZLP for STATUS phase
    while ( !OSUsbDevice::Is_usb_in_ready() )
      {
        ;
      }
  }

/*
 * Initialise the main usb variables:
 * - endpoint status
 * - connection status
 */
void OSUsbDevice::usb_enum_var_init(void)
  {
    unsigned char ep_num;

    for (ep_num = 0; ep_num < NB_ENDPOINTS; ep_num++)
      {
        endpoint_status[ ep_num ] = 0;
      }
    usb_connected = false; // USB is not connected
    usb_configuration_nb = 0; // Default configuration number is 0
  }

/*
 * Set the length and address of data to return
 */
void OSUsbDevice::usb_set_return(unsigned char len, PGM_VOID_P pbuf)
  {
    data_to_transfer = len;
    pbuffer = pbuf;
  }

#endif
