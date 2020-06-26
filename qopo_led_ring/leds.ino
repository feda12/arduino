#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            4

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible val ues.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

int delayval = 500; // delay for half a second

void leds_setup() {
    pixels.begin(); // This initializes the NeoPixel library.
}

void leds_lightup(int r=0, int g=0, int b=255, int delayval=500) {
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(r,g,b));
    pixels.show();
    delay(delayval);
  } 
}

void leds_set_color(int r, int g, int b) {
    for(int i=0;i<NUMPIXELS;i++){
      pixels.setPixelColor(i, pixels.Color(r,g,b));
    } 
    pixels.show();
}

void leds_off() {
  leds_set_color(0,0,0);
}

void leds_flash(int r=255, int g=0, int b=0, int duration=1000, int flash_duration=100) {
  leds_set_color(r,g,b);
  delay(flash_duration);
  leds_off();
}

void leds_wakeup(int time) {
  
}

void leds_sleep() {
  
}
