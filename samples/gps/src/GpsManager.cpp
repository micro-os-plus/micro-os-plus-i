/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "GpsManager.h"
#include "Application.h"

//#include "portable/kernel/include/ostream_OSThread.h"

GpsManager::GpsManager(const char *pName, OSDeviceCharacter& dev) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_dev(dev)
{
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
  debug.putString("GpsManager()=");
  debug.putPtr(this);
  debug.putNewLine();
#endif
}

void
GpsManager::taskMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          clog << "GpsManager::taskMain()" << endl;
        }
      os.sched.lock.exit();
    }

  //OSDeviceCharacter& dev = m_dev;

  // task endless loop
  for (;;)
    {
      m_eventFlags.clear(OS_ALL_FLAGS);
      m_eventFlags.wait(OS_ALL_FLAGS);

      if ((m_eventFlags.get() & OS_REQUEST_GPS_POSITION) != 0)
        {
          getPosition(&m_gpsPosition);
          app.com.sendGpsPosition(&m_gpsPosition);
        }
    }
}

OSReturn_t
GpsManager::requestGpsPosition(void)
{
  m_eventFlags.notify(OS_REQUEST_GPS_POSITION);

  return OSReturn::OS_OK;
}

OSReturn_t
GpsManager::getPosition(gpsPosition_t* p)
{
  gpsCoordinate_t* latitude; // N/S
  gpsCoordinate_t* longitude; // E/W

  latitude = p->getLatitude();
  longitude = p->getLongitude();

#if defined(APP_INCLUDE_GPSMANAGER_GETPOSITION_SIMULATED)

  latitude->setHemisphere((unsigned char)'N');
  latitude->setDegrees(45);
  latitude->setMinutesInteger(35);
  latitude->setMinutesDecimals(1234);

  longitude->setHemisphere((unsigned char)'E');
  longitude->setDegrees(26);
  longitude->setMinutesInteger(25);
  longitude->setMinutesDecimals(4321);

  p->setAltitude(-1);

#else

  p = p;

#endif

  return OSReturn::OS_OK;
}

