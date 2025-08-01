#pragma once

#include "SDT.h"

#include "AudioSignal.h"

void jsonLoadIfExist(JsonDocument &j, const char *name, char *dst, size_t length);
void jsonLoadIfExist(JsonDocument &j, const char *name, bool &dst);
void jsonLoadIfExist(JsonDocument &j, const char *name, int &dst);
void jsonLoadIfExist(JsonDocument &j, const char *name, float32_t &dst);
void jsonLoadIfExist(JsonDocument &j, const char *name, unsigned int &dst);

void jsonLoadIfExistLoop(JsonDocument &j, const char *name, float dst[], size_t length);
void jsonLoadIfExistLoop(JsonDocument &j, const char *name, int dst[], size_t length);
void jsonLoadIfExistLoop(JsonDocument &j, const char *name, unsigned int dst[], size_t length);

// Custom converters
bool convertToJson(const RadioMode &src, JsonVariant dst);
RadioMode convertFromJson(JsonVariantConst src, RadioMode &dst);

bool convertToJson(const AudioState &src, JsonVariant dst);
AudioState convertFromJson(JsonVariantConst src, AudioState &dst);

bool convertToJson(const Sideband &src, JsonVariant dst);
Sideband convertFromJson(JsonVariantConst src, Sideband &dst);

bool convertToJson(const VfoState &src, JsonVariant dst);
VfoState convertFromJson(JsonVariantConst src, VfoState &dst);

bool convertToJson(const SpectrumZoomState &src, JsonVariant dst);
SpectrumZoomState convertFromJson(JsonVariantConst src, SpectrumZoomState &dst);

bool convertToJson(const KeyTypeEnum &src, JsonVariant dst);
KeyTypeEnum convertFromJson(JsonVariantConst src, KeyTypeEnum &dst);

bool convertToJson(const PaddleFlipEnum &src, JsonVariant dst);
PaddleFlipEnum convertFromJson(JsonVariantConst src, PaddleFlipEnum &dst);

bool convertToJson(const HwVersion &src, JsonVariant dst);
HwVersion convertFromJson(JsonVariantConst src, HwVersion &dst);
