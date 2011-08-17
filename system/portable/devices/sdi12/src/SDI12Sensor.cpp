/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_SDI12SENSOR) || defined(OS_INCLUDE_SDI12SENSOR_TINY)

#include "portable/kernel/include/OS.h"

#include "portable/devices/sdi12/include/SDI12Sensor.h"
#include "portable/devices/debug/include/OSDebugLed1.h"
#include "portable/devices/debug/include/OSDebugLed2.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// ----- Static Data ----------------------------------------------------------

volatile unsigned char SDI12Sensor::ms_ownAddress;

unsigned char SDI12Sensor::ms_buf[OS_CFGINT_SDI12SENSOR_BUFFER_SIZE
    + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA];

volatile unsigned short SDI12Sensor::ms_bufCount;

volatile unsigned char* SDI12Sensor::ms_pBuf;
volatile unsigned char* SDI12Sensor::ms_pBufMax;

unsigned char* SDI12Sensor::ms_pParse;

unsigned char SDI12Sensor::ms_tmp[OS_CFGINT_SDI12SENSOR_TMP_SIZE];

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
unsigned short SDI12Sensor::ms_crc;
unsigned char SDI12Sensor::ms_exclamationTicks;
bool SDI12Sensor::bUseCRC;
#endif

#if defined(OS_INCLUDE_SDI12SENSOR)

OSThread* SDI12Sensor::ms_pThread;
OSThread* SDI12Sensor::ms_pThreadA;

unsigned char SDI12Sensor::ms_doReset;
volatile OSTimerTicks_t SDI12Sensor::ms_periodicSeconds;

volatile OSTimerTicks_t SDI12Sensor::ms_timerTimeout;
volatile OSTimerTicks_t SDI12Sensor::ms_timerMarking;
//bool ms_cancelOnBreak;
volatile bool SDI12Sensor::ms_breakDetectEnable;
volatile OSTimerTicks_t SDI12Sensor::ms_cntMarking;

volatile OSTimerTicks_t SDI12Sensor::ms_ticksBRK;
volatile OSTimerTicks_t SDI12Sensor::ms_ticksExclamation;

volatile unsigned char SDI12Sensor::ms_outIndex;
volatile bool SDI12Sensor::ms_useCRC;
volatile bool SDI12Sensor::ms_cmdCRC;

volatile unsigned char SDI12Sensor::ms_measureCmd;
volatile unsigned char SDI12Sensor::ms_measureCmdDigit;
volatile unsigned char SDI12Sensor::ms_currentCmd;
volatile unsigned char SDI12Sensor::ms_currentCmdDigit;
unsigned int SDI12Sensor::ms_dSeconds;
volatile unsigned char SDI12Sensor::ms_nextDindex;

volatile unsigned char SDI12Sensor::ms_currentDindex;
volatile unsigned char SDI12Sensor::ms_previousDindex;
volatile unsigned char SDI12Sensor::ms_previousDdigit;

volatile int SDI12Sensor::ms_timeoutCounter;
volatile int SDI12Sensor::ms_failedMarkingCounter;

volatile bool SDI12Sensor::ms_bDAvailable;
volatile bool SDI12Sensor::ms_bAcquire;
volatile bool SDI12Sensor::ms_bServiceRequest;

unsigned char SDI12Sensor::ms_delayedMode;

//volatile OSTimerTicks_t SDI12Sensor::ms_exclamationTicks;

OSTime_t SDI12Sensor::ms_periodicNextSecond;
OSEventFlags SDI12Sensor::ms_flags;

//bool ms_bIsCancelled;
unsigned char SDI12Sensor::ms_state;
volatile OSTimerTicks_t SDI12Sensor::ms_ticksRX;
bool SDI12Sensor::ms_moreProcessing;

SDI12Value_t SDI12Sensor::ms_rValues[OS_CFGINT_SDI12SENSOR_RVALUES_SIZE];
unsigned char SDI12Sensor::ms_rCount;

#if !defined(OS_EXCLUDE_PCINT3_VECT)
unsigned char SDI12Sensor::ms_prevValue3;
#endif

SDI12Value_t SDI12Sensor::ms_dValues[OS_CFGINT_SDI12SENSOR_DVALUES_SIZE];
unsigned char SDI12Sensor::ms_dCount;

#endif

#if defined(OS_INCLUDE_SDI12SENSOR)

SDI12Sensor::SDI12Sensor(const char* pNameAcquire,
    const unsigned char* pStackAcquire, unsigned short stackSizeAcquire,
    OSThreadPriority_t priorityAcquire, const char* pNamePeriodic,
    const unsigned char* pStackPeriodic, unsigned short stackSizePeriodic,
    OSThreadPriority_t priorityPeriodic, const char* pNameSDI12,
    const unsigned char* pStackSDI12, unsigned short stackSizeSDI12,
    OSThreadPriority_t prioritySDI12)

:
      threadSDI12(pNameSDI12, (OSThreadMainPtr_t) staticMainSDI12,
          (void*) this, pStackSDI12, stackSizeSDI12, prioritySDI12),

      threadAcquire(pNameAcquire, (OSThreadMainPtr_t) staticMainAcquire,
          (void*) this, pStackAcquire, stackSizeAcquire, priorityAcquire),

      threadPeriodic(pNamePeriodic, (OSThreadMainPtr_t) staticMainPeriodic,
          (void*) this, pStackPeriodic, stackSizePeriodic, priorityPeriodic)
{
  OSDeviceDebug::putConstructor_P(PSTR("SDI12Sensor"), this);

  //ms_bInitialised = false;

  ms_pThread = &threadSDI12;
  ms_pThreadA = &threadAcquire;

  ms_doReset = 0;
  ms_periodicSeconds = 0; // no periodic acquisition

  ms_timerTimeout = 0;
  ms_timerMarking = 0;

  ms_timeoutCounter = 0;
  ms_failedMarkingCounter = 0;

  disableBreakDetect();
}

SDI12Sensor::~SDI12Sensor()
{
  OSDeviceDebug::putDestructor_P(PSTR("SDI12Sensor"), this);
}

void
SDI12Sensor::processPeriodicMeasurements(void)
{
  ; // virtual
}

// ----------------------------------------------

void
SDI12Sensor::cancelPeriodic(void)
{
  threadPeriodic.requestInterruption();
}

void
SDI12Sensor::cancelAcquisition(void)
{
  //ms_bIsCancelled = true;

  // request interruption of acquisition thread
  threadAcquire.requestInterruption();
}

void
SDI12Sensor::enableTimeout(OSTimerTicks_t ticks)
{
  ms_timerTimeout = ticks;
}

void
SDI12Sensor::disableTimeout(void)
{
  ms_timerTimeout = 0;
}

void
SDI12Sensor::enableMarking(OSTimerTicks_t ticks)
{
  ms_timerMarking = ticks;
  ms_cntMarking = ticks;
}

void
SDI12Sensor::disableMarking(void)
{
  ms_timerMarking = 0;
  ms_cntMarking = 0;
}

void
SDI12Sensor::enableBreakDetect(void)
{
  ms_breakDetectEnable = true;
  ms_ticksBRK = OSScheduler::timerTicks.getTicks();
}

// critical section moved here from OSTimerTicks::interruptServiceRoutine
void
SDI12Sensor::interruptTick(void)
{
  OSCriticalSection::enter();
    {
      //OSDeviceDebug::putChar('.');
      if (ms_timerTimeout != 0)
        {
          if (--ms_timerTimeout == 0)
            {
              ms_flags.notify(TIMEOUT);
#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTTICK)
              OSDeviceDebug::putString(" TO ");
#endif
            }
        }

      if (ms_timerMarking != 0)
        {
          //OSDeviceDebug::putChar(',');
          if (!pinChangedIsHigh())
            {
              // pin is low, automatically reset timer
              ms_timerMarking = ms_cntMarking;
              // and notify task that this is not a marking condition
              ms_flags.notify(MARKING_CANCELLED);
            }
          else
            {
              if (--ms_timerMarking == 0)
                {
                  ms_flags.notify(MARKING);
#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTTICK)
                  OSDeviceDebug::putString("^");
#endif
                }
            }
        }

      if (false /* ms_bIsCancelled */)
        {
          // if acquisition thread is in waiting state
          if (ms_pThreadA->isWaiting())
            {
#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTTICK)
              OSDeviceDebug::putChar('>');
#endif
              // iterate cancellation of pending event
              OSScheduler::eventNotify(ms_pThreadA->getEvent(),
                  OSEventWaitReturn::OS_CANCELED);
            }
        }
    }
  OSCriticalSection::exit();
}

// ---------------------------------------------------------------------------

void
SDI12Sensor::staticMainSDI12(SDI12Sensor *pt)
{

  pt->threadMainSDI12();
  for (;;)
    {
      ; // watchdog should trigger
    }
}

/*
 * Thread main code.
 *
 */

void
SDI12Sensor::threadMainSDI12(void)
{
  OSSchedulerLock::enter();
    {
#if defined(DEBUG)
      OSDeviceDebug::putString("SDI12Sensor::threadMainSDI12()");
      OSDeviceDebug::putNewLine();
#endif

      ms_bServiceRequest = false;

      // implementation initialization
      init();

      // application initialization
      customInit(); // call virtual

      // must be after customInit() where pin change is enabled
      interruptPinChangeEnable();

      // start acquisition thread;
      // all initializations must be done at this point
      ms_flags.notify(INITIALIZED);
    }
  OSSchedulerLock::exit();

  OSDebugLed1::init();
  OSDebugLed1::on();

  OSDebugLed2::init();
  OSDebugLed2::off();

  OSEventFlagsBits_t flags;

  unsigned char prevAddress;
  prevAddress = 0;

  bool bIsCcmd;
  bIsCcmd = false;

  OSThread* pThreadSDI12;
  pThreadSDI12 = OSScheduler::getThreadCurrent();

  OSThread* pThreadIdle;
  pThreadIdle = OSScheduler::getThreadIdle();

  OSCpuSleepCriticalSection& cpuSleepCritical =
      ms_pThread->getCpuSleepCriticalSection();

  // initial state
  ms_state = STATE0;

  // thread endless loop
  for (;;)
    {
#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_STATE)
      OSDeviceDebug::putString(" S");
      OSDeviceDebug::putDec(ms_state);
      OSDeviceDebug::putChar(' ');
#endif

      // Allow to enter sleep
      cpuSleepCritical.clear();

      switch (ms_state)
        {
      case STATE0:
        // low power standby, waiting for break

        pThreadSDI12->virtualWatchdogSet(0);

        ms_flags.clear(BREAK);

        // disable USART reception
        usartRxDisable();

        // enable break detection
        enableBreakDetect();

        // wait for break and clear condition
        ms_flags.wait(BREAK);
        flags = ms_flags.get();

        // For just in case, the sleep critical section status
        // should be already cleared by the interrupt
        cpuSleepCritical.clear();

        pThreadSDI12->virtualWatchdogSet(
            OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS);

        ms_state = STATE1;
        break;

      case STATE1:
        // initialise the sensor, without clearing any previously measured data
        // in it's buffer

        prevAddress = 0; // not much to do...

        ms_state = STATE2;
        break;

      case STATE2:
        // look for 8.33ms marking

        cpuSleepCritical.enter();
          {
            // enable rx early, otherwise we loose address
            clearInputBuff();
            usartRxEnable();

            OSDeviceDebug::putChar('(');
            ms_flags.clear(MARKING | RX_ERROR | MARKING_CANCELLED);
            enableMarking(MARKING8_TICKS);
            ms_flags.wait(MARKING | RX_ERROR | MARKING_CANCELLED);
            flags = ms_flags.get();
            disableMarking();
            OSDeviceDebug::putChar(')');
          }
        cpuSleepCritical.exit();

        if ((flags & (RX_ERROR | MARKING_CANCELLED)) != 0)
          {
            OSDeviceDebug::putString("->0");

            // count the number of failed attempts to identify a marking
            ms_failedMarkingCounter++;

            // NOTICE: this is not in the specs, but we use it as an
            // safety net, in case hunting marking is not fully accurate
            // (without it the virtual WD will trigger)
            ms_state = STATE0;
          }
        else if ((flags & MARKING) != 0)
          {
            ms_state = STATE3;
          }

        break;

      case STATE3:
        // look for address, break or 100 ms timeout

        cpuSleepCritical.enter();
          {
            ms_flags.clear(ADDRESS | BREAK | TIMEOUT);
            enableTimeout(TIMEOUT100_TICKS);
            ms_flags.wait(ADDRESS | BREAK | TIMEOUT);
            flags = ms_flags.get();
            disableTimeout();
          }
        cpuSleepCritical.exit();

        if ((flags & BREAK) != 0)
          {
            ms_state = STATE1;
          }
        else if ((flags & TIMEOUT) != 0)
          {
            ms_state = STATE0;
          }
        else if ((flags & ADDRESS) != 0)
          {
            if (isAddressValid(ms_buf[0]))
              {
                // address valid
                ms_state = STATE4;
              }
            else
              {
                ms_state = STATE0;
              }
          }
        break;

      case STATE4:
        // look for !, or 8.33 ms marking while loading the command

        cpuSleepCritical.enter();
          {
            ms_flags.clear(EXCLAMATION | MARKING);
            enableMarking(MARKING8_TICKS);
            ms_flags.wait(EXCLAMATION | MARKING);
            flags = ms_flags.get();
            disableMarking();

            if ((flags & EXCLAMATION) != 0)
              {
                // ms_ticksExclamation = ms_ticksRX; // Already set in IRQ

                usartRxDisable();

                prevAddress = ms_ownAddress;
                bIsCcmd = (ms_buf[1] == 'C');

                // process request, result is in the same buffer
                // if invalid command, return false
                if (processCommand()) // sleep is disabled
                  {
                    ms_state = STATE5;
                  }
                else
                  {
                    ms_state = STATE2;
                  }
              }
            else if ((flags & MARKING) != 0)
              {
                ms_state = STATE3;
              }
          }
        cpuSleepCritical.exit();

        break;

      case STATE5:
        // send the response, release the data line

        cpuSleepCritical.enter();
          {
            unsigned int delta;
            delta = OSScheduler::timerTicks.getTicks() - ms_ticksExclamation;

            if (delta < OSTimerTicks::microsToTicks(8330))
              {
                // be sure the mandatory marking is respected
                OSScheduler::timerTicks.sleep(
                    OSTimerTicks::microsToTicks(8330) - delta + 1);
              }
            else if (delta > OSTimerTicks::microsToTicks(14000))
              {
                // The specs require the response to be ready in 15 millis
                // What to do if the response is not ready in the required time?
                //
                // Our choice is count the timeouts and to not transmit the
                // response at all (an obvious timeout is better than a
                // marginal one)
                // NOTICE: this is not in the specs!


                ms_timeoutCounter++;

                ms_state = STATE2;

#if defined(DEBUG)
                OSDeviceDebug::putString(" ign ");
#endif
                //break;
              }

            if (ms_state == STATE5)
              {
                transmitResponse();

                // Was the command a D?
                if (ms_currentCmd == 'D')
                  {
                    // Assume the recorder received this D properly;
                    // advance the index for the next D
                    ms_nextDindex = ms_currentDindex;

#if defined(OS_DEBUG_SDI12SENSOR_D_COMMAND)
                    OSDeviceDebug::putString(" D");
                    OSDeviceDebug::putDec(ms_previousDdigit);
                    OSDeviceDebug::putChar('@');
                    OSDeviceDebug::putDec(ms_previousDindex);
                    OSDeviceDebug::putChar('-');
                    OSDeviceDebug::putDec(ms_nextDindex);
                    OSDeviceDebug::putNewLine();
#endif
                  }

                if (!ms_moreProcessing)
                  {
                    ms_state = STATE3;
                  }
                else if (bIsCcmd)
                  {
                    // C command processing
                    ms_state = STATE7;
                  }
                else
                  {
                    // normal (non-C) processing
                    ms_state = STATE6;
                  }
              }
          }
        cpuSleepCritical.exit();

        break;

      case STATE6:
        // look for break while processing non C command

        if (ms_bAcquire)
          {
            ms_bAcquire = false;

            enableBreakDetect();

            // Start the acquisition process
            ms_flags.notify(ACQUIRE);

            pThreadSDI12->virtualWatchdogSet(
                ms_dSeconds + OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS);

            // Wait for the acquisition process to complete
            // It might be prematurely terminated by break
            ms_flags.clear(ACQUIRE_COMPLETED | BREAK);
            ms_flags.wait(ACQUIRE_COMPLETED | BREAK);
            flags = ms_flags.get();

            disableBreakDetect();

            // For just in case of unbalanced interrupts
            cpuSleepCritical.clear();

            if (flags & BREAK)
              {
                // initiaite cancellation of M acquisition
                cancelAcquisition();
                // wait acquisition to complete
                ms_flags.wait(ACQUIRE_COMPLETED);
                flags = ms_flags.get();

                // acknowledge thread interrupted
                threadAcquire.ackInterruption();

                // mark D values are no longer available
                // no need to clearDValues();
                ms_bDAvailable = false;

#if defined(DEBUG)
                OSDeviceDebug::putDec(
                    OSScheduler::timerTicks.getTicks() - ms_ticksBRK);
                OSDeviceDebug::putChar('y');
#endif
                ms_state = STATE2;
              }
            else
              {
                // mark D values are available
                ms_bDAvailable = true;

                if (ms_bServiceRequest)
                  {
                    ms_bServiceRequest = false;

#if defined(DEBUG)
                    OSDeviceDebug::putString(" srv ");
#endif
                    transmitOwnAddressResponse();

                    //OSDeviceDebug::putString(" done ");
                  }
              }
          }

        if (ms_state == STATE6)
          {
            // When transmission is known to be finished,
            // update the address; the standard gives us maximum 1 second
            if (prevAddress != ms_ownAddress)
              {
                storeAddressToNonVolatileMemory(ms_ownAddress);
              }

            if (ms_delayedMode == DELAYED_MODE_PROTO)
              {
                cpuSleepCritical.enter();
                  {
                    processDelayedActions();
                  }
                cpuSleepCritical.exit();
              }

            if (ms_doReset != 0)
              {
#if defined(DEBUG)
                OSDeviceDebug::putString(" soft reset ");
                OSDeviceDebug::putChar(ms_doReset);
                OSDeviceDebug::putNewLine();
#endif
                processSoftReset(ms_doReset);
              }
          }
        ms_state = STATE3;

        break;

        // Concurrent C commands have a special treatment
        // to allow for late cancellation only by valid commands

      case STATE7:
        // look for break
        // while processing the C command

        // re-enable to detect break
        enableBreakDetect();

        // Start the acquisition process
        ms_flags.notify(ACQUIRE);

        pThreadSDI12->virtualWatchdogSet(
            ms_dSeconds + OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS);

        // Wait for the acquisition process to complete
        // It might be prematurely terminated by break
        ms_flags.clear(ACQUIRE_COMPLETED | BREAK);
        ms_flags.wait(ACQUIRE_COMPLETED | BREAK);
        flags = ms_flags.get();

        // Just to compensate for unmatched break detect interrupts
        cpuSleepCritical.clear();

        if (flags & BREAK)
          {
            ms_state = STATE8;
          }
        else
          {
            // mark D values are available
            ms_bDAvailable = true;

            // acquisition completed normally
            ms_state = STATE0;
          }
        break;

      case STATE8:
        // look for address, break or 100 ms timeout
        // while processing the C command

        cpuSleepCritical.enter();
          {
            clearInputBuff();
            usartRxEnable();

            ms_flags.clear(ACQUIRE_COMPLETED | ADDRESS | BREAK | TIMEOUT);
            enableTimeout(TIMEOUT100_TICKS);
            ms_flags.wait(ACQUIRE_COMPLETED | ADDRESS | BREAK | TIMEOUT);
            flags = ms_flags.get();

            disableTimeout();

            if ((flags & BREAK) != 0)
              {
                ms_state = STATE8;
              }
            else if ((flags & TIMEOUT) != 0)
              {
                ms_state = STATE7;
              }
            else if ((flags & ADDRESS) != 0)
              {
                if (isAddressValid(ms_buf[0]))
                  {
                    // address valid
                    ms_state = STATE9;
                  }
                else
                  {
                    ms_state = STATE7;
                  }
              }
            else if ((flags & ACQUIRE_COMPLETED) != 0)
              {
                ms_state = STATE0;
              }
          }
        cpuSleepCritical.exit();

        break;

      case STATE9:
        // look for address, break or 100 ms timeout
        // while processing the C command

        cpuSleepCritical.enter();
          {
            ms_flags.clear(EXCLAMATION | MARKING);
            enableMarking(MARKING8_TICKS);
            ms_flags.wait(EXCLAMATION | MARKING);
            flags = ms_flags.get();

            disableMarking();

            if ((flags & EXCLAMATION) != 0)
              {
                // ms_ticksExclamation = ms_ticksRX; // already don in IRQ

                usartRxDisable();

                prevAddress = ms_ownAddress;

                // process request, result is in the same buffer
                // if bad address, false
                if (processCommand())
                  {
                    // cancel C acquisition
                    cancelAcquisition();
                    ms_flags.wait(ACQUIRE_COMPLETED);
                    flags = ms_flags.get();

                    // to stop automatic cancellation notification
                    //ms_bIsCancelled = false;
                    // acknowledge thread interrupted
                    threadAcquire.ackInterruption();

                    ms_state = STATE5;

                    // disable for now, may be set again if new command
                    // does a new acquisition
                    ms_bDAvailable = false;
                  }
                else
                  {
                    ms_state = STATE7;
                  }
              }
            else if ((flags & MARKING) != 0)
              {
                ms_state = STATE7;
              }
          }
        cpuSleepCritical.exit();

        break;

      default:
#if defined(DEBUG)
        OSDeviceDebug::putString(" NA ");
#endif
        break;
        }

      // go to next state
    }
}

void
SDI12Sensor::transmitOwnAddressResponse(void)
{
  OSCpuSleepCriticalSection& cpuSleepCritical =
      ms_pThread->getCpuSleepCriticalSection();

  unsigned char* pBuf;
  pBuf = &ms_buf[0];
  *pBuf++ = ms_ownAddress;
  *pBuf++ = '\r';
  *pBuf++ = '\n';
  ms_bufCount = pBuf - &ms_buf[0];

  cpuSleepCritical.enter();
    {
      transmitResponse(); // transmit service request
    }
  cpuSleepCritical.exit();
}

void
SDI12Sensor::clearDindex(void)
{
  ms_nextDindex = 0;
  ms_currentDindex = 0;
}

void
SDI12Sensor::clearPreviousDdigit(void)
{
  // Here legal values are 0-9, so any other values are equally good
  ms_previousDdigit = 0xFF;
}

// if bad command or bad address, return false
// this will make the stame machine to go to STATE2
// i.e. the command will timeout
bool
SDI12Sensor::processCommand(void)
{
  // clear all possible conditions
  ms_moreProcessing = false;
  ms_cmdCRC = false;
  ms_bAcquire = false;
  ms_delayedMode = DELAYED_MODE_NONE;
  ms_doReset = 0;
  ms_bServiceRequest = false;
  ms_currentCmdDigit = 0xFF;
  ms_measureCmdDigit = 0xFF;

  unsigned char cnt;
  cnt = ms_bufCount;

  ms_buf[cnt] = '\0';
  unsigned char addr;
  addr = ms_buf[0];

  unsigned char cmd;
  cmd = ms_buf[1];
  ms_currentCmd = cmd;

  unsigned char cmdp1;
  cmdp1 = ms_buf[2];

  bool cmdC;
  unsigned char cmdDigit;
  if (cmdp1 == 'C')
    {
      cmdC = true;
      cmdDigit = ms_buf[3];
      cnt--;
    }
  else
    {
      cmdC = false;
      cmdDigit = cmdp1;
    }
  if (cnt > 2)
    cmdDigit -= '0';
  else
    cmdDigit = 0;

  ms_currentCmdDigit = cmdDigit;

  // add string terminating mark
  //unsigned char* ms_pBuf;
  ms_pBuf = &ms_buf[0];

  *ms_pBuf++ = ms_ownAddress;

  if (addr == '?' && cnt == 1)
    {
      // ?! - a<CR><LF> : address query
      ;
    }
  else if ((addr != ms_ownAddress)
#if OS_CFGBOOL_SDI12SENSOR_SUPPORT_WILD_CARD_ADDRESS
      && (addr != '?')
#endif
  )
    return false; // bad address

  if (cnt == 1)
    {
      // a! - a<CR><LF> : acknowledge active
      ;
    }
  else
    {
      // For D commands, we need to remember the digit of the previous command.
      // All other commands must clear this information.
      if (cmd != 'D')
        clearPreviousDdigit();

      switch (cmd)
        {
      case 'I':
        if (cnt == 2)
          {
            // aI! - allcc..xx<CR><LF> : send identification
            ms_pBufMax = &ms_buf[33]; // 33 is max size as per specs
            prepareResponseSendIdentification();
            // user provides string via storeStringResponse()
          }
        else
          {
            return false;
          }
        break;

      case 'A':
        if (cnt == 3)
          {
            // aAb! - b<CR><LF> : change address

            if (!isalnum(cmdp1))
              {
                return false;
              }
            ms_ownAddress = cmdp1;
            ms_buf[0] = cmdp1;
            ms_moreProcessing = true;
          }
        else
          {
            return false;
          }
        break;

      case 'V':
        if (cnt == 2)
          {
            // aV! - atttn<CR><LF> : start verification

            goto mc_lbl;
          }
        return false;

      case 'M':
      case 'C':
        if ((cmdDigit <= 9) && (cnt <= 3))
          {
            // aM! - atttn<CR><LF> : start measurement
            // aM1! aM9! aMC! aMC1! amC9!

            // aC! - attnn<CR><LF> : start concurrent measurement
            // aC1! aC9! aCC! aCC1! aCC9!

            if (cnt == 3 && cmdDigit == 0)
              {
                return false;
              }
            mc_lbl: ;
            ms_dSeconds = 0;
            ms_dCount = 0;

            prepareResponseStartMeasurement(cmd, cmdDigit);

            ms_pBufMax = &ms_buf[10];

            storeUnsignedResponse((unsigned short) ms_dSeconds,
                (unsigned short) 3);
            storeUnsignedResponse((unsigned short) ms_dCount,
                (unsigned short) ((cmd == 'C') ? 2 : 1));

            ms_measureCmd = cmd;
            ms_measureCmdDigit = cmdDigit;
            clearDindex();
            ms_bDAvailable = false;
            ms_useCRC = cmdC;

            // do not initiate acquisition if no data is
            // expected to be returned
            ms_bAcquire = (ms_dCount != 0);

            if (((ms_measureCmd == 'M') || (ms_measureCmd == 'V'))
                && (ms_dSeconds != 0))
              {
                ms_bServiceRequest = true;
              }
            else
              {
                ms_bServiceRequest = false;
              }

            ms_moreProcessing = ms_bAcquire;
          }
        else
          {
            return false;
          }
        break;

      case 'D':
        if ((cmdDigit <= 9) && (cnt == 3))
          {
            if ((ms_dCount != 0) && ms_bDAvailable)
              {
                // aD0! - a<values><CR><LF> : send data
                // aD1! aD9!

                // ms_dPtr points to ms_dCount values to be returned
                ms_cmdCRC = ms_useCRC;

                ms_currentCmdDigit = cmdDigit;

#if defined(OS_DEBUG_SDI12SENSOR_D_COMMAND)
                OSDeviceDebug::putChar('K');
                OSDeviceDebug::putChar('0' + cmdDigit);
                OSDeviceDebug::putChar('0' + ms_previousDdigit);
#endif

                if (cmdDigit == 0)
                  {
                    clearDindex();
                  }
                else if (cmdDigit == ms_previousDdigit)
                  {
                    // Same D command as before, start from the same index
                    ms_currentDindex = ms_previousDindex;
#if defined(OS_DEBUG_SDI12SENSOR_D_COMMAND)
                    OSDeviceDebug::putChar('P');
                    OSDeviceDebug::putChar('0' + ms_currentDindex);
#endif
                  }
                else if (cmdDigit == (ms_previousDdigit + 1))
                  {
                    // Continue from where we left on the previous D
                    ms_currentDindex = ms_nextDindex;
#if defined(OS_DEBUG_SDI12SENSOR_D_COMMAND)
                    OSDeviceDebug::putChar('0' + ms_currentDindex);
#endif
                  }
                else
                  {
                    // Due to memory limitations, we cannot reply with
                    // the same data to previously replied commands, so
                    // better return an empty response
                    ms_currentDindex = 0xFF;
                  }

                if (ms_currentDindex != 0xFF)
                  {
                    // Remember the current command digit and index, for just in case
                    // the data recorder resends it
                    ms_previousDdigit = ms_currentCmdDigit;
                    ms_previousDindex = ms_currentDindex;

                    if (ms_measureCmd == 'C')
                      {
                        if (sizeof(ms_buf) > 75
                            + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA)
                          {
                            ms_pBufMax = &ms_buf[75
                                + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA]; // limit to 75
                          }
                        else
                          {
                            ms_pBufMax = &ms_buf[sizeof(ms_buf)];
                          }
                      }
                    else
                      {
                        if (sizeof(ms_buf) > 35
                            + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA)
                          {
                            ms_pBufMax = &ms_buf[35
                                + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA]; // limit to 35
                          }
                        else
                          {
                            ms_pBufMax = &ms_buf[sizeof(ms_buf)];
                          }
                      }

                    ms_pBufMax -= 1; // adjust for last usable, not next
                    ms_pBufMax -= 2; // reserve space for <cr><lf>
                    if (ms_cmdCRC)
                      ms_pBufMax -= 3; // reserve space for crc

                    // pack response
                    SDI12Value_t* pv;
                    pv = &ms_dValues[ms_currentDindex];
                    //Led1::on();

                    // iterate while we have values and enough time to convert
                    // or no space in buffer
                    for (unsigned int i = 0; (ms_currentDindex < ms_dCount)
                        && (i < OS_CFGINT_SDI12SENSOR_MAX_DCMD_VALUES); ++i, pv++)
                      {
                        //OSDeviceDebug::putDec(pv->precision);
                        //OSDeviceDebug::putNewLine();

                        //Led1::toggle();
#if defined(OS_CFGINT_SDI12SENSOR_MAX_DCMD_DURATION_MICROS)
                        unsigned int u;
                        u = OSScheduler::timerTicks.getTicks()
                            - ms_ticksExclamation;

                        // time almost out
                        if (u >= OSTimerTicks::microsToTicks(
                            OS_CFGINT_SDI12SENSOR_MAX_DCMD_DURATION_MICROS))
                          {
                            break;
                          }
#endif /* defined(OS_CFGINT_SDI12SENSOR_MAX_DCMD_DURATION_MICROS) */

                        if (!storeFloatResponse(pv->value, pv->precision, true))
                          {
                            break;
                          }
                        ms_currentDindex++;
                      }
                  }
              }
          }
        else
          {
            return false;
          }
        break;

      case 'R':
        if ((cmdDigit <= 9) && (cnt == 3))
          {
            // aR0! - a<values><CR><LF>
            // aR1! aR9! aCR0! aCR9!

            ms_cmdCRC = cmdC;

            ms_rCount = 0;
            prepareResponseContinuousMeasurement(cmdDigit);

            if (sizeof(ms_buf) > 75 + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA)
              ms_pBufMax = &ms_buf[75 + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA]; // limit to 75

            else
              ms_pBufMax = &ms_buf[sizeof(ms_buf)];

            ms_pBufMax -= 1; // adjust for last usable, not next
            ms_pBufMax -= 2; // reserve space for <cr><lf>
            if (ms_cmdCRC)
              ms_pBufMax -= 3; // reserve space for crc

            if (ms_rCount != 0)
              {
                // pack response
                SDI12Value_t* pv;
                pv = &ms_rValues[0];

                for (unsigned int i = 0; i < ms_rCount; ++i, pv++)
                  {
                    if (!storeFloatResponse(pv->value, pv->precision, true))
                      break;
                  }
              }
          }
        else
          {
            return false;
          }
        break;

      default:
        {
          // extended commands?
          if (sizeof(ms_buf) > 35 + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA)
            {
              ms_pBufMax = &ms_buf[35 + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA]; // limit to 35
            }
          else
            {
              ms_pBufMax = &ms_buf[sizeof(ms_buf)];
            }

          bool bRet;
          bRet = false;
          if ((cmd == 'X') && (cmdp1 == '$') && (cnt >= 4))
            {
              bRet = processSystemXCommand();
              //OSDeviceDebug::putString("[A");
              //OSDeviceDebug::putChar(bRet?'T':'F');
            }

          if (!bRet)
            {
              bRet = prepareResponseExtendedCommands((const char*) ms_pBuf,
                  ms_bufCount - 1);
              //OSDeviceDebug::putString("[B");
              //OSDeviceDebug::putChar(bRet?'T':'F');
            }

          // conservatively
          ms_moreProcessing = true;

          if (!bRet)
            {
              return false;
            }
        }
        break;
        }
    }

  if (ms_cmdCRC)
    {
      // if this command should have CRC, append it
      // beware that last 2 characters might not be printable
      unsigned short crc;
      crc = 0;

      unsigned char* p;
      for (p = &ms_buf[0]; p < ms_pBuf; ++p)
        crc = crc16(crc, *p);

      *ms_pBuf++ = (crc >> 12) | 0x40;
      *ms_pBuf++ = ((crc >> 6) & 0x3F) | 0x40;
      *ms_pBuf++ = (crc & 0x3F) | 0x40;
    }
  *ms_pBuf++ = '\r';
  *ms_pBuf++ = '\n';
  ms_bufCount = ms_pBuf - &ms_buf[0];

  return true;
}

bool
SDI12Sensor::processSystemXCommand(void)
{
  // system commands
  // aX$H : help
  // aX$TN : threads names
  // aX$TS : threads stacks
  // aX$TP : threads priorities
  // aX$Tn : thread n info
  // aX$U : uptime
  // aX$V : OS version
  // aX$C : counters (timeouts)

  unsigned char syscmd;
  syscmd = ms_buf[3];

  unsigned char cnt;
  cnt = ms_bufCount;

  parseInit((const char*) &ms_buf[4]);
  if (syscmd == 'H')
    {
      storeStringResponse("TN,TS,TP,Tn,U,V,R,B,C");
    }
  else if (syscmd == 'T')
    {
      int cThreads;
      cThreads = OSScheduler::getThreadsCount();

      OSThread* pt;
      syscmd = ms_buf[4];
      if (syscmd == 'N' || syscmd == 'S' || syscmd == 'P')
        {

          for (int j = 0; j < cThreads; ++j)
            {
              pt = OSScheduler::getThread(j);
              if (j > 0)
                storeCharResponse(',');

              if (syscmd == 'N')
                {
                  storeStringResponse(pt->getName());
                }
              else if (syscmd == 'S')
                {
                  storeUnsignedResponse(pt->getStackUsed());
                  storeCharResponse('/');
                  storeUnsignedResponse(pt->getStackSize());
                }
              else if (syscmd == 'P')
                {
                  storeUnsignedResponse((unsigned short) pt->getPriority());
                }
            }
        }
      else
        {
          unsigned char i;
          i = parseUnsigned();
          if (i < cThreads)
            {
              pt = OSScheduler::getThread(i);
              storeStringResponse(pt->getName());
              if (pt->isSuspended() || pt->isWaiting())
                {
                  storeCharResponse(',');
                  if (pt->isSuspended())
                    storeCharResponse('S');

                  if (pt->isWaiting())
                    storeCharResponse('W');

                  if (pt->isWaiting())
                    {
                      storeCharResponse(',');
                      storeUnsignedResponse((unsigned short) pt->getEvent());
                    }
                }
            }
        }
    }
#if defined(OS_INCLUDE_OSTIMERSECONDS_UPTIME)
  else if (syscmd == 'U')
    {
      unsigned long ul;
      ul = OSScheduler::timerSeconds.getUptime();

      unsigned char um, uh;
      unsigned short ud;
      ul /= 60; // minutes
      um = ul % 60;
      ul /= 60; // hours
      uh = ul % 24;
      ud = ul / 24; // days

      storeUnsignedResponse(ud);
      storeStringResponse("d,");

      storeUnsignedResponse((unsigned short) uh);
      storeStringResponse("h,");

      storeUnsignedResponse((unsigned short) um);
      storeStringResponse("m");

    }
#endif
  else if (syscmd == 'V')
    {
      // uOS++ version
      storeStringResponse(OS_CFGSTR_VERSION_MAJOR);
      storeCharResponse(',');
      storeStringResponse(OS_CFGSTR_VERSION_MINOR);
      storeCharResponse(',');
      storeStringResponse(OS_CFGSTR_VERSION_REVISION);
    }
  else if (syscmd == 'R')
    {
      // reset bits
      unsigned char b;
      b = OSCPU::getResetBits();
      storeUnsignedResponse((unsigned short) OSCPU::getResetBits());
      storeCharResponse(',');
      storeStringResponse(
          (b == 0) ? "soft" : ((b == 1) ? "pwr" : (((b == 2) ? "rst" : (((b
              == 4) ? "bout" : (((b == 8) ? "wd" : (((b == 16) ? "jtag" : ((b
              == 6) ? "pwr,bout" : "?")))))))))));

    }
  else if (syscmd == 'B')
    {
      if (cnt == 4)
        {
          storeCharResponse('0');
        }
      else if (cnt == 5)
        {
          // boot
          ms_doReset = parseNext();
        }
    }
  else if (syscmd == 'C')
    {
      storeUnsignedResponse((unsigned short) ms_timeoutCounter);
      storeCharResponse(',');
      storeUnsignedResponse((unsigned short) ms_failedMarkingCounter);
    }
  else
    return false;

  return true;
}

unsigned char*
SDI12Sensor::parseString(unsigned char* pStr, unsigned short len)
{
  unsigned char* p;
  p = pStr;

  len--;
  for (unsigned int i = 0; i < len; ++i)
    {
      unsigned char c;

      c = *ms_pParse;
      if (c == '\0' || c == ',')
        break;

      // Store only if printable or not space
      if (isgraph(c))
        *p++ = c;

      ms_pParse++;
    }
  *p = '\0';

  return pStr;
}

float
SDI12Sensor::parseFloat(void)
{
  float f;
  unsigned char c;

  bool bMinus;
  bMinus = false;
  c = *ms_pParse;
  if (c == '-')
    {
      bMinus = true;
      ms_pParse++;
    }
  f = 0.0f;
  while ((c = *ms_pParse) != '\0')
    {
      if ('0' <= c && c <= '9')
        {
          f *= 10.0f;
          f += (float) (c - '0');
        }
      else if (c == '.')
        {
          ms_pParse++;
          break;
        }
      else
        goto retminus;

      ms_pParse++;
    }

  float dm;
  dm = 0.1f;
  while ((c = *ms_pParse) != '\0')
    {
      if ('0' <= c && c <= '9')
        {
          f += (float) (c - '0') * dm;
          dm /= 10.0f;
        }
      else
        goto retminus;

      ms_pParse++;
    }
  retminus: return bMinus ? -f : f;
}

void
SDI12Sensor::setPeriodicSeconds(OSTimerTicks_t seconds)
{
  ms_periodicSeconds = seconds;
#if defined(DEBUG)
  OSDeviceDebug::putString_P(PSTR("setPeriodicSeconds="));
  OSDeviceDebug::putDec(ms_periodicSeconds);
  OSDeviceDebug::putNewLine();
#endif

  ms_periodicNextSecond = 0;
}

// ---------------------------------------------------------------------------

// upon request data acquisition thread
// should be scheduled *after* the main sdi12 thread

void
SDI12Sensor::staticMainAcquire(SDI12Sensor *pt)
{
  pt->threadMainAcquire();
  for (;;)
    ; // watchdog should trigger
}

void
SDI12Sensor::threadMainAcquire(void)
{

#if defined(DEBUG)
  OSSchedulerLock::enter();
    {
      OSDeviceDebug::putString_P(PSTR("threadMainAcquire()"));
      OSDeviceDebug::putNewLine();
    }
  OSSchedulerLock::exit();
#endif

  // wait for initializations to complete
  ms_flags.wait(INITIALIZED);

  OSThread* pThread;
  pThread = OSScheduler::getThreadCurrent();

  // thread endless loop
  for (;;)
    {
      ms_flags.clear(ACQUIRE);
      ms_flags.wait(ACQUIRE);

      clearDValues();

      OSTimerTicks_t ticksBegin;
      ticksBegin = OSScheduler::timerSeconds.getTicks();

#if defined(DEBUG)
      OSDeviceDebug::putString(" [");
      OSDeviceDebug::putChar(ms_measureCmd);
      OSDeviceDebug::putChar(ms_measureCmdDigit + '0');
      OSDeviceDebug::putChar(' ');
#endif

      pThread->virtualWatchdogSet(
          ms_dSeconds + OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS);

      prepareMeasuredData(ms_measureCmd, ms_measureCmdDigit, ms_dSeconds,
          ms_dCount);

      pThread->virtualWatchdogSet(0);

#if defined(DEBUG)
      OSDeviceDebug::putString("]");
      OSDeviceDebug::putNewLine();
#endif

#if defined(DEBUG)
      if (OSScheduler::timerSeconds.getTicks() - ticksBegin > ms_dSeconds)
        {
          OSDeviceDebug::putString("WARNING: ");
          OSDeviceDebug::putString(" duration exceeded");
          OSDeviceDebug::putNewLine();
        }
#endif

      ms_flags.notify(ACQUIRE_COMPLETED);

      // command complete, loop for next one
    }
}

void
SDI12Sensor::clearDValues(void)
{
  int i;
  for (i = 0; i < OS_CFGINT_SDI12SENSOR_DVALUES_SIZE; ++i)
    {
      // initialise at zero
      ms_dValues[i].value = 0.0f;
      ms_dValues[i].precision = 0;
    }
}

bool
SDI12Sensor::storeDValue(int iPos, float value, unsigned int precision)
{
  if (iPos >= ms_dCount)
    return false;

#if OS_CFGINT_SDI12SENSOR_DVALUES_SIZE > 0
  ms_dValues[iPos].value = value;
  ms_dValues[iPos].precision = precision;
#else
  value = value;
  precision = precision;
#endif

  return true;
}

bool
SDI12Sensor::storeRValue(int iPos, float value, unsigned int precision)
{
  if (iPos >= OS_CFGINT_SDI12SENSOR_RVALUES_SIZE)
    return false;

#if OS_CFGINT_SDI12SENSOR_RVALUES_SIZE > 0
  ms_rValues[iPos].value = value;
  ms_rValues[iPos].precision = precision;
#else
  value = value;
  precision = precision;
#endif

  if (iPos >= ms_rCount)
    ms_rCount = iPos + 1;

  //OSDeviceDebug::putDec(iPos);
  //OSDeviceDebug::putChar(' ');

  return true;
}

void
SDI12Sensor::storeStartMeasurementResponse(unsigned int seconds,
    unsigned char count)
{
  ms_dSeconds = seconds;
  ms_dCount = count;
}

bool
SDI12Sensor::storeUnsignedResponse(uint32_t l, unsigned short n)
{

  unsigned char* pSave;
  pSave = (unsigned char*) ms_pBuf;

  int i;

  for (i = sizeof(ms_tmp) - 1; i >= 0; i--)
    {
      ldiv_t d;
      d = ldiv(l, 10);
      ms_tmp[i] = d.rem + '0';
      l = d.quot;
    }

  if (0 < n && n <= (int) sizeof(ms_tmp))
    i = (int) sizeof(ms_tmp) - n;
  else
    {
      for (i = 0; i < (int) sizeof(ms_tmp) - 1; ++i)
        if (ms_tmp[i] != '0')
          break;
    }

  bool b;
  b = true;
  for (; b && i < (int) sizeof(ms_tmp); ++i)
    b = storeCharResponse((unsigned char) ms_tmp[i]);

  // if buffer exceeded, restore pointer
  if (!b)
    ms_pBuf = pSave;

  return b;
}

bool
SDI12Sensor::storeFloatResponse(float f, unsigned short prec, bool bPlus)
{
  //OSDebugLed1::off();

  unsigned char* pSave;
  pSave = (unsigned char*) ms_pBuf;

  bool minus;
  long l;
  if (f < 0.0f)
    {
      minus = true;
      f = -f;
    }
  else
    {
      minus = false;
    }

  l = (long) f;

  if (prec > 6)
    prec = 6;

  unsigned char nDecs;
  if (l < 10)
    nDecs = 6; // max 6 decimals

  else if (l < 100)
    nDecs = 5;
  else if (l < 1000)
    nDecs = 4;
  else if (l < 10000)
    nDecs = 3;
  else if (l < 100000)
    nDecs = 2;
  else if (l < 1000000)
    nDecs = 1;
  else if (l < 10000000)
    nDecs = 0;
  else
    {
      nDecs = 0;
      f = 10000000.0f; // max value exceeded
    }

  if (prec < nDecs)
    nDecs = prec; // reduce precision if requested

  if (nDecs != 0)
    {
      float fmul;
      fmul = 10.0f;
      if (nDecs == 2)
        fmul = 100.0f;
      else if (nDecs == 3)
        fmul = 1000.0f;
      else if (nDecs == 4)
        fmul = 10000.0f;
      else if (nDecs == 5)
        fmul = 100000.0f;
      else if (nDecs == 6)
        fmul = 1000000.0f;

      f *= fmul;
    }
  if (f != 10000000.0f)
    l = (long) (f + 0.5f);
  else
    l = 9999999l;

  //OSDebugLed1::on();
  unsigned char* p;
  unsigned int nDigits;
  for (p = &ms_tmp[sizeof(ms_tmp) - 1], nDigits = 0; p > &ms_tmp[0]; p--)
    {
      if (l == 0)
        {
          *p = '0';
        }
      else
        {
          ldiv_t ld;
          ld = ldiv(l, 10);
          *p = ld.rem + '0';
          l = ld.quot;
          //OSDebugLed1::toggle();
          nDigits++;
        }
      //OSDeviceDebug::putChar(*p);
    }
  //OSDebugLed1::on();

  if (nDigits < (unsigned int) (nDecs + 1))
    nDigits = nDecs + 1; // be sure we have at least a 0 for integral part

  // count trailing zeros in decimal part
  int nZero;
  for (nZero = 0; nZero < nDecs; ++nZero)
    if (ms_tmp[sizeof(ms_tmp) - 1 - nZero] != '0')
      break;

  // adjust total number of digits and decimals
  nDigits -= nZero;
  nDecs -= nZero;

  bool b;
  b = true;

  if (bPlus)
    {
      b = storeCharResponse(minus ? '-' : '+');
    }
  else
    {
      if (minus)
        b = storeCharResponse('-');
    }

  unsigned int i;
  for (p = &ms_tmp[sizeof(ms_tmp) - nZero - nDigits], i = 0; b && (i < nDigits); i++, p++)
    {
      b = storeCharResponse(*p);
      if ((i == (nDigits - nDecs - 1)) && (nDecs != 0))
        {
          b &= storeCharResponse('.');
        }
    }

  // if buffer exceeded, restore pointer
  if (!b)
    ms_pBuf = pSave;

  return b;
}

void
SDI12Sensor::storeDelayedMode(unsigned char mode)
{
  ms_delayedMode = mode;
}

// ---------------------------------------------------------------------------

// periodic data acquisition thread
// should be scheduled *after* the main sdi12 thread

void
SDI12Sensor::staticMainPeriodic(SDI12Sensor *pt)
{
  pt->threadMainPeriodic();
  for (;;)
    ; // watchdog should trigger
}

void
SDI12Sensor::threadMainPeriodic(void)
{
#if defined(DEBUG)
  OSSchedulerLock::enter();
    {
      OSDeviceDebug::putString("threadMainPeriodic()");
      OSDeviceDebug::putNewLine();
    }
  OSSchedulerLock::exit();
#endif

  // wait for initializations to complete
  ms_flags.wait(INITIALIZED);

  // wait for things to settle down and start with second 1
  OSScheduler::timerSeconds.sleep(1);

  OSThread* pThread;
  pThread = OSScheduler::getThreadCurrent();

  // thread endless loop
  for (;;)
    {

      OSTime_t crtSecond;

      crtSecond = OSScheduler::timerSeconds.getUptime();
      if (ms_periodicSeconds == 0)
        {
          ms_periodicNextSecond = crtSecond + 0x0000FFF0;
          //OSDeviceDebug::putString("*");
        }
      else
        {
          ms_periodicNextSecond = crtSecond + ms_periodicSeconds;

#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_THREADMAINPERIODIC)
          OSDeviceDebug::putString(" { ");
          OSDeviceDebug::putDec((unsigned short) crtSecond);
          OSDeviceDebug::putString("s ");
          //OSDeviceDebug::putNewLine();
#endif

          pThread->virtualWatchdogSet(
              ms_periodicSeconds + OS_CFGINT_SDI12SENSOR_VIRTUALWD_SECONDS);

          processPeriodicMeasurements();

          pThread->virtualWatchdogSet(0);

#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_THREADMAINPERIODIC)
          OSDeviceDebug::putString(" }");
          OSDeviceDebug::putNewLine();
#endif
        }

      for (;;)
        {
          crtSecond = OSScheduler::timerSeconds.getUptime();
          if (crtSecond >= ms_periodicNextSecond && !pThread->isInterrupted())
            break;

          if (!pThread->isInterrupted())
            {
              // sleep enough to the next periodic acquisition moment
              // to be able to react quickly to periodicSeconds changes,
              // a cancel event should be issued
              OSScheduler::timerSeconds.sleep(ms_periodicNextSecond - crtSecond);
            }

          if (pThread->isInterrupted())
            {
              pThread->ackInterruption();
              OSDeviceDebug::putString(" cancelled");
            }
        }
      //OSDeviceDebug::putChar('p');

      // command complete, loop for next one
    }
}

OSTime_t
SDI12Sensor::getPeriodicNextSecond(void)
{
  return ms_periodicNextSecond;
}

#endif

// ---------------------------------------------------------------------------

/*
 *   Optimized CRC-16 calculation.
 *
 *   Polynomial: x^16 + x^15 + x^2 + 1 (0xa001)
 *   Initial value: 0xffff (the SDI12 manual says to initialise as zero)
 *
 *   This CRC is normally used in disk-drive controllers.
 */

unsigned short
SDI12Sensor::crc16(unsigned short crc, unsigned char ch)
{
  crc ^= ch;

  int i;
  for (i = 0; i < 8; ++i)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ 0xA001;
      else
        crc = (crc >> 1);
    }

  return crc;
}

#if defined(OS_INCLUDE_SDI12SENSOR)
void
SDI12Sensor::transmitResponse(void)
{
  OSTimerTicks_t tick;
  tick = OSScheduler::timerTicks.getTicks();

  rs485Transmit();

  ms_outIndex = 0;

  ms_flags.clear(TX_COMPLETED);

  usartTxEnable();

  // wait for transmit complete (shift register)
  //os.sched.eventWait(OS_EVENT_TX_COMPLETE);
  ms_flags.wait(TX_COMPLETED);

#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_TIMINGS)
  OSDeviceDebug::putChar(' ');
  OSDeviceDebug::putDec(tick-ms_exclamationTicks);
  OSDeviceDebug::putChar(':');
  OSDeviceDebug::putDec(os.sched.timerTicks.getTicks()-ms_exclamationTicks);
  OSDeviceDebug::putChar('t');
  OSDeviceDebug::putChar(' ');
#endif

  OSScheduler::timerTicks.sleep(OSTimerTicks::microsToTicks(7500));

  rs485Receive();
}

void
SDI12Sensor::interruptPinChangeServiceRoutine(unsigned char crt,
    unsigned char prev)
{
  OSCpuSleepCriticalSection& cpuSleepCritical =
      ms_pThread->getCpuSleepCriticalSection();

  if (!isBreakDetectEnabled())
    return;

  if (pinChanged(crt, prev))
    {
      if (pinChangedIsHigh(crt))
        {
          // break off

          OSTimerTicks_t now;
          now = OSScheduler::timerTicks.getTicks();

          OSDebugLed2::off();
#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTPINCHANGESERVICEROUTINE)
          OSDeviceDebug::putDec(now - ms_ticksBRK);
          OSDeviceDebug::putChar('/');
#endif

          if ((now - ms_ticksBRK) >= MIN_BREAK_TICKS)
            {
              // break recognized only if pulse longer
              // than MIN_BREAK_TICKS

              //#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTPINCHANGESERVICEROUTINE)
              OSDeviceDebug::putChar('#');
              //#endif

              ms_flags.notify(BREAK);

              disableBreakDetect();
            }
          else
            {
#if defined(DEBUG)
              OSDeviceDebug::putChar('?');
#endif
            }
#if defined(OS_INCLUDE_OSCPUSLEEPCRITICALSECTION)
          cpuSleepCritical.exit();
#else
          ms_pThread->setAllowSleep(true);
#endif
        }
      else
        {
          // break on
#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTPINCHANGESERVICEROUTINE)
          OSDeviceDebug::putChar('\\');
#endif

          // must be here, otherwise we cannot measure pulse duration
#if defined(OS_INCLUDE_OSCPUSLEEPCRITICALSECTION)
          cpuSleepCritical.enter();
#else
          ms_pThread->setAllowSleep(false);
#endif
          OSDebugLed2::on();
        }
      ms_ticksBRK = OSScheduler::timerTicks.getTicks();
    }
}

void
SDI12Sensor::interruptTxServiceRoutine(void)
{
  if (ms_outIndex < ms_bufCount)
    {
      unsigned char ch;
      ch = ms_buf[ms_outIndex++];

#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTTXSERVICEROUTINE)
      // show transmitted char on debug
      OSDeviceDebug::putChar(ch);
      //OSDeviceDebug::putChar('_');
#endif
      usartWriteByte(ch);
    }
  else
    {
      usartTxDisable();
      interruptTxCompleteEnable();
    }
}

void
SDI12Sensor::interruptTxCompleteServiceRoutine(void)
{
  //OSDeviceDebug::putChar(']');

  interruptTxCompleteDisable();

  ms_flags.notify(TX_COMPLETED);
}

void
SDI12Sensor::interruptRxServiceRoutine(void)
{
  bool isFramingError;
  bool isParityError;

  // should be set before reading the byte
  // otherwise the conditions are lost
  isFramingError = usartIsFramingError();
  isParityError = usartIsParityError();

  // read byte (and clear conditions)
  unsigned char ch;
  ch = usartReadByte();

  if (isFramingError)
    {
#if defined(DEBUG)
      OSDeviceDebug::putChar(' ');
      OSDeviceDebug::putHex(ch);
      OSDeviceDebug::putChar('f');
#endif
      ms_flags.notify(RX_ERROR);
    }
  else if (isParityError)
    {
#if defined(DEBUG)
      OSDeviceDebug::putChar(' ');
      OSDeviceDebug::putHex(ch);
      OSDeviceDebug::putChar('p');
#endif
      ms_flags.notify(RX_ERROR);
    }
  else
    {
      OSTimerTicks_t now;
      now = OSScheduler::timerTicks.getTicks();

      //enableTimeout(10);

      // remember the moment when this interrupt was received
      ms_ticksRX = now;
      if (ch == '!')
        {
          // remember the moment when the '!' was received
          ms_ticksExclamation = now;

          disableMarking();

          // ignore trailing spaces
          while (ms_buf[ms_bufCount - 1] == ' ')
            {
              --ms_bufCount;
            }

          // notify thread that command is in, do not store it
          ms_flags.notify(EXCLAMATION);
        }
      else if (ms_bufCount < sizeof(ms_buf))
        {
          // if enough space, store char in receive buffer
          ms_buf[ms_bufCount++] = ch;

          // if first char in buffer, notify address
          if (ms_bufCount == 1)
            ms_flags.notify(ADDRESS);
        }
      else
        {
          // ignore character and notify error
          ms_flags.notify(RX_ERROR);
        }

#if defined(DEBUG) && defined(OS_DEBUG_SDI12SENSOR_INTERRUPTRXSERVICEROUTINE)
      // show received char on debug
      OSDeviceDebug::putChar(ch);
#endif
    }
}

#endif

void
SDI12Sensor::setAddress(unsigned char addr)
{
  ms_ownAddress = addr;

#if defined(DEBUG)
  OSDeviceDebug::putString("SDI12Sensor::setAddress(");
  OSDeviceDebug::putChar(addr);
  OSDeviceDebug::putString(")");
  OSDeviceDebug::putNewLine();
#endif
}

bool
SDI12Sensor::isAddressValid(unsigned char addr)
{
  return (addr == ms_ownAddress || addr == '?');
}

void
SDI12Sensor::clearInputBuff(void)
{
  ms_bufCount = 0;
  memset(ms_buf, '\0', sizeof(ms_buf));
}

// return false if buffer overflow
bool
SDI12Sensor::storeCharResponse(unsigned char ch)
{
  if (ms_pBuf >= ms_pBufMax)
    {
      OSDeviceDebug::putString("ovf ");
      return false;
    }

  //OSDeviceDebug::putChar(ch);
  //OSDeviceDebug::putHex(ch);
  //OSDeviceDebug::putChar(' ');

  *ms_pBuf++ = ch;
  return true;
}

bool
SDI12Sensor::storeStringResponse(const char* pStr, unsigned short n)
{
  unsigned char* pSave;
  pSave = (unsigned char*) ms_pBuf;

  bool b;
  b = true;
  for (unsigned int i = 0; b && (n != 0) ? (i < n) : (*pStr != '\0'); pStr++, ++i)
    b = storeCharResponse((unsigned char) *pStr);

  // if buffer exceeded, restore pointer
  if (!b)
    ms_pBuf = pSave;

  return b;
}

#if defined(OS_INCLUDE_SDI12SENSOR)
bool
SDI12Sensor::storeUnsignedResponse(uint16_t w, unsigned short n)
{
  unsigned char* pSave;
  pSave = (unsigned char*) ms_pBuf;

  int i;

  for (i = sizeof(ms_tmp) - 1; i >= 0; i--)
    {
      div_t d;
      d = div(w, 10);
      ms_tmp[i] = d.rem + '0';
      w = d.quot;
    }

  if (0 < n && n <= (int) sizeof(ms_tmp))
    i = (int) sizeof(ms_tmp) - n;
  else
    {
      for (i = 0; i < (int) sizeof(ms_tmp) - 1; ++i)
        if (ms_tmp[i] != '0')
          break;
    }

  bool b;
  b = true;
  for (; b && i < (int) sizeof(ms_tmp); ++i)
    b = storeCharResponse((unsigned char) ms_tmp[i]);

  // if buffer exceeded, restore pointer
  if (!b)
    ms_pBuf = pSave;

  return b;
}
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
bool SDI12Sensor::storeHexNibbleResponse(unsigned char b)
  {
    b &= 0xF;
    //OSDeviceDebug::putHex(b);
    return storeCharResponse((b <= 9) ? ('0' + b) : 'A' + b - 10);
  }

bool SDI12Sensor::storeHexResponse(unsigned char b)
  {
    storeHexNibbleResponse(b >> 4);
    return storeHexNibbleResponse(b);
  }
bool SDI12Sensor::storeHexResponse(unsigned short w)
  {
    storeHexResponse((unsigned char)(w >> 8));
    return storeHexResponse((unsigned char)w);
  }
#endif

void
SDI12Sensor::parseInit(const char* p)
{
  ms_pParse = (unsigned char*) p;
#if true
  ;
#else
  unsigned char c;
  for (;;)
    {
      c = *ms_pParse;
      if (c == '\0')
      break;

      if (c != ' ' && c != '\t')
      break;

      ms_pParse++;
    }
#endif
}

unsigned char
SDI12Sensor::parseNext()
{
  unsigned char c;
#if true
  c = *ms_pParse;
  if (c != '\0')
    ms_pParse++;
#else
  do
    {
      c = *ms_pParse;
      if (c == '\0')
      break;

      ms_pParse++;
    }while (c == ' ' || c == '\t');
#endif
  return c;
}

unsigned char
SDI12Sensor::parsePeek()
{
  return *ms_pParse;
}

#if defined(OS_INCLUDE_SDI12SENSOR)

bool
SDI12Sensor::parseIsValid()
{
  unsigned char c;
  c = *ms_pParse;
  if (c == '\0' || c == ',')
    return false;

  return true;
}

#endif

bool
SDI12Sensor::parseIsEnd()
{
  unsigned char c;
  c = *ms_pParse;
  if (c == '\0')
    return true;

  return false;
}

#if defined(OS_INCLUDE_SDI12SENSOR)
unsigned int
SDI12Sensor::parseUnsigned(void)
{
  unsigned short w;
  unsigned char c;

  w = 0;
  while ((c = *ms_pParse) != '\0')
    {
      if ('0' <= c && c <= '9')
        {
          w *= 10;
          w += (c - '0');
        }
      else
        return w;

      ms_pParse++;
    }
  return w;
}
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
unsigned short SDI12Sensor::parseHex(void)
  {
    unsigned short w;
    unsigned char c;

    w = 0;
    while ((c = *ms_pParse) != '\0')
      {
        c = toupper(c);
        if ('0' <= c && c <= '9')
          {
            w <<= 4;
            w |= (unsigned char)(c - '0');
          }
        else if ('A' <= c && c <= 'F')
          {
            w <<= 4;
            w |= (unsigned char)(c - 'A' + 10);
          }
        else
        return w;

        ms_pParse++;
      }
    return w;
  }
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)

// minimal implementation of SDI-12 sensor protocol

bool SDI12Sensor::readCommand(void)
  {
    OSScheduler::ledActiveOff();

    while (!pinChangedIsHigh())
      {
        ;
      }

    unsigned int i;

    do
      {
        // wait for falling rx
        while (pinChangedIsHigh())
          {
            OS::WDTreset(); // the only WD reset
          }

        OSDebugLed2::on();

        //OSDeviceDebug::putChar('\\');
        if (false)
          {
            i = 0;
            while (!pinChangedIsHigh())
              {
                OS::busyWaitMillis(1);
                i++;
              }
          }
        else
          {
            i = timerRead();

            //OSDeviceDebug::putDec(timerRead());
            //OSDeviceDebug::putChar(' ');

            // wait for raising rx
            while (!pinChangedIsHigh())
              {
                ;
              }
            i = (unsigned char)(timerRead() - i);
          }
        OSDebugLed2::off();

        //OSDeviceDebug::putDec(timerRead());
        //OSDeviceDebug::putChar(' ');

        //OSDeviceDebug::putDec(i);
        //OSDeviceDebug::putChar('/');

      }while (i < 8);

    OSDeviceDebug::putChar('#');

    OSScheduler::ledActiveOn();

    clearInputBuff();
    usartRxEnable();

    bool bInCRC;
    bInCRC = false;

    for (;;)
      {
        unsigned char t0;
        t0 = timerRead();
        while (!usartReadAvailable())
          {
            if ((unsigned char)(timerRead()-t0)> 100)
            goto return_false; // timeout
          }
        if (usartIsFramingError() || usartIsParityError())
        goto return_false;

        unsigned char ch;
        ch = usartReadByte();

        if (ms_bufCount == 0)
          {
            if (!isAddressValid(ch))
              {
                OSDeviceDebug::putString(" !=");
                OSDeviceDebug::putHex(ch);
                goto return_false;
              }
            ms_crc = 0;
            bInCRC = true;
          }

        if (ch == '*')
          {
            // stop crc computation
            bInCRC = false;
          }
        else if (ch == '!')
          {
            ms_exclamationTicks = timerRead();
            OSDeviceDebug::putChar(ch);
            break;
          }

        if (ms_bufCount < sizeof(ms_buf))
          {
            // if enough space, store char in receive buffer
            ms_buf[ms_bufCount++] = ch;
            if (bInCRC)
            ms_crc = SDI12Sensor::crc16(ms_crc, ch);
          }
        else
          {
            OSDeviceDebug::putString("nnn ");
            goto return_false;
          }
        OSDeviceDebug::putChar(ch);
      }

    OSDebugLed1::on();

    // ignore trailing spaces
    while (ms_buf[ms_bufCount-1] == ' ')
    --ms_bufCount;

    ms_buf[ms_bufCount] = '\0';

    // disable receiver
    usartRxDisable();

    ms_pBuf = &ms_buf[1];
    ms_pBufMax = &ms_buf[sizeof(ms_buf)-5];

    parseInit((const char*)&ms_buf[1]);

    return true;

    return_false: //
    usartRxDisable();
    return false;

  }

bool SDI12Sensor::writeResponse(void)
  {
    if (bUseCRC)
      {
        // if this command should have CRC, append it
        // beware that last 2 characters might not be printable
        unsigned short crc;
        crc = 0;

        unsigned char* p;
        for (p = &ms_buf[0]; p < ms_pBuf; ++p)
        crc = crc16(crc, *p);

        *ms_pBuf++ = (crc >> 12) | 0x40;
        *ms_pBuf++ = ((crc >> 6) & 0x3F) | 0x40;
        *ms_pBuf++ = (crc & 0x3F) | 0x40;
      }
    *ms_pBuf++ = '\r';
    *ms_pBuf++ = '\n';
    ms_bufCount = ms_pBuf - &ms_buf[0];

    //OSDeviceDebug::putDec(ms_bufCount);
    //OSDeviceDebug::putChar(' ');

    if (false)
      {
        OS::busyWaitMillis(10);
      }
    else
      {
        if ((unsigned char)(timerRead() - ms_exclamationTicks)> 15)
          {
            // if more than 15 millis, do not send response
            OSDebugLed1::off();
            return false;
          }

        //OSDeviceDebug::putDec(timerRead());
        //OSDeviceDebug::putChar(' ');

        // leave more than 8 millis before sending response
        while ((unsigned char)(timerRead() - ms_exclamationTicks) < 12)
          {
            ;
          }
      }

    //OSDeviceDebug::putDec(timerRead());
    //OSDeviceDebug::putChar(' ');

    rs485Transmit();

    usartTxEnable();

    OSDebugLed1::off();
    for (unsigned int i = 0; i < ms_bufCount; ++i)
      {
        while (!usartWriteAvailable())
        ;

        unsigned char ch;
        ch = ms_buf[i];
        usartWriteByte(ch);
        OSDeviceDebug::putChar(ch);
      }

    while (!usartWriteComplete())
    ;

    // flush input buffer
    while (usartReadAvailable())
    usartReadByte();

    usartTxDisable();

    rs485Receive();

    return true;
  }

#endif

// ---------------------------------------------------------------------------

#endif
