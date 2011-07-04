/*
 * Storage.cpp
 *
 *  Created on: 24 Mar 2011
 *      Author: Liviu Ionescu
 */

#include "Storage.h"

Storage::Storage()
{
  m_index = HEADER_LENGTH;
  m_sampleLenght = 4;
}

int
Storage::storeSample(AcquisitionSample_t sample)
{
  if (m_index + m_sampleLenght > sizeof(m_buffer))
    {
      // simulate writing to storage
      //os.sched.timerTicks.sleep(4);

      // reinitialise buffer
      m_index = HEADER_LENGTH;
    }

  unsigned char c;
  int i;

  // transfer sample to storage, little endian format
  for (i = m_sampleLenght; i > 0; --i)
    {
      c = (sample & 0xFF);
      sample >>= 8;
      m_buffer[m_index++] = c;
    }

  return 0;
}
