// Brunch.ino
#include "Prot.h"
#include "Mess.h"

#include "Debug.h"

// RTC DS1307chip libraries
#include <SoftI2C.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <TimeAlarms.h>

// Bluetooth serial
#include <SoftwareSerial.h>
#define bluetoothTx 4
#define bluetoothRx 5
SoftwareSerial btSerial(bluetoothTx, bluetoothRx);

#define ledsCount 8
#define latchPin 8
#define analogPin 9
#define clockPin 12
#define dataPin 11

SoftI2C bus(A2,A3);
DS1307RTC rtc(bus, 0);

void setup() {
	btSerial.begin(9600);
#ifdef __DEBUG
	// USB serial is set up only if in debug mode
	// Serial.begin(9600);
#endif


	prot_setup( &btSerial );
	// initialisation de la tâche de distribution des messages
	mess_setup();

	pinMode(latchPin, OUTPUT);
	pinMode(analogPin, OUTPUT); 
	pinMode(dataPin, OUTPUT);  
	pinMode(clockPin, OUTPUT);
	changeLedState(LOW);
	btSerial.println("Hello bt");
	DEBUGLN("Connected");
}

void loop() {
	prot_loop();
	// Distribution des messages
	if ( prot_isMessageAvailable() )
		mess_dispatch();
}

void changeLedState(int onOff) {
	// the bits you want to send
	byte bitsToSend = 0;

	// turn off the output so the pins don't light up
	// while you're shifting bits:
	digitalWrite(latchPin, LOW);
	for(int pin = 0; pin < ledsCount; pin++) 
		// turn on the next highest bit in bitsToSend:
		bitWrite(bitsToSend, pin, onOff);

	// shift the bits out:
	shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

	// turn on the output so the LEDs can light up:
	digitalWrite(latchPin, HIGH);
}

void setLedsBrightness(int brightness) // 0 to 100
{
	if(brightness > 100)
		brightness = 100;
	if(brightness < 0)
		brightness = 0;
	byte bright = (byte)(brightness/100*255);
	analogWrite(analogPin, 255-brightness);
}

int brunch_alarm(bool onOff = -1, int hours = -1, int minutes = -1)
{
	DEBUG("Brunching alarm. State:");
	DEBUG(onOff);
	DEBUG(", time ");
	DEBUG(hours);
	DEBUG(":");
	DEBUGLN(minutes);
	if(onOff)
	{
		// Set alarm time is alarm time is valid
		DEBUGLN("Setting alarm on");
	} else if(onOff == 0) {
		// Set alarm off no matter if the current time is valid
		DEBUGLN("Setting alarm off");
	} else {
		DEBUGLN("Alarm set at 07:30");
	}
}

int brunch_time(int hours = -1, int minutes = -1)
{

}
int brunch_energyLight(bool onOff, int intensity = -1, int duration = -1)
{

}
int brunch_setting(char *name, char * value = NULL)
{

}
int brunch_reset()
{
	
}