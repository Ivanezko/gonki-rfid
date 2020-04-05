#include "power.h"
#include "rfid/rfid.h"
#include "led/led.h"

unsigned long snooze_period = 10000;
unsigned long snooze_last_millis = 0;

unsigned long sleep_last_millis = 0;

unsigned long snooze_usb_period = 3000;
unsigned long snooze_usb_last_millis = 0;

extern const int WAKEUP_USB_PIN;


bool POWER::status = false;

void POWER::setup()
{
    pinMode(POWER_PIN, OUTPUT);
    pinMode(WAKEUP_USB_PIN, OUTPUT);
    pinMode(WAKEUP_RFID_PIN, INPUT_PULLUP);
    POWER::on();
}

void POWER::on()
{
    Serial.print(F("\nRFID POWER ON"));
    POWER::status = true;
    digitalWrite(POWER_PIN, HIGH);
}
void POWER::off()
{
    Serial.print("try OFF");
    if (digitalRead(WAKEUP_RFID_PIN) == HIGH) {// если не нажата кнопка пробуждения
        POWER::status = false;
        digitalWrite(POWER_PIN, LOW);
        Serial.print(F("\nRFID POWER OFF"));
        delay(1000); 
    }
}

void POWER::loop()
{
    //Serial.println(POWER::status);
    if (POWER::status && (millis()-snooze_last_millis > snooze_period)) {
        POWER::off();
    }

    if (digitalRead(WAKEUP_RFID_PIN) == LOW) {
        POWER::wake();
    }

    //wake USB
    if (millis()-snooze_usb_last_millis > snooze_usb_period) {
        digitalWrite(WAKEUP_USB_PIN, HIGH);
        delay(50);
        digitalWrite(WAKEUP_USB_PIN, LOW);
        snooze_usb_last_millis = millis();
    }

    // моргаем лениво когда спим
    if (!POWER::status && millis()-sleep_last_millis > 1000) {
        LED::on();
        delay(50);
        LED::off();
        sleep_last_millis = millis();
    }

}

void POWER::wake()
{
    Serial.println("\nWAKE!!!");
    snooze_last_millis = millis();
    if (!POWER::status) {
        POWER::on();
    }
}