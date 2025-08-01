#include "Band.h"
#include "ConfigurationData.h"

// Using ARRL table: https://www.arrl.org/frequency-bands

Bands bands = {
    // bands[NUMBER_OF_BANDS]
    {
        // initilaze later according to ITU_REGION
        {
            .freq = 0,
            .fBandLow = 0,
            .fBandHigh = 0,
            .name = "80M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 0,
            .FLoCut = 0,
            .FAMCut = 0,
            .RFgain = 0.0,
            .band_type = HAM_BAND,
            .gainCorrection = 0.0,
            .AGC_thresh = 0,
        },
        // initilaze later according to ITU_REGION
        {
            .freq = 0,
            .fBandLow = 0,
            .fBandHigh = 0,
            .name = "40M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 0,
            .FLoCut = 0,
            .FAMCut = 0,
            .RFgain = 0.0,
            .band_type = HAM_BAND,
            .gainCorrection = 0.0,
            .AGC_thresh = 0,
        },
        {
            .freq = 14200000,
            .fBandLow = 14000000,
            .fBandHigh = 14350000,
            .name = "20M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 3000,
            .FLoCut = 200,
            .FAMCut = 5000,
            .RFgain = 15.0,
            .band_type = HAM_BAND,
            .gainCorrection = 1.0,
            .AGC_thresh = 20,
        },
        {
            .freq = 18100000,
            .fBandLow = 18068000,
            .fBandHigh = 18168000,
            .name = "17M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 3000,
            .FLoCut = 200,
            .FAMCut = 5000,
            .RFgain = 15.0,
            .band_type = HAM_BAND,
            .gainCorrection = 1.0,
            .AGC_thresh = 20,
        },
        {
            .freq = 21200000,
            .fBandLow = 21000000,
            .fBandHigh = 21450000,
            .name = "15M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 3000,
            .FLoCut = 200,
            .FAMCut = 5000,
            .RFgain = 15.0,
            .band_type = HAM_BAND,
            .gainCorrection = 1.0,
            .AGC_thresh = 20,
        },
        {
            .freq = 24920000,
            .fBandLow = 24890000,
            .fBandHigh = 24990000,
            .name = "12M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 3000,
            .FLoCut = 200,
            .FAMCut = 5000,
            .RFgain = 15.0,
            .band_type = HAM_BAND,
            .gainCorrection = 1.0,
            .AGC_thresh = 20,
        },
        {
            .freq = 28350000,
            .fBandLow = 28000000,
            .fBandHigh = 29700000,
            .name = "10M",
            .mode = RadioMode::SSB_MODE,
            .sideband = Sideband::UPPER,
            .FHiCut = 3000,
            .FLoCut = 200,
            .FAMCut = 5000,
            .RFgain = 15.0,
            .band_type = HAM_BAND,
            .gainCorrection = 1.0,
            .AGC_thresh = 20,
        },
    },
};

const band band80mItuRegion1 = {
    .freq = 3700000UL,
    .fBandLow = 3500000,
    .fBandHigh = 3800000,
    .name = "80M",
    .mode = RadioMode::SSB_MODE,
    .sideband = Sideband::LOWER,
    .FHiCut = 3000,
    .FLoCut = 200,
    .FAMCut = 5000,
    .RFgain = 15.0,
    .band_type = HAM_BAND,
    .gainCorrection = 1.0,
    .AGC_thresh = 20,
};
const band band40mItuRegion1 = {
    .freq = 7150000,
    .fBandLow = 7000000,
    .fBandHigh = 7200000,
    .name = "40M",
    .mode = RadioMode::SSB_MODE,
    .sideband = Sideband::LOWER,
    .FHiCut = 3000,
    .FLoCut = 200,
    .FAMCut = 5000,
    .RFgain = 15.0,
    .band_type = HAM_BAND,
    .gainCorrection = 1.0,
    .AGC_thresh = 20,
};

const band band80mItuRegion2 = {
    .freq = 3700000UL,
    .fBandLow = 3500000,
    .fBandHigh = 4000000,
    .name = "80M",
    .mode = RadioMode::SSB_MODE,
    .sideband = Sideband::LOWER,
    .FHiCut = 3000,
    .FLoCut = 200,
    .FAMCut = 5000,
    .RFgain = 15.0,
    .band_type = HAM_BAND,
    .gainCorrection = 1.0,
    .AGC_thresh = 20,
};
const band band40mItuRegion2 = {
    .freq = 7150000,
    .fBandLow = 7000000,
    .fBandHigh = 7300000,
    .name = "40M",
    .mode = RadioMode::SSB_MODE,
    .sideband = Sideband::LOWER,
    .FHiCut = 3000,
    .FLoCut = 200,
    .FAMCut = 5000,
    .RFgain = 15.0,
    .band_type = HAM_BAND,
    .gainCorrection = 1.0,
    .AGC_thresh = 20,
};

const band band80mItuRegion3 = {
    .freq = 3700000UL,
    .fBandLow = 3500000,
    .fBandHigh = 3900000,
    .name = "80M",
    .mode = RadioMode::SSB_MODE,
    .sideband = Sideband::LOWER,
    .FHiCut = 3000,
    .FLoCut = 200,
    .FAMCut = 5000,
    .RFgain = 15.0,
    .band_type = HAM_BAND,
    .gainCorrection = 1.0,
    .AGC_thresh = 20,
};
const band band40mItuRegion3 = {
    .freq = 7150000,
    .fBandLow = 7000000,
    .fBandHigh = 7200000,
    .name = "40M",
    .mode = RadioMode::SSB_MODE,
    .sideband = Sideband::LOWER,
    .FHiCut = 3000,
    .FLoCut = 200,
    .FAMCut = 5000,
    .RFgain = 15.0,
    .band_type = HAM_BAND,
    .gainCorrection = 1.0,
    .AGC_thresh = 20,
};

void BandsAccordingToItuRegion(void) {
  switch (ConfigData.ituRegion) {
  case ItuRegionEnum::ITU_REGION_1:
    bands.bands[BandEnum::BAND_80M] = band80mItuRegion1;
    bands.bands[BandEnum::BAND_40M] = band40mItuRegion1;
    break;
  case ItuRegionEnum::ITU_REGION_2:
    bands.bands[BandEnum::BAND_80M] = band80mItuRegion2;
    bands.bands[BandEnum::BAND_40M] = band40mItuRegion2;
    break;
  case ItuRegionEnum::ITU_REGION_3:
    bands.bands[BandEnum::BAND_80M] = band80mItuRegion3;
    bands.bands[BandEnum::BAND_40M] = band40mItuRegion3;
    break;
  }
}
