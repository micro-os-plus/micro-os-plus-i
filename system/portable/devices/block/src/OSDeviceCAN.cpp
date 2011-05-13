/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_OSDEVICECAN)

#include "portable/kernel/include/OS.h"

#include "portable/devices/block/include/OSDeviceCAN.h"

namespace OSDeviceCAN
  {
    bool m_isConnected;
    bool m_isPowered;
    CircularBlockBuffer m_rxBuf;
    CircularBlockBuffer m_txBuf;
  }

OSDeviceCAN::OSDeviceCAN(CANPacket *pRxBuf, unsigned short rxBufSize,
    unsigned short rxHWM, unsigned short rxLWM, CANPacket *pTxBuf,
    unsigned short txBufSize, unsigned short txHWM, unsigned short txLWM)
  {
    OSDeviceDebug::putConstructor_P(PSTR("OSDeviceCAN"), this);

    m_rxBuf.init((unsigned char*)pRxBuf, sizeof(CANPacket), rxBufSize, rxHWM,
        rxLWM);

    m_txBuf.init((unsigned char*)pTxBuf, sizeof(CANPacket), txBufSize, txHWM,
        txLWM);

    m_isConnected = false;
    m_isPowered = true;
  }

void OSDeviceCAN::init(unsigned char mode)
  {
    OSDeviceDebug::putString_P(PSTR("OSDeviceCAN::init()"));
    OSDeviceDebug::putNewLine();

    implInit(mode);

    m_mode = mode;
    m_isConnected = true;
  }

int OSDeviceCAN::readPacket(CANPacket *p)
  {
    bool flag;
    for (;;)
      {
        OSCriticalSection::enter();
          {
            flag = implCanRead() || !isConnected();
          }
        OSCriticalSection::exit();
        if (flag)
        break;

#if (false)
        if (getReadTimeout() == OS_TIMEOUT_IMMEDIATELY )
        return OS_ERROR_WOULD_BLOCK;
#endif

        //OSDeviceDebug::putChar('<');
#if 0
        setReadEvent(implGetReadEvent());
#endif

        OSEventWaitReturn_t ret;
        // TODO: timeout getReadTimeout() 
        ret = OSScheduler::eventWait(getReadEvent());
        //OSDeviceDebug::putHex((unsigned short)event);
        //OSDeviceDebug::putNewLine();

        if (ret == OSEventWaitReturn::OS_TIMEOUT)
          {
#if defined(DEBUG) && defined(OS_DEBUG_OSDEVICECHARACTER_READBYTE)
            OSDeviceDebug::putString( "OSDeviceCAN::readPacket() return timeout" );
            OSDeviceDebug::putNewLine();
#endif
            return OSReturn::OS_TIMEOUT;
          }
      }

    if ( !isConnected() )
    return OSReturn::OS_DISCONNECTED;

    int c;
    OSCriticalSection::enter();
      {
        c = implReadPacket(p);
      }
    OSCriticalSection::exit();
    return c;
  }

int OSDeviceCAN::availableRead(void)
  {
    return implAvailableRead();
  }

// return 
// OK if packet queued for sending
// BUFFER_FULL if ignored

int OSDeviceCAN::writePacket(CANPacket *p)
  {
    int c;
#if defined(OS_INCLUDE_OSDEVICECAN_WRITEPACKET_SLEEP)
    OSScheduler::timerTicks.sleep(1);
#endif

    OSCriticalSection::enter();
      {
        // TODO: if needed, make it block

        // adjust illegal packet length
        if (p->len > 8)
        p->len = 8;

        c = implWritePacket(p);
      }
    OSCriticalSection::exit();

    return c;
  }

// ---


#endif /*OS_INCLUDE_OSDEVICECAN*/
