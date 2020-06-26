//////////////////////////////////////////////////////////////////////////////////

// REMIXED BY: TECHBITAR (HAZIM BITAR)
// LICENSE: PUBLIC DOMAIN
// DATE: MAY 2, 2012
// CONTACT: techbitar at gmail dot com

#include "Wire.h"
#define DS1307_I2C_ADDRESS 0x68


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Stops the DS1307, but it has the side effect of setting seconds to 0
// Probably only want to use this for testing
/*void stopDs1307()
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(0x80);
  Wire.endTransmission();
}*/

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
void setDateDs1307(byte second,        // 0-59
                   byte minute,        // 0-59
                   byte hour,          // 1-23
                   byte dayOfWeek,     // 1-7
                   byte dayOfMonth,    // 1-28/29/30/31
                   byte month,         // 1-12
                   byte year)          // 0-99
{
   Wire.beginTransmission(DS1307_I2C_ADDRESS);
   Wire.write(0);
   Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
   Wire.write(decToBcd(minute));
   Wire.write(decToBcd(hour));      // If you want 12 hour am/pm you need to set
                                   // bit 6 (also need to change readDateDs1307)
   Wire.write(decToBcd(dayOfWeek));
   Wire.write(decToBcd(dayOfMonth));
   Wire.write(decToBcd(month));
   Wire.write(decToBcd(year));
   Wire.endTransmission();
}

// Gets the date and time from the ds1307
void getDateDs1307(byte *second,
          byte *minute,
          byte *hour,
          byte *dayOfWeek,
          byte *dayOfMonth,
          byte *month,
          byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}



int dataPin = 2; 
int clockPin = 3;
int latchPin = 4;
char INBYTE;
int  LED = 13; // LED on pin 13
int LEDCOUNT = 8;
byte data;
byte dataArray[10];

void setup() {
  Serial.begin(9600); 
  Wire.begin();
  pinMode(LED, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);  
  pinMode(latchPin, OUTPUT);
  
  //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
  dataArray[0] = 0xFF; //11111111
  dataArray[1] = 0xFE; //11111110
  dataArray[2] = 0xFC; //11111100
  dataArray[3] = 0xF8; //11111000
  dataArray[4] = 0xF0; //11110000
  dataArray[5] = 0xE0; //11100000
  dataArray[6] = 0xC0; //11000000
  dataArray[7] = 0x80; //10000000
  dataArray[8] = 0x00; //00000000
  dataArray[9] = 0xE0; //11100000
  
//  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  // Change these values to what you want to set your clock to.
  // You probably only want to set your clock once and then remove
  // the setDateDs1307 call.
//  second = 45;
//  minute = 10;
//  hour = 9;
//  dayOfWeek = 4;
//  dayOfMonth = 24;
//  month = 4;
//  year = 14;
//  setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void loop() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

  getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  while(!Serial.available());
  Serial.println("Press 1 to turn Arduino pin 13 LED ON or 0 to turn it OFF:");
  if (Serial.available() > 0){
    INBYTE = Serial.read();        // read next available byte
    if( INBYTE == '0' ){
      setLEDIntensity(0);
      digitalWrite(LED, LOW);
      digitalWrite(8, LOW);
    }  // if it's a 0 (zero) tun LED off
    if( INBYTE == '1' ){
         setLEDIntensity(255);
         digitalWrite(8, HIGH);
         digitalWrite(LED, HIGH); // if it's a 1 (one) turn LED on
    }
  }
  delay(50);
}

void setLEDIntensity(int intensity){
  for(int i = 0; i < LEDCOUNT; i++){
      registerWrite(i, intensity);
  }
}
  
void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = whichState;

  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);

}

