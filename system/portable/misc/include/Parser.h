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
  setInput(unsigned char* pLine);

  void
  setSeparators(unsigned char* pSeparators);
  void
  setSpaces(unsigned char* pSpaces);

  void
  setTokenBuffer(unsigned char* pToken, unsigned short tokenSize);

  // Deprecated
  void
  setToken(unsigned char* pToken, unsigned short tokenSize);
  unsigned char*
  getToken(void) const;

  void
  setCurrent(unsigned short index);

  bool
  isEndOfLine(void) const;

  OSReturn_t
  skipTokens(unsigned short nTokens);
  OSReturn_t
  skipTokens(unsigned short nTokens, unsigned char* pSeparators);

  OSReturn_t
  parseToken(void);
  OSReturn_t
  parseToken(unsigned char* pSeparators, unsigned char* pSpaces);

  OSReturn_t
  parseSubstring(unsigned short len);

  unsigned short
  getTokenLength(void) const;
  unsigned char
  getSeparator(void);

  OSReturn_t
  parseHex(unsigned char* pChar);
  OSReturn_t
  parseUnsigned(unsigned short* pShort);
  OSReturn_t
  parseSigned(signed long* pLong);

  OSReturn_t
  parseFixedPrec(signed long* pLong, unsigned short prec);

  int
  tokenCompare(const unsigned char* pStr);

  // Warning: does not work on AVR32
  int
  tokenCompareIgnoreCase(const unsigned char* pStr);

  // static methods
  static OSReturn_t
  parseNibble(unsigned char nibble);
  static OSReturn_t
  parseHex(unsigned char* pStr, unsigned char* pChar);
  static OSReturn_t
  parseUnsigned(unsigned char* pStr, unsigned short* pShort);
  static OSReturn_t
  parseSigned(unsigned char* pStr, signed long* pLong);
  static OSReturn_t
  parseUnsigned(unsigned char* pStr, unsigned long* pLong);

  static OSReturn_t
  parseFixedPrec(unsigned char* pStr, signed long* pLong, unsigned short prec,
      bool hasSign);

protected:
  // members
  unsigned char* m_pLine;
  unsigned char* m_pCrt;

  unsigned char* m_pSeparators;
  unsigned char* m_pSpaces;

  unsigned char* m_pToken;
  unsigned short m_tokenSize;

  unsigned char m_sep;
  unsigned short m_len;
};

// ----------------------------------------------------------------------------

inline unsigned char* 
Parser::getToken(void) const
{
  return m_pToken;
}

inline unsigned char
Parser::getSeparator(void)
{
  return m_sep;
}

inline void
Parser::setTokenBuffer(unsigned char* pToken, unsigned short tokenSize)
{
  m_pToken = pToken;
  m_tokenSize = tokenSize;
}

inline void
Parser::setToken(unsigned char* pToken, unsigned short tokenSize)
{
  m_pToken = pToken;
  m_tokenSize = tokenSize;
}

inline void
Parser::setSeparators(unsigned char* pSeparators)
{
  m_pSeparators = pSeparators;
}

inline void
Parser::setSpaces(unsigned char* pSpaces)
{
  m_pSpaces = pSpaces;
}

inline unsigned short
Parser::getTokenLength(void) const
{
  return m_len;
}

inline bool
Parser::isEndOfLine(void) const
{
  return (m_sep == '\0');
}

#endif /* PARSER_H_ */
