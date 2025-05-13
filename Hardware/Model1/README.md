# About these examples

This folder contains 3 CMSIS Solution files each representing an example using a specific SDS Interface.

## SDS-FileSystem.csolution.yml

This CMSIS Solution represents **SDS Recording and Playback using File System Interface** on the embedded device
using the on-board Memory Card (SD Card) as media.

It is predefined for the STMicroelectronics **STM32H735G-DK** board, but can be easily retargeted to a
different board providing that a compatible layer is available.

## SDS-Network.csolution.yml

This CMSIS Solution represents **SDS Recording and Playback using Network (Ethernet) Interface** and it requires
SDSIO Server running on the Host machine.

It is predefined for the STMicroelectronics **STM32H735G-DK** board, but can be easily retargeted to a
different board providing that a compatible layer is available.

## SDS-USB.csolution.yml

This CMSIS Solution represents **SDS Recording and Playback using USB (Virtual COM Port) Interface** and it requires
SDSIO Server running on the Host machine.

It is predefined for the STMicroelectronics **B-U585I-IOT02A** board, but can be easily retargeted to a
different board providing that a compatible layer is available.

The B-U585I-IOT02A board is default in this example because it contains a vStream driver for the on-board accelerometer
required by the `RecordSensor` example.

## Running the example

Follow the steps below to test the example on hardware:

1.  open the folder containing `.csolution.yml` files in Visual Studio Code
2.  select desired Interface `.csolution` file as **Active Solution**
3.  select desired **Context**: board, project, build type
4.  build the application
5.  download the application to the board
6.  connect to the STDIO channel, for STMicroelectronics targets start the serial terminal application
    and connect to the `STMicroelectronics STLink Virtual COM Port (COMx)`
7.  for examples using Network or USB interface, start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server)
8.  reset the board
9.  press the VIO vioBUTTON0 push-button on the board to start the recording or playback, VIO vioLED1 will turn on
10. press the VIO vioBUTTON0 push-button on the board again to stop the recording or playback, VIO vioLED1 will turn off
11. observe the results in the STDIO channel
