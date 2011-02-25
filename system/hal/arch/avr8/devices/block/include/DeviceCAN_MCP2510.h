/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECAN_MCP2510_H_
#define DEVICECAN_MCP2510_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceCAN.h"
#include "portable/devices/misc/include/DeviceCANTransceiver.h"

#include "portable/devices/misc/include/DeviceMCP2510.h"

#define CAN_PUT_TX0_INCLUDED
#define CAN_GET_RX0_INCLUDED
#define CAN_GET_RX1_INCLUDED
#define CAN_ENABLE_INTERRUPTS
#define CAN_TX0_ISR_INCLUDED
#define CAN_DUMP_INCLUDED
#define DEBUG_CAN_MERR
#define DEBUG_CAN_IERR

class DeviceCAN_MCP2510 : public OSDeviceCAN, public DeviceCANTransceiver
  {
public:
  DeviceCAN_MCP2510(CANPacket *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, CANPacket *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  static void interruptInit(void);
  static void interruptEnable(void);
  static void interruptDisable(void);

  static void busLockInit(void);
  static void busLock(void);
  static void busUnlock(void);

  static void interruptServiceRoutine(void);

  static DeviceCAN_MCP2510 *pThis;

private:
  void implInit(unsigned char mode);
  //bool implIsConnected(void);

  bool implCanWrite(void);
  int implWritePacket(CANPacket *p);

  bool implCanRead(void);
  int implAvailableRead(void);
  int implReadPacket(CANPacket *p);

  void implTransmit(void);
  // implementation methods

  // mcp2510 inspired methods
  static unsigned char get_ICOD(void);
  static void put_reg(unsigned char reg, unsigned char val);
  static unsigned char get_reg(unsigned char reg);
  static void clear_inte_bits(unsigned char mask);
  static void clear_intf_bits(unsigned char mask);
  static void get_RX0(CANPacket *p);
  static void get_RX1(CANPacket *p);
  static void tx0_transmit(void);
  static void put_TX0(CANPacket *p);
  static void tx1_transmit(void);
  static void put_TX1(CANPacket *p);
  static void tx2_transmit(void);
  static void put_TX2(CANPacket *p);
  static void send(CANPacket *p);
  static unsigned char receive(CANPacket *p);

  // implementation methods
  static unsigned char _getICOD(void);

  static void _set_CANINTF_bits(unsigned char mask);
  static void _clear_CANINTF_bits(unsigned char mask);
  static void _set_CANINTE_bits(unsigned char mask);
  static void _clear_CANINTE_bits(unsigned char mask);
  static unsigned char _get_mode(void);
  static unsigned char _set_mode(unsigned char mask);
  static unsigned char _read_status(void);
  static unsigned char _get_reg(unsigned char reg);
  static void _put_reg(unsigned char reg, unsigned char data);
  static void _rts(unsigned char channel);
  static void _transmit(void);
  static void _tx0_transmit(void);
  static void _tx1_transmit(void);
  static void _tx2_transmit(void);

  static bool tx0_isr(CANPacket *p);

  };

inline void DeviceCAN_MCP2510::interruptInit(void)
  {
    // disable interrupt
    interruptDisable();

    // low level on INT5 generates an interrupt request
    EICRB &= ~(_BV(ISC51) | _BV(ISC50));

    // clear flag by writing a 1 to it
    EIFR |= _BV(5);
  }

inline void DeviceCAN_MCP2510::interruptEnable(void)
  {
    // enable interrupt5
    EIMSK |= _BV(INT5);
  }

inline void DeviceCAN_MCP2510::interruptDisable(void)
  {
    // disable interrupt5
    EIMSK &= ~_BV(INT5);
  }

#define  APP_CONFIG_CAN_BUSLOCK_PORT_INIT               DDRC
#define  APP_CONFIG_CAN_BUSLOCK_RS_PORT                 PORTC
#define  APP_CONFIG_CAN_BUSLOCK_RS_BIT                  3

inline void DeviceCAN_MCP2510::busLockInit(void)
  {
    APP_CONFIG_CAN_BUSLOCK_RS_PORT |= _BV(APP_CONFIG_CAN_BUSLOCK_RS_BIT);
    APP_CONFIG_CAN_BUSLOCK_PORT_INIT |= _BV(APP_CONFIG_CAN_BUSLOCK_RS_BIT);
  }

inline void DeviceCAN_MCP2510::busLock(void)
  {
    APP_CONFIG_CAN_BUSLOCK_RS_PORT &= ~_BV(APP_CONFIG_CAN_BUSLOCK_RS_BIT);
  }

inline void DeviceCAN_MCP2510::busUnlock(void)
  {
    APP_CONFIG_CAN_BUSLOCK_RS_PORT |= _BV(APP_CONFIG_CAN_BUSLOCK_RS_BIT);
  }

// ----------------------------------------------------------------------------

inline unsigned char DeviceCAN_MCP2510::_getICOD(void)
  {
    unsigned char ret;
    ret = DeviceMCP2510::cmdRead(DeviceMCP2510::CANSTAT);
    return ((ret>>1)&0x07);
  }

inline void DeviceCAN_MCP2510::_set_CANINTF_bits(unsigned char mask)
  {
    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::CANINTF, mask, 0xFF);
  }

inline void DeviceCAN_MCP2510::_clear_CANINTF_bits(unsigned char mask)
  {
    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::CANINTF, mask, 0x00);
  }

inline void DeviceCAN_MCP2510::_set_CANINTE_bits(unsigned char mask)
  {
    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::CANINTE, mask, 0xFF);
  }

inline void DeviceCAN_MCP2510::_clear_CANINTE_bits(unsigned char mask)
  {
    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::CANINTE, mask, 0x00);
  }

inline unsigned char DeviceCAN_MCP2510::_get_mode(void)
  {
    unsigned char ret;

    ret = DeviceMCP2510::cmdRead(DeviceMCP2510::CANSTAT);
    return (ret >> 5);
  }

inline unsigned char DeviceCAN_MCP2510::_set_mode(unsigned char mask)
  {
    unsigned char ret;

    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::CANCTRL, 0xE0, mask << 5);
    ret = DeviceMCP2510::cmdRead(DeviceMCP2510::CANSTAT);

    return (ret >> 5);
  }

inline unsigned char DeviceCAN_MCP2510::_read_status(void)
  {
    return DeviceMCP2510::cmdReadStatus();
  }

inline unsigned char DeviceCAN_MCP2510::_get_reg(unsigned char reg)
  {
    return DeviceMCP2510::cmdRead(reg);
  }

inline void DeviceCAN_MCP2510::_put_reg(unsigned char reg, unsigned char data)
  {
    DeviceMCP2510::cmdWrite(reg, data);
  }

inline void DeviceCAN_MCP2510::_rts(unsigned char channel)
  {
    return DeviceMCP2510::cmdRts(channel);
  }

/* ---- Common macros ---------------------------------------- */

inline void DeviceCAN_MCP2510::_transmit(void)
  {
    _tx0_transmit();
  }

inline void DeviceCAN_MCP2510::_tx0_transmit(void)
  {
    _set_CANINTE_bits(DeviceMCP2510::TX0IE);
  }

inline void DeviceCAN_MCP2510::_tx1_transmit(void)
  {
    _set_CANINTE_bits(DeviceMCP2510::TX1IE);
  }

inline void DeviceCAN_MCP2510::_tx2_transmit(void)
  {
    _set_CANINTE_bits(DeviceMCP2510::TX2IE);
  }

#endif /*DEVICECAN_MCP2510_H_*/
