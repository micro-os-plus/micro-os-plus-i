/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECHARACTERUSB_H_
#define DEVICECHARACTERUSB_H_

#include "portable/devices/character/include/OSDeviceCharacter.h"
#include "portable/devices/usb/include/OSUsbDevice.h"

#define USB_CDC_GET_LINE_CODING         0x21
#define USB_CDC_SET_LINE_CODING         0x20
#define USB_CDC_SET_CONTROL_LINE_STATE  0x22
#define USB_CDC_SEND_BREAK              0x23

class DeviceCharacterUsb : public OSDeviceCharacter
{
public:
  DeviceCharacterUsb(int device = 0);
  virtual
  ~DeviceCharacterUsb();

  void
  setOpened(bool flag);

private:
  // implementation virtuals
  virtual int
  implOpen(void);
  virtual OSEvent_t
  implGetOpenEvent(void);

  virtual int
  implClose(void);

  virtual bool
  implIsConnected(void) const;

  virtual bool
  implCanWrite(void);
  virtual OSEvent_t
  implGetWriteEvent(void);
  virtual int
  implWriteByte(unsigned char b);

  virtual int
  implFlush(void);

  virtual bool
  implCanRead(void);
  virtual int
  implAvailableRead(void);
  virtual OSEvent_t
  implGetReadEvent(void);
  virtual int
  implReadByte(void);
  virtual int
  implReadBytes(unsigned char* pBuf, int size);
  virtual int
  implWriteBytes(const unsigned char* pBuf, int size);

  friend class OSUsbDevice;
  friend class OSUsbDeviceImpl;

  static void
  specificCdcInterruptServiceRoutine(void);

  static bool
  specificCdcProcessReadRequest(unsigned char type, unsigned char request);
  static void
  specificCdcEndpointInit(unsigned char confNum);
  static bool
  specificCdcGetDescriptor(unsigned char type, unsigned char index);

  void
  cdcGetLineCoding(void);
  void
  cdcSetLineCoding(void);
  void
  cdcSetControlLineState(void);

  typedef struct
  {
    unsigned long baudRate;
    unsigned char charFormat;
    unsigned char parityType;
    unsigned char dataBits;
  } UsbCdcLineCoding_t;

  UsbCdcLineCoding_t m_lineCoding;

  // local members
  int m_device;

  bool m_opened;
  int m_txCounter;
  int m_rxCounter;

  int m_tx_ep;
  int m_rx_ep;
  int m_int_ep;
};

#if true
extern DeviceCharacterUsb *g_usb0;
#if defined(OS_INCLUDE_USB_CDC_DUAL_INTERFACE)
extern DeviceCharacterUsb *g_usb1;
#endif
#endif

#endif /* DEVICECHARACTERUSB_H_ */
