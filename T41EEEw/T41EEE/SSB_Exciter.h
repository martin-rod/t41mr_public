#pragma once

#include "SDT.h"

#include "Display.h"
#include "Eeprom.h"

// Class SSBCalibrate.  Greg KF5N July 10, 2024

// This function sets the microphone gain and compressor parameters.  Greg KF5N March 9, 2025.
void updateMic();

/*****
  Purpose: Retrieve I and Q samples from the Open Audio Library CESSB object at
48ksps. Apply calibration factors and scale for power.  Push the modified I and
Q back into the Teensy audio system to drive the Audio Adapter outputs which are
connected to the QSE.

  Parameter list: none

  Return value;
    void
    Notes:
    There are several actions in this function
    1.  Read in the I and Q data from the CESSB object.  16 blocks of 128
samples each.
    2.  Apply magnitude and phase calibration factors.
    3.  Scale for power.
    4.  Push the blocks back to the Teensy audio system to the Audio Adapter
outputs at 48ksps.
*****/
/**********************************************************************************
   AFP 12-31-20 Get samples from queue buffers Teensy Audio Library stores ADC
   data in two buffers size=128, Q_in_L and Q_in_R as initiated from the audio
   lib. Then the buffers are  read into two arrays in blocks of 128 up to
   N_BLOCKS.  The arrays are of size BUFFER_SIZE*N_BLOCKS.  BUFFER_SIZE is
   128. N_BLOCKS = FFT_L / 2 / BUFFER_SIZE * (uint32_t)DF; // should be 16
   with DF == 8 and FFT_L = 512 BUFFER_SIZE * N_BLOCKS = 2048 samples
   **********************************************************************************/
void ExciterIQData();

/*****
  Purpose: Set the current band relay ON or OFF.  Reduce relay cycling.  Greg
KF5N March 24, 2025

  Parameter list:
    void

  Return value;
    void
*****/
void SetBandRelay();

/*****
  Purpose: Allow user to set the microphone compression level.
           A typical value is -20 dB.
  Parameter list:
    void

  Return value;
    void
*****/
void SetCompressionThreshold();

/*****
  Purpose: Allow user to set the microphone compression ratio.
           A typical value is in the 10 to 100 range.
  Parameter list:
    void

  Return value
    void
*****/
void SetCompressionRatio();

/*****
  Purpose: Set microphone gain.  The default is 0 dB.

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void MicGainSet();
