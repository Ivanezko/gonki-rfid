#include <Arduino.h>
#include "rfid.h"


// send scanned tag somewhere
void RFID::publish_bort()
{
  Serial.print(F("\n======================= publish bort\n"));
  Serial.print(Last_brt);
  Serial.print(F("\n=======================\n"));
}

// show RFID alive by LED or buzzer or send to the server (every 30sec) AND on scan!
void RFID::publish_status()
{
  Serial.print(F("\n====== publish status\n"));
  Serial.println(millis()/1000);
  Serial.println(Active);
  Serial.println(Heartbeat_no);
  Serial.println(Antennas);
  Serial.println(Temp);
  Serial.println(Last_brt);
  Serial.println(Last_brt_duplicates);


}