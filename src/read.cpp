#include <Arduino.h>
#include "rfid.h"

void RFID::read_answer() 
{
  int bytes_count = 0;
  delay(100); // wait a little bit after sending the command
  bytes_count = scan_result();
  if (0 && debug) {
    Serial.print("\nbytes scanned:"); 
    Serial.print(bytes_count);
  }
}

int RFID::scan_result()
{
  int bytes_count = 0;
  unsigned char incomingByte;
  if (SerialRFID.available() > 0)
  {
    Last_rx_millis = millis();
    Active = "1";
    if (debug) Serial.print(F("\n\tREAD: "));
    int i = 0;
    while (SerialRFID.available() > 0)
    {
      incomingByte = SerialRFID.read();
      if (debug) {
        Serial.print(incomingByte, HEX);
        Serial.print(":");
      }
      receive_buffer[i] = incomingByte;
      i++;
      bytes_count++;
    }
    parse_response(i);
  }
  return bytes_count;
}