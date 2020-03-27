#include <Arduino.h>
#include "rfid.h"



void RFID::send_command(int cmd, unsigned char *payload, int len) {
    unsigned char frame[20]; 
    frame[0] = len+4;
    frame[1] = 0x00;
    frame[2] = cmd;
    for (int i=0; i<len; i++) {
      frame[3+i] = payload[i];
    }
    crc.sign(frame, frame[0]-1);
    if (debug) Serial.print(".");
    for(int i = 0; i < frame[0]+1; i++)
    {
      if (0 && debug) {Serial.print(frame[i], HEX); Serial.print(":");}
      SerialRFID.write(frame[i]);   
    }
  }


void RFID::get_info() {
  Serial.println ("rfid get info");
  unsigned char dt[0];
  send_command(0x21, dt, sizeof(dt));
}

void RFID::send_inventorytime() {
  if (debug) Serial.println ("set inventory time");
  unsigned char dt[1];
  dt[0] = (byte)0x01; 
  send_command(0x25, dt, sizeof(dt));
}

void RFID::send_power() {
  if (debug) Serial.println ("set power");
  unsigned char dt[1];
  dt[0] = (byte)0x30; 
  send_command(0x2F, dt, sizeof(dt));
}

void RFID::send_antenna_multiplexing() {
  if (debug) Serial.println ("set antennas multiplexing mode");
  unsigned char dt[1];
  dt[0] = (byte)0x01; // enable all antennas
  send_command(0x3F, dt, sizeof(dt));
}

void RFID::send_realtime_mode_on() {
  if (debug) Serial.println ("set realtime inventory params");
  unsigned char dt2[5];
  dt2[0] = (byte)0x00; // TagProtocol 
  dt2[1] = (byte)0x00; // read pause
  dt2[2] = (byte)0x00; // Filter (0 ~ 255)*1s
  dt2[3] = (byte)0x00; // QValue
  dt2[4] = (byte)0x00; // Session
  //dt2[5] = (byte)0x01; // MaskMem
  //dt2[6] = (byte)0x00; // MaskAdr
  //dt2[7] = (byte)0x00; // MaskLen
  //dt2[8] = (byte)0x00; // MaskData
  send_command(0x75, dt2, sizeof(dt2));

  if (debug) Serial.println ("set realtime inventory mode ON");
  unsigned char dt[1];
  dt[0] = (byte)0x01; // 0-answering, 1-realtime,3-realtime(triggered)
  send_command(0x76, dt, sizeof(dt));
}


void RFID::send_realtime_mode_off() {
  if (debug) Serial.println ("set realtime inventory mode OFF");
  unsigned char dt[1];
  dt[0] = (byte)0x00; // 0-answering, 1-realtime,3-realtime(triggered)
  send_command(0x76, dt, sizeof(dt));
}


void RFID::send_scan() { // tag inventory
  unsigned char dt[2];
  dt[0] = (byte)0x04;
  dt[1] = (byte)0xff;
  send_command(0x01, dt, sizeof(dt));
}

void RFID::send_speed() {
  unsigned char dt[1];
  dt[0] = (byte)0x05; // 00 - 9600, 01 - 19200, 02 - 38400, 05 - 57600, 06 - 115200
  send_command(0x28, dt, sizeof(dt));
}

void RFID::send_buzzer() {
  unsigned char dt[1];
  dt[0] = (byte)0x01;
  send_command(0x40, dt, sizeof(dt));
}

void RFID::send_temp() {
  unsigned char dt[0];
  send_command(0x92, dt, 0);
}