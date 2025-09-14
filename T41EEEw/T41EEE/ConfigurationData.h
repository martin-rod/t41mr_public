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
  // for checking of config_t version too
  char versionSettings[16] = "T41EEE.9";

  //////
  // alphabetical order - primitive types

  bool AGCMode = true;
  float32_t AGCThreshold = -40.0;
  VfoState activeVFO = VfoState::VFO_A;
  AudioState audioOut = AudioState::SPEAKER;
  int audioVolume = 30;
  bool autoGain = true;
  bool autoSpectrum = true;

  int CWFilterIndex = 5; // Off
  int CWOffset = 2;      // Default is 750 Hz.
  unsigned int centerFreq = 7030000;
  unsigned int centerTuneStep = DEFAULT_CENTER_TUNE;
  bool cessb = false;
  int currentScale = 1;
  int currentBand = BandEnum::BAND_40M;
  int currentBandA = BandEnum::BAND_40M;
  int currentBandB = BandEnum::BAND_40M;
  // initilaze later according to ITU_REGION
  unsigned int currentFreqA = 7100000;
  unsigned int currentFreqB = 7030000;
  int currentWPM = DEFAULT_KEYER_WPM;
  bool compressorFlag = false;
  unsigned int cwTransmitDelay = 1000;

  int decoderFlag = false;

  unsigned int fineTuneStep = DEFAULT_FINE_TUNE;

  float headphoneScale = DEFAULT_HEADPHONE_SCALE;
  HwVersion hwVersion = DEFAULT_HW_VERSION;

  ItuRegionEnum ituRegion = DEFAULT_ITU_REGION;

  KeyTypeEnum keyType = DEFAULT_KEY_TYPE;

  float LPFcoeff = 0.0;

  char mapFileName[filenameSize] = {0};
  float micCompRatio = 5.0;
  float micGain = 0.0; // Open Audio gain element.  Gain is in dB.
  float micThreshold = -15.0;
  unsigned int morseDecodeSensitivity = 2000;
  char myCallsign[10] = {0};
  float myLat = DEFAULT_QTH_LAT;
  float myLong = DEFAULT_QTH_LON;
  char myTimeZone[10] = {0};

  float NR_alpha = 0.95; // 4 bytes
  float NR_beta = 0.85;  // 4 bytes
  float NR_PSI = 0.0;    // 4 bytes
  int nrOptionSelect = 0;

  float omegaN = 200.0; // 4 bytes

  int paddleDah = KEYER_DAH_INPUT_RING;
  int paddleDit = KEYER_DIT_INPUT_TIP;
  PaddleFlipEnum paddleFlip = DEFAULT_PADDLE_FLIP;
  float pll_fmax = 4000.0; // 4 bytes

  bool receiveEQFlag = false;
  int rfGainCurrent = 0;
  float rfgainScale = DEFAULT_RFGAIN_SCALE;

  SerialPort0Mode serialPort0Mode = SerialPort0Mode::TRACE;
  SerialPort1Mode serialPort1Mode = SerialPort1Mode::FT8_PTT_RTS;
  int separationCharacter = (int)DEFAULT_FREQ_SEP_CHARACTER;
  unsigned int sidetoneHeadphone = 40;
  unsigned int sidetoneSpeaker = 40;
  float speakerScale = DEFAULT_SPEAKER_SCALE;
  SpectrumZoomState spectrum_zoom = SpectrumZoomState::SPECTRUM_ZOOM_2;

  TimeFormatEnum timeFormat = DEFAULT_TIME_FORMAT;
  float32_t transmitPowerLevel = DEFAULT_POWER_LEVEL;

  bool xmitEQFlag = false;

  //////
  // alphabetical order - arrays
  int equalizerRec[EQUALIZER_CELL_COUNT] = {
      100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  };
  // Provide equalizer optimized for SSB voice based on Neville's tests.  KF5N November 2, 2023
  int equalizerXmt[EQUALIZER_CELL_COUNT] = {
      -50, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  unsigned int favoriteFreqs[ConfigDataFavoriteFreqsSize] = {
      3560000, 3690000, 7030000, 7200000, 14060000, 14200000, 21060000, 21285000, 28060000, 28365000, 5000000, 10000000, 15000000,
  };
  unsigned int lastFrequencies[NUMBER_OF_BANDS][2] = {
      {3590000, 3560000},   {7190000, 7030000},   {14285000, 14060000}, {18130000, 18096000},
      {21400000, 21060000}, {24950000, 24906000}, {28365000, 28060000},
  };
  Sideband lastSideband[NUMBER_OF_BANDS] = {
      Sideband::LOWER, Sideband::LOWER, Sideband::UPPER, Sideband::UPPER, Sideband::UPPER, Sideband::UPPER, Sideband::UPPER,
  };
  // powerOutCW and powerOutSSB are derived from the TX power setting and calibration factors.
  float powerOutCW[NUMBER_OF_BANDS] = {
      0.035, 0.035, 0.035, 0.035, 0.035, 0.035, 0.035,
  };
  float powerOutSSB[NUMBER_OF_BANDS] = {0.035, 0.035, 0.035, 0.035, 0.035, 0.035, 0.035};
  int rfGain[NUMBER_OF_BANDS]{0};
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

private:
  bool serializeConfiguration(const config_t &ConfigData, std::string &textData);
};

extern ConfigurationData configurationData;

void ConfigurationAccordingToItuRegion(void);
