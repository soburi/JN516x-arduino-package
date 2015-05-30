About
=====

This is arduino add-on package for NXP JN516x platform.


Install
=======

Install JN516x plugin to arduino
--------------------------------

### for Arduino 1.6.4 or newer versions.

*  Use this additional package definition by 'Additional Boards Manager URLs' option that is on Arduino's preferences.
[http://soburi.github.io/JN516x-arduino-package/package_soburi_jn516x_index.json](http://soburi.github.io/JN516x-arduino-package/package_soburi_jn516x_index.json)

### for Arduino 1.6.3 or older versions.

- Extract [plugin archive](https://github.com/soburi/JN516x-arduino-package/archive/master.zip)
into _[arduino\_root]/hardware/soburi_ folder.
- Extract [toolchain archive](https://dl.dropboxusercontent.com/u/498101/jn516x-tools_i686-mingw32_r8352.tar.bz2) into into _[arduino\_root]/hardware/tools_ folder.
- Put a platform.local.txt that contains following line,
```
runtime.tools.jn516x-tools.path={runtime.ide.path}/hardware/tools/jn516x-tools/
```
into a plugin archive extracted folder.


Install JN516x SDK
------------------
Install JN516x-JenNet-IP Stack SDK.

* [JN-SW-4065-JN516x-JenNet-IP-SDK-v979](http://www.nxp.com/documents/other/JN-SW-4065.zip)

### Windows

Simply install to default destination (C:\Jennic).

### MacOSX and Linux

Use [wine](https://www.winehq.org/) to launch installer program.
Install to Z:\opt\Jennic(point to /opt/Jennic in filesystem).


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
