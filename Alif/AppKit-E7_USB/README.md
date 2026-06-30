# SDS Application for Alif AppKit-E7-AIML board with SDSIO using USB Interface

This application demonstrates how to test DSP and ML algorithms using the SDS framework.
It lets you record and play back real-world data streams on physical hardware, feeding
them to your algorithm for testing. The data streams are stored in SDS data files.

## Pre-Requisite

To run this example:

- Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) and run a Blinky example on the board to verify tool installation.
- Use `cpackget add ARM::SDS` to add the SDS Framework and [Setup the SDS Python utilities](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server).
- Setup the [Python environment](https://arm-software.github.io/SDS-Framework/main/utilities.html#setup) for running the SDS Utilities.
- Install Alif Ensemble CMSIS DFP pack v2.2.0 or later with:
  - `cpackget add AlifSemiconductor::Ensemble@2.2.0`

## Alif AppKit-E7-AIML board

The [Alif AppKit-E7-AIML](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-d1-34b5d51/projects/)
features a dual-core Cortex-M55 each paired with an Ethos-U55 NPU.

Before using this SDS example on the board it is required to program the ATOC of the device using the Alif SETOOLS.  
Refer to the section [Usage](https://www.keil.arm.com/packs/ensemble-alifsemiconductor/overview/)
in the overview page of the Alif Semiconductor Ensemble DFP/BSP for information on how to setup
these tools.

In VS Code use the menu command **Terminal - Run Tasks** and execute:

- `"Alif: Install M55_HE or M55_HP debug stubs (single core configuration)"`

For Windows ensure that the Terminal default is Git Bash or PowerShell.

> Note:
>
> - Configure J15 to position SE (Secure UART) to enable SETOOLS communication with the device.
> - Configure J15 to position U4 (UART4) to see STDIO messages from the application.

## Projects

The `SDS.csolution.yml` application is configured for the targets [AppKit-E7-U55](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-d1-34b5d51/projects/) or [SSE-300-U55](https://github.com/ARM-software/AVH) FVP simulation models.

It contains two projects:

- **`DataTest.cproject.yml`**: Verifies the SDSIO interface on hardware.
- **`AlgorithmTest.cproject.yml`**: Verifies a LiteRT Object Detection (YOLO Fastest) ML model with recording and playback of SDS data files.

## Layer Type: Board and Layer Type: SDSIO

The board layer implements the Hardware Abstraction Layer (HAL) layer. Depending on the selected target, a different board implementation with the appropriate I/O interfaces is used:

- `Board/AppKit-E7_M55_HP/Board_HP.clayer.yml` and `sdsio_usb.clayer.yml` use the development board's **USB interface** for SDSIO communication.
- `Board/Corstone-300/Board-U55.clayer.yml` and `sdsio_fvp.clayer.yml` use the **VSI Interface** to the AVH FVP simulator for SDSIO communication.

## Build Targets

- **`Debug`**: Debug version of the application used for recording/playback of input data and algorithm output data.
- **`Release`**: Release version of the application used for recording/playback of input data and algorithm output data.

> Note: Only difference between `Debug` and `Release` targets is compiler optimization level and the amount of debug information printed to the STDIO.

For more details, refer to the [SDS Template Application](https://arm-software.github.io/SDS-Framework/main/template.html).

## DataTest Project on the Alif AppKit-E7-AIML board

The **DataTest** project allows you to verify the SDSIO communication and it is recommended to use this project first.

Build and run this project in VS Code by following these steps:

1. Use **Manage Solution Settings** and select:
     - Target type **AppKit-E7-U55**.
     - Project **DataTest** with Build Type **Debug**.
2. Open in VS Code a Terminal window and start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server) with `sdsio-server usb`
3. **Build Solution** to create an executable image.
4. Configure **J15** for **UART4 (U4 position)** used for the application output (STDIO).
5. Connect a **USB cable** between the host **PC** and the **PRG USB (J2)** connector on the **AppKit-E7-AIML** board.
6. Connect a second **USB cable** between host **PC** and the **SOC USB (J1)** connector on the **AppKit-E7-AIML** board.
7. Open the VS Code **Serial Monitor** and start monitoring the application output (STDIO) on the J-Link Virtual COM port.
8. **Load Application to Target** to download the application to the board.
9. Reset the board with **RESET (SW1)** button and observe the application output (STDIO) like below

```txt
SDSIO-Client USB interface initialized successfully
99% idle
99% idle
```

alternatively if SDSIO-Server is not reachable or not running you will see the output:

```txt
SDSIO-Client USB interface initialization failed!
Ensure that device is connected via USB to the host PC running SDSIO-Server, then restart the application!
```

> **Note:**
>
>   If the SDSIO-Server is not found, verify your [SDS Utilities Setup](https://arm-software.github.io/SDS-Framework/main/utilities.html#setup).

### Recording/playback testing

For executing the **recording** or **playback** test, follow the steps below:


### Recording

To perform a recording, follow these steps:
- To start recording, press the `R` key in the SDSIO-Server window, or press the **joystick (SW2)** on the board.
- To stop recording, press the `S` key in the SDSIO-Server window, or press the **joystick (SW2)** on the board again.

**Output of SDSIO-Server during recording**

```txt
>sdsio-server usb 
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
99% idle.
SDSIO command: start recording ('R').
sdsFlags = 0x90000000.
Record:   Test_In (Test_In.0.sds)
Record:   Test_Out (Test_Out.0.sds)
SDSIO command: stop ('s').
sdsFlags = 0x10000000.
Closed:   Test_In (Test_In.0.sds)
Closed:   Test_Out (Test_Out.0.sds)
```

**Application output in the Serial Monitor**

```txt
99% idle
==== SDS recording started
99% idle
99% idle
==== SDS recording stopped
99% idle
```

Each run records two files: `Test_In.<n>.sds` and `Test_Out.<n>.sds` in the folder where SDSIO-Server was started. `<n>` is a sequential number.

**Check SDS Files**

The [SDS-Check](https://arm-software.github.io/SDS-Framework/main/utilities.html#sds-check) utility verifies SDS files for consistency. For example:

```bash
>sds-check -i Test_In.0.sds
File Name         : Test_In.0.sds
File Size         : 240.960 bytes
Number of Records : 240
Recording Time    : 2.390 ms
Recording Interval: 10 ms
Data Size         : 239.040 bytes
Block Size        : 996 bytes
Data Rate         : 99.600 byte/s
Jitter            : Not detected
Validation passed
```

### Playback

To perform a playback, follow these steps:
- To start the playback, press the `P` key in the SDSIO-Server window.
- The playback will stop automatically when it plays all the data from the SDS file.

The stream `Test_In.<n>.sds` is read back and the algorithm processes this data.
The stream `Test_Out.<n>.p.sds` is written containing results of the test algorithm.
The SDS file `Test_Out.<n>.p.sds` created during playback should be identical to the `Test_Out.<n>.sds` file created during the recording.

**Output of SDSIO-Server during playback**

```txt
>sdsio-server usb
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
99% idle.
SDSIO command: start playback ('P').
sdsFlags = 0xB0000000.
Playback: Test_In (Test_In.0.sds)
Record:   Test_Out (Test_Out.0.p.sds)
Closed:   Test_In (Test_In.0.sds)
Closed:   Test_Out (Test_Out.0.p.sds)
sdsFlags = 0x30000000.
```

**Application output in the Serial Monitor**

```txt
99% idle
==== SDS playback started
99% idle
==== SDS playback stopped
99% idle
```

## DataTest Project on the AVH-FVP Simulator

The **DataTest** can be also executed on [AVH-FVP](https://github.com/ARM-software/AVH) simulation models using the steps below:

1. Use **Manage Solution Settings** and select:
     - Target type **SSE-300-U55**.
     - Project **DataTest** with Build Type **Debug**.
2. **Build Solution** to create an executable image.
3. **Load and Run** starts the application on the AVH-FVP simulation. The output is shown in the Terminal console.

> **Notes:**
>
> - The simulator target only supports playback mode.
> - This example includes an `algorithm.sdsio.yml` configuration file that sets `algorithm/SDS Recordings` as the working directory for SDS playback.  
>   To test the previous example, either: copy recorded files `Test_In.0.sds` and `Test_Out.0.sds` into that directory, or update the
>   `workdir` in the `algorithm.sdsio.yml` and change stream names `ML_In` to `Test_In` and `ML_Out` to `Test_Out`.  
>   For details on the **sdsio.yml** configuration format and available options, refer to
>   the [documentation](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-control-file-sdsioyml).
> - The VSI script used by the simulator also generates the `sdsio.log` output file.

**FVP simulation output in the terminal**

```txt
Executing task: FVP_Corstone_SSE-300_Ethos-U55 -f Board/Corstone-300/fvp_config.txt -a out/DataTest/SSE-300-U55/Debug/DataTest.hex  

Ethos-U version info:
        Arch:       v1.1.0
        MACs/cc:    256
        Cmd stream: v0
SDSIO VSI interface initialized successfully
==== SDS playback started
==== SDS playback stopped
```

## AlgorithmTest Project on the Alif AppKit-E7-AIML board

The **AlgorithmTest** project includes an **LiteRT Object Detection (YOLO Fastest) ML model** that you can verify using the SDS-Framework.

#### Key Components

**Video Frame Capture** (`data_in_user.c`):

- Initializes the on-board camera input stream using the CMSIS vStream driver
- Captures video frames and prepares them for ML inference by converting them to RGB format and resizing them to the model's input dimensions
- Provides the processed frame data for SDS recording

**Algorithm Processing** (`algorithm_user.cpp`):

- Initializes the ML model and LCD display stream using the CMSIS vStream driver
- Performs the ML inference pipeline, including pre-processing, inference, and post-processing
- Copies detection results to the output buffer for SDS recording
- Displays video frames on the LCD with overlaid detection bounding boxes using the CMSIS vStream driver

#### Hardware setup and running the example

Build and run this project in VS Code using the following steps:

1. Use **Manage Solution Settings** and select:
     - Target type **AppKit-E7-U55**.
     - Project **AlgorithmTest** with Build Type **Debug**.
2. Open in VS Code a Terminal window and start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server) with `sdsio-server usb`
3. **Build Solution** to create an executable image.
4. Configure **J15** for **UART4 (U4 position)** used for the application output (STDIO).
5. Connect a **USB cable** between the host **PC** and the **PRG USB (J2)** connector on the **AppKit-E7-AIML** board.
6. Connect a second **USB cable** between host **PC** and the **SOC USB (J1)** connector on the **AppKit-E7-AIML** board.
7. Open the VS Code **Serial Monitor** and start monitoring the UART output.
8. **Load Application to Target** to download the application to the board.
9. Reset the board with **RESET (SW1)** button and observe the application output (STDIO) like below

### Recording

To perform a recording, follow these steps:
- To start recording, press the `R` key in the SDSIO-Server window, or press the **joystick (SW2)** on the board.
- To stop recording, press the `S` key in the SDSIO-Server window, or press the **joystick (SW2)** on the board again.

**Output of SDSIO-Server during recording**

```txt
>sdsio-server usb 
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
41% idle.
43% idle.
SDSIO command: start recording ('R').
sdsFlags = 0x90000000.
41% idle.
Record:   ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.sds)
47% idle.
52% idle.
56% idle.
SDSIO command: stop ('s').
sdsFlags = 0x10000000.
60% idle.
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.sds)
57% idle.
```

**Application output in the Serial Monitor**

```txt
SDSIO-Client USB interface initialized successfully
INFO - Added ethos-u support to op resolver
INFO - Creating allocator using tensor arena at 0x02000370
  :
INFO - .Operator 0: ethos-u
No object detected
43% idle
No object detected
  :
==== SDS recording started
No object detected
No object detected
Detected objects :: [x=69, y=126, w=33, h=45]
Detected objects :: [x=57, y=133, w=28, h=37]
Detected objects :: [x=52, y=100, w=48, h=66]
  :
No object detected
41% idle
==== SDS recording stopped
No object detected
```

Each run records two files: `ML_In.<n>.sds` and `ML_Out.<n>.sds` in the folder where SDSIO-Server was started. `<n>` is a sequential number.

### Playback

To perform a playback, follow these steps:
- To start the playback, press the `P` key in the SDSIO-Server window.
- The playback will stop automatically when it plays all the data from the SDS file.

The stream `ML_In.<n>.sds` is read back and the algorithm processes this data.
The stream `ML_Out.<n>.p.sds` is written containing results of the test algorithm.
The SDS file `ML_Out.<n>.p.sds` created during playback should be identical to the `ML_Out.<n>.sds` file created during the recording.

**Output of SDSIO-Server during playback**

```txt
>sdsio-server usb
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
41% idle.
43% idle.
SDSIO command: start playback ('P').
sdsFlags = 0xB0000000.
Playback: ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.p.sds)
62% idle.
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.p.sds)
sdsFlags = 0x30000000.
64% idle.
```

**Application output in the Serial Monitor**

```txt
==== SDS playback started
No object detected
No object detected
No object detected
No object detected
Detected objects :: [x=143, y=9, w=40, h=55]
Detected objects :: [x=137, y=9, w=33, h=55]
62% idle
  :
Detected objects :: [x=109, y=69, w=43, h=58]
==== SDS playback stopped
64% idle
```

> **Notes:**
>
> - The playback implementation replays recordings as quickly as possible and does not account for timeslot data.
>   During playback, the ML system receives the same recorded input data, so timing information is not relevant in this context.

## AlgorithmTest playback on the AVH-FVP Simulator

The **AlgorithmTest** can be also executed on [AVH-FVP](https://github.com/ARM-software/AVH) simulation models using the steps below:

1. Use **Manage Solution Settings** and select:
     - Target type **SSE-300-U55**.
     - Project **AlgorithmTest** with Build Type **Debug**.
2. **Build Solution** to create an executable image.
3. **Load and Run** starts the application on the AVH-FVP simulation. The output is shown in the Terminal console.

**FVP simulation output in the terminal**

```txt
Executing task: FVP_Corstone_SSE-300_Ethos-U55 -f Board/Corstone-300/fvp_config.txt -a out/AlgorithmTest/SSE-300-U55/Debug/AlgorithmTest.hex  

Ethos-U version info:
        Arch:       v1.1.0
        MACs/cc:    256
        Cmd stream: v0
SDSIO VSI interface initialized successfully
INFO - Added ethos-u support to op resolver
INFO - Creating allocator using tensor arena at 0x31000000
  :
INFO -  Operator 0: ethos-u
==== SDS playback started
No object detected
No object detected
No object detected
No object detected
Detected objects :: [x=143, y=9, w=40, h=55]
Detected objects :: [x=137, y=9, w=33, h=55]
  :
Detected objects :: [x=109, y=69, w=43, h=58]
==== SDS playback stopped
```

**Content of the sdsio.log file recorded during the run**

```txt
Created by ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB\Board\Corstone-300\vsi\python\arm_vsi3.py

SDSIO VSI version 3.0.0
SDSIO_FVP environment variable not set.
Working directory: ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB\algorithm\SDS Recordings.
SDSIO configuration YAML: ...\Arm-Examples\SDS-Examples\Alif\AppKit-E7_USB\algorithm.sdsio.yml.
sdsFlags = 0xB0000000.
Playback step 1/1: Play ML_In.0.sds.
Playback: ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.p.sds)
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.p.sds)
Playback complete - no more steps remaining.
sdsFlags = 0x30000000.
sdsFlags = 0x70000000.
sdsFlags = 0x30000000.
```

> **Notes:**
>
> - The simulator target only supports playback mode.
> - This example includes an `algorithm.sdsio.yml` configuration file that sets `algorithm/SDS Recordings` as the working directory for SDS playback.  
>   To test the previous example, either: copy recorded files `ML_In.0.sds` and `ML_Out.0.sds` into that directory, or update the
>   `workdir` in the `algorithm.sdsio.yml`.  
>   For details on the **sdsio.yml** configuration format and available options, refer to
>   the [documentation](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-control-file-sdsioyml).
> - The VSI script used by the simulator also generates the `sdsio.log` output file.
