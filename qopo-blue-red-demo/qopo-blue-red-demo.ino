#include "Tlc5940.h"
#include "tlc_fades.h"

#define OFF 0
#define RED 1
#define BLUE 2
#define WHITE 3

int incomingByte = 0;
const int ledsCount = 10;
const int nbChannels = ledsCount*3;
// Number of colors used for animating, higher = smoother and slower animation)
int numColors = 255;

  // The combination of numColors and animationDelay determines the
  // animation speed, I recommend a higher number of colors if you want
  // to slow down the animation. Higher number of colors = smoother color changing.
int animationDelay = 10; // number milliseconds before RGB LED changes to next color
int counter = 0;

int tlcVal(int val, int brightness)
{
  return (val*4095/255)*brightness/100;
}

void setLeadValue(int channel, int value = 255, int brightness = 100)
{
  Tlc.set(channel, tlcVal(value, brightness));
  Tlc.update();
}

// Take led number, and 0-255 rgb values
void setLedColor(int l, int r, int g, int b, int brightness = 100)
{
  Serial.print("Switching channels:");
  Serial.print(l*3);
  Serial.print(", ");
  Serial.print(l*3+1);
  Serial.print(", ");
  Serial.println(l*3+2);
  Tlc.set(l*3, tlcVal(r, brightness));
  Tlc.set(l*3+1, tlcVal(g, brightness));
  Tlc.set(l*3+2, tlcVal(b, brightness));
}

void setLedsColor(int r, int g, int b, int brightness = 100)
{
  Serial.print("Setting LEDs to brightness=");
  Serial.println(brightness);
  for(int i = 0; i < ledsCount; i ++)
    setLedColor(i,r,g,b,brightness);
  Tlc.update();
}

int channel = 0;
void setup()
{
  Serial.begin(9600);
  Tlc.init(0);
  Tlc.clear();
  Tlc.update();
  setLedsColor(0, 0, 255); 
}

void loop() {
  if (Serial.available() > 0)
  {
   // lecture des bytes arrivant :
   incomingByte = Serial.read(); 
    switch(incomingByte) {
     case RED:
      setLedsColor(255, 0, 0);
      break;
     case BLUE:
      setLedsColor(0, 0, 255);
      break;
     case WHITE:
       setLedsColor(255, 255, 255);
      break;
     case OFF:
      Tlc.clear();
      Tlc.update();
      break;
    } 
  }
}


