#include <Arduino.h>
#include "SoftwareSerial.h"
#include "rfid.h"

RFID::RFID() {
  debug_realtime = true;
  debug=true;
  SerialRFID.begin(115200);

  Serial.println(F("RFID setup start"));
  if (debug) Serial.println(F("RFID debug mode"));
  delay(100);
  send_realtime_mode_off();
  send_speed();
  delay(100);
  SerialRFID.end();
  delay(100);
  SerialRFID.begin(57600);
  delay(100);
  send_realtime_mode_off();delay(100);scan_result();
  get_info();delay(100);scan_result();
  send_inventorytime();delay(100);scan_result();
  send_power();delay(100);scan_result();
  //send_buzzer();
  send_temp();delay(100);scan_result();
  send_antenna_multiplexing();delay(100);scan_result();
  get_info();delay(100);scan_result();

  send_realtime_mode_on();

  Serial.println(F("RFID setup end"));
}


void RFID::loop() {
  //send_scan();
  scan_result();
}

void RFID::publish(char* txt) {
  Serial.println(F("======================="));
  Serial.println(txt);
  Serial.println(F("======================="));
}















  

