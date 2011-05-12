/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include <stdlib.h>
#include "AppSDI12Sensor.h"

AppSDI12Sensor::AppSDI12Sensor(const char *pNameAquire,
    const char *pNamePeriodic, const char *pNameSDI12,
    OSThreadPriority_t priorityAquire, OSThreadPriority_t priorityPeriodic,
    OSThreadPriority_t prioritySDI12) :

  SDI12Sensor(pNameAquire, m_stackAquire, sizeof(m_stackAquire),
      priorityAquire, pNamePeriodic, m_stackPeriodic, sizeof(m_stackPeriodic),
      priorityPeriodic, pNameSDI12, m_stackSDI12, sizeof(m_stackSDI12),
      prioritySDI12)
  {
    if (os.isDebug())
      {
        debug.putString("AppSDI12Sensor()=");
        debug.putPtr(this);
        debug.putNewLine();
      }
  }

AppSDI12Sensor::~AppSDI12Sensor()
  {
    ;
  }

void AppSDI12Sensor::customInit(void)
  {
    if (os.isDebug())
      {
        debug.putString("init() ");
        debug.putNewLine();
      }

    // custom application inits, like read EEPROM values
    // ...
    ms_periodic = APP_CFGINT_SDI12SENSOR_PERIODICITY;

    setPeriodicSeconds(ms_periodic);
  }

void AppSDI12Sensor::storeAddressToNonVolatileMemory(unsigned char addr)
  {
    if (os.isDebug())
      {
        debug.putString("New address: ");
        debug.putChar(addr);
        debug.putNewLine();
      }
  }

// aI!

void AppSDI12Sensor::prepareResponseSendIdentification()
  {
    //debug.putString("Info ");
    storeStringResponse("13" "AVI     " "TEST  " "1.1" " 987654321");
  }

// aR0! aR9! aRC0! aRC9!

void AppSDI12Sensor::prepareResponseContinuousMeasurement(unsigned char digit)
  {
    int i;
    i = 0;
    if (digit == 0)
      {
        // R0
        storeRValue(i++, 1.23f, 2);
        storeRValue(i++, -1234.567f, 4);
        storeRValue(i++, 12345678.9f, 4); // overflow, shown as 9999999
        storeRValue(i++, -1234.567f, 2);
        storeRValue(i++, -0.1234567f, 7);
      }
    else if (digit == 1)
      {
        // R1
        storeRValue(i++, 68.5f, 3);
        storeRValue(i++, 69.5f, 3);
        storeRValue(i++, 71.5f, 3);
        storeRValue(i++, 72.5f, 3);
        storeRValue(i++, 9.9f, 3);
        storeRValue(i++, 9.1f, 3);
      }
    else if (digit == 2)
      {
        // R2
        storeRValue(i++, 82.0005f, 6); // shown as 82.00051
        storeRValue(i++, 82.00005f, 6); // shown as 82.00006
        storeRValue(i++, 823.005f, 5);
        storeRValue(i++, -0.05f, 4);
      }
  }

// aM! aM1! aM9! aMC! aMC1! aMC9!
// aC! aC1! aC9! aCC! aCC1! aCC9!
// aV!

void AppSDI12Sensor::prepareResponseStartMeasurement(unsigned char cmd,
    unsigned char digit)
  {
    unsigned char count;
    unsigned int seconds;
    unsigned short delay;

    count = 0;
    seconds = 0;
    delay = 0;

    if (cmd == 'V')
      {
        // V command, should prepare a verification response, 
        // like ROM signature, CRC's, RAM test results, diagnostics, etc
        // if you do not need it, just leave these values null
        count = 3;
        seconds = 10;
      }
    else if (cmd == 'M')
      {
        // M commands, should prepare a measurement response 
        switch (digit)
          {
        case 0: // M0
          seconds = APP_CFGINT_SDI12SENSOR_M0_DURATION;
          count = APP_CFGINT_SDI12SENSOR_M0_COUNT;
          delay = digit;
          break;

        case 1: // M1
          seconds = APP_CFGINT_SDI12SENSOR_M1_DURATION;
          count = APP_CFGINT_SDI12SENSOR_M1_COUNT;
          delay = digit;
          break;

        case 2: // M2
          seconds = APP_CFGINT_SDI12SENSOR_M2_DURATION;
          count = APP_CFGINT_SDI12SENSOR_M2_COUNT;
          delay = digit;
          break;

        default:
          return;
          }

      }
    else if (cmd == 'C')
      {
        // C commands, should prepare a concurrent measurement response

        // sample, we assume only a few commands accepted, 
        // in real life it should be more elaborated, as above.
        if (digit < 5)
          {
            count = digit + 1;
            seconds = 2 * digit + 1;
            delay = digit;
          }
        else
          return;
      }
    else
      return;

    // return values
    storeStartMeasurementResponse(seconds, count);
    //storeStartMeasurementDelay(delay);
  }

// aX...!

// input buffer starts with command, just after address.
// should call storeCharResponse() to set response
bool AppSDI12Sensor::prepareResponseExtendedCommands(const char *pBuf,
    unsigned int count)
  {
    count = count;

    // sample command aXX!
    if (pBuf[0] == 'X'&& pBuf[1] == 'X')
      {
        storeCharResponse('O');
        storeCharResponse('K');

        storeDelayedMode(DELAYED_MODE_PROTO);
      }
    else
      {
        // unrecognized command, do nothing
        return false;
      }

    return true;
  }

/*
 * Data Acquisition code.
 * Runs on low priority thread, so speed is not critical.
 * The only requirement is to finish before the declared value 
 * returned in M!
 * The results should be sent back via storeDValue().
 */

void AppSDI12Sensor::prepareMeasuredData(unsigned char cmd,
    unsigned char digit, unsigned int seconds, unsigned char count)
  {
    OSTimerTicks_t ticksBegin;
    ticksBegin = os.sched.timerSeconds.getTicks();

    if (os.isDebug())
      {
        debug.putString("measure ");
        debug.putChar(cmd);
        debug.putChar(digit+'0');
        debug.putChar(' ');
      }

    // seed random generator, not necessary in real applications
    srand(os.sched.timerTicks.getTicks());

    int i;
    if (cmd == 'V')
      {
        // generate random response values
        for (i = 0; i < count; ++i)
          {
            storeDValue(i, (rand() % 100) * 1.234f, i % 3);
          }
      }
    else if (cmd == 'M')
      {
        // generate random response values
        for (i = 0; i < count; ++i)
          {
            float f;
            f = (rand() % 100) * 1.23456f;
            if (i & 1)
              f = -f;

            storeDValue(i, f, (count - i) % 4);
          }
      }

    // simulate acquisition duration
    for (i = 0; i < (int)seconds; ++i)
      {
        os.sched.timerSeconds.sleep(1);
        if (os.isDebug())
          {
            debug.putChar('.');
          }
      }

    if (os.isDebug())
      {
        debug.putString(" completed");
        debug.putNewLine();
      }

    if (os.isDebug())
      {
        if (os.sched.timerSeconds.getTicks() - ticksBegin > seconds)
          {
            debug.putString("WARNING: ");
            debug.putChar(cmd);
            debug.putChar(digit+'0');
            debug.putString(" duration exceeded");
            debug.putNewLine();
          }
      }
  }

/*
 * Runs on the fast thread to process delayed action,
 * like storing values in eeprom for extended commands.
 */

void AppSDI12Sensor::processDelayedActions(void)
  {
    if (os.isDebug())
      {
        debug.putString("delayed ");;
      }
  }

/*
 * Periodic Data Acquisition code.
 * Runs on periodic thread, independently of the other two.
 * If resources are used in common with the acquisition thread,
 * synchronization objects must be used (like mutex).
 */
void AppSDI12Sensor::processPeriodicMeasurements(void)
  {

    OSTimerTicks_t ticksBegin;
    ticksBegin = os.sched.timerSeconds.getTicks();

    if (os.isDebug())
      {
        debug.putString("[ ");
      }

    // simulate acquisition duration
    unsigned int i;
    for (i = 0; i < APP_CFGINT_SDI12SENSOR_PERIODIC_DURATION&& i < ms_periodic; ++i)
      {
        os.sched.timerSeconds.sleep(1);
        if (os.isDebug())
          {
            debug.putChar('.');
          }
      }

    if (os.isDebug())
      {
        debug.putString(" ]");
        debug.putNewLine();
      }

    if (os.isDebug())
      {
        if ((os.sched.timerSeconds.getTicks() - ticksBegin) > ms_periodic)
          {
            debug.putString("WARNING: periodic duration exceeded");
            debug.putNewLine();
          }
      }
  }

    unsigned char AppSDI12Sensor::m_stackSDI12[APP_CFGINT_SDI12SENSOR_PROTO_STACK_SIZE];
    unsigned char AppSDI12Sensor::m_stackAquire[APP_CFGINT_SDI12SENSOR_DACQ_STACK_SIZE];
    unsigned char AppSDI12Sensor::m_stackPeriodic[APP_CFGINT_SDI12SENSOR_PERIODIC_STACK_SIZE];

    unsigned short AppSDI12Sensor::ms_periodic;

