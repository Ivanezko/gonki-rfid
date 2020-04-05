#include <Arduino.h>

extern const int LED_RFID_PIN;

class LED
{
    private:
    static bool status;
    static byte mode;

    public:
    static void setup();
    static void loop();
    static void on();
    static void off();
    static void show_bort_detected();
};