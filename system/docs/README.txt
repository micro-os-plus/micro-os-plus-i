Generalities
------------

"uOS++" (micro OS plus plus) is based on a simple preemptive scheduler 
written in C++ that runs on ARM Cortex M3 and Atmel AVR microprocessors. 

It was originally developed for Atmel AVR at90usb devices and used for 
some experimental aviation instruments. Lately it was ported on 
atmega324p and used to build commercial SDI-12 sensors.

Starting with version 3.x it was also ported on ARM Cortex M3, specifically
the STM32F10X family.

It is expected that most future developments to be done for ARM Cortex 
devices, with the AVR port maintained for historic reasons.


Scheduler
---------

The preemptive scheduler uses both static priorities and round robin
strategies to schedule threads. Priorities range between 01-FF, with higher 
values representing higher priorities. If no special priority policies 
are needed, threads are created with default priority. When multiple threads 
with equal priorities are detected, the scheduler uses round robin 
for these threads, i.e. a thread is always inserted in the running list *after*
all threads with equal priority inserted before, so equal priority threads are 
given a more or less fair access to the CPU.

Synchronization primitives are based on Unix sleep/wakeup mechanism,
but the functions were named to more meaningfull eventWait()/eventNotify().

eventWait() puts the current thread to sleep, waiting for the event to occur.
The thread is removed from the running list and will not be scheduled to
run.

eventNotify() notifies all threads waiting to the given event. Notified
threads are inserted in the running list and will wait their turn to the CPU.
A return value can be specified, and this value will be returned by eventWait().

Device drivers
--------------

Simple character devices are defined. Optionally these devices can be
derived from "streambuf" and so can be used to construct standard 
input/output streams.

Portability
-----------

Code is split into a portable part and a hardware dependent part (hal).

Hardware dependent part is hierarchicaly split into architecture (like 
"avr" or "arm-cortex-m3"), family (like "at90usb" or "stm32f10x"), 
variant (like "at90usb1287") and board (like "stk525" or "stm32_h103").

Build toolchain
---------------

The only compiler I considered is the GNU C++ compiler. (currently 4.3.x);
on Windows systems the AVR compiler is packed in WinAVR and the ARM 
compiler is packed in Code Sourcery G++ Lite.

The recommended build environment is based on Eclipse (currently 3.4),
with the GNU ARM Plug-in and the AVR Plug-in (both on SourceForge)

Debug support
-------------

As for debug, any methods you are accustomed can be used. Based on my
experience with embedded devices, I preffer to use 2 dedicated pins and 
bit-bang an I2C protocol on them; the I2C bus is connected to a proprietary
device that dumps the debug characters on a USB serial output.

Samples
-------

Several samples are provided:

- the 'minimal' sample is the simplest code and does... nothing;
- 'blinkX3' shows how to run 3 instances of the same thread to blink 3
different leds with different blink rates;
- 'blinkX3Sync' defines 4 different threads and shows some simple thread
synchronization methods;
- 'calibrateBusyWait' helps determine the busyWait loop constants.

More advanced samples, using special hardware:

- 'cliX2' shows how to run two instances of a simple CLI process on both 
the USART port and the USB port, emulating a serial port (CDC);
- 'sdcard' shows how to read data from a MMC/SD Card; 
- 'sdi12sensor' sample shows how to build a SDI-12 sensor.

Final thoughts
--------------

Althought not required by the license, I would be very happy if you could
drop me a notice after downloading these sources. 

Any comments/critics/suggestions/etc would be highly appreciated.

Enjoy,

Liviu Ionescu
ilg@livius.net

