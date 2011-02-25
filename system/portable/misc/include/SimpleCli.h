/*
 *	Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef SIMPLECLI_H_
#define SIMPLECLI_H_

#include "portable/kernel/include/OS.h"

#include "portable/stdlib/include/istream"
#include "portable/stdlib/include/ostream"

class SimpleCli
  {
public:
  SimpleCli(unsigned char *pLine, unsigned short iSize);
  ~SimpleCli();

  int readLine(istream& cin, ostream& cout);

  void parseReset(void);
  unsigned char *parseNext(void);

  char parseHexNibble(unsigned char *p, unsigned char *pch);
  char parseHex(unsigned char *p, unsigned char *pch);
  char parseHex(unsigned char *p, unsigned short *psh);
  char parseHex(unsigned char *p, unsigned long *plh);
  char parseUnsigned(unsigned char *p, unsigned short *psh);

private:
  unsigned char *m_pLine;
  unsigned short m_iSize;
  unsigned char *m_pNext;
  };

#endif /*SIMPLECLI_H_ */
