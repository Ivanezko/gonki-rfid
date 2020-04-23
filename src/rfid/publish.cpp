#include <Arduino.h>
#include "rfid.h"
#include "power/power.h"
#include "rf24mod/rf24mod.h"


// send scanned tag somewhere
void RFID::publish_bort()
{
  Serial.print(F("\n======================= publish bort\n"));
  Serial.print(RFID::Last_brt);
  RF24MOD::send('4', RFID::Last_brt);
  Serial.print(F("\n=======================\n"));
}

// show RFID alive by LED or buzzer or send to the server (every 30sec) AND on scan!
void RFID::publish_status()
{
  /*unsigned int sec = millis()/1000;
  Serial.print(F("\n====== publish status\n"));
  //Serial.println(sec);
  if (POWER::active) {
    Serial.println("1");
  } else {
    Serial.println("0");
  }
  Serial.println(RFID::Heartbeat_no);
  Serial.println(Antennas);
  Serial.println(Temp);
  Serial.println(RFID::Last_brt);
  Serial.println(RFID::Last_brt_duplicates);
  RF24MOD::send('4', RFID::Last_brt);
  */
}