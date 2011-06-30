/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "GpsManager.h"
#include "Application.h"

//#include "portable/kernel/include/ostream_OSThread.h"

const unsigned char gpsCmdSetOutput[] =
    "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n";//51
const unsigned char gpsReplySetSetOutput[] = "PMTK001,314,3";
const unsigned char gpsCmdSetBaud[] = "$PMTK251,19200*22\r\n";//19
const unsigned char gpsFixReq[] = "$PMTK448,4*22\r\n";
const unsigned char gpsCmdReqGGA[] = "$PMTK448,4*22\r\n";

GpsManager::GpsManager(const char *pName, OSDeviceCharacter& dev) :
  OSThread(pName, m_stack, sizeof(m_stack)), m_dev(dev)
{
  debug.putConstructor("GpsManager", this);

  m_isOn = false;
}

void
GpsManager::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          clog << "GpsManager::threadMain()" << std::endl;
        }
      os.sched.lock.exit();
    }

  //OSDeviceCharacter& dev = m_dev;

  // thread endless loop
  for (;;)
    {
      m_eventFlags.clear(OS_ALL_FLAGS);
      m_eventFlags.wait(OS_ALL_FLAGS);

      if ((m_eventFlags.get() & OS_REQUEST_GPS_POSITION) != 0)
        {
          getPosition(m_gpsPosition);
          app.com.rpcSendGpsPosition(m_gpsPosition);
        }
    }
}

OSReturn_t
GpsManager::RequestGpsPosition(void)
{
  m_eventFlags.notify(OS_REQUEST_GPS_POSITION);

  return OSReturn::OS_OK;
}

OSReturn_t
GpsManager::getPosition(GpsPosition_t& p)
{

  GpsLatitude_t& latitude = p.getLatitude(); // N/S
  GpsLongitude_t& longitude = p.getLongitude(); // E/W

#if defined(APP_INCLUDE_GPSMANAGER_GETPOSITION_SIMULATED)

  latitude.setHemisphereNorth();
  latitude.setDegrees(45);
  latitude.setMinutesInteger(35);
  latitude.setMinutesDecimals(1234);

  longitude.setHemisphereEst();
  longitude.setDegrees(26);
  longitude.setMinutesInteger(25);
  longitude.setMinutesDecimals(4321);

  p.setAltitude(-1);

#else

  OSDeviceCharacter& dev = m_dev;
  dev.setReadTimer(&os.sched.timerTicks);

  bool onSt;
  onSt = getOnStatus();

  int n;

  n = sleepExit();
  if (n < 0)
    return OSReturn::OS_TIMEOUT;

  dev.open();
  //Send request position and fix NMEA message
  n = dev.writeBytes((unsigned char*) gpsCmdReqGGA, 15);
  dev.flush();
  if (n < 0)
    {
      if (!onSt)
        sleepEnter();

      return OSReturn::OS_TIMEOUT;
    }

  int c;
  int i;

  unsigned char rB[OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE];
  unsigned char* pRB;
  pRB = &rB[0];
  i = sizeof(rB);
  //Read position and fix NMEA message
  for (;;)
    {
      dev.setReadTimeout(GPS_START_UP_READ_TIMEOUT);
      dev.setReadMatchArray((unsigned char*) "\n");

      n = dev.readBytes(pRB, i, &c);
      if (n < 0)
        {
          //if (onSt)
          // sleepEnter();

          return OSReturn::OS_TIMEOUT;
        }
      i -= n;
      pRB += n;
      if (*(pRB - 1) == '\n')
        break;
    }
  dev.close();

  Parser& parser = m_parser;

  parser.setTokenBuffer(m_token, sizeof(m_token));

  parser.setInput(rB);

  parser.setSeparators((unsigned char*) ",");

  parser.parseToken();

  if (parser.tokenCompare((unsigned char*) "$GPGGA") != 0)
    {
      if (onSt)
        // sleepEnter();

        return OSReturn::OS_ITEM_NOT_FOUND;
    }

  parser.skipTokens(1);

  parser.parseToken();
  long int val;
  parser.parseFixedPrec(&val, 4);
  latitude.setDegrees(val / (100 * 10000));
  val = val - val % 1000000;
  latitude.setMinutesInteger(val / 10000);
  val = val - val % 10000;
  latitude.setMinutesDecimals(val);

  parser.parseToken();
  latitude.setHemisphere(m_token[0]);

  parser.parseToken();
  parser.parseFixedPrec(&val, 4);
  longitude.setDegrees(val / (100 * 10000));
  val = val - val % 1000000;
  longitude.setMinutesInteger(val / 10000);
  val = val - val % 10000;
  longitude.setMinutesDecimals(val);

  parser.parseToken();
  longitude.setHemisphere(m_token[0]);

  parser.parseToken();
  unsigned short valS;
  parser.parseUnsigned(&valS);
  //p.setFix(valS);

  parser.skipTokens(2);
  parser.parseToken((unsigned char*) ".", (unsigned char*) "");
  parser.parseSigned(&val);
  p.setAltitude(val);

  //if (onSt)
  //  sleepEnter();

#endif

  return OSReturn::OS_OK;
}

OSReturn_t
GpsManager::sleepEnter(void)
{
  if (!getOnStatus())
    return OSReturn::OS_OK;

  setOnStatus(false);

  OS_GPIO_PIN_LOW(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
  OS_GPIO_PIN_CONFIG_INPUT(GPS_RST_PORT,GPS_RST_PIN);
  //OS_GPIO_PIN_CONFIG_INPUT(GPS_WK_UP_PORT,GPS_WK_UP_PIN);

  return OSReturn::OS_OK;
}

OSReturn_t
GpsManager::sleepExit(void)
{
  if (getOnStatus())
    return OSReturn::OS_OK;

  int k;
  int c, n;
  unsigned char pRB[OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE];

  OSDeviceCharacter& dev = m_dev;
  dev.setReadTimer(&os.sched.timerTicks);

  dev.setBaudRate(19200);
  dev.open();

  OS_GPIO_PIN_CONFIG_OUTPUT(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
  OS_GPIO_PIN_HIGH(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);

  OS_GPIO_PIN_CONFIG_OUTPUT(GPS_RST_PORT,GPS_RST_PIN);
  OS_GPIO_PIN_HIGH(GPS_RST_PORT,GPS_RST_PIN);

  // OS_GPIO_PIN_CONFIG_OUTPUT(GPS_WK_UP_PORT,GPS_WK_UP_PIN);
  // OS_GPIO_PIN_HIGH(GPS_WK_UP_PORT,GPS_WK_UP_PIN);


#if defined(DEBUG1)
  OSDeviceDebug::putString("opened1");
  OSDeviceDebug::putNewLine();
#endif

  for (k = 0; k < GPS_START_UP_NR_MSG; k++)
    {
      dev.setReadTimeout(GPS_START_UP_READ_TIMEOUT);
#if defined(DEBUG)
      OSDeviceDebug::putString("timeout");
      OSDeviceDebug::putNewLine();
#endif
      dev.setReadMatchArray((unsigned char*) "\n");
#if defined(DEBUG)
      //OSDeviceDebug::putString("match");
      OSDeviceDebug::putNewLine();
#endif

      c = dev.readBytes(pRB, OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE, &n);

      if (c < 0)
        k = GPS_START_UP_NR_MSG;

    }

  dev.close();
#if defined(DEBUG)
  OSDeviceDebug::putString("closed");
  OSDeviceDebug::putNewLine();
#endif
  if ((c == OSReturn::OS_OK) && (pRB[n - 1] == '\n'))
    {
      setOnStatus(true);
      return OSReturn::OS_OK;
    }

  dev.setBaudRate(9600);
#if defined(DEBUG)
  OSDeviceDebug::putString("9600");
  OSDeviceDebug::putNewLine();
#endif
  dev.open();
#if defined(DEBUG)
  OSDeviceDebug::putString("opened");
  OSDeviceDebug::putNewLine();
#endif
  c = dev.writeBytes((unsigned char*) gpsCmdSetOutput, 51);
  dev.flush();

  n = 1;
  pRB[n - 1] = 0;
  c = OSReturn::OS_OK;

  while ((n > 0) && (pRB[n - 1] != '$'))
    {
      dev.setReadTimeout(GPS_MSG_READ_TIMEOUT);
      dev.setReadMatchArray((unsigned char*) "$");

      c = dev.readBytes(pRB, OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE, &n);
    }
  if (c < 0)
    {
      OS_GPIO_PIN_LOW(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
      OS_GPIO_PIN_CONFIG_INPUT(GPS_RST_PORT,GPS_RST_PIN);
      dev.close();
      return OSReturn::OS_TIMEOUT;
    }

  dev.setReadTimeout(GPS_MSG_READ_TIMEOUT);
  dev.setReadMatchArray((unsigned char*) "\n");

  c = dev.readBytes(pRB, OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE, &n);

  if (c < 0)
    {
      OS_GPIO_PIN_LOW(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
      OS_GPIO_PIN_CONFIG_INPUT(GPS_RST_PORT,GPS_RST_PIN);
      dev.close();
      return OSReturn::OS_TIMEOUT;
    }

  c = memcmp(pRB, gpsReplySetSetOutput, 13);

  if (c != 0)
    {
      OS_GPIO_PIN_LOW(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
      OS_GPIO_PIN_CONFIG_INPUT(GPS_RST_PORT,GPS_RST_PIN);
      dev.close();
      return OSReturn::OS_TIMEOUT;
    }

  c = dev.writeBytes((unsigned char*) gpsCmdSetBaud, 19);
  dev.flush();

  //TODO: wait until command has been sent
  while (dev.isSending())
    {
      //Yield
      os.sched.timerTicks.sleep(1);
    }
  os.sched.timerTicks.sleep(30);
  OS_GPIO_PIN_LOW(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
  OS_GPIO_PIN_CONFIG_INPUT(GPS_RST_PORT,GPS_RST_PIN);

  dev.close();

  os.sched.timerTicks.sleep(30);

  OS_GPIO_PIN_HIGH(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
  OS_GPIO_PIN_CONFIG_OUTPUT(GPS_RST_PORT,GPS_RST_PIN);
  OS_GPIO_PIN_HIGH(GPS_RST_PORT,GPS_RST_PIN);

  os.sched.timerTicks.sleep(30);

  dev.setBaudRate(19200);
  dev.open();
  c = dev.writeBytes((unsigned char*) gpsFixReq, 15);

  n = 1;
  pRB[n - 1] = 0;
  c = OSReturn::OS_OK;

  while ((n > 0) && (pRB[n - 1] != '$'))
    {
      dev.setReadTimeout(GPS_MSG_READ_TIMEOUT);
      dev.setReadMatchArray((unsigned char*) "\n");

      c = dev.readBytes(pRB, OS_CFGINT_GPSMANAGER_RXBUF_ARRAY_SIZE, &n);
    }
  if (c < 0)
    {
      OS_GPIO_PIN_LOW(GPS_PWR_SW_PORT,GPS_POWER_SW_PIN);
      OS_GPIO_PIN_CONFIG_INPUT(GPS_RST_PORT,GPS_RST_PIN);
      dev.close();
      return OSReturn::OS_TIMEOUT;
    }

  dev.close();

  setOnStatus(true);

  return OSReturn::OS_OK;
}

void
GpsManager::setOnStatus(bool value)
{
  m_isOn = value;
}

bool
GpsManager::getOnStatus(void)
{
  return m_isOn;
}
