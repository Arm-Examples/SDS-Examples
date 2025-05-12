# About this project

This is a standalone test application that uses [SDS Recorder](https://github.com/ARM-software/SDS-Framework)
to record test data on the host computer or SD card.

The application generates the following data:

- **input test data** to simulate IMU sensor data,
- **output test data** to simulate the ML output data.

Examples of 7 seconds of recorded data are available in the `./SDS Recordings` subfolder,
which also contains the `.sds.yml` metadata files.

### Configuration

The application is configured by specifying the required bandwidth and the time interval for
recording in the file `DataTest.h`.

The required **bandwidth** is specified in bytes/sec:

```c
#define TEST_BANDWIDTH          100000U
```

The time **interval** is specified in milliseconds:

```c
#define TEST_INTERVAL           10U
```

The stream buffering in the SDS framework is then configured automatically using these
configuration settings.

### Running

To run the network and USB solutions, the [SDSIO-Server](https://github.com/ARM-software/SDS-Framework/blob/main/documentation/utilities.md#sdsio-server)
must be started on the host computer.

### Validation of recorded data

Recorded data can be checked with the [SDS-Check](https://github.com/ARM-software/SDS-Framework/blob/main/documentation/utilities.md#sds-check)
utility program. This tool reads in the data and checks it for correctness and consistency.
