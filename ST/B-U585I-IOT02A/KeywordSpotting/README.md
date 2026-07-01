# SDS Application for ST B-U585I-IOT02A board with SDSIO using USB Interface

This application is an example of using [**Edge Impulse Keyword Spotting ML algorithm**](https://studio.edgeimpulse.com/public/547356/live) on a physical board.
It lets you record and play back real-world data streams on physical hardware, feeding
them to your algorithm for testing. The data streams are stored in SDS data files.

## Pre-Requisite

To run this example:

- Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) and run a Blinky example on the board to verify tool installation.
- Use `cpackget add ARM::SDS` to add the SDS Framework and [Setup the SDS Python utilities](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server).
- Setup the [Python environment](https://arm-software.github.io/SDS-Framework/main/utilities.html#setup) for running the SDS Utilities.

## ST B-U585I-IOT02A board

The [ST B-U585I-IOT02A](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/) board is based on a Cortex-M33 processor. For using the integrated ST-Link install the [ST-Link USB driver](https://www.st.com/en/development-tools/stsw-link009.html) or your computer and update the firmware.

## Projects

The `SDS.csolution.yml` application is configured for the targets [B-U585I-IOT02A](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/guide/) or [SSE-300-U55](https://github.com/ARM-software/AVH) FVP simulation models.

It contains two projects:

- **`DataTest.cproject.yml`**: Verifies the SDSIO interface on hardware.
- **`AlgorithmTest.cproject.yml`**: Verifies a Keyword Spotting ML algorithm with recording and playback of SDS data files.

## Layer Type: Board and Layer Type: SDSIO

The board layer implements the Hardware Abstraction Layer (HAL) layer. Depending on the selected target, a different board implementation with the appropriate I/O interfaces is used:

- `Board/B-U585I-IOT02A/Board.clayer.yml` and `sdsio_usb.clayer.yml` use the development board's **USB interface** for SDSIO communication.
- `Board/Corstone-300/Board-U55.clayer.yml` and `sdsio_fvp.clayer.yml` use the **VSI Interface** to the AVH FVP simulator for SDSIO communication.

## Layer Type: Edge Impulse layer

This layer contains the ML model that is used in the `AlgorithmTest.cproject.yml`.

#### ML Model update procedure

1.  **Clone the project**  
    Clone the [**Tutorial: Responding to your voice**](https://studio.edgeimpulse.com/public/547356/live)
    project from Edge Impulse.
2.  **Build CMSIS pack**  
    - Under **Time series data, Spectral Analysis, Classification, Anomaly Detection (K-means)**, select **Deployment**.
    - Under **Search deployment options**, type `Open`, then choose **Open CMSIS pack (Generates a CMSIS Software Component pack)**.
    - Click **Build** to generate the CMSIS pack.
3.  **Download and extract**  
    Once the build completes, download the generated ZIP file: `tutorial_-responding-to-your-voice-cmsis-package-v<version>.zip`
    Extract its contents locally.
4.  **Copy updated packs**  
    Copy the contents of the extracted `.pack` files into: `./algorithm/ML/Packs/EdgeImpulse`.
    Maintain the same directory structure and organization as the existing packs.
5.  **Update layer configuration**  
    Edit the [**edgeimpulse.clayer.yml**](./algorithm/ML/EI/Layer/edgeimpulse.clayer.yml) file and update all version references to match the new pack versions.

## Projects

- **`DataTest.cproject.yml`**: Verifies SDSIO interface on hardware.
- **`AlgorithmTest.cproject.yml`**: Verifies a Keyword Spotting ML algorithm with recording (on hardware only) and playback of SDS data files.

## Build Targets

- **`Debug`**: Debug version of the application used for recording/playback of input data and algorithm output data.
- **`Release`**: Release version of the application used for recording/playback of input data and algorithm output data.

> Note: Only difference between `Debug` and `Release` targets is compiler optimization level and the amount of debug information printed to the STDIO.

For more details, refer to the [SDS Template Application](https://arm-software.github.io/SDS-Framework/main/template.html).

## DataTest Project on the ST B-U585I-IOT02A board

The **DataTest** project allows you to verify the SDSIO communication and it is recommended to use this project first.

Build and run this project in VS Code by following these steps:

1. Use **Manage Solution Settings** and select:
     - Target type **B-U585I-IOT02A**.
     - Project **DataTest** with Build Type **Debug**.
2. Open in VS Code a Terminal window and start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server) with `sdsio-server usb` .
3. **Build Solution** to create an executable image.
4. Connect a **USB cable** between the host **PC** and the **STLK (CN8)** USB connector on the **B-U585I-IOT02A** board.
5. Connect a second **USB cable** between host **PC** and the **USB-C (CN1)** connector on the **B-U585I-IOT02A** board.
6. Open the VS Code **Serial Monitor** and start monitoring the application output (STDIO) on the STLink Virtual COM port.
7. **Load and Run** to download the application to the board and start it.
8. Observe the application output (STDIO) like below

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
> - If the SDSIO-Server is not found, verify your [SDS Utilities Setup](https://arm-software.github.io/SDS-Framework/main/utilities.html#setup).
> - It is possible to configure the input data bandwidth by editing `ALGO_TEST_BANDWIDTH` define in the `algorithm_config.h` file.
>   Default bandwidth is configured to `100000U` which means approximately 100000 bytes per second.

### Recording/playback testing

For executing the **recording** or **playback** test, follow the steps below:

### Recording

To perform a recording, follow these steps:
- To start recording, press the `R` key in the SDSIO-Server window, or press the **User** button on the board.
- To stop recording, press the `S` key in the SDSIO-Server window, or press the **User** button on the board again.

**Output of SDSIO-Server during recording**

```txt
>sdsio-server usb 
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Server waiting for USB SDSIO-Client...
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
99% idle.
SDSIO command: start recording ('R').
sdsFlags = 0x90000000.
Record:   Test_In (Test_In.0.sds)
Record:   Test_Out (Test_Out.0.sds)
98% idle.
SDSIO command: stop ('s').
sdsFlags = 0x10000000.
Closed:   Test_In (Test_In.0.sds)
Closed:   Test_Out (Test_Out.0.sds)
99% idle.
```

**Application output in the Serial Monitor**

```txt
99% idle
==== SDS recording started
98% idle
98% idle
==== SDS recording stopped
99% idle
```

Each run records two files: `Test_In.<n>.sds` and `Test_Out.<n>.sds` in the folder where SDSIO-Server was started. `<n>` is a sequential number.

**Check SDS Files**

The [SDS-Check](https://arm-software.github.io/SDS-Framework/main/utilities.html#sds-check) utility verifies SDS files for consistency. For example:

```bash
>sds-check -i Test_In.0.sds
File Name         : Test_In.0.sds
File Size         : 271.080 bytes
Number of Records : 270
Recording Time    : 2.690 ms
Recording Interval: 10 ms
Data Size         : 268.920 bytes
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
Working directory: ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
99% idle.
SDSIO command: start playback ('P').
sdsFlags = 0xB0000000.
Playback: Test_In (Test_In.0.sds)
Record:   Test_Out (Test_Out.0.p.sds)
98% idle.
Closed:   Test_In (Test_In.0.sds)
Closed:   Test_Out (Test_Out.0.p.sds)
sdsFlags = 0x30000000.
99% idle.
```

**Application output in the Serial Monitor**

```txt
99% idle
==== SDS playback started
98% idle
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

## AlgorithmTest Project on the ST B-U585I-IOT02A board

The **AlgorithmTest** project includes an **Edge Impulse Keyword Spotting ML model** that you can verify using the SDS-Framework.

#### Key Components

**Audio Capture** (`data_in_user.c`):

- Initializes the on-board microphone using the CMSIS vStream driver
- Captures audio samples and prepares them for ML inference by removing DC offset in the captured samples
- Provides the processed audio data for SDS recording

**Algorithm Processing** (`algorithm_user.cpp`):

- Initializes the ML model
- Performs the ML inference pipeline, including pre-processing, inference, and post-processing
- Copies detection results to the output buffer for SDS recording
- Outputs `helloworld` classification results to the STDIO when the detection confidence exceeds 60 %

#### Hardware setup and running the example

Build and run this project in VS Code using the following steps:

1. Use **Manage Solution Settings** and select:
     - Target type **B-U585I-IOT02A**.
     - Project **AlgorithmTest** with Build Type **Debug**.
2. Open in VS Code a Terminal window and start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server) with `sdsio-server usb` .
3. **Build Solution** to create an executable image.
4. Connect a **USB cable** between the host **PC** and the **STLK (CN8)** USB connector on the **B-U585I-IOT02A** board.
5. Connect a second **USB cable** between host **PC** and the **USB-C (CN1)** connector on the **B-U585I-IOT02A** board.
6. Open the VS Code **Serial Monitor** and start monitoring the application output (STDIO) on the STLink Virtual COM port.
7. **Load and Run** to download the application to the board and start it.

### Recording

To perform a recording, follow these steps:
- To start recording, press the `R` key in the SDSIO-Server window, or press the **User** button on the board.
- To stop recording, press the `S` key in the SDSIO-Server window, or press the **User** button on the board again.

**Output of SDSIO-Server during recording**

```txt
>sdsio-server usb 
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Server waiting for USB SDSIO-Client...
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
88% idle.
86% idle.
SDSIO command: start recording ('R').
sdsFlags = 0x90000000.
Record:   ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.sds)
86% idle.
SDSIO command: stop ('s').
sdsFlags = 0x10000000.
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.sds)
87% idle.
```

**Application output in the Serial Monitor**

```txt
==== SDS recording started
87% idle
86% idle
Timing: DSP 20 ms, inference 9 ms, anomaly 0 ms
Classification predictions:
  helloworld: 0.769531
  noise: 0.085938
  unknown: 0.144531
87% idle
88% idle
Timing: DSP 19 ms, inference 10 ms, anomaly 0 ms
Classification predictions:
  helloworld: 0.968750
  noise: 0.003906
  unknown: 0.027344
88% idle
==== SDS recording stopped
88% idle
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
Working directory: ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Server waiting for USB SDSIO-Client...
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
87% idle.
SDSIO command: start playback ('P').
sdsFlags = 0xB0000000.
Playback: ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.p.sds)
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.p.sds)
sdsFlags = 0x30000000.
87% idle.
```

**Application output in the Serial Monitor**

```txt
==== SDS playback started
86% idle
Timing: DSP 20 ms, inference 10 ms, anomaly 0 ms
Classification predictions:
  helloworld: 0.964844
  noise: 0.015625
  unknown: 0.019531
Timing: DSP 20 ms, inference 10 ms, anomaly 0 ms
Classification predictions:
  helloworld: 0.867188
  noise: 0.132812
  unknown: 0.000000
==== SDS playback stopped
87% idle
```

> **Notes:**
>
> - The playback algorithm output data generated during playback is different then the one recorded during recording because
>   algorithm depends on data preceding the recording thus results are not identical.  
> - The playback implementation replays recordings as quickly as possible and does not account for timeslot data.
>   During playback, the ML system receives the same recorded input data, so timing information is not relevant in this context.
> - This example includes an `algorithm.sdsio.yml` configuration file that sets `algorithm/SDS Recordings` as the working directory for SDS playback.  
>   To test the previous example, either: copy recorded files `ML_In.0.sds` and `ML_Out.0.sds` into that directory, or update the `workdir` in the `algorithm.sdsio.yml`.  
>   For details on the **sdsio.yml** configuration format and available options, refer to
>   the [documentation](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-control-file-sdsioyml).

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
==== SDS playback started
0% idle
0% idle
==== SDS playback stopped
```

**Content of the sdsio.log file recorded during the run**

```txt
Created by ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting\Board\Corstone-300\vsi\python\arm_vsi3.py

SDSIO VSI version 3.0.0
SDSIO_FVP environment variable not set.
Working directory: ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting\algorithm\SDS Recordings.
SDSIO configuration YAML: ...\Arm-Examples\SDS-Examples\ST\B-U585I-IOT02A\KeywordSpotting\algorithm.sdsio.yml.
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
> - The playback algorithm output data generated during playback is different then the one recorded during recording because
>   algorithm depends on data preceding the recording thus results are not identical.  
> - This example includes an `algorithm.sdsio.yml` configuration file that sets `algorithm/SDS Recordings` as the working directory for SDS playback.  
>   To test the previous example, either: copy recorded files `ML_In.0.sds` and `ML_Out.0.sds` into that directory, or update the
>   `workdir` in the `algorithm.sdsio.yml`.  
>   For details on the **sdsio.yml** configuration format and available options, refer to
>   the [documentation](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-control-file-sdsioyml).
> - The VSI script used by the simulator also generates the `sdsio.log` output file.
