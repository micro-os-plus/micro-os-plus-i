/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include "DataAcquisition.h"
#include "Storage.h"

// global variables defined in main.cpp
extern Storage storage;
extern unsigned long ticks;

/*
 * Active object constructor.
 * Initialise parent system thread and store parameters in private members.
 */

DataAcquisition::DataAcquisition(int maxCount) :
  OSThread("DAQ", m_stack, sizeof(m_stack))
{
  debug.putConstructor_P(PSTR("DataAcquisition"), this);

  m_maxCount = maxCount;
}

/*
 * Thread main code.
 */

void
DataAcquisition::threadMain(void)
{
  if (os.isDebug())
    {
      os.sched.lock.enter();
        {
          debug.putString("Thread '");
          debug.putString(getName());
          debug.putString("', Loops=");
          debug.putDec((unsigned long) m_maxCount);
          debug.putNewLine();
        }
      os.sched.lock.exit();
    }

  // Thread endless loop
  for (;;)
    {
      for (int len = 1; len <= 4; len++)
        {
          debug.putString("Sample length=");
          debug.putDec((unsigned long) len);
          debug.putNewLine();

          unsigned long startTicks;
          os.sched.critical.enter();
            {
              startTicks = ticks;
            }
          os.sched.critical.exit();
          for (int count = 0; count < m_maxCount; count++)
            {
              for (unsigned int ix = 0; ix < sizeof(m_buffer)
                  / sizeof(AcquisitionSample_t); ix++)
                {
                  unsigned char* pChar;
                  pChar = (unsigned char*) &m_buffer[ix];

                  AcquisitionSample_t sample;
                  sample = 0;

                  if (len == 4)
                    {
                      sample |= pChar[BYTE3_INDEX];
                      sample <<= 8;
                    }
                  if (len >= 3)
                    {
                      sample |= pChar[BYTE2_INDEX];
                      sample <<= 8;
                    }
                  if (len >= 2)
                    {
                      sample |= pChar[BYTE1_INDEX];
                      sample <<= 8;
                    }
                  sample |= pChar[BYTE0_INDEX];

                  storage.storeSample(sample);
                }
            }
          unsigned long stopTicks;
          os.sched.critical.enter();
            {
              stopTicks = ticks;
            }
          os.sched.critical.exit();

          unsigned int nSamples;
          nSamples = m_maxCount * sizeof(m_buffer)
              / sizeof(AcquisitionSample_t);

          unsigned long nDeltaTicks;
          nDeltaTicks = stopTicks - startTicks;

          debug.putString("Samples=");
          debug.putDec((unsigned long) nSamples);
          debug.putString(", Ticks=");
          debug.putDec(nDeltaTicks);
          debug.putString(", ");
          debug.putDec(nSamples*OS_CFGINT_TICK_RATE_HZ/nDeltaTicks);
          debug.putString(" Samples/sec");
          debug.putNewLine();
        }
    }
}

