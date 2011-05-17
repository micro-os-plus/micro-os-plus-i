/*
 *      Copyright (C) 2008-2011 Liviu Ionescu.
 *
 *      This file is part of the uOS++ distribution.
 */

#include "portable/kernel/include/OS_Defines.h"

#if defined(OS_INCLUDE_SDI12BOOTLOADER)

#include "portable/kernel/include/OS.h"

#include "portable/devices/sdi12/include/SDI12BootLoader.h"

// ----- Static Data ----------------------------------------------------------

unsigned char SDI12BootLoader::ms_doReset;
unsigned short SDI12BootLoader::ms_addr;
unsigned short SDI12BootLoader::ms_len;
unsigned short SDI12BootLoader::ms_first;
unsigned short SDI12BootLoader::ms_last;
bool SDI12BootLoader::ms_hasInputCrc;
unsigned short SDI12BootLoader::ms_CRCin;
unsigned short SDI12BootLoader::ms_CRCout;
unsigned short SDI12BootLoader::ms_prevFirst;

unsigned char SDI12BootLoader::ms_moreCommand;

unsigned char SDI12BootLoader::ms_bufW[Flash::BLOCK_SIZE+3];
unsigned char SDI12BootLoader::ms_bufOut[Flash::BLOCK_SIZE];
unsigned char SDI12BootLoader::ms_tmp[4];

unsigned char SDI12BootLoader::ms_base64chars[65] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void SDI12BootLoader::base64out(unsigned char ch)
  {
    //debug.putHex((unsigned char)(ch & 0x3F));
    //debug.putChar(' ');
    storeCharResponse(ms_base64chars[ch & 0x3F]);
  }

void SDI12BootLoader::init(void)
  {
    if (OS::isDebug())
      {
        OSDeviceDebug::putString("SDI12BootLoader::init()");
        OSDeviceDebug::putNewLine();
      }

    SDI12Sensor::init();

    ms_prevFirst = ms_first = 0xFFFF;

  }

unsigned char SDI12BootLoader::base64in(unsigned char ch)
  {
    unsigned char i;
    for (i = 0; i < 64; ++i)
    if (ch == ms_base64chars[i])
    return i;

    if (ch == '=')
    return 0;

    return 0xFF;
  }

unsigned char SDI12BootLoader::hexIn(unsigned char ch)
  {
    unsigned char b;
    b = 0;
    ch = toupper(ch);
    if ('0' <= ch && ch <= '9')
    b = ch - '0';
    else if ('A' <= ch && ch <= 'F')
    b = ch - 'A' + 10;

    return b;
  }

void SDI12BootLoader::storeOkUnsigned(unsigned short w)
  {
    storeStringResponse("0,"); // return ok
    if ((w >> 8) == 0)
    storeHexResponse((unsigned char)w);
    else
    storeHexResponse(w);
  }

bool SDI12BootLoader::processCommand(void)
  {
    bUseCRC = false;

    ms_moreCommand = 0;

    ms_doReset = 0;

    if (false)
      {
        // ignore trailing '*' used to minimize maching of '!' by
        // noncompliant sensors
        if (ms_buf[ms_bufCount - 1] == '*')
          {
            ms_buf[--ms_bufCount] = '\0';
          }
      }

    unsigned char ch;
    ch = parseNext();

    if (ch != 'X')
    return false;

    ch = parseNext();
    if (ch == 'C')
      {

        if (ms_bufCount < 6)
          {
            return false; // input too short
          }

        bUseCRC = true;

        unsigned char* p;

        unsigned short crc;
        crc = 0;

        p = &ms_buf[ms_bufCount - 3];
        crc = ms_crc;

        if ((*p == '=') && (*(p+1) == '=') && (*(p+2) == '='))
          {
            // test case, ignore crc
          }
        else
          {
            unsigned short w;
            if (*p < 0x40 || *p> 0x7F)return false;

            w = (*p++) & 0x3F;
            if (*p < 0x40 || *p> 0x7F)
            return false;
            w = (w << 6) | ((*p++) & 0x3F);
            if (*p < 0x40 || *p> 0x7F)
            return false;
            w = (w << 6) | ((*p++) & 0x3F);

            if (w != crc)
              {
                OSDeviceDebug::putString("crc ");
                OSDeviceDebug::putHex(crc);
                OSDeviceDebug::putChar(' ');
                OSDeviceDebug::putHex(w);
                OSDeviceDebug::putChar(' ');
                OSDeviceDebug::putChar((crc >> 12) | 0x40);
                OSDeviceDebug::putChar(((crc >> 6) & 0x3F) | 0x40);
                OSDeviceDebug::putChar((crc & 0x3F) | 0x40);
                OSDeviceDebug::putChar(' ');

                return false;
              }
          }
        // resize input string, including terminator
        ms_bufCount -= 3;
        ms_buf[ms_bufCount] = '\0';

        ch = parseNext();
      }

    if (ch != '$')
    return false;

    ch = parseNext();
    if (ch == 'B')
      {
        //debug.putString("boot ");

        // boot
        if (parseIsEnd())
          {
            // reply with 1 to confirm we are in bootloader mode
            storeCharResponse('1');
          }
        else
          {
            ms_doReset = parseNext();
            if (!parseIsEnd())
            return false;

            ms_moreCommand = 'B';
          }
      }
    else if (ch == 'V')
      {
        if (parseIsEnd())
          {
            //debug.putString("ver ");

            storeHexResponse((unsigned char)APP_CFGINT_MAJOR);
            storeCharResponse(',');
            storeHexResponse((unsigned char)APP_CFGINT_MINOR);
            storeCharResponse(',');
            storeHexResponse((unsigned short)APP_CFGINT_REVISION);

            //storeStringResponse(APP_CONFIG_MAJOR "," APP_CONFIG_MINOR "," APP_CONFIG_REVISION);
          }
        else
          {
            return false;
          }
      }
    else if (ch == 'S')
      {
        if (parseIsEnd())
          {
            //debug.putString("sig ");

            unsigned char c;
            c = boot_signature_byte_get(0x0);
            //debug.putHex(c);
            storeHexResponse(c);
            storeCharResponse(',');
            c = boot_signature_byte_get(0x2);
            //debug.putHex(c);
            storeHexResponse(c);
            storeCharResponse(',');
            c = boot_signature_byte_get(0x4);
            //debug.putHex(c);
            storeHexResponse(c);
          }
        else
          {
            return false;
          }
      }
    else if (ch == 'W' || ch == 'R' || ch == 'E' || ch == 'C')
      {
        unsigned char cmd;
        cmd = ch;

        unsigned char dest;
        dest = parseNext();

        if (dest != 'F')
        return false; // for the moment only flash implemented


        if (cmd == 'W' || cmd == 'R' || cmd == 'E')
          {
            if (cmd == 'W' && parseIsEnd())
              {
                if (ms_first == 0xFFFF)
                  {
                    goto return_one;
                  }
                // flush command; void in the current implementation
                storeOkUnsigned(Flash::BLOCK_WRITE_MILLIS + BLOCK_WRITE_GUARD_MILLIS);
                ms_moreCommand = 'W';
                ms_first = 0xFFFF;
                ms_len = 0;
                return true;
              }

            ms_addr = parseHex();
            ch = parseNext();
            if (ch != ',')
            return false;
            ms_len = parseHex();

            if (ms_len == 0)
              {
                goto return_one;
              }
          }
        if (cmd == 'W')
          {
            if (parseNext() != ',')
            return false;

            if (ms_len> Flash::BLOCK_SIZE)
            return false;

            ms_last = ms_addr + ms_len - 1;
            ms_last |= (Flash::BLOCK_SIZE - 1);

            ms_first = ms_addr & ~(Flash::BLOCK_SIZE - 1);
            if ((ms_last - ms_first) >= Flash::BLOCK_SIZE || ms_addr >= Flash::BOOTLOADER_START)
              {
                goto return_one;
              }

#if false
            debug.putChar(' ');
            debug.putHex(ms_addr);
            debug.putChar(',');
            debug.putHex(ms_len);
            debug.putChar(',');
            debug.putHex(ms_first);
            debug.putChar(',');
            debug.putHex(ms_last);
            debug.putChar(' ');
#endif
            unsigned int i;
            if (!bUseCRC)
              {
                // hex binary
                for (i = 0; i < ms_len; ++i)
                  {
                    unsigned char b;
                    b = 0;
                    for (unsigned char j = 0; j < 2; ++j)
                      {
                        b <<= 4;

                        ch = parseNext();
                        if (isxdigit(ch))
                          {
                            b |= hexIn(ch);
                          }
                        else
                          {
                            goto return_one;
                          }
                      }
                    ms_bufW[i] = b;
#if false
                    debug.putHex(b);
                    debug.putChar(' ');
#endif
                  }
              }
            else
              {
                // base64 binary

                OSDebugLed1::off();

                unsigned short k;
                for (k = 0; k < ms_len;)
                  {
                    for (unsigned char j = 0; j < 4; ++j)
                      {
                        ch = base64in(parseNext());
                        if (ch == 0xFF)
                          {
                            goto return_one;
                          }
                        ms_tmp[j] = ch;
                      }

                    ms_bufW[k++] = (ms_tmp[0] << 2) | ((ms_tmp[1] >> 4) & 0x03);
                    ms_bufW[k++] = (ms_tmp[1] << 4) | ((ms_tmp[2] >> 2) & 0x0F);
                    ms_bufW[k++] = (ms_tmp[2] << 6) | (ms_tmp[3]);

#if false
                    debug.putHex(ms_bufW[k-3]);
                    debug.putHex(ms_bufW[k-2]);
                    debug.putHex(ms_bufW[k-1]);
                    debug.putChar(' ');
#endif
                  }
                OSDebugLed1::on();

              }

            storeOkUnsigned(Flash::BLOCK_WRITE_MILLIS + BLOCK_WRITE_GUARD_MILLIS);
            ms_moreCommand = 'W';
          }
        else if (cmd == 'R')
          {
            unsigned char base;
            base = 16;
            if (!parseIsEnd())
              {
                ch = parseNext();
                if (ch != ',')
                return false;

                base = parseHex();
              }
            if (!parseIsEnd())
              {
                return false;
              }

#if false
            debug.putChar(' ');
            debug.putHex(ms_addr);
            debug.putChar(',');
            debug.putHex(ms_len);
            debug.putChar(',');
            debug.putHex(base);
            debug.putNewLine();
#endif

            storeStringResponse("0,"); // return ok

            if (!bUseCRC)
              {
                for (unsigned int i=0; i < ms_len; ++i, ++ms_addr)
                  {
                    ch = Flash::readByte(ms_addr);

                    storeHexResponse(ch);
                  }
              }
            else
              {
                for (unsigned int i=0; i < ms_len; i+=3, ms_addr+=3)
                  {
                    signed short delta;
                    delta = ms_len-i;

                    ms_tmp[0] = Flash::readByte(ms_addr);
                    ms_tmp[1] = (delta >= 2) ? Flash::readByte(ms_addr+1) : 0;
                    ms_tmp[2] = (delta >= 3) ? Flash::readByte(ms_addr+2) : 0;

                    base64out(ms_tmp[0]>>2);
                    base64out(((ms_tmp[0] << 4) & 0x30) | ((ms_tmp[1] >> 4) & 0x0F));
                    if (delta >= 2)
                    base64out(((ms_tmp[1] << 2) & 0x3C) | ((ms_tmp[2] >> 6) & 0x03));
                    else
                    storeCharResponse('=');

                    if (delta >= 3)
                    base64out(ms_tmp[2]);
                    else
                    storeCharResponse('=');
                  }
              }
          }
        else if (cmd == 'E')
          {
            if (!parseIsEnd())
              {
                return false;
              }

#if false
            debug.putChar(' ');
            debug.putHex(ms_addr);
            debug.putChar(',');
            debug.putHex(ms_len);
            debug.putNewLine();
#endif

            storeOkUnsigned((ms_len+Flash::BLOCK_SIZE-1)
                /Flash::BLOCK_SIZE*Flash::BLOCK_WRITE_MILLIS + BLOCK_WRITE_GUARD_MILLIS); // return ok

            ms_moreCommand = 'E';
          }
        else if (cmd == 'C')
          {
            if (parseIsEnd())
              {
                if (ms_hasInputCrc)
                  {
                    storeCharResponse(ms_CRCin == ms_CRCout ? '0' : '1'); // return ok if correct
                  }
                else
                  {
                    storeOkUnsigned(ms_CRCout);
                  }
              }
            else
              {
                ms_addr = parseHex();
                ch = parseNext();
                if (ch != ',')
                return false;
                ms_len = parseHex();

                if (ms_len == 0)
                  {
                    goto return_one;
                  }
                ms_hasInputCrc = false;
                ms_CRCin = 0;
                if (!parseIsEnd())
                  {
                    ch = parseNext();
                    if (ch != ',')
                    return false;

                    ms_CRCin = parseHex();
                    ms_hasInputCrc = true;
                  }
                if (!parseIsEnd())
                  {
                    return false;
                  }

#if false
                debug.putChar(' ');
                debug.putHex(ms_addr);
                debug.putChar(',');
                debug.putHex(ms_len);
                debug.putChar(',');
                debug.putHex(ms_CRCin);
                debug.putNewLine();
#endif

                storeOkUnsigned((ms_len+31)/32); // return ok

                ms_moreCommand = 'C';
              }
          }
      }
    else
      {
        return false;
      }
    return true;

    return_one:
    storeCharResponse('1'); // return err
    return true;

  }

void SDI12BootLoader::processDelayedActions(void)
  {
    if (ms_moreCommand != 0)
      {
        OSDeviceDebug::putString("more ");
        OSDebugLed1::on();

        if (ms_moreCommand == 'B')
          {
            OSDeviceDebug::putString("reset ");
            OSDeviceDebug::putChar(ms_doReset);

            if (ms_doReset == '0')
              {
                EEPROM::writeByte(OS_CONFIG_SDI12BOOTLOADER_APP_MAGIC_OFFSET, OS_CONFIG_SDI12BOOTLOADER_APP_MAGIC_VALUE);
                EEPROM::busyWait();
              }

            OS::SOFTreset();
          }
        else if (ms_moreCommand == 'C')
          {
            ms_CRCout = 0;

            for (unsigned int i=0; i < ms_len; ++i, ++ms_addr)
              {
                ms_CRCout = crc16(ms_CRCout, Flash::readByte(ms_addr));
              }
            OSDeviceDebug::putChar(' ');
            OSDeviceDebug::putHex(ms_CRCout);
            OSDeviceDebug::putChar(' ');
          }
        else if (ms_moreCommand == 'E')
          {
            OSDeviceDebug::putString("era ");

            unsigned short last;
            last = ms_addr + ms_len - 1;
            last |= (Flash::BLOCK_SIZE - 1);

            ms_addr &= ~(Flash::BLOCK_SIZE - 1);
            ms_len = (last + 1 - ms_addr)/Flash::BLOCK_SIZE;

            //debug.putDec(len);
            EEPROM::busyWait();

            for (unsigned int i = 0; i < ms_len; ++i)
              {
                Flash::pageErase(ms_addr);
                Flash::pageEraseBusyWait();

                OSDeviceDebug::putHex(ms_addr);
                OSDeviceDebug::putChar(' ');

                ms_addr += Flash::BLOCK_SIZE;
              }

            Flash::enableRead();

            ms_prevFirst = ms_first = 0xFFFF;
          }
        else if (ms_moreCommand == 'W')
          {
            OSDeviceDebug::putString("wrt ");

            unsigned int i;

            if (ms_first != ms_prevFirst)
              {
                // current block different than cached one

                // eventually write out cached one and read in new one

                if (ms_prevFirst != 0xFFFF)
                  {
                    // write current block

                    OSDeviceDebug::putString("W ");
                    OSDeviceDebug::putHex(ms_prevFirst);

                    EEPROM::busyWait();

                    for (i = 0; i < Flash::BLOCK_SIZE; i+=2)
                      {
                        unsigned short w;
                        // prepare little endian word
                        w = (ms_bufOut[i+1] << 8) | (ms_bufOut[i]);

                        Flash::pageFillWord(ms_prevFirst+i, w);
                      }

                    Flash::pageWrite(ms_prevFirst);
                    Flash::pageWriteBusyWait();

                    Flash::enableRead();
                  }

                if (ms_first != 0xFFFF)
                  {
                    OSDeviceDebug::putString("R ");
                    OSDeviceDebug::putHex(ms_first);

                    // read in original block
                    for (i = 0; i < Flash::BLOCK_SIZE; ++i)
                    ms_bufOut[i] = Flash::readByte(ms_first+i);
                  }
                // remember cached block
                ms_prevFirst = ms_first;
              }

            // overwrite new bytes
            for (i = 0; i < ms_len; ++i)
              {
                ms_bufOut[(ms_addr & (Flash::BLOCK_SIZE-1)) + i] = ms_bufW[i];
              }

          }
        OSDebugLed1::off();
        ms_moreCommand = 0;
      }
  }

void SDI12BootLoader::threadMain(void)
  {
    SDI12Sensor::timerInit();

    OSDebugLed1::init();
    OSDebugLed2::init();

    OSScheduler::ledActiveInit();

    for (;;)
      {
        OSDeviceDebug::putNewLine();
        if (readCommand())
          {
            if (processCommand() && writeResponse())
              {
                // comand valid and response transmitted,
                // we can process delayed actions
                processDelayedActions();
              }
          }
      }

    // should not return
  }

#endif

