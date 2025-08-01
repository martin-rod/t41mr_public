#pragma once

#include "SDT.h"

extern float32_t FFT_spec_old[];
extern const size_t FFT_spec_old_size;

extern float32_t buffer_spec_FFT[];
extern const size_t buffer_spec_FFT_size;

void InitilizeFFT();

void ZoomFFTPrep();

void ZoomFFTExe(uint32_t blockSize);

/*****
  Purpose: CalcZoom1Magn()
  Parameter list:
    void
  Return value;
    void
    Used when Spectrum Zoom =1
*****/
void CalcZoom1Magn();
