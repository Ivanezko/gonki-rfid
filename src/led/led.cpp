#include "led.h"
#include "power/power.h"

bool LED::status = false;
byte LED::mode = 0;

int bort_detection_sequence_curr = 0;
unsigned long bort_detection_base_millis = 0;

void LED::setup()
{
    pinMode(LED_RFID_PIN, OUTPUT);
}

void LED::loop()
{
    switch (LED::mode)
    {
    case 0:
        if (POWER::status && !LED::status) {// включаем LED
            LED::on();
        }
        if (!POWER::status && LED::status) {// выключаем LED
            LED::off();
        }
        break;
      case 1:
        if (millis()-bort_detection_base_millis < 100) {
                LED::off();
        } else {
            LED::mode=0;
            LED::on();
        }
        break;
  
    }
}

void LED::show_bort_detected()
{
    Serial.print(F("\nLED show bort detected"));
    bort_detection_base_millis = millis();
    LED::mode = 1;
}

void LED::on()
{
    LED::status = true;
    digitalWrite(LED_RFID_PIN, HIGH);
}

void LED::off()
{
    LED::status = false;
    digitalWrite(LED_RFID_PIN, LOW);
}
