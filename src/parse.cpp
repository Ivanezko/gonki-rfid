#include <Arduino.h>
#include "rfid.h"


void RFID::scan_result() {
  unsigned char incomingByte;
  if (SerialRFID.available () > 0) {
    Serial.println(F("\nRFID First byte from rfid!"));
    delay(5);
    Serial.println(F("\nRFID First byte from rfid - wait done!"));
    int i=0;
    while(SerialRFID.available() > 0)
    {
        incomingByte=SerialRFID.read();
        //if (i < 99) {
          Serial.print (incomingByte,HEX);
          receive_buffer[i] = incomingByte;
          Serial.print (":");
        //}
        i++;
    }
    Serial.println();
    Serial.print(F("RFID last byte from rfid! length:"));
    Serial.println(i);
    parse_response(i);
  }
}


String RFID::error_text(char c) {
  if (c == 0xF8) return "Antenna disconnected";
  if (c == 0xF9) return "Command execution error";
  if (c == 0x14) return "Fail to adjust the RF power";
  return "OK";
}


void RFID::parse_command(unsigned char *frame) {
    if (debug) Serial.print ("\nFRAME to parse:\n");
    for (byte i=0; i<frame[0]+1; i++) {
      if (debug) Serial.print (frame[i],HEX);
      if (debug) Serial.print (":");
    }
    if (debug) Serial.print ("\nFRAME to parse END\n");
    bool crcCheck = crc.check(frame, frame[0]+1);
    if (debug) {
      Serial.print ("\ncrc:\n");
      Serial.print(crcCheck);
      Serial.print ("\ncrc END\n");
    }

    if (!crcCheck) return;
    

    if (frame[2] == 0x21) {
      if (debug) {
        Serial.print("\nRFID info, status: \n");
        Serial.println(error_text(frame[3]));
        Serial.print("\nRFID info, status END\n");
      }
    }

    if (frame[2] == 0x2F) {
      if (debug) {
        Serial.print("\nRFID RF power set, status: ");
        Serial.println(error_text(frame[3]));
      }
    }


   if (frame[2] == 0xEE && frame[3] == 0x28) {
      Serial.print("\nRFID realtime status");
      Serial.print("\nRFID HeartbeatNo:"); Serial.println(frame[4], HEX);
      Serial.print("\nRFID Antenna1:"); Serial.println(frame[5], HEX);
      Serial.print("\nRFID Antenna2:"); Serial.println(frame[6], HEX);
      Serial.print("\nRFID Antenna3:"); Serial.println(frame[7], HEX);
      Serial.print("\nRFID Antenna4:"); Serial.println(frame[8], HEX);
    }

    if (frame[2] == 0xEE && frame[3] == 0x00) {
      if (debug) {
        Serial.print("\nRFID realtime tag found!");
        Serial.print("\nRFID Antenna:"); Serial.print(frame[4], HEX);
        Serial.print("\nRFID Length:"); Serial.print(frame[5], HEX);
        Serial.print("\nRFID RSSI:"); Serial.print(6+frame[5], HEX);
        
      }
      if (debug || debug_realtime) Serial.print("\nRFID Tag:");
      //unsigned char buf[4];
      //check if its OUR tag - should have 42 at start
      if (frame[6] == 0x04 && frame[7] == 0x02) {
        if (debug || debug_realtime) Serial.println (F("RFID correct tag detected"));
        for (int i=8; i<12; i++) {
          if (debug || debug_realtime) Serial.print (frame[i],HEX);
          if (debug || debug_realtime) Serial.print (":");
          current_bort[i-8] = frame[i];
        }
        Serial.println (F("RFID current bort:"));
        Serial.println (current_bort);
        if (debug) Serial.println (F("RFID prev bort:"));
        if (debug) Serial.println (last_brt);
        String brt = String(current_bort);
        if (last_brt != brt) {
          last_brt = brt;
          publish(current_bort);
        } else {
          Serial.println (F("RFID correct tag but same as previose - ignoring"));
        }
      } else {
        if (debug || debug_realtime) {
          Serial.print ("RFID wrong tag detected, no 42 prefix, ignoring!");
          for (int i=6; i<6+frame[5]; i++) {
            if (debug || debug_realtime) Serial.print (frame[i],HEX);
            if (debug || debug_realtime) Serial.print (":");
          }
        }
      }
      
    }

    if (frame[2] == 0x76) {
      if (debug) {
        Serial.print("\nRFID set mode, status:");
        Serial.println(error_text(frame[3]));
      }
    }

    if (frame[2] == 0x3F) {
      if (debug) {
        Serial.print("\nRFID set antennas multiplexing mode, status:");
        Serial.println(error_text(frame[3]));
      }
    }


    if (frame[2] == 0x40) {
      if (debug) {
        Serial.println("RFID Buzzer OFF");
      }
    }

    if (frame[2] == 0x92) {
      Serial.print("RFID temperature:");
      if (frame[4] == 0) Serial.print("-"); else Serial.print("+");
      Serial.println(frame[5]);
    }

    if (frame[2] == 0x01 && frame[3] == 0x01) {
      if (debug) Serial.println("Inventory tag command received");
    }

    if (frame[2] == 0x01 && frame[3] == 0x03) {
      if (debug) {
        Serial.print("\nRFID Tag:");
        for (int i=4; i<frame[0]-1-1; i++) {
          Serial.print (frame[i],HEX);
          Serial.print (":");
        }
      }
    }
    
}

void RFID::parse_response(int len) {
  unsigned char frame[1000];
  byte chunk_len=0;
  int i=0;
  
  /*if (debug) Serial.print ("\nRESPONSE:");
  for (i=0; i<len; i++) {
    if (debug) Serial.print (receive_buffer[i],HEX);
    if (debug) Serial.print (":");
  }*/

  for (i=0; i<len; i++) {
    if (chunk_len == 0) {
      chunk_len = receive_buffer[i]+1;
      //if (i+chunk_len <= len || chunk_len < 6 || chunk_len > 100) {
        for (int j=chunk_len; j>0; j--) {
          frame[chunk_len-j] = receive_buffer[i+chunk_len-j];
        }
        parse_command(frame);
      //} else {
      //  Serial.println("\nFrame length broken");
      //}
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