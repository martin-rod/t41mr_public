#pragma once

#include "SDT.h"

struct band {
  uint32_t freq;      // Current frequency in Hz * 100
  uint32_t fBandLow;  // Lower band edge
  uint32_t fBandHigh; // Upper band edge
  char name[4];       // name of band, 3 characters + terminator.
  RadioMode mode;
  Sideband sideband;
  int FHiCut;
  int FLoCut;
  int FAMCut;       // Used for AM and SAM modes.
  float32_t RFgain; // This is not being used.  Greg KF5N February 14, 2024
  uint32_t band_type;
  float32_t gainCorrection; // This is probably not required if the gain across frequency is flat enough.
  int AGC_thresh;
};

enum BandEnum : size_t {
  BAND_80M = 0,
  BAND_40M = 1,
  BAND_20M = 2,
  BAND_17M = 3,
  BAND_15M = 4,
  BAND_12M = 5,
  BAND_10M = 6,
  NUMBER_OF_BANDS = 7,
};

enum class ItuRegion {
  ITU_REGION_1,
  ITU_REGION_2,
  ITU_REGION_3,
};

// Because of the way the Arduino EEPROM write works, it was necessary to wrap the bands[] array in a struct.
struct Bands {
  band bands[BandEnum::NUMBER_OF_BANDS];
};

extern Bands bands;

void BandsAccordingToItuRegion(void);

bool FindBandForFrequency(uint32_t frequency, int &currentBand);

/*****
  Purpose: Set the current band relay ON or OFF.  Reduce relay cycling.  Greg
KF5N March 24, 2025

Parameter list:
 void

Return value;
 void
*****/
void SetBandRelay();
