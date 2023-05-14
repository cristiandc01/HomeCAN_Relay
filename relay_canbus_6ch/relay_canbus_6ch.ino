/*
DCDOMOTICS - CANDOMOTIC PROJECT  REV 1 BETA
RELAY FIRMWARE 6CH

*/

/*
WARNING! THIS FIRMWARE REQUIRES FIRST CONFIGURATION USING SERIAL.

GUIDE: HOW TO ASSIGN ID TO THE DEVICE

The device requires the assignment of an id in the network. 
To do this, connect to pins PA10 and PA9 with a usb/serial converter and 
put the device in configmode by connecting pin PA0 to gnd for 10sec.

Release the connection when led 13 starts flashing rapidly.
When restarted, the config mode appears on the serial console and LED 13 flashes slowly.

Now type the command "set id X" where X is the id selected in DEC (range 0-2047).

To exit the config mode reconnect the PA0 pin to gnd or write "exit conf" in the console

*/



#include <arduino.h>
#include "STM32_CAN.h"
#include <EEPROM.h>
#define bluePillLED PC13

#define rele1 PA1
#define rele2 PA2
#define rele3 PA3
#define rele4 PA4
#define rele5 PA5
#define rele6 PA6

// tx frame setup #1

#define input1 PC14     // Pin for controlling relay1
#define in_conf PA0     // Pin for enter in config mode
uint32_t txDly = 10000;  // mSec

bool configMode = false;
int my_id = 0x101;
int id, fltIdx;
static CAN_message_t CAN_RX_msg, CAN_TX_msg;
int stato1, stato2, stato3, stato4, stato5, stato6;
bool awayMode, away1, away2, away3, away4, away5, away6, inAway, ep_update;
int away_id;


STM32_CAN Can(CAN1, DEF);  //Use PA11/12 pins for CAN1.
//STM32_CAN Can( CAN1, ALT );  //Use PB8/9 pins for CAN1.
//STM32_CAN Can( CAN1, ALT_2 );  //Use PD0/1 pins for CAN1.
//STM32_CAN Can( CAN2, DEF );  //Use PB12/13 pins for CAN2.
//STM32_CAN Can( CAN2, ALT );  //Use PB5/6 pins for CAN2
//STM32_CAN Can( CAN3, DEF );  //Use PA8/15 pins for CAN3.
//STM32_CAN Can( CAN3, ALT );  //Use PB3/4 pins for CAN3

void canISR() {
  if (Can.read(CAN_RX_msg)) {
    // Serial.print("Received a message with id: ");
    //Serial.println(id);

    if (CAN_RX_msg.id == my_id) {
      if (CAN_RX_msg.buf[0] == 0) {

        if (CAN_RX_msg.buf[1] == 1) {  // CHANNEL 1
          if (CAN_RX_msg.buf[2] == 0) {
            digitalWrite(rele1, 1);
            stato1 = 0;

            update_state1();

          } else {
            digitalWrite(rele1, 0);
            stato1 = 1;

            update_state1();
          }
        } else if (CAN_RX_msg.buf[1] == 2) {  // CHANNEL 2
          if (CAN_RX_msg.buf[2] == 0) {
            digitalWrite(rele2, 1);
            stato2 = 0;

            update_state2();

          } else {
            digitalWrite(rele2, 0);
            stato2 = 1;

            update_state2();
          }
        } else if (CAN_RX_msg.buf[1] == 3) {  // CHANNEL 3
          if (CAN_RX_msg.buf[2] == 0) {
            digitalWrite(rele3, 1);
            stato3 = 0;

            update_state3();

          } else {
            digitalWrite(rele3, 0);
            stato3 = 1;

            update_state3();
          }
        } else if (CAN_RX_msg.buf[1] == 4) {  // CHANNEL 4
          if (CAN_RX_msg.buf[2] == 0) {
            digitalWrite(rele4, 1);
            stato4 = 0;

            update_state4();

          } else {
            digitalWrite(rele4, 0);
            stato4 = 1;

            update_state4();
          }
        } else if (CAN_RX_msg.buf[1] == 5) {  // CHANNEL 5
          if (CAN_RX_msg.buf[2] == 0) {
            digitalWrite(rele5, 1);
            stato5 = 0;

            update_state5();

          } else {
            digitalWrite(rele5, 0);
            stato5 = 1;

            update_state5();
          }
        } else if (CAN_RX_msg.buf[1] == 6) {  // CHANNEL 6
          if (CAN_RX_msg.buf[2] == 0) {
            digitalWrite(rele6, 1);
            stato6 = 0;

            update_state6();

          } else {
            digitalWrite(rele6, 0);
            stato6 = 1;

            update_state6();
          }
        }
      } else if (CAN_RX_msg.buf[0] == 2) {
        update_all_state();
      }
    } 
  }
}

void setup() {
  Can.begin();
  Can.setBaudRate(250000);  //250KBPS
  Serial.begin(9600);
  pinMode(in_conf, INPUT_PULLUP);
  pinMode(input1, INPUT_PULLUP);
  EEPROM.get(1, my_id);


  configMode = EEPROM.read(0);
  Serial.print("SetupMode: ");
  Serial.println(configMode);
  Serial.print("my ID (HEX): 0x");
  Serial.println(my_id, HEX);
  Serial.print("my ID: ");
  Serial.println(my_id);
  Serial.print("away ID (HEX): 0x");
  Serial.println(away_id, HEX);
  Serial.print("away ID: ");
  Serial.println(away_id);
  pinMode(bluePillLED, OUTPUT);


  if (configMode == true) {

    configLoop();
  } else {
    //can.filterMask16Init(0, my_id, 0x7ff);  // filter bank 0, filter 0: allow ID = rxMsgID, mask = 0x7ff (must match)
    //TO DO
  }

  



  
    stato1 = EEPROM.read(11);
    stato2 = EEPROM.read(12);
    stato3 = EEPROM.read(13);
    stato4 = EEPROM.read(14);
    stato5 = EEPROM.read(15);
    stato6 = EEPROM.read(16);
  
  pinMode(rele1, OUTPUT);
  digitalWrite(rele1, !stato1);
  pinMode(rele2, OUTPUT);
  digitalWrite(rele2, !stato2);
  pinMode(rele3, OUTPUT);
  digitalWrite(rele3, !stato3);
  pinMode(rele4, OUTPUT);
  digitalWrite(rele4, !stato4);
  pinMode(rele6, OUTPUT);
  digitalWrite(rele5, !stato5);
  pinMode(rele6, OUTPUT);
  digitalWrite(rele6, !stato6);


  Serial.print("stato 1: ");
  Serial.println(stato1);
  Serial.print("stato 2: ");
  Serial.println(stato2);
  Serial.print("stato 3: ");
  Serial.println(stato3);
  Serial.print("stato 4: ");
  Serial.println(stato4);
  Serial.print("stato 5: ");
  Serial.println(stato5);
  Serial.print("stato 6: ");
  Serial.println(stato6);
  Serial.println("AWAY Configuration");
  Serial.print("away 1: ");
  Serial.println(away1);
  Serial.print("away 2: ");
  Serial.println(away2);
  Serial.print("away 3: ");
  Serial.println(away3);
  Serial.print("away 4: ");
  Serial.println(away4);
  Serial.print("away 5: ");
  Serial.println(away5);
  Serial.print("away 6: ");
  Serial.println(away6);
  CAN_TX_msg.id = (my_id);
  update_all_state();
}

uint32_t last = 0;
void loop() {
  canISR();
  if (millis() / txDly != last) {
    //update_all_state();
    last = millis() / txDly;

    if (ep_update) {
      EEPROM.update(11, stato1);
      EEPROM.update(12, stato2);
      EEPROM.update(13, stato3);
      EEPROM.update(14, stato4);
      EEPROM.update(15, stato5);
      EEPROM.update(16, stato6);
      ep_update = false;
    }
  }

  //CONFIG MODE ENTER CODE
  if (!digitalRead(in_conf)) {
    int cnt = 0;
    while (!digitalRead(in_conf)) {
      delay(1000);
      cnt++;
      if (cnt > 5) {

        configMode = 1;
        Serial.print("Going to ConfigMode");
        EEPROM.write(0, configMode);
        while (!digitalRead(in_conf)) {
          delay(100);
          digitalWrite(bluePillLED, 1);

          delay(100);
          digitalWrite(bluePillLED, 0);
        }
        Serial.print("Reboot....");
        HAL_NVIC_SystemReset();
      }
    }
    if (digitalRead(in_conf)) {
      cnt = 0;
    }
  }

  if (digitalRead(input1) == 0) {
    if (stato1 == 0) {
      digitalWrite(rele1, 0);
      stato1 = 1;
      update_state1();
      update_all_state();
      delay(300);
      while (digitalRead(input1) == 0) {}
    } else {
      digitalWrite(rele1, 1);
      stato1 = 0;
      update_state1();
      update_all_state();
      delay(300);
      while (digitalRead(input1) == 0) {}
    }
  }
  delay(100);
}


void update_all_state() {

  CAN_TX_msg.len = 7;
  CAN_TX_msg.buf[0] = 0x02;
  CAN_TX_msg.buf[1] = stato1;
  CAN_TX_msg.buf[2] = stato2;
  CAN_TX_msg.buf[3] = stato3;
  CAN_TX_msg.buf[4] = stato4;
  CAN_TX_msg.buf[5] = stato5;
  CAN_TX_msg.buf[6] = stato6;

  Can.write(CAN_TX_msg);
}

void update_state1() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x01;
  CAN_TX_msg.buf[2] = stato1;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state2() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x02;
  CAN_TX_msg.buf[2] = stato2;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state3() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x03;
  CAN_TX_msg.buf[2] = stato3;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state4() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x04;
  CAN_TX_msg.buf[2] = stato4;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state5() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x05;
  CAN_TX_msg.buf[2] = stato5;
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
void update_state6() {
  ep_update = true;


  CAN_TX_msg.buf[0] = 0x01;
  CAN_TX_msg.buf[1] = 0x06;
  CAN_TX_msg.buf[2] = stato6;
  
  CAN_TX_msg.len = 4;
  Can.write(CAN_TX_msg);
}
