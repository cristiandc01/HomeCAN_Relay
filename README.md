# CANBUS relay Firmware for STM32


This firmware allows you to create a configurable 6ch relay node controlled via canbus

## How it works?
Simply use the canbus line present on the stm32 chips connected to a transceiver to drive up to 6 outputs. All enriched with state saving in eeprom and serial programming of the node ID

##The Protocol

The Protocol is described here: https://github.com/cristiandc01/dc_bus

## How to install and compile

To compile and load the firmware you need to use STM32CUBEIDE software.




## Configuration guide

The HomeCAN firmware has a configuration string called CODE STRING. this string represents the entire node configuration.

It can be changed using the CAN BUS using UtilityHomeCAN Software.

### Configuration via CANBUS


## The ConfigMode

To access and change the coding you need to enter configuration mode using the hardware button on the node.

`In this mode the Node no longer responds to channel command messages.`

The Node responds to global search messages with id: 0 and to messages relating to the display and modification of the configuration string.

## ToDo

My idea is to create an open source and economic home automation bus alternative to the current standards in use, usable stand-alone and interfaced with any home automation systems (like Home Assistant).

Currently the firmware can be installed on STM32 chips and is built to be subsequently reconfigured without the need to recompile the code.


