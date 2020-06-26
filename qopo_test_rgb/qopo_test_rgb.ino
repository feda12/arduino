#include "Tlc5940.h"
#include "tlc_fades.h"

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
  Tlc.update();
}

void setLedsColor(int r, int g, int b, int brightness = 100)
{
  Serial.print("Setting LEDs to brightness=");
  Serial.println(brightness);
  for(int i = 0; i < ledsCount; i ++)
    setLedColor(i,r,g,b,brightness);
}

int channel = 0;
void setup()
{
  Serial.begin(9600);
  Tlc.init(0);
  Tlc.clear();
  Tlc.update();
  setLedsColor(20, 20, 255);
}

void loop() {
//  if(channel <= nbChannels)
//  {
//    Tlc.clear();
//    setLeadValue(channel);
//    channel++;
//    Serial.print("Channel ");
//    Serial.println(channel);
//  } else {
//    channel = 0;
//  }
//  delay(5000);
  
//  delay(animationDelay);
//    uint32_t startMillis = millis() + 50;  // The 10 is added so the processer has time to do calculations and stuff before executing the add fade command
//    uint32_t endMillis = startMillis + 10000;  // the 3000 is your fade duration in milliseconds, so 3 seconds
//        for(int i = 0; i < 10; i ++)
//           tlc_addFade (i,0,4095,startMillis, endMillis);
//    tlc_updateFades();
}


