#pragma once

#include "SDT.h"

#include "CWCalibrate.h"
#include "Process.h"
#include "SSBCalibrate.h"

extern Process process;            // Receiver DSP object.
extern CWCalibrate cwcalibrater;   // CW mode calibration object.
extern SSBCalibrate ssbcalibrater; // SSB mode calibration object.

extern Rotary *volumeEncoder;
extern Rotary *tuneEncoder;
extern Rotary *filterEncoder;
extern Rotary *fineTuneEncoder;

extern Si5351 si5351;

extern RA8875 tft;

extern RadioState radioState;
extern RadioState lastState;

extern arm_fir_instance_f32 FIR_CW_DecodeL; // AFP 10-25-22
extern arm_fir_instance_f32 FIR_CW_DecodeR; // AFP 10-25-22

extern arm_fir_interpolate_instance_f32 FIR_int1_EX_I;
extern arm_fir_interpolate_instance_f32 FIR_int1_EX_Q;
extern arm_fir_interpolate_instance_f32 FIR_int2_EX_I;
extern arm_fir_interpolate_instance_f32 FIR_int2_EX_Q;

extern float32_t float_buffer_L_EX[];
extern float32_t float_buffer_R_EX[];
extern float32_t float_buffer_LTemp[];
extern float32_t float_buffer_RTemp[];

extern int32_t NCOFreq;

extern float32_t *cosBuffer; // This is a pointer to an array. Greg KF5N February 7, 2024
extern float32_t *sinBuffer; // Buffers commonized.  Greg KF5N, February 7, 2024.

// Global CW Correlation
extern float32_t cwRiseBuffer[];
extern float32_t cwFallBuffer[];

extern SampleRateEnum SampleRate;

// decimation factor
constexpr float32_t DF1 = 4.0;
// decimation factor
constexpr float32_t DF2 = 2.0;
// decimation factor
constexpr float32_t DF = DF1 * DF2;
// sample rate before decimation
constexpr float32_t n_samplerate = 176.0;

// 512/2/128 * 8 = 16
constexpr uint32_t N_B = FFT_length / 2 / BUFFER_SIZE * (uint32_t)DF;
constexpr uint32_t N_DEC_B = N_B / (uint32_t)DF;

// desired max BW of the filters
constexpr float32_t n_desired_BW = 9.0;
constexpr float32_t n_fpass1 = n_desired_BW / n_samplerate;
constexpr float32_t n_fpass2 = n_desired_BW / (n_samplerate / DF1);
constexpr float32_t n_fstop1 = ((n_samplerate / DF1) - n_desired_BW) / n_samplerate;
constexpr float32_t n_fstop2 = ((n_samplerate / (DF1 * DF2)) - n_desired_BW) / (n_samplerate / DF1);

// desired stopband attenuation
constexpr float32_t n_att = 90.0;

constexpr uint16_t n_dec1_taps = (1 + (uint16_t)(n_att / (22.0 * (n_fstop1 - n_fpass1))));
constexpr uint16_t n_dec2_taps = (1 + (uint16_t)(n_att / (22.0 * (n_fstop2 - n_fpass2))));

extern int attenuator;
extern bool calOnFlag;
constexpr uint8_t chipSelect = BUILTIN_SDCARD;

constexpr int DEC2STATESIZE = n_dec2_taps + (BUFFER_SIZE * N_B / (uint32_t)DF1) - 1;
constexpr int INT1_STATE_SIZE = 24 + BUFFER_SIZE * N_B / (uint32_t)DF - 1;
constexpr int INT2_STATE_SIZE = 8 + BUFFER_SIZE * N_B / (uint32_t)DF1 - 1;

extern uint32_t currentFreq;
extern uint32_t TxRxFreq; // = centerFreq+NCOFreq  NCOFreq from FreqShift2()

extern float32_t float_buffer_L[];
extern float32_t float_buffer_R[];
extern float32_t iFFT_buffer[];

extern bool volumeChangeFlag;
extern float32_t coefficient_set[];
extern float32_t dbm;
extern const float32_t volumeLog[];
extern uint32_t elapsed_micros_idx_t;
extern uint32_t elapsed_micros_sum;
extern bool powerUp;

void InitializeDataArrays();
int SetI2SFreq(int freq);
float TGetTemp();
