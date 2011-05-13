/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_DEVICECAN_MCP2510)

#include "hal/arch/avr8/devices/block/include/DeviceCAN_MCP2510.h"

//namespace DeviceCAN_AT90CAN
//{
DeviceCAN_MCP2510 *DeviceCAN_MCP2510::pThis;
//}

DeviceCAN_MCP2510::DeviceCAN_MCP2510(CANPacket *pRxBuf,
    unsigned short rxBufSize, unsigned short rxHWM, unsigned short rxLWM,
    CANPacket *pTxBuf, unsigned short txBufSize, unsigned short txHWM,
    unsigned short txLWM) :
  OSDeviceCAN(pRxBuf, rxBufSize, rxHWM, rxLWM, pTxBuf, txBufSize, txHWM, txLWM)
  {
    OSDeviceDebug::putConstructor_P(PSTR("DeviceCAN_MCP2510"), this);

    pThis = this;
  }

void DeviceCAN_MCP2510::implInit(unsigned char mode)
  {
    OSDeviceDebug::putString("DeviceCAN_MCP2510::implInit()");
    OSDeviceDebug::putNewLine();

    transceiverInit();
    busLockInit();
    interruptInit();

    DeviceMCP2510::init();

    // ----

    DeviceMCP2510::cmdReset();

    // After RESET we are in configuration mode

#if defined(CAN_125K)
    put_reg(DeviceMCP2510::CNF1, 0x03); /* SJW=1*Tq, 125K */
    put_reg(DeviceMCP2510::CNF2, 0xBC); /* PHSEG=8*Tq, PRSEG=5*Tq */
    put_reg(DeviceMCP2510::CNF3, 0x01); /* PHSEG2=2*Tq */
#elif defined(CAN_1M)
    put_reg(DeviceMCP2510::CNF1, 0x00); /* SJW=1*Tq, 1M */
    put_reg(DeviceMCP2510::CNF2, 0x8A); /* PHSEG=2*Tq, PRSEG=3*Tq */
    put_reg(DeviceMCP2510::CNF3, 0x01); /* PHSEG2=2*Tq */
#else /* defined(CAN_125K_CK) CAN_KING setting */
    put_reg(DeviceMCP2510::CNF1, 0x03); /* SJW=1*Tq, 125K */
    put_reg(DeviceMCP2510::CNF2, 0x9E); /* PHSEG=4*Tq, PRSEG=7*Tq */
    put_reg(DeviceMCP2510::CNF3, 0x03); /* PHSEG2=4*Tq */
#endif

    put_reg(DeviceMCP2510::RXB0CTRL, 0x64); /* receive any message, rollover */
    put_reg(DeviceMCP2510::RXB1CTRL, 0x60); /* receive any message */
#if 0
    put_reg(DeviceMCP2510::RXM0SIDH, 0x00); /* null mask, accept all */
    put_reg(DeviceMCP2510::RXM0SIDL, 0x00);
#endif

    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::TXB0CTRL, 0x03, 0x03); /* Highest priority */
    DeviceMCP2510::cmdBitsModify(DeviceMCP2510::TXB1CTRL, 0x03, 0x03); /* Highest priority */

#ifdef CAN_ENABLE_INTERRUPTS
    _set_CANINTE_bits(DeviceMCP2510::ERRIE|DeviceMCP2510::RX1IE
        |DeviceMCP2510::RX0IE);

    // MERRE usually used for autobaud
    //_set_CANINTE_bits(MCP2510::MERRE);

#ifdef CAN_TX0_ISR_INCLUDED
    _set_CANINTF_bits(DeviceMCP2510::TX0IF);
#endif /* CAN_TX0_ISR_INCLUDED */
#ifdef CAN_TX1_ISR_INCLUDED
    _set_CANINTF_bits(DeviceMCP2510::TX1IF);
#endif /* CAN_TX1_ISR_INCLUDED */
#endif

    // Switch to normal operation mode (no need to loop)
      {
        unsigned char c;

        if (mode == OSDeviceCAN::MODE_ENABLED)
          {
            mode = DeviceMCP2510::CAN_RUN;
          }
        else if (mode == OSDeviceCAN::MODE_LISTEN)
          {
            //OSDeviceDebug::putString("LISTEN mode faulty ");
            // workaround to bug 2, avoid entering sleep
            _set_mode(DeviceMCP2510::CAN_LOOPBACK);
            mode = DeviceMCP2510::CAN_LISTEN;
          }

        c = _set_mode(mode);
#ifdef DEBUG
        if (c != mode)
          {
            OSDeviceDebug::putString("mode ");
            OSDeviceDebug::putHex(c);
            OSDeviceDebug::putString("<>");
            OSDeviceDebug::putHex(mode);
            OSDeviceDebug::putNewLine();
          }
#endif
      }
    transceiverHighSpeed();

    interruptEnable();

    m_isConnected = true;
  }

void DeviceCAN_MCP2510::interruptServiceRoutine(void)
  {
    //OSDeviceDebug::putString(" IRQ ");

    //OSDeviceDebug::putChar('!');
    if (false)
      {
        OSDeviceDebug::putString(" A ");
        OSDeviceDebug::putHex((unsigned char)SPSR);
        OSDeviceDebug::putChar(' ');
        OSDeviceDebug::putHex((unsigned char)SPCR);
        OSDeviceDebug::putChar(' ');
      }
    //can_spi_init();
    for (;;)
      { // loop until no more irq sources
        unsigned char e;
        unsigned char f;
        unsigned char c;
        CANPacket can_packet;

        e = get_reg(DeviceMCP2510::CANINTE);
        f = get_reg(DeviceMCP2510::CANINTF);

        c = e & f;
        if (c == 0)
          break; // no more irqs

        if (c & DeviceMCP2510::MERRF)
          {
            // Message Error
#ifdef DEBUG_CAN_MERR
            OSDeviceDebug::putString(" MERRF ");
            //OSDeviceDebug::putNewLine();
#endif
            clear_intf_bits(DeviceMCP2510::MERRF);
          }

        if (c & DeviceMCP2510::ERRIF)
          {
            // Error (multiple sources as shown in EFLG)
            e = get_reg(DeviceMCP2510::EFLG);
            if (((get_reg(DeviceMCP2510::CANSTAT) >> 5)
                == DeviceMCP2510::CAN_LISTEN) && ((e & DeviceMCP2510::RX0OVR)
                != 0))
              {
                ;
              }
            else
              {
#ifdef DEBUG_CAN_IERR
                OSDeviceDebug::putString(" ERRIF=");
                OSDeviceDebug::putHex(e);
                //OSDeviceDebug::putChar(' ');
                OSDeviceDebug::putNewLine();
#endif
              }
            clear_intf_bits(DeviceMCP2510::ERRIF);
          }

        if (c & DeviceMCP2510::WAKIF)
          {
            // Wake Up
            OSDeviceDebug::putString(" WAKIF ");
            //OSDeviceDebug::putNewLine();

            clear_intf_bits(DeviceMCP2510::WAKIF);
          }

        if (c & DeviceMCP2510::TX0IF)
          {
            // Transmit Buffer 0
#ifdef CAN_PUT_TX0_INCLUDED
            if (tx0_isr(&can_packet))
              {
                put_TX0(&can_packet);
#ifdef DEBUG_CAN_TX0
                OSDeviceDebug::putChar('!');
#endif
              }
            else
              {
                // nothing to send, disable transmitter
                // leave INTF enabled
                clear_inte_bits(DeviceMCP2510::TX0IE);
#ifdef DEBUG_CAN_TX0
                OSDeviceDebug::putChar('#');
#endif
              }
#else
            // disable transmitter
            clear_inte_bits(DeviceMCP2510::TX0IE);
#endif
          }

        if (c & DeviceMCP2510::TX1IF)
          {
            // Transmit Buffer 1
#ifdef CAN_TX1_ISR_INCLUDED
            if ( tx1_isr(&can_packet) )
              {
                put_TX1(&can_packet);
#ifdef _DEBUG
                OSDeviceDebug::putChar('!');
#endif
              }
            else
              {
                // nothing to send, disable transmitter
                // leave INTF enabled
                clear_inte_bits(DeviceMCP2510::TX1IE);
#ifdef _DEBUG
                OSDeviceDebug::putChar('#');
#endif
              }
#else
            // disable transmitter
            clear_inte_bits(DeviceMCP2510::TX1IE);
#endif /* CAN_TX1_ISR_INCLUDED */
          }

        if (c & DeviceMCP2510::TX2IF)
          {
            // Transmit Buffer 2
#ifdef CAN_TX2_ISR_INCLUDED
            if ( tx2_isr(&can_packet) )
              {
                put_TX2(&can_packet);
                // clear INTF but leave INTE enabled
                clear_intf_bits(DeviceMCP2510::TX2IF);
#ifdef _DEBUG
                OSDeviceDebug::putChar('!');
#endif
              }
            else
              {
                // nothing to send, disable transmitter
                // leave INTF enabled
                clear_inte_bits(DeviceMCP2510::TX2IE);
#ifdef _DEBUG
                OSDeviceDebug::putChar('#');
#endif
              }
#else
            // disable transmitter
            clear_inte_bits(DeviceMCP2510::TX2IE);
#endif /* CAN_TX2_ISR_INCLUDED */
          }

        if (c & DeviceMCP2510::RX0IF)
          {
            // Receive Buffer 0
#ifdef _DEBUG
            OSDeviceDebug::putChar('R');
            OSDeviceDebug::putChar('0');
#endif
            get_RX0(&can_packet);
            //can_clear_intf_bits(RX0IF); //included 

#if 0
            if (can_packet.id != 0)
            dump(&can_packet);
#endif

            if ( !m_rxBuf.isFull() )
              {
                m_rxBuf.put((unsigned char*)&can_packet);
              }

            OSScheduler::eventNotify(pThis->getReadEvent());
          }

        if (c & DeviceMCP2510::RX1IF)
          {
            // Receive Buffer 1
#ifdef _DEBUG
            OSDeviceDebug::putChar('R');
            OSDeviceDebug::putChar('1');
#endif
            get_RX1(&can_packet);
            //can_clear_intf_bits(RX1IF); // included

#if 0
            if (can_packet.id != 0)
            dump(&can_packet);
#endif

            if ( !m_rxBuf.isFull() )
              {
                m_rxBuf.put((unsigned char*)&can_packet);
              }

            OSScheduler::eventNotify(pThis->getReadEvent());
          }
      } /* end for */

    if (false)
      {
        OSDeviceDebug::putString(" Z ");
        OSDeviceDebug::putHex((unsigned char)SPSR);
        OSDeviceDebug::putChar(' ');
        OSDeviceDebug::putHex((unsigned char)SPCR);
        OSDeviceDebug::putChar(' ');
      }
  }

bool DeviceCAN_MCP2510::implCanRead(void)
  {
    return !m_rxBuf.isEmpty();
  }

int DeviceCAN_MCP2510::implAvailableRead(void)
  {
    return 0; // TODO:
  }

bool DeviceCAN_MCP2510::implCanWrite(void)
  {
    return true; // TODO:
  }

int DeviceCAN_MCP2510::implReadPacket(CANPacket *p)
  {
    m_rxBuf.get((unsigned char *)p);
    return 0;
  }

int DeviceCAN_MCP2510::implWritePacket(CANPacket *p)
  {
    if (!m_txBuf.isFull())
      {
        m_txBuf.put((unsigned char*)p);
        implTransmit();
        
        return OSReturn::OS_OK;
      }
    else
      {
        OSDeviceDebug::putString(" tx full ");
        
        return OSReturn::OS_BUFFER_FULL;
      }
  }

// it runs with interrupts disabled, it is safe to set and use MOB
void DeviceCAN_MCP2510::implTransmit(void)
  {
    tx0_transmit();
  }

extern "C"
void  INT5_vect(void) __attribute__((signal, naked));

void INT5_vect(void)
  {
    // interrupts disabled in here
    OSScheduler::interruptEnter();
      {
        DeviceCAN_MCP2510::interruptServiceRoutine();
      }
    OSScheduler::interruptExit();
    // interrupts enabled after this point
  }

// ----------------------------------------------------------------------------

void DeviceCAN_MCP2510::put_reg(unsigned char r, unsigned char v)
  {
    _put_reg(r, v);
  }

unsigned char DeviceCAN_MCP2510::get_reg(unsigned char r)
  {
    unsigned char i;

    i = _get_reg(r);
    return i;
  }

void DeviceCAN_MCP2510::clear_inte_bits(unsigned char b)
  {
    _clear_CANINTE_bits(b);
  }

void DeviceCAN_MCP2510::clear_intf_bits(unsigned char b)
  {
    _clear_CANINTF_bits(b);
  }

unsigned char DeviceCAN_MCP2510::get_ICOD(void)
  {
    unsigned char i;

    i = _get_reg(DeviceMCP2510::CANSTAT);
    return ((i>>1)&0x07);
  }

#ifdef CAN_GET_RX0_INCLUDED

void DeviceCAN_MCP2510::get_RX0(CANPacket *pc)
  {
    unsigned char i, j, len;
    unsigned short id;
    unsigned char *p;

    i = _get_reg(DeviceMCP2510::RXB0SIDH);
    id = i << 8;
    i = _get_reg(DeviceMCP2510::RXB0SIDL);
    id |= i;
    pc->id = id >> 5;

    i = _get_reg(DeviceMCP2510::RXB0DLC);
    len = i & 0xF;
    if (len> 8)
      len = 8;

    pc->len = len;

    for (p = pc->data, j = 0; len != 0; ++j, ++p, --len)
      {
        *p = _get_reg(DeviceMCP2510::RXB0D0+j);
      }

    // Clear RX0IF to free buffer
    _clear_CANINTF_bits(DeviceMCP2510::RX0IF);
  }

#endif /* CAN_GET_RX0_INCLUDED */

#ifdef CAN_GET_RX1_INCLUDED

void DeviceCAN_MCP2510::get_RX1(CANPacket *pc)
  {
    unsigned char i, j, len;
    unsigned short id;
    unsigned char *p;

    i = _get_reg(DeviceMCP2510::RXB1SIDH);
    id = i << 8;
    i = _get_reg(DeviceMCP2510::RXB1SIDL);
    id |= i;
    pc->id = id >> 5;

    i = _get_reg(DeviceMCP2510::RXB1DLC);
    len = i & 0xF;
    if (len> 8)
      len = 8;

    pc->len = len;

    for (p = pc->data, j = 0; len != 0; ++j, ++p, --len)
      {
        *p = _get_reg(DeviceMCP2510::RXB1D0+j);
      }

    // Clear RX1IF to free buffer
    _clear_CANINTF_bits(DeviceMCP2510::RX1IF);
  }

#endif /* CAN_GET_RX1_INCLUDED */

#ifdef CAN_PUT_TX0_INCLUDED

void DeviceCAN_MCP2510::tx0_transmit(void)
  {
    _tx0_transmit();
  }

bool DeviceCAN_MCP2510::tx0_isr(CANPacket *pc)
  {
    if (m_txBuf.isEmpty())
      return false;

    m_txBuf.get((unsigned char*)pc);
    return true;
  }

// TXB0 transmit, to be used on interrupts
void DeviceCAN_MCP2510::put_TX0(CANPacket *pc)
  {
    unsigned char i, j;
    unsigned short id;
    unsigned char *p;

    id = pc->id << 5;
    _put_reg(DeviceMCP2510::TXB0SIDH, id >> 8);
    _put_reg(DeviceMCP2510::TXB0SIDL, id);

    i = pc->len & 0x0F;
    if (i> 8)
      i = 8;
    _put_reg(DeviceMCP2510::TXB0DLC, i);

    for (j = 0, p = pc->data; i != 0; --i, ++j, ++p)
      {
        _put_reg(DeviceMCP2510::TXB0D0+j, *p);
      }

    _rts(1);
  }

#endif /* CAN_PUT_TX0_INCLUDED */

#endif /*OS_INCLUDE_DEVICECAN_MCP2510*/
