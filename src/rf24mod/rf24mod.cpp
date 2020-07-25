#include <Arduino.h>
#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 
#include "rf24mod/rf24mod.h"
#include "power/power.h"

// 9 и 10 - это номера пинов CE и CSN модуля nRF24L01+
RF24 radio(9, 10); 

extern char DEVICE_GROUP;
extern char DEVICE_TYPE;

uint8_t RF24MOD::buffer[30] = {}; 

void RF24MOD::send(char msg_type, String msg) {
  uint8_t header_len = 5;
  //Serial.println(msg.length());
  uint8_t total_len = msg.length()+header_len;
  RF24MOD::buffer[0] = total_len; 
  RF24MOD::buffer[1] = uint8_t(DEVICE_GROUP); 
  RF24MOD::buffer[2] = uint8_t(DEVICE_TYPE); 
  RF24MOD::buffer[3] = msg_type;
  RF24MOD::buffer[4] = ';';
  for (int i = 0; i<total_len-header_len; i++) {
    RF24MOD::buffer[i+header_len] = msg[i];
    //Serial.println(msg[i], HEX);
  }
  Serial.print(F("\nSend by RF24: "));
  for (int i = 0; i<total_len; i++) {
    Serial.print(RF24MOD::buffer[i], HEX);
    Serial.print(":");
  }
  radio.stopListening();
  radio.openWritingPipe (0x1234567890LL + uint64_t(DEVICE_TYPE-48)); 
  radio.write(&RF24MOD::buffer, total_len); 
  RF24MOD::listen();
}

void RF24MOD::listen() {
  radio.openReadingPipe (1, 0x1234567891LL);
  radio.openReadingPipe (2, 0x1234567892LL);
  radio.openReadingPipe (3, 0x1234567893LL);
  radio.openReadingPipe (4, 0x1234567894LL);
  radio.openReadingPipe (5, 0x1234567895LL);
  radio.startListening();
}

void RF24MOD::setup(void)
{
  radio.begin(); 
  radio.setChannel(100); // 0-127
  radio.setDataRate (RF24_1MBPS); //RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setPALevel (RF24_PA_HIGH); // RF24_PA_MIN = -18dBm, RF24_PA_LOW = -12dBm, RF24_PA_HIGH = -6dBm, RF24_PA_MAX = 0dBm
  RF24MOD::listen();
}

void RF24MOD::loop(void) 
{
  if (radio.available()){ 
    // читаем данные    
    radio.read(&RF24MOD::buffer, sizeof(RF24MOD::buffer));
    Serial.print(F("\nReceive by RF24: ")); 
    for (int i=0; i<RF24MOD::buffer[0]; i++) {
      Serial.print(RF24MOD::buffer[i], HEX); 
      Serial.print(":");
    }

    // process wake signal
    if (RF24MOD::buffer[3] == '3') {
      Serial.print("WAKE");
      POWER::wake();
    }

  } 
}

