#include <Arduino.h>
#include "rfid.h"

void RFID::read_answer() 
{
  int bytes_count = 0;
  delay(150); // wait a little bit after sending the command
  bytes_count = scan_result();
  if (0 && RFID::debug) {
    Serial.print("\nbytes scanned:"); 
    Serial.print(bytes_count);
  }
}

void RFID::drain_answer() 
{
  if (RFID::debug) Serial.print(F("\n\tDRAIN ANSWER"));
  while (RFID::SerialRFID.available() > 0) {
    RFID::SerialRFID.read();
    // do nothing
  }
}

int RFID::scan_result()
{
  int bytes_count = 0;
  unsigned char incomingByte;
  if (RFID::SerialRFID.available() > 0)
  {
    RFID::Last_rx_millis = millis();
    Active = "1";
    if (RFID::debug) Serial.print(F("\n\tREAD: "));
    int i = 0;
    while (RFID::SerialRFID.available() > 0)
    {
      incomingByte = RFID::SerialRFID.read();
      if (RFID::debug) {
        Serial.print(incomingByte, HEX);
        Serial.print(":");
      }
      RFID::receive_buffer[i] = incomingByte;
      i++;
      if (i >= 199) {
        Serial.print(F("\n!!!!! scan buffer OVERFLOW!!!!!"));
        break;
      }
      bytes_count++;
      delay(2); // без этого выскакивает из цикла :(
    }
    parse_response(i);
  }
  return bytes_count;
}