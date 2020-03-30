#include <Arduino.h>
#include "rfid.h"


// send scanned tag somewhere
void RFID::publish(char *txt)
{
  Serial.print(F("\n=======================\n"));
  Serial.print(txt);
  Serial.print(F("\n=======================\n"));
}

// show RFID alive by LED or buzzer or send to the server (every 30sec)
void RFID::heartbeat()
{
  Serial.print(F("\n====== rfid alive!\n"));
}