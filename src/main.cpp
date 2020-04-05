#include <Arduino.h>
#include "rfid/rfid.h"
#include "power/power.h"
#include "led/led.h"

volatile uint32_t newlines = 0UL;

extern const int RFID_TX = 9; // - 9(rx) on rfid
extern const int RFID_RX = 2; // to 10(tx) on rfid 
extern const int POWER_PIN = 4; // S on relay
extern const int LED_RFID_PIN = 5; // led to show power ond. resistor 10kOm to gnd
extern const int WAKEUP_RFID_PIN = 6; // button
extern const int WAKEUP_USB_PIN = 13; // send ground to this pin will wake the USB power supplier

void setup()
{
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println(F("===SETUP START V2.0.0"));

  LED::setup();
  LED::on();
  POWER::setup();
  RFID::setup(); // возможно и нефиг делать каждый раз setup? все настройки же запоминаются 

  Serial.println(F("===SETUP END V2.0.0"));
  Serial.println(F("\n\n===LOOP START\n\n"));
  
}

void loop()
{
  RFID::loop();
  POWER::loop();
  LED::loop();
}