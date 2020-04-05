#include "power.h"
#include "rfid/rfid.h"

unsigned long snooze_period = 10000;
unsigned long snooze_last_millis = 0;

bool POWER::status = false;

void POWER::setup()
{
    pinMode(POWER_PIN, OUTPUT);
    pinMode(WAKEUP_PIN, INPUT_PULLUP);
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
    if (digitalRead(WAKEUP_PIN) == HIGH) {// если не нажата кнопка пробуждения
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

    if (digitalRead(WAKEUP_PIN) == LOW) {
        POWER::wake();
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