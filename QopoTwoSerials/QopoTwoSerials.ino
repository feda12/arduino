/*
	Qopo Morning Shower Arduino Software
	Ben Le Cam - Qopo Inc.
	June 2014
	All rights reserved
 */

#include <SoftwareSerial.h>

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

SoftwareSerial btport(1,0);

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if (Serial.available() > 0) {
    // ASCII '0' through '9' characters are
    // represented by the values 48 through 57.
    // so if the user types a number from 0 through 9 in ASCII, 
    // you can subtract 48 to get the actual value:
    int serial = Serial.read() - 48;
    serial == '0' ? LOW : HIGH;
    changeLEDState(serial, 8);
  }
}

// This method sends bits to the shift register:

void changeLEDState(int whichState, int nbLeds) {
// the bits you want to send
  byte bitsToSend = 0;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);
  for(int pin = 0; pin < nbLeds; pin++) 
  // turn on the next highest bit in bitsToSend:
    bitWrite(bitsToSend, pin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}

