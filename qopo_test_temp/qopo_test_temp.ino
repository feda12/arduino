#include <DHT.h>

dht DHT;


#define DHT22_PIN 5
#define LIGHT_PIN A0
void setup()
{
    Serial.begin(9600);
    Serial.println("DHT TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT_LIB_VERSION);
    Serial.println();
    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C) \tBrightness(%)");
}

void loop()
{
    // READ DATA
    Serial.print("DHT22, \t");
    int chk = DHT.read22(DHT22_PIN);
    switch (chk)
    {
        case DHTLIB_OK: 
            Serial.print("OK,\t"); 
            break;
        case DHTLIB_ERROR_CHECKSUM: 
            Serial.print("Checksum error,\t"); 
            break;
        case DHTLIB_ERROR_TIMEOUT: 
            Serial.print("Time out error,\t"); 
            break;
        default: 
            Serial.print("Unknown error,\t"); 
            break;
    }
  int lightLevel = analogRead(LIGHT_PIN);
  lightLevel = map(lightLevel, 0, 900, 0, 100); 
  lightLevel = constrain(lightLevel, 0, 100);
  lightLevel = 100-lightLevel;
    // DISPLAY DATA
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t");
    Serial.println(lightLevel);

    delay(1000);

}
