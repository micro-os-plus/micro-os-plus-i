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
  static const unsigned char STX = 0x02;
  static const unsigned char ETX = 0x03;

  static const unsigned char BELL = 0x07;

  static const unsigned char LF = 0x0A;
  static const unsigned char CR = 0x0D;

  const static uchar_t ESC = 0x1B;

};

#endif /* ASCII_H_ */
