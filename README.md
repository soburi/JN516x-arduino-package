About
=====

This is arduino add-on package for NXP JN516x platform.


Install
====

Copy into Arduino 1.6.x SDK.

Install BeyondStudio for NXP, JN516x-JenNet-IP Stack SDK and JN516x Production Flash Programmer..

[BeyondStudio for NXP](http://www.nxp.com/documents/other/JN-SW-4141.zip)
[JN516x-JenNet-IP Stack SDK](http://www.nxp.com/documents/other/JN-SW-4165.zip)
[JN516x Production Flash Programmer](http://www.nxp.com/documents/other/JN-SW-4107.zip)

Install to default destination (C:\NXP\bstudio_nxp).

Append build.path=[SOMEWHRER] and preproc.save_build_files=true.
to Arduino preferences (%APPDATA%\Arduino15\preferences.txt).


Build and Upload sketch
===

Select [Tools]->[Board]->[TOCOS TWE-Lite] from menu, and 
[Upload], runs your sketch on JN516x.

Status
=====

Limitation by Hardware difference
====

##### Interrupt mode
JN516x does not support LEVEL triggerd and both edge triggerd nterruption.
 attatchInterrupt() treat HIGH, LOW, CHANGE as RISING.


API status
====

!!! This software is in Early development stage .

## NOT WORK

- String

## working

- pinMode()
- digitalWrite()
- digitalRead() 
- analogReference()
- analogRead()
- analogWrite()
- millis()
- micros()
- delay()
- attachInterrupt()
- detachInterrupt()
- Serial

## not implemented yet

- delayMicroseconds() 
- interrupts()
- noInterrupts() 

## not confirmed

- analogReadResolution()
- analogWriteResolution() 
- tone()
- noTone()
- shiftOut()
- shiftIn()
- pulseIn() 
- min()
- max()
- abs()
- constrain()
- map()
- pow()
- sqrt() 
- sin()
- cos()
- tan() 
- randomSeed()
- random() 
- lowByte()
- highByte()
- bitRead()
- bitWrite()
- bitSet()
- bitClear()
- bit()
- Stream

Device independed function maybe work correctly.

