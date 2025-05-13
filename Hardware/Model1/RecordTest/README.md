# About this application

This is an standalone application using [Edge Impulse](https://edgeimpulse.com/) Machine Learning and
[SDS-Framework](https://github.com/ARM-software/SDS-Framework) for recording ML model input and output data.

The example has a trained model for continuous motion recognition using an tutorial from the Edge Impulse
(Tutorial: Continuous motion recognition).

> Note: ML Model was trained with sensor data sampled at 62.5 Hz so results with using sensor data sampled
>       at 52 Hz are not reliable

Model input data:

- const test accelerometer data of `wave` motion

Model output data:

- results of inference executed when 375 samples are available (approx. 2.4 seconds), containing
  probabilities for 4 possible classification groups

Prerecorded SDS files containing approximately 50 seconds of recorded data are available in the `./SDS Recordings` sub-folder, containing
also `.sds.yml` metadata files.

# User Interface

The recording is started by pressing a (VIO vioBUTTON0) push-button on the board.

> Note: Depending on the selected Interface the SDSIO server has to be running on the Host machine.
>       Except for the FileSystem interface where media is the SD Card inserted on the board and where
>       new recorded .sds files will be created.

## Output

The output in STDIO of this example running looks like below:

```txt
Recording started
97% idle
99% idle
Predictions (DSP: 4.000000 ms., Classification: 0 ms., Anomaly: 0ms.): 
#Classification results:
    idle: 0.000000
    snake: 0.000000
    updown: 0.003906
    wave: 0.996094
Anomaly prediction: -0.183448
97% idle
99% idle
Predictions (DSP: 4.000000 ms., Classification: 0 ms., Anomaly: 0ms.): 
#Classification results:
    idle: 0.000000
    snake: 0.000000
    updown: 0.003906
    wave: 0.996094
Anomaly prediction: -0.183448
```
