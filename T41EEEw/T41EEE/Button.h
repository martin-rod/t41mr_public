#pragma once

#include "SDT.h"

//  Primary menu selections from the switch matrix.  This is for a 6x3 matrix for a total of 18 buttons.
enum class MenuSelect {
  MENU_OPTION_SELECT,
  MAIN_MENU_UP,
  BAND_UP,
  ZOOM,
  MAIN_MENU_DN,
  BAND_DN,
  FILTER,
  DEMODULATION,
  SET_MODE,
  NOISE_REDUCTION,
  NOTCH_FILTER,
  MUTE_AUDIO,
  FINE_TUNE_INCREMENT,
  DECODER_TOGGLE,
  MAIN_TUNE_INCREMENT,
  RESET_TUNING,
  UNUSED_1,
  BEARING,
  BOGUS_PIN_READ,
  DEFAULT
};

constexpr size_t TOP_MENU_COUNT = 12;

MenuSelect readButton(MenuSelect lastUsedTask);
MenuSelect readButton();

class Button {
public:
  IntervalTimer buttonInterrupts;
  bool buttonInterruptsEnabled = false;
  int buttonRead = 0;
  int minPinRead = 1024;
  int secondaryMenuChoiceMade;

  bool save_last_frequency = false; // Make this the default behavior.  Greg KF5N October 16, 2024.
  int directFreqFlag = 0;
  int32_t subMenuMaxOptions = 0; // Holds the number of submenu options.
  uint32_t TxRxFreqOld = 0;

  void EnableButtonInterrupts();
  MenuSelect ProcessButtonPress(int valPin);
  int ReadSelectedPushButton();
  void ExecuteButtonPress(MenuSelect val);
  void ButtonCenterFreqIncrement();
  void ButtonFineFreqIncrement();
  void ButtonMenuIncrease();
  void ButtonMenuDecrease();
  void ButtonBandIncrease();
  void ButtonBandDecrease();
  void BandSet(int band);
  void ButtonZoom();
  void ButtonFilter();
  void ButtonSelectSideband();
  void ButtonMode();
  void ButtonNR();
  void ButtonNotchFilter();
  void ButtonMuteAudio();
  int DrawNewFloor(int floor);
  void ExecuteModeChange();
  void ResetZoom(int zoomIndex1);
  void ButtonFrequencyEntry();
};

// These constants are used by the voltage divider network so only 1 analog pin
// is used for the 16 option switches. These may need to be changed for the
// exact value for your system. They are initialized in the INO file.

// This is the analog pin that controls the 18 switches
constexpr int BUSY_ANALOG_PIN = 39;

// If the analog pin is greater than this value, nothing's going on
constexpr int NOTHING_TO_SEE_HERE = 950;

// If no push button read. GET RID OF THIS!!!
constexpr int BOGUS_PIN_READ = -1;

// This is the maximum value that can added to a BUSY_ANALOG_PIN pin read value of a push button and still have the switch value be
// associated with the correct push button.  This is the maximum value that can added to a BUSY_ANALOG_PIN pin read value of a push
// button and still have the switch value be associated with the correct push button.
constexpr int WIGGLE_ROOM = 20;

// Number of push button switches.
constexpr size_t NUMBER_OF_SWITCHES = 18;

extern bool switchFilterSideband;
extern int zoomIndex;

extern Button button;

extern int32_t mainMenuIndex;
