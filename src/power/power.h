#include <Arduino.h>

extern const int POWER_PIN;
extern const int WAKEUP_PIN;



class POWER
{
    private:
    

    public:
    static bool status;
    static void setup();
    static void loop();
    static void on();
    static void off();
    static void wake();

};
