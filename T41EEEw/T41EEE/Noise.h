#pragma once

#include "SDT.h"

extern bool ANR_notch; // KF5N March 2, 2024
extern uint8_t NR_first_time;

void InitilizeNR();

/*****
  Purpose: Kim1_NR()
  Parameter list:
    void
  Return value;
    void
*****/
void Kim1_NR();

/*****
  This function appears to have both notch and noise reduction capability.
  Purpose:   void xanr
  Parameter list:
    void
  Return value;
    void
*****/
// variable leak LMS algorithm for automatic notch or noise
// reduction
// (c) Warren Pratt wdsp library 2016
void Xanr();

/*****
  Purpose: spectral_noise_reduction
  Parameter list:
    void
  Return value;
    void
*****/
/************************************************************************************************************

   Noise reduction with spectral subtraction rule
   based on Romanin et al. 2009 & Schmitt et al. 2002
   and MATLAB voicebox
   and Gerkmann & Hendriks 2002
   and Yao et al. 2016

STAND: UHSDR github 14.1.2018
************************************************************************************************************/
void SpectralNoiseReduction();

/*****
  Purpose: void LMSNoiseReduction(

  Parameter list:
    void

  Return value;
    void
*****/
void LMSNoiseReduction(int16_t blockSize, float32_t *nrbuffer);

/*****
  Purpose: void InitLMSNoiseReduction()

  Parameter list:
    void

  Return value;
    void
*****/
void InitLMSNoiseReduction();

/*****
  Purpose:
  Parameter list:
    void
  Return value;
    void
*****/
FLASHMEM void SpectralNoiseReductionInit();
