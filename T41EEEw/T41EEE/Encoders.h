#pragma once

#include "SDT.h"

extern volatile int filterEncoderMove;
extern volatile long fineTuneEncoderMove;

extern int resetTuningFlag; // Experimental flag for ResetTuning() due to possible timing issues.  KF5N July 31, 2023
extern int centerTuneFlag;

extern int last_filter_pos;
extern int filter_pos;

/*****
  Purpose: Audio filter adjust with encoder.
           This function runs only if the encoder has been rotated.
  Parameter list:
    void
  Return value;
    void
*****/
// A major change to this function.  It only sets FHiCut and FLoCut variables in
// the bands2 struct/array. No other functions are performed.  Filter settings,
// demodulation, and graphics changes are delegated to other functions. This
// version limits the result to an fhigh and an flow number.  Note that this
// function works in concert with EncoderFilter() which is attached to an
// interrupt.  Graphics changes are handled by UpdateAudioGraphics() in
// Display.cpp.
void FilterSetSSB();

/*****
  Purpose: EncoderCenterTune.  This is "coarse" tuning.
  Parameter list:
    void
  Return value;
    void
*****/
void EncoderCenterTune();

/*****
  Purpose: Encoder volume control.  Sets ConfigData.audioVolume between 0 and
100.

  Parameter list:
    void

  Return value;
    void
*****/
void EncoderVolume();

/*****
  Purpose: Use the encoder to change the value of a number in some other
function This function does not have a while loop.  Thus it must be used inside
           some other loop.

  Parameter list:
    int minValue                the lowest value allowed
    int maxValue                the largest value allowed
    int startValue              the numeric value to begin the count
    int increment               the amount by which each increment changes the
value char prompt[]               the input prompt Return value; int the new
value
*****/
float GetEncoderValueLive(float minValue, float maxValue, float startValue, float increment, std::string prompt, bool left);

/*****
  Purpose: Use the encoder to change the value of a number in some other
function. This function does not have a while loop.  Thus it must be used inside some other loop.

  Parameter list:
    int minValue                the lowest value allowed
    int maxValue                the largest value allowed
    int startValue              the numeric value to begin the count
    int increment               the amount by which each increment changes the
value char prompt[]             the input prompt Return value; int the new value
*****/
float GetEncoderValueLiveString(float minValue, float maxValue, float startValue, float increment, std::string prompt, bool left);

/*****
  Purpose: Use the encoder to change the value of a number in some other
function This function does not have a while loop.  Thus it must be used inside some other loop.

  Parameter list:
    int minValue                the lowest value allowed
    int maxValue                the largest value allowed
    int startValue              the numeric value to begin the count
    int increment               the amount by which each increment changes the
value char prompt[]               the input prompt Return value; int the new value
*****/
q15_t GetEncoderValueLiveQ15t(int minValue, int maxValue, int startValue, int increment, char prompt[], bool left);

/*****
  Purpose: Use the encoder to change the value of a number in some other
function. This function has a while loop, and it can be used independently.

  Parameter list:
    int minValue                the lowest value allowed
    int maxValue                the largest value allowed
    int startValue              the numeric value to begin the count
    int increment               the amount by which each increment changes the
value char prompt[]               the input prompt Return value; int the new
value
*****/
int GetEncoderValue(int minValue, int maxValue, int startValue, int increment, std::string prompt);

/*****
  Purpose: Allows quick setting of WPM without going through a menu

  Parameter list:
    void

  Return value;
    int           the current WPM
*****/
int SetWPM();

/*****
  Purpose: Determines how long the transmit relay remains on after last CW atom
is sent.

  Parameter list:
    void

  Return value;
    long            the delay length in milliseconds
*****/
uint32_t SetTransmitDelay();

/*****
  Purpose: Fine frequency tune control.

  Parameter list:
    void

  Return value;
    void
*****/
void EncoderFineTune();

// This function is attached to interrupts (in the .ino file).
void EncoderFilter();
