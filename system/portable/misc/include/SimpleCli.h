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
  SimpleCli(unsigned char* pLine, unsigned short iSize);
  ~SimpleCli();

  int
  readLine(std::istream& cin, std::ostream& cout);

#if defined(OS_INCLUDE_SIMPLECLI_PARSER)
  void
  parseReset(void);
  unsigned char* 
  parseNext(void);

  int8_t
  parseHexNibble(unsigned char* p, unsigned char* pch);
  int8_t
  parseHex(unsigned char* p, unsigned char* pch);
  int8_t
  parseHex(unsigned char* p, unsigned short* psh);
  int8_t
  parseHex(unsigned char* p, unsigned long* plh);
  int8_t
  parseUnsigned(unsigned char* p, unsigned short* psh);
#if defined(OS_INCLUDE_SIMPLECLI_PARSE_UNSIGNED_LONG)
  int8_t
  parseUnsigned(unsigned char* p, uint32_t* pl);
#endif /* defined(OS_INCLUDE_SIMPLECLI_PARSE_UNSIGNED_LONG) */

#endif /* defined(OS_INCLUDE_SIMPLECLI_PARSER) */

private:
  unsigned char* m_pLine;
  unsigned short m_iSize;
#if defined(OS_INCLUDE_SIMPLECLI_PARSER)
  unsigned char* m_pNext;
#endif /* defined(OS_INCLUDE_SIMPLECLI_PARSER) */
};

#endif /* SIMPLECLI_H_ */
