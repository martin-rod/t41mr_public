#include "MyConfigurationFile.h"

#include "JsonData.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME CFG

#include "trace.h"

// load one time configuration data  from json file
FLASHMEM void loadMyConfigutation(const char *filename, config_t &data) {
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

  jsonLoadIfExist(doc, "mapFileName", data.mapFileName, sizeof(data.mapFileName));
  jsonLoadIfExist(doc, "myLong", data.myLong);
  jsonLoadIfExist(doc, "myLat", data.myLat);

  jsonLoadIfExist(doc, "ituRegion", reinterpret_cast<int &>(data.ituRegion));

  jsonLoadIfExist(doc, "myCallsign", data.myCallsign, sizeof(data.myCallsign));
  jsonLoadIfExist(doc, "myTimeZone", data.myTimeZone, sizeof(data.myTimeZone));

  jsonLoadIfExist(doc, "keyType", reinterpret_cast<int &>(data.keyType));
  jsonLoadIfExist(doc, "paddleFlip", reinterpret_cast<int &>(data.paddleFlip));

  jsonLoadIfExist(doc, "speakerScale", data.speakerScale);
  jsonLoadIfExist(doc, "headphoneScale", data.headphoneScale);
  jsonLoadIfExist(doc, "rfgainScale", data.rfgainScale);
  jsonLoadIfExist(doc, "hwVersion", reinterpret_cast<int &>(data.hwVersion));

  jsonLoadIfExist(doc, "serialPort0Mode", reinterpret_cast<int &>(ConfigData.serialPort0Mode));
  jsonLoadIfExist(doc, "serialPort1Mode", reinterpret_cast<int &>(ConfigData.serialPort1Mode));
}

// load one time calibration data  from json file
FLASHMEM void loadMyCalibration(const char *filename, calibration_t &data) {
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

  jsonLoadIfExist(doc, "freqCorrectionFactor", data.freqCorrectionFactor);
}

FLASHMEM void copyMyConfigutation(config_t &data) {
  constexpr const char MY_MAP_FILE_NAME[] = "Cestice.bmp";
  constexpr float MY_QTH_LAT = 49.167821;
  constexpr float MY_QTH_LON = 13.803697;

  constexpr const char MY_CALLSIGN[] = "OK4MR";
  constexpr const char MY_TIMEZONE[] = "CET";
  constexpr TimeFormatEnum MY_TIME_FORMAT = TimeFormatEnum::TIME_24H;
  constexpr ItuRegionEnum MY_ITU_REGION = ItuRegionEnum::ITU_REGION_1;

  constexpr KeyTypeEnum MY_KEY_TYPE = KeyTypeEnum::STRAIGHT_KEY;
  constexpr PaddleFlipEnum MY_PADDLE_FLIP = PaddleFlipEnum::RIGHT_PADDLE_DIT;

  // The audio amplifier gain may need to be adjusted for the best volume range
  constexpr float MY_SPEAKER_SCALE = 1.0;
  constexpr float MY_HEADPHONE_SCALE = 10.0;

  // This adjusts for RF gain differences in the QSD.  QSD should use a value of 3000.  QSD2 should use a value of 1000.0.
  constexpr float MY_RFGAIN_SCALE = 3000.0;

  constexpr HwVersion MY_HW_VERSION = HwVersion::T41_1_4SQRP;

  strlcpy(data.mapFileName, MY_MAP_FILE_NAME, ConfigDataMapFileNameSize);
  data.myLat = MY_QTH_LAT;
  data.myLong = MY_QTH_LON;

  strlcpy(data.myCallsign, MY_CALLSIGN, ConfigDataCallsignSize);
  strlcpy(data.myTimeZone, MY_TIMEZONE, ConfigDataMyTimeZoneSize);
  data.timeFormat = MY_TIME_FORMAT;
  data.ituRegion = MY_ITU_REGION;

  data.keyType = MY_KEY_TYPE;
  data.paddleFlip = MY_PADDLE_FLIP;
  data.speakerScale = MY_SPEAKER_SCALE;
  data.headphoneScale = MY_HEADPHONE_SCALE;
  data.rfgainScale = MY_RFGAIN_SCALE;
  data.hwVersion = MY_HW_VERSION;
}

FLASHMEM void copyMyCalibration(calibration_t &data) {
  // The nominal frequency calibration. This can be set here permanently after determining the unique value for your radio.
  constexpr int MY_FREQUENCYCAL = -3200;

  data.freqCorrectionFactor = MY_FREQUENCYCAL;
}
