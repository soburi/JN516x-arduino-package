RaspberryPi
===========

Prerequirement
--------------

### Target system

This package works on RPi3 and Raspbian disk image.
It may work with other RPi.


### libncurses-dev

```
$ sudo apt-get install libncurses5-dev
```


### OnBoard LED setup (optional)

RPi's ACT LED is controlled by LED driver.
It's can control by SYSFS not GPIO.
But LED driver can set blink according to the specific GPIO pin state.

Run following commands.

```
echo gpio > /sys/class/leds/led0/trigger
echo 27 > /sys/class/leds/led0/gpio # this program define BUILTIN_LED as 27
```


### PWM settings (optional)

librpip project shows good description of the difficulties of to work RPi's PWM.
http://librpip.frasersdev.net/peripheral-config/pwm0and1/


#### overlay

analogWrite() uses SYSFS interface which provided by PWM driver.
PWM driver is not load default system setting.
So, we must add overlay to /boot/config.txt.

Append this line to the end of /boot/config.txt

```
dtoverlay=pwm
```

or

```
dtoverlay=pwm-2chan
```

and resolv pimux conflict to comment out audio.
```
# dtparam=audio=on
```

More parameter to pwm overlay, see /boot/overlays/README.

### PWM CLK

Pwm overlay not start PWM CLK by default.
librpip project provides EXCELLENT workaround.

```
wget http://librpip.frasersdev.net/wp-content/uploads/2016/06/librpip-0.4.0.tar.gz
tar xvfz librpip-0.4.0.tar.gz
cd librpip-0.4.0
./configure
make
sudo ./bin/pwmclk

```

#### udev

PWM's SYSFS file default owner is set root:root.
We set root:gpio to owner when device loaded.

Add new rules-file to /etc/udev/rules.d.

```
# /etc/udev/rules.d/90-pwm.rules
SUBSYSTEM=="pwm", PROGRAM="/bin/sh -c '\
	chown -R root:gpio /sys$devpath && chmod -R 770 /sys$devpath\
'"
```


#### directory watch and enforce permission

PWM SYSFS interface owned by XXX when pin was exported.
It cannot change by udev, 
Run such program to enforce permission as background process.

```
#!/bin/sh

# enforce_pwm_permission

PWMPATH=/sys/class/pwm/pwmchip0
while [ ! -e ${PWMPATH}/export ] ; do true ; done

while [ -e ${PWMPATH}/export ] ; do
	inotifywait -e modify ${PWMPATH}/export 
	for XXX in `seq 0 10` ; do
		chown -R root:gpio ${PWMPATH}/pwm*
		chmod -R       770 ${PWMPATH}/pwm*
	done
done
```


Limitation
----------

- RPi has no ADC. analogRead() is not work.
- TCP/IP function was not tested.
