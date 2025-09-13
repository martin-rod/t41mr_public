#pragma once

#include "SDT.h"

#include "AudioSignal.h"
#include "Band.h"

// SD library uses 8.3 filenames
constexpr const char *configFilename = "/config.txt";

constexpr float32_t DEFAULT_POWER_LEVEL = 10.0;
constexpr uint32_t DEFAULT_CENTER_TUNE = 1000;
constexpr uint32_t DEFAULT_FINE_TUNE = 50;

constexpr int DEFAULT_KEYER_WPM = 15;
constexpr char DEFAULT_FREQ_SEP_CHARACTER = '.';
constexpr const char DEFAULT_MAP_FILE_NAME[50] = "Greenwich.bmp";
constexpr float DEFAULT_QTH_LAT = 51.477;
constexpr float DEFAULT_QTH_LON = 0.0;
constexpr const char DEFAULT_CALLSIGN[10] = "Callsign";
constexpr const char DEFAULT_TIMEZONE[] = "UTC";
constexpr TimeFormatEnum DEFAULT_TIME_FORMAT = TimeFormatEnum::TIME_24H;
constexpr ItuRegionEnum DEFAULT_ITU_REGION = ItuRegionEnum::ITU_REGION_2;
constexpr KeyTypeEnum DEFAULT_KEY_TYPE = KeyTypeEnum::STRAIGHT_KEY;
constexpr PaddleFlipEnum DEFAULT_PADDLE_FLIP = PaddleFlipEnum::RIGHT_PADDLE_DAH;

constexpr float DEFAULT_SPEAKER_SCALE = 1.0;
constexpr float DEFAULT_HEADPHONE_SCALE = 10.0;

// This adjusts for RF gain differences in the QSD.  QSD should use a value of 3000.  QSD2 should use a value of 1000.0.
constexpr float DEFAULT_RFGAIN_SCALE = 3000.0;

constexpr HwVersion DEFAULT_HW_VERSION = HwVersion::T41_1_AL_JACK;

constexpr size_t ConfigDataFavoriteFreqsSize = 13;

// Configuration data structure.
struct config_t {
  char versionSettings[10] = "T41EEE.9";
  bool AGCMode = true;
  float32_t AGCThreshold = -40.0;
  int audioVolume = 30;
  int rfGainCurrent = 0;
  int rfGain[NUMBER_OF_BANDS]{0};
  bool autoGain = true;
  bool autoSpectrum = true;
  unsigned int centerTuneStep = DEFAULT_CENTER_TUNE;
  unsigned int fineTuneStep = DEFAULT_FINE_TUNE;
  float32_t transmitPowerLevel = DEFAULT_POWER_LEVEL;
  AudioState audioOut = AudioState::SPEAKER; // Default audio output is speaker.
  int nrOptionSelect = 0;
  int currentScale = 1;
  SpectrumZoomState spectrum_zoom = SpectrumZoomState::SPECTRUM_ZOOM_2;
  int CWFilterIndex = 5; // Off
  int paddleDit = KEYER_DIT_INPUT_TIP;
  int paddleDah = KEYER_DAH_INPUT_RING;
  int decoderFlag = false;
  unsigned int morseDecodeSensitivity = 2000; // Greg KF5N February 19, 2025
  KeyTypeEnum keyType = DEFAULT_KEY_TYPE;
  int currentWPM = DEFAULT_KEYER_WPM;
  int CWOffset = 2; // Default is 750 Hz.
  unsigned int sidetoneSpeaker = 40;
  unsigned int sidetoneHeadphone = 40;
  unsigned int cwTransmitDelay = 1000;
  VfoState activeVFO = VfoState::VFO_A;
  int currentBand = BandEnum::BAND_40M;
  int currentBandA = BandEnum::BAND_40M;
  int currentBandB = BandEnum::BAND_40M;

  // initilaze later according to ITU_REGION
  unsigned int currentFreqA = 7100000;
  unsigned int currentFreqB = 7030000;

  int equalizerRec[EQUALIZER_CELL_COUNT] = {
      100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  };
  // Provide equalizer optimized for SSB voice based on Neville's tests.  KF5N November 2, 2023
  int equalizerXmt[EQUALIZER_CELL_COUNT] = {
      -50, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  float micThreshold = -15.0;
  float micCompRatio = 5.0;
  float micGain = 0.0;     // Open Audio gain element.  Gain is in dB.
  float LPFcoeff = 0.0;    // 4 bytes
  float NR_PSI = 0.0;      // 4 bytes
  float NR_alpha = 0.95;   // 4 bytes
  float NR_beta = 0.85;    // 4 bytes
  float omegaN = 200.0;    // 4 bytes
  float pll_fmax = 4000.0; // 4 bytes
  // powerOutCW and powerOutSSB are derived from the TX power setting and calibration factors.
  float powerOutCW[NUMBER_OF_BANDS] = {
      0.035, 0.035, 0.035, 0.035, 0.035, 0.035, 0.035,
  };
  float powerOutSSB[NUMBER_OF_BANDS] = {0.035, 0.035, 0.035, 0.035, 0.035, 0.035, 0.035};
  unsigned int favoriteFreqs[ConfigDataFavoriteFreqsSize] = {
      3560000, 3690000, 7030000, 7200000, 14060000, 14200000, 21060000, 21285000, 28060000, 28365000, 5000000, 10000000, 15000000,
  };

  // initilaze later according to ITU_REGION
  unsigned int lastFrequencies[NUMBER_OF_BANDS][2] = {
      {3590000, 3560000},   {7190000, 7030000},   {14285000, 14060000}, {18130000, 18096000},
      {21400000, 21060000}, {24950000, 24906000}, {28365000, 28060000},
  };

  Sideband lastSideband[NUMBER_OF_BANDS] = {
      Sideband::LOWER, Sideband::LOWER, Sideband::UPPER, Sideband::UPPER, Sideband::UPPER, Sideband::UPPER, Sideband::UPPER,
  };
  unsigned int centerFreq = 7030000;
  char mapFileName[filenameSize] = {0};
  char myTimeZone[10] = {0};
  int separationCharacter = (int)DEFAULT_FREQ_SEP_CHARACTER;
  PaddleFlipEnum paddleFlip = DEFAULT_PADDLE_FLIP;
  float myLong = DEFAULT_QTH_LON;
  float myLat = DEFAULT_QTH_LAT;
  bool compressorFlag = false;
  bool xmitEQFlag = false;
  bool receiveEQFlag = false;
  bool cessb = false;

  char myCallsign[10] = {0};

  TimeFormatEnum timeFormat = DEFAULT_TIME_FORMAT;
  ItuRegionEnum ituRegion = DEFAULT_ITU_REGION;

  float speakerScale = DEFAULT_SPEAKER_SCALE;
  float headphoneScale = DEFAULT_HEADPHONE_SCALE;
  float rfgainScale = DEFAULT_RFGAIN_SCALE;

  HwVersion hwVersion = DEFAULT_HW_VERSION;
};

extern config_t ConfigData;

constexpr size_t ConfigDataVersionSettingsSize = sizeof(ConfigData.versionSettings);
constexpr size_t ConfigDataMapFileNameSize = sizeof(ConfigData.mapFileName);
constexpr size_t ConfigDataMyTimeZoneSize = sizeof(ConfigData.myTimeZone);
constexpr size_t ConfigDataCallsignSize = sizeof(ConfigData.myCallsign);

class ConfigurationData {
public:
  void loadConfiguration(const char *filename, config_t &ConfigData);
  void saveConfiguration(const char *filename, const config_t &ConfigData);
  void printConfiguration(const config_t &ConfigData);
};

extern ConfigurationData configurationData;

void ConfigurationAccordingToItuRegion(void);
