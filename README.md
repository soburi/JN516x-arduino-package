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


Configure arduino preference for debug
--------------------------------------
Append build.path=_[SOMEWHRER]_ and preproc.save_build_files=true.
to Arduino preferences (%APPDATA%\Arduino15\preferences.txt).


Patching SDK
------------
newlib functions are failed linking.
To resolve, applying this patch to SDK.
Linker was set to prefer to read LD file from _[JN516x plugin folder]_/linker.
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
=======================

Select [Tools]->[Board]->[TOCOS TWE-Lite] from menu, and
[Upload], runs your sketch on JN516x.

Status
======

Limitation by Hardware difference
---------------------------------

### Interrupt mode
JN516x does not support LEVEL triggerd and both edge triggerd nterruption.
 attatchInterrupt() treat HIGH, LOW, CHANGE as RISING.


API status
----------

** !!! This software is in Early development stage . **

### NOT WORK

- String

### working

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

### not implemented yet

- delayMicroseconds()
- interrupts()
- noInterrupts()

### not confirmed

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

