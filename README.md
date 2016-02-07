About
=====

This is Arduino add-on package for NXP JN516x platform.
This version is built on [Contiki](http://contiki-os.org/).

Install
=======

Install JN516x plugin to Arduino
--------------------------------

### For Arduino 1.6.7 or newer versions.

Use this additional package definition of 'Additional Boards Manager URLs' option that is on Arduino's preferences.
* [http://soburi.github.io/JN516x-arduino-package/package_soburi_intiki_index.json](http://soburi.github.io/JN516x-arduino-package/package_soburi_intiki_index.json)

This package does download external package on first build time.

Install Compiler and SDK.
----------------------------

Download and install compiler, sdk and flash programmer from [ NXP's JN516x page](http://www.nxp.com/ja/products/interface-and-connectivity/wireless-connectivity/2.4-ghz-wireless-solutions/ieee-802.15.4-for-jn516x:IEEE802.15.4).

* [BeyondStudio for NXP](http://cache.nxp.com/documents/other/JN-SW-4141.zip)

* [JN516x IEEE 802.15.4 SDK](http://cache.nxp.com/documents/other/JN-SW-4163.zip)

* [JN51xx Producton Flash Programmer](http://cache.nxp.com/documents/other/JN-SW-4107.zip)


### For Windows

Install to installer's default destination (C:\NXP).

API implements status
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
