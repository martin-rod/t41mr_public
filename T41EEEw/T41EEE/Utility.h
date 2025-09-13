#pragma once

#include "SDT.h"

/*****
  Purpose: Generate Array with variable sinewave frequency tone AFP 05-17-22
  Parameter list:
    int numCycles  (This must be an integer value)
  Return value;
    void
*****/
FLASHMEM void sineTone(int numCycles);

/*****
  Purpose: Correct Phase angle between I and Q channels.  Not used with SSB.
  Parameter list:
    float32_t *I_buffer, float32_t *Q_buffer, float32_t factor, uint32_t
blocksize Return value; void
*****/
void IQPhaseCorrection(float32_t *I_buffer, float32_t *Q_buffer, float32_t factor, uint32_t blocksize);

/*****
  Purpose: Calculate sinc function

  Parameter list:
    void
  Return value;
    void
*****/
float MSinc(int m, float fc);

/*****
  Purpose: Izero

  Parameter list:
    void
  Return value;
    void
*****/
float32_t Izero(float32_t x);

/*****
  Purpose:    Fast algorithm for log10
              This is a fast approximation to log2()
              Y = C[0]*F*F*F + C[1]*F*F + C[2]*F + C[3] + E;
              log10f is exactly log2(x)/log2(10.0f)
              Math_log10f_fast(x) =(log2f_approx(x)*0.3010299956639812f)

  Parameter list:
    float32_t X       number for conversion

  Return value;
    void
*****/
float32_t log10f_fast(float32_t X);

/*****
  Purpose:
  Parameter list:
    float32_t inphase
    float32_t quadrature

  Return value;
    float32_t
*****/
// (c) András Retzler taken from libcsdr: https://github.com/simonyiszk/csdr
float32_t AlphaBetaMag(float32_t inphase, float32_t quadrature);

/*****
  Purpose: copied from https://www.dsprelated.com/showarticle/1052.php
           Polynomial approximating arctangenet on the range -1,1.
           Max error < 0.005 (or 0.29 degrees)

  Parameter list:
    float z         value to approximate

  Return value;
    float           atan vakye
*****/
float ApproxAtan(float z);

/*****
  Purpose: function reads the analog value for each matrix switch and stores
that value in EEPROM. Only called if STORE_SWITCH_VALUES is uncommented.

  Parameter list:
    void

  Return value;
    void
*****/
void SaveAnalogSwitchValues();

/*****
  Purpose: Tries to open the EEPROM SD file to see if an SD card is present in
the system

  Parameter list:
    void

  Return value;
    int               0 = SD not initialized, 1 = has data
*****/
int SDPresentCheck();

/*****
  Purpose: Initialize power coefficients based on transmit power level and
calibration factor.

  Parameter list:
    void

  Return value;
    void
*****/
FLASHMEM void initPowerCoefficients();

FLASHMEM void initUserDefinedStuff();

/*****
  Purpose: Arm function which is not included in the older library included with
TeensyDuino.

  https://www.keil.com/pack/doc/cmsis/dsp/html/arm__clip__f32_8c.html

*****/
void arm_clip_f32(const float32_t *pSrc, float32_t *pDst, float32_t low, float32_t high, uint32_t numSamples);

void printFile(const char *filename);
