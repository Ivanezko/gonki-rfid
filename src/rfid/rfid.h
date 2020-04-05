#include <Arduino.h>
//#include "SoftwareSerial.h"
#include <NeoICSerial.h>
#include "crc16/crc16.h"

extern const int RFID_TX;
extern const int RFID_RX;

class RFID
{

private:
  static NeoICSerial SerialRFID;
  static unsigned char receive_buffer[200];
  static bool debug;
  static bool debug_realtime;
  static bool debug_parse;
  static String Last_brt;
  static int Last_brt_duplicates;
  static unsigned long Heartbeat_no;
  static unsigned long Last_rx_millis;
  static String Antennas;
  static String Temp;

  static void send_command(int cmd, unsigned char *payload, int len);
  static String error_text(char c);
  static void read_answer();
  static void drain_answer();
  static void send_heartbeat();
  static void send_realtime_mode_on();
  static void send_realtime_mode_off();
  static void parse_command(unsigned char *frame);
  static void parse_response(int len);
  static int scan_result();
  static void get_info();
  static void send_inventorytime();
  static void send_frequency();
  static void send_power();
  static void send_antenna_multiplexing();
  static void send_scan();
  static void send_speed();
  static void send_buzzer();
  static void send_temp();
  static void send_writetag(char bort[5]);

  static String char_arr_to_string(char *arr);

  static void publish_bort();
  static void publish_status();

  static void serial_loop();

public:
  static String Active;
  static void setup();
  static void loop();
};