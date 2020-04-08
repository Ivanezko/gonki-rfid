#include <Arduino.h>

extern const int POWER_PIN;
extern const int WAKEUP_RFID_PIN;



class POWER
{
    private:
    

    public:
    static bool active;
    static void setup();
    static void loop();
    static void on();
    static void off();
    static void wake();

};
