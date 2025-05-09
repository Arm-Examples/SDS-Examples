# About this application

This is a standalone test application that uses the [SDS Player](https://github.com/ARM-software/SDS-Framework)
to validate test data previously recorded with the **DataTest recording** application.

This example is pre-configured for STMicroelectronics [STM32H735G-DK](https://www.st.com/en/evaluation-tools/stm32h735g-dk.html) board.

It uses the Ethernet interface to validate the data from the host computer.

Validation data:
- `DataInput.0.sds` and `DataOutput.0.sds` files recorded on the host computer.

### Configuration

The application is configured by specifying the required **bandwidth** and the **recording interval** in the file `DataTest.h`.
The settings should match the configuration for the DataTest recording application.

The recording bandwidth is specified in bytes/sec:

```c
#define TEST_BANDWIDTH          100000U
```

The recording interval is specified in milliseconds:

```c
#define TEST_INTERVAL           10U
```

The stream buffering of the SDS framework is automatically configured using these
configuration settings.

### Measuring the CPU load

The idle time is the time during which the CPU is not executing the application code. This means that it executes
the code of the idle thread, incrementing the `idle_cnt` counter. The code for incrementing the idle counter is located
in the `osRtxIdle_Thread`:

```c
__NO_RETURN void osRtxIdleThread(void *argument) {
  (void)argument;

  for (;;) {
    idle_cnt++;
  }
}
```

The counter is incremented for one second, then the idle factor is calculated as the ratio between the idle counter
and the evaluated idle counter for the system without load `no_load_cnt` and output on the debug console:

```c
if (++cnt == 10U) {
  cnt = 0U;

  printf("%d%% idle\n",(idle_cnt - prev_cnt) / no_load_cnt);
  prev_cnt = idle_cnt;
}
```

For a correct measurement, the loop interval must really be periodic, therefore the function `osDelayUntil` is used
to create time intervals in the measurement loop:

```c
timestamp = osKernelGetTickCount();
for (;;) {
   :
  timestamp += 100U;
  osDelayUntil(timestamp);
}
```
