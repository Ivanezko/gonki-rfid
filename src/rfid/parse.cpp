#include <Arduino.h>
#include "rfid.h"
#include "power/power.h"
#include "led/led.h"

void RFID::parse_command(unsigned char *frame)
{
  if (RFID::debug_parse)
    Serial.print(F("\nFRAME to parse: "));
  for (byte i = 0; i < frame[0] + 1; i++)
  {
    if (RFID::debug_parse) {
      Serial.print(frame[i], HEX); 
      Serial.print(":");
    }
  }
  bool crcCheck = CRC16::check(frame, frame[0] + 1);

  if (!crcCheck) {
    if (RFID::debug_parse) Serial.print(F("\n================================================== CRC ERROR"));
    return;
  } else {
    if (RFID::debug_parse) Serial.print(F("\n================================================== CRC OK"));
  }
    
  if (frame[2] == 0x22)
  {
    if (RFID::debug)
    {
      Serial.print(F("\n\tPARSED set frequency: "));
      Serial.println(error_text(frame[3]));
      Serial.println(frame[2], HEX);
      Serial.println(frame[3], HEX);
    }
  }

  if (frame[2] == 0x21)
  {
    Serial.print(F("\nFirmware version:"));
    Serial.print(frame[4], HEX);
    Serial.print(frame[5], HEX);
    Serial.print(F("\nReader model code, 0x20 is UHFReader288MP:"));
    Serial.print(frame[6], HEX);
    Serial.print(F("\nSupported prototypes:"));
    Serial.print(frame[7], BIN);
    Serial.print(F("\ndmaxfre:"));
    Serial.print(frame[8], BIN);
    Serial.print(F("\ndminfre:"));
    Serial.print(frame[9], BIN);
    Serial.print(F("\nOutput RF power (0-30):"));
    Serial.print(frame[10], HEX);
    Serial.print(F("\nInventory time:"));
    Serial.print(frame[11], HEX);
    Serial.print(F("\nAntenna configuration:"));
    Serial.print(frame[12], BIN);
    Serial.print(F("\nAntenna check configuration:"));
    Serial.print(frame[15], BIN);

    if (RFID::debug)
    {
      Serial.print(F("\n\tPARSED get info: "));
      //Serial.print("\nRFID info, status: \n");
      Serial.print(error_text(frame[3]));
      //Serial.print("\nRFID info, status END\n");
    }
  }

  if (frame[2] == 0x2F)
  {
    if (RFID::debug)
    {
      Serial.print(F("\n\tPARSED RF power set: "));
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0xEE && frame[3] == 0x28)
  {
    if (RFID::debug) Serial.print("\nRFID realtime status");
    if (RFID::debug) Serial.print("\nRFID HeartbeatNo:");
    RFID::Heartbeat_no = frame[4];
    RFID::Heartbeat_no = RFID::Heartbeat_no << 8 | frame[5];
    RFID::Heartbeat_no = RFID::Heartbeat_no << 8 | frame[6];
    RFID::Heartbeat_no = RFID::Heartbeat_no << 8 | frame[7];
    if (RFID::debug) Serial.print(RFID::Heartbeat_no);
    Antennas[0] = frame[8]+48;
    Antennas[1] = frame[9]+48;
    Antennas[2] = frame[10]+48;
    Antennas[3] = frame[11]+48;
    if (RFID::debug) Serial.print("\nRFID Antennas:");
    if (RFID::debug) Serial.print(Antennas); // 0x00 - idle, 0x01  - working, 0x02 - disconnected

    publish_status();
    
  }

  if (frame[2] == 0xEE && frame[3] == 0x00)
  {
    POWER::wake();
    LED::show_bort_detected();
    String current_bort = "____";
    if (RFID::debug_realtime)
    {
      Serial.print(F("\nRFID realtime tag found!"));
      Serial.print(F("\nRFID Antenna:"));
      Serial.print(frame[4], HEX);
      Serial.print(F("\nRFID Length:"));
      Serial.print(frame[5], HEX);
      Serial.print(F("\nRFID RSSI:"));
      Serial.print(6 + frame[5], HEX);
    }
    if (RFID::debug_realtime)
      Serial.print(F("\nRFID Tag:"));
    //unsigned char buf[4];
    //check if its OUR tag - should have 42 at start
    if (frame[6] == 0x04 && frame[7] == 0x02)
    {
      if (RFID::debug_realtime)
        Serial.print(F("\nRFID correct tag detected:"));
      for (int i = 8; i < 12; i++)
      {
        if (RFID::debug_realtime) {
          Serial.print(frame[i], HEX);
          Serial.print(":");
        }
          if (frame[i] >= 48) {
            current_bort[i - 8] = frame[i];
          } else {
            current_bort[i - 8] = '0';
          }
      }
      if (RFID::debug_realtime) Serial.print(F("\nRFID current bort:"));
      if (RFID::debug_realtime) Serial.print(current_bort);
      if (RFID::debug_realtime) {
        Serial.print(F("\nRFID prev bort:"));
        Serial.print(RFID::Last_brt);
      }
      if (RFID::Last_brt != current_bort)
      {
        RFID::Last_brt = current_bort;
        publish_bort();
        RFID::RFID::Last_brt_duplicates = 0;
      }
      else
      {
        RFID::RFID::Last_brt_duplicates++;
        if (RFID::debug_realtime) Serial.print(F("\nduplicate tag - ignoring"));
      }
    }
    else
    {
      if (RFID::debug_realtime)
      {
        Serial.print(F("\nPARSED wrong tag detected, no 42 prefix, ignoring!"));
        for (int i = 6; i < 6 + frame[5]; i++)
        {
          Serial.print(frame[i], HEX);
          Serial.print(":");
        }
      }
    }
  }

  if (frame[2] == 0x76)
  {
    if (RFID::debug)
    {
      Serial.print("\n\tPARSED realtime mode:");
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0x3F)
  {
    if (RFID::debug)
    {
      Serial.print(F("\n\tPARSED set antennas multiplexing mode: "));
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0x40)
  {
    if (RFID::debug)
    {
      Serial.print(F("\n\tPARSED Buzzer:"));
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0x92)
  {
    if (RFID::debug) Serial.print(F("\n\tPARSED temperature:"));
    if (frame[4] == 0) {
      if (RFID::debug) Serial.print("-");
      Temp = "-" + String(frame[5]);
    } else {
      if (RFID::debug) Serial.print("+");
      Temp = "+" + String(frame[5]);
    }
    if (RFID::debug) Serial.print(frame[5]);
  }

  if (frame[2] == 0x01 && frame[3] == 0x01)
  {
    if (RFID::debug)
      Serial.print("Inventory tag command received");
  }

  if (frame[2] == 0x01 && frame[3] == 0x03)
  {
    if (RFID::debug)
    {
      Serial.print("\nRFID Tag:");
      for (int i = 4; i < frame[0] - 1 - 1; i++)
      {
        Serial.print(frame[i], HEX);
        Serial.print(":");
      }
    }
  }
}

void RFID::parse_response(int len)
{
  // correct realtime: RESPONSE[15]:E:0:EE:0:1:6:4:2:30:30:34:35:4B:96:8A:
  unsigned char frame[200];
  int i = 0;

  if (RFID::debug) {
    Serial.print ("\nRESPONSE[");
    Serial.print (len);
    Serial.print ("]:");
  }
  for (i=0; i<len; i++) {
    if (RFID::debug) Serial.print (RFID::receive_buffer[i],HEX);
    if (RFID::debug) Serial.print (":");
  }

  for (int index = 0; index < len; index++) // перебираем все байты
  {
    unsigned char frame_len = RFID::receive_buffer[index];
    if (index + frame_len + 1 > len) {// слишком длинный фрейм
      if (RFID::debug) Serial.print(F("\nerror: frame len is too big, skip the rest of the buffer!"));
      return;
    } else {
      for (int j = index; j < index+frame_len+1; j++) {
        frame[j-index] = RFID::receive_buffer[index+j];
      }
      parse_command(frame);
    }
    index = index + frame_len + 1;
  }

}