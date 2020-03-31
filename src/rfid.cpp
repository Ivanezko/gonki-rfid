#include <Arduino.h>
#include "SoftwareSerial.h"
#include "rfid.h"

#define RFID_RX 2
#define RFID_TX 3
SoftwareSerial SerialRFID(2, 3, false);

unsigned long publish_status_period = 1000;
unsigned long publish_status_last = 0;

unsigned long rfid_last_activity_maximum = 40000; // 40 seconds no RX - means RFID gone away

RFID::RFID()
{
}

void RFID::setup()
{
  debug_realtime = false;
  debug = false;
  Serial.print(F("\n\n===============RFID setup start============="));

  SerialRFID.begin(115200);
  delay(100);
  send_realtime_mode_off();
  read_answer();
  send_speed();
  read_answer();
  SerialRFID.end();
  
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

  send_frequency();
  read_answer();

  send_inventorytime();
  read_answer();

  send_power();
  read_answer();

  send_temp();
  read_answer();

  send_antenna_multiplexing();
  read_answer();

  send_heartbeat();
  read_answer();

  get_info();
  read_answer();

  send_realtime_mode_on();
  read_answer();

  Serial.print(F("\n==============RFID setup end==============\n"));
}

void RFID::loop()
{
  scan_result();
  //send_writetag("25");
  //read_answer();
  //delay(10000);

  if (millis()-publish_status_last > publish_status_period) {
    publish_status_last = millis();
    publish_status();
  }

  if ((Active == "1") && (millis()-Last_rx_millis > rfid_last_activity_maximum)) {
    Active = "0";
    if (debug) Serial.print(F("\nRFID HAS GONE AWAY"));
    publish_status();
  }


}


