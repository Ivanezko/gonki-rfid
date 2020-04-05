#include <Arduino.h>
#include "rfid.h"


// send scanned tag somewhere
void RFID::publish_bort()
{
  Serial.print(F("\n======================= publish bort\n"));
  Serial.print(RFID::Last_brt);
  Serial.print(F("\n=======================\n"));
}

// show RFID alive by LED or buzzer or send to the server (every 30sec) AND on scan!
void RFID::publish_status()
{
  unsigned int sec = millis()/1000;
  Serial.print(F("\n====== publish status\n"));
  Serial.println(sec);
  Serial.println(Active);
  Serial.println(RFID::Heartbeat_no);
  Serial.println(Antennas);
  Serial.println(Temp);
  Serial.println(RFID::Last_brt);
  Serial.println(RFID::RFID::Last_brt_duplicates);
}