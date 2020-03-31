#include <Arduino.h>
#include "rfid.h"

void RFID::parse_command(unsigned char *frame)
{
  bool debug_parse=false;
  if (debug_parse)
    Serial.print("\nFRAME to parse:\n");
  for (byte i = 0; i < frame[0] + 1; i++)
  {
    if (debug_parse)
      Serial.print(frame[i], HEX); 
    if (debug_parse)
      Serial.print(":");
  }
  if (debug_parse)
    Serial.print("\nFRAME to parse END\n");
  bool crcCheck = crc.check(frame, frame[0] + 1);
  if (debug_parse)
  {
    Serial.print("\ncrc:\n");
    Serial.print(crcCheck);
    Serial.print("\ncrc END\n");
  }

  if (!crcCheck) {
    if (debug) Serial.print("\nCRC ERROR!\n");
    return;
  }
    
  if (frame[2] == 0x22)
  {
    if (debug)
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

    if (debug)
    {
      Serial.print(F("\n\tPARSED get info: "));
      //Serial.print("\nRFID info, status: \n");
      Serial.print(error_text(frame[3]));
      //Serial.print("\nRFID info, status END\n");
    }
  }

  if (frame[2] == 0x2F)
  {
    if (debug)
    {
      Serial.print(F("\n\tPARSED RF power set: "));
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0xEE && frame[3] == 0x28)
  {
    if (debug) Serial.print("\nRFID realtime status");
    if (debug) Serial.print("\nRFID HeartbeatNo:");
    Heartbeat_no = frame[4];
    Heartbeat_no = Heartbeat_no << 8 | frame[5];
    Heartbeat_no = Heartbeat_no << 8 | frame[6];
    Heartbeat_no = Heartbeat_no << 8 | frame[7];
    if (debug) Serial.print(Heartbeat_no);
    Antennas[0] = frame[8]+48;
    Antennas[1] = frame[9]+48;
    Antennas[2] = frame[10]+48;
    Antennas[3] = frame[11]+48;
    if (debug) Serial.print("\nRFID Antennas:");
    if (debug) Serial.print(Antennas); // 0x00 - idle, 0x01  - working, 0x02 - disconnected

    publish_status();
    
  }

  if (frame[2] == 0xEE && frame[3] == 0x00)
  {
    String current_bort = "____";
    if (debug)
    {
      Serial.print(F("\nRFID realtime tag found!"));
      Serial.print(F("\nRFID Antenna:"));
      Serial.print(frame[4], HEX);
      Serial.print(F("\nRFID Length:"));
      Serial.print(frame[5], HEX);
      Serial.print(F("\nRFID RSSI:"));
      Serial.print(6 + frame[5], HEX);
    }
    if (debug || debug_realtime)
      Serial.print(F("\nRFID Tag:"));
    //unsigned char buf[4];
    //check if its OUR tag - should have 42 at start
    if (frame[6] == 0x04 && frame[7] == 0x02)
    {
      if (debug_realtime)
        Serial.print(F("\nRFID correct tag detected:"));
      for (int i = 8; i < 12; i++)
      {
        if (debug || debug_realtime)
          Serial.print(frame[i], HEX);
        if (debug || debug_realtime)
          Serial.print(":");
          if (frame[i] >= 48) {
            current_bort[i - 8] = frame[i];
          } else {
            current_bort[i - 8] = '0';
          }
      }
      if (debug_realtime) Serial.print(F("\nRFID current bort:"));
      if (debug_realtime) Serial.print(current_bort);
      if (debug)
        Serial.print(F("\nRFID prev bort:"));
      if (debug)
        Serial.print(Last_brt);
      if (Last_brt != current_bort)
      {
        Last_brt = current_bort;
        publish_bort();
        Last_brt_duplicates = 0;
      }
      else
      {
        Last_brt_duplicates++;
        if (debug_realtime) Serial.print(F("\nduplicate tag - ignoring"));
      }
    }
    else
    {
      if (debug || debug_realtime)
      {
        Serial.print(F("\nPARSED wrong tag detected, no 42 prefix, ignoring!"));
        for (int i = 6; i < 6 + frame[5]; i++)
        {
          if (debug || debug_realtime)
            Serial.print(frame[i], HEX);
          if (debug || debug_realtime)
            Serial.print(":");
        }
      }
    }
  }

  if (frame[2] == 0x76)
  {
    if (debug)
    {
      Serial.print("\n\tPARSED realtime mode:");
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0x3F)
  {
    if (debug)
    {
      Serial.print(F("\n\tPARSED set antennas multiplexing mode: "));
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0x40)
  {
    if (debug)
    {
      Serial.print(F("\n\tPARSED Buzzer:"));
      Serial.print(error_text(frame[3]));
    }
  }

  if (frame[2] == 0x92)
  {
    if (debug) Serial.print(F("\n\tPARSED temperature:"));
    if (frame[4] == 0) {
      if (debug) Serial.print("-");
      Temp = "-" + String(frame[5]);
    } else {
      if (debug) Serial.print("+");
      Temp = "+" + String(frame[5]);
    }
    if (debug) Serial.print(frame[5]);
  }

  if (frame[2] == 0x01 && frame[3] == 0x01)
  {
    if (debug)
      Serial.print("Inventory tag command received");
  }

  if (frame[2] == 0x01 && frame[3] == 0x03)
  {
    if (debug)
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
  unsigned char frame[1000];
  byte chunk_len = 0;
  int i = 0;

  /*if (debug) Serial.print ("\nRESPONSE:");
  for (i=0; i<len; i++) {
    if (debug) Serial.print (receive_buffer[i],HEX);
    if (debug) Serial.print (":");
  }*/

  for (i = 0; i < len; i++)
  {
    if (chunk_len == 0)
    {
      chunk_len = receive_buffer[i] + 1;
      for (int j = chunk_len; j > 0; j--)
      {
        frame[chunk_len - j] = receive_buffer[i + chunk_len - j];
      }
      parse_command(frame);
    }
    chunk_len--;
  }

  // {15:0:1:3}: {1:1:C:E2:0:0:17:10:D:1}:{23:16}:
  // [0:75:AC:42:22:27:15:0:1:3:1:1:C:E2:/0
  // [0:0:17:10:D:1:23:16:0:75:AC:46:6:61:7:0:1:1:1:0:1E:4B:

  // empty
  // {7:0:1:1}:1:0:{1E:4B}:

  // buzzer
  // {5:0:40:FD}{0}:{7A:7}:

  // {7:0:1:1}:[1:0]:[1E:4B]:
  // {5:0:40:0}:{10:2B}:
  // {7:0:92:0}:[1:19]:{80:57}
  // {7:0:1:1}:[1:0]:{1E:4B}:
}