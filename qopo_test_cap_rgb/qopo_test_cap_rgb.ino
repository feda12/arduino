#include "Tlc5940.h"
#include <CapacitiveSensor.h>

// Pin to connect to your drawing
int capSensePin = 2;
int touchedCutoff = 60;

CapacitiveSensor cs = CapacitiveSensor(4,2);

int delayTime = 20;

int brightnessVal(int val, int dime)
{
  return (val*4095/255)*dime/100;
}


// Take led number, and 0-255 rgb values
void setLedColor(int l, int r, int g, int b, int dime = 100)
{
//  Serial.print("Switching channels:");
//  Serial.print(l*3);
//  Serial.print(", ");
//  Serial.print(l*3+1);
//  Serial.print(", ");
//  Serial.println(l*3+2);
  Tlc.set(l*3, brightnessVal(r, dime));
  Tlc.set(l*3+1, brightnessVal(g, dime));
  Tlc.set(l*3+2, brightnessVal(b, dime));
}

int i = 0;
void setup()
{
  Tlc.init(0);
  Tlc.clear();
  Serial.begin(9600);
      cs.set_CS_AutocaL_Millis(0xFFFFFFFF);

}

void loop()
{
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  long csTotal =  cs.capacitiveSensor(30); 
  if (csTotal > touchedCutoff){
      Serial.println("TOUCHDOWN!");
      Tlc.clear();
      setLedColor(i%11, 255, 255, 255, 100);
      Tlc.update();
      i++;
  }
  delay(500);
}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//          how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  In order for this to work now,
// The pin should have a 1+Megaohm resistor pulling
//  it up to +5v.
uint8_t readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}


