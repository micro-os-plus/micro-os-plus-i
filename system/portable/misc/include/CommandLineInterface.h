/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef COMMANDLINEINTERFACE_H_
#define COMMANDLINEINTERFACE_H_

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_COMMANDLINEINTERFACE)


#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/OSDeviceCharacter.h"

#include "portable/misc/include/Parser.h"

#include "portable/stdlib/include/istream"
#include "portable/stdlib/include/ostream"

class CommandLineInterface
{
public:
  typedef int CommandParameter_t;

  typedef OSReturn_t
  (*pCommandMethod_t)(std::istream& cin, std::ostream& cout, Parser& parser,
      CommandLineInterface& cli, void* pObject,
      CommandLineInterface::CommandParameter_t parameter);
  typedef void* pObject_t;

  class Token;
  typedef class Token Token_t;

  class Token
  {
  public:
    const char* pString;
    Token_t* pDown;
    pCommandMethod_t pMethod;
    CommandParameter_t parameter;
    const char* pHelp;
  };

public:
  CommandLineInterface(std::istream& cin, std::ostream& cout,
      unsigned char* pHistory, unsigned short iHistorySizeBytes);
  ~CommandLineInterface();

  void
  setCommands(Token_t* pToken, pObject_t* pObject);

  OSReturn_t
  loop(OSDeviceCharacter& dev, unsigned char* greeting = 0);

  int
  readLine(void);

  void
  processLine(void);

  void
  setPrompt(unsigned char* pPrompt);
  unsigned char*
  getPrompt(void) const;

  Parser&
  getParser(void);

  void
  recurseHelp(Token_t* pToken, uchar_t* pBuf, size_t bufSize, uint_t index);

private:

  size_t
  updateCurrentFromHistory(uchar_t* pHistory);

  OSReturn_t
  recurse(Token_t* p);

  std::istream m_cin;
  std::ostream m_cout;

  uchar_t* m_pHistory;
  uint_t m_iHistorySizeBytes;
  uchar_t* m_pHistoryCurrentPosition;
  uchar_t* m_pHistoryLineBeginning;

  unsigned char* m_pPrompt;

  Parser m_parser;
  unsigned char m_token[20]; // parsed token

  Token_t* m_pToken;
  pObject_t* m_pObject;
};

inline void
CommandLineInterface::setCommands(Token_t* pToken, pObject_t* pObject)
{
  m_pToken = pToken;
  m_pObject = pObject;
}

inline void
CommandLineInterface::setPrompt(unsigned char* pPrompt)
{
  m_pPrompt = pPrompt;
}

inline unsigned char*
CommandLineInterface::getPrompt(void) const
{
  return m_pPrompt;
}

inline Parser&
CommandLineInterface::getParser(void)
{
  return m_parser;
}

#endif /* defined(OS_INCLUDE_COMMANDLINEINTERFACE) */

#endif /* COMMANDLINEINTERFACE_H_ */
