
#include "SDT.h"

// JSON format used to save and read from SD card.  This was derived from a JSON example from the ArduinoJSON library.

// Custom converters are needed for the mode states.
bool convertToJson(const RadioMode& src, JsonVariant dst) {
int state;
state = static_cast<int>(src);
//  char buf[32];
//  strftime(buf, sizeof(buf), "%FT%TZ", &src);

  return dst.set(state);
}


RadioMode convertFromJson(JsonVariantConst src, RadioMode& dst) {
int state;
state = src.as<int>();
return dst = static_cast<RadioMode>(state);
}


// Loads the EEPROMData configuration from a file
FLASHMEM void JSON::loadConfiguration(const char *filename, config_t &EEPROMData) {
  // Open file for reading
  File file = SD.open(filename);

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  // StaticJsonDocument<512> doc;
  JsonDocument doc;  // This uses the heap.

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(F("Failed to read configuration file."));
    return;
  }

  // Copy values from the JsonDocument to the EEPROMData
  strlcpy(EEPROMData.versionSettings, doc["versionSettings"] | "t41pp.0", 10);
  EEPROMData.AGCMode = doc["AGCMode"];
  EEPROMData.audioVolume = doc["audioVolume"];
  EEPROMData.rfGainCurrent = doc["rfGainCurrent"];
  for (int i = 0; i < NUMBER_OF_BANDS; i++) EEPROMData.rfGain[i] = doc["rfGain"][i];
  EEPROMData.autoGain = doc["autoGain"];
  EEPROMData.autoSpectrum = doc["autoSpectrum"];
  EEPROMData.spectrumNoiseFloor = doc["spectrumNoiseFloor"];  // This is a constant.  This does not need to be included in user data.
  EEPROMData.centerTuneStep = doc["centerTuneStep"];
  EEPROMData.fineTuneStep = doc["fineTuneStep"];
  EEPROMData.transmitPowerLevel = doc["transmitPowerLevel"];
  EEPROMData.xmtMode = doc["xmtMode"];
  EEPROMData.nrOptionSelect = doc["nrOptionSelect"];
  EEPROMData.currentScale = doc["currentScale"];
  EEPROMData.spectrum_zoom = doc["spectrum_zoom"];
  EEPROMData.CWFilterIndex = doc["CWFilterIndex"];
  EEPROMData.paddleDit = doc["paddleDit"];
  EEPROMData.paddleDah = doc["paddleDah"];
  EEPROMData.decoderFlag = doc["decoderFlag"];
  EEPROMData.keyType = doc["keyType"];
  EEPROMData.currentWPM = doc["currentWPM"];
  EEPROMData.CWOffset = doc["CWOffset"];  
  EEPROMData.sidetoneVolume = doc["sidetoneVolume"];
  EEPROMData.cwTransmitDelay = doc["cwTransmitDelay"];
  EEPROMData.activeVFO = doc["activeVFO"];
  EEPROMData.currentBand = doc["currentBand"];
  EEPROMData.currentBandA = doc["currentBandA"];
  EEPROMData.currentBandB = doc["currentBandB"];
  EEPROMData.currentFreqA = doc["currentFreqA"];
  EEPROMData.currentFreqB = doc["currentFreqB"];
  EEPROMData.freqCorrectionFactor = doc["freqCorrectionFactor"];
  for (int i = 0; i < 14; i++) EEPROMData.equalizerRec[i] = doc["equalizerRec"][i];
  for (int i = 0; i < 14; i++) EEPROMData.equalizerXmt[i] = doc["equalizerXmt"][i];
  EEPROMData.equalizerXmt[0] = doc["equalizerXmt"][0];
  EEPROMData.currentMicThreshold = doc["currentMicThreshold"];
  EEPROMData.currentMicCompRatio = doc["currentMicCompRatio"];
  //EEPROMData.currentMicAttack = doc["currentMicAttack"];
  //EEPROMData.currentMicRelease = doc["currentMicRelease"];
  EEPROMData.currentMicGain = doc["currentMicGain"];
  for (int i = 0; i < 18; i++) EEPROMData.switchValues[i] = doc["switchValues"][i];
  EEPROMData.LPFcoeff = doc["LPFcoeff"];
  EEPROMData.NR_PSI = doc["NR_PSI"];
  EEPROMData.NR_alpha = doc["NR_alpha"];
  EEPROMData.NR_beta = doc["NR_beta"];
  EEPROMData.omegaN = doc["omegaN"];
  EEPROMData.pll_fmax = doc["pll_fmax"];
  EEPROMData.powerOutCW[0] = doc["powerOutCW"][0];
  EEPROMData.powerOutSSB[0] = doc["powerOutSSB"][0];
  for (int i = 0; i < 7; i++) EEPROMData.CWPowerCalibrationFactor[i] = doc["CWPowerCalibrationFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.SSBPowerCalibrationFactor[i] = doc["SSBPowerCalibrationFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQCWRXAmpCorrectionFactor[i] = doc["IQCWRXAmpCorrectionFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQCWRXPhaseCorrectionFactor[i] = doc["IQCWRXPhaseCorrectionFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQCWAmpCorrectionFactor[i] = doc["IQCWAmpCorrectionFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQCWPhaseCorrectionFactor[i] = doc["IQCWPhaseCorrectionFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQSSBRXAmpCorrectionFactor[i] = doc["IQSSBRXAmpCorrectionFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQSSBRXPhaseCorrectionFactor[i] = doc["IQSSBRXPhaseCorrectionFactor"][i];  
  for (int i = 0; i < 7; i++) EEPROMData.IQSSBAmpCorrectionFactor[i] = doc["IQSSBAmpCorrectionFactor"][i];
  for (int i = 0; i < 7; i++) EEPROMData.IQSSBPhaseCorrectionFactor[i] = doc["IQSSBPhaseCorrectionFactor"][i];

  for (int i = 0; i < 13; i++) EEPROMData.favoriteFreqs[i] = doc["favoriteFreqs"][i];
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 2; j++) EEPROMData.lastFrequencies[i][j] = doc["lastFrequencies"][i][j];
  }
  EEPROMData.centerFreq = doc["centerFreq"];
  //EEPROMData.mapFileName  = doc["mapFileName"] | "Boston";
  strlcpy(EEPROMData.mapFileName, doc["mapFileName"] | "Boston", 50);
  //EEPROMData.myCall  = doc["myCall"];
//  strlcpy(EEPROMData.myCall, doc["myCall"] | "Your Call", 10);
  //EEPROMData.myTimeZone  = doc["myTimeZone"];
  strlcpy(EEPROMData.myTimeZone, doc["myTimeZone"] | "EST", 10);
  EEPROMData.separationCharacter = doc["separationCharacter"];
  EEPROMData.paddleFlip = doc["paddleFlip"];
  EEPROMData.sdCardPresent = doc["sdCardPresent"];
  EEPROMData.myLong = doc["myLong"];
  EEPROMData.myLat = doc["myLat"];
  for (int i = 0; i < 7; i++) EEPROMData.currentNoiseFloor[i] = doc["currentNoiseFloor"][i];
  EEPROMData.compressorFlag = doc["compressorFlag"];
  EEPROMData.xmitEQFlag = doc["xmitEQFlag"];
  EEPROMData.receiveEQFlag = doc["receiveEQFlag"];
//  EEPROMData.calFreq = doc["calFreq"];
  EEPROMData.buttonThresholdPressed = doc["buttonThresholdPressed"] | 944;
  EEPROMData.buttonThresholdReleased = doc["buttonThresholdReleased"] | 964;
  EEPROMData.buttonRepeatDelay = doc["buttonRepeatDelay"] | 300000;
  EEPROMData.autoGain = doc["autoGain"] | true;
  #ifdef QSE2
  for (int i = 0; i < 7; i++) EEPROMData.iDCoffsetCW[i] = doc["iDCoffsetCW"][i];
  for (int i = 0; i < 7; i++) EEPROMData.qDCoffsetCW[i] = doc["qDCoffsetCW"][i];
  for (int i = 0; i < 7; i++) EEPROMData.iDCoffsetSSB[i] = doc["iDCoffsetSSB"][i];
  for (int i = 0; i < 7; i++) EEPROMData.qDCoffsetSSB[i] = doc["qDCoffsetSSB"][i];  
  EEPROMData.dacOffsetCW = doc["dacOffsetCW"] | 0;
  EEPROMData.dacOffsetSSB = doc["dacOffsetSSB"] | 0; 
  #endif
  EEPROMData.CWradioCalComplete = doc["CWradioCalComplete"] | false;
  EEPROMData.SSBradioCalComplete = doc["SSBradioCalComplete"] | false;

  // How to copy strings:
  //  strlcpy(EEPROMData.myCall,                  // <- destination
  //          doc["myCall"],  // <- source
  //          sizeof(EEPROMData.myCall));         // <- destination's capacity

  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}


// Saves the configuration EEPROMData to a file or writes to serial.  toFile == true for file, false for serial.
FLASHMEM void JSON::saveConfiguration(const char *filename, const config_t &EEPROMData, bool toFile) {

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/assistant to compute the capacity.
  //StaticJsonDocument<256> doc;  // This uses the stack.
  JsonDocument doc;  // This uses the heap.

  // Set the values in the document
  doc["versionSettings"] = EEPROMData.versionSettings;    // Fix for version not updating in JSON file.  KF5N March 18, 2024.
  doc["AGCMode"] = EEPROMData.AGCMode;
  doc["audioVolume"] = EEPROMData.audioVolume;
  doc["rfGainCurrent"] = EEPROMData.rfGainCurrent;
  for (int i = 0; i < NUMBER_OF_BANDS; i++) doc["rfGain"][i] = EEPROMData.rfGain[i];
  doc["autoGain"] = EEPROMData.autoGain;
  doc["autoSpectrum"] = EEPROMData.autoSpectrum;
  doc["spectrumNoiseFloor"] = EEPROMData.spectrumNoiseFloor;
  doc["centerTuneStep"] = EEPROMData.centerTuneStep;
  doc["fineTuneStep"] = EEPROMData.fineTuneStep;
  doc["transmitPowerLevel"] = EEPROMData.transmitPowerLevel;
  doc["xmtMode"] = EEPROMData.xmtMode;
  doc["nrOptionSelect"] = EEPROMData.nrOptionSelect;
  doc["currentScale"] = EEPROMData.currentScale;
  doc["spectrum_zoom"] = EEPROMData.spectrum_zoom;
  doc["CWFilterIndex"] = EEPROMData.CWFilterIndex;
  doc["paddleDit"] = EEPROMData.paddleDit;
  doc["paddleDah"] = EEPROMData.paddleDah;
  doc["decoderFlag"] = EEPROMData.decoderFlag;
  doc["keyType"] = EEPROMData.keyType;
  doc["currentWPM"] = EEPROMData.currentWPM;
  doc["CWOffset"] = EEPROMData.CWOffset;  
  doc["sidetoneVolume"] = EEPROMData.sidetoneVolume;
  doc["cwTransmitDelay"] = EEPROMData.cwTransmitDelay;
  doc["activeVFO"] = EEPROMData.activeVFO;
  doc["currentBand"] = EEPROMData.currentBand;
  doc["currentBandA"] = EEPROMData.currentBandA;
  doc["currentBandB"] = EEPROMData.currentBandB;
  doc["currentFreqA"] = EEPROMData.currentFreqA;
  doc["currentFreqB"] = EEPROMData.currentFreqB;
  doc["freqCorrectionFactor"] = EEPROMData.freqCorrectionFactor;
  for (int i = 0; i < 14; i++) doc["equalizerRec"][i] = EEPROMData.equalizerRec[i];
  for (int i = 0; i < 14; i++) doc["equalizerXmt"][i] = EEPROMData.equalizerXmt[i];
  doc["currentMicThreshold"] = EEPROMData.currentMicThreshold;
  doc["currentMicCompRatio"] = EEPROMData.currentMicCompRatio;
//  doc["currentMicAttack"] = EEPROMData.currentMicAttack;
//  doc["currentMicRelease"] = EEPROMData.currentMicRelease;
  doc["currentMicGain"] = EEPROMData.currentMicGain;
  for (int i = 0; i < 18; i++) doc["switchValues"][i] = EEPROMData.switchValues[i];
  doc["LPFcoeff"] = EEPROMData.LPFcoeff;
  doc["NR_PSI"] = EEPROMData.NR_PSI;
  doc["NR_alpha"] = EEPROMData.NR_alpha;
  doc["NR_beta"] = EEPROMData.NR_beta;
  doc["omegaN"] = EEPROMData.omegaN;
  doc["pll_fmax"] = EEPROMData.pll_fmax;
  for (int i = 0; i < 7; i++) doc["powerOutCW"][i] = EEPROMData.powerOutCW[i];
  for (int i = 0; i < 7; i++) doc["powerOutSSB"][i] = EEPROMData.powerOutSSB[i];
  for (int i = 0; i < 7; i++) {
    doc["CWPowerCalibrationFactor"][i] = EEPROMData.CWPowerCalibrationFactor[i];
  }
  for (int i = 0; i < 7; i++) doc["SSBPowerCalibrationFactor"][i] = EEPROMData.SSBPowerCalibrationFactor[i];
  for (int i = 0; i < 7; i++) doc["IQCWRXAmpCorrectionFactor"][i] =   EEPROMData.IQCWRXAmpCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQCWRXPhaseCorrectionFactor"][i] = EEPROMData.IQCWRXPhaseCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQCWAmpCorrectionFactor"][i] =   EEPROMData.IQCWAmpCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQCWPhaseCorrectionFactor"][i] = EEPROMData.IQCWPhaseCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQSSBRXAmpCorrectionFactor"][i] =   EEPROMData.IQSSBRXAmpCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQSSBRXPhaseCorrectionFactor"][i] = EEPROMData.IQSSBRXPhaseCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQSSBAmpCorrectionFactor"][i] =   EEPROMData.IQSSBAmpCorrectionFactor[i];
  for (int i = 0; i < 7; i++) doc["IQSSBPhaseCorrectionFactor"][i] = EEPROMData.IQSSBPhaseCorrectionFactor[i];  
  for (int i = 0; i < 13; i++) doc["favoriteFreqs"][i] = EEPROMData.favoriteFreqs[i];
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 2; j++) doc["lastFrequencies"][i][j] = EEPROMData.lastFrequencies[i][j];
  }
  doc["centerFreq"] = EEPROMData.centerFreq;
  doc["mapFileName"] = EEPROMData.mapFileName;
//  doc["myCall"] = EEPROMData.myCall;
  doc["myTimeZone"] = EEPROMData.myTimeZone;
  doc["separationCharacter"] = EEPROMData.separationCharacter;
  doc["paddleFlip"] = EEPROMData.paddleFlip;
  doc["sdCardPresent"] = EEPROMData.sdCardPresent;
  doc["myLong"] = EEPROMData.myLong;
  doc["myLat"] = EEPROMData.myLat;
  for (int i = 0; i < 7; i++) doc["currentNoiseFloor"][i] = EEPROMData.currentNoiseFloor[i];
  doc["compressorFlag"] = EEPROMData.compressorFlag;
  doc["xmitEQFlag"] = EEPROMData.xmitEQFlag;
  doc["receiveEQFlag"] = EEPROMData.receiveEQFlag;
//  doc["calFreq"] = EEPROMData.calFreq;
  doc["buttonThresholdPressed"] = EEPROMData.buttonThresholdPressed;
  doc["buttonThresholdReleased"] = EEPROMData.buttonThresholdReleased;
  doc["buttonRepeatDelay"] = EEPROMData.buttonRepeatDelay;
  doc["autoGain"] = EEPROMData.autoGain;
  #ifdef QSE2
  for (int i = 0; i < 7; i++) doc["iDCoffsetCW"][i] = EEPROMData.iDCoffsetCW[i];
  for (int i = 0; i < 7; i++) doc["qDCoffsetCW"][i] = EEPROMData.qDCoffsetCW[i];
  for (int i = 0; i < 7; i++) doc["iDCoffsetSSB"][i] = EEPROMData.iDCoffsetSSB[i];
  for (int i = 0; i < 7; i++) doc["qDCoffsetSSB"][i] = EEPROMData.qDCoffsetSSB[i];
  doc["dacOffsetCW"] = EEPROMData.dacOffsetCW;
  doc["dacOffsetSSB"] = EEPROMData.dacOffsetSSB;
  #endif
  doc["CWradioCalComplete"] = EEPROMData.CWradioCalComplete;
  doc["SSBradioCalComplete"] = EEPROMData.SSBradioCalComplete;

  if (toFile) {
    // Delete existing file, otherwise EEPROMData is appended to the file
    SD.remove(filename);
    // Open file for writing
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
      Serial.println(F("Failed to create file"));
      return;
    }
    // Serialize JSON to file
    if (serializeJsonPretty(doc, file) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    // Close the file
    file.close();
  } else {
    // Write to the serial port.

    serializeJsonPretty(doc, Serial);

  }
}


// Prints the content of a file to the Serial
void JSON::printFile(const char *filename) {
  // Open file for reading
  File file = SD.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}


