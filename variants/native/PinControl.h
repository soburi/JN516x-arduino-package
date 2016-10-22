/*
  Copyright (c) 2016 TOKITA Hiroshi.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdint.h>
#include <poll.h>
#include <pthread.h>
#include "wiring_private.h"

class PinControl {
public:
	virtual ~PinControl() {}

	virtual int setMode(uint32_t mode) = 0;
	virtual int getMode() = 0;

	virtual int setValue(uint32_t value) = 0;
	virtual int getValue() = 0;

	virtual int exportPin() = 0;
	virtual bool isExported() = 0;

	virtual int startInterruptPolling(uint32_t ulMode, voidFuncPtr callback) = 0;
	virtual int stopInterruptPolling() = 0;

	static int sysfs_read(const char* path, char* buf, size_t buflen);
	static int sysfs_write_buf(const char* path, const char* buf);
	static int sysfs_write(const char* path, int value);

	static int sysfs_set(int fd, int value, int (val2str)(int, char*) );
	static int sysfs_get(int fd, int& value, int (str2val)(const char*) );

	static int value2string(int value, char* buf);

protected:
	PinControl(uint32_t pin_, int* fds_) : pin(pin_), fds(fds_) { }

	void setFd(uint32_t fdid, int fd) { fds[fdid] = fd; }
	int  getFd(uint32_t fdid) { return fds[fdid]; }

	int openFd(uint32_t fdid, const char* pathfmt, int value);
	int openFd(uint32_t fdid, const char* pathfmt) { return openFd(fdid, pathfmt, pin); }

	uint32_t pin;
	int* fds;
};

class GpioPinControl : public PinControl {
public:
	GpioPinControl(uint32_t pin);

	int setMode(uint32_t mode);
	int getMode();

	int setValue(uint32_t value);
	int getValue();

	int exportPin();
	bool isExported();

	int startInterruptPolling(uint32_t ulMode, voidFuncPtr callback);
	int stopInterruptPolling();

	static int direction_val2str(int value, char* buf);
	static int direction_str2val(const char* str);
	static int edge_val2str(int value, char* buf);
	static int edge_str2val(const char* str);

private:
	enum {
		VALUE,
		DIRECTION,
		EDGE
	};

	int getFdValue() { return openFd(VALUE, "/sys/class/gpio/gpio%d/value"); }
	int getFdDirection() { return openFd(DIRECTION, "/sys/class/gpio/gpio%d/direction"); }
	int getFdEdge() { return openFd(EDGE, "/sys/class/gpio/gpio%d/edge"); }

	int setEdge(uint32_t value);
	int getEdge();

	static void delete_buffer(void* arg);
	static void* poll_interrupt_proc(void* arg);

	int fds_[3];

	voidFuncPtr callback;
	uint32_t interrupt_mode;

	pthread_t thread;
	struct pollfd poll_params;

};

class PwmPinControl : public PinControl {
friend void initVariant();
public:
	PwmPinControl(uint32_t pin);

	int setMode(uint32_t) { return 0; }
	int getMode() { return OUTPUT; }

	int setValue(uint32_t value);
	int getValue();

	int exportPin();
	bool isExported();

	int startInterruptPolling(uint32_t, voidFuncPtr) { return 0; }
	int stopInterruptPolling() { return 0; }

	static int pwm_id(uint32_t pin);

private:
	enum {
		DUTY_CYCLE,
		PERIOD,
		ENABLE
	};

	int getFdDutyCycle() { return openFd(DUTY_CYCLE, "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", pwm_id(pin) ); }
	int getFdPeriod() { return openFd(PERIOD, "/sys/class/pwm/pwmchip0/pwm%d/period", pwm_id(pin) ); }
	int getFdEnable() { return openFd(ENABLE, "/sys/class/pwm/pwmchip0/pwm%d/enable", pwm_id(pin) ); }

	static void set_pwm_pins(const char* pwm_pins_str);

	int fds_[3];

	static uint32_t* pwm_pins;
	static size_t pwm_num;
};
