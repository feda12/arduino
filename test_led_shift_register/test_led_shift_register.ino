/*
  Shift Register Example
 for 74HC595 shift register

 This sketch turns reads serial input and uses it to set the pins
 of a 74HC595 shift register.

 Hardware:
 * 74HC595 shift register attached to pins 2, 3, and 4 of the Arduino,
 as detailed below.
 * LEDs attached to each of the outputs of the shift register

 Created 22 May 2009
 Created 23 Mar 2010
 by Tom Igoe

 */

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;
//Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;
// Pin connected to Analog of 74HC595
const int analogPin= 9;

const int ledsCount = 8;

void setup() {
	//set pins to output because they are addressed in the main loop
	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);  
	pinMode(clockPin, OUTPUT);
	pinMode(analogPin, OUTPUT);

	Serial.begin(9600);
	Serial.println("Testing for LEDs");
	changeLedState(HIGH);
	setLedsBrightness(100);
}

void loop() {

}

// This method sends bits to the shift register:
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
