#include "CalibrationData.h"

#include "ConfigurationData.h"
#include "JsonData.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME CFG

#include "trace.h"

calibration_t CalData;
CalibrationData calibrationData;

// Loads the ConfigData configuration from a file
FLASHMEM void CalibrationData::loadCalibration(const char *filename, calibration_t &CalData) {
  TRACE_LEVEL(TR_L_TRACE);

  File file = SD.open(filename, FILE_READ);
  if (not file) {
    TRACE_T41(TR_L_ERROR, "cannot open file filename:%s", filename);
    return;
  }

  // https://arduinojson.org/v7/assistant
  JsonDocument doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    TRACE_T41(TR_L_ERROR, "Failed to deserilaze filename:%s", filename);
    file.close();
    return;
  }

  TRACE_T41(TR_L_INFO, "read filename:%s", filename);

  jsonLoadIfExist(doc, "freqCorrectionFactor", CalData.freqCorrectionFactor);
  jsonLoadIfExist(doc, "buttonThresholdPressed", CalData.buttonThresholdPressed);
  jsonLoadIfExist(doc, "buttonThresholdReleased", CalData.buttonThresholdReleased);
  jsonLoadIfExist(doc, "buttonRepeatDelay", CalData.buttonRepeatDelay);
  jsonLoadIfExist(doc, "CWradioCalComplete", CalData.CWradioCalComplete);
  jsonLoadIfExist(doc, "SSBradioCalComplete", CalData.SSBradioCalComplete);
  jsonLoadIfExist(doc, "dBm_calibration", CalData.dBm_calibration);

  jsonLoadIfExistLoop(doc, "CWPowerCalibrationFactor", CalData.CWPowerCalibrationFactor, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "SSBPowerCalibrationFactor", CalData.SSBPowerCalibrationFactor, BandEnum::NUMBER_OF_BANDS);

  jsonLoadIfExistLoop(doc, "IQCWRXAmpCorrectionFactorLSB", CalData.IQCWRXAmpCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQCWRXPhaseCorrectionFactorLSB", CalData.IQCWRXPhaseCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQCWAmpCorrectionFactorLSB", CalData.IQCWAmpCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQCWPhaseCorrectionFactorLSB", CalData.IQCWPhaseCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);

  jsonLoadIfExistLoop(doc, "IQSSBRXAmpCorrectionFactorLSB", CalData.IQSSBRXAmpCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQSSBRXPhaseCorrectionFactorLSB", CalData.IQSSBRXPhaseCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQSSBAmpCorrectionFactorLSB", CalData.IQSSBAmpCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQSSBPhaseCorrectionFactorLSB", CalData.IQSSBPhaseCorrectionFactorLSB, BandEnum::NUMBER_OF_BANDS);

  jsonLoadIfExistLoop(doc, "IQCWRXAmpCorrectionFactorUSB", CalData.IQCWRXAmpCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQCWRXPhaseCorrectionFactorUSB", CalData.IQCWRXPhaseCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQCWAmpCorrectionFactorUSB", CalData.IQCWAmpCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQCWPhaseCorrectionFactorUSB", CalData.IQCWPhaseCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);

  jsonLoadIfExistLoop(doc, "IQSSBRXAmpCorrectionFactorUSB", CalData.IQSSBRXAmpCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQSSBRXPhaseCorrectionFactorUSB", CalData.IQSSBRXPhaseCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQSSBAmpCorrectionFactorUSB", CalData.IQSSBAmpCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "IQSSBPhaseCorrectionFactorUSB", CalData.IQSSBPhaseCorrectionFactorUSB, BandEnum::NUMBER_OF_BANDS);

  jsonLoadIfExistLoop(doc, "switchValues", CalData.switchValues, NUMBER_OF_SWITCHES);

#ifdef QSE2
  for (int i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    CalData.iDCoffsetCW[i] = doc["iDCoffsetCW"][i];
  }
  for (int i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    CalData.qDCoffsetCW[i] = doc["qDCoffsetCW"][i];
  }
  for (int i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    CalData.iDCoffsetSSB[i] = doc["iDCoffsetSSB"][i];
  }
  for (int i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    CalData.qDCoffsetSSB[i] = doc["qDCoffsetSSB"][i];
  }
  CalData.dacOffsetCW = doc["dacOffsetCW"] | 0;
  CalData.dacOffsetSSB = doc["dacOffsetSSB"] | 0;
#endif

  file.close();
}

// Saves the configuration CalData to a file or writes to serial.  toFile == true for file, false for serial.
FLASHMEM void CalibrationData::saveCalibration(const char *filename, const calibration_t &CalData, bool toFile) {
  TRACE_LEVEL(TR_L_TRACE);

  JsonDocument doc;

  doc["freqCorrectionFactor"] = CalData.freqCorrectionFactor;
  doc["buttonThresholdPressed"] = CalData.buttonThresholdPressed;
  doc["buttonThresholdReleased"] = CalData.buttonThresholdReleased;
  doc["buttonRepeatDelay"] = CalData.buttonRepeatDelay;
  doc["CWradioCalComplete"] = CalData.CWradioCalComplete;
  doc["SSBradioCalComplete"] = CalData.SSBradioCalComplete;
  doc["dBm_calibration"] = CalData.dBm_calibration;

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["CWPowerCalibrationFactor"][i] = CalData.CWPowerCalibrationFactor[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["SSBPowerCalibrationFactor"][i] = CalData.SSBPowerCalibrationFactor[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWRXAmpCorrectionFactorLSB"][i] = CalData.IQCWRXAmpCorrectionFactorLSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWRXPhaseCorrectionFactorLSB"][i] = CalData.IQCWRXPhaseCorrectionFactorLSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWAmpCorrectionFactorLSB"][i] = CalData.IQCWAmpCorrectionFactorLSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWPhaseCorrectionFactorLSB"][i] = CalData.IQCWPhaseCorrectionFactorLSB[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBRXAmpCorrectionFactorLSB"][i] = CalData.IQSSBRXAmpCorrectionFactorLSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBRXPhaseCorrectionFactorLSB"][i] = CalData.IQSSBRXPhaseCorrectionFactorLSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBAmpCorrectionFactorLSB"][i] = CalData.IQSSBAmpCorrectionFactorLSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBPhaseCorrectionFactorLSB"][i] = CalData.IQSSBPhaseCorrectionFactorLSB[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWRXAmpCorrectionFactorUSB"][i] = CalData.IQCWRXAmpCorrectionFactorUSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWRXPhaseCorrectionFactorUSB"][i] = CalData.IQCWRXPhaseCorrectionFactorUSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWAmpCorrectionFactorUSB"][i] = CalData.IQCWAmpCorrectionFactorUSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQCWPhaseCorrectionFactorUSB"][i] = CalData.IQCWPhaseCorrectionFactorUSB[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBRXAmpCorrectionFactorUSB"][i] = CalData.IQSSBRXAmpCorrectionFactorUSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBRXPhaseCorrectionFactorUSB"][i] = CalData.IQSSBRXPhaseCorrectionFactorUSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBAmpCorrectionFactorUSB"][i] = CalData.IQSSBAmpCorrectionFactorUSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["IQSSBPhaseCorrectionFactorUSB"][i] = CalData.IQSSBPhaseCorrectionFactorUSB[i];
  }

  for (size_t i = 0; i < NUMBER_OF_SWITCHES; i++) {
    doc["switchValues"][i] = CalData.switchValues[i];
  }

#ifdef QSE2
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["iDCoffsetCW"][i] = CalData.iDCoffsetCW[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["qDCoffsetCW"][i] = CalData.qDCoffsetCW[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["iDCoffsetSSB"][i] = CalData.iDCoffsetSSB[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["qDCoffsetSSB"][i] = CalData.qDCoffsetSSB[i];
  }
  doc["dacOffsetCW"] = CalData.dacOffsetCW;
  doc["dacOffsetSSB"] = CalData.dacOffsetSSB;
#endif

  if (toFile) {
    File file = SD.open(filename, FILE_WRITE_BEGIN);
    if (!file) {
      TRACE_T41(TR_L_ERROR, "Failed to create file filename:%s", filename);
      return;
    }

    size_t written = serializeJsonPretty(doc, file);
    if (written == 0) {
      TRACE_T41(TR_L_ERROR, "Failed to serilaze filename:%s", filename);
      file.close();
      return;
    }

    TRACE_T41(TR_L_INFO, "write filename:%s bytes:%u", filename, written);

    file.close();
  } else {
    TRACE_T41(TR_L_INFO, "--- print json begin  filename:%s", filename);
    size_t written = serializeJsonPretty(doc, Serial);
    Serial.println();
    TRACE_T41(TR_L_INFO, "--- print json end  filename:%s bytes:%u", filename, written);
  }
}

// Using ARRL table: https://www.arrl.org/frequency-bands.
const uint32_t calFrequenciesItuRegion1[BandEnum::NUMBER_OF_BANDS][2] = {
    {3590000, 3560000},   {7190000, 7030000},   {14285000, 14060000}, {18130000, 18096000},
    {21400000, 21060000}, {24950000, 24906000}, {28365000, 28060000},
};

const uint32_t calFrequenciesItuRegion2[BandEnum::NUMBER_OF_BANDS][2] = {
    {3985000, 3560000},   {7290000, 7030000},   {14285000, 14060000}, {18130000, 18096000},
    {21400000, 21060000}, {24950000, 24906000}, {28385000, 28060000},
};

const uint32_t calFrequenciesItuRegion3[BandEnum::NUMBER_OF_BANDS][2] = {
    {3885000, 3560000},   {7190000, 7030000},   {14285000, 14060000}, {18130000, 18096000},
    {21400000, 21060000}, {24950000, 24906000}, {28385000, 28060000},
};

void CalibrationAccordingToItuRegion(void) {
  switch (ConfigData.ituRegion) {
  case ItuRegionEnum::ITU_REGION_1:
    for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
      CalData.calFrequencies[i][0] = calFrequenciesItuRegion1[i][0];
      CalData.calFrequencies[i][1] = calFrequenciesItuRegion1[i][1];
    }
    break;
  case ItuRegionEnum::ITU_REGION_2:
    for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
      CalData.calFrequencies[i][0] = calFrequenciesItuRegion2[i][0];
      CalData.calFrequencies[i][1] = calFrequenciesItuRegion2[i][1];
    }
    break;
  case ItuRegionEnum::ITU_REGION_3:
    for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
      CalData.calFrequencies[i][0] = calFrequenciesItuRegion3[i][0];
      CalData.calFrequencies[i][1] = calFrequenciesItuRegion3[i][1];
    }
    break;
  }
}
