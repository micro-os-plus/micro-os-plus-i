/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef SDI12SENSOR_H_
#define SDI12SENSOR_H_

#include "portable/kernel/include/OS.h"

#include "portable/kernel/include/OSEventFlags.h"

#if defined(OS_INCLUDE_SDI12SENSOR)

#if !defined(OS_CFGINT_SDI12SENSOR_BUFFER_SIZE)
#define OS_CFGINT_SDI12SENSOR_BUFFER_SIZE                       (75)
#endif

#if !defined(OS_CFGBOOL_SDI12SENSOR_SUPPORT_WILD_CARD_ADDRESS)
#define OS_CFGBOOL_SDI12SENSOR_SUPPORT_WILD_CARD_ADDRESS        (true)
#endif

#if !defined(OS_CFGINT_SDI12SENSOR_DVALUES_SIZE)
#define OS_CFGINT_SDI12SENSOR_DVALUES_SIZE                      (9)
#endif

#if !defined(OS_CFGINT_SDI12SENSOR_MAX_DCMD_VALUES)
#define OS_CFGINT_SDI12SENSOR_MAX_DCMD_VALUES                   OS_CFGINT_SDI12SENSOR_DVALUES_SIZE
#endif

#if !defined(OS_CFGINT_SDI12SENSOR_RVALUES_SIZE)
#define OS_CFGINT_SDI12SENSOR_RVALUES_SIZE                      (7)
#endif

typedef struct SDI12Value
{
  float value;
  unsigned char precision; // number of digits after period
} SDI12Value_t;

#endif

#define OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA                      (6)

#define OS_CFGINT_SDI12SENSOR_TMP_SIZE                          (10)

class SDI12Sensor
{
public:

#if defined(OS_INCLUDE_SDI12SENSOR)

  SDI12Sensor(const char* pNameAcquire, const unsigned char* pStackAcquire,
      unsigned short stackSizeAcquire, OSThreadPriority_t priorityAcquire,
      const char* pNamePeriodic, const unsigned char* pStackPeriodic,
      unsigned short stackSizePeriodic, OSThreadPriority_t priorityPeriodic,
      const char* pNameSDI12, const unsigned char* pStackSDI12,
      unsigned short stackSizeSDI12, OSThreadPriority_t prioritySDI12);

  virtual
  ~SDI12Sensor();

  virtual void
  customInit(void) = 0;

  virtual void
  prepareResponseSendIdentification(void) = 0;

  virtual void
  prepareResponseContinuousMeasurement(unsigned char digit) = 0;
  static bool
  storeRValue(int iPos, float value, unsigned int precision);

  virtual void
  prepareResponseStartMeasurement(unsigned char cmd, unsigned char digit) = 0;
  static void
  storeStartMeasurementResponse(unsigned int seconds, unsigned char count);
  static void
  storeStartMeasurementDelay(unsigned short delay);

  virtual bool
  prepareResponseExtendedCommands(const char* pBuf, unsigned int count) = 0;

  static bool
  storeUnsignedResponse(unsigned long l, unsigned short n = 0);
  static bool
  storeFloatResponse(float f, unsigned short precision = 6, bool bPlus = false);

  static unsigned char*
  parseString(unsigned char* pStr, unsigned short len);
  static float
  parseFloat(void);

  virtual void
  storeAddressToNonVolatileMemory(unsigned char newAddress) = 0;

  virtual void
  prepareMeasuredData(unsigned char cmd, unsigned char digit,
      unsigned int seconds, unsigned char count) = 0;
  static bool
  storeDValue(int iPos, float value, unsigned int precision);

  virtual void
  processDelayedActions(void) = 0;
  static void
  storeDelayedMode(unsigned char mode);

  virtual void
  processPeriodicMeasurements(void);

  virtual void
  processSoftReset(unsigned char param) = 0;

  static void
  interruptPinChangeServiceRoutine(unsigned char crt, unsigned char prev);
  static void
  interruptRxServiceRoutine(void);
  static void
  interruptTxServiceRoutine(void);
  static void
  interruptTxCompleteServiceRoutine(void);

#endif

  static OSTimerTicks_t
  getPeriodicSeconds(void);
  static void
  setPeriodicSeconds(OSTimerTicks_t seconds);

  static const unsigned char DEFAULT_ADDRESS = '0';

  static unsigned char
  getAddress(void);
  static void
  setAddress(unsigned char addr);

  static bool
  storeCharResponse(unsigned char ch);
  static bool
  storeStringResponse(const char* pStr, unsigned short n = 0);
#if defined(OS_INCLUDE_SDI12SENSOR)
  static bool
  storeUnsignedResponse(uint16_t w, unsigned short n = 0);
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
  static bool storeHexNibbleResponse(unsigned char b);
  static bool storeHexResponse(unsigned char b);
  static bool storeHexResponse(unsigned short w);

#endif

  static unsigned short
  getOutputBufferSize(void);

  static void
  parseInit(const char* p);
  static unsigned char
  parsePeek(void); // return '\0' at end
  static unsigned char
  parseNext(void); // return '\0' at end
  static bool
  parseIsValid(void);
  static bool
  parseIsEnd(void);

  static unsigned short
  crc16(unsigned short crc, unsigned char ch);

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
  static unsigned short parseHex(void);
#endif

#if defined(OS_INCLUDE_SDI12SENSOR)

  static unsigned int
  parseUnsigned(void);
  static OSTime_t
  getPeriodicNextSecond(void);

  void
  cancelPeriodic(void);
  void
  cancelAcquisition(void);
  static bool
  isAcquisitionCancelled(void);

  static const unsigned char DELAYED_MODE_NONE = 0;
  static const unsigned char DELAYED_MODE_PROTO = 1;
  static const unsigned char DELAYED_MODE_DACQ = 2;

  static void
  interruptTick(void);
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
  static void init(void);
  static bool readCommand(void);
  static bool writeResponse(void);
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
  static void timerInit(void);
  static unsigned char timerRead(void);
#endif

protected:
#if defined(OS_INCLUDE_SDI12SENSOR)
  virtual void
  threadMainAcquire(void);
  virtual void
  threadMainPeriodic(void);
#endif

private:
  static bool
  isAddressValid(unsigned char addr);

  static void
  rs485Init(void);
  static void
  rs485Receive(void);
  static void
  rs485Transmit(void);

  static void
  usartRxEnable(void);
  static void
  usartRxDisable(void);
  static void
  usartTxEnable(void);
  static void
  usartTxDisable(void);

  static bool
  usartWriteAvailable(void);
  static void
  usartWriteByte(unsigned char ch);
  static bool
  usartWriteComplete(void);
  static bool
  usartReadAvailable(void);
  static unsigned char
  usartReadByte(void);
  static bool
  usartIsFramingError(void);
  static bool
  usartIsParityError(void);

  static void
  pinInit(void);
  static bool
  pinChangedIsHigh(void);

  static void
  clearInputBuff(void);

#if defined(OS_INCLUDE_SDI12SENSOR)

  static void
  init(void);

  static bool
  processSystemXCommand(void);

  static void
  clearDValues(void);

  static void
  enableTimeout(OSTimerTicks_t ticks);
  static void
  disableTimeout(void);

  static void
  enableMarking(OSTimerTicks_t ticks);
  static void
  disableMarking(void);

  static void
  enableBreakDetect(void);

  static void
  disableBreakDetect(void);

  static bool
  isBreakDetectEnabled(void);

  static void
  interruptPinChangeEnable(void);
  static void
  interruptPinChangeDisable(void);

  static bool
  pinChanged(unsigned char crt, unsigned char prev);
  static bool
  pinChangedIsHigh(unsigned char crt);

  static void
  interruptTxCompleteEnable(void);
  static void
  interruptTxCompleteDisable(void);

  static void
  transmitResponse(void);

  static void
  staticMainSDI12(SDI12Sensor* pt);
  static void
  staticMainAcquire(SDI12Sensor* pt);
  static void
  staticMainPeriodic(SDI12Sensor* pt);

  void
  threadMainSDI12(void);
  bool
  processCommand(void);

  static unsigned char*
  putFloat(unsigned char* pBuf, unsigned char* pMax, float f,
      unsigned char prec = 6);

  // --------------------------------------------
  // constants

  // HW flags
  static const OSEventFlagsBits_t BREAK = 0x0001;
  static const OSEventFlagsBits_t EXCLAMATION = 0x0002;
  static const OSEventFlagsBits_t RX_ERROR = 0x0004;
  static const OSEventFlagsBits_t TX_COMPLETED = 0x0008;
  static const OSEventFlagsBits_t ADDRESS = 0x0010;
  static const OSEventFlagsBits_t MARKING = 0x0020;
  static const OSEventFlagsBits_t TIMEOUT = 0x0040;
  //static const OSEventFlagsBits_t TIMEOUT = 0x0080;

  // proto flags
  static const OSEventFlagsBits_t INITIALIZED = 0x0100;
  static const OSEventFlagsBits_t ACQUIRE = 0x0200;
  static const OSEventFlagsBits_t ACQUIRE_COMPLETED = 0x0400;
  //static const OSEventFlagsBits_t SERVICE_REQUEST = 0x0800;

  // proto states
  static const unsigned char STATE0 = 0;
  static const unsigned char STATE1 = 1;
  static const unsigned char STATE2 = 2;
  static const unsigned char STATE3 = 3;
  static const unsigned char STATE4 = 4;
  static const unsigned char STATE5 = 5;
  static const unsigned char STATE6 = 6;
  static const unsigned char STATE7 = 7;
  static const unsigned char STATE8 = 8;
  static const unsigned char STATE9 = 9;

  // other constants
  static const OSTimerTicks_t MIN_BREAK_TICKS = 7;
  static const OSTimerTicks_t TIMEOUT100_TICKS = 100;
#if !defined(DEBUG)
  static const OSTimerTicks_t MARKING8_TICKS = 8;
#else
  static const OSTimerTicks_t MARKING8_TICKS = 5;
#endif

  // --------------------------------------------
  // members

  OSThread threadSDI12;
  OSThread threadAcquire;
  OSThread threadPeriodic;

  static OSEventFlags ms_flags;

  static volatile bool ms_useCRC;
  static volatile bool ms_cmdCRC;

#endif

  // own address
  static volatile unsigned char ms_ownAddress;

public:
  // common input & output buffer
  static unsigned char ms_buf[OS_CFGINT_SDI12SENSOR_BUFFER_SIZE
      + OS_CFGINT_SDI12SENSOR_BUFFER_EXTRA];

  // count of chars in ms_buf
  static volatile unsigned short ms_bufCount;
  static unsigned char* ms_pParse;

#if !defined(OS_EXCLUDE_PCINT3_VECT)
  unsigned char ms_prevValue3;
#endif

#if defined(OS_INCLUDE_SDI12SENSOR_TINY)
  static unsigned short ms_crc;
  static unsigned char ms_exclamationTicks;
  static bool bUseCRC;
#endif

private:
  static volatile unsigned char* ms_pBuf;
  static volatile unsigned char* ms_pBufMax;

  static unsigned char ms_tmp[OS_CFGINT_SDI12SENSOR_TMP_SIZE];

#if defined(OS_INCLUDE_SDI12SENSOR)

  // index in output buffer, used by tx routines
  static volatile unsigned char ms_outIndex;

  // current tick set by the ISR
  static volatile OSTimerTicks_t ms_ticksBRK;

  static volatile OSTimerTicks_t ms_ticksRX;

  static unsigned char ms_delayedMode;

  static unsigned char ms_doReset;

  static OSThread* ms_pThread;
  static OSThread* ms_pThreadA;

  static volatile OSTimerTicks_t ms_exclamationTicks;

  static unsigned char ms_state;
  static bool ms_moreProcessing;
  static volatile OSTimerTicks_t ms_timerTimeout;
  static volatile OSTimerTicks_t ms_timerMarking;
  static volatile OSTimerTicks_t ms_cntMarking;

  //static volatile bool ms_cancelOnBreak;
  static volatile bool ms_breakDetectEnable;

  // --------------------------------------------
  // acquisition thread variables

  static SDI12Value_t ms_dValues[OS_CFGINT_SDI12SENSOR_DVALUES_SIZE];
  static unsigned char ms_dCount;

  static SDI12Value_t ms_rValues[OS_CFGINT_SDI12SENSOR_RVALUES_SIZE];
  static unsigned char ms_rCount;

  static volatile bool ms_bAcquire;

  static volatile unsigned char ms_dCmd;
  static volatile unsigned char ms_dDigit;
  static unsigned int ms_dSeconds;
  static volatile unsigned char ms_dIndex;

  //static volatile bool ms_bIsCancelled;
  static volatile bool ms_bDAvailable;
  static volatile bool ms_bServiceRequest;

  // --------------------------------------------
  // periodic thread variables

  static volatile OSTimerTicks_t ms_periodicSeconds;
  static OSTime_t ms_periodicNextSecond;

#endif

};

inline unsigned short
SDI12Sensor::getOutputBufferSize(void)
{
  return OS_CFGINT_SDI12SENSOR_BUFFER_SIZE;
}

inline void
SDI12Sensor::disableBreakDetect(void)
{
 ms_breakDetectEnable = false;
}

inline bool
SDI12Sensor::isBreakDetectEnabled(void)
{
  return ms_breakDetectEnable;
}

#if defined(OS_INCLUDE_SDI12SENSOR)

inline bool
SDI12Sensor::isAcquisitionCancelled(void)
{
  return OSScheduler::getThreadCurrent()->isInterrupted();
}

inline OSTimerTicks_t
SDI12Sensor::getPeriodicSeconds(void)
{
  return ms_periodicSeconds;
}

inline unsigned char
SDI12Sensor::getAddress(void)
{
  return ms_ownAddress;
}

#endif

#if defined(OS_CONFIG_BOARD_METRILOG_M512) || defined(OS_CONFIG_BOARD_METRILOG_M514)
#include "hal/boards/Metrilog/m512/include/SDI12Sensor_Inlines.h"
#else
#error "Missing OS_CONFIG_BOARD_* definition"
#endif

#endif /* SDI12SENSOR_H_ */
