/*
	Qopo.ino
	Arduino sketch for Qopo Morning Shower device
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved.
 */

// TODO for final version: squish it to a 1 line version for space saving?

// SoftwareSerial include
// Give the opportunity to have both USB and bluetooth running
#include <SoftwareSerial.h>
#include <StandardCplusplus.h>
#include <string>
#include <vector>

// Local includes
#include "debug.h"
#include "morninghandler.h"
#include "message.h"
#include "protocol.h"
#include <avr/pgmspace.h>

// Initializing bluetooth serial
#define bluetoothTx = 4;
#define bluetoothRx = 5;
SoftwareSerial btSerial(bluetoothTx, bluetoothRx);

#ifdef __DEBUG
Qopo::Protocol *usbProt;
Qopo::Message<MorningHandler> *usbMess;
#endif

Qopo::Protocol *btProt;
Qopo::Message<MorningHandler> *btMess;

//Function to calculate checksum
byte calcChecksum(char * p)
{
	byte ck = 0;
	for ( ; *p ; p++ )
		ck ^= (byte)(*p);
	return ck;
}

typedef Qopo::Message<MorningHandler> Morning;

void setup() {

#ifdef __DEBUG
	// USB serial is set up only if in debug mode
	Serial.begin(9600);
#endif
	// Setup MorningHandler
	MorningHandler morningHandler;
	morningHandler.setup();
	Morning::CommandList cmdList;
	Morning::CommandNames cmdNames;

	cmdList.push_back(&MorningHandler::alarm);
	cmdList.push_back(&MorningHandler::setAlarm);
	cmdList.push_back(&MorningHandler::time);
	cmdList.push_back(&MorningHandler::reset);
	cmdList.push_back(&MorningHandler::forget);
	cmdList.push_back(&MorningHandler::energyLight);
	cmdList.push_back(&MorningHandler::setting);

	cmdNames.push_back("a");
	cmdNames.push_back("t");
	cmdNames.push_back("r");
	cmdNames.push_back("f");
	cmdNames.push_back("e");
	cmdNames.push_back("s");
	
#ifdef __DEBUG
	usbProt = new Qopo::Protocol(&Serial, "usb");
	usbMess = new Qopo::Message<MorningHandler>(usbProt, &morningHandler, &cmdList, &cmdNames);
	// Serial.println(calcChecksum("mess,1,2"), HEX);
#endif
 	// Bluetooth serial
//	btSerial.begin(9600);
	// Setting up Bluetooth details

//	btProt = new Qopo::Protocol(&btSerial, "bluetooth");
//	btMess = new Qopo::Message<MorningHandler>(btProt, &morningHandler, &cmdList, &cmdNames);


	DEBUGLN(__DATE__);
	DEBUGLN(__TIME__);
	DEBUGLN("setup -- ready");
}

void loop() {
 	// If in debug mode, we process usb messages
	usbProt->loop();
	if(usbProt->isMessageAvailable()){
		usbMess->dispatch();
	}
 	// Bluetooth looping
	btProt->loop();
	if(btProt->isMessageAvailable())
		btMess->dispatch();
}


