#include <Arduino.h>
#include "SoftwareSerial.h"
#include "crc16.h"

class RFID
{

private:
  SoftwareSerial SerialRFID = SoftwareSerial(2, 3);
  unsigned char receive_buffer[100];
  bool debug;
  bool debug_realtime;
  String Last_brt;
  int Last_brt_duplicates = 0;
  unsigned long Heartbeat_no;
  unsigned long Last_rx_millis = 0;
  String Active = "0";
  String Antennas = "____";
  String Temp = "___";
  CRC16 crc = CRC16();

  void send_command(int cmd, unsigned char *payload, int len);
  String error_text(char c);
  void read_answer();
  void send_heartbeat();
  void send_realtime_mode_on();
  void send_realtime_mode_off();
  void parse_command(unsigned char *frame);
  void parse_response(int len);
  int scan_result();
  void get_info();
  void send_inventorytime();
  void send_frequency();
  void send_power();
  void send_antenna_multiplexing();
  void send_scan();
  void send_speed();
  void send_buzzer();
  void send_temp();
  void send_writetag(String bort);

  String char_arr_to_string(char *arr);

  void publish_bort();
  void publish_status();

public:
  RFID();
  void setup();
  void loop();
};