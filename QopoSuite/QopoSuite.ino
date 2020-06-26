/* Qopo Full Working Sensors:
  - Temperature
  - Humidity
  - Light
  
  + LEDS
  + Clock
  
  April 2015
  
*/

#include <CapacitiveSensor.h>
#include "Tlc5940.h"

// this function will return the number of bytes currently free in RAM      
extern int  __bss_end; 
extern int  *__brkval; 
int freemem()
{ 
int free_memory; 
if((int)__brkval == 0) 
   free_memory = ((int)&free_memory) - ((int)&__bss_end); 
else 
   free_memory = ((int)&free_memory) - ((int)__brkval); 
return free_memory; 
}

// Pin for the LED
int LEDPin = 13;
// Pin to connect to your drawing
int capSensePin = 2;
// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
int touchedCutoff = 800;

CapacitiveSensor cs = CapacitiveSensor(4,2);

int readingLightLevel = 0;
bool readingLightOn = false;

void changeReadingLight()
{
//  readingLightOn = !readingLightOn;
//  if(readingLightOn)
//  {
//    Tlc.set(30, 4095);
//    Tlc.set(31, 4095);    
//  }
//   else{
//      Tlc.set(30, 0);
//      Tlc.set(31, 0);
//   }
  if(readingLightLevel == 3)
     readingLightLevel = 0;
  else
    readingLightLevel ++;
   Tlc.set(30, 4095/3*readingLightLevel);
   Tlc.update();
}

void setup(){
    Serial.begin(9600);
    Tlc.init(0);
    Tlc.clear();
    cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

void loop(){
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED

  long csTotal =  cs.capacitiveSensor(30); 
  if (csTotal > touchedCutoff){
    Serial.println(csTotal);
    changeReadingLight();
    delay(500);
  }
  
}


