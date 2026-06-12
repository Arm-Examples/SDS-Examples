# SDS Application on STMicroelectronics B-U585I-IOT02A board with SDSIO using USB Interface

This application is an example of using [**Edge Impulse Continuous Motion Recognition ML algorithm**](https://studio.edgeimpulse.com/public/14299/latest) on a physical board.
It enables recording and playback of real-world data streams using physical hardware or just playback using simulation models (Arm Virtual Hardware - FVP).
The real-world data streams are captured in SDS data files.

## Pre-Requisite

To run this example:

- Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) and run a blinky example on the board to verify tool installation.
- Use `cpackget add ARM::SDS` to add the SDS Framework and [Setup the SDS Python utilities](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server).
- Setup the [Python environment](https://arm-software.github.io/SDS-Framework/main/utilities.html#setup) for running the SDS Utilities.

## STMicroelectronics B-U585I-IOT02A board

The [STMicroelectronics B-U585I-IOT02A](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/) board is based on a Cortex-M33 processor. For using the integrated ST-Link install the [ST-Link USB driver](https://www.st.com/en/development-tools/stsw-link009.html) or your computer and update the firmware.

## Projects

The `SDS.csolution.yml` application is configured for the targets [ST B-U585I-IOT02A](https://www.keil.arm.com/boards/stmicroelectronics-b-u585i-iot02a-revc-c3bc599/guide/) or [SSE-300-U55](https://github.com/ARM-software/AVH) FVP simulation models.

It contains two projects:

- **`DataTest.cproject.yml`**: Verifies the SDSIO interface on hardware.
- **`AlgorithmTest.cproject.yml`**: Verifies a Continuous Motion Recognition ML algorithm with recording and playback of SDS data files.

## Layer Type: Board and Layer Type: SDSIO

The board layer implements the Hardware Abstraction Layer (HAL) layer. Depending on the selected target, a different board implementation with the appropriate I/O interfaces is used:

- `Board/B-U585I-IOT02A/Board.clayer.yml` and `sdsio_usb.clayer.yml` use the **USB Interface** for SDSIO on the development board.
- `Board/Corstone-300/Board-U55.clayer.yml` and `sdsio_fvp.clayer.yml` use the **VSI Interface** for SDSIO on AVH FVP simulation.

## Layer Type: Edge Impulse layer

This layer contains the ML model that is used in the `AlgorithmTest.cproject.yml`.

#### ML Model update procedure

1.  **Clone the project**  
    Clone the [**Tutorial: Continuous motion recognition**](https://studio.edgeimpulse.com/public/14299/latest)
    project from Edge Impulse.
2.  **Build CMSIS pack**  
    - Under **Time series data, Spectral Analysis, Classification, Anomaly Detection (K-means)**, select **Deployment**.
    - Under **Search deployment options**, type `Open`, then choose **Open CMSIS pack (Generates a CMSIS Software Component pack)**.
    - Click **Build** to generate the CMSIS pack.
3.  **Download and extract**  
    Once the build completes, download the generated ZIP file: `tutorial_-continuous-motion-recognition-cmsis-package-v<version>.zip`
    Extract its contents locally.
4.  **Copy updated packs**  
    Copy the contents of the extracted `.pack` files into: `./algorithm/ML/Packs/EdgeImpulse`.
    Maintain the same directory structure and organization as the existing packs.
5.  **Update layer configuration**  
    Edit the [**edgeimpulse.clayer.yml**](./algorithm/ML/EI/Layer/edgeimpulse.clayer.yml) file and update all version references to match the new pack versions.

## Projects

- **`DataTest.cproject.yml`**: Verifies SDSIO interface on hardware.
- **`AlgorithmTest.cproject.yml`**: Verifies a Continuous Motion Recognition ML algorithm with recording (on hardware only) and playback of SDS data files.

## Build Targets

- **`Debug`**: Debug version of the application used for recording/playback of input data and algorithm output data.
- **`Release`**: Release version of the application used for recording/playback of input data and algorithm output data.

> Note: Only difference between `Debug` and `Release` targets is compiler optimization level and the amount of debug information printed to the STDIO.

For more details, refer to the [SDS Template Application](https://arm-software.github.io/SDS-Framework/main/template.html).

## DataTest Project on ST B-U585I-IOT02A board

The DataTest project allows you to verify the SDSIO communication and it is recommended to use this project first.

Build and run this project in VS Code using the following steps:

1. Use **Manage Solution Settings** and select:
     - Target type **B-U585I-IOT02A**.
     - Project **DataTest** with Build Type **Debug**.
1. Use **Manage Solution Settings** and select as Active Project **DataTest** with Build Type **Debug**.
2. **Build solution** creates the executable image.
3. Connect to STLK USB connector of the ST B-U585I-IOT02A board and use the **`...`** menu **Target Information** to validate [ST-Link installation](https://www.st.com/en/development-tools/stsw-link009.html).
4. **Load and Run** to download the application.
5. Use the VS Code **Serial Monitor** to observe the output below.

```txt
SDSIO-Client USB interface initialization failed!
Ensure that device is connected via USB to the host PC running SDSIO-Server, then restart the application!
Error: SDS_ERROR_CHECK status = -4: D:/Repositories/GitHub/Arm-Examples/SDS-Examples/ST/B-U585I-IOT02A/KeywordSpotting.tst/datatest/sds_control.c: 174
90% idle
90% idle
```

> Note:
>
> It is possible to configure the input data bandwidth by editing `ALGO_TEST_BANDWIDTH` define in the `algorithm_config.h` file.
> Default bandwidth is configured to `100000U` which means approximately 100000 bytes per second.

### Recording/playback testing

For executing the **recording** or **playback** test, follow the steps below:

1. Connect a second USB cable between host PC and ST B-U585I-IOT02A board USB-C connector.
2. Open in VS Code a Terminal window and start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server) with `sdsio-server usb`
3. Open the VS Code **Serial Monitor** and start monitoring the UART output.
4. **Load and Run** the application on the ST B-U585I-IOT02A board.

### Recording

To perform a recording, follow these steps:
- To start recording, press the `R` key in the SDSIO Server window, or press the **User** button on the board.
- To stop recording, press the `S` key in the SDSIO Server window, or press the **User** button on the board again.

**Output of SDSIO-Server during recording**

```txt
>sdsio-server usb 
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: c:\ARM\PACK\ARM\SDS\3.0.0\utilities
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Server waiting for USB SDSIO-Client...
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
90% idle.
SDSIO command: start recording ('R').
sdsFlags = 0x90000000.
Record:   Test_In (Test_In.0.sds)
Record:   Test_Out (Test_Out.0.sds)
89% idle.
SDSIO command: stop ('s').
sdsFlags = 0x10000000.
Closed:   Test_In (Test_In.0.sds)
Closed:   Test_Out (Test_Out.0.sds)
90% idle.
```

**Application output in the Serial Monitor**

```txt
==== SDS recording started
89% idle
89% idle
==== SDS recording stopped
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
- To start the playback, press the `P` key in the SDSIO Server window.
- The playback will stop automatically when it plays all the data from the SDS file.

The stream `Test_In.<n>.sds` is read back and the algorithm processes this data.
The stream `Test_Out.<n>.p.sds` is written containing results of the test algorithm.
The SDS file `Test_Out.<n>.p.sds` created during playback should be identical to the `Test_Out.0.sds` file created during the recording.

**Output of SDSIO-Server during playback**

```txt
>sdsio-server usb
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: c:\ARM\PACK\ARM\SDS\3.0.0\utilities
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
90% idle.
SDSIO command: start playback ('P').
sdsFlags = 0xB0000000.
Playback: Test_In (Test_In.0.sds)
Record:   Test_Out (Test_Out.0.p.sds)
89% idle.
Closed:   Test_In (Test_In.0.sds)
Closed:   Test_Out (Test_Out.0.p.sds)
sdsFlags = 0x30000000.
88% idle.
90% idle.
```

**Application output in the Serial Monitor**

```txt
89% idle
==== SDS playback started
89% idle
==== SDS playback stopped
```

## DataTest Project on AVH-FVP Simulation

The DataTest can be also executed on [AVH-FVP](https://github.com/ARM-software/AVH) simulation models using the steps below.

1. Use **Manage Solution Settings** and select:
     - Target type **SSE-300-U55**.
     - Project **DataTest** with Build Type **Debug**.
2. **Build solution** to create an executable image.
3. **Load and Run** starts the application on the AVH-FVP simulation. The output is shown in the Terminal console.

> **Notes:**
>
> - The simulator target only supports playback mode.
> - This example includes an `SDS.sdsio.yml` configuration file that sets `algorithm/SDS Recordings` as the working directory for SDS playback.
>   To test the previous example, either: copy recorded files `Test_In.0.sds` and `Test_Out.0.sds` into that directory, or update the
>   `workdir` in the `SDS.sdsio.yml`.
>   For details on the sdsio.yml configuration format and available options, refer to
>   the [documentation](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-control-file-sdsioyml).

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

Info: /OSCI/SystemC: Simulation stopped by user.
```

## AlgorithmTest Project on ST B-U585I-IOT02A board

The **AlgorithmTest** project includes an **Edge Impulse Motion Recognition ML model** that you can verify using the SDS-Framework.

Build and run this project in VS Code using the following steps:

1. Use **Manage Solution Settings** and select:
     - Target type **B-U585I-IOT02A**.
     - Project **AlgorithmTest** with Build Type **Debug**.
2. **Build solution** creates the executable image.
3. Open in VS Code a Terminal window and start the [SDSIO-Server](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-server) with `sdsio-server usb`
4. **Load and Run** to download the application.
5. Use the VS Code **Serial Monitor** to observe the output.

### Recording

To perform a recording, follow these steps:
- To start recording, press the `R` key in the SDSIO Server window, or press the **User** button on the board.
- To stop recording, press the `S` key in the SDSIO Server window, or press the **User** button on the board again.

**Output of SDSIO-Server during recording**

```txt
>sdsio-server usb 
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: c:\ARM\PACK\ARM\SDS\3.0.0\utilities
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Server waiting for USB SDSIO-Client...
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
88% idle.
84% idle.
87% idle.
sdsFlags = 0x90000000.
Record:   ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.sds)
85% idle.
87% idle.
84% idle.
sdsFlags = 0x10000000.
87% idle.
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.sds)
84% idle.
```

**Application output in the Serial Monitor**

```txt
==== SDS recording started
85% idle
87% idle
Predictions (DSP: 18.000000 ms., Classification: 0 ms., Anomaly: 0ms.): 
#Classification results:
    idle: 0.000000
    snake: 0.000000
    updown: 0.996094
    wave: 0.000000
Anomaly prediction: 0.041957
84% idle
87% idle
Predictions (DSP: 18.000000 ms., Classification: 0 ms., Anomaly: 0ms.): 
#Classification results:
    idle: 0.101562
    snake: 0.898438
    updown: 0.000000
    wave: 0.000000
Anomaly prediction: 0.241648
==== SDS recording stopped
84% idle
```

### Playback

To perform a playback, follow these steps:
- To start the playback, press the `P` key in the SDSIO Server window.
- The playback will stop automatically when it plays all the data from the SDS file.

The stream `ML_In.<n>.sds` is read back and the algorithm processes this data.
The stream `ML_Out.<n>.p.sds` is written containing results of the test algorithm.
The SDS file `ML_Out.<n>.p.sds` created during playback should be identical to the `ML_Out.0.sds` file created during the recording.

**Output of SDSIO-Server during playback**

```txt
>sdsio-server usb
SDSIO-Server v3.0.0
Press 'Ctrl+C' or 'X' to exit.
Working directory: c:\ARM\PACK\ARM\SDS\3.0.0\utilities
SDSIO command input: R=Record, P=playback, S/s=stop, T/t=reset, X/x=exit, A-H=set flags 0-7, a-h=clear flags 0-7.
SDSIO-Server waiting for USB SDSIO-Client...
SDSIO-Client USB device connected.
sdsFlags = 0x10000000.
88% idle.
84% idle.
SDSIO command: start playback ('P').
87% idle.
sdsFlags = 0xB0000000.
Playback: ML_In (ML_In.0.sds)
Record:   ML_Out (ML_Out.0.p.sds)
Closed:   ML_In (ML_In.0.sds)
Closed:   ML_Out (ML_Out.0.p.sds)
sdsFlags = 0x30000000.
83% idle.
```

**Application output in the Serial Monitor**

```txt
==== SDS playback started
Predictions (DSP: 18.000000 ms., Classification: 0 ms., Anomaly: 1.000000ms.): 
#Classification results:
    idle: 0.101562
    snake: 0.898438
    updown: 0.000000
    wave: 0.000000
Anomaly prediction: 0.241648
==== SDS playback stopped
83% idle
```

> **Notes:**
>
> - The playback implementation replays recordings as quickly as possible and does not account for timestamp data.
>   During playback, the ML system receives the same recorded input data, so timing information is not relevant in this context.
> - This example includes an `SDS.sdsio.yml` configuration file that sets `algorithm/SDS Recordings` as the working directory for SDS playback.
>   To test the previous example, either: copy recorded files `ML_In.0.sds` and `ML_Out.0.sds` into that directory, or update the
>   `workdir` in the `SDS.sdsio.yml`.
>   For details on the sdsio.yml configuration format and available options, refer to
>   the [documentation](https://arm-software.github.io/SDS-Framework/main/utilities.html#sdsio-control-file-sdsioyml).

## AlgorithmTest playback on AVH-FVP Simulation

To playback the recorded SDS data files use in VS Code the following steps:

1. Use **Manage Solution Settings** and select:
     - Target type **SSE-300-U55**.
     - Project **AlgorithmTest** with Build Type **Debug**.
2. **Build solution** creates the executable image.
3. **Load and Run** to runs the application on the simulator and uses the SDS files previous captured.

> **Notes:**
>
> - Simulator uses `SDS.sdsio.yml` file as configuration and script for playback.
> - Simulator generates the output in the file `sdsio.log`.
