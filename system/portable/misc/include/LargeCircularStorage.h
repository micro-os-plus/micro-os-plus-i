/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef LARGECIRCULARSTORAGE_H_
#define LARGECIRCULARSTORAGE_H_

#include "portable/kernel/include/uOS.h"

// ----------------------------------------------------------------------------

class LargeCircularStorage
{
public:
  LargeCircularStorage(OSDeviceBlock& dev);
  ~LargeCircularStorage();

private:
  OSDeviceBlock& m_dev;
};

// ----------------------------------------------------------------------------

#endif /* LARGECIRCULARSTORAGE_H_ */
