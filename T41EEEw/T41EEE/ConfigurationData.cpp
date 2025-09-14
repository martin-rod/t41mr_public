#include "ConfigurationData.h"

#include "JsonData.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME CFG

#include "trace.h"

config_t ConfigData;
ConfigurationData configurationData;

// Loads the ConfigData configuration from a file
FLASHMEM void ConfigurationData::loadConfiguration(const char *filename, config_t &ConfigData) {
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

  jsonLoadIfExist(doc, "versionSettings", ConfigData.versionSettings, sizeof(ConfigData.versionSettings));
  jsonLoadIfExist(doc, "AGCMode", ConfigData.AGCMode);
  jsonLoadIfExist(doc, "audioVolume", ConfigData.audioVolume);
  jsonLoadIfExist(doc, "rfGainCurrent", ConfigData.rfGainCurrent);
  jsonLoadIfExist(doc, "autoGain", ConfigData.autoGain);
  jsonLoadIfExist(doc, "autoSpectrum", ConfigData.autoSpectrum);
  jsonLoadIfExist(doc, "centerTuneStep", ConfigData.centerTuneStep);
  jsonLoadIfExist(doc, "fineTuneStep", ConfigData.fineTuneStep);
  jsonLoadIfExist(doc, "transmitPowerLevel", ConfigData.transmitPowerLevel);
  jsonLoadIfExist(doc, "audioOut", reinterpret_cast<int &>(ConfigData.audioOut));
  jsonLoadIfExist(doc, "nrOptionSelect", ConfigData.nrOptionSelect);
  jsonLoadIfExist(doc, "currentScale", ConfigData.currentScale);
  jsonLoadIfExist(doc, "spectrum_zoom", reinterpret_cast<int &>(ConfigData.spectrum_zoom));
  jsonLoadIfExist(doc, "CWFilterIndex", ConfigData.CWFilterIndex);
  jsonLoadIfExist(doc, "paddleDit", ConfigData.paddleDit);
  jsonLoadIfExist(doc, "paddleDah", ConfigData.paddleDah);
  jsonLoadIfExist(doc, "decoderFlag", ConfigData.decoderFlag);
  jsonLoadIfExist(doc, "morseDecodeSensitivity", ConfigData.morseDecodeSensitivity);
  jsonLoadIfExist(doc, "keyType", reinterpret_cast<int &>(ConfigData.keyType));
  jsonLoadIfExist(doc, "currentWPM", ConfigData.currentWPM);
  jsonLoadIfExist(doc, "CWOffset", ConfigData.CWOffset);
  jsonLoadIfExist(doc, "sidetoneSpeaker", ConfigData.sidetoneSpeaker);
  jsonLoadIfExist(doc, "sidetoneHeadphone", ConfigData.sidetoneHeadphone);
  jsonLoadIfExist(doc, "cwTransmitDelay", ConfigData.cwTransmitDelay);
  jsonLoadIfExist(doc, "activeVFO", reinterpret_cast<int &>(ConfigData.activeVFO));
  jsonLoadIfExist(doc, "currentBand", ConfigData.currentBand);
  jsonLoadIfExist(doc, "currentBandA", ConfigData.currentBandA);
  jsonLoadIfExist(doc, "currentBandB", ConfigData.currentBandB);
  jsonLoadIfExist(doc, "currentFreqA", ConfigData.currentFreqA);
  jsonLoadIfExist(doc, "currentFreqB", ConfigData.currentFreqB);
  jsonLoadIfExist(doc, "micThreshold", ConfigData.micThreshold);
  jsonLoadIfExist(doc, "micCompRatio", ConfigData.micCompRatio);
  jsonLoadIfExist(doc, "micGain", ConfigData.micGain);
  jsonLoadIfExist(doc, "LPFcoeff", ConfigData.LPFcoeff);
  jsonLoadIfExist(doc, "NR_PSI", ConfigData.NR_PSI);
  jsonLoadIfExist(doc, "NR_alpha", ConfigData.NR_alpha);
  jsonLoadIfExist(doc, "NR_beta", ConfigData.NR_beta);
  jsonLoadIfExist(doc, "omegaN", ConfigData.omegaN);
  jsonLoadIfExist(doc, "pll_fmax", ConfigData.pll_fmax);
  jsonLoadIfExist(doc, "centerFreq", ConfigData.centerFreq);
  jsonLoadIfExist(doc, "mapFileName", ConfigData.mapFileName, sizeof(ConfigData.mapFileName));
  jsonLoadIfExist(doc, "myTimeZone", ConfigData.myTimeZone, sizeof(ConfigData.myTimeZone));
  jsonLoadIfExist(doc, "separationCharacter", ConfigData.separationCharacter);
  jsonLoadIfExist(doc, "paddleFlip", reinterpret_cast<int &>(ConfigData.paddleFlip));
  jsonLoadIfExist(doc, "myLong", ConfigData.myLong);
  jsonLoadIfExist(doc, "myLat", ConfigData.myLat);
  jsonLoadIfExist(doc, "compressorFlag", ConfigData.compressorFlag);
  jsonLoadIfExist(doc, "xmitEQFlag", ConfigData.xmitEQFlag);
  jsonLoadIfExist(doc, "receiveEQFlag", ConfigData.receiveEQFlag);
  jsonLoadIfExist(doc, "autoGain", ConfigData.autoGain);

  jsonLoadIfExistLoop(doc, "rfGain", ConfigData.rfGain, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "powerOutCW", ConfigData.powerOutCW, BandEnum::NUMBER_OF_BANDS);
  jsonLoadIfExistLoop(doc, "powerOutSSB", ConfigData.powerOutSSB, BandEnum::NUMBER_OF_BANDS);

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    for (size_t j = 0; j < 2; j++) {
      if (doc["lastFrequencies"][i][j].is<unsigned int>()) {
        ConfigData.lastFrequencies[i][j] = doc["lastFrequencies"][i][j];
      } else {
        TRACE_T41(TR_L_ERROR, "lastFrequencies[%u][%u] not found", i, j);
      }
    }
  }

  jsonLoadIfExistLoop(doc, "lastSideband", reinterpret_cast<int *>(ConfigData.lastSideband), BandEnum::NUMBER_OF_BANDS);

  jsonLoadIfExistLoop(doc, "equalizerRec", ConfigData.equalizerRec, EQUALIZER_CELL_COUNT);
  jsonLoadIfExistLoop(doc, "equalizerXmt", ConfigData.equalizerXmt, EQUALIZER_CELL_COUNT);

  jsonLoadIfExistLoop(doc, "favoriteFreqs", ConfigData.favoriteFreqs, ConfigDataFavoriteFreqsSize);

  jsonLoadIfExist(doc, "serialPort0Mode", reinterpret_cast<int &>(ConfigData.serialPort0Mode));
  jsonLoadIfExist(doc, "serialPort1Mode", reinterpret_cast<int &>(ConfigData.serialPort1Mode));

  // MyConfiguration part
  jsonLoadIfExist(doc, "myCallsign", ConfigData.myCallsign, sizeof(ConfigData.myCallsign));
  jsonLoadIfExist(doc, "speakerScale", ConfigData.speakerScale);
  jsonLoadIfExist(doc, "headphoneScale", ConfigData.headphoneScale);
  jsonLoadIfExist(doc, "rfgainScale", ConfigData.rfgainScale);
  jsonLoadIfExist(doc, "hwVersion", reinterpret_cast<int &>(ConfigData.hwVersion));

  file.close();
}

// Saves the configuration ConfigData to a file
FLASHMEM void ConfigurationData::saveConfiguration(const char *filename, const config_t &ConfigData) {
  TRACE_LEVEL(TR_L_TRACE);

  JsonDocument doc;

  doc["versionSettings"] = ConfigData.versionSettings;
  doc["AGCMode"] = ConfigData.AGCMode;
  doc["AGCThreshold"] = ConfigData.AGCThreshold;
  doc["audioVolume"] = ConfigData.audioVolume;
  doc["rfGainCurrent"] = ConfigData.rfGainCurrent;
  doc["autoGain"] = ConfigData.autoGain;
  doc["autoSpectrum"] = ConfigData.autoSpectrum;
  doc["centerTuneStep"] = ConfigData.centerTuneStep;
  doc["fineTuneStep"] = ConfigData.fineTuneStep;
  doc["transmitPowerLevel"] = ConfigData.transmitPowerLevel;
  doc["audioOut"] = ConfigData.audioOut;
  doc["nrOptionSelect"] = ConfigData.nrOptionSelect;
  doc["currentScale"] = ConfigData.currentScale;
  doc["spectrum_zoom"] = ConfigData.spectrum_zoom;
  doc["CWFilterIndex"] = ConfigData.CWFilterIndex;
  doc["paddleDit"] = ConfigData.paddleDit;
  doc["paddleDah"] = ConfigData.paddleDah;
  doc["decoderFlag"] = ConfigData.decoderFlag;
  doc["morseDecodeSensitivity"] = ConfigData.morseDecodeSensitivity;
  doc["keyType"] = ConfigData.keyType;
  doc["currentWPM"] = ConfigData.currentWPM;
  doc["CWOffset"] = ConfigData.CWOffset;
  doc["sidetoneSpeaker"] = ConfigData.sidetoneSpeaker;
  doc["sidetoneHeadphone"] = ConfigData.sidetoneHeadphone;
  doc["cwTransmitDelay"] = ConfigData.cwTransmitDelay;
  doc["activeVFO"] = ConfigData.activeVFO;
  doc["currentBand"] = ConfigData.currentBand;
  doc["currentBandA"] = ConfigData.currentBandA;
  doc["currentBandB"] = ConfigData.currentBandB;
  doc["currentFreqA"] = ConfigData.currentFreqA;
  doc["currentFreqB"] = ConfigData.currentFreqB;
  doc["micThreshold"] = ConfigData.micThreshold;
  doc["micCompRatio"] = ConfigData.micCompRatio;
  doc["micGain"] = ConfigData.micGain;
  doc["LPFcoeff"] = ConfigData.LPFcoeff;
  doc["NR_PSI"] = ConfigData.NR_PSI;
  doc["NR_alpha"] = ConfigData.NR_alpha;
  doc["NR_beta"] = ConfigData.NR_beta;
  doc["omegaN"] = ConfigData.omegaN;
  doc["pll_fmax"] = ConfigData.pll_fmax;
  doc["centerFreq"] = ConfigData.centerFreq;
  doc["mapFileName"] = ConfigData.mapFileName;
  doc["myTimeZone"] = ConfigData.myTimeZone;
  doc["separationCharacter"] = ConfigData.separationCharacter;
  doc["paddleFlip"] = ConfigData.paddleFlip;
  doc["myLong"] = ConfigData.myLong;
  doc["myLat"] = ConfigData.myLat;
  doc["compressorFlag"] = ConfigData.compressorFlag;
  doc["xmitEQFlag"] = ConfigData.xmitEQFlag;
  doc["receiveEQFlag"] = ConfigData.receiveEQFlag;
  doc["autoGain"] = ConfigData.autoGain;

  doc["serialPort0Mode"] = ConfigData.serialPort0Mode;
  doc["serialPort1Mode"] = ConfigData.serialPort1Mode;

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["rfGain"][i] = ConfigData.rfGain[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["powerOutCW"][i] = ConfigData.powerOutCW[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["powerOutSSB"][i] = ConfigData.powerOutSSB[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    for (int j = 0; j < 2; j++) {
      doc["lastFrequencies"][i][j] = ConfigData.lastFrequencies[i][j];
    }
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["lastSideband"][i] = ConfigData.lastSideband[i];
  }

  for (int i = 0; i < EQUALIZER_CELL_COUNT; i++) {
    doc["equalizerRec"][i] = ConfigData.equalizerRec[i];
  }

  for (int i = 0; i < EQUALIZER_CELL_COUNT; i++) {
    doc["equalizerXmt"][i] = ConfigData.equalizerXmt[i];
  }

  for (size_t i = 0; i < ConfigDataFavoriteFreqsSize; i++) {
    doc["favoriteFreqs"][i] = ConfigData.favoriteFreqs[i];
  }

  // MyConfiguration part
  doc["myCallsign"] = ConfigData.myCallsign;
  doc["speakerScale"] = ConfigData.speakerScale;
  doc["headphoneScale"] = ConfigData.headphoneScale;
  doc["rfgainScale"] = ConfigData.rfgainScale;
  doc["hwVersion"] = ConfigData.hwVersion;

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
}

FLASHMEM void ConfigurationData::printConfiguration(const config_t &ConfigData) {
  TRACE_LEVEL(TR_L_TRACE);

  JsonDocument doc;

  doc["versionSettings"] = ConfigData.versionSettings;
  doc["AGCMode"] = ConfigData.AGCMode;
  doc["AGCThreshold"] = ConfigData.AGCThreshold;
  doc["audioVolume"] = ConfigData.audioVolume;
  doc["rfGainCurrent"] = ConfigData.rfGainCurrent;
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["rfGain"][i] = ConfigData.rfGain[i];
  }
  doc["autoGain"] = ConfigData.autoGain;
  doc["autoSpectrum"] = ConfigData.autoSpectrum;
  doc["centerTuneStep"] = ConfigData.centerTuneStep;
  doc["fineTuneStep"] = ConfigData.fineTuneStep;
  doc["transmitPowerLevel"] = ConfigData.transmitPowerLevel;
  doc["audioOut"] = ConfigData.audioOut;
  doc["nrOptionSelect"] = ConfigData.nrOptionSelect;
  doc["currentScale"] = ConfigData.currentScale;
  doc["spectrum_zoom"] = ConfigData.spectrum_zoom;
  doc["CWFilterIndex"] = ConfigData.CWFilterIndex;
  doc["paddleDit"] = ConfigData.paddleDit;
  doc["paddleDah"] = ConfigData.paddleDah;
  doc["decoderFlag"] = ConfigData.decoderFlag;
  doc["morseDecodeSensitivity"] = ConfigData.morseDecodeSensitivity;
  doc["keyType"] = ConfigData.keyType;
  doc["currentWPM"] = ConfigData.currentWPM;
  doc["CWOffset"] = ConfigData.CWOffset;
  doc["sidetoneSpeaker"] = ConfigData.sidetoneSpeaker;
  doc["sidetoneHeadphone"] = ConfigData.sidetoneHeadphone;
  doc["cwTransmitDelay"] = ConfigData.cwTransmitDelay;
  doc["activeVFO"] = ConfigData.activeVFO;
  doc["currentBand"] = ConfigData.currentBand;
  doc["currentBandA"] = ConfigData.currentBandA;
  doc["currentBandB"] = ConfigData.currentBandB;
  doc["currentFreqA"] = ConfigData.currentFreqA;
  doc["currentFreqB"] = ConfigData.currentFreqB;
  for (int i = 0; i < EQUALIZER_CELL_COUNT; i++) {
    doc["equalizerRec"][i] = ConfigData.equalizerRec[i];
  }

  for (int i = 0; i < EQUALIZER_CELL_COUNT; i++) {
    doc["equalizerXmt"][i] = ConfigData.equalizerXmt[i];
  }
  doc["micThreshold"] = ConfigData.micThreshold;
  doc["micCompRatio"] = ConfigData.micCompRatio;
  doc["micGain"] = ConfigData.micGain;
  doc["LPFcoeff"] = ConfigData.LPFcoeff;
  doc["NR_PSI"] = ConfigData.NR_PSI;
  doc["NR_alpha"] = ConfigData.NR_alpha;
  doc["NR_beta"] = ConfigData.NR_beta;
  doc["omegaN"] = ConfigData.omegaN;
  doc["pll_fmax"] = ConfigData.pll_fmax;
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["powerOutCW"][i] = ConfigData.powerOutCW[i];
  }

  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["powerOutSSB"][i] = ConfigData.powerOutSSB[i];
  }
  for (size_t i = 0; i < ConfigDataFavoriteFreqsSize; i++) {
    doc["favoriteFreqs"][i] = ConfigData.favoriteFreqs[i];
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    for (int j = 0; j < 2; j++) {
      doc["lastFrequencies"][i][j] = ConfigData.lastFrequencies[i][j];
    }
  }
  for (size_t i = 0; i < BandEnum::NUMBER_OF_BANDS; i++) {
    doc["lastSideband"][i] = ConfigData.lastSideband[i];
  }
  doc["centerFreq"] = ConfigData.centerFreq;
  doc["mapFileName"] = ConfigData.mapFileName;
  doc["myTimeZone"] = ConfigData.myTimeZone;
  doc["separationCharacter"] = ConfigData.separationCharacter;
  doc["paddleFlip"] = ConfigData.paddleFlip;
  doc["myLong"] = ConfigData.myLong;
  doc["myLat"] = ConfigData.myLat;
  doc["compressorFlag"] = ConfigData.compressorFlag;
  doc["xmitEQFlag"] = ConfigData.xmitEQFlag;
  doc["receiveEQFlag"] = ConfigData.receiveEQFlag;
  doc["cessb"] = ConfigData.cessb;
  doc["autoGain"] = ConfigData.autoGain;
  doc["myCallsign"] = ConfigData.myCallsign;
  doc["timeFormat"] = ConfigData.timeFormat;
  doc["ituRegion"] = ConfigData.ituRegion;
  doc["speakerScale"] = ConfigData.speakerScale;
  doc["headphoneScale"] = ConfigData.headphoneScale;
  doc["rfgainScale"] = ConfigData.rfgainScale;
  doc["hwVersion"] = ConfigData.hwVersion;

  doc["serialPort0Mode"] = ConfigData.serialPort0Mode;
  doc["serialPort1Mode"] = ConfigData.serialPort1Mode;

  TRACE_T41(TR_L_INFO, "--- print json begin");
  size_t written = serializeJsonPretty(doc, Serial);
  Serial.println();
  TRACE_T41(TR_L_INFO, "--- print json end bytes:%u", written);
}

void ConfigurationAccordingToItuRegion(void) {
  switch (ConfigData.ituRegion) {
  case ItuRegionEnum::ITU_REGION_1:
    ConfigData.currentFreqA = 7100000;
    break;
  case ItuRegionEnum::ITU_REGION_2:
    ConfigData.currentFreqA = 7200000;
    break;
  case ItuRegionEnum::ITU_REGION_3:
    ConfigData.currentFreqA = 7100000;
    break;
  }
}
