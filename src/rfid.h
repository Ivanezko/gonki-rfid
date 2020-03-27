#include <Arduino.h>
#include "SoftwareSerial.h"
#include "crc16.h"

class RFID {
  
  private:
    SoftwareSerial SerialRFID = SoftwareSerial(2,3);
    unsigned char receive_buffer[100];
    bool debug;
    bool debug_realtime;
    char current_bort[5];
    String last_brt;
    CRC16 crc = CRC16();
    

    void send_command(int cmd, unsigned char *payload, int len);
    String error_text(char c);
    void send_realtime_mode_on();
    void send_realtime_mode_off();
    void parse_command(unsigned char *frame);
    void parse_response(int len);
    void scan_result();
    void get_info();
    void send_inventorytime();
    void send_power();
    void send_antenna_multiplexing();
    void send_scan();
    void send_speed();
    void send_buzzer();
    void send_temp();

    void publish(char* txt);

  public:
    RFID();  
    void loop(); 
};