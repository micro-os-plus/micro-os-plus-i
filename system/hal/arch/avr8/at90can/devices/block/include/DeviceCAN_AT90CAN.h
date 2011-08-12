/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef DEVICECAN_AT90CAN_H_
#define DEVICECAN_AT90CAN_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceCAN.h"
#include "portable/devices/misc/include/DeviceCANTransceiver.h"

class DeviceCAN_AT90CAN : public OSDeviceCAN, public DeviceCANTransceiver
  {
public:
  DeviceCAN_AT90CAN(CANPacket *pRxBuf, unsigned short rxBufSize,
      unsigned short rxHWM, unsigned short rxLWM, CANPacket *pTxBuf,
      unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM);

  static void interruptInit(void);
  static void interruptEnable(void);
  static void interruptDisable(void);

  void interruptServiceRoutine(void);

  static DeviceCAN_AT90CAN *pThis;

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

  static void Can_reset(void);
  static void Can_enable(void);
  static void can_clear_all_mob(void);
  static void Can_clear_mob(void);
  static void Can_set_mob(unsigned char mob);
  static void Can_set_std_id(unsigned short identifier);
  static void Can_clear_rtr(void);
  static void Can_set_dlc(unsigned char dlc);
  static void DISABLE_MOB(void);
  static void Can_config_tx(void);
  static void Can_config_rx(void);
  static bool Can_isMobDisabled(void);
  static unsigned short Can_get_std_id(void);
  static unsigned char Can_get_dlc(void);
  static void can_get_data(unsigned char*  p_can_message_data);
  
  const static unsigned char NB_MOB = 15;
  const static unsigned char NB_DATA_MAX = 8;
  const static unsigned char LAST_MOB_NB = (NB_MOB-1);
  const static unsigned char NO_MOB = 0xFF;

  const static unsigned char NB_MOB_TX = 0;
  const static unsigned char NB_MOB_RX = 1;

  const static unsigned char MOB_Tx_ENA = 1;
  const static unsigned char MOB_Rx_ENA = 2;
  const static unsigned char MOB_Rx_BENA = 3;

  };

inline void DeviceCAN_AT90CAN::Can_reset(void)
  {
    CANGCON = _BV(SWRES);
  }

inline void DeviceCAN_AT90CAN::Can_enable(void)
  {
    CANGCON |= _BV(ENASTB);
  }

inline void DeviceCAN_AT90CAN::Can_clear_mob(void)
  {
    CANSTMOB = 0;
    CANCDMOB = 0;
    CANIDT4 = CANIDT3 = CANIDT2 = CANIDT1 = 0;
    CANIDM4 = CANIDM3 = CANIDM2 = CANIDM1 = 0;
  }

inline void DeviceCAN_AT90CAN::can_clear_all_mob(void)
  {
    for (unsigned char mob_number = 0; mob_number < NB_MOB; mob_number++)
      {
        Can_set_mob(mob_number); //! Page index
        Can_clear_mob(); //! All MOb Registers=0
      }
  }

inline void DeviceCAN_AT90CAN::Can_set_mob(unsigned char mob)
  {
    CANPAGE = ((mob) << 4);
  }

inline void DeviceCAN_AT90CAN::Can_set_std_id(unsigned short identifier)
  {
    CANIDT1 = (identifier >> 3); // bits 10:3
    CANIDT2 = (identifier << 5); // bits 2:0 at left
    CANCDMOB &= ~_BV(IDE);
  }

inline unsigned short DeviceCAN_AT90CAN::Can_get_std_id(void)
  {
    unsigned short id;
    id = CANIDT1;
    id <<= 3;
    id |= ((CANIDT2 >> 5) & 0x7);

    return id;
  }

inline void DeviceCAN_AT90CAN::Can_clear_rtr(void)
  {
    CANIDT4 &= ~_BV(RTRTAG);
  }

inline void DeviceCAN_AT90CAN::Can_set_dlc(unsigned char dlc)
  {
    CANCDMOB |= (dlc & 0x0F);
  }

inline unsigned char DeviceCAN_AT90CAN::Can_get_dlc(void)
  {
    return (CANCDMOB & 0xF);
  }

inline void DeviceCAN_AT90CAN::can_get_data(unsigned char*  p_can_message_data)
{
  unsigned char data_index;

    for (data_index = 0; data_index < (Can_get_dlc()); data_index++)
    {
        *(p_can_message_data + data_index) = CANMSG;
    }
}

inline void DeviceCAN_AT90CAN::DISABLE_MOB(void)
  {
    CANCDMOB &= ~(_BV(CONMOB1)|_BV(CONMOB0));
  }

inline bool DeviceCAN_AT90CAN::Can_isMobDisabled(void)
  {
    return (CANCDMOB & (_BV(CONMOB1)|_BV(CONMOB0))) == 0;
  }

inline void DeviceCAN_AT90CAN::Can_config_tx(void)
  {
    //DISABLE_MOB();
    CANCDMOB |= (MOB_Tx_ENA << CONMOB0);
  }

inline void DeviceCAN_AT90CAN::Can_config_rx(void)
  {
    //DISABLE_MOB();
    CANCDMOB |= (MOB_Rx_ENA << CONMOB0);
  }

inline void DeviceCAN_AT90CAN::interruptInit(void)
  {
    CANGIE = 0;
    
    CANGIE |= _BV(ENTX);
    CANGIE |= _BV(ENRX);
    CANGIE |= _BV(ENERR);
    CANGIE |= _BV(ENBX);
    CANGIE |= _BV(ENERG);
    CANGIE |= _BV(ENBOFF);
  }

inline void DeviceCAN_AT90CAN::interruptEnable(void)
  {
    // enable interrupt
    CANGIE |= _BV(ENIT);
  }

inline void DeviceCAN_AT90CAN::interruptDisable(void)
  {
    // disable interrupt
    CANGIE &= ~_BV(ENIT);
  }

#endif /* DEVICECAN_AT90CAN_H_ */
