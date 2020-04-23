#include <Arduino.h>
//#include "SoftwareSerial.h"
#include "rfid.h"

//SoftwareSerial SerialRFID(2, 3, false);

unsigned long publish_status_period = 1000;
unsigned long publish_status_last = 0;

unsigned long rfid_last_activity_maximum = 40000; // 40 seconds no RX - means RFID gone away

extern const int RFID_TX;
extern const int RFID_RX;

//SoftwareSerial RFID::SerialRFID = SoftwareSerial(3, 2);
//NeoICSerial RFID::SerialRFID;
NeoSWSerial RFID::SerialRFID(RFID_RX, RFID_TX);
String RFID::Last_brt = "";
int RFID::Last_brt_duplicates = 0;
unsigned long RFID::Last_rx_millis = 0;
bool RFID::RealtimeOn = false;
String RFID::Antennas = "____";
String RFID::Temp = "___";
bool RFID::debug = false;
bool RFID::debug_realtime = false;
bool RFID::debug_parse = false;
unsigned char RFID::receive_buffer[200];
unsigned long RFID::Heartbeat_no = 0;

void RFID::Realtime(bool active) {
  delay(50);
  if (active) {// очистим хвосты пакетов
    RFID::drain_answer();
  }
  RFID::RealtimeOn = active;
}

void RFID::setup()
{
  Serial.print(F("\n\n===============RFID setup start============="));

  //Serial.print(F("\n\nstart rfid tune speed"));
/*
  RFID::SerialRFID.begin(38400);
  delay(300);
  send_realtime_mode_off();
  drain_answer();
  send_speed();
  drain_answer();
  RFID::SerialRFID.end();

  RFID::SerialRFID.begin(19200);
  delay(300);
  send_realtime_mode_off();
  drain_answer();
  send_speed();
  drain_answer();
  RFID::SerialRFID.end();

  RFID::SerialRFID.begin(38400);
  delay(300);
  send_realtime_mode_off();
  drain_answer();
  send_speed();
  drain_answer();
  RFID::SerialRFID.end();

  RFID::SerialRFID.begin(57600);
  delay(300);
  send_realtime_mode_off();
  drain_answer();
  send_speed();
  drain_answer();
  RFID::SerialRFID.end();
*/
  /*RFID::SerialRFID.begin(115200);
  delay(300);
  send_realtime_mode_off();
  drain_answer();
  send_speed();
  drain_answer();
  RFID::SerialRFID.end();
  */

  Serial.print(F("\n\nstart rfid on normal speed"));

  RFID::SerialRFID.begin(19200);
  delay(1000);

  drain_answer();
  
  if (RFID::debug)
    Serial.print(F("\nRFID RFID::debug mode ON\n"));

/*
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
*/
  Serial.print(F("\n==============RFID setup end==============\n"));
}

void RFID::loop()
{
  if (RFID::RealtimeOn) {
    scan_result();
  }

  if (millis()-publish_status_last > publish_status_period) {
    publish_status_last = millis();
    publish_status();
  }

  if (RFID::RealtimeOn && (millis()-RFID::Last_rx_millis > rfid_last_activity_maximum)) {
    RFID::RealtimeOn = true;
    Serial.print(F("\nRFID DONT SEND ANYTHING FOR 40sec"));
    RFID::Last_rx_millis = millis();
    publish_status();
  }

  // get_info allowed in inventory mode.
  //get_info();
  //read_answer();

  serial_loop();
}



// tmp write bort
int8_t mode = 0;
char bort[5];
int8_t bort_char_pos = 0;

void RFID::serial_loop()
{
  if (Serial.available()) {
    while (Serial.available()) {
      unsigned char c = Serial.read();
      switch (mode)
      {
      case 0:
        if (c == 'w') {
          //Serial.print(F("\nwrite mode"));
          mode = 1;
          bort_char_pos = 0;
        }
        break;
      case 1:
        bort[bort_char_pos++] = c;
        break;
      default:
        break;
      }
    }
    if (mode == 1 && bort_char_pos == 4) {
      bort[4] = '\0';
      Serial.print(F("\nwrite bort:"));
      Serial.print(bort);
      send_writetag(bort);
      read_answer();
      mode = 0;
      delay(10000);
    }
    
  }
}




