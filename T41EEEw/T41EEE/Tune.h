#pragma once

#include "SDT.h"

#ifdef PLLMODULE
/*****
  Purpose: A special variant of SetFreq() used only for calibration.

  Parameter list:
    void

  Return value;
    void

  CAUTION: SI5351_FREQ_MULT is set in the si5253.h header file and is 100UL
*****/
void SetFreqCal(int calFreqShift);

/*****
  Purpose: SetFrequency

  Parameter list:
    void

  Return value;
    void

  CAUTION: SI5351_FREQ_MULT is set in the si5253.h header file and is 100UL
*****/
void SetFreq();
#else
/*****
  Purpose: A special variant of SetFreq() used only for calibration.

  Parameter list:
    void

  Return value;
    void

  CAUTION: SI5351_FREQ_MULT is set in the si5253.h header file and is 100UL
*****/
void SetFreqCal(int calFreqShift);

/*****
  Purpose: SetFrequency

  Parameter list:
    void

  Return value;
    void

  CAUTION: SI5351_FREQ_MULT is set in the si5253.h header file and is 100UL
*****/
void SetFreq();
#endif

/***** //AFP 10-11-22 all new
  Purpose:  Reset tuning to center

  Parameter list:
  void

  Return value;
  void
*****/
void ResetTuning();

/*****
  Purpose: Purpose is to sety VFOA to receive frequency and VFOB to the transmit
frequency

  Parameter list:
    void

  Return value;
    int           the offset as an int

  CAUTION: SI5351_FREQ_MULT is set in the si5253.h header file and is 100UL
*****/
int DoSplitVFO();

/*****
  Purpose: Reset the flip-flops in the QSD2/QSE2 quadrature generators.

  Parameter list:
    void

  Return value;
    void
*****/
void ResetFlipFlops();
