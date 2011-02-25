/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICEMCP2510_H_
#define DEVICEMCP2510_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/misc/include/DeviceMCP2510Select.h"
#include "portable/devices/misc/include/DeviceMCP2510SPI.h"

class DeviceMCP2510 : public DeviceMCP2510Select, public DeviceMCP2510SPI
{
public:
  static void
  init();

  static void
  cmdReset(void);
  static void
  cmdWrite(unsigned char addr, unsigned char data);
  static unsigned char
  cmdRead(unsigned char addr);
  static void
  cmdBitsModify(unsigned char addr, unsigned char mask, unsigned char data);
  static unsigned char
  cmdReadStatus(void);
  static void
  cmdRts(unsigned char channel);

  // Register definitions
  const static unsigned char RXF0SIDH = 0x00;
  const static unsigned char RXF0SIDL = 0x01;
  const static unsigned char RXF0EID8 = 0x02;
  const static unsigned char RXF0EID0 = 0x03;
  const static unsigned char RXF1SIDH = 0x04;
  const static unsigned char RXF1SIDL = 0x05;
  const static unsigned char RXF1EID8 = 0x06;
  const static unsigned char RXF1EID0 = 0x07;
  const static unsigned char RXF2SIDH = 0x08;
  const static unsigned char RXF2SIDL = 0x09;
  const static unsigned char RXF2EID8 = 0x0A;
  const static unsigned char RXF2EID0 = 0x0B;
  const static unsigned char BFPCTRL = 0x0C;
  const static unsigned char TXRTSCTRL = 0x0D;
  const static unsigned char CANSTAT = 0x0E;
  const static unsigned char CANCTRL = 0x0F;

  const static unsigned char RXF3SIDH = 0x10;
  const static unsigned char RXF3SIDL = 0x11;
  const static unsigned char RXF3EID8 = 0x12;
  const static unsigned char RXF3EID0 = 0x13;
  const static unsigned char RXF4SIDH = 0x14;
  const static unsigned char RXF4SIDL = 0x15;
  const static unsigned char RXF4EID8 = 0x16;
  const static unsigned char RXF4EID0 = 0x17;
  const static unsigned char RXF5SIDH = 0x18;
  const static unsigned char RXF5SIDL = 0x19;
  const static unsigned char RXF5EID8 = 0x1A;
  const static unsigned char RXF5EID0 = 0x1B;
  const static unsigned char TEC = 0x1C;
  const static unsigned char REC = 0x1D;
  const static unsigned char CANSTAT1 = 0x1E;
  const static unsigned char CANCTRL1 = 0x1F;

  const static unsigned char RXM0SIDH = 0x20;
  const static unsigned char RXM0SIDL = 0x21;
  const static unsigned char RXM0EID8 = 0x22;
  const static unsigned char RXM0EID0 = 0x23;
  const static unsigned char RXM1SIDH = 0x24;
  const static unsigned char RXM1SIDL = 0x25;
  const static unsigned char RXM1EID8 = 0x26;
  const static unsigned char RXM1EID0 = 0x27;
  const static unsigned char CNF3 = 0x28;
  const static unsigned char CNF2 = 0x29;
  const static unsigned char CNF1 = 0x2A;
  const static unsigned char CANINTE = 0x2B;
  const static unsigned char CANINTF = 0x2C;
  const static unsigned char EFLG = 0x2D;
  const static unsigned char CANSTAT2 = 0x2E;
  const static unsigned char CANCTRL2 = 0x2F;

  const static unsigned char TXB0CTRL = 0x30;
  const static unsigned char TXB0SIDH = 0x31;
  const static unsigned char TXB0SIDL = 0x32;
  const static unsigned char TXB0EID8 = 0x33;
  const static unsigned char TXB0EID0 = 0x34;
  const static unsigned char TXB0DLC = 0x35;
  const static unsigned char TXB0D0 = 0x36;
  const static unsigned char TXB0D1 = 0x37;
  const static unsigned char TXB0D2 = 0x38;
  const static unsigned char TXB0D3 = 0x39;
  const static unsigned char TXB0D4 = 0x3A;
  const static unsigned char TXB0D5 = 0x3B;
  const static unsigned char TXB0D6 = 0x3C;
  const static unsigned char TXB0D7 = 0x3D;
  const static unsigned char CANSTAT3 = 0x3E;
  const static unsigned char CANCTRL3 = 0x3F;

  const static unsigned char TXB1CTRL = 0x40;
  const static unsigned char TXB1SIDH = 0x41;
  const static unsigned char TXB1SIDL = 0x42;
  const static unsigned char TXB1EID8 = 0x43;
  const static unsigned char TXB1EID0 = 0x44;
  const static unsigned char TXB1DLC = 0x45;
  const static unsigned char TXB1D0 = 0x46;
  const static unsigned char TXB1D1 = 0x47;
  const static unsigned char TXB1D2 = 0x48;
  const static unsigned char TXB1D3 = 0x49;
  const static unsigned char TXB1D4 = 0x4A;
  const static unsigned char TXB1D5 = 0x4B;
  const static unsigned char TXB1D6 = 0x4C;
  const static unsigned char TXB1D7 = 0x4D;
  const static unsigned char CANSTAT4 = 0x4E;
  const static unsigned char CANCTRL4 = 0x4F;

  const static unsigned char TXB2CTRL = 0x50;
  const static unsigned char TXB2SIDH = 0x51;
  const static unsigned char TXB2SIDL = 0x52;
  const static unsigned char TXB2EID8 = 0x53;
  const static unsigned char TXB2EID0 = 0x54;
  const static unsigned char TXB2DLC = 0x55;
  const static unsigned char TXB2D0 = 0x56;
  const static unsigned char TXB2D1 = 0x57;
  const static unsigned char TXB2D2 = 0x58;
  const static unsigned char TXB2D3 = 0x59;
  const static unsigned char TXB2D4 = 0x5A;
  const static unsigned char TXB2D5 = 0x5B;
  const static unsigned char TXB2D6 = 0x5C;
  const static unsigned char TXB2D7 = 0x5D;
  const static unsigned char CANSTAT5 = 0x5E;
  const static unsigned char CANCTRL5 = 0x5F;

  const static unsigned char RXB0CTRL = 0x60;
  const static unsigned char RXB0SIDH = 0x61;
  const static unsigned char RXB0SIDL = 0x62;
  const static unsigned char RXB0EID8 = 0x63;
  const static unsigned char RXB0EID0 = 0x64;
  const static unsigned char RXB0DLC = 0x65;
  const static unsigned char RXB0D0 = 0x66;
  const static unsigned char RXB0D1 = 0x67;
  const static unsigned char RXB0D2 = 0x68;
  const static unsigned char RXB0D3 = 0x69;
  const static unsigned char RXB0D4 = 0x6A;
  const static unsigned char RXB0D5 = 0x6B;
  const static unsigned char RXB0D6 = 0x6C;
  const static unsigned char RXB0D7 = 0x6D;
  const static unsigned char CANSTAT6 = 0x6E;
  const static unsigned char CANCTRL6 = 0x6F;

  const static unsigned char RXB1CTRL = 0x70;
  const static unsigned char RXB1SIDH = 0x71;
  const static unsigned char RXB1SIDL = 0x72;
  const static unsigned char RXB1EID8 = 0x73;
  const static unsigned char RXB1EID0 = 0x74;
  const static unsigned char RXB1DLC = 0x75;
  const static unsigned char RXB1D0 = 0x76;
  const static unsigned char RXB1D1 = 0x77;
  const static unsigned char RXB1D2 = 0x78;
  const static unsigned char RXB1D3 = 0x79;
  const static unsigned char RXB1D4 = 0x7A;
  const static unsigned char RXB1D5 = 0x7B;
  const static unsigned char RXB1D6 = 0x7C;
  const static unsigned char RXB1D7 = 0x7D;
  const static unsigned char CANSTAT7 = 0x7E;
  const static unsigned char CANCTRL7 = 0x7F;

  /* EFLG bits */
  const static unsigned char RX1OVR = 0x80;
  const static unsigned char RX0OVR = 0x40;
  const static unsigned char TXBO = 0x20;
  const static unsigned char TXEP = 0x10;
  const static unsigned char RXEP = 0x08;
  const static unsigned char TXWAR = 0x04;
  const static unsigned char RXWAR = 0x02;
  const static unsigned char EWARN = 0x01;

  /* CANINTF bits */
  const static unsigned char MERRF = 0x80;
  const static unsigned char WAKIF = 0x40;
  const static unsigned char ERRIF = 0x20;
  const static unsigned char TX2IF = 0x10;
  const static unsigned char TX1IF = 0x08;
  const static unsigned char TX0IF = 0x04;
  const static unsigned char RX1IF = 0x02;
  const static unsigned char RX0IF = 0x01;

  /* CANINTE bits */
  const static unsigned char MERRE = 0x80;
  const static unsigned char WAKIE = 0x40;
  const static unsigned char ERRIE = 0x20;
  const static unsigned char TX2IE = 0x10;
  const static unsigned char TX1IE = 0x08;
  const static unsigned char TX0IE = 0x04;
  const static unsigned char RX1IE = 0x02;
  const static unsigned char RX0IE = 0x01;

  /* Operation Modes */
  const static unsigned char CAN_RUN = 0x00;
  const static unsigned char CAN_SLEEP = 0x01;
  const static unsigned char CAN_LOOPBACK = 0x02;
  const static unsigned char CAN_LISTEN = 0x03;
  const static unsigned char CAN_CONFIG = 0x04;

private:

  // SPI Commands
  const static unsigned char RESET_CMD = 0xC0;
  const static unsigned char READ_CMD = 0x03;
  const static unsigned char WRITE_CMD = 0x02;
  const static unsigned char RTS_CMD = 0x80;
  const static unsigned char READ_STATUS_CMD = 0xA0;
  const static unsigned char BIT_MODIFY_CMD = 0x05;

};

#endif /*DEVICEMCP2510_H_*/
