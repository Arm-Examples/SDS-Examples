# SDS-Examples

This repository contains examples that show how to use the [Synchronous Data Streaming (SDS) Framework](https://github.com/ARM-software/SDS-Framework).

The SDS examples use the [MDK-Middleware](https://www.keil.arm.com/packs/mdk-middleware-keil/overview/) for the [SDSIO Interface](https://arm-software.github.io/SDS-Framework/main/sdsio.html). New hardware targets can added easily using board layers that provide with required [CMSIS-Driver interfaces]().

## Directory Structure

Directory                                                                        | Content
:--------------------------------------------------------------------------------|:---------------------------------------------------------
**[Hardware/SDS_examples.csolution.yml](./Hardware/SDS_examples.csolution.yml)** | **Collection of *csolution project* SDS examples running on boards**
[Hardware/DataTest](./Hardware/DataTest)                                         | Record test data to validate configuration and SDS interface bandwidth
[Hardware/Model1](./Hardware/Model1)                                             | Model1: Connects to a Gyroscope sensor
[Hardware/Model1/Test](./Hardware/Model1/Test)                                   | Model1: Record data with ML model and test data
[Hardware/Model1/Sensor](./Hardware/Model1/Sensor)                               | Model1: Record data with ML model and physical sensor data
[Hardware/Model1/Play](./Hardware/Model1/Play)                                   | Model1: Playback data to an ML model
[Hardware/Model2](./Hardware/Model2)                                             | Model2: Connects to a audio source
[Hardware/Model3](./Hardware/Model3)                                             | Model3: Connects to a video stream
**[Simulator/SDS_examples.csolution.yml](./Simulator/SDS_examples.csolution.yml)**  | **Collection of *csolution project* SDS examples running on AVH-FVP**
[Simulator/DataTest](./Simulator/DataTest)                                       | Record test data to validate configuration and SDS interface bandwidth
**[SDS_Interface](./SDS_Interface)**                                             | **SDS Interface implementations**
[SDS_Interface/FileSystem](./SDS_Interface/FileSystem)                           | SDS Interface (SDS_IO via file system)
[SDS_Interface/Network](./SDS_Interface/Network)                                 | SDS Interface (SDS_IO via network)
[SDS_Interface/USB](./SDS_Interface/USB)                                         | SDS Interface (SDS_IO via USB Device - VCOM)
[SDS_Interface/WiFi](./SDS_Interface/WiFi)                                       | SDS Interface (SDS_IO via WiFi)
[Boards](./Boards)                                                               | Various board layers for running SDS examples on different targets
[Sensors](./Sensors)                                                             | Interface to physical sensors
simulation
