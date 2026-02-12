# SDS Application for Alif DevKit-E8 with SDSIO using Ethernet Interface

This application demonstrates how to test DSP and ML algorithms using the SDS framework.
It lets you record and play back real-world data streams on physical hardware, feeding
them to your algorithm for testing. The data streams are stored in SDS data files.

## Pre-Requisite

To run this example:

- Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) and run a Blinky example on the board to verify tool installation.
- Clone, build and install development version of the SDS-Framework pack from:
  - `https://github.com/ARM-software/SDS-Framework.git`
- Setup the [Python environment](https://arm-software.github.io/SDS-Framework/main/utilities.html#setup) for running the SDS Utilities.
- Clone, build and install development version of the Alif Ensemble CMSIS DFP pack from:
  - `git clone https://github.com/VladimirUmek/alif_ensemble-cmsis-dfp.git`

## Alif DevKit-E8

The [Alif DevKit-E8](https://www.keil.arm.com/boards/alif-semiconductor-devkit-e8-gen-1-2558a7b/)
features a dual-core Cortex-M55 each paired with an Ethos-U55 NPU. Ethos-U85 NPU is also available
on Alif Ensemble E8 devices.

Before using this SDS example on the board it is required to program the ATOC of the device using
the Alif SETOOLS.
Refer to the section [Usage](https://www.keil.arm.com/packs/ensemble-alifsemiconductor/overview/)
in the overview page of the Alif Semiconductor Ensemble DFP/BSP for information on how to setup
these tools.

In VS Code use the menu command **Terminal - Run Tasks** and execute:

- `"Alif: Install M55_HE or M55_HP debug stubs (single core configuration)"`

For Windows ensure that the Terminal default is Git Bash or PowerShell.

> Note:
>
> - Configure SW4 to position SE (Secure UART) to enable SETOOLS communication with the device.
> - Configure SW4 to position U4 (UART4) to see STDIO messages from the application.

## Projects

The `SDS.csolution.yml` application is pre-configured for [DevKit-E8](https://www.keil.arm.com/boards/alif-semiconductor-devkit-e8-gen-1-2558a7b/).
It contains two projects:

- **`DataTest.cproject.yml`**: Verifies the SDSIO interface on hardware.
- **`AlgorithmTest.cproject.yml`**: Verifies a user algorithm with recording and playback of SDS data files.

## Layer Type: Board and Layer Type: SDSIO

The board layer implements the Hardware Abstraction Layer (HAL) layer.
The SDSIO layer implements communication layer that communicates with SDSIO-Server.

- `Board/DevKit-E8_M55_HP/Board_HP.clayer.yml` provides board/device drivers
- `sdsio/network/sdsio_network.clayer.yml` provides the **Ethernet Interface** for SDS I/O communication interface

## Build Types

- **`DebugRec`**: Debug version of application used for recording of generated input data and results of simple checksum algorithm output data.
- **`DebugPlay`**: Debug version of application used for verification of SDS component, play back the previously recorded SDS file and verify results of simple checksum algorithm.
- **`ReleaseRec`**: Release version of application used for recording of generated input data and results of simple checksum algorithm output data.
- **`ReleasePlay`**: Release version of application used for verification of SDS component, play back the previously recorded SDS file and verify results of simple checksum algorithm.

> Note:
>
> Only difference between `Debug` and `Release` targets is compiler optimization level and debug information.

For more details, refer to the [SDS Template Application](https://arm-software.github.io/SDS-Framework/main/template.html).

## DataTest Project

The DataTest project allows you to verify the SDS I/O communication and it is recommended
to use this project first.

Begin by starting the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server).
Open Terminal and type `sdsio-server.py socket`.
Check [SDS Utilities](https://arm-software.github.io/SDS-Framework/main/utilities.html) configuration
if SDSIO-Server is not found.

**SDSIO-Server Output:**

```bash
>sdsio-server.py socket
Press Ctrl+C to exit.
Socket server listening on 192.168.4.102:505
```

> Note: IP address 192.168.4.102 serves as example - it will be different on your local network.

Now open CMSIS view in VS Code to build and run the project using the following steps:

1. Use **Manage Solution Settings** and select as Active Project **DataTest** with Build Type **DebugRec**.
2. Open `sdsio_config_socket.h` and configure SDSIO_SOCKET_SERVER_IP to the SDSIO server provided IP.
3. **Build solution** creates the executable image.
4. Connect the PRG USB (J3) of the DevKit-E8 and configure SW4 switch for SETOOLS (SE position).
5. If not already done, download debug stubs using `"Alif: Install M55_HE or M55_HP debug stubs (single core configuration)"` task
6. **Load application to target** to download the application to the board.
7. Configure SW4 for UART4 (U4 position) and use the VS Code **Serial Monitor** to observe the output below.
8. Connect the local network Ethernet cable to the J2 (RJ45) connector on the DevKit-E8.
9. Reset the board with RESET (SW1) button

```txt
Initializing sockets...
IP address: 192.168.4.103
Connection to SDSIO-Server at 192.168.4.102:5050 established
99% idle
99% idle
```

alternatively if SDSIO-Server is not reachable or not running you will see the output:

```txt
Initializing sockets...
ERROR: Network cable not connected.
SDS I/O Network interface initialization failed or 'sdsio-server socket' unavailable at 192.168.4.102:5050 !
Ensure that SDSIO-Server is running, then restart the application!
```

### Recording Test

To execute the **recording** test, just:

1. Press the joystick (SW2) on the board to start recording.
2. Press the joystick (SW2) again to stop recording.

**SDSIO-Server Output:**

```bash
>sdsio-server.py socket
Press Ctrl+C to exit.
Socket server listening on 192.168.4.102:5050
Ping received.
Record:   DataInput (.\DataInput.0.sds).
Record:   DataOutput (.\DataOutput.0.sds).
..............
Closed:   DataInput (.\DataInput.0.sds).
Closed:   DataOutput (.\DataOutput.0.sds).
```

**Serial Monitor Output:**

```txt
SDS recording (#0) started
98% idle
No object detected
No object detected
No object detected
SDS recording (#0) stopped
====
```

Each run records two files: `DataInput.<n>.sds` and `DataOutput.<0>.sds` in the directory
where SDSIO-Server was started. `<n>` is a sequential number.

#### Check SDS Files

The [SDS-Check](https://arm-software.github.io/SDS-Framework/main/utilities.html#sds-check)
utility verifies SDS files for consistency. For example:

```bash
>sds-check.py -s DataInput.0.sds
File Name         : DataInput.0.sds
File Size         : 2.212.000 bytes
Number of Records : 20
Recording Time    : 6 s
Recording Interval: 320 ms
Data Size         : 2.211.840 bytes
Data Block        : 110.592 bytes
Data Rate         : 345.600 byte/s
Max Jitter        : 1 ms, in record 4
Max Delta Time    : 321 ms, in record 4
Validation passed
```

### Playback Test

To execute the **playback** test, follow the steps below:

1. Use **Manage Solution Settings** and select as Active Project **DataTest** with Build Type **DebugPlay**.
2. Open `sdsio_config_socket.h` and configure SDSIO_SOCKET_SERVER_IP to the SDSIO server provided IP.
3. **Build solution** creates the executable image.
4. Connect the PRG USB (J3) of the DevKit-E8 and configure SW4 switch for SETOOLS (SE position).
5. If not already done, download debug stubs using `"Alif: Install M55_HE or M55_HP debug stubs (single core configuration)"` task
6. **Load application to target** to download the application to the board.
7. Configure SW4 for UART4 (U4 position) and use the VS Code **Serial Monitor** to observe the application output (STDIO).
8. Connect the local network Ethernet cable to the J2 (RJ45) connector on the DevKit-E8.
9. Reset the board with RESET (SW1) button
10. Press a joystick (SW2) on the board to start playback of `DataInput` and recording of `DataOutput`.
11. Wait for playback to finish, it will finish automatically when all data from `DataInput.0.sds` SDS file was played back.

The stream `DataInput.<n>.sds` is read back and the algorithm processes this data. The stream `DataOutput.<m>.sds` is written whereby `<m>` is the next available file index.

> Note:
>
> The playback implementation replays recordings as quickly as possible and does not
> account for timestamp data. During playback, the ML system receives the same recorded
> input data, so timing information is not relevant in this context.

**SDSIO-Server Output:**

```bash
>sdsio-server.py socket
Press Ctrl+C to exit.
Socket server listening on 192.168.4.102:5050
Ping received.
Playback: DataInput (.\DataInput.0.sds).
Record:   DataOutput (.\DataOutput.1.sds).
......
Closed:   DataInput (.\DataInput.0.sds).
Closed:   DataOutput (.\DataOutput.1.sds).
```

> Note:
>
> DataOutput file recorded during playback should be identical to one recorded during recording.

## AlgorithmTest Project

The AlgorithmTest project demonstrates real-world usage of the SDS Framework on an object detection ML model.

This project, when configured as **Recorder**:

- **Captures on-board camera stream** via SDS recording stream (DataInput.<n>.sds file)
- **Executes ML inference** using an object detection ML model
- **Captures algorithm output** via SDS recording stream (DataOutput.<n>.sds file)

Alternatively, when configured as **Player**:

- **Replays pre-recorded video stream** via SDS playback stream (DataInput.<n>.sds file)
- **Executes ML inference** using an object detection ML model
- **Captures algorithm output** via SDS recording stream (DataOutput.<m>.sds file)

### Key Components

**Video Frame Capture** (`sds_data_in_user.c`):

- Initializes on-board camera input stream using CMSIS vStream driver
- Captures video frames and processes frames for ML model input
- Provides frame data for SDS recording

**Algorithm Processing** (`sds_algorithm_user.cpp`):

- Initializes ML model and LCD display stream using CMSIS vStream driver
- Executes ML inference (pre-processing, inference, post-processing)
- Copies detection results to output buffer for SDS recording
- Displays frames on LCD with overlaid boxes using CMSIS vStream driver

One can use the **AlgorithmTest** project in a same way as **DataTest**. In VS Code, open
CMSIS view and use **Manage Solution Settings** to select **AlgorithmTest** as Active Project.

> Note:
>
> - connect MT9M114 camera to the J16 connector on the bottom of the DevKit-E8 board.
> - connect display to the J21 connector on the top of the DevKit-E8 board.
