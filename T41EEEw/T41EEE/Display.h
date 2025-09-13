#pragma once

#include "SDT.h"

constexpr int XPIXELS = 800; // This is for the 5.0" display
constexpr int YPIXELS = 480;
constexpr int CHAR_HEIGHT = 32;

constexpr int SPECTRUM_HEIGHT = 150; // This is the pixel height of spectrum plot area without disturbing the axes
constexpr int SPECTRUM_TOP_Y = 100;  // Start of spectrum plot space
constexpr int SPECTRUM_BOTTOM = (SPECTRUM_TOP_Y + SPECTRUM_HEIGHT - 3);

constexpr int MAX_WATERFALL_WIDTH = 512;
constexpr int MAX_WATERFALL_ROWS = 170;
constexpr int SPECTRUM_LEFT_X = 3;
constexpr int WATERFALL_LEFT_X = SPECTRUM_LEFT_X;
constexpr int WATERFALL_RIGHT_X = (WATERFALL_LEFT_X + MAX_WATERFALL_WIDTH); // 3 + 512
constexpr int WATERFALL_TOP_Y = (SPECTRUM_TOP_Y + SPECTRUM_HEIGHT + 5);
constexpr int FIRST_WATERFALL_LINE = (WATERFALL_TOP_Y + 20);                  // 255 + 35 = 290
constexpr int WATERFALL_BOTTOM = (FIRST_WATERFALL_LINE + MAX_WATERFALL_ROWS); // 290 + 170 = 460
constexpr int DECODER_X = WATERFALL_RIGHT_X + 43;                             // 512 +  43 = 555
constexpr int DECODER_Y = WATERFALL_TOP_Y + 190;                              // 255 + 190 = 345

constexpr int INFORMATION_WINDOW_X = WATERFALL_RIGHT_X + 25; // 512 + 25 = 537
constexpr int INFORMATION_WINDOW_Y = WATERFALL_TOP_Y + 37;   // 255 + 37 = 292

constexpr int OPERATION_STATS_X = 130;

constexpr int FILTER_PARAMETERS_X = (XPIXELS * 0.22);
constexpr int FILTER_PARAMETERS_Y = (YPIXELS * 0.213);

constexpr int centerLine = (MAX_WATERFALL_WIDTH + SPECTRUM_LEFT_X) / 2;
// SPECTRUM_HEIGHT + 3;
constexpr int h = 135;
constexpr int SPECTRUM_RES = 512;

constexpr int FREQUENCY_X = 5;
constexpr int FREQUENCY_Y = 45;

constexpr int FREQUENCY_X_SPLIT = 280;

extern int16_t pixelnew[];
extern int16_t pixelold[];
extern int16_t pixelCurrent[];

extern int filterWidth;
extern int16_t fftOffset;
extern int16_t audioFFToffset;

extern int16_t y_old;
extern int16_t y_new;
extern int16_t y1_new;
extern int16_t y1_old;
extern int16_t y_old2;

extern int newCursorPosition;
extern int oldCursorPosition;

extern bool updateDisplayFlag;
extern int updateDisplayCounter;

FLASHMEM void Splash();

/*****
  Purpose: Draw audio spectrum box.  AFP added 3-14-21

  Parameter list:

  Return value;
    void
*****/
void DrawAudioSpectContainer();

/*****
  Purpose: Show the program name and version number.

  Parameter list:
    void

  Return value;
    void
*****/
void ShowName();

/*****
  Purpose: Show Spectrum display with auto RF gain for T41EEE.1.  Harry GM3RVL,
January 16, 2024 Note that this routine calls the Audio process Function during
each display cycle, for each of the 512 display frequency bins.  This means that
the audio is refreshed at the maximum rate and does not have to wait for the
display to complete drawing the full spectrum. However, the display data are
only updated ONCE during each full display cycle, ensuring consistent data for
the erase/draw cycle at each frequency point.

  Parameter list:
    void

  Return value;
    void
*****/
void ShowSpectrum();

/*****
  Purpose: Show filter bandwidth near center of spectrum.

  Parameter list:
    void

  Return value;
    void
        // AudioNoInterrupts();
        // M = demod_mode, FU & FL upper & lower frequency
        // this routine prints the frequency bars under the spectrum display
*****/
void ShowBandwidth();

/*****
  Purpose: DrawSMeterContainer()
  Parameter list:
    void
  Return value;
    void
*****/
void DrawSMeterContainer();

/*****
  Purpose: Print the vertical dB setting to the spectrum display.
  Parameter list:
    void
  Return value;
    void
*****/
void ShowSpectrumdBScale();

/*****
  Purpose: This function draws spectrum display container
  Parameter list:
    void
  Return value;
    void
*****/
void DrawSpectrumDisplayContainer();

/*****
  Purpose: This function draws the frequency bar at the bottom of the spectrum
scope, putting markers at every graticule and the full frequency

  Parameter list:
    void

  Return value;
    void
*****/
void DrawFrequencyBarValue();

/*****
  Purpose:  Indicate Auto-Gain or Auto-Spectrum is active.

  Parameter list:
    void

  Return value;
    void
*****/
void ShowAutoStatus();

void ShowVFOStatus();

/*****
  Purpose: To display the current transmission frequency, band, mode, and
sideband above the spectrum display

  Parameter list:
    void

  Return value;
    void

*****/
void BandInformation();

/*****
  Purpose: Display current power setting

  Parameter list:
    void

  Return value;
    void
*****/
void ShowCurrentPowerSetting();

/*****
  Purpose: Format frequency for printing
  Parameter list:
    void
  Return value;
    void
    // show frequency
*****/
void FormatFrequency(uint32_t freq, char *freqBuffer, size_t freqBufferSize);

/*****
  Purpose: Show Main frequency display at top.  This shows currentFreqA and
currentFreqB.

  Parameter list:
    void

  Return value;
    void
    // show frequency
*****/
void ShowFrequency();

/*****
  Purpose: Display dBm
  Parameter list:
    void
  Return value;
    void
*****/
void DisplaydbM();

/*****
  Purpose: Display the current temperature and load figures for T4.1

  Parameter list:
    int notchF        the notch to use
    int MODE          the current MODE

  Return value;
    void
*****/
void ShowTempAndLoad();

/*****
  Purpose: format a floating point number

  Parameter list:
    float val         the value to format
    int decimals      the number of decimal places
    int x             the x coordinate for display
    int y                 y          "

  Return value;
    void
*****/
void MyDrawFloat(float val, int width, int precision, int x, int y, char *buff);

/*****
  Purpose: Shows the startup settings for the information displayed int the
lower-right box.

  Parameter list:
    void

  Return value;
    void
*****/
FLASHMEM void UpdateInfoWindow();

/*****
  Purpose: Updates the states of the speaker and headphone.

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateAudioField();

/*****
  Purpose: Updates the Volume setting on the display

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateVolumeField();

void UpdateAGCField();

/*****
  Purpose: Updates the increment setting on the display

  Parameter list:
    void

  Return value;
    void
*****/
void DisplayIncrementField();

/*****
  Purpose: Updates the notch value on the display

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateNotchField();

/*****
  Purpose: Updates the zoom setting on the display

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateZoomField();

/*****
  Purpose: Updates the compression setting in the info window.

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateCompressionField();

/*****
  Purpose: Updates whether the decoder is on or off and decoder related graphics
including CW, highpass, and lowpass filter bandwidths.

  Parameter list:
    void

  Return value;
    void
*****/
FLASHMEM void UpdateAudioGraphics();

/*****
  Purpose: Updates the Rx and Tx equalizer states
           shown on the display.

  Parameter list:
    bool rxEqState, txEqState

  Return value;
    void
*****/
FLASHMEM void UpdateEqualizerField(bool rxEqState, bool txEqState);

/*****
  Purpose: Updates the Keyer and WPM setting on the display

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateWPMField();

/*****
  Purpose: Updates the noise field on the display

  Parameter list:
    void

  Return value;
    void
*****/
void UpdateNoiseField();

/*****
  Purpose: This function draws the Info Window frame

  Parameter list:
    void

  Return value;
    void
*****/
void DrawInfoWindowFrame();

/*****
  Purpose: This function redraws the entire display screen.

  Parameter list:
    void

  Return value;
    void
*****/
void RedrawDisplayScreen();

/*****
  Purpose: Draw Tuned Bandwidth on Spectrum Plot. // Calculations simplified
KF5N April 22, 2024

  Parameter list:

  Return value;
    void
*****/
void DrawBandWidthIndicatorBar();

/*****
  Purpose: This function removes the spectrum display container

  Parameter list:
    void

  Return value;
    void
*****/
void EraseSpectrumDisplayContainer();

/*****
  Purpose: This function erases the contents of the spectrum display

  Parameter list:
    void

  Return value;
    void
*****/
void EraseSpectrumWindow();

/*****
  Purpose: To erase both primary and secondary menus from display

  Parameter list:

  Return value;
    void
*****/
void EraseMenus();

/*****
  Purpose: To erase primary menu from display

  Parameter list:

  Return value;
    void
*****/
void ErasePrimaryMenu();

/*****
  Purpose: To erase secondary menu from display

  Parameter list:

  Return value;
    void
*****/
void EraseSecondaryMenu();

/*****
  Purpose: Shows transmit (red) and receive (green) mode

  Parameter list:

  Return value;
    void
*****/
void ShowTransmitReceiveStatus();

void DisplayClock();
