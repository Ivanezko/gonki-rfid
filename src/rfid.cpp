#include <Arduino.h>
#include "SoftwareSerial.h"
#include "rfid.h"

#define RFID_RX 2
#define RFID_TX 3
SoftwareSerial SerialRFID(2, 3, false);


RFID::RFID()
{
}

void RFID::setup()
{
  debug_realtime = true;
  debug = true;
  Serial.print(F("\n\n===============RFID setup start=============\n\n"));

  
  SerialRFID.begin(57600);
  delay(100);
  
  if (debug)
    Serial.print(F("\nRFID debug mode ON\n"));

  send_realtime_mode_off();
  read_answer();

  //send_buzzer();
  //read_answer();

  //send_speed();
  //read_answer();

  get_info();
  read_answer();

  send_inventorytime();
  read_answer();

  send_power();
  read_answer();

  send_temp();
  read_answer();

  send_antenna_multiplexing();
  read_answer();

  get_info();
  read_answer();

  send_heartbeat();
  read_answer();

  send_realtime_mode_on();
  read_answer();

  Serial.print(F("\n\n==============RFID setup end==============\n\n"));
}

void RFID::loop()
{
  //send_scan();
  scan_result();
}


