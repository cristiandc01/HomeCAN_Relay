# CANBUS relay Firmware for STM32


This firmware allows you to create a configurable 6ch relay node controlled via canbus

## How it works?
Simply use the canbus line present on the stm32 chips connected to a transceiver to drive up to 6 outputs. All enriched with state saving in eeprom and serial programming of the node ID

## How to install and compile

This Firmware is writed in Arduino IDE using STM32duino (https://github.com/stm32duino/Arduino_Core_STM32).

Download this repo and open the .ino file using Arduino IDE.
Download and install STM32_CAN library.
Then select your bluepill board from dropdown menu and click "compile and upload".
When he's done follow the configuration guide below for setting the node correctly.



## Library used
The firmware requires STM32_CAN Library by pazi88 (https://github.com/pazi88/STM32_CAN)

## Configuration guide

The firmeare requires the assignment of an id in the network. To do this, connect to pins PA10 and PA9 with a usb/serial converter and put the device in configmode by connecting pin PA0 to gnd for 10sec. 
Release the connection when led 13 starts flashing rapidly.
When restarted, the config mode appears on the serial console and LED 13 flashes slowly.

Now type the command "set id X" where X is the id selected in DEC (range 0-2047).

To exit the config mode reconnect the PA0 pin to gnd or write "exit conf" in the console


## ToDo

My idea is to create an open source and economic home automation bus alternative to the current standards in use, usable stand-alone and interfaced with any home automation systems (like Home Assistant).

Currently the firmware can be installed on STM32 chips and is built to be subsequently reconfigured without the need to recompile the code.

Currently reconfiguration is only possible via Serial Monitor. In development I have the configuration via canbus and a program useful for reading and writing on the device (https://github.com/cristiandc01/NodeConfigurator)
