/*
 *      Copyright (C) 2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef UTIL_JSON_PARSER_H_
#define UTIL_JSON_PARSER_H_

#include "portable/kernel/include/OS.h"

namespace util
{
  namespace json
  {
    class Parser
    {
    public:
      Parser(const uchar_t* pBuf, std::size_t len);
      ~Parser();

      void
      setInput(const uchar_t* pBuf, std::size_t len);

      OSReturn_t
      skipCharacter(uchar_t c);

      OSReturn_t
      parseVariableName(uchar_t* pName, std::size_t siz);

      OSReturn_t
      parseString(uchar_t* pName, std::size_t siz);

      OSReturn_t
      parseNumber(uint32_t& num);

      OSReturn_t
      parseNumber(uint64_t& num);

      OSReturn_t
      parseNumber(int32_t& num);

      OSReturn_t
      parseByteArray(uint8_t* byteArray);

      uchar_t
      skipSpaces(void);

      bool
      hasMoreContent(void);

      uchar_t
      getCurrentChar(void);

      void
      advanceCurrent(void);

    private:
      const uchar_t* m_pBuf;
      std::size_t m_len;

      const uchar_t* m_pCrt;
      std::size_t m_remLen;

    };

    inline bool
    Parser::hasMoreContent(void)
    {
      return (m_remLen > 0);
    }

#if true
    inline uchar_t
    Parser::getCurrentChar(void)
    {
      return *m_pCrt;
    }
#endif
  }

}

#endif /* UTIL_JSON_PARSER_H_ */
