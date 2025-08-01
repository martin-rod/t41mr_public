#pragma once

#include "SDT.h"

extern float32_t CW_Filter_Coeffs2[];

/*****
  Purpose: void calc_FIR_coeffs
    // pointer to coefficients variable, no. of coefficients to calculate,
frequency where it happens, stopband attenuation in dB,
    // filter type, half-filter bandwidth (only for bandpass and notch)

  Parameter list:
    float * coeffs_I
    int numCoeffs
    float32_t fc
    float32_t Astop
    int type
    float dfc
    float Fsamprate

  Return value;
    void
*****/
void CalcFIRCoeffs(float *coeffs_I, int numCoeffs, float32_t fc, float32_t Astop, int type, float dfc, float Fsamprate);

//////////////////////////////////////////////////////////////////////
//  Call to setup filter parameters
// SampleRate in Hz
// FLowcut is low cutoff frequency of filter in Hz
// FHicut is high cutoff frequency of filter in Hz
// Offset is the CW tone offset frequency
// cutoff frequencies range from -SampleRate/2 to +SampleRate/2
//  HiCut must be greater than LowCut
//    example to make 2700Hz USB filter:
//  SetupParameters( 100, 2800, 0, 48000);
//////////////////////////////////////////////////////////////////////

/*****
  Purpose: void calc_cplx_FIR_coeffs

  Parameter list:
    float *coeffs_I
    float *coeffs_Q
    int numCoeffs
    float32_t FLoCut
    float32_t FHiCut
    float SampleRate

  Return value;
    void
*****/
void CalcCplxFIRCoeffs(float *coeffs_I, float *coeffs_Q, int numCoeffs, float32_t FLoCut, float32_t FHiCut, float SampleRate);

/*****
  Purpose: void set_IIR_coeffs

  Parameter list:
    void
  Return value;
    void
*****/
void SetIIRCoeffs(float32_t f0, float32_t Q, float32_t sample_rate, uint8_t filter_type);
