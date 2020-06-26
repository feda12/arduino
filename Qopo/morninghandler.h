/*
	morninghandler.h
	C++ header file for Qopo Morning Shower device
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
*/
#ifndef _MORNINGHANDLER_H
#define _MORNINGHANDLER_H

// C++ libraries
#include <StandardCplusplus.h>
#include <string>
#include <vector>


// Hardware specific libraries
#include <DS1307RTC.h>

#include "message.h"
#include "helpers.h"

typedef DS1307RTC Clock;

class MorningHandler
{
	public:
		MorningHandler();
		void setup();

		// Commands
		int alarm(const std::vector<std::string> &args);
		int setAlarm(const std::vector<std::string> &args);
		int time(const std::vector<std::string> &args);
		int reset(const std::vector<std::string> &args);
		int forget(const std::vector<std::string> &args);
		int energyLight(const std::vector<std::string> &args);
		int setting(const std::vector<std::string> &args);

	protected:
		// This method sends bits to the shift register:
		void changeLedState(int onOff);
		// Method to adjust LEDs brightness
		void setLedsBrightness(int brightness);

		// TODO: Transform these two functions into helpers 
		int setAlarmTime(const std::vector<std::string> &args);
		int setTime(const std::vector<std::string> &args);
	private:
		// Communication with clock
		Clock *_rtc;
		// Number of LEDs used on the device
		int _ledsCount;
		//Pin connected to latch pin (ST_CP) of 74HC595
		int _latchPin;
		//Pin connected to pin 13(GND) of 74HC595
		int _analogPin;
		//Pin connected to clock pin (SH_CP) of 74HC595
		int _clockPin;
		//Pin connected to Data in (DS) of 74HC595
		int _dataPin;
};

#endif