#include "led.h"
#include "power/power.h"

bool LED::status = false;
byte LED::mode = 0;

int bort_detection_sequence[5] = { 100, 200, 300, 400, 500 }; // -+-+-
unsigned long bort_detection_base_millis = 0;

void LED::setup()
{
    pinMode(LED_PIN, OUTPUT);
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
        bool found = false;
        for (int i=4; i>=0; i--) {
            if (millis()-bort_detection_base_millis < bort_detection_sequence[i]) {
                    found = true;
                    if (LED::status) LED::off();
                    else LED::on();
                break;
            }
            if (!found) {
                LED::mode=1;
                LED::on();
            }
        }


        break;
  
    default:
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
    digitalWrite(LED_PIN, HIGH);
}

void LED::off()
{
    LED::status = false;
    digitalWrite(LED_PIN, LOW);
}
