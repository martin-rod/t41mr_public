#pragma once

#include "SDT.h"

extern bool calibrateFlag;
extern bool morseDecodeAdjustFlag;

constexpr int PRIMARY_MENU_X = 0;
constexpr int SECONDARY_MENU_X = 250;
constexpr int MENUS_Y = 0;
constexpr int EACH_MENU_WIDTH = 260;

constexpr int VFOB_PIXEL_LENGTH = 280;
constexpr int FREQUENCY_PIXEL_HI = 45;

/*****
  Purpose: Select AGC option.

Parameter list:
 void

Return value
 void
*****/
void AGCOptions();

/*****
  Purpose: Allow user to set current user configuration values or restore
default settings.

Parameter list:
 void

Return value
 void
*****/
void ConfigDataOptions();
/*****
  Purpose: Allow user to set restore calibration values or restore default
settings.

Parameter list:
 void

Return value
 void
*****/
void CalDataOptions();

// Updates by KF5N to CalibrateOptions() function.  Added SSB Carrier and SSB
// Transmit cal.  Greg KF5N July 10, 2024 Updated receive calibration code to
// clean up graphics.  KF5N August 3, 2023
/*****
  Purpose: Present the Calibrate options available and return the selection.
  This function is embedded in the mail receiver loop.  It gets called
repeatedly during calibration.

Parameter list:
void

Return value
void
*****/
#ifdef QSE2
void CalibrateOptions();
#else // Not using QSE2 (No carrier calibration)
void CalibrateOptions();
#endif

/*****
  Purpose: Present the CW options available to the user.  Change and store to
ConfigData.

Parameter list:
void

Return value
void
*****/
void CWOptions();

/*****
  Purpose: Receive EQ set

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void EqualizerRecOptions();

/*****
  Purpose: This option reverses the dit and dah paddles on the keyer

Parameter list:
void

Return value
void
*****/
void DoPaddleFlip();

/*****
  Purpose: Xmit EQ options

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void EqualizerXmtOptions();

/*****
  Purpose: Set options for the SSB exciter.

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void SSBOptions();

/*****
  Purpose: Show the list of scales for the spectrum divisions

Parameter list:
 void

Return value
 int           an index into displayScale[] array, or -1 on cancel
*****/
void SpectrumOptions();

/*****
  Purpose: Present the bands available and return the selection

Parameter list:
void

Return value12
int           an index into the band array
*****/
void RFOptions();

/*****
  Purpose: To select an option from a submenu

  Parameter list:
    char *options[]           submenus
    int numberOfChoices       choices available
    int defaultState          the starting option

  Return value
    int           an index into the band array
*****/
int SubmenuSelect(const std::string options[], int numberOfChoices, int defaultStart);

void ShowMenu(const char *menu[]);

/*****
  Purpose: Used to change the currently active VFO

Parameter list:
 void

Return value
 int             the currently active VFO, A = 1, B = 0
*****/
void VFOSelect();
