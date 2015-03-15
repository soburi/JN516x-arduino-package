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

Patching SDK
====
newlib functions are failed linking.
To resolve, applying this patch to SDK.
Linker was set to prefer to read LD file from hardware\arduino\jen\linker.
Patched file put on to this folder.

```
--- NXP/bstudio_nxp/sdk/JN-SW-4165/Chip/JN5164/Build/AppBuildEnd_JN5164.ld	2014-08-27 18:50:14.000000000 +0900
+++ NXP/bstudio_nxp/sdk/JN-SW-4165/Chip/JN5164/Build/AppBuildEnd_JN5164.ld.newlib	2015-02-10 02:06:11.024287900 +0900
@@ -65,6 +65,7 @@
             . = ALIGN (4);
             _bss_end = ABSOLUTE(.);
         } > ram
+	end = ABSOLUTE(.);
 
         /* reserve minimum heap size */
         .heap ALIGN (0x4) :
@@ -80,6 +81,7 @@
             _stack_low_water_mark = ABSOLUTE(.);
             . += _stack_size;
         } > ram
+	_stack = ABSOLUTE(.);
 
         /* Test to check if the .data initialisation values in flash has gone
            past the end of the available space; the linker doesn't throw an
```

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

