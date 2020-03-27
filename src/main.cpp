#include <Arduino.h>
#include "rfid.h"

RFID MyRF;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } 

  Serial.println(F("===SETUP START V1.0.0"));
  MyRF = RFID();
  
}

void loop() {
  MyRF.loop();

  

}