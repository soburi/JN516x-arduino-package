/*
  Copyright (c) 2016 Tokita, Hiroshi  All right reserved.

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

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "wiring_constants.h"
#include "WInterrupts.h"
#include "platform.h"
#include "PinControl.h"

uint32_t* PwmPinControl::pwm_pins = NULL;
size_t PwmPinControl::pwm_num = 0;

int PinControl::sysfs_read(const char* path, char* buf, size_t buflen) {
	int fd = open(path, O_RDONLY);
	WARN_ON_ERROR(fd<0, "open(%s) failure:%d %s\n", path, errno, strerror(errno));
	if(fd < 0) return fd;
	int ret = read(fd, buf, buflen);
	WARN_ON_ERROR(ret<0, "read(%s, %p, %u) failure:%d %s\n", path, buf, buflen, errno, strerror(errno));
	close(fd);
	return ret;
}

int PinControl::sysfs_write_buf(const char* path, const char* buf) {
	int fd = open(path, O_WRONLY);
	WARN_ON_ERROR(fd<0, "open(%s) failure:%d %s\n", path, errno, strerror(errno));
	if(fd < 0) return fd;
	int ret = write(fd, buf, strnlen(buf,FILENAME_MAX));
	WARN_ON_ERROR(ret<0, "write(%s, %s, %d) failure:%d %s\n", path, buf, strnlen(buf,FILENAME_MAX), errno, strerror(errno));
	close(fd);
	return ret;
}

int PinControl::sysfs_write(const char* path, int value) {
	char buf[4];
	sprintf(buf, "%d", value);
	return sysfs_write_buf(path, buf);
}

int PinControl::value2string(int value, char* buf) {
	return sprintf(buf, "%d\n", value);
}

int PinControl::sysfs_set(int fd, int value, int (val2str)(int, char*) ) {
	LOG_DEBUG("%s fd=%d value=%d val2str=%p\n", __PRETTY_FUNCTION__, fd, value, val2str);

	char* buf = new char[FILENAME_MAX];
	int ret;

	ret = val2str(value, buf);
	if(ret < 0) return ret;

	ret = write(fd, buf, strnlen(buf, FILENAME_MAX));
	WARN_ON_ERROR(ret<0, "write(%d, %s, %d) failure:%d %s\n", fd, buf, strnlen(buf,FILENAME_MAX), errno, strerror(errno));
	delete[] buf;
	return ret;
}

int PinControl::sysfs_get(int fd, int& value, int (str2val)(const char*) ) {
	//LOG_DEBUG("%s fd=%d str2val=%p\n", __PRETTY_FUNCTION__, fd, str2val);
	char* buf = new char[FILENAME_MAX];
	int ret;

	lseek(fd, 0, SEEK_SET);
	ret = ::read(fd, buf, FILENAME_MAX);
	LOG_DEBUG("read(%d)\n", fd);
	LOG_DEBUG("buf=%s\n", buf);
	LOG_DEBUG("ret=%d\n", ret);
	LOG_DEBUG("failure:%d %s\n", errno, strerror(errno));
	if(ret < 0) return ret;

	value = str2val(buf);
	delete[] buf;
	return ret;
}

int PinControl::openFd(uint32_t fdid, const char* pathfmt, int value)
{
	int fd = getFd(fdid);
	if(fd == 0) {
		char* buf = new char[FILENAME_MAX];
		snprintf(buf, FILENAME_MAX, pathfmt, value);
		LOG_DEBUG("open(%s)\n", buf);
		fd = open(buf, O_RDWR);
		LOG_DEBUG("fd=%d open(%s)\n", fd, buf);
		setFd(fdid, fd);
		delete[] buf;
	}
	return fd;
}

////////////////////////////////////////////////////

GpioPinControl::GpioPinControl(uint32_t pin) : PinControl(pin, fds_)
{
}

int GpioPinControl::exportPin()
{
	LOG_DEBUG("%s pin=%u\n", __PRETTY_FUNCTION__, pin);
	if( isExported() ) { return 0; }

	int ret = sysfs_write("/sys/class/gpio/export", pin);

	if (ret < 0) { return ret; }

	int count = 0;
	int fd = -1;

	//Wait for the reflection of udev
	while(fd < 0 && count++ <50) {
		fd = getFdDirection();
	}

	return fd;
}

bool GpioPinControl::isExported() {
	char* buf = new char[FILENAME_MAX];
	struct stat st;

	sprintf(buf, "/sys/class/gpio/gpio%d", pin);

	int ret = stat(buf, &st);
	delete[] buf;

	return (ret==0);
}

int GpioPinControl::setMode(uint32_t mode)
{
	LOG_DEBUG("%s(val=%d)\n", __PRETTY_FUNCTION__, mode);
	return sysfs_set(getFdDirection(), mode, direction_val2str);
}


int GpioPinControl::getMode()
{
	int mode;
	LOG_DEBUG("%s()\n", __PRETTY_FUNCTION__);
	int ret = sysfs_get(getFdDirection(), mode, direction_str2val);
	WARN_ON_ERROR(ret<0, "%s pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, pin, ret, errno, strerror(errno) );

	return mode;
}


int GpioPinControl::setValue(uint32_t value)
{
	LOG_DEBUG("%s value=%u\n", __PRETTY_FUNCTION__, value);
	return sysfs_set(getFdValue(), value,  value2string);
}


int GpioPinControl::getValue()
{
	int value;
	LOG_DEBUG("%s\n", __PRETTY_FUNCTION__);
	int ret = sysfs_get(getFdValue(), value, atoi);
	WARN_ON_ERROR(ret<0, "%s pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, pin, ret, errno, strerror(errno) );

	return value;
}

int GpioPinControl::setEdge(uint32_t value)
{
	LOG_DEBUG("%s value=%u\n", __PRETTY_FUNCTION__, value);
	return sysfs_set(getFdEdge(), value,  edge_val2str);
}

int GpioPinControl::getEdge()
{
	int value;
	LOG_DEBUG("%s\n", __PRETTY_FUNCTION__);
	int ret = sysfs_get(getFdEdge(), value, edge_str2val);
	WARN_ON_ERROR(ret<0, "%s pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, pin, ret, errno, strerror(errno) );

	return value;
}

int GpioPinControl::startInterruptPolling(uint32_t ulMode, voidFuncPtr callback)
{
	this->callback = callback;
	this->interrupt_mode = ulMode;

	LOG_DEBUG("pthread_create %p\n", poll_interrupt_proc);
	pthread_create(&thread, NULL, poll_interrupt_proc, this);
	LOG_DEBUG("end pthread_create %p\n", poll_interrupt_proc);

	return 0;
}

int GpioPinControl::stopInterruptPolling()
{
	LOG_DEBUG("pthread_cancel %p\n", this);
	pthread_cancel(thread);
	LOG_DEBUG("end pthread_create %p\n", this);
	pthread_join(thread, NULL);
	LOG_DEBUG("end pthread_join %p\n", this);
	return 0;
}

void GpioPinControl::delete_buffer(void* arg)
{
	char* buf = reinterpret_cast<char*>(arg);
	LOG_DEBUG("delete_buffer %p\n", buf);
	delete[] buf;
}

void* GpioPinControl::poll_interrupt_proc(void* arg)
{
	LOG_DEBUG("%s %p\n", __PRETTY_FUNCTION__, arg);

	GpioPinControl* pc = reinterpret_cast<GpioPinControl*>(arg);

	LOG_DEBUG("arg %p\n", arg);
	LOG_DEBUG("params %p\n", pc);
	LOG_DEBUG("params->interrupt_mode %d\n", pc->interrupt_mode);
	LOG_DEBUG("params->callback %p\n", pc->callback);

	int fd = pc->getFdValue();
	pc->poll_params.fd = fd;
	pc->poll_params.events = POLLPRI;

	int ret = pc->setEdge(pc->interrupt_mode);

	char* buf = new char[FILENAME_MAX];
	LOG_DEBUG("pthread_cleanup_push %p\n", buf);
	pthread_cleanup_push(delete_buffer, buf);

	ret = ::read(fd, buf, 64);
	while(true) {
		ret = poll(&pc->poll_params, 1, -1);

		if(ret > 0 && pc->poll_params.revents & POLLPRI) {

			ret = ::read(fd, buf, 64);
			LOG_DEBUG("read(%d) ret=%d\n", fd, ret);
			lseek(fd, 0, SEEK_SET);

			pc->callback();
		}
	}

	pthread_cleanup_pop(0);
}


int GpioPinControl::direction_val2str(int value, char* buf) {
	int ret = -1;
	if(value == 0) {
		ret = sprintf(buf, "in\n");
	}
	else if(value == 1) {
		ret = sprintf(buf, "out\n");
	}

	return ret;
}

int GpioPinControl::direction_str2val(const char* str) {
	if(strcmp(str, "in\n") == 0) {
		return INPUT;
	}
	else if(strcmp(str, "out\n") == 0) {
		return OUTPUT;
	}
	return -1;
}

int GpioPinControl::edge_val2str(int value, char* buf) {
	int ret = -1;
	if(value == CHANGE) {
		ret = sprintf(buf, "both\n");
	}
	else if(value == FALLING) {
		ret = sprintf(buf, "falling\n");
	}
	else if(value == RISING) {
		ret = sprintf(buf, "rising\n");
	}
	else if(value == LOW) {
		ret = sprintf(buf, "none\n");
	}
	else if(value == HIGH) {
		ret = sprintf(buf, "none\n");
	}
	return ret;
}

int GpioPinControl::edge_str2val(const char* str) {
	if(strcmp(str, "both\n") == 0) {
		return CHANGE;
	}
	else if(strcmp(str, "falling\n") == 0) {
		return FALLING;
	}

	else if(strcmp(str, "rising\n") == 0) {
		return RISING;
	}

	return -1;
}

////////////////////////////////////////////////////

PwmPinControl::PwmPinControl(uint32_t pin) : PinControl(pin, fds_)
{
}

int PwmPinControl::exportPin()
{
	LOG_DEBUG("%s pin=%u\n", __PRETTY_FUNCTION__, pin);
	if( isExported() ) { return 0; }

	int ret = sysfs_write("/sys/class/pwm/pwmchip0/export", pwm_id(pin) );

	if (ret < 0) { return ret; }

	int count = 0;
	int fd = -1;

	//Wait for the reflection of udev
	while(fd < 0 && count++ <50) {
		fd = getFdEnable();
	}

	return fd;
}


bool PwmPinControl::isExported() {
	char* buf = new char[FILENAME_MAX];
	struct stat st;

	sprintf(buf, "/sys/class/pwm/pwmchip0/pwm%d", pwm_id(pin));

	int ret = stat(buf, &st);
	delete[] buf;

	return (ret==0);
}

int PwmPinControl::setValue(uint32_t value)
{
	int ret = 0;
	int period = value ? (2000*255/value) : 0;
	int enable = value == 0 ? 0 : 1;

	LOG_DEBUG("%s value=%u\n", __PRETTY_FUNCTION__, value);
	ret = sysfs_set(getFdPeriod(), period,  value2string);
	WARN_ON_ERROR(ret<0, "%s period:%d pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, period, pin, ret, errno, strerror(errno) );

	ret = sysfs_set(getFdDutyCycle(), 2000,  value2string);
	WARN_ON_ERROR(ret<0, "%s duty_cycle:2000 pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, pin, ret, errno, strerror(errno) );

	ret = sysfs_set(getFdEnable(), enable,  value2string);
	WARN_ON_ERROR(ret<0, "%s enable:%d pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, enable, pin, ret, errno, strerror(errno) );

	return ret;
}

int PwmPinControl::getValue()
{
	int period;
	LOG_DEBUG("%s\n", __PRETTY_FUNCTION__);
	int ret = sysfs_get(getFdPeriod(), period, atoi);
	WARN_ON_ERROR(ret<0, "%s pin=%u return:%d errno:%d(%s)\n", __PRETTY_FUNCTION__, pin, ret, errno, strerror(errno) );

	return 2000*255/period;
}

int PwmPinControl::pwm_id(uint32_t pin) {
	int pwmid= -1;
	for(size_t i=0; i<pwm_num; i++) {
		if(pwm_pins[i] == pin) pwmid = i;
	}
	return pwmid;
}

void PwmPinControl::set_pwm_pins(const char* pwm_pin_str) {
	int len = strlen(pwm_pin_str);

	char* buf = new char[len+1];
	strcpy(buf, pwm_pin_str);

	if(len < 0) return;

	int sep_num = 0;
	for(int i=0; i<len; i++) {
		if(buf[i] == ',') {
			buf[i] = '\0';
			sep_num++;
		}
	}
	pwm_num = sep_num + 1;
	pwm_pins = new uint32_t[pwm_num];

	char* p = buf;
	for(size_t i=0; i<pwm_num; i++ ) {
		pwm_pins[i] = atoi(p);
		p += (strlen(p) + 1);
	}
	delete[] buf;
}

