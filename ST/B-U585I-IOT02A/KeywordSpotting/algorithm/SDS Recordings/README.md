# SDS Recordings with B-U585I-IOT02A Board

## Overview
This folder contains SDS recordings captured with the **B-U585I-IOT02A** board.  
The recordings include microphone (audio in) data as well as machine learning (ML) inference results.
Metadata files describe the format and usage of these recordings.

## File description
- **ML_In.n.sds** – streams of microphone data used as input for ML.  
- **ML_Out.n.sds** – streams of ML inference results.  
- **ML_In.sds.yml** – metadata description for all `ML_In.n.sds` files.  
- **ML_Out.sds.yml** – metadata description for all `ML_Out.n.sds` files.  

### Available Recordings

`ML_In.0.sds` file contains **10 seconds** of microphone data (with 2 'helloworld').  
`ML_Out.0.sds` file contains **10 seconds** of ML inference results.

## Visualization
You can graphically represent SDS files using the **SDS-View** utility.

1. Copy the `sds-view.py` file from the SDS installation (`/utilities` sub-folder) into this folder.  
2. Run the following command to visualize an input recording:

   ```bash
   python sds-view.py -i ML_In.0.sds -y ML_In.sds.yml
   ```
   Example visualization:

   ![ML_In.0.sds](ML_In.0.sds.png)

3. To visualize an output recording:

   ```bash
   python sds-view.py -i ML_Out.0.sds -y ML_Out.sds.yml
   ```
   Example visualization:

   ![ML_Out.3.sds](ML_Out.0.sds.png)

## Audio data
You can convert recorded microphone data with **SDS-Convert** utility into a .wav format.

1. Copy the `sds-convert.py` file from the SDS installation (`/utilities` sub-folder) into this folder.  
2. Run the following command to convert input recording into a **.wav** file:

   ```bash
   python sds-convert.py audio_wav -i ML_In.0.sds -o ML_In.0.wav -y ML_In.sds.yml
   ```

You can now play `ML_In.0.wav` file in a media player to check what was recorded and fed to ML algorithm.
