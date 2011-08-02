/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef COMMANDLINEINTERFACE_H_
#define COMMANDLINEINTERFACE_H_

#include "portable/kernel/include/OS.h"

#include "portable/devices/character/include/OSDeviceCharacter.h"

#include "portable/misc/include/Parser.h"

#include "portable/stdlib/include/istream"
#include "portable/stdlib/include/ostream"

class CommandLineInterface;

class CommandLineInterface
{
public:
  typedef OSReturn_t
  (CommandLineInterface::*pCommandMethod_t)(void);
  typedef CommandLineInterface* pClass_t;

  class Token;
  typedef class Token Token_t;

  typedef int CommandParameter_t;

  class Token
  {
  public:
    const char* pString;
    Token_t* pDown;

    pCommandMethod_t pMethod;

    CommandParameter_t parameter;
  };

public:
  CommandLineInterface(std::istream& cin, std::ostream& cout,
      unsigned char* pLine, unsigned short iSize);
  ~CommandLineInterface();

  void
  setCommands(Token_t* pToken, pClass_t* pClass);

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

private:
  OSReturn_t
  recurse(Token_t* p);

  std::istream m_cin;
  std::ostream m_cout;

  unsigned char* m_pLine;
  unsigned short m_iSize;

  unsigned char* m_pPrompt;

  Parser m_parser;
  unsigned char m_token[20]; // parsed token

  Token_t* m_pToken;
  pClass_t* m_pClass;
};

inline void
CommandLineInterface::setCommands(Token_t* pToken, pClass_t* pClass)
{
  m_pToken = pToken;
  m_pClass = pClass;
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

#endif /* COMMANDLINEINTERFACE_H_ */
