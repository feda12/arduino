#define DEBUG 0 //set to 1 for debug mode
#define AX 0 //accelerometer pin X
#define AY 1 //accelerometer pin Y
#define AZ 2 //accelerometer pin Z
#define DELAY 1000
#define SENSITIVITY -1 //movement required to send a movement event
 
//storage for x,y,z values
int x, y, z;
//storing the old values to compare relation movement
int old_x, old_y, old_z;
//our 'relative motion'
int movementFactor;
 
void setup() {
  Serial.begin(9600);
  readSensors();
  saveValues();//initialize
}
 
void loop() {
 
   readSensors();
   determineMovementFactor();
 
   if (movementFactor > SENSITIVITY) { 
 
      if (DEBUG) {
         Serial.print("x = ");
         Serial.print(x);
         Serial.print(" | y = ");
         Serial.print(y);
         Serial.print(" | z = ");
         Serial.print(z);
         Serial.print(" | movement = ");
         Serial.println(movementFactor);
      } else {
         Serial.println(movementFactor);
      }
 
      saveValues();
      delay(DELAY);
   }
}
 
/**
 * All we are doing here is finding the greatest derivative from
 * the 3 axises and choosing that as our relative movement factor
 */
void determineMovementFactor() {
   movementFactor = abs(old_x - x);
   int temp = abs(old_y - y); //due to impl of max(), no inline functions
   movementFactor = movementFactor+temp;
   temp = abs(old_z - z);
   movementFactor = movementFactor+temp;
}
 
void readSensors() {
   x = analogRead(AX);
   y = analogRead(AY);
   z = analogRead(AZ);
}
 
void saveValues() {
   old_x = x;
   old_y = y;
   old_z = z;
}
