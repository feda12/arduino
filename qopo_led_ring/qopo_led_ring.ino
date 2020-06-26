// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library


void setup() {
  leds_setup();
  leds_lightup();
  
  
  bool bluetoothOn = bluetooth_setup();
  if(!bluetoothOn) {
    leds_flash(); 
  }
  leds_off();
}

void loop() {


}
