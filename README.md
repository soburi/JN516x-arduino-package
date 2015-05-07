About
=====

This is arduino add-on package for NXP JN516x platform.


Install
=======

Install JN516x plugin to arduino
--------------------------------

### for Arduino 1.6.2 or newer versions.

* See [jn516x arduino proxy](https://github.com/soburi/jn516x-arduino-proxy).

### for Arduino 1.6.1 or older versions.

- Extract [plugin archive](https://github.com/soburi/JN516x-arduino-package/archive/master.zip)
into _[arduino\_root]/hardware/arduino_ folder.
- Extrace [toolchain archive](https://dl.dropboxusercontent.com/u/498101/jn516x-tools_i686-mingw32_r8352.tar.bz2) into into _[arduino\_root]/hardware/tools_ folder.


Install JN516x SDK
------------------
Install JN516x-JenNet-IP Stack SDK.

* [JN-SW-4065-JN516x-JenNet-IP-SDK-v979](http://www.nxp.com/documents/other/JN-SW-4065.zip)

### Windows

Simply install to default destination (C:\Jennic).

### MacOSX and Linux

Use [wine](https://www.winehq.org/) to launch installer program.
Install to Z:\opt\Jennic(point to /opt/Jennic in filesystem).


Install Flash Programmer
------------------------

### Windows
Install JN516x Production Flash Programmer.

* [JN516x Production Flash Programmer](http://www.nxp.com/documents/other/JN-SW-4107.zip)

Install to default destination (C:\NXP\bstudio_nxp).

### MacOSX and Linux
Install jenprog

* [jenprog](https://github.com/pengphei/jenprog)


API implement status
====================

Already implemented
-------------------
### Digital I/O
- pinMode()
- digitalWrite()
- digitalRead()

### Analog I/O
- analogReference()
- analogRead()
- analogWrite()

### Advanced I/O
- shiftOut()
- shiftIn()

### Time
- millis()
- micros()
- delay()

### Math
- min()
- max()
- abs()
- constrain()
- map()
- pow()
- sqrt()

### Trigonometry
- sin()
- cos()
- tan()

### Random Numbers
- randomSeed()
- random()

### Bits and Bytes
- lowByte()
- highByte()
- bitRead()
- bitWrite()
- bitSet()
- bitClear()
- bit()

### Interrupts
- interrupts()
- noInterrupts()

### External Interrupts
- attachInterrupt()
- detachInterrupt()

### Communication
- Serial
- Stream

Not implemented yet.
--------------------

### Time
- delayMicroseconds()

### Advanced I/O
- tone() _This function will follow the implementation status of Arduino Due._
- noTone() _This function will follow the implementation status of Arduino Due._
- pulseIn() _System dependent constants are not tuned._


Limitation by Hardware difference
=================================
### Not support function.
- analogReadResolution()
- analogWriteResolution()
- Keyboard
- Mouse


### Interrupt mode
JN516x does not support LEVEL triggerd and both edge triggerd interruption, support only RISING and FALLING.
attatchInterrupt() treat HIGH, LOW, CHANGE as RISING.
