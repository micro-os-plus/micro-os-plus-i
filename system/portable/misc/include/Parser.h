/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "portable/kernel/include/OS.h"

class Parser
{
public:
  Parser();

  void
  setInput(unsigned char *pLine);

  void
  setSeparators(unsigned char *pSeparators);
  void
  setSpaces(unsigned char *pSpaces);

  void
  setTokenBuffer(unsigned char *pToken, unsigned short tokenSize);

  // Deprecated
  void
  setToken(unsigned char *pToken, unsigned short tokenSize);
  unsigned char *
  getToken(void);

  void
  setCurrent(unsigned short index);

  OSReturn_t
  parseToken(void);
  OSReturn_t
  parseToken(unsigned char *pSeparators, unsigned char *pSpaces);

  OSReturn_t
  parseSubstring(unsigned short len);

  unsigned short
  getTokenLength(void);
  unsigned char
  getSeparator(void);

  OSReturn_t
  parseHex(unsigned char *pChar);
  OSReturn_t
  parseUnsigned(unsigned short *pShort);
  OSReturn_t
  parseSigned(signed long *pLong);

  OSReturn_t
  parseFixedPrec(signed long *pLong, unsigned short prec);

  int
  tokenCompare(const unsigned char* pStr);

  // Warning: does not work on AVR32
  int
  tokenCompareIgnoreCase(const unsigned char* pStr);

  // static methods
  static OSReturn_t
  parseNibble(unsigned char nibble);
  static OSReturn_t
  parseHex(unsigned char *pChar, unsigned char *pStr);
  static OSReturn_t
  parseUnsigned(unsigned short *pShort, unsigned char *pStr);
  static OSReturn_t
  parseSigned(signed long *pLong, unsigned char *pStr);
  static OSReturn_t
  parseUnsigned(unsigned long *pLong, unsigned char *pStr);

  static OSReturn_t
  parseFixedPrec(signed long *pLong, unsigned short prec, bool hasSign,
      unsigned char *pStr);

protected:
  // members
  unsigned char *m_pLine;
  unsigned char *m_pCrt;

  unsigned char *m_pSeparators;
  unsigned char *m_pSpaces;

  unsigned char *m_pToken;
  unsigned short m_tokenSize;

  unsigned char m_sep;
  unsigned short m_len;
};

// ----------------------------------------------------------------------------

inline unsigned char *
Parser::getToken(void)
{
  return m_pToken;
}

inline unsigned char
Parser::getSeparator(void)
{
  return m_sep;
}

inline void
Parser::setTokenBuffer(unsigned char *pToken, unsigned short tokenSize)
{
  m_pToken = pToken;
  m_tokenSize = tokenSize;
}

inline void
Parser::setToken(unsigned char *pToken, unsigned short tokenSize)
{
  m_pToken = pToken;
  m_tokenSize = tokenSize;
}

inline void
Parser::setSeparators(unsigned char *pSeparators)
{
  m_pSeparators = pSeparators;
}

inline void
Parser::setSpaces(unsigned char *pSpaces)
{
  m_pSpaces = pSpaces;
}

inline unsigned short
Parser::getTokenLength(void)
{
  return m_len;
}

#endif /* PARSER_H_ */
