#include <Arduino.h>
#include "rfid.h"

void RFID::send_command(int cmd, unsigned char *payload, int len)
{
  unsigned char frame[20];
  frame[0] = len + 4;
  frame[1] = 0x00;
  frame[2] = cmd;
  for (int i = 0; i < len; i++)
  {
    frame[3 + i] = payload[i];
  }
  CRC16::sign(frame, frame[0] - 1);
  if (RFID::debug) Serial.print(F("\n\tWRITE: "));
  for (int i = 0; i < frame[0] + 1; i++)
  {
    if (RFID::debug_realtime)
    {
      Serial.print(frame[i], HEX);
      Serial.print(":");
    }
    RFID::SerialRFID.write(frame[i]);
  }
}

void RFID::get_info()
{
  if (RFID::debug) Serial.print(F("\ncommand get info"));
  unsigned char dt[0];
  send_command(0x21, dt, sizeof(dt));
}

void RFID::send_inventorytime()
{
  if (RFID::debug)
    Serial.print(F("\ncommand set inventory time"));
  unsigned char dt[1];
  dt[0] = (byte)0x01;
  send_command(0x25, dt, sizeof(dt));
}

void RFID::send_frequency()
{
  if (RFID::debug)
    Serial.print(F("\ncommand set frequency"));
  unsigned char dt[2];
  // EU
  //dt[0] = (byte)0b01001110;
  //dt[1] = (byte)0b00000000;
  // UA
  dt[0] = 0b01000110;
  dt[1] = 0b10000000;
  send_command(0x22, dt, sizeof(dt));
}

void RFID::send_heartbeat()
{
  if (RFID::debug)
    Serial.print(F("\ncommand set heartbeat"));
  unsigned char dt[1];
  dt[0] = (byte)0x81; // 7bit-1, rest 6 bits: (0 ~ 127)*30s
  send_command(0x78, dt, sizeof(dt));
}

void RFID::send_power()
{
  if (RFID::debug)
    Serial.print(F("\ncommand set power"));
  unsigned char dt[1];
  dt[0] = (byte)0x15; // power 0-30 (dec!), 0-0x1E
  send_command(0x2F, dt, sizeof(dt));
}

void RFID::send_antenna_multiplexing()
{
  if (RFID::debug)
    Serial.print(F("\ncommand set antennas multiplexing mode"));
  unsigned char dt[1];
  dt[0] = (byte)0x01; // enable all antennas - 0F
  send_command(0x3F, dt, sizeof(dt));
}

void RFID::send_realtime_mode_on()
{
  if (RFID::debug)
    Serial.print(F("\ncommand set realtime inventory params"));
  unsigned char dt2[5];
  /*ReadPauseTime: 1 byte, time break between 2 real time inventories.
  0x00 – 10ms;
  0x01 – 20ms;
  0x02 – 30ms;
  0x03 – 50ms;
  0x04 – 100ms.
  FliterTime: 1 byte, tag filtering time. 
  The valid value of this parameter is 0 ~ 255, corresponds to (0 ~ 255)*1s. 
  In real time inventory, if reader detects a particular tag for more than 1 time, reader will only upload tag information 
  of this tag once within the pre-defined filtering time. For FliterTime = 0, disable tag filtering function.
  */
  dt2[0] = (byte)0x00; // TagProtocol
  dt2[1] = (byte)0x00; // ReadPauseTime
  dt2[2] = (byte)0x01; // Filter (0 ~ 255)*1s
  dt2[3] = (byte)0x00; // QValue
  dt2[4] = (byte)0x00; // Session
  //dt2[5] = (byte)0x01; // MaskMem
  //dt2[6] = (byte)0x00; // MaskAdr
  //dt2[7] = (byte)0x00; // MaskLen
  //dt2[8] = (byte)0x00; // MaskData
  send_command(0x75, dt2, sizeof(dt2));
  
  if (RFID::debug)
    Serial.print(F("\ncommand set realtime inventory mode ON"));
  unsigned char dt[1];
  dt[0] = (byte)0x01; // 0-answering, 1-realtime,3-realtime(triggered)
  send_command(0x76, dt, sizeof(dt));
}

void RFID::send_realtime_mode_off()
{
  if (RFID::debug) Serial.print(F("\ncommand realtime inventory mode OFF"));
  unsigned char dt[1];
  dt[0] = (byte)0x00; // 0-answering, 1-realtime,3-realtime(triggered)
  send_command(0x76, dt, sizeof(dt));
}

void RFID::send_scan()
{ // tag inventory
  unsigned char dt[2];
  dt[0] = (byte)0x04;
  dt[1] = (byte)0xff;
  send_command(0x01, dt, sizeof(dt));
}

void RFID::send_speed()
{
  if (RFID::debug) Serial.print(F("\ncommand set speed"));
  unsigned char dt[1];
  dt[0] = (byte)0x05; // 00 - 9600, 01 - 19200, 02 - 38400, 05 - 57600, 06 - 115200
  send_command(0x28, dt, sizeof(dt));
}

void RFID::send_buzzer()
{
  if (RFID::debug) Serial.print(F("\ncommand buzzer"));
  unsigned char dt[1];
  dt[0] = (byte)0x01; // 0-disable, 1-enable
  send_command(0x40, dt, sizeof(dt));
}

void RFID::send_temp()
{
  if (RFID::debug) Serial.print(F("\ncommand get temp"));
  unsigned char dt[0];
  send_command(0x92, dt, 0);
}


void RFID::send_writetag(char bort[5]) {
  /*while (bort.length() < 4) {
    bort = "0" + bort;
  }*/
  send_realtime_mode_off();
  read_answer();
  if (RFID::debug) Serial.print(F("\nwrite tag"));
  unsigned char dt[11];
  dt[0] = (byte)0x03; // length in words(2bytes)
  dt[1] = (byte)0x00; //  pwd 
  dt[2] = (byte)0x00; //  pwd 
  dt[3] = (byte)0x00; //  pwd 
  dt[4] = (byte)0x00; //  pwd 
  dt[5] = (byte)0x04; // bort mark - "42" :)
  dt[6] = (byte)0x02; // bort mark - "42" :)

  for (byte i = 0; i < 4; i++) {
    dt[7+i] = bort[i];
  }

  send_command(0x04, dt, sizeof(dt));
  read_answer();
  if (RFID::debug) Serial.print(F("\nwrite tag END"));
  send_realtime_mode_on();
  read_answer();
}