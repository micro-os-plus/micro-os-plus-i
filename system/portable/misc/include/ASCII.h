/*
 *      Copyright (C) 2007-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#ifndef ASCII_H_
#define ASCII_H_

class ASCII
{

public:
  const static uchar_t STX = 0x02;
  const static uchar_t ETX = 0x03;

  const static uchar_t BELL = 0x07;

  const static uchar_t LF = 0x0A;
  const static uchar_t CR = 0x0D;

  const static uchar_t ESC = 0x1B;
  const static uchar_t DEL = 0x7F;
};

#endif /* ASCII_H_ */
