#include <Arduino.h>
#include <SPI.h> 
// Файл с настройками для библиотеки RF24
#include <nRF24L01.h> 
// Библиотека RF24
#include <RF24.h> 

extern const int LED_RFID_PIN;

class RF24MOD
{
    private:
    static uint8_t buffer[30]; 
    public:
    static void setup();
    static void loop();
    static void send(char msg_type, String msg);
    static void listen();
};