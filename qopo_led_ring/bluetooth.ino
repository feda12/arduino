#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLE_READPACKET_TIMEOUT         500   // Timeout in ms waiting to read a response

#define BLUEFRUIT_SWUART_RXD_PIN       8   // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN       6   // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN         7   // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN         -1   // Optional, set to -1 if unused
#define BLUEFRUIT_UART_MODE_PIN        12    // Set to -1 if unused

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN, BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

bool bluetooth_setup() {
    // Bluetooth setup
  while (!Serial);  // required for Flora & Micro
  delay(500);
  
  Serial.begin(115200);
  if(!ble.begin(false)) {
    return false;
  }
  ble.setMode(BLUEFRUIT_MODE_DATA);
  return true;
}
