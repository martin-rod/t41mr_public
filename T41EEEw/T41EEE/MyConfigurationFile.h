#pragma once

#include "CalibrationData.h"
#include "ConfigurationData.h"

// SD library uses 8.3 filenames
constexpr const char *myConfigFilename = "/my_conf.txt";
constexpr const char *myCalFilename = "/my_cal.txt";

void loadMyConfigutation(const char *filename, config_t &data);
void loadMyCalibration(const char *filename, calibration_t &data);

void copyMyConfigutation(config_t &data);
void copyMyCalibration(calibration_t &data);
