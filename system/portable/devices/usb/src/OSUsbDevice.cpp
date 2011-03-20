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
    unsigned char OSUsbDevice::usb_interface_status[NB_INTERFACE];
    UnionVPtr OSUsbDevice::pep_fifo[AVR32_USBB_EPT_NUM];
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

    w = usb_format_usb_to_mcu_data(16, w);

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

    l = usb_format_usb_to_mcu_data(32, l);

    return l;
  }

/*
 * Write USB Word, LSB | MSB
 */
void OSUsbDevice::writeWord(unsigned short w)
  {
    w = usb_format_mcu_to_usb_data(16, w);
    writeByte((unsigned char)w & 0xFF);
    w >>= 8;
    writeByte((unsigned char)w & 0xFF);
  }

/*
 * Write USB Long, LSB | MSB
 */
void OSUsbDevice::writeLong(unsigned long l)
  {
    l = usb_format_mcu_to_usb_data(16, l);
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

    endpointSelect(EP_CONTROL);

    Usb_reset_endpoint_fifo_access(EP_CONTROL);
    bmRequestType = readByte();
    bmRequest      = readByte();

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
          OSUsbDevice::usb_get_descriptor();
        }
      else goto unsupported_request;
      break;

    case GET_CONFIGURATION:
      if (bmRequestType == 0x80)
        {
          usb_get_configuration();
        }
      else goto unsupported_request;
      break;

    case SET_ADDRESS:
      if (bmRequestType == 0x00)
        {
          usb_set_address();
        }
      else goto unsupported_request;
      break;

    case SET_CONFIGURATION:
      if (bmRequestType == 0x00)
        {
          usb_set_configuration();
        }
      else goto unsupported_request;
      break;

    case CLEAR_FEATURE:
      if (bmRequestType <= 0x02)
        {
          usb_clear_feature();
        }
      else goto unsupported_request;
      break;

    case SET_FEATURE:
      if (bmRequestType <= 0x02)
        {
          usb_set_feature();
        }
      else goto unsupported_request;
      break;

    case GET_STATUS:
      if (0x7F < bmRequestType && bmRequestType <= 0x82)
        {
          usb_get_status();
        }
      else goto unsupported_request;
      break;

    case GET_INTERFACE:
      if (bmRequestType == 0x81)
      {
        if(!usb_get_interface())
        {
          Usb_enable_stall_handshake();
          Usb_ack_receive_setup();
        }
      }
      else goto unsupported_request;
      break;

    case SET_INTERFACE:
      if (bmRequestType == 0x01) usb_set_interface();
      else goto unsupported_request;
      break;

    case SET_DESCRIPTOR:
    case SYNCH_FRAME:
    default:  //!< unsupported request => call to user read request
  unsupported_request:
      if (!DeviceCharacterUsb::specificCdcProcessReadRequest(bmRequestType, bmRequest))
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
#if TO_BE_PORTED
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
#else
  Bool    zlp;
  U16     wLength, counter;
  U8      descriptor_type;
  U8      string_type;
  Union32 temp;
#if (USB_HIGH_SPEED_SUPPORT==ENABLED)
  Bool    b_first_data = TRUE;
#endif

  zlp             = FALSE;                                  /* no zero length packet */
  string_type     = readByte();  /* read LSB of wValue    */
  descriptor_type = readByte();  /* read MSB of wValue    */

  switch (descriptor_type)
  {
  case DEVICE_DESCRIPTOR:
    //push_in_my_debug_table('1');
    data_to_transfer = Usb_get_dev_desc_length();   //!< sizeof(AVR32_usb_dev_desc);
    pbuffer          = (PGM_VOID_P)Usb_get_dev_desc_pointer();
    break;

#if (USB_HIGH_SPEED_SUPPORT==DISABLED)
  case CONFIGURATION_DESCRIPTOR:
    //push_in_my_debug_table('2');
    data_to_transfer = Usb_get_conf_desc_length();  //!< sizeof(usb_conf_desc);
    //push_in_my_debug_table(data_to_transfer);
    pbuffer          = (PGM_VOID_P)Usb_get_conf_desc_pointer();
    break;

#else
  case CONFIGURATION_DESCRIPTOR:
    if( Is_usb_full_speed_mode() )
    {
       data_to_transfer = Usb_get_conf_desc_fs_length();  //!< sizeof(usb_conf_desc_fs);
       pbuffer          = Usb_get_conf_desc_fs_pointer();
    }else{
       data_to_transfer = Usb_get_conf_desc_hs_length();  //!< sizeof(usb_conf_desc_hs);
       pbuffer          = Usb_get_conf_desc_hs_pointer();
    }
    break;

  case OTHER_SPEED_CONFIGURATION_DESCRIPTOR:
    if( !Is_usb_full_speed_mode() )
    {
       data_to_transfer = Usb_get_conf_desc_fs_length();  //!< sizeof(usb_conf_desc_fs);
       pbuffer          = Usb_get_conf_desc_fs_pointer();
    }else{
       data_to_transfer = Usb_get_conf_desc_hs_length();  //!< sizeof(usb_conf_desc_hs);
       pbuffer          = Usb_get_conf_desc_hs_pointer();
    }
    break;

  case DEVICE_QUALIFIER_DESCRIPTOR:
    data_to_transfer = Usb_get_qualifier_desc_length();  //!< sizeof(usb_qualifier_desc);
    pbuffer          = Usb_get_qualifier_desc_pointer();
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

  temp.u32 = readLong();
  //temp.u32 = AVR32_usb_read_endpoint_data(EP_CONTROL, 32);      //!< read wIndex and wLength with a 32-bit access
                                                          //!< since this access is aligned with a 32-bit
                                                          //!< boundary from the beginning of the endpoint
  wLength = usb_format_usb_to_mcu_data(16, temp.u16[1]);  //!< ignore wIndex, keep and format wLength
  Usb_ack_receive_setup();                          //!< clear the setup received flag

  if (wLength > data_to_transfer)
  {
    zlp = !(data_to_transfer % EP_CONTROL_LENGTH);  //!< zero length packet condition
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
    while (!Is_usb_tx_ready() && !Is_usb_nak_out()) ;

    if (Is_usb_nak_out())
      break;  // don't clear the flag now, it will be cleared after

    Usb_reset_endpoint_fifo_access(EP_CONTROL);

#if (USB_HIGH_SPEED_SUPPORT==ENABLED) // To support other descriptors like OTHER_SPEED_CONFIGURATION_DESCRIPTOR
    if( b_first_data ) {
      b_first_data = FALSE;
      if( 0!= data_to_transfer ) {
        usb_write_ep_txpacket(EP_CONTROL, pbuffer, 1, &pbuffer);
        data_to_transfer--;
      }
      if( 0!= data_to_transfer ) {
        usb_write_ep_txpacket(EP_CONTROL, &descriptor_type, 1, NULL);
        pbuffer = ((const U8*)pbuffer)+1;
        data_to_transfer--;
      }
    }
#endif
    if( 0!= data_to_transfer ) {
//         data_to_transfer = usb_write_ep_txpacket(EP_CONTROL, pbuffer,
//                                                  data_to_transfer, &pbuffer);
        //TODO: replace this with the code above.
        while( !OSUsbDevice::Is_usb_tx_ready() ) ;     // Wait Endpoint ready...
        for(counter=0; counter<UsbGetEndpointSize(EP_CONTROL) ;counter++)
          {
            writeByte(*((char*)pbuffer));

            pbuffer = (char*)pbuffer + 1;
            data_to_transfer--;
            if(data_to_transfer == 0)
              break;
          }
        Usb_send_control_in();
        if(data_to_transfer == 0 && counter == UsbGetEndpointSize(EP_CONTROL))// ZLP
          {
            while( !OSUsbDevice::Is_usb_tx_ready() ) ;     // Wait Endpoint ready...
            OSUsbDevice::Usb_send_in();             // ...and Send ZLP
          }
    }
    if (Is_usb_nak_out())
      break;
    Usb_send_control_in(); //!< Send data until necessary
  }

  if (zlp && !Is_usb_nak_out())
  {
    while (!Is_usb_read_control_enabled()) ;
    Usb_send_control_in();
  }

  while (!Is_usb_nak_out()) ;
  Usb_ack_nak_out();
  while (!isInterruptReceiveOut()) ;
  interruptReceiveOutAck();
#endif
  }

/*
 * Manage the GET CONFIGURATION request. The current
 * configuration number is returned.
 */
void OSUsbDevice::usb_get_configuration(void)
  {
    Usb_ack_receive_setup();

    Usb_reset_endpoint_fifo_access(EP_CONTROL);
    writeByte(usb_configuration_nb);
    Usb_send_control_in();

    while (!isInterruptReceiveOut()) ;
    interruptReceiveOutAck();
  }
/*
 * Manage the SET ADDRESS request. When complete, the device
 * will filter the requests using the new address.
 */
void OSUsbDevice::usb_set_address(void)
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

    Usb_send_control_in();    //!< send a ZLP for STATUS phase
    while ( !OSUsbDevice::Is_usb_in_ready() )
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
void OSUsbDevice::usb_set_configuration(void)
  {
  unsigned char configuration_number, u8_i;

  configuration_number = OSUsbDevice::readByte();
#if defined(DEBUG) && defined(OS_DEBUG_OSUSBDEVICE_REQUEST)
  OSDeviceDebug::putString("SET_CONFIGURATION ");
  OSDeviceDebug::putHex(configuration_number);
  OSDeviceDebug::putNewLine();
#endif

    if (configuration_number <= NB_CONFIGURATION)
    {
      Usb_ack_receive_setup();
      usb_configuration_nb = configuration_number;
      for( u8_i=0; u8_i<NB_INTERFACE; u8_i++) usb_interface_status[u8_i]=0;

      DeviceCharacterUsb::specificCdcEndpointInit(usb_configuration_nb); //!< endpoint configuration
      Usb_set_configuration_action();

      Usb_send_control_in();              //!< send a ZLP for STATUS phase
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
void OSUsbDevice::usb_clear_feature(void)
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
      wValue = OSUsbDevice::readByte();
      dummy = OSUsbDevice::readByte(); // dummy read

      if (wValue == FEATURE_ENDPOINT_HALT)
      {
        wIndex = (OSUsbDevice::readByte() & MSK_EP_DIR );

        OSUsbDevice::endpointSelect(wIndex);
        if (OSUsbDevice::Is_usb_endpoint_enabled())
        {
          if (wIndex != EP_CONTROL)
          {
            Usb_disable_stall_handshake();
            endpointReset(wIndex);
            Usb_reset_data_toggle();

            if(wIndex == RX_EP)
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
void OSUsbDevice::usb_set_feature(void)
  {
#if TO_BE_PORTED
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
#else
    U16 wValue  = usb_format_usb_to_mcu_data(16, readWord());
    U16 wIndex  = usb_format_usb_to_mcu_data(16, readWord());
    U16 wLength = usb_format_usb_to_mcu_data(16, readWord());

    if (wLength)
      goto unsupported_request;

    switch (wValue)
    {
    case FEATURE_ENDPOINT_HALT:
      wIndex = Get_desc_ep_nbr(wIndex);  // clear direction flag

      endpointSelect(wIndex);

      if (bmRequestType != ENDPOINT_TYPE ||
          wIndex == EP_CONTROL ||
          !Is_usb_endpoint_enabled())
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

  unsupported_request:
    endpointSelect(EP_CONTROL);
    Usb_enable_stall_handshake();
    Usb_ack_receive_setup();
#endif
  }

/*
 * Manage the GET STATUS request. The device, interface or
 * endpoint status is returned.
 */
void OSUsbDevice::usb_get_status(void)
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
      OSUsbDevice::Usb_ack_receive_setup();
      OSUsbDevice::writeByte(endpoint_status[ wIndex ]);
#else
      Usb_ack_receive_setup();
      Usb_reset_endpoint_fifo_access(EP_CONTROL);
      writeByte(Is_usb_endpoint_stall_requested(wIndex) );
#endif
      break;

    default:
      Usb_enable_stall_handshake();
      Usb_ack_receive_setup();
      return;
    }

    writeByte(0x00);
    Usb_send_control_in();

    while (!isInterruptReceiveOut()) ;
#if TO_BE_PORTED
    OSUsbDevice::Usb_ack_receive_out();
#else
    interruptReceiveOutAck();
#endif
  }

/*
 * Manage the GET_INTERFACE request.
 */
Bool OSUsbDevice::usb_get_interface(void)
  {
#if TO_BE_PORTED
    OSUsbDevice::Usb_enable_stall_handshake();
    OSUsbDevice::Usb_ack_receive_setup();
#else
    U16   wInterface;
    U16   wValue;

    // Read wValue
    wValue = usb_format_usb_to_mcu_data(16, readWord());
    // wValue = Alternate Setting
    // wIndex = Interface
    wInterface=usb_format_usb_to_mcu_data(16, readWord());
    if(0!=wValue)
       return FALSE;
    Usb_ack_receive_setup();

    Usb_reset_endpoint_fifo_access(EP_CONTROL);
    writeByte( usb_interface_status[wInterface] );
    Usb_send_control_in();

    while( !isInterruptReceiveOut() ) ;
    interruptReceiveOutAck();
    return TRUE;
#endif
  }

/*
 * Manage the SET_INTERFACE request.
 */
void OSUsbDevice::usb_set_interface(void)
  {
#if TO_BE_PORTED
    OSUsbDevice::Usb_ack_receive_setup();
    OSUsbDevice::Usb_send_control_in(); // send a ZLP for STATUS phase
    while ( !OSUsbDevice::Is_usb_in_ready() )
      {
        ;
      }
#else
    U8 u8_i;

    endpointSelect(EP_CONTROL);

    // wValue = Alternate Setting
    // wIndex = Interface
    U16 wValue  = usb_format_usb_to_mcu_data(16, readWord());
    U16 wIndex  = usb_format_usb_to_mcu_data(16, readWord());
    Usb_ack_receive_setup();

    // Get descriptor
  #if (USB_HIGH_SPEED_SUPPORT==ENABLED)
    if( Is_usb_full_speed_mode() )
    {
       data_to_transfer = Usb_get_conf_desc_fs_length();  //!< sizeof(usb_conf_desc_fs);
       pbuffer          = Usb_get_conf_desc_fs_pointer();
    }else{
       data_to_transfer = Usb_get_conf_desc_hs_length();  //!< sizeof(usb_conf_desc_hs);
       pbuffer          = Usb_get_conf_desc_hs_pointer();
    }
  #else
    data_to_transfer = Usb_get_conf_desc_length();  //!< sizeof(usb_conf_desc);
    pbuffer          = (PGM_VOID_P)Usb_get_conf_desc_pointer();
  #endif

    //** Scan descriptor

    //* Find configuration selected
    if( usb_configuration_nb == 0 )
    {
       // No configuration selected then no interface enable
       Usb_enable_stall_handshake();
       Usb_ack_receive_setup();
       return;
    }
    u8_i = usb_configuration_nb;
    while( u8_i != 0 )
    {
       if( CONFIGURATION_DESCRIPTOR != ((S_usb_configuration_descriptor*)pbuffer)->bDescriptorType )
       {
          data_to_transfer -=  ((S_usb_configuration_descriptor*)pbuffer)->bLength;
          pbuffer =  (U8*)pbuffer + ((S_usb_configuration_descriptor*)pbuffer)->bLength;
          continue;
       }
       u8_i--;
       if( u8_i != 0 )
       {
          data_to_transfer -=  ((S_usb_configuration_descriptor*)pbuffer)->wTotalLength;
          pbuffer =  (U8*)pbuffer + ((S_usb_configuration_descriptor*)pbuffer)->wTotalLength;
       }
    }

    // Find interface selected
    if( wIndex >= ((S_usb_configuration_descriptor*)pbuffer)->bNumInterfaces )
    {
       // Interface number unknow
       Usb_enable_stall_handshake();
       Usb_ack_receive_setup();
       return;
    }
    while( 1 )
    {
       if( data_to_transfer <= ((S_usb_interface_descriptor*)pbuffer)->bLength )
       {
          // Interface unknow
          Usb_enable_stall_handshake();
          Usb_ack_receive_setup();
          return;
       }
       data_to_transfer -=  ((S_usb_interface_descriptor*)pbuffer)->bLength;
       pbuffer =  (U8*)pbuffer + ((S_usb_interface_descriptor*)pbuffer)->bLength;
       if( INTERFACE_DESCRIPTOR != ((S_usb_interface_descriptor*)pbuffer)->bDescriptorType )
          continue;
       if( wIndex != ((S_usb_interface_descriptor*)pbuffer)->bInterfaceNumber )
          continue;
       if( wValue != ((S_usb_interface_descriptor*)pbuffer)->bAlternateSetting )
          continue;
       usb_interface_status[wIndex] = wValue;
       break;
    }

    //* Find endpoints of interface and reset it
    while( 1 )
    {
       if( data_to_transfer <= ((S_usb_endpoint_descriptor*)pbuffer)->bLength )
          break;    // End of interface
       data_to_transfer -=  ((S_usb_endpoint_descriptor*)pbuffer)->bLength;
       pbuffer =  (U8*)pbuffer + ((S_usb_endpoint_descriptor*)pbuffer)->bLength;
       if( INTERFACE_DESCRIPTOR == ((S_usb_endpoint_descriptor*)pbuffer)->bDescriptorType )
          break;    // End of interface
       if( ENDPOINT_DESCRIPTOR == ((S_usb_endpoint_descriptor*)pbuffer)->bDescriptorType )
       {
          // Reset endpoint
          u8_i = ((S_usb_endpoint_descriptor*)pbuffer)->bEndpointAddress & (~MSK_EP_DIR);
          endpointSelect(u8_i);
          Usb_disable_stall_handshake();
          endpointReset(u8_i);
          Usb_reset_data_toggle();
       }
    }

    endpointSelect(EP_CONTROL);
    // send a ZLP for STATUS phase
    Usb_send_control_in();
    while (!Is_usb_read_control_enabled()) ;
#endif
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
