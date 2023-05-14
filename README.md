# CANBUS relay Firmware for STM32


This firmware allows you to create a 6ch relay node controlled via canbus

## How it works?
Simply use the canbus line present on the stm32 chips connected to a transceiver to drive up to 6 outputs. All enriched with state saving in eeprom and serial programming of the node ID

## Library used
The firmware requires STM32_CAN Library by pazi88 (https://github.com/pazi88/STM32_CAN)

## Configuration guide

The firmeare requires the assignment of an id in the network. To do this, connect to pins PA10 and PA9 with a usb/serial converter and put the device in configmode by connecting pin PA0 to gnd for 10sec. 
Release the connection when led 13 starts flashing rapidly.
When restarted, the config mode appears on the serial console and LED 13 flashes slowly.

Now type the command "set id X" where X is the id selected in DEC (range 0-2047).

To exit the config mode reconnect the PA0 pin to gnd or write "exit conf" in the console
