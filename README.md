About
=====

This is arduino add-on package for NXP JN516x platform.


Install
====

Copy into Arduino 1.5.x SDK.

Install BeyondStudio for NXP and JN516x-JenNet-IP Stack SDK.

[BeyondStudio for NXP](http://www.nxp.com/documents/other/JN-SW-4141.zip)
[JN516x-JenNet-IP Stack SDK](http://www.nxp.com/documents/other/JN-SW-4165.zip)

Install to default destination (C:\NXP\bstudio_nxp).

Append build.path=[SOMEWHRER] and preproc.save_build_files=true.
to Arduino preferences (%APPDATA%\Arduino15\preferences.txt).

Status
=====


API status
====

!!! This software is in Early development stage .


## partially working

- pinMode()
- digitalWrite()
- digitalRead() 
- analogReference()
- analogRead()
- analogWrite()
- millis()
- micros()
- delay()
- Serial
- Stream

GPIO Function is mostly works.

Timer is work correctly until 15 seconds from system boot.

Serial.print function is bad. Work only Serial.write(uint8).

## not implemented yet

- delayMicroseconds() 
- attachInterrupt()
- detachInterrupt()
- interrupts()
- noInterrupts() 

## not confirmed.

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

Device independed function maybe work correctly.


Image Burn
====

Binary files generated on build.path after build, burn the file to chip by writer program.
