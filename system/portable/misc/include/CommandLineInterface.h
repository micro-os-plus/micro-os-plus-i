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
class CliToken;

typedef OSReturn_t
(CommandLineInterface::*pCliMethod_t)(void);
typedef CommandLineInterface* pCliClass_t;
typedef class CliToken cliToken_t;

class CliToken
{
public:
  const char* pToken;
  cliToken_t* pDown;

  pCliMethod_t pMethod;
};

class CommandLineInterface
{
public:
  CommandLineInterface(std::istream& cin, std::ostream& cout, unsigned char* pLine,
      unsigned short iSize);
  ~CommandLineInterface();

  void
  setCommands(cliToken_t* pToken, pCliClass_t* pClass);

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
  recurse(cliToken_t* p);

  std::istream m_cin;
  std::ostream m_cout;

  unsigned char* m_pLine;
  unsigned short m_iSize;

  unsigned char* m_pPrompt;

  Parser m_parser;
  unsigned char m_token[20]; // parsed token

  cliToken_t* m_pToken;
  pCliClass_t* m_pClass;
};

inline void
CommandLineInterface::setCommands(cliToken_t* pToken, pCliClass_t* pClass)
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
