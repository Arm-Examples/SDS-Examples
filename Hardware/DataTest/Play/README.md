# About this project

This is a standalone test application that uses the [SDS Player](https://github.com/ARM-software/SDS-Framework)
to validate test data previously recorded with the **DataTest recording** application.

The validation data are files recorded on the host computer or SD card:
- `DataInput.0.sds` recorded input test data,
- `DataOutput.0.sds` recorded output test data.

The application reads the recorded data and compares it with the expected simulation data that is
newly generated in the application. The check fails if the recorded data differs from the expected data.

### Configuration

The application is configured by specifying the required bandwidth and the time interval for
recording in the file `DataTest.h`.
The settings should match the configuration for the DataTest recording application.

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
