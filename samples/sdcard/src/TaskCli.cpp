/*
 *	Copyright (C) 2007-2008 Liviu Ionescu.
 *
 *	This file is part of the uOS++ distribution.
 */

#include <ctype.h>

#include "TaskCli.h"
#include "portable/kernel/include/ostream_OSTask.h"

/*
 * Task constructor. 
 * Initialize system task object, initialize member objects
 * and store parameters in private members.
 *
 */

TaskCli::TaskCli(const char *pName, OSDeviceCharacter& dev, DeviceMemCard& card) :
  OSTask(pName, m_stack, sizeof(m_stack)), m_dev(dev), m_cin(&m_dev),
      m_cout(&m_dev), m_cli(m_line, sizeof(m_line)), m_card(card)
  {
    if (os.isDebug())
      {
        debug.putString("TaskCli()=");
        debug.putHex((unsigned short)this);
        debug.putNewLine();
      }
  }

// ---------------------------------------------------------------------------

static const char prompt[] = "> ";

/*
 * Task main code. 
 * Open device, output greeting and in a loop read and process lines.
 * I/O is done via standard streams routed to the attached device.
 *
 */

void TaskCli::taskMain(void)
  {
    if (os.isDebug())
      {
        os.sched.lock();
          {
            clog << "TaskCli::taskMain(" << showbase << hex
                << (unsigned short) this << ") SP="<< hex
                << (unsigned short) SP << endl;
          }
        os.sched.unlock();
      }

    OSDeviceCharacter& dev = m_dev;

    istream& cin = m_cin;
    ostream& cout = m_cout;

    SimpleCli & cli = m_cli;

    OS_CONFIG_USBINT_LED_PORT_INIT |= _BV( OS_CONFIG_USBINT_LED_BIT );
    OS_CONFIG_USBINT_LED_PORT_INIT |= _BV( PORTD0 );
    OS_CONFIG_USBINT_LED_PORT_INIT |= _BV( PORTD1 );

    // task endless loop
    for (;;)
      {
        dev.open(); // wait for dtr

        cout << noshowbase << endl << endl << greeting << endl;

        if (dev.getDeviceType() == OS_DEVICECHARACTER_USB_CDC)
          {
            cmdMI();
          }

        for (; dev.isConnected();)
          {
            cout << noshowbase << endl << prompt;

            int c;

            OS_CONFIG_USBINT_LED_PORT |= _BV( PORTD0 );
            c = cli.readLine(cin, cout);
            // turn on led

            if (c == traits::eof() )
              {
                if (os.isDebug)
                  clog << "disconnected"<< endl;

                break;
              }
            else if (c == OSReturn::OS_TIMEOUT)
              {
                if (os.isDebug)
                  clog << "timeout"<< endl;

                break;
              }
            else if (c < 0)
              {
                if (os.isDebug)
                  clog << "error -" << dec << ( int ) ( -c ) << endl;

                break;
              }

            lineProcess();

            OS_CONFIG_USBINT_LED_PORT &= ~_BV( OS_CONFIG_USBINT_LED_BIT );
            OS_CONFIG_USBINT_LED_PORT &= ~_BV( PORTD0 );
            //OS_CONFIG_USBINT_LED_PORT &= ~_BV( PORTD1 );

          }
        if (dev.getDeviceType() == OS_DEVICECHARACTER_USB_CDC)
          {
            m_card.close();
            bInit = false;
          }
        dev.close();
      }
  }

static const char str_checksum[] = "Checksum?";

/*
 * For cases when commands are sent by a device it is preferable to use
 * a method to validate the correctness of the command.
 * 
 * For this we addopted the following syntax:
 * 
 * 	$abcde*XX 
 * 
 * where XX is the hex value of the xor-ed 'abcde' string, starting with 00
 * 
 * To help human operators compute the checksum of some commands we also 
 * accept the form:
 * 
 * 	$abcde
 * 
 * situation when the command is not executed but only the checksum is 
 * computed and displayed.
 *
 */

int TaskCli::xorCheck(unsigned char *pc)
  {
    ostream & cout = m_cout;

    unsigned char c;
    unsigned char xor1, xor2;
    unsigned char *q;
    unsigned char *qq;

    *pc = ' ';

    for (xor1 = 0, q = pc + 1; (c = *q ) != '\0'; ++q)
      {
        if (c == '*')
          break;

        xor1 ^= c;
      }

    if (c == '\0')
      {
        // no xor received, return computed xor
        cout << endl << ("*");
        cout.width(2);
        cout << hex << (unsigned short) xor1 ;
        cout.flush();
        return -2;
      }

    qq = q;

    xor2 = 0;
    q++;
    c = *q;
    if (c == '\0')
      {
        // syntax error
        goto err;
      }

    if ( '0' <= c && c <= '9')
      xor2 = c - '0';
    else if ( 'A' <= c && c <= 'F')
      xor2 = c - 'A'+ 10;
    else
      {
        // syntax error
        goto err;
      }
    q++;
    c = *q;
    if (c == '\0')
      {
        // syntax error
        goto err;
      }

    xor2 <<= 4;

    if ( '0' <= c && c <= '9')
      xor2 |= c - '0';
    else if ( 'A' <= c && c <= 'F')
      xor2 |= c - 'A'+ 10;
    else
      {
        // syntax error
        goto err;
      }

    q++;
    c = *q;
    if (c != '\0')
      {
        // syntax error
        goto err;
      }

    if (xor1 != xor2)
      {
        // crc error
        cout << endl << str_checksum;
        return -2; // return 'crc error'
      }
    //*qq++ = ' ';
    *qq = '\0';

    return 0; // return OK

    err: return -1; // return 'syntax error'
  }

static const char str_help[] = "mi|mc|mr | ri|rd | st";
static const char str_unknown[] = "Cmd?";

/*
 * Parse input line, identify commands and output results.
 * 
 * Accepted commands:
 * 
 * 	mi 		- memory init
 * 	mc 		- memory config
 * 	mr blk	 	- memory read
 * 
 * 	st 		- show tasks
 * 
 * 	ri		- recorder info
 * 	rd blk w	- read data
 */

void TaskCli::lineProcess()
  {
    unsigned char * pc;
    pc = m_line;

    SimpleCli & cli = m_cli;
    ostream& cout = m_cout;
    //OSDeviceCharacter& dev = m_dev;

    unsigned char *p;
    unsigned char c;
    unsigned long l;
    unsigned short u;
    unsigned char v;
    unsigned char i;

    int r;

    os.WDTreset();

    //bInit = true;

    if ( *pc == '$') // is command using checksum?
      {
        r = xorCheck(pc);
        if (r == -2)
          return;
        else if (r == -1)
          goto err;
      }

    cli.parseReset(); // reset pointer to start of line

    p = cli.parseNext();
    if (p == 0)
      return;

    c = *p | 0x20;
    if ( *p == '?')
      {
        cout << endl << str_help;
      }
    else if (c == 's')
      {
        ++p;
        c = *p;
        if (c == '\0')
          goto err;

        c |= 0x20;
        if (c == 't')
          {
            cmdST();
          }
        else
          goto err;
      }
    else if (c == 'x')
      {
        if ( (p = cli.parseNext() ) == 0)
          goto err;

        if (cli.parseHex(p, &u) < 0)
          goto err;

        cmdX(u);
      }
    else if (c == 'm')
      {
        // test commands
        ++p;
        c = *p;
        if (c == '\0')
          goto err;

        c |= 0x20;
        if (c == 'i')
          {
            cmdMI();
          }
        else if (c == 'c')
          {
            cmdMC();
          }
        else if (c == 'r')
          {
            if ( (p = cli.parseNext() ) == 0)
              goto err;

            if (cli.parseHex(p, &l) < 0)
              goto err;

            cmdRD(l, 512 / 4);
          }
        else if (c == 'z')
          {
            m_card.close();
            bInit = true;
          }
        else
          goto err;
      }
    else if (c == 'r')
      {
        // flight recorder test commands
        ++p;
        c = *p;
        if (c == '\0')
          goto err;

        c |= 0x20;
        ++p;
        if ( *p != '\0')
          goto err;

        if (c == 'i')
          {
            if ( (p = cli.parseNext() ) != 0)
              goto err;

            cmdRI();
          }
        else if (c == 'd')
          {
            if ( (p = cli.parseNext() ) == 0)
              goto err;

            if (cli.parseHex(p, &l) < 0)
              goto err;

            if ( (p = cli.parseNext() ) != 0 && cli.parseHexNibble(p, &i) == 0)
              {
                v = i;
              }
            else
              {
                v = 0;
              }
            cmdRD(l, v);
          }
        else
          goto err;
      }
    else
      goto err;

    return;

    err: cout << endl << str_unknown;
    cout << endl << str_help;
  }

void TaskCli::cmdMI()
  {
    int r;
    ostream& cout = m_cout;

    r = m_card.open();

    if (r < 0)
      {
        cout << endl;
        if (r == MEMCARD_ERROR_MISSING)
          cout << "No Card";
        else
          cout << dec << r;

        bInit = true;
      }
    else
      {
        cout << endl;

        int status;
        status = m_card.getCapabilities();
        if ( (status & MEMCARD_CAPABILITIES_MMC) != 0)
          {
            cout << "MMC";
            if ( (status & MEMCARD_CAPABILITIES_MMC13) != 0)
              {
                cout << " 1.3";
              }
          }
        else if ( (status & MEMCARD_CAPABILITIES_SDC) != 0)
          {
            cout << "SDC";
            if ( (status & MEMCARD_CAPABILITIES_SDC2) != 0)
              {
                cout << " 2.0";
                if ( (status & MEMCARD_CAPABILITIES_SDC2_HC) != 0)
                  {
                    cout << " HC";
                  }
              }
            else
              {
                cout << " 1.x";
              }
          }

        bInit = false;
      }
  }

const char * speed[] =
  { "?", "1.0", "1.2", "1.3", "1.5", "2.0", "2.5", "3.0", "3.5", "4.0", "4.5",
      "5.0", "5.5", "6.0", "7.0", "8.0" };
const char * timeUnits[] =
  { "1ns", "10ns", "100ns", "1us", "10us", "100us", "1ms", "10ms" };
const char * speedUnits[] =
  { "100Kb/s", "1Mb/s", "10Mb/s", "100Mb/s", "?", "?", "?", "?s" };

void TaskCli::cmdMC()
  {
    int r;
    int i;
    unsigned char ch;
    ostream& cout = m_cout;
    unsigned short capabilities;

    capabilities = m_card.getCapabilities();
    cout << endl;

    r = m_card.readCtrl(MMC_SEND_CID, &m_buf[ MMC_CID_OFFSET ], 16);

    if (r == 0)
      {
        unsigned char * p;
        p = (unsigned char *) &m_buf[MMC_CID_OFFSET];

        cout << hex << "CID=";
        for (i = 0; i < 16; ++i)
          {
            cout.width( 2);
            cout << ( unsigned short ) (p[i]);
            cout << ' ';
          }
        cout << endl;

        if ( (capabilities & MEMCARD_CAPABILITIES_SDC) != 0)
          {
            cout << "'";
            for (i = 0; i < 5; ++i)
              {
                ch = p[ i + 3 ];
                if ( !isprint(ch) )
                  break;
                cout.put(ch);
              }
            cout << "' v";

            cout << dec << ( unsigned short ) (p[ 8 ] >> 4);
            cout << '.';
            cout << dec << ( unsigned short ) (p[ 8 ] & 0x0F);

            cout.width( 2);
            cout << ' '<< hex << ( unsigned short ) (p[ 0 ] );
            cout.width( 4);
            cout << '-'<< ( unsigned short ) ( (p[ 1 ] << 8) | p[ 2 ]);
            cout << '-'<< ( unsigned short ) ( (p[ 9 ] << 8) | p[ 10 ]);
            cout.width( 4);
            cout << ( unsigned short ) ( (p[ 11 ] << 8) | p[ 12 ]);

            unsigned short w;
            w = ( unsigned short ) (p[ 14 ] & 0x0F);
            cout << ' '<< dec << w;
            w = ( unsigned short ) ( ( (p[ 13 ] << 8) | p[ 14 ]) >> 4) + 2000;
            cout << '/'<< w;
          }
        else if ( (capabilities & MEMCARD_CAPABILITIES_MMC13) != 0)
          {
            cout << "'";
            for (i = 0; i < 6; ++i)
              {
                ch = p[ i + 3 ];
                if ( !isprint(ch) )
                  break;
                cout.put(ch);
              }
            cout << "' v";

            cout << dec << ( unsigned short ) (p[ 9 ] >> 4);
            cout << '.';
            cout << dec << ( unsigned short ) (p[ 9 ] & 0x0F);

            cout.width( 2);
            cout << ' '<< hex << ( unsigned short ) (p[ 0 ] );
            cout.width( 4);
            cout << '-'<< ( unsigned short ) ( (p[ 1 ] << 8) | p[ 2 ]);
            cout.width( 4);
            cout << '-'<< ( unsigned short ) ( (p[ 10 ] << 8) | p[ 11 ]);
            cout.width( 4);
            cout << ( unsigned short ) ( (p[ 12 ] << 8) | p[ 13 ]);

            unsigned short w;
            w = ( unsigned short ) (p[ 14 ] >> 4);
            debug.putHex(w);
            cout << ' ' << dec << w;
            w = ( unsigned short ) ( (p[ 14 ] & 0x0F) + 1997);
            debug.putHex(w);
            cout << '/' << w;

          }
      }
    else
      {
        cout << dec << r;
      }

    cout << endl << endl;

    r = m_card.readCtrl( MMC_SEND_CSD, m_buf, 16);

    if (r == 0)
      {
        unsigned char * p;
        p = ( unsigned char * ) &m_buf[ 0 ];

        cout << hex << "CSD=";
        for (i = 0; i < 16; ++i)
          {
            cout.width( 2);
            cout << ( unsigned short ) (p[ i ] );
            cout << ' ';
          }
        cout << endl;

        unsigned int w;
        unsigned long l;
        unsigned int csdVer;
        unsigned int c_size;
        unsigned int c_size_mult;
        unsigned int read_bl_len;

        if ( (capabilities & MEMCARD_CAPABILITIES_SDC) != 0)
          {
            csdVer = p[ 0 ] >> 6;
            cout << dec << 'v'<< (csdVer + 1) << ".0";

            if (csdVer == 0)
              {
                c_size = p[ 6 ] & 0x03;
                c_size <<= 8;
                c_size |= p[ 7 ];
                c_size <<= 2;
                c_size |= ( (p[ 8 ] >> 6) & 0x03);

                c_size_mult = (p[ 9 ] & 0x03);
                c_size_mult <<= 1;
                c_size_mult |= ( (p[ 10 ] >> 7) & 0x01);

                read_bl_len = (p[ 5 ] & 0x0F);

                l = (c_size + 1);
                l <<= (c_size_mult + 2+ read_bl_len );
                cout << " SIZE=(" << dec << c_size << "+1)*" << ( 1
                    << (c_size_mult + 2)) << "*" << ( 1 << read_bl_len ) << "=";
#if defined(MEMCARD_OUTPUT_SIZEBYTES)

                cout << l << "B=";
#endif

                cout << (l >> 20)<< "MB";

                w = (p[ 3 ] >> 3) & 0x0F;
                cout << " SPEED="<< speed[ w ];
                w = (p[ 3 ] & 0x07);
                cout << "*"<< speedUnits[ w ];

                cout << endl << "TAAC=";
                w = (p[ 1 ] >> 3) & 0x0F;
                cout << speed[ w ];
                w = (p[ 1 ] & 0x07);
                cout << "*" << timeUnits[ w ];

                w = p[ 2 ];
                cout << " NSAC=" << w;

                w = 1 << ( (p[ 12 ] >> 5) & 0x07);
                cout << " R2W=" << w;

                w = ( (p[ 12 ] & 0x03) << 2) | ( (p[ 13 ] >> 6) & 0x03);
                w = 1 << w;
                cout << " WBLEN=" << w;

              }
            else if (csdVer == 1)
              {
                l = p[ 7 ] & 0x3F;
                l <<= 8;
                l |= p[ 8 ];
                l <<= 8;
                l |= p[ 9 ];

                cout << " SIZE=(" << dec << l << "+1)*512=";

#if defined(MEMCARD_OUTPUT_SIZEBYTES)

                cout << ( l + 1 ) * 512 << "B=";
#endif

                cout << (l >> 20) << "MB";

                w = (p[ 3 ] >> 3) & 0x0F;
                cout << " SPEED=" << speed[ w ];
                w = (p[ 3 ] & 0x07);
                cout << "*" << speedUnits[ w ];

              }

          }
        else if ( (capabilities & MEMCARD_CAPABILITIES_MMC) != 0)
          {
            csdVer = (p[ 0 ] >> 6) & 0x03;
            cout << dec << "v1." << csdVer;

            int specVer;
            specVer = (p[ 0 ] >> 2) & 0x0F;
            cout << dec << " (" << specVer << ".x)";

            c_size = p[ 6 ] & 0x03;
            c_size <<= 8;
            c_size |= p[ 7 ];
            c_size <<= 2;
            c_size |= ( (p[ 8 ] >> 6) & 0x03);

            c_size_mult = (p[ 9 ] & 0x03);
            c_size_mult <<= 1;
            c_size_mult |= ( (p[ 10 ] >> 7) & 0x01);

            read_bl_len = (p[ 5 ] & 0x0F);

            l = (c_size + 1);
            l <<= (c_size_mult + 2+ read_bl_len );
            cout << " SIZE=(" << dec << c_size << "+1)*" << ( 1 << (c_size_mult
                + 2)) << "*" << ( 1 << read_bl_len ) << "=";
#if defined(MEMCARD_OUTPUT_SIZEBYTES)

            cout << l << "B=";
#endif

            cout << (l >> 20) << "MB";

            w = (p[ 3 ] >> 3) & 0x0F;
            cout << " SPEED=" << speed[ w ];
            w = (p[ 3 ] & 0x07);
            cout << "*" << speedUnits[ w ];

            cout << endl << "TAAC=";
            w = (p[ 1 ] >> 3) & 0x0F;
            cout << speed[ w ];
            w = (p[ 1 ] & 0x07);
            cout << "*" << timeUnits[ w ];

            w = p[ 2 ];
            cout << " NSAC=" << w;

            w = 1 << ( (p[ 12 ] >> 5) & 0x07);
            cout << " R2W=" << w;

            w = ( (p[ 12 ] & 0x03) << 2) | ( (p[ 13 ] >> 6) & 0x03);
            w = 1 << w;
            cout << " WBLEN=" << w;
          }
      }
  }

void TaskCli::cmdRI()
  {
    int r;
    ostream& cout = m_cout;

    if (bInit)
      {
        r = m_card.open();
        if (r < 0)
          {
            cout << endl << 'I' << dec << r;
            return;
          }
      }

    r = m_card.readCtrl( MMC_SEND_CSD, m_buf, 16);
    if (r != 0)
      {
        cout << endl << "CS" << dec << r;
        return;
      }

    r = m_card.readCtrl( MMC_SEND_CID, &m_buf[ MMC_CID_OFFSET ], 16);
    if (r != 0)
      {
        cout << endl << "CI" << dec << r;
        return;
      }
#if 0
    cout << endl << hex;

    for ( i = 0; i < MMC_BUFSIZE; ++i )
      {
        cout.width( 2 );
        cout << ( unsigned short ) m_buf[ i ];
      }
#endif

    signed short w;
    unsigned long l;

    w = ( signed char ) m_buf[ MMC_CID_OFFSET + 11 ];
    l = w;
    l <<= 8;
    l |= m_buf[ MMC_CID_OFFSET + 12 ];
    l <<= 8;
    l |= m_buf[ MMC_CID_OFFSET + 13 ];

    cout << endl << "uid="<< hex;
    cout.width( 4);
    cout << ( ( unsigned short ) (l >> 16) );
    cout.width( 4);
    cout << ( ( unsigned short ) (l) );

    cout << endl << "nmax=";

    unsigned short u;
    // Compute total size in blocks
    u = ( ( unsigned short ) (m_buf[ 6 ] & 0x03) << 10)
        | ( ( unsigned short ) m_buf[ 7 ] << 2)
        | ( ( unsigned short ) (m_buf[ 8 ] & 0xC0) >> 6);

    r = ( (m_buf[ 9 ] & 0x03) << 1) | ( (m_buf[ 10 ] & 0x80) >> 7);
    l = u + 1;
    l <<= (r + 2);
    --l;
    cout.width( 4);
    cout << ( ( unsigned short ) (l >> 16) );
    cout.width( 4);
    cout << ( ( unsigned short ) (l ) );

    unsigned long last;
    r = searchLast(l, &last, 0);
    if (r != 0)
      {
        cout << endl << "L" << dec << r;
        return;
      }
    l = last;
    cout << endl << "nbeg=";
    cout.width( 4);
    cout << ( ( unsigned short ) (l >> 16) );
    cout.width( 4);
    cout << ( ( unsigned short ) (l) );

    if (bInit)
      {
        bInit = false;
      }
  }

void TaskCli::cmdRD(unsigned long l, unsigned short v)
  {
    int r;
    ostream& cout = m_cout;

    if (bInit)
      {
        r = m_card.open();
        if (r < 0)
          {
            cout << endl;
            cout << 'I'<< dec << r;
            return;
          }
        bInit = false;
      }

    unsigned short len;
    len = ( (v == 0) ? MMC_BUFSIZE : (v * 4) );

#if 1

    r = m_card.setBlkLen(len);
    if (r != 0)
      {
        cout << endl;
        cout << 'B'<< dec << r;
        return;
      }
#endif

    OS_CONFIG_USBINT_LED_PORT &= ~_BV( PORTD0 );

    r = m_card.readBuffer( (l << MMC_BSHIFT), m_buf, len);
    if (r != 0)
      {
        cout << endl;
        cout << 'R' << dec << r;
        return;
      }
    OS_CONFIG_USBINT_LED_PORT |= _BV( PORTD0 );

    cout << endl << '$'<< hex;

    unsigned int i;
    unsigned char cxor;
    for (i = 0, cxor = 0; i < len; ++i)
      {
        unsigned char b;
        b = m_buf[ i ];

#if 0

        unsigned char c;
        c = ( ( b >> 4 ) & 0x0F );
        cout.put( c < 10 ? c + '0' : c + 'A' - 10 );
        //dev.writeByte( c < 10 ? c + '0' : c + 'A' - 10 );

        c = ( b & 0x0F );
        cout.put( c < 10 ? c + '0' : c + 'A' - 10 );
        //dev.writeByte( c < 10 ? c + '0' : c + 'A' - 10 );
#else

        cout.width( 2);
        cout << ( unsigned short ) b;
#endif

        cxor ^= b;
      }
    cout.width( 2);
    cout << '*' << ( unsigned short ) cxor;
    cout.flush();
    OS_CONFIG_USBINT_LED_PORT &= ~_BV( PORTD0 );

  }

void TaskCli::cmdST()
  {
    ostream & cout = m_cout;

    int cnt;
    cnt = os.sched.getTasksCount();

    for (int i = 0; i < cnt; ++i)
      {
        OSTask *pt;
        pt = os.sched.getTask(i);

        cout << endl;
        if (pt == this)
          cout << '*';
        else
          cout << ' ';

        cout << *pt; // print task info
      }
  }

void TaskCli::cmdX(unsigned short v)
  {
    ostream & cout = m_cout;
    istream & cin = m_cin;

    cout << endl << "'^D' to quit";

    for (;;)
      {
        cout << endl;
        cout << hex << v << ' ';
        cout.width(2);
        cout << ( unsigned short ) ( *( ( unsigned char* ) v ) ) << ' ';
        cout.flush();

        unsigned char c;

        c = cin.get();

        //clog << hex << ( unsigned short ) c << endl;

        if (c == ( 'D' - '@'))
          break;

        if ( (c == '\r') || (c == '\n'))
          {
            v++;
          }
        else
          {
            unsigned char b;
            b = 0;

            while ( !( (c == '\r') || (c == '\n')))
              {
                if ( '0' <= c && c <= '9')
                  {
                    b = (b << 4) | (c - '0');
                    cout.put(c);
                  }
                else if ( 'A' <= c && c <= 'F')
                  {
                    b = (b << 4) | (c - 'A'+ 10);
                    cout.put(c);
                  }
                else if ( 'a' <= c && c <= 'f')
                  {
                    b = (b << 4) | (c - 'a'+ 10);
                    cout.put(c);
                  }
                else
                  {
                    cout.put( '\b');
                  }
                cout.flush();
                c = cin.get();
              }
            //clog << hex << ( unsigned short ) b << endl;
            ( *( ( unsigned char* ) v ) ) = b;
          }
      }
  }

/*
 * Binary search on a circular structure. 
 * Each record has a monotone increasing key and a pointer (block number)
 * to the series begin.
 * 
 */
int TaskCli::searchLast(unsigned long max, unsigned long * plast,
    unsigned long * pid)
  {
    unsigned char i;
    int r;
    unsigned char rollover;

    unsigned char * mem_buf;
    mem_buf = m_buf;

    unsigned char ready; // true after mmc_init()
    ready = 0;

    unsigned long id_beg; // First block ID
    id_beg = 0;

    unsigned long id_last; // Last used block ID
    unsigned long blk_crt; // Current Block number
    unsigned long blk_beg; // Begin address (block)
    blk_beg = 0;
    unsigned long blk_max; // Max Block number
    blk_max = max;

    unsigned long blk_i, blk_j, blk_k;
    unsigned long id_i, id_j, id_k;
    unsigned long begblk_i, begblk_j, begblk_k;

    // search for last writen block
    r = m_card.setBlkLen( 2 * 4);
    if (r != 0)
      {
        if (os.isDebug() )
          {
            clog << "U"<< dec << r;
          }
        return r;
      }

    for (rollover = 0, i = 0; i < 2; ++i)
      {
        blk_i = 0;
        blk_j = blk_max;

        r = mmc_read_blkid(mem_buf, blk_i, &id_i, &begblk_i);
        //r = mmc_read_blkid(&mi, &vi, 0);
        if (r != 0)
          {
            if (os.isDebug() )
              {
                clog << "V"<< dec << r;
              }
            return r;
          }
        r = mmc_read_blkid(mem_buf, blk_j, &id_j, &begblk_j);
        //r = mmc_read_blkid(&mj, &vj, 0);
        if (r != 0)
          {
            if (os.isDebug() )
              {
                clog << "W"<< dec << r;
              }
            return r;
          }

        // Some additional checks required for first values
        if (i == 0)
          {
            if (id_i == 0&& id_j == 0&& begblk_i == 0&& begblk_j == 0)
              {
                // be sure we do not start at zero!
                blk_beg = blk_max; // start from last block
                id_beg = 0xFEF00000;
                if (os.isDebug() )
                  {
                    clog << "d";
                  }
                ready = 1;
                break;
              }
            if ( ( (id_j >> 24) == 0xFF) && ( (id_i >> 24) == 0x00))
              {
                // ID rollover right at last block
                rollover = 1;
                continue;
              }
            if (id_i < id_j)
              {
                blk_beg = 0;
                id_beg = id_j + 1;
                if (os.isDebug() )
                  {
                    clog << "i";
                  }
                ready = 1;
                break;
              }
          }
        else
          {
            if ( ( signed long ) id_i < ( signed long ) id_j )
              {
                blk_beg = 0;
                id_beg = id_j + 1;
                if (os.isDebug() )
                  {
                    clog << "I";
                  }
                ready = 1;
                break;
              }
          }

        for (; blk_j - blk_i > 1;)
          {
            if (i == 0)
              {
                if ( ( (id_i >> 24) == 0xFF) && ( (id_j >> 24) == 0x00))
                  {
                    // Rollover occured
                    rollover = 1;
                    break;
                  }
              }
            blk_k = (blk_i + blk_j ) / 2;
            r = mmc_read_blkid(mem_buf, blk_k, &id_k, &begblk_k);
            //r = mmc_read_blkid(&mk, &vk, 0);
            if (r != 0)
              {
                if (os.isDebug() )
                  {
                    clog << 'X'<< dec << r;
                  }
                return r;
              }
#if 0
            if ( os.isDebug() )
              {
                clog << endl << blk_i << '-' << id_i;
                clog << ' ' << blk_k << '-' << id_k;
                clog << ' ' << blk_j << '-' << id_j;
              }
#endif

            if (i == 0)
              {
                if ( (id_k < id_i ) || (id_k == 0))
                  {
                    blk_j = blk_k;
                    id_j = id_k;
                    begblk_j = begblk_k;
                  }
                else
                  {
                    blk_i = blk_k;
                    id_i = id_k;
                    begblk_i = begblk_k;
                  }
              }
            else
              {
                // Rollover case: use signed comparisons
                if ( ( ( signed long ) id_k < ( signed long ) id_i ) || (id_k
                    == 0))
                  {
                    blk_j = blk_k;
                    id_j = id_k;
                    begblk_j = begblk_k;
                  }
                else
                  {
                    blk_i = blk_k;
                    id_i = id_k;
                    begblk_i = begblk_k;
                  }
              }

            OS::WDTreset();
          }
        blk_beg = blk_j;
        id_beg = id_i + 1;

        if (rollover == 0|| i == 1)
          {
            // Consistency test to prevent overwriting usefull data
            // Warning: single session memory rollover case???
#if 0
            if ( begblk_i != begblk_j )
              {
                ready = 1;
              }
            else
              {
                if ( os.isDebug() )
                  {
                    clog << "#";
                  }
              }
            // Warning: this test is not correct, after memory rollover
            // we might have same session begin
#else
            ready = 1;
#endif

            break;
          }
      }

    id_last = id_beg;

    blk_crt = blk_beg;

    if (pid != 0)
      *pid = id_beg;

    if (plast != 0)
      *plast = blk_beg;

    return 0;

  }

#define BLK_ID_SIZE (4 + 4)

int TaskCli::mmc_read_blkid(unsigned char * pbuf, unsigned long blk,
    unsigned long * pid, unsigned long * pbeg)
  {
    unsigned long arg;
    int res;
    unsigned int j;

    for (j = 5; j != 0; --j)
      {
        res = m_card.readBuffer( (blk ) << MMC_BSHIFT, pbuf, BLK_ID_SIZE);

        if (res == 0)
          {
            if (pid != 0)
              {
                arg = pbuf[ 0 ];
                arg <<= 8;
                arg |= pbuf[ 1 ];
                arg <<= 8;
                arg |= pbuf[ 2 ];
                arg <<= 8;
                arg |= pbuf[ 3 ];
                *pid = arg;
              }

            if (pbeg != 0)
              {
                arg = pbuf[ 4 ];
                arg <<= 8;
                arg |= pbuf[ 5 ];
                arg <<= 8;
                arg |= pbuf[ 6 ];
                arg <<= 8;
                arg |= pbuf[ 7 ];
                *pbeg = arg;
              }
            break;
          }
        os.busyWaitMillis( 1);
      }

    return res;
  }

bool DeviceMemCard::validateVoltage(unsigned long l)
  {
    l = l;
    return true;
  }
