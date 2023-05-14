

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

  readSerial();
  configLoop();
}


void canConfig() {
  
}





void readSerial() {
  if (Serial.available()) {
    String val = Serial.readString();

    if (val.indexOf("set id") == 0) {   // assign a id to the device
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
    } else if (val == "id?") { //print id
      Serial.print(my_id);
    } else if (val == "typ?") { //print devicetype
      Serial.print("2");
    } else if (val == "away?") {
      Serial.println("AWAY Configuration");
      Serial.print("away?: ");
      Serial.println(awayMode);
      Serial.print("away ID: ");
      Serial.println(away_id);
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
    } else if (val == "resetAll") {   //restore device to factory settings
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
    } else if (val.indexOf("set awaych1") == 0) {// GLOBAL ON OFF ON NETWORK WORK IN PROGRESS
      String value = val.substring(12);
      if (value == "1") {
        away1 = 1;
        EEPROM.write(25, 1);
        Serial.print("AwayMode ch1 ON");
      } else {
        away1 = 0;
        EEPROM.write(25, 0);
        Serial.print("AwayMode ch1 OFF");
      }
    } else if (val.indexOf("set awaych2") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        away2 = 1;
        EEPROM.write(26, 1);
        Serial.print("AwayMode ch2 ON");
      } else {
        away2 = 0;
        EEPROM.write(26, 0);
        Serial.print("AwayMode ch2 OFF");
      }
    } else if (val.indexOf("set awaych3") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        away3 = 1;
        EEPROM.write(27, 1);
        Serial.print("AwayMode ch3 ON");
      } else {
        away3 = 0;
        EEPROM.write(27, 0);
        Serial.print("AwayMode ch3 OFF");
      }
    } else if (val.indexOf("set awaych4") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        away4 = 1;
        EEPROM.write(28, 1);
        Serial.print("AwayMode ch4 ON");
      } else {
        away4 = 0;
        EEPROM.write(28, 0);
        Serial.print("AwayMode ch4 OFF");
      }
    } else if (val.indexOf("set awaych5") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        away5 = 1;
        EEPROM.write(29, 1);
        Serial.print("AwayMode ch5 ON");
      } else {
        away5 = 0;
        EEPROM.write(29, 0);
        Serial.print("AwayMode ch5 OFF");
      }
    } else if (val.indexOf("set awaych6") == 0) {
      String value = val.substring(12);
      if (value == "1") {
        away6 = 1;
        EEPROM.write(30, 1);
        Serial.print("AwayMode ch6 ON");
      } else {
        away6 = 0;
        EEPROM.write(30, 0);
        Serial.print("AwayMode ch6 OFF");
      }
    } else if (val.indexOf("set awayid") == 0) {
      String value = val.substring(11);
      away_id = value.toInt();
      EEPROM.put(21, away_id);
      Serial.print("New ID= ");
      Serial.println(away_id);
    } else if (val.indexOf("set away") == 0) {
      String value = val.substring(9);
      if (value == "1") {
        awayMode = 1;
        EEPROM.write(20, 1);
        Serial.print("AwayMode ON");
      } else {
        awayMode = 0;
        EEPROM.write(20, 0);
        Serial.print("AwayMode OFF");
      }
    }
  }
}