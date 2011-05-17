/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECAN_AT90CAN)

#include "hal/arch/avr8/at90can/devices/block/include/DeviceCAN_AT90CAN.h"

//#include "portable/devices/debug/include/OSDebugLed2.h"

//namespace DeviceCAN_AT90CAN
//{
DeviceCAN_AT90CAN *DeviceCAN_AT90CAN::pThis;
//}

DeviceCAN_AT90CAN::DeviceCAN_AT90CAN(CANPacket *pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    CANPacket *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
  OSDeviceCAN(pRxBuf, rxBufSize, rxHWM, rxLWM, pTxBuf, txBufSize, txHWM, txLWM)
  {
    OSDeviceDebug::putConstructor_P(PSTR("DeviceCAN_AT90CAN"), this);

    pThis = this;
  }

void DeviceCAN_AT90CAN::implInit(unsigned char mode)
  {
    OSDeviceDebug::putString_P(PSTR("DeviceCAN_AT90CAN::implInit()"));
    OSDeviceDebug::putNewLine();

    transceiverInit();

    Can_reset();

    interruptInit(); // must be after reset()

    /* CAN_KING setting */
    /* 125K, 75%, Tq=0.5uS, Tbit=16*Tq=8uS */
    /* Tprs=7*Tq, Tph1=4*Tq, Tph2=4*Tq, Tsjw=1*Tq */
    CANBT1 = 0x0E; /* 125K@16MHz */
    CANBT2 = 0x0C;
    CANBT3 = 0x37;

    can_clear_all_mob();

    // enable reception on the reception mob
    Can_set_mob(NB_MOB_RX);
    Can_config_rx();

    // enable both RX and TX mobs
    CANEN2 = _BV(NB_MOB_RX) | _BV(NB_MOB_TX);

    // enable interrupts for both RX and TX mobs
    CANIE2 = _BV(NB_MOB_RX) | _BV(NB_MOB_TX);

    mode = mode; // set to given mode
    Can_enable();

    transceiverHighSpeed();

    //OSDebugLed2::init();

    interruptEnable();

    m_isConnected = true;
  }

void DeviceCAN_AT90CAN::interruptServiceRoutine(void)
  {
    //OSDeviceDebug::putString(" IRQ ");

    unsigned char c;
    if ((CANGIT & _BV(CANIT)) != 0)
      {
        unsigned char c;
        c = CANSIT2 & CANIE2;

        // we currently use only two MOBs, one for TX and 
        // one for RX, no need to iterrate all
        if ((c & _BV(NB_MOB_TX)) != 0)
          {
            // MOB0, transmit
            Can_set_mob(NB_MOB_TX);

            c = CANSTMOB;
            if ((c & _BV(TXOK)) != 0)
              {
                // read-modify-write
                CANSTMOB &= ~_BV(TXOK);

                //OSDebugLed2::on();

                if (!m_txBuf.isEmpty())
                  {
                    CANPacket packet;
                    m_txBuf.get((unsigned char*)&packet);

                    Can_clear_mob(); // previous status saved in c

                      {
                        Can_set_std_id(packet.id);
                        for (unsigned char cpt = 0; cpt < packet.len; cpt++)
                          CANMSG = packet.data[cpt];
                        Can_clear_rtr();
                        Can_set_dlc(packet.len);
                      }
                    Can_config_tx(); // already cleared
                    //OSDeviceDebug::putString(" tx ");
                  }

                else
                  {
                    // nothing more to transmit
                    DISABLE_MOB();

                    // OSDeviceDebug::putString(" empty ");
                  }

                if (m_txBuf.isBelowLowWM())
                  OSScheduler::eventNotify(getWriteEvent());

                //OSDebugLed2::off();
              }
            else
              {
                // other interrupts...
                OSDeviceDebug::putString_P(PSTR(" CANSTMOB0="));
                OSDeviceDebug::putHex(c);
                OSDeviceDebug::putChar(' ');

                CANSTMOB = 0;
              }
          }

        if ((c & _BV(NB_MOB_RX)) != 0)
          {
            // MOB1, receive
            Can_set_mob(NB_MOB_RX);

            c = CANSTMOB;
            if ((c & _BV(RXOK)) != 0)
              {
                //OSDeviceDebug::putString(" rx ");
                if ((c & _BV(IDE)) == 0)
                  {
                    // standard 11 bits ID; reply bit ignored 
                    CANPacket can_packet;

                    can_packet.id = Can_get_std_id();
                    can_packet.len = Can_get_dlc();
                    can_get_data(can_packet.data);

#if 0
                    if (can_packet.id != 0)
                      {
                        can_packet.dump();
                      }
#endif

                    if (!m_rxBuf.isFull())
                      {
                        m_rxBuf.put((unsigned char*)&can_packet);
                      }

                    OSScheduler::eventNotify(getReadEvent());
                  }
                // re-enable reception on this mob
                Can_clear_mob();
                // DISABLE_MOB();
                Can_config_rx();

                //CANSTMOB = c & ~_BV(RXOK);
              }
            else
              {
                // other interrupts...
                OSDeviceDebug::putString_P(PSTR(" CANSTMOB1="));
                OSDeviceDebug::putHex(c);
                OSDeviceDebug::putChar(' ');

                CANSTMOB = 0;
              }
          }
      }

    c = CANGIT & ~(_BV(CANIT) | _BV(OVRTIM));
    if (c != 0)
      {
        // other interrupts...
        OSDeviceDebug::putString_P(PSTR(" CANGIT="));
        OSDeviceDebug::putHex(c);
        OSDeviceDebug::putChar(' ');

        // reset interrupt flag 
        CANGIT |= c;
      }
  }

bool DeviceCAN_AT90CAN::implCanRead(void)
  {
    return !m_rxBuf.isEmpty();
  }

int DeviceCAN_AT90CAN::implAvailableRead(void)
  {
    return 0; // TODO:
  }

bool DeviceCAN_AT90CAN::implCanWrite(void)
  {
    return true; // TODO:
  }

int DeviceCAN_AT90CAN::implReadPacket(CANPacket *p)
  {
    m_rxBuf.get((unsigned char*)p);
    return 0;
  }

int DeviceCAN_AT90CAN::implWritePacket(CANPacket *p)
  {
    if (!m_txBuf.isFull())
      {
        m_txBuf.put((unsigned char*)p);
        implTransmit();
        
        return OSReturn::OS_OK;
      }
    else
      {
        OSDeviceDebug::putString("ctx full ");
        
        return OSReturn::OS_BUFFER_FULL;
      }
  }

// it runs with interrupts disabled, it is safe to set and use MOB
void DeviceCAN_AT90CAN::implTransmit(void)
  {
    if ((CANGSTA & _BV(TXBSY)) == 0)
      {
        Can_set_mob(NB_MOB_TX);
        CANSTMOB |= _BV(TXOK); // trigger transmit interrupt
      }
  }

extern "C"
void  CANIT_vect(void) __attribute__((signal, naked));

void CANIT_vect(void)
  {
    OSScheduler::interruptEnter(); // interrupts disabled in here
      {
        DeviceCAN_AT90CAN::pThis->interruptServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

#endif /* OS_INCLUDE_DEVICECAN_AT90CAN */
