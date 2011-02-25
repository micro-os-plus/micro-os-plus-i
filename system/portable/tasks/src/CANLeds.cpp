/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_CANLEDS)

#include "portable/tasks/include/CANLeds.h"

/*
 * Task constructor. 
 * Initialise system task object, initialise member objects
 * and store parameters in private members.
 *
 */

CANLeds::CANLeds(const char *pName) :
  OSTask(pName, m_stack, sizeof(m_stack))
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CANLeds()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
  }

// ----------------------------------------------------------------------------

CANLed::CANLed()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CANLed()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    m_status = STATUS_OFF;
    m_crtStatus = STATUS_OFF;
    m_count = 0;
  }

unsigned char CANLed::getStatus(void)
  {
    return m_status;
  }

void CANLed::setStatus(unsigned char status)
  {
    m_status = status;
  }

// ----------------------------------------------------------------------------

CANLedGreen::CANLedGreen()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CANLedGreen()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    init();
  }

void CANLedGreen::update(bool last)
  {

    switch (m_count)
      {
    case 0:
      if (!last)
        {
          ++m_count;
          break;
        }

      /* Start Green LED blinking cycle */
      m_crtStatus = m_status;

      switch (m_crtStatus)
        {
      default:
      case STATUS_OFF:
      case STATUS_FLICKERING:
        off();
        m_count = 1; /* -> 0 */
        break;

      case STATUS_ON:
        on();
        m_count = 1;
        break;

      case STATUS_BLINKING:
        off();
        m_count = 2;
        break;

      case STATUS_SINGLE_FLASH:
        off();
        m_count = 6;
        break;

      case STATUS_DOUBLE_FLASH:
        off();
        m_count = 8;
        break;

      case STATUS_TRIPLE_FLASH:
        off();
        m_count = 10;
        break;
        }
      break;

    case 1:
      if (m_crtStatus == STATUS_BLINKING)
        on();
      break;

    case 5:
    case 7:
    case 9:
      on();
      break;

    case 4:
    case 6:
    case 8:
      off();
      break;

    default:
      ;
      }

    --m_count;
  }

// ----------------------------------------------------------------------------

CANLedRed::CANLedRed()
  {
#if defined(DEBUG) && defined(OS_DEBUG_CONSTRUCTORS)
    OSDeviceDebug::putString_P(PSTR("CANLedRed()="));
    OSDeviceDebug::putPtr(this);
    OSDeviceDebug::putNewLine();
#endif
    init();
  }

void CANLedRed::update(bool last)
  {
    switch (m_count)
      {
    case 0:
      if (!last)
        {
          ++m_count;
          break;
        }

      /* Red LED ended blinking cycle */
      m_crtStatus = m_status;

      switch (m_crtStatus)
        {
      case STATUS_ON:
      case STATUS_FLICKERING:
        on();
        m_count = 1;
        break;

      default:
      case STATUS_OFF:
        off();
        m_count = 1;
        break;

      case STATUS_BLINKING:
        on();
        m_count = 2;
        break;

      case STATUS_SINGLE_FLASH:
        on();
        m_count = 6;
        break;

      case STATUS_DOUBLE_FLASH:
        on();
        m_count = 8;
        break;

      case STATUS_TRIPLE_FLASH:
        on();
        m_count = 10;
        break;
        }
      break;

    case 1:
      if (m_crtStatus == STATUS_BLINKING)
        off();
      break;

    case 9:
    case 7:
    case 5:
      off();
      break;

    case 8:
    case 6:
      on();
      break;

    default:
      ;
      }

    --m_count;

  }

// ----------------------------------------------------------------------------

void CANLeds::taskMain(void)
  {
#if defined(DEBUG)
    OSScheduler::lock();
      {
        OSDeviceDebug::putString_P(PSTR("CANLeds::taskMain()"));
        OSDeviceDebug::putNewLine();
      }
      OSScheduler::unlock();
#endif

    m_semiperiods = 1;

    //debug.putDec(OSTimer::microsToTicks(50000));

    // task endless loop
    for (;;)
      {
        OSScheduler::timerTicks.sleep(OSTimerTicks::microsToTicks(50000));

        --m_semiperiods;

        if ((m_semiperiods & 1) != 0)
          {
            if (green.m_crtStatus == CANLed::STATUS_FLICKERING)
              {
                green.on();
              }
            if (red.m_crtStatus == CANLed::STATUS_FLICKERING)
              {
                red.off();
              }
          }
        else
          {
            if (green.m_crtStatus == CANLed::STATUS_FLICKERING)
              {
                green.off();
              }
            if (red.m_crtStatus == CANLed::STATUS_FLICKERING)
              {
                red.on();
              }
          }

        if (m_semiperiods == 4)
          {
            // In the middle of a 400ms cycle (4/8 flickers)

            green.update(false);
            red.update(false);

          }
        else if (m_semiperiods == 0)
          {
            // At the end of a 400ms cycle (8/8 flickers)

            green.update(true);
            red.update(true);

            m_semiperiods = 8;
          }

      }
  }

#endif /*OS_INCLUDE_CANLEDS*/

