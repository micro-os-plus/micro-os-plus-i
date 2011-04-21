/*
 *	Copyright (C) 2011 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#ifndef COMMANDLINEINTERFACE_H_
#define COMMANDLINEINTERFACE_H_

#include "portable/kernel/include/OS.h"

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
  CommandLineInterface(istream& cin, ostream& cout, unsigned char *pLine, unsigned short iSize);
  ~CommandLineInterface();

  void
  setCommands(cliToken_t* pToken, pCliClass_t* pClass);

  int
  readLine();

  void
  processLine(void);

private:
  OSReturn_t recurse(cliToken_t* p);

  istream m_cin;
  ostream m_cout;

  unsigned char *m_pLine;
  unsigned short m_iSize;

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

#endif /* COMMANDLINEINTERFACE_H_ */
