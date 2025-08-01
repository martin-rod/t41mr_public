#pragma once

#include "SDT.h"

extern float32_t bin_BW;

extern float32_t FIR_filter_mask[];

extern float32_t FIR_dec1_I_state[];
extern float32_t FIR_dec1_Q_state[];
extern float32_t FIR_dec1_coeffs[];

extern float32_t FIR_dec2_I_state[];
extern float32_t FIR_dec2_Q_state[];
extern float32_t FIR_dec2_coeffs[];

extern float32_t FIR_int1_I_state[];
extern float32_t FIR_int1_Q_state[];
extern float32_t FIR_int1_coeffs[];

extern float32_t FIR_int2_I_state[];
extern float32_t FIR_int2_Q_state[];
extern float32_t FIR_int2_coeffs[];

void DoReceiveEQ();

void DoExciterEQ();

/*****
  Purpose: Adjust the audio filter band limits based on user input from encoder.
    void
  Return value;
    void
*****/
void FilterBandwidth();

void SetDecIntFilters();
