About
=====

This is Arduino add-on package for NXP JN516x platform.
This package is built on [ContikiOS](http://contiki-os.org/).

Install
=======

Install JN516x plugin to Arduino
--------------------------------

Use this additional package definition of 'Additional Boards Manager URLs' option that is on Arduino's preferences.
* [http://soburi.github.io/JN516x-arduino-package/package_soburi_intiki_index.json](http://soburi.github.io/JN516x-arduino-package/package_soburi_intiki_index.json)

Install Compiler and SDK.
----------------------------

Download and install compiler and sdk from [NXP's JN516x page](https://www.nxp.com/products/wireless-connectivity/proprietary-ieee-802.15.4-based/ieee-802.15.4-for-jn516x-7x:IEEE802.15.4).

* `JN-SW-4141` BeyondStudio for NXP 
* `JN-SW-4163` JN516x IEEE 802.15.4 SDK

### For Windows

Install `JN-SW-4141` and `JN-SW-4163` to installer's default destination (C:\NXP).

### For Linux/MacOSX

Install *JN516x IEEE 802.15.4 SDK* contents (JN-SW-4163 directory, need to extract on windows) under `/usr/jn516x-sdk/`(default directory as Contiki expects).
If installed to another directory, set the path to `SDK_BASE_DIR` and export it.


Limitations
===========

### Not support function.

- analogReadResolution()
- analogWriteResolution()
- Keyboard
- Mouse


### Interrupt mode
JN516x does not support LEVEL triggerd and both edge triggerd interruption, support only RISING and FALLING.
attatchInterrupt() treat HIGH, LOW, CHANGE as RISING.
