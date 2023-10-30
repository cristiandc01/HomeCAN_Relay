# HomeCAN Relay Firmware For STM32


This firmware allows you to create a configurable 6ch relay node controlled via canbus

## How it works?
Simply use the canbus line present on the stm32 chips connected to a transceiver to drive up to 6 outputs. All enriched with state saving in memory and 6 voltage input.

## The PCB

![full_board](https://github.com/cristiandc01/HomeCAN_Relay/blob/main/Images/full_board.jpeg)

This PCB is the first prototype. Uses the blue_pill development board and a tja1050 can transceiver.
For power the PCB requires 12V DC which is converted into 5V for the relays and 3.3V for the STM32.
6 voltage inputs are also available for controlling the outputs.

## The Protocol

The Protocol is described here: https://github.com/cristiandc01/dc_bus

## How to install and compile

To compile and load the firmware you need to use STM32CUBEIDE software.
Download this repo and open the project with the ide. 

Connect an stm32 (stm32f103c8t6) to the pc using the st-link tool. 
Then click the RUN button. the stm software will compile and upload the firmware to the chip.

You can also run the chip in debug mode.
## Configuration guide

The HomeCAN firmware has a configuration string called CODE STRING. this string represents the entire node configuration.

It can be changed using the CAN BUS using UtilityHomeCAN Software.

## The ConfigMode

To access and change the coding you need to enter configuration mode using the hardware button on the node.

`In this mode the Node no longer responds to channel command messages.`

The Node responds to global search messages with id: 0 and to messages relating to the display and modification of the configuration string.

## ToDo

My idea is to create an open source and economic home automation bus alternative to the current standards in use, usable stand-alone and interfaced with any home automation systems (like Home Assistant).

Currently the firmware can be installed on STM32 chips and is built to be subsequently reconfigured without the need to recompile the code.


