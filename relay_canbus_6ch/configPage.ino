

void configLoop() {
  delay(250);
  digitalWrite(bluePillLED, 1);
  if (my_id == -1) {
    Serial.print("NEW Device! Re-align EEPROM");
    EEPROM.put(1, 0);
    EEPROM.write(11, 0);
    EEPROM.write(12, 0);
    EEPROM.write(13, 0);
    EEPROM.write(14, 0);
    EEPROM.write(15, 0);
    EEPROM.write(16, 0);
    EEPROM.put(21, 0);
    EEPROM.write(20, 0);
    EEPROM.write(25, 0);
    EEPROM.write(26, 0);
    EEPROM.write(27, 0);
    EEPROM.write(28, 0);
    EEPROM.write(29, 0);
    EEPROM.write(30, 0);
    delay(5000);
    Serial.print("REBOOT");
    HAL_NVIC_SystemReset();
  }
  delay(250);
  digitalWrite(bluePillLED, 0);


  if (!digitalRead(in_conf)) {
    configMode = 0;
    Serial.print("Out from ConfigMode");
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
  canConfig();
  readSerialConfig();
  configLoop();
}


void canConfig() {  // CONFIG DEVICE VIA CAN BUS
  if (Can.read(CAN_RX_msg)) {

    if (CAN_RX_msg.id == 0) {        // in config mode always answer to id 0
      if (CAN_RX_msg.buf[0] == 1) {  // id set

        my_id = (CAN_RX_msg.buf[1] * 1000) + (CAN_RX_msg.buf[2] * 100) + (CAN_RX_msg.buf[3] * 10) + CAN_RX_msg.buf[4];

        EEPROM.put(1, my_id);
        Serial.print("New ID= ");
        Serial.println(my_id);

      }



      else if (CAN_RX_msg.buf[0] == 0) {  // main menu info set
        if (CAN_RX_msg.buf[1] == 1) {
          if (CAN_RX_msg.buf[2] == 0) {
            Serial.print("Out from ConfigMode");
            configMode = 0;
            EEPROM.write(0, configMode);

            Serial.print("Reboot....");
            HAL_NVIC_SystemReset();
          }
        } else if (CAN_RX_msg.buf[1] == 0) {

          CAN_TX_msg.id = (my_id);  // TRANSMIT DISCOVER PACKAGE IN DEVICE ID
          CAN_TX_msg.len = 3;
          CAN_TX_msg.buf[0] = 0x05;  // DISCOVER MESSAGE
          CAN_TX_msg.buf[1] = 0x02;  // DEVICE TYPE
          CAN_TX_msg.buf[2] = 0x06;  // CHANNEL NUMBER


          Can.write(CAN_TX_msg);
        }
      }


      else if (CAN_RX_msg.buf[0] == 2) {  // GBL MODE
        if (CAN_RX_msg.buf[1] == 0) {
          CAN_TX_msg.id = (0);  // TRANSMIT GBL MODE OUT CONFIGURATION
          CAN_TX_msg.len = 7;
          CAN_TX_msg.buf[0] = gblMode;  // gblMode State
          CAN_TX_msg.buf[1] = gbl1;     // gbl 1
          CAN_TX_msg.buf[2] = gbl2;     // gbl 2
          CAN_TX_msg.buf[3] = gbl3;     // gbl 3
          CAN_TX_msg.buf[4] = gbl4;     // gbl 4
          CAN_TX_msg.buf[5] = gbl5;     // gbl 5
          CAN_TX_msg.buf[6] = gbl6;     // gbl 6
          Can.write(CAN_TX_msg);
        }
        if (CAN_RX_msg.buf[1] == 1) {  // SAVE NEW GBLMODE OUT CONFIGURATION
          gbl1 = CAN_RX_msg.buf[2];
          gbl2 = CAN_RX_msg.buf[3];
          gbl3 = CAN_RX_msg.buf[4];
          gbl4 = CAN_RX_msg.buf[5];
          gbl5 = CAN_RX_msg.buf[6];
          gbl6 = CAN_RX_msg.buf[7];

          EEPROM.write(25, gbl1);
          EEPROM.write(26, gbl2);
          EEPROM.write(27, gbl3);
          EEPROM.write(28, gbl4);
          EEPROM.write(29, gbl5);
          EEPROM.write(30, gbl6);
        }
        if (CAN_RX_msg.buf[1] == 2) {  // GBL MODE SET ON OFF
          gblMode = CAN_RX_msg.buf[2];
          EEPROM.write(20, gblMode);
        }

        if (CAN_RX_msg.buf[1] == 3) {    // gbl id menu
          if (CAN_RX_msg.buf[2] == 0) {  // TRANSMIT GBL MODE ID
            CAN_TX_msg.id = (gbl_id);    // TRANSMIT DISCOVER PACKAGE IN DEVICE gbl ID
            CAN_TX_msg.len = 3;
            CAN_TX_msg.buf[0] = 0x05;  // DISCOVER MESSAGE
            CAN_TX_msg.buf[1] = 0x02;  // DEVICE TYPE
            CAN_TX_msg.buf[2] = 0x06;  // CHANNEL NUMBER


            Can.write(CAN_TX_msg);

          } else if (CAN_RX_msg.buf[2] == 1) {  // SAVE GBL MODE ID


            gbl_id = (CAN_RX_msg.buf[3] * 1000) + (CAN_RX_msg.buf[4] * 100) + (CAN_RX_msg.buf[5] * 10) + CAN_RX_msg.buf[6];

            EEPROM.put(21, gbl_id);
            Serial.print("New gbl ID= ");
            Serial.println(gbl_id);
          }
        }
      }

      else if (CAN_RX_msg.buf[0] == 3) {  // RELAY CONFIG
        if (CAN_RX_msg.buf[1] == 0) {     // GET CONFIG
          CAN_TX_msg.id = (0);            // TRANSMIT RELAY STATE
          CAN_TX_msg.len = 8;
          CAN_TX_msg.buf[0] = 0x03;  // RELAY CONFIG
          CAN_TX_msg.buf[1] = 0x00;  // GET CONFIG
          CAN_TX_msg.buf[2] = stato1;
          CAN_TX_msg.buf[3] = stato2;
          CAN_TX_msg.buf[4] = stato3;
          CAN_TX_msg.buf[5] = stato4;
          CAN_TX_msg.buf[6] = stato5;
          CAN_TX_msg.buf[7] = stato6;

          Can.write(CAN_TX_msg);
        } else if (CAN_RX_msg.buf[1] == 1) {
          stato1 = CAN_RX_msg.buf[2];
          stato2 = CAN_RX_msg.buf[3];
          stato3 = CAN_RX_msg.buf[4];
          stato4 = CAN_RX_msg.buf[5];
          stato5 = CAN_RX_msg.buf[6];
          stato6 = CAN_RX_msg.buf[7];

          EEPROM.write(11, stato1);
          EEPROM.write(12, stato2);
          EEPROM.write(13, stato3);
          EEPROM.write(14, stato4);
          EEPROM.write(15, stato5);
          EEPROM.write(16, stato6);
        }
      }
    }
  }
}





void readSerialConfig() {
  if (Serial.available()) {
    String val = Serial.readString();

    if (val.indexOf("set id") == 0) {  // assign a id to the device
      String value = val.substring(7);
      my_id = value.toInt();
      EEPROM.put(1, my_id);
      Serial.print("New ID= ");
      Serial.println(my_id);
    } else if (val == "exit conf") {  //exit from configmode
      configMode = 0;
      Serial.print("Out from ConfigMode");
      EEPROM.write(0, configMode);

      Serial.print("Reboot....");
      HAL_NVIC_SystemReset();
    } else if (val == "id?") {  //print id
      Serial.print(my_id);
    } else if (val == "typ?") {  //print devicetype
      Serial.print("2");
    } else if (val == "cfm?") {  //print configMode
      Serial.print("1");
    } else if (val == "gbl?") {
      Serial.println("GLOBAL Mode Configuration");
      Serial.print("gbMode?: ");
      Serial.println(gblMode);
      Serial.print("gb ID: ");
      Serial.println(gbl_id);
      Serial.print("gb 1: ");
      Serial.println(gbl1);
      Serial.print("gb 2: ");
      Serial.println(gbl2);
      Serial.print("gb 3: ");
      Serial.println(gbl3);
      Serial.print("gb 4: ");
      Serial.println(gbl4);
      Serial.print("gb 5: ");
      Serial.println(gbl5);
      Serial.print("gb 6: ");
      Serial.println(gbl6);
    } else if (val == "resetAll") {  //restore device to factory settings
      Serial.print("Reset ALL");
      EEPROM.put(1, 0);
      EEPROM.write(11, 0);
      EEPROM.write(12, 0);
      EEPROM.write(13, 0);
      EEPROM.write(14, 0);
      EEPROM.write(15, 0);
      EEPROM.write(16, 0);
      EEPROM.put(21, 0);
      EEPROM.write(20, 0);
      EEPROM.write(25, 0);
      EEPROM.write(26, 0);
      EEPROM.write(27, 0);
      EEPROM.write(28, 0);
      EEPROM.write(29, 0);
      EEPROM.write(30, 0);
      delay(5000);
      Serial.print("REBOOT");
      HAL_NVIC_SystemReset();
    } else if (val.indexOf("set gblch1") == 0) {  // GLOBAL ON OFF ON NETWORK WORK IN PROGRESS
      String value = val.substring(12);
      if (value == "1") {
        gbl1 = 1;
        EEPROM.write(25, 1);
        Serial.print("gblMode ch1 ON");
      } else {
        gbl1 = 0;
        EEPROM.write(25, 0);
        Serial.print("gblMode ch1 OFF");
      }
    } else if (val.indexOf("set gblch2") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        gbl2 = 1;
        EEPROM.write(26, 1);
        Serial.print("gblMode ch2 ON");
      } else {
        gbl2 = 0;
        EEPROM.write(26, 0);
        Serial.print("gblMode ch2 OFF");
      }
    } else if (val.indexOf("set gblch3") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        gbl3 = 1;
        EEPROM.write(27, 1);
        Serial.print("gblMode ch3 ON");
      } else {
        gbl3 = 0;
        EEPROM.write(27, 0);
        Serial.print("gblMode ch3 OFF");
      }
    } else if (val.indexOf("set gblch4") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        gbl4 = 1;
        EEPROM.write(28, 1);
        Serial.print("gblMode ch4 ON");
      } else {
        gbl4 = 0;
        EEPROM.write(28, 0);
        Serial.print("gblMode ch4 OFF");
      }
    } else if (val.indexOf("set gblch5") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        gbl5 = 1;
        EEPROM.write(29, 1);
        Serial.print("gblMode ch5 ON");
      } else {
        gbl5 = 0;
        EEPROM.write(29, 0);
        Serial.print("gblMode ch5 OFF");
      }
    } else if (val.indexOf("set gblch6") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        gbl6 = 1;
        EEPROM.write(30, 1);
        Serial.print("gblMode ch6 ON");
      } else {
        gbl6 = 0;
        EEPROM.write(30, 0);
        Serial.print("gblMode ch6 OFF");
      }
    } else if (val.indexOf("set gblid") == 0) {
      String value = val.substring(11);
      gbl_id = value.toInt();
      EEPROM.put(21, gbl_id);
      Serial.print("New ID= ");
      Serial.println(gbl_id);
    } else if (val.indexOf("set gbl") == 0) {
      String value = val.substring(9);
      if (value == "1") {
        gblMode = 1;
        EEPROM.write(20, 1);
        Serial.print("gblMode ON");
      } else {
        gblMode = 0;
        EEPROM.write(20, 0);
        Serial.print("gblMode OFF");
      }
    }
  }
}