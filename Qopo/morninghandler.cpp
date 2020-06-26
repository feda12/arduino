/*
	morningroutine.cpp
	C++ source file for Qopo Morning Shower device
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
*/

#include "morninghandler.h"
// RTC DS1307chip libraries
#include <SoftI2C.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <TimeAlarms.h>

#include "protocol.h"
#include "helpers.h"

MorningHandler::MorningHandler()
{
	_ledsCount = 8;
	_latchPin = 8;
	_analogPin = 9;
	_clockPin = 12;
	_dataPin = 11;
	SoftI2C bus(A2,A3);
	_rtc = new Clock(bus, 0);
}

void MorningHandler::setup()
{
	RTCTime time;
	_rtc->readTime(&time);
	DEBUG(time.hour);
	DEBUG(":");
	DEBUG(time.minute);
	DEBUG(":");
	DEBUGLN(time.second);

	pinMode(_latchPin, OUTPUT);
	pinMode(_analogPin, OUTPUT); 
	pinMode(_dataPin, OUTPUT);  
	pinMode(_clockPin, OUTPUT);
	changeLedState(HIGH);
	setLedsBrightness(100);
}

int MorningHandler::alarm(const std::vector<std::string> &args)
{
	return 0;
}

int MorningHandler::setAlarm(const std::vector<std::string> &args)
{
	return 0;
}

int MorningHandler::setAlarmTime(const std::vector<std::string> &args)
{
	return 0;
}

int MorningHandler::setTime(const std::vector<std::string> &args)
{
	// We skip args[0] as it is command name
	if(args.size() > 2)
	{	
		int hours = (int)args.at(1).c_str();
		int minutes = (int)args.at(1).c_str();
		if(hours < 0 || hours > 24 || minutes < 0 || minutes > 59)
			// To replace by Invalid arguments error code
			return 9;

		RTCTime time;
		time.hour = hours;
		time.minute = minutes;
		_rtc->writeTime(&time);
	} else {
		return 3;
		// To replace by Invalid arguments count code
	}
	return 0;
}

int MorningHandler::time(const std::vector<std::string> &args)
{
	RTCTime time;
	_rtc->readTime(&time);
	DEBUG("morning-returning time:");
	DEBUG(time.hour);
	DEBUG(":");
	DEBUG(time.minute);
	DEBUG(":");
	DEBUGLN(time.second);
	return 0;
}

int MorningHandler::reset(const std::vector<std::string> &args)
{
	// Reset variable setting in EEPROM to default
	return 0;
}

int MorningHandler::forget(const std::vector<std::string> &args)
{
	return 0;
}

int MorningHandler::energyLight(const std::vector<std::string> &args)
{
	int argsCount = args.size();
	if(argsCount == 1)
	{
		changeLedState(HIGH);
		setLedsBrightness(100);
	}
	return 0;
}

int MorningHandler::setting(const std::vector<std::string> &args)
{
	// Change a setting in EEPROM
	// We skip args[0] as it is command name
	if(args.size() > 2)
	{	
	} else {
		return 3;
		// To replace by Invalid arguments count code
	}
	return 0;
}

// This method sends bits to the shift register:
void MorningHandler::changeLedState(int onOff) {
	// the bits you want to send
	byte bitsToSend = 0;

	// turn off the output so the pins don't light up
	// while you're shifting bits:
	digitalWrite(_latchPin, LOW);
	for(int pin = 0; pin < _ledsCount; pin++) 
		// turn on the next highest bit in bitsToSend:
		bitWrite(bitsToSend, pin, onOff);

	// shift the bits out:
	shiftOut(_dataPin, _clockPin, MSBFIRST, bitsToSend);

	// turn on the output so the LEDs can light up:
	digitalWrite(_latchPin, HIGH);
}

void MorningHandler::setLedsBrightness(int brightness) // 0 to 100
{
	if(brightness > 100)
		brightness = 100;
	if(brightness < 0)
		brightness = 0;
	byte bright = (byte)(brightness/100*255);
	analogWrite(_analogPin, 255-brightness);
}