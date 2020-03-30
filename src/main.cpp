#include <Arduino.h>
#include "rfid.h"

RFID MyRF;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  delay(300);
  Serial.println(F("===SETUP START V2.0.0"));

  MyRF.setup();

}

void loop()
{
  //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  //Serial.print("\nasd");
  //delay(1000);
  MyRF.loop();
}