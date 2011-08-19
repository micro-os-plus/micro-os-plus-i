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

  // Set the byte array to be parsed
  void
  setInput(uchar_t* pLine);

  // Separators are characters between tokens
  void
  setSeparators(const char* pSeparators);

  // Spaces are characters to be ignored in the parsed string
  void
  setSpaces(const char* pSpaces);

  void
  setTokenBuffer(uchar_t* pToken, size_t tokenSize);

  // Deprecated
  //void
  //setToken(unsigned char* pToken, unsigned short tokenSize);

  uchar_t*
  getToken(void) const;

  uchar_t*
  getCurrentPosition(void) const;

  // Set the parser to an absolute position
  void
  setCurrentPosition(uint_t index);

  // Check if the parser reached the end of line
  bool
  isEndOfLine(void) const;

  OSReturn_t
  skipTokens(uint_t nTokens);
  OSReturn_t
  skipTokens(uint_t nTokens, const char* pSeparators);

  // Parse token, using the pre-programmed separators/spaces
  OSReturn_t
  parseNextToken(void);

  // Parse token, using the given separators/spaces
  OSReturn_t
  parseNextToken(const char* pSeparators, const char* pSpaces);

  // Parse next len bytes
  OSReturn_t
  parseNextSubstring(size_t len);

  size_t
  getTokenLength(void) const;
  bool
  isTokenEmpty(void);

  uchar_t
  getSeparator(void);

  OSReturn_t
  convertHex(uint8_t* pChar);
  OSReturn_t
  convertHex(uint16_t* pShort);
  OSReturn_t
  convertHex(uint32_t* pLong);
  OSReturn_t
  convertHex(uint_t* pChar);

  OSReturn_t
  convertUnsigned(uint16_t* pShort);
  OSReturn_t
  convertUnsigned(uint32_t* pLong);
  OSReturn_t
  convertUnsigned(uint_t* pInt);


  OSReturn_t
  convertSigned(int32_t* pLong);

  OSReturn_t
  convertFixedPrecision(int32_t* pLong, uint_t prec);

  int
  compareStringWithToken(const char* pStr);

  bool
  doesStringMatchToken(const char* pStr);

  // Check if the token is a substring at the beginning of the given string
  // Return false if the token is empty.
  bool
  doesStringStartWithToken(const char* pStr);

  // Warning: does not work on AVR32
  int
  compareStringWithTokenIgnoreCase(const char* pStr);

  // static methods
  static OSReturn_t
  convertNibble(uchar_t nibble);
  static OSReturn_t
  convertHex(uchar_t* pStr, void* p, size_t size);

  static OSReturn_t
  convertUnsigned(uchar_t* pStr, uint16_t* pShort);
  static OSReturn_t
  convertSigned(uchar_t* pStr, int32_t* pLong);
  static OSReturn_t
  convertUnsigned(uchar_t* pStr, uint32_t* pLong);
  static OSReturn_t
  convertUnsigned(uchar_t* pStr, uint_t* pInt);

  static OSReturn_t
  convertFixedPrecision(uchar_t* pStr, int32_t* pLong, uint_t prec,
      bool hasSign);

protected:

  void
  clearToken(void);

  // members
  uchar_t* m_pLine;
  uchar_t* m_pCrt;

  uchar_t* m_pSeparators;
  uchar_t* m_pSpaces;

  uchar_t* m_pToken;
  size_t m_tokenSize;

  uchar_t m_sep;
  size_t m_tokenLength;
};

// ----------------------------------------------------------------------------

inline uchar_t*
Parser::getToken(void) const
{
  return m_pToken;
}

inline uchar_t*
Parser::getCurrentPosition(void) const
{
  return m_pCrt;
}

inline uchar_t
Parser::getSeparator(void)
{
  return m_sep;
}

inline void
Parser::setTokenBuffer(uchar_t* pToken, size_t tokenSize)
{
  m_pToken = pToken;
  m_tokenSize = tokenSize;
}

//inline void
//Parser::setToken(uchar_t* pToken, size_t tokenSize)
//{
//  m_pToken = pToken;
//  m_tokenSize = tokenSize;
//}

inline void
Parser::setSeparators(const char* pSeparators)
{
  m_pSeparators = (uchar_t*)pSeparators;
}

inline void
Parser::setSpaces(const char* pSpaces)
{
  m_pSpaces = (uchar_t*)pSpaces;
}

inline size_t
Parser::getTokenLength(void) const
{
  return m_tokenLength;
}

inline bool
Parser::isTokenEmpty(void)
{
  return (m_tokenLength == 0);
}

inline bool
Parser::isEndOfLine(void) const
{
  return (m_sep == '\0');
}

#endif /* PARSER_H_ */
