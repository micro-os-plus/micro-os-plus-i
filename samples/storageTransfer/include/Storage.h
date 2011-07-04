/*
 * Storage.h
 *
 *  Created on: 24 Mar 2011
 *      Author: Liviu Ionescu
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "portable/kernel/include/uOS.h"

typedef unsigned long AcquisitionSample_t;

class Storage
{
public:
  Storage();

  int
  storeSample(AcquisitionSample_t sample);

  void
  setSampleLength(int len);

private:
  unsigned char m_buffer[4*1024];
  unsigned int m_index;
  unsigned int m_sampleLenght;

  static const int HEADER_LENGTH = 7;
};

inline  void
Storage::setSampleLength(int len)
{
  m_sampleLenght = len;
}

#endif /* STORAGE_H_ */
