#include "JsonData.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME CFG

#include "trace.h"

FLASHMEM void jsonLoadIfExist(JsonDocument &j, const char *name, char *dst, size_t length) {
  TRACE_LEVEL(TR_L_TRACE);
  if (j[name].is<const char *>()) {
    strlcpy(dst, j[name], length);
  } else {
    TRACE_T41(TR_L_ERROR, "%s not found", name);
  }
}

FLASHMEM void jsonLoadIfExist(JsonDocument &j, const char *name, bool &dst) {
  TRACE_LEVEL(TR_L_TRACE);
  if (j[name].is<bool>()) {
    dst = j[name];
  } else {
    TRACE_T41(TR_L_ERROR, "%s not found", name);
  }
}

FLASHMEM void jsonLoadIfExist(JsonDocument &j, const char *name, int &dst) {
  TRACE_LEVEL(TR_L_TRACE);
  if (j[name].is<int>()) {
    dst = j[name];
  } else {
    TRACE_T41(TR_L_ERROR, "%s not found", name);
  }
}

FLASHMEM void jsonLoadIfExist(JsonDocument &j, const char *name, float32_t &dst) {
  TRACE_LEVEL(TR_L_TRACE);
  if (j[name].is<float32_t>()) {
    dst = j[name];
  } else {
    TRACE_T41(TR_L_ERROR, "%s not found", name);
  }
}

FLASHMEM void jsonLoadIfExist(JsonDocument &j, const char *name, unsigned int &dst) {
  TRACE_LEVEL(TR_L_TRACE);
  if (j[name].is<uint32_t>()) {
    dst = j[name];
  } else {
    TRACE_T41(TR_L_ERROR, "%s not found", name);
  }
}

FLASHMEM void jsonLoadIfExistLoop(JsonDocument &j, const char *name, float dst[], size_t length) {
  TRACE_LEVEL(TR_L_TRACE);
  for (size_t i = 0; i < length; i++) {
    if (j[name][i].is<float>()) {
      dst[i] = j[name][i];
    } else {
      TRACE_T41(TR_L_ERROR, "%s[%u] not found", name, i);
    }
  }
}

FLASHMEM void jsonLoadIfExistLoop(JsonDocument &j, const char *name, int dst[], size_t length) {
  TRACE_LEVEL(TR_L_TRACE);
  for (size_t i = 0; i < length; i++) {
    if (j[name][i].is<int>()) {
      dst[i] = j[name][i];
    } else {
      TRACE_T41(TR_L_ERROR, "%s[%u] not found", name, i);
    }
  }
}

FLASHMEM void jsonLoadIfExistLoop(JsonDocument &j, const char *name, unsigned int dst[], size_t length) {
  TRACE_LEVEL(TR_L_TRACE);
  for (size_t i = 0; i < length; i++) {
    if (j[name][i].is<unsigned int>()) {
      dst[i] = j[name][i];
    } else {
      TRACE_T41(TR_L_ERROR, "%s[%u] not found", name, i);
    }
  }
}

// Custom converter - RadioMode
FLASHMEM bool convertToJson(const RadioMode &src, JsonVariant dst) {
  int state;
  state = static_cast<int>(src);
  return dst.set(state);
}

// Custom converter - RadioMode
FLASHMEM RadioMode convertFromJson(JsonVariantConst src, RadioMode &dst) {
  int state;
  state = src.as<int>();
  return dst = static_cast<RadioMode>(state);
}

// Custom converter - AudioState
FLASHMEM bool convertToJson(const AudioState &src, JsonVariant dst) {
  int state;
  state = static_cast<int>(src);
  return dst.set(state);
}

// Custom converter - AudioState
FLASHMEM AudioState convertFromJson(JsonVariantConst src, AudioState &dst) {
  int state;
  state = src.as<int>();
  return dst = static_cast<AudioState>(state);
}

// Custom converter - Sideband
FLASHMEM bool convertToJson(const Sideband &src, JsonVariant dst) {
  int state;
  state = static_cast<int>(src);
  return dst.set(state);
}

// Custom converter - Sideband
FLASHMEM Sideband convertFromJson(JsonVariantConst src, Sideband &dst) {
  int state;
  state = src.as<int>();
  return dst = static_cast<Sideband>(state);
}

// Custom converter - VfoState
FLASHMEM bool convertToJson(const VfoState &src, JsonVariant dst) {
  int state;
  state = static_cast<int>(src);
  return dst.set(state);
}

// Custom converter - VfoState
FLASHMEM VfoState convertFromJson(JsonVariantConst src, VfoState &dst) {
  int state;
  state = src.as<int>();
  return dst = static_cast<VfoState>(state);
}

// Custom converter - SpectrumZoomState
FLASHMEM bool convertToJson(const SpectrumZoomState &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - SpectrumZoomState
FLASHMEM SpectrumZoomState convertFromJson(JsonVariantConst src, SpectrumZoomState &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<SpectrumZoomState>(state);
}

// Custom converter - KeyTypeEnum
FLASHMEM bool convertToJson(const KeyTypeEnum &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - KeyTypeEnum
FLASHMEM KeyTypeEnum convertFromJson(JsonVariantConst src, KeyTypeEnum &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<KeyTypeEnum>(state);
}

// Custom converter - PaddleFlipEnum
FLASHMEM bool convertToJson(const PaddleFlipEnum &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - PaddleFlipEnum
FLASHMEM PaddleFlipEnum convertFromJson(JsonVariantConst src, PaddleFlipEnum &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<PaddleFlipEnum>(state);
}

// Custom converter - HwVersion
FLASHMEM bool convertToJson(const HwVersion &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - HwVersion
FLASHMEM HwVersion convertFromJson(JsonVariantConst src, HwVersion &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<HwVersion>(state);
}

// Custom converter - ItuRegionEnum
FLASHMEM bool convertToJson(const ItuRegionEnum &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - ItuRegionEnum
FLASHMEM ItuRegionEnum convertFromJson(JsonVariantConst src, ItuRegionEnum &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<ItuRegionEnum>(state);
}

// Custom converter - TimeFormatEnum
FLASHMEM bool convertToJson(const TimeFormatEnum &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - TimeFormatEnum
FLASHMEM TimeFormatEnum convertFromJson(JsonVariantConst src, TimeFormatEnum &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<TimeFormatEnum>(state);
}

// Custom converter - SerialPort0Mode
FLASHMEM bool convertToJson(const SerialPort0Mode &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - SerialPort0Mode
FLASHMEM SerialPort0Mode convertFromJson(JsonVariantConst src, SerialPort0Mode &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<SerialPort0Mode>(state);
}

// Custom converter - SerialPort1Mode
FLASHMEM bool convertToJson(const SerialPort1Mode &src, JsonVariant dst) {
  int32_t state;
  state = static_cast<int32_t>(src);
  return dst.set(state);
}

// Custom converter - SerialPort1Mode
FLASHMEM SerialPort1Mode convertFromJson(JsonVariantConst src, SerialPort1Mode &dst) {
  int32_t state;
  state = src.as<int32_t>();
  return dst = static_cast<SerialPort1Mode>(state);
}
