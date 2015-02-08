About
=====

This is arduino add-on package for NXP JN516x platform.


Install
====

Copy into Arduino 1.5.x SDK.

Install TOCOS TWE Engine's SDK.

[ftp://tocos-wireless.com/TWESDK_201408-31/TWESDK_wo_Eclipse_201408-31.zip](ftp://tocos-wireless.com/TWESDK_201408-31/TWESDK_wo_Eclipse_201408-31.zip)
[ftp://tocos-wireless.com/TWESDK_201406/TWESDK_Eclipse_Kepler.zip](ftp://tocos-wireless.com/TWESDK_201406/TWESDK_Eclipse_Kepler.zip)


Append build.path=[SOMEWHRER] and preproc.save_build_files=true.
to Arduino preferences (%APPDATA%\Arduino15\preferences.txt).

Currently, the author use TOCOS TWE Engine, the which is JN5164 breakout  board selling in japan.

Build environment is tested in TOCOS TWE Engine.
Unzip this files to C:\TWESDK.

(Behavior in SDK of Jennic has not yet been confirmed.)



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
