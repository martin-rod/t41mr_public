#pragma once

#include "SDT.h"

#include "Band.h"
#include "Button.h"

// SD library uses 8.3 filenames
constexpr const char *calFilename = "/calibration.txt";

constexpr int DEFAULT_FREQUENCYCAL = 0;

// Calibration data structure.
struct calibration_t {

  // Conventional crystal with freq offset needs a correction factor
  // for TCXO 0
  int freqCorrectionFactor = DEFAULT_FREQUENCYCAL;
  int switchValues[NUMBER_OF_SWITCHES] = {924, 870, 817, 769, 713, 669, 616, 565, 513, 459, 407, 356, 298, 242, 183, 131, 67, 10};

  float CWPowerCalibrationFactor[NUMBER_OF_BANDS] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
  float SSBPowerCalibrationFactor[NUMBER_OF_BANDS] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

  float IQCWRXAmpCorrectionFactorLSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQCWRXPhaseCorrectionFactorLSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  float IQCWAmpCorrectionFactorLSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQCWPhaseCorrectionFactorLSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  float IQSSBRXAmpCorrectionFactorLSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQSSBRXPhaseCorrectionFactorLSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  float IQSSBAmpCorrectionFactorLSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQSSBPhaseCorrectionFactorLSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};

  float IQCWRXAmpCorrectionFactorUSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQCWRXPhaseCorrectionFactorUSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  float IQCWAmpCorrectionFactorUSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQCWPhaseCorrectionFactorUSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  float IQSSBRXAmpCorrectionFactorUSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQSSBRXPhaseCorrectionFactorUSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  float IQSSBAmpCorrectionFactorUSB[NUMBER_OF_BANDS] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
  float IQSSBPhaseCorrectionFactorUSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};

  // initilaze later according to ITU_REGION
  uint32_t calFrequencies[NUMBER_OF_BANDS][2] = {
      {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
  };

  int buttonThresholdPressed = 944;        // switchValues[0] + WIGGLE_ROOM
  int buttonThresholdReleased = 964;       // buttonThresholdPressed + WIGGLE_ROOM
  unsigned int buttonRepeatDelay = 300000; // Increased to 300000 from 200000 to
                                           // better handle cheap, wornout buttons.
#ifdef QSE2
  q15_t iDCoffsetCW[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  q15_t qDCoffsetCW[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  q15_t iDCoffsetSSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  q15_t qDCoffsetSSB[NUMBER_OF_BANDS] = {0, 0, 0, 0, 0, 0, 0};
  q15_t dacOffsetCW = 1500;  // This must be "tuned" for each radio and/or Audio Adapter board.
  q15_t dacOffsetSSB = 1500; // This must be "tuned" for each radio and/or Audio Adapter board.
#endif
  bool CWradioCalComplete = false;
  bool SSBradioCalComplete = false;
  float32_t dBm_calibration = 50.0; // This parameter is adjusted in the calibration menu.
};

extern calibration_t CalData;

class CalibrationData {
public:
  void loadCalibration(const char *filename, calibration_t &CalData);
  void saveCalibration(const char *filename, const calibration_t &CalData, bool toFile);
};

extern CalibrationData calibrationData;

void CalibrationAccordingToItuRegion(void);
