#include "MenuProc.h"

#include "AudioSignal.h"
#include "Button.h"
#include "CWProcessing.h"
#include "CalibrationData.h"
#include "ConfigurationData.h"
#include "Display.h"
#include "Eeprom.h"
#include "Encoders.h"
#include "Filter.h"
#include "MyConfigurationFile.h"
#include "SSB_Exciter.h"
#include "T41EEE.h"
#include "Tune.h"
#include "Utility.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME MennProc

#include "trace.h"

constexpr int PIXELS_PER_EQUALIZER_DELTA = 10; // Number of pixels per detent of encoder for equalizer changes

constexpr int DEFAULT_EQUALIZER_BAR = 100; // Default equalizer bar height
constexpr int VFOA_PIXEL_LENGTH = 275;

int micChoice = 0;
int splitOn = 0;
int IQChoice = 0;

bool calibrateFlag = false;
bool morseDecodeAdjustFlag = false;

int SubmenuSelectString(std::string options[], int numberOfChoices, int defaultStart);

void ShowMenu(const char *menu[]) {
  tft.setFontScale((enum RA8875tsize)1);

  tft.fillRect(PRIMARY_MENU_X, MENUS_Y, 300, CHAR_HEIGHT, RA8875_BLUE); // Top-left of display
  tft.setCursor(5, 0);
  tft.setTextColor(RA8875_WHITE);
  tft.print(*menu); // Primary Menu
}

// Updates by KF5N to CalibrateOptions() function.  Added SSB Carrier and SSB
// Transmit cal.  Greg KF5N July 10, 2024 Updated receive calibration code to
// clean up graphics.  KF5N August 3, 2023
// ==============  AFP 10-22-22 ==================
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
void CalibrateOptions() {
  int freqCorrectionFactorOld = 0;
  int32_t increment = 100L;
  MenuSelect menu;
  char freqCal[] = "Freq Cal: ";
  tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 30, CHAR_HEIGHT, RA8875_BLACK);

  // Select the type of calibration, and then skip this during the loop()
  // function. Note that some calibrate options run inside the loop() function!
  if (calibrateFlag == false) { //    0             1           2               3 4
                                //    5             6              7 8 9 10 11 12 13
                                //    14               15              16 17 18
    const std::string IQOptions[]{
        "Freq Cal",        "CW PA Cal",        "CW Rec Cal",   "CW Carrier Cal", "CW Xmit Cal",   "SSB PA Cal",     "SSB Rec Cal",
        "SSB Carrier Cal", "SSB Transmit Cal", "CW Radio Cal", "CW Refine Cal",  "SSB Radio Cal", "SSB Refine Cal", "dBm Level Cal",
        "DAC Offset CW",   "DAC Offset SSB",   "Btn Cal",      "Btn Repeat",     "Cancel"}; // AFP 10-21-22
    IQChoice = SubmenuSelect(IQOptions, 19, 0);                                             // AFP 10-21-22
  }
  calibrateFlag = 1;
  switch (IQChoice) {

  case 0: // Calibrate Frequency  - uses WWV
    CalData.freqCorrectionFactor = GetEncoderValueLive(-200000, 200000, CalData.freqCorrectionFactor, increment, freqCal, false);
    if (CalData.freqCorrectionFactor != freqCorrectionFactorOld) {
      si5351.set_correction(CalData.freqCorrectionFactor, SI5351_PLL_INPUT_XO);
      freqCorrectionFactorOld = CalData.freqCorrectionFactor;
    }
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X - 1, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT + 1, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 1: // CW PA Cal
    CalData.CWPowerCalibrationFactor[ConfigData.currentBand] =
        GetEncoderValueLive(0.0, 1.0, CalData.CWPowerCalibrationFactor[ConfigData.currentBand], 0.01, (char *)"CW PA Cal: ", false);
    ConfigData.powerOutCW[ConfigData.currentBand] =
        sqrt(ConfigData.transmitPowerLevel / 20.0) * CalData.CWPowerCalibrationFactor[ConfigData.currentBand];
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.ConfigDataWrite();
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 2: // CW IQ Receive Cal - Gain and Phase
    cwcalibrater.DoReceiveCalibrate(0, false, false,
                                    true); // This function was significantly revised.  KF5N August 16, 2023
    break;

  case 3: // CW Xmit Carrier calibration.
    cwcalibrater.DoXmitCarrierCalibrate(0, false, false, true);
    break;

  case 4: // CW IQ Transmit Cal - Gain and Phase  //AFP 2-21-23
    cwcalibrater.DoXmitCalibrate(0, false, false,
                                 true); // This function was significantly revised.  KF5N August 16, 2023
    break;

  case 5: // SSB PA Cal
    CalData.SSBPowerCalibrationFactor[ConfigData.currentBand] = GetEncoderValueLive(
        0.0, 1.0, CalData.SSBPowerCalibrationFactor[ConfigData.currentBand], 0.01, (char *)"SSB PA Cal: ", false);
    ConfigData.powerOutSSB[ConfigData.currentBand] =
        sqrt(ConfigData.transmitPowerLevel / 20.0) * CalData.SSBPowerCalibrationFactor[ConfigData.currentBand];
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.CalDataWrite();
        eeprom.ConfigDataWrite();
        calibrateFlag = false;
      }
    }
    break; // Missing break.  KF5N August 12, 2023

  case 6: // SSB receive cal
    cwcalibrater.DoReceiveCalibrate(1, false, false,
                                    true); // This function was significantly revised.  KF5N August 16, 2023
                                           //      eeprom.CalDataWrite(); // Save calibration numbers and
                                           //      configuration.  KF5N August 12, 2023
    break;

  case 7: // SSB Carrier Cal
    ssbcalibrater.DoXmitCarrierCalibrate(false, false, true);
    //      eeprom.CalDataWrite();  // Save calibration numbers and
    //      configuration.  KF5N August 12, 2023
    break;

  case 8: // SSB Transmit cal
    ssbcalibrater.DoXmitCalibrate(false, false,
                                  true); // This function was significantly revised.  KF5N August 16, 2023
                                         //      eeprom.CalDataWrite();  // Save calibration numbers and
                                         //      configuration.  KF5N August 12, 2023
    break;

  case 9: // CW fully automatic radio calibration.
    cwcalibrater.RadioCal(false);
    calibrateFlag = false;
    //      eeprom.CalDataWrite();  // Save calibration numbers and
    //      configuration.  KF5N August 12, 2023
    break;

  case 10: // CW full automatic calibration refinement.
    cwcalibrater.RadioCal(true);
    calibrateFlag = false;
    //      eeprom.CalDataWrite();  // Save calibration numbers and
    //      configuration.  KF5N August 12, 2023
    break;

  case 11: // SSB fully automatic radio calibration.
    ssbcalibrater.RadioCal(false);
    calibrateFlag = false;
    break;

  case 12: // SSB fully automatic calibration refinement.
    ssbcalibrater.RadioCal(true);
    calibrateFlag = false;
    break;

  case 13: // dBm level cal.  Was choose CW calibration tone frequency.
           //      calibrater.SelectCalFreq();
           //      calibrateFlag = false;
    CalData.dBm_calibration = GetEncoderValueLive(0, 100, CalData.dBm_calibration, 1, (char *)"dBm Cal: ", false);
    if (CalData.dBm_calibration != freqCorrectionFactorOld) {
      //        si5351.set_correction(ConfigData.freqCorrectionFactor,
      //        SI5351_PLL_INPUT_XO);
      freqCorrectionFactorOld = CalData.dBm_calibration;
    }
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X - 1, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT + 1, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 14: // Set DAC offset for CW carrier cancellation.
    CalData.dacOffsetCW = GetEncoderValueLiveQ15t(-5000, 5000, CalData.dacOffsetCW, 50, (char *)"DC Offset:", false);
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {
      if (menu == MenuSelect::MENU_OPTION_SELECT) {
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    //      eeprom.CalDataWrite();  // Save calibration numbers and
    //      configuration.  KF5N August 12, 2023
    break;

  case 15: // Set DAC offset for SSB carrier cancellation.
    CalData.dacOffsetSSB = GetEncoderValueLiveQ15t(-5000, 5000, CalData.dacOffsetSSB, 50, (char *)"DC Offset:", false);
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {
      if (menu == MenuSelect::MENU_OPTION_SELECT) {
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    //      eeprom.CalDataWrite();  // Save calibration numbers and
    //      configuration.  KF5N August 12, 2023
    break;

  case 16: // Calibrate buttons
    SaveAnalogSwitchValues();
    calibrateFlag = false;
    RedrawDisplayScreen();
    eeprom.CalDataWrite(); // Save calibration numbers and configuration.  KF5N
                           // August 12, 2023
    break;

  case 17: // Set button repeat rate
    CalData.buttonRepeatDelay =
        1000 * GetEncoderValueLive(0, 5000, CalData.buttonRepeatDelay / 1000, 1, (char *)"Btn Repeat:  ", false);
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {
      if (menu == MenuSelect::MENU_OPTION_SELECT) {
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 18: // Cancelled choice
           //      RedrawDisplayScreen();
           //      currentFreq = TxRxFreq = ConfigData.centerFreq + NCOFreq;
           //      DrawBandWidthIndicatorBar();  // AFP 10-20-22
           //      ShowFrequency();
           //      BandInformation();
    calibrateFlag = false;
    break;

  default:
    break;
  }
  //  UpdateEqualizerField(ConfigData.receiveEQFlag, ConfigData.xmitEQFlag);
}
#else // Not using QSE2 (No carrier calibration)
void CalibrateOptions() {
  int freqCorrectionFactorOld = 0;
  int32_t increment = 100L;
  MenuSelect menu;

  tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 30, CHAR_HEIGHT, RA8875_BLACK);

  // Select the type of calibration, and then skip this during the loop() function.
  if (calibrateFlag == false) {
    const std::string IQOptions[15]{"Freq Cal",       "CW PA Cal",        "CW Rec Cal",   "CW Xmit Cal",   "SSB PA Cal",
                                    "SSB Rec Cal",    "SSB Transmit Cal", "CW Radio Cal", "CW Refine Cal", "SSB Radio Cal",
                                    "SSB Refine Cal", "dBm Level Cal",    "Btn Cal",      "Btn Repeat",    "Cancel"};
    IQChoice = SubmenuSelect(IQOptions, 15, 0);
  }
  calibrateFlag = true;
  switch (IQChoice) {

  case 0: // Calibrate Frequency  - uses WWV
    CalData.freqCorrectionFactor =
        GetEncoderValueLive(-200000, 200000, CalData.freqCorrectionFactor, increment, (char *)"Freq Cal: ", false);
    if (CalData.freqCorrectionFactor != freqCorrectionFactorOld) {
      si5351.set_correction(CalData.freqCorrectionFactor, SI5351_PLL_INPUT_XO);
      freqCorrectionFactorOld = CalData.freqCorrectionFactor;
    }
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X - 1, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT + 1, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 1: // CW PA Cal
    CalData.CWPowerCalibrationFactor[ConfigData.currentBand] =
        GetEncoderValueLive(0.0, 1.0, CalData.CWPowerCalibrationFactor[ConfigData.currentBand], 0.01, (char *)"CW PA Cal: ", false);
    ConfigData.powerOutCW[ConfigData.currentBand] =
        sqrt(ConfigData.transmitPowerLevel / 20.0) * CalData.CWPowerCalibrationFactor[ConfigData.currentBand];
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.ConfigDataWrite();
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;
  case 2: // CW IQ Receive Cal - Gain and Phase
    // This function was significantly revised.  KF5N August 16, 2023
    cwcalibrater.DoReceiveCalibrate(0, false, false, true);
    // Save calibration numbers and configuration.  KF5N August 12, 2023
    // eeprom.CalDataWrite();
    break;
  case 3: // CW IQ Transmit Cal - Gain and Phase
    // This function was significantly revised.  KF5N August 16, 2023
    cwcalibrater.DoXmitCalibrate(0, false, false, true);

    // Save calibration numbers and configuration.  KF5N August 12, 2023
    // eeprom.CalDataWrite();
    break;
  case 4: // SSB PA Cal
    CalData.SSBPowerCalibrationFactor[ConfigData.currentBand] = GetEncoderValueLive(
        0.0, 1.0, CalData.SSBPowerCalibrationFactor[ConfigData.currentBand], 0.01, (char *)"SSB PA Cal: ", false);
    ConfigData.powerOutSSB[ConfigData.currentBand] =
        sqrt(ConfigData.transmitPowerLevel / 20.0) * CalData.SSBPowerCalibrationFactor[ConfigData.currentBand];
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.ConfigDataWrite();
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }

    break; // Missing break.  KF5N August 12, 2023

  case 5: // SSB IQ Receive Cal - Gain and Phase
    cwcalibrater.DoReceiveCalibrate(1, false, false,
                                    true); // This function was significantly revised.  KF5N August 16, 2023
                                           //      eeprom.CalDataWrite();                             //
                                           //      Save calibration numbers and configuration.  KF5N August
                                           //      12, 2023
    break;

  case 6:
    ssbcalibrater.DoXmitCalibrate(false, false, true); // SSB Transmit cal
                                                       //      eeprom.CalDataWrite(); // Save calibration
                                                       //      numbers and configuration.  KF5N August 12,
                                                       //      2023
    break;

  case 7: //  CW fully automatic radio calibration.
    cwcalibrater.RadioCal(false);
    calibrateFlag = false;
    break;

  case 8: // CW fully automatic calibration refinement.
    cwcalibrater.RadioCal(true);
    calibrateFlag = false;
    break;

  case 9: // SSB fully automatic radio calibration.
    ssbcalibrater.RadioCal(false);
    calibrateFlag = false;
    break;

  case 10: // SSB fully automatic calibration refinement.
    ssbcalibrater.RadioCal(true);
    calibrateFlag = false;
    break;

  case 11: // dBm level cal.  Was choose CW calibration tone frequency.
           //      calibrater.SelectCalFreq();
           //      calibrateFlag = false;
    CalData.dBm_calibration = GetEncoderValueLive(0, 100, CalData.dBm_calibration, 1, (char *)"dBm Cal: ", false);
    if (CalData.dBm_calibration != freqCorrectionFactorOld) {
      //        si5351.set_correction(ConfigData.freqCorrectionFactor,
      //        SI5351_PLL_INPUT_XO);
      freqCorrectionFactorOld = CalData.dBm_calibration;
    }
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X - 1, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT + 1, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 12: // Calibrate buttons
    SaveAnalogSwitchValues();
    calibrateFlag = false;
    RedrawDisplayScreen();
    //      ShowFrequency();
    //      DrawFrequencyBarValue();
    eeprom.CalDataWrite(); // Save calibration numbers and configuration.  KF5N
                           // August 12, 2023
    break;

  case 13: // Set button repeat rate
    CalData.buttonRepeatDelay =
        1000 * GetEncoderValueLive(0, 5000, CalData.buttonRepeatDelay / 1000, 1, (char *)"Btn Repeat:  ", false);
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {
      if (menu == MenuSelect::MENU_OPTION_SELECT) {
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT, RA8875_BLACK);
        eeprom.CalDataWrite();
        calibrateFlag = false;
      }
    }
    break;

  case 14: // Cancelled choice
    calibrateFlag = false;
    break;

  default:
    break;
  }
}
#endif

/*****
  Purpose: Present the CW options available to the user.  Change and store to
ConfigData.

  Parameter list:
    void

  Return value
    void
*****/
void CWOptions() // new option for Sidetone and Delay JJP 9/1/22
{
  // const char *cwChoices[]{ "Decode Sens", "CW Filter", "CW Offset", "WPM",
  // "Sidetone Volume", "Key Type", "Paddle Flip", "Transmit Delay", "Cancel" };
  std::string cwChoices[]{"Decode Sens", "CW Filter",   "CW Offset",      "WPM",   "Sidetone Speaker", "Sidetone Headpho",
                          "Key Type",    "Paddle Flip", "Transmit Delay", "Cancel"};
  int CWChoice = 0;
  uint32_t morseDecodeSensitivityOld = 0;
  //  uint32_t increment = 10;
  MenuSelect menu;

  if (morseDecodeAdjustFlag == false) {
    CWChoice = SubmenuSelectString(cwChoices, 10, 0);
    if (CWChoice == 0) {
      morseDecodeAdjustFlag = true; // Handle the special case of Morse decoder
                                    // adjust; the loop must run.
    }
  }

  switch (CWChoice) {

  case 0: // Set Morse decoder sensitivity.
    ConfigData.morseDecodeSensitivity =
        GetEncoderValueLiveString(0, 10000, ConfigData.morseDecodeSensitivity, 100, cwChoices[CWChoice], false);
    if (ConfigData.morseDecodeSensitivity != morseDecodeSensitivityOld) {
      morseDecodeSensitivityOld = ConfigData.morseDecodeSensitivity;
    }
    menu = readButton();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // Any button press??
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Yep. Make a choice??
        tft.fillRect(SECONDARY_MENU_X - 1, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT + 1, RA8875_BLACK);
        eeprom.ConfigDataWrite();
        morseDecodeAdjustFlag = false;
      }
    }
    break;

  case 1:             // CW Filter BW:      // AFP 10-18-22
    SelectCWFilter(); // in CWProcessing    // AFP 10-18-22
    break;            // AFP 10-18-22

  case 2:             // Select a preferred CW offset frequency.
    SelectCWOffset(); //  Located in CWProcessing.cpp
    break;

  case 3: // WPM
    SetWPM();
    SetTransmitDitLength(ConfigData.currentWPM); // Afp 09-22-22     // JJP 8/19/23
    break;

  case 4: // Sidetone volume for speaker.
    SetSideToneVolume(true);
    break;

  case 5: // Sidetone volume for headphone.
    SetSideToneVolume(false);
    break;

  case 6:         // Type of key:
    SetKeyType(); // Straight key or keyer? Stored in ConfigData.keyType.
    SetKeyPowerUp();
    UpdateWPMField();
    break;

  case 7: // Flip paddles
    DoPaddleFlip();
    break;

  case 8:               // new function JJP 9/1/22
    SetTransmitDelay(); // Transmit relay hold delay
    break;

  default: // Cancel
    break;
  }
}

/*****
  Purpose: Show the list of scales for the spectrum divisions

  Parameter list:
    void

  Return value
    int           an index into displayScale[] array, or -1 on cancel
*****/
void SpectrumOptions() { /*
  dispSc displayScale[] =  //r *dbText,dBScale, baseOffset
  {
    {"20 dB/", 10.0, 24},
    {"10 dB/", 20.0, 10},  // JJP 7/14/23
  };
  */
  const std::string spectrumChoices[] = {"20 dB/unit", "10 dB/unit", "Cancel"};
  int spectrumSet = ConfigData.currentScale; // JJP 7/14/23

  spectrumSet = SubmenuSelect(spectrumChoices, 3, spectrumSet);
  if (strcmp(spectrumChoices[spectrumSet].c_str(), "Cancel") == 0) {
    return;
  }
  ConfigData.currentScale = spectrumSet; // Yep...
  eeprom.ConfigDataWrite();
  //  RedrawDisplayScreen();
  ShowSpectrumdBScale();
  lastState = RadioState::NOSTATE; // Force update of operating state.
}

/*****
  Purpose: Select AGC option.

  Parameter list:
    void

  Return value
    void
*****/
void AGCOptions() {
  const std::string AGCChoices[] = {"AGC On", "AGC Off", "AGC Threshold", "Cancel"}; // G0ORX (Added Long) September 5, 2023
  int agcSet = 0;

  agcSet = SubmenuSelect(AGCChoices, 4, ConfigData.AGCMode); // G0ORX

  switch (agcSet) {
  case 0: // AGC On
    ConfigData.AGCMode = true;
    SetAudioOperatingState(radioState);
    UpdateAGCField();
    break;

  case 1: // AGC Off
    ConfigData.AGCMode = false;
    SetAudioOperatingState(radioState);
    UpdateAGCField();
    break;

  case 2: // Set AGC threshold
          //      ConfigData.AGCThreshold =
          //      static_cast<float32_t>(GetEncoderValue(-60, -20,
          //      ConfigData.AGCThreshold, 1, "AGC Threshold"));
          //      ConfigData.AGCThreshold = GetEncoderValueLiveString(-60.0,
          //      -20.0, ConfigData.AGCThreshold, 1.0, "AGC Thr ", false);
    ConfigData.AGCThreshold = static_cast<float32_t>(GetEncoderValue(-60, -20, ConfigData.AGCThreshold, 1, "AGC Threshold "));

    initializeAudioPaths();
    break;

  case 4: // Cancel
    return;
    break;

  default:
    break;
  }
  eeprom.ConfigDataWrite();
}

/*****
  Purpose: To process the graphics for the 14 chan equalizer option

  Parameter list:
    int array[]         The array to fill in.  0 is receive, 1 is transmit.
    char *title         The equalizer being set.
  Return value
    void
*****/
void ProcessEqualizerChoices(int EQType, char *title) {
  //  for (int i = 0; i < EQUALIZER_CELL_COUNT; i++) {
  //  }

  std::string rXeqFreq[14]{" 200", " 250", " 315", " 400", " 500", " 630", " 800",
                           "1000", "1250", "1600", "2000", "2500", "3150", "4000"};
  std::string tXeqFreq[14]{"  50", "  71", " 100", " 141", " 200", " 283", " 400",
                           " 566", " 800", "1131", "1600", "2263", "3200", "4526"};

  int yLevel[EQUALIZER_CELL_COUNT]; // EQUALIZER_CELL_COUNT 14

  int columnIndex;
  int iFreq;
  int newValue;
  int xOrigin = 50;
  int xOffset;
  int yOrigin = 50;
  int wide = 700;
  int high = 300;
  int barWidth = 46;
  int barTopY;
  int barBottomY;
  MenuSelect menu = MenuSelect::DEFAULT;

  for (iFreq = 0; iFreq < EQUALIZER_CELL_COUNT; iFreq++) {
    if (EQType == 0) {
      yLevel[iFreq] = ConfigData.equalizerRec[iFreq];
    } else {
      if (EQType == 1) {
        yLevel[iFreq] = (ConfigData.equalizerXmt[iFreq] * 10) + 100;
      }
    }
  }
  tft.writeTo(L2);
  tft.clearMemory();
  tft.writeTo(L1);
  tft.fillWindow(RA8875_BLACK);

  tft.fillRect(xOrigin - 50, yOrigin - 25, wide + 50, high + 50,
               RA8875_BLACK); // Clear data area
  tft.setTextColor(RA8875_GREEN);
  tft.setFontScale((enum RA8875tsize)1);
  tft.setCursor(200, 0);
  tft.print(title);

  tft.drawRect(xOrigin - 4, yOrigin, wide + 4, high, RA8875_BLUE);
  tft.drawFastHLine(xOrigin - 4, yOrigin + (high / 2), wide + 4,
                    RA8875_RED); // Print center zero line center
  tft.setFontScale((enum RA8875tsize)0);

  tft.setTextColor(RA8875_WHITE);
  tft.setCursor(xOrigin - 4 - tft.getFontWidth() * 3, yOrigin + tft.getFontHeight());
  tft.print("+12");
  tft.setCursor(xOrigin - 4 - tft.getFontWidth() * 3, yOrigin + (high / 2) - tft.getFontHeight());
  tft.print(" 0");
  tft.setCursor(xOrigin - 4 - tft.getFontWidth() * 3, yOrigin + high - tft.getFontHeight() * 2);
  tft.print("-12");

  barTopY = yOrigin + (high / 2);               // 50 + (300 / 2) = 200
  barBottomY = barTopY + DEFAULT_EQUALIZER_BAR; // Default 200 + 100

  for (iFreq = 0; iFreq < EQUALIZER_CELL_COUNT; iFreq++) {
    tft.fillRect(xOrigin + (barWidth + 4) * iFreq, barTopY - (yLevel[iFreq] - DEFAULT_EQUALIZER_BAR), barWidth, yLevel[iFreq],
                 RA8875_CYAN);
    tft.setCursor(xOrigin + (barWidth + 4) * iFreq, yOrigin + high - tft.getFontHeight() * 2);
    if (EQType == 0) {
      tft.print(rXeqFreq[iFreq].c_str());
    } else {
      tft.print(tXeqFreq[iFreq].c_str());
    }
    tft.setCursor(xOrigin + (barWidth + 4) * iFreq + tft.getFontWidth() * 1.5, yOrigin + high + tft.getFontHeight() * 2);
    if (EQType == 0) {
      tft.print(yLevel[iFreq]);
    }
    if (EQType == 1) {
      tft.print(ConfigData.equalizerXmt[iFreq]);
    }
  }

  columnIndex = 0; // Get ready to set values for columns
  newValue = 0;
  while (columnIndex < EQUALIZER_CELL_COUNT) {
    xOffset = xOrigin + (barWidth + 4) * columnIndex; // Just do the math once
                                                      //    tft.fillRect(xOffset, // Indent to proper bar...
                                                      //    Removed this rectangle.  Seems unnecessary.  KF5N
                                                      //    November 12, 2023
                                                      //                 barBottomY - yLevel[columnIndex] -
                                                      //                 1,  // Start at red line barBottomY
                                                      //                 - 1, barWidth, // Set bar width
                                                      //                 newValue + 1, // Erase old bar -100,
                                                      //                 RA8875_BLACK);

    tft.fillRect(xOffset,                          // Indent to proper bar...
                 barBottomY - yLevel[columnIndex], // Start at red line
                 barWidth,                         // Set bar width
                 yLevel[columnIndex],              // Draw new bar
                 RA8875_MAGENTA);
    while (true) {
      newValue = yLevel[columnIndex]; // Get current value
      if (filterEncoderMove != 0) {

        tft.fillRect(xOffset,                   // Indent to proper bar...
                     barBottomY - newValue - 1, // Start at red line
                     barWidth,                  // Set bar width
                     yLevel[columnIndex] + 1,   // Erase old bar
                     RA8875_BLACK);
        //        newValue += (PIXELS_PER_EQUALIZER_DELTA * filterEncoderMove);
        //        // Find new bar height. OK since filterEncoderMove equals 1 or
        //        -1. PIXELS_PER_EQUALIZER_DELTA = 10
        yLevel[columnIndex] += (PIXELS_PER_EQUALIZER_DELTA * filterEncoderMove);
        tft.fillRect(xOffset,                          // Indent to proper bar...
                     barBottomY - yLevel[columnIndex], // Start at red line
                     barWidth,                         // Set bar width
                     yLevel[columnIndex],              // Draw new bar
                     RA8875_MAGENTA);
        //        yLevel[columnIndex] = newValue;

        tft.fillRect(xOffset + tft.getFontWidth() * 1.5 - 1,
                     yOrigin + high + tft.getFontHeight() * 2, // Update bottom number
                     barWidth, CHAR_HEIGHT, RA8875_BLACK);
        tft.setCursor(xOffset + tft.getFontWidth() * 1.5, yOrigin + high + tft.getFontHeight() * 2);
        if (EQType == 0) {
          tft.print(yLevel[columnIndex]);
        } else {
          tft.print((yLevel[columnIndex] - 100) / 10);
        }
        if (yLevel[columnIndex] < DEFAULT_EQUALIZER_BAR) { // Repaint red center line if erased
          tft.drawFastHLine(xOrigin - 4, yOrigin + (high / 2), wide + 4,
                            RA8875_RED); // Clear hole in display center
        }
      }
      filterEncoderMove = 0;
      menu = readButton(); // Read the ladder value
      if (menu != MenuSelect::BOGUS_PIN_READ) {

        tft.fillRect(xOffset,               // Indent to proper bar...
                     barBottomY - newValue, // Start at red line
                     barWidth,              // Set bar width
                     newValue,              // Draw new bar
                     RA8875_GREEN);

        if (EQType == 0) {
          ConfigData.equalizerRec[columnIndex] = newValue;
        } else {
          if (EQType == 1) {
            ConfigData.equalizerXmt[columnIndex] = (yLevel[columnIndex] - 100) / 10;
          }
        }

        filterEncoderMove = 0;
        columnIndex++;
        break;
      } // end inner while
    } // end outer while
    eeprom.ConfigDataWrite();
  }
  RedrawDisplayScreen();
  lastState = RadioState::NOSTATE; // Force update of operating state.
}

/*****
  Purpose: Receive EQ set

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void EqualizerRecOptions() {
  const std::string RecEQChoices[] = {"RX EQ On", "RX EQ Off", "RX EQSet", "Cancel"}; // Add code practice oscillator
  int EQChoice = 0;

  EQChoice = SubmenuSelect(RecEQChoices, 4, 0);

  switch (EQChoice) {
  case 0:
    ConfigData.receiveEQFlag = true;
    button.ExecuteModeChange();
    break;
  case 1:
    ConfigData.receiveEQFlag = false;
    button.ExecuteModeChange();
    break;
  case 2:
    ProcessEqualizerChoices(0, (char *)"Receive Equalizer");
    break;
  case 3:
    break;
  }
  eeprom.ConfigDataWrite();
  //  RedrawDisplayScreen();
  UpdateEqualizerField(ConfigData.receiveEQFlag, ConfigData.xmitEQFlag);
}

/*****
  Purpose: Xmit EQ options

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void EqualizerXmtOptions() {
  const std::string XmtEQChoices[] = {"TX EQ On", "TX EQ Off", "TX EQSet", "Cancel"}; // Add code practice oscillator
  int EQChoice = 0;

  EQChoice = SubmenuSelect(XmtEQChoices, 4, 0);

  switch (EQChoice) {
  case 0:
    ConfigData.xmitEQFlag = true;
    break;
  case 1:
    ConfigData.xmitEQFlag = false;
    break;
  case 2:
    ProcessEqualizerChoices(1, (char *)"Transmit Equalizer");
    break;
  case 3: // Do nothing and exit.
    break;
  }
  eeprom.ConfigDataWrite();
  //  RedrawDisplayScreen();
  UpdateEqualizerField(ConfigData.receiveEQFlag, ConfigData.xmitEQFlag);
}

/*****
  Purpose: Set options for the SSB exciter.

  Parameter list:
    void

  Return value
    int           an index into the band array
*****/
void SSBOptions() // AFP 09-22-22 All new
{
  static int micChoice = 0;
  float imdAmplitude = 0.0;
  int imdAmplitudedB = 100;
  MenuSelect menu = MenuSelect::BOGUS_PIN_READ;
  const std::string micChoices[] = {"CESSB",          "SSB",        "FT8",      "Comp On", "Comp Off", "Mic Gain",
                                    "Comp Threshold", "Comp Ratio", "IMD Test", "Cancel"};

  micChoice = SubmenuSelect(micChoices, 10, micChoice);
  switch (micChoice) {

  case 0: // CESSB on
    ConfigData.cessb = true;
    cessb1.setProcessing(ConfigData.cessb);
    //      Serial.printf("processing = %d", cessb1.getProcessing());
    BandInformation();
    break;

  case 1: // SSB Data on
    ConfigData.cessb = false;
    cessb1.setProcessing(ConfigData.cessb);
    //      Serial.printf("processing = %d", cessb1.getProcessing());
    BandInformation();
    break;

  case 2: // FT8
    ConfigData.cessb = false;
    cessb1.setProcessing(ConfigData.cessb);
    BandInformation();
    break;

  case 3: // Compressor On
    ConfigData.compressorFlag = true;
    UpdateCompressionField();
    break;

  case 4: // Compressor Off
    ConfigData.compressorFlag = false;
    UpdateCompressionField();
    break;

  case 5: // Adjust mic gain in dB.  Default 0 db.
    MicGainSet();
    break;

  case 6: // Set compression ratio.  Default -10 dB.
    SetCompressionThreshold();
    UpdateCompressionField();
    break;

  case 7: // Set compressor threshold.  Default 100.0.
    SetCompressionRatio();
    UpdateCompressionField();
    break;

  case 8: // IMD test.  This is a self-contained loop which uses the SSB
          // exciter.

    radioState = RadioState::SSB_IM3TEST_STATE;
    bands.bands[ConfigData.currentBand].mode = RadioMode::SSB_MODE;
    SetFreq();
    digitalWrite(RXTX, HIGH); // xmit on
    ShowTransmitReceiveStatus();
    SetAudioOperatingState(radioState);
    button.ExecuteModeChange();
    while (menu != MenuSelect::MENU_OPTION_SELECT) {
      menu = readButton(); // Use this to quit.
      // Return IMD amplitude in dB.
      imdAmplitudedB = GetEncoderValueLive(0.0, 200.0, imdAmplitudedB, 1.0, micChoices[8], false);
      imdAmplitude = pow(10.0, (static_cast<float>(imdAmplitudedB) - 1.0) / 40.0) * 0.00001;
      AudioNoInterrupts();
      toneSSBCal1.amplitude(imdAmplitude);
      toneSSBCal2.amplitude(imdAmplitude);
      AudioInterrupts();
      ExciterIQData();
    }
    radioState = RadioState::SSB_RECEIVE_STATE;
    digitalWrite(RXTX, LOW); // Transmitter off.
    SetAudioOperatingState(radioState);
    button.ExecuteModeChange();
    ShowTransmitReceiveStatus();
    if (menu != MenuSelect::BOGUS_PIN_READ) {       // A button press?
      if (menu == MenuSelect::MENU_OPTION_SELECT) { // Exit.
        tft.fillRect(SECONDARY_MENU_X - 1, MENUS_Y, EACH_MENU_WIDTH + 35, CHAR_HEIGHT + 1, RA8875_BLACK);
      }
    }
    break;

  case 9: // Cancel
    return;
    break;

  default:
    return;
    break;
  }
  updateMic();
  eeprom.ConfigDataWrite();
}

/*****
  Purpose: Present the bands available and return the selection

  Parameter list:
    void

  Return value12
    int           an index into the band array
*****/
void RFOptions() {
  //  const char *rfOptions[] = { "TX Power Set", "RF Gain Set", "RF Auto-Gain
  //  On", "RF Auto-Gain Off", "Auto-Spectrum On", "AutoSpectrum Off", "Cancel"
  //  };
  const std::string rfOptions[] = {"TX Power Set",     "RF Gain Set",      "RF Auto-Gain On", "RF Auto-Gain Off",
                                   "Auto-Spectrum On", "AutoSpectrum Off", "Cancel"};
  int rfSet = 0;
  rfSet = SubmenuSelect(rfOptions, 7, rfSet);

  switch (rfSet) {
  case 0: // TX Power Set.  AFP 10-21-22
    ConfigData.transmitPowerLevel = static_cast<float32_t>(GetEncoderValue(1, 20, ConfigData.transmitPowerLevel, 1, "Power: "));
    // When the transmit power level is set, this means ALL of the power
    // coefficients must be revised! powerOutCW and powerOutSSB must be updated.
    initPowerCoefficients();
    eeprom.ConfigDataWrite(); // AFP 10-21-22
    //      BandInformation();
    ShowCurrentPowerSetting();
    break;

  case 1: // Manual gain set.
    ConfigData.rfGain[ConfigData.currentBand] =
        GetEncoderValue(-60, 20, ConfigData.rfGain[ConfigData.currentBand], 5, (char *)"RF Gain dB: ");
    eeprom.ConfigDataWrite();
    break;

  case 2: // Auto-Gain On
    ConfigData.autoGain = true;
    ConfigData.autoSpectrum = false; // Make sure Auto-Spectrum is off.
                                     //      fftOffset = 0;
    ShowAutoStatus();
    eeprom.ConfigDataWrite();
    break;

  case 3: // Auto-Gain Off
    ConfigData.autoGain = false;
    ShowAutoStatus();
    eeprom.ConfigDataWrite();
    break;

  case 4: // Auto-Spectrum On
    ConfigData.autoSpectrum = true;
    ConfigData.autoGain = false; // Make sure Auto-Gain is off.
    ShowAutoStatus();
    eeprom.ConfigDataWrite();
    break;

  case 5: // Auto-Spectrum Off
    ConfigData.autoSpectrum = false;
    //      fftOffset = 0;
    ShowAutoStatus();
    eeprom.ConfigDataWrite();
    break;

  default: // Cancel
    break;
  }
}

/*****
  Purpose: This option reverses the dit and dah paddles on the keyer

  Parameter list:
    void

  Return value
    void
*****/
void DoPaddleFlip() {
  const char *paddleState[] = {"Right paddle = dah", "Right paddle = dit"};
  int choice, lastChoice;
  MenuSelect pushButtonSwitchIndex;
  //  int valPin;

  ConfigData.paddleDah = KEYER_DAH_INPUT_RING; // Defaults
  ConfigData.paddleDit = KEYER_DIT_INPUT_TIP;
  choice = lastChoice = 0;

  tft.setTextColor(RA8875_BLACK);
  tft.fillRect(SECONDARY_MENU_X - 100, MENUS_Y, EACH_MENU_WIDTH + 100, CHAR_HEIGHT, RA8875_GREEN);
  tft.setCursor(SECONDARY_MENU_X - 93, MENUS_Y + 1);
  tft.print(paddleState[choice]); // Show the default (right paddle = dah

  while (true) {
    delay(150L);
    //    valPin = ReadSelectedPushButton();                     // Poll buttons
    //    if (valPin != -1) {                                    // button was
    //    pushed
    pushButtonSwitchIndex = readButton(); // Winner, winner...chicken dinner!
    if (pushButtonSwitchIndex == MenuSelect::MAIN_MENU_UP || pushButtonSwitchIndex == MenuSelect::MAIN_MENU_DN) {
      choice = !choice; // Reverse the last choice
      tft.fillRect(SECONDARY_MENU_X - 100, MENUS_Y, EACH_MENU_WIDTH + 100, CHAR_HEIGHT, RA8875_GREEN);
      tft.setCursor(SECONDARY_MENU_X - 93, MENUS_Y + 1);
      tft.print(paddleState[choice]);
    }
    if (pushButtonSwitchIndex == MenuSelect::MENU_OPTION_SELECT) {
      if (choice) {
        // Means right-paddle dit
        ConfigData.paddleDit = KEYER_DAH_INPUT_RING;
        ConfigData.paddleDah = KEYER_DIT_INPUT_TIP;
        ConfigData.paddleFlip = PaddleFlipEnum::RIGHT_PADDLE_DIT;
      } else {
        ConfigData.paddleDit = KEYER_DIT_INPUT_TIP;
        ConfigData.paddleDah = KEYER_DAH_INPUT_RING;
        ConfigData.paddleFlip = PaddleFlipEnum::RIGHT_PADDLE_DAH;
        ;
      }
      EraseMenus();
      UpdateWPMField();
      break;
    }
  }
  eeprom.ConfigDataWrite();
}

FLASHMEM void VFOSelect() {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_TRACE, "ConfigData.activeVFO:%d", ConfigData.activeVFO);
  constexpr size_t VFOOptionsSize = 4;
  const std::string VFOOptions[VFOOptionsSize] = {"VFO A", "VFO B", "VFO Split", "Cancel"};

  int selectedItem = SubmenuSelect(VFOOptions, 4, 0);

  if (selectedItem < 0 or selectedItem >= VFOOptionsSize) {
    TRACE_T41(TR_L_TRACE, "unsupported selectedItem:%d", selectedItem);
    return;
  }

  VfoState choice;
  choice = static_cast<VfoState>(selectedItem);

  tft.setTextColor(RA8875_BLACK);
  tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH, CHAR_HEIGHT, RA8875_GREEN);
  tft.setCursor(SECONDARY_MENU_X + 7, MENUS_Y + 1);
  tft.print(VFOOptions[static_cast<size_t>(choice)].c_str());

  delay(10);
  NCOFreq = 0L;
  splitOn = 0;
  switch (choice) {
  case VfoState::VFO_A: // VFO A
    ConfigData.centerFreq = TxRxFreq = ConfigData.currentFreqA;
    ConfigData.activeVFO = VfoState::VFO_A;
    ConfigData.currentBand = ConfigData.currentBandA;
    // tft.fillRect(FILTER_PARAMETERS_X + 180, FILTER_PARAMETERS_Y, 150, 20, RA8875_BLACK); // Erase split message
    splitOn = 0;
    break;
  case VfoState::VFO_B: // VFO B
    ConfigData.centerFreq = TxRxFreq = ConfigData.currentFreqB;
    ConfigData.activeVFO = VfoState::VFO_B;
    ConfigData.currentBand = ConfigData.currentBandB;
    // tft.fillRect(FILTER_PARAMETERS_X + 180, FILTER_PARAMETERS_Y, 150, 20, RA8875_BLACK); // Erase split message
    splitOn = 0;
    break;
  case VfoState::VFO_SPLIT: // Split
    DoSplitVFO();
    ConfigData.activeVFO = VfoState::VFO_SPLIT;
    splitOn = 1;
    break;

  default: // Cancel
    break;
  }

  bands.bands[ConfigData.currentBand].freq = TxRxFreq;
  //  SetBand();           // KF5N July 12, 2023
  SetBandRelay(); // Required when switching VFOs. KF5N July 12, 2023
  SetFreq();
  RedrawDisplayScreen();
  //  BandInformation();
  //  ShowBandwidth();
  FilterBandwidth();
  // delete old digit
  tft.fillRect(FREQUENCY_X_SPLIT, FREQUENCY_Y - 12, VFOB_PIXEL_LENGTH, FREQUENCY_PIXEL_HI, RA8875_BLACK);
  // delete old digit
  tft.fillRect(FREQUENCY_X, FREQUENCY_Y - 12, VFOA_PIXEL_LENGTH, FREQUENCY_PIXEL_HI, RA8875_BLACK);
  // tft.setFontScale( (enum RA8875tsize) 0);
  ShowFrequency();
  // Draw or not draw CW filter graphics to audio spectrum area.  KF5N July 30, 2023
  tft.writeTo(L2);
  tft.clearMemory();
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE) {
    BandInformation();
  }
  //  DrawBandWidthIndicatorBar();
  //  DrawFrequencyBarValue();
  //  UpdateAudioGraphics();
}

/*****
  Purpose: Allow user to set current user configuration values or restore
default settings.

  Parameter list:
    void

  Return value
    void
*****/
void ConfigDataOptions() {
  const std::string ConfigDataOpts[11] = {
      "Save RAM->EEPROM", "Load Def.->RAM",  "Get Favorite", "Set Favorite", "EEPROM->SD", "SD->RAM->EEPROM",
      "EEPROM->Serial",   "Default->Serial", "RAM->Serial",  "SD->Serial",   "Cancel",
  };
  int defaultOpt = SubmenuSelect(ConfigDataOpts, 11, 0);
  switch (defaultOpt) {
  case 0:
    // Save RAM->EEPROM - Save current configs to ConfigData non-volatile memory.
    eeprom.ConfigDataWrite();
    eeprom.BandsWrite();
    break;
  case 1:
    // Load Def.->RAM - Restore defaults to ConfigData struct and refresh display.
    eeprom.ConfigDataDefaults();
    break;
  case 2:
    // Get a stored frequency and store in active VFO
    eeprom.GetFavoriteFrequency();
    break;
  case 3:
    // Set favorites
    eeprom.SetFavoriteFrequency();
    break;
  case 4:
    // Copy EEPROM->SD
    {
      config_t tempConfig;
      EEPROM.get(EEPROM_BASE_ADDRESS + 4, tempConfig);
      configurationData.saveConfiguration(configFilename, tempConfig);
    }
    break;
  case 5:
    // SD->RAM->EEPROM
    configurationData.loadConfiguration(configFilename, ConfigData);
    loadMyConfigutation(myConfigFilename, ConfigData);
    eeprom.ConfigDataWrite(); // Write to ConfigData non-volatile memory.
    initUserDefinedStuff();   // Various things must be initialized.  This is normally done in setup().  KF5N February 21, 2024
    tft.writeTo(L2);          // This is specifically to clear the bandwidth indicator bar.  KF5N August 7, 2023
    tft.clearMemory();
    tft.writeTo(L1);
    RedrawDisplayScreen(); // Assume there are lots of changes and do a heavy-duty refresh.  KF5N August 7, 2023
    break;
  case 6:
    // EEPROM->Serial
    {
      Serial.println(F("Begin EEPROM->Serial"));
      config_t ConfigData_temp;
      EEPROM.get(EEPROM_BASE_ADDRESS + 4, ConfigData_temp);
      // Write the temporary struct to the serial monitor.
      configurationData.printConfiguration(ConfigData_temp);
      Serial.println(F("End EEPROM->Serial"));
    }
    break;
  case 7:
    // Defaults->Serial
    {
      Serial.println(F("Begin Defaults->Serial"));
      config_t defaultConfig;
      // Write default ConfigData struct to the Serial monitor.
      configurationData.printConfiguration(defaultConfig);
      Serial.println(F("End Defaults->Serial"));
    }
    break;

  case 8:
    // RAM->Serial
    Serial.println(F("Begin RAM->Serial"));
    configurationData.printConfiguration(ConfigData);
    Serial.println(F("End RAM->Serial"));
    break;
  case 9:
    // SD->Serial
    Serial.println(F("Begin SD->Serial print file"));
    printFile(configFilename);
    printFile(myConfigFilename);
    Serial.println(F("End SD->Serial print file"));
    break;
  default:
    break;
  }
}

/*****
  Purpose: Allow user to set restore calibration values or restore default
settings.

  Parameter list:
    void

  Return value
    void
*****/
void CalDataOptions() {
  const std::string CalDataOpts[9] = {"Save RAM->EEPROM", "Load Def.->RAM", "EEPROM->SD", "SD->RAM->EEPROM", "EEPROM->Serial",
                                      "Default->Serial",  "RAM->Serial",    "SD->Serial", "Cancel"};
  int defaultOpt = SubmenuSelect(CalDataOpts, 9, 0);
  switch (defaultOpt) {
  case 0: // Save RAM->EEPROM - Save current CalData struct to ConfigData non-volatile memory.
    eeprom.CalDataWrite();
    break;
  case 1:
    // Load Def.->RAM - Restore defaults to CalData struct and refresh display.
    eeprom.CalDataDefaults();
    break;
  case 2:
    // Copy EEPROM->SD
    {
      calibration_t tempCal; // A temporary calibration_t struct to copy CalData data into.
      EEPROM.get(CAL_BASE_ADDRESS + 4, tempCal);
      calibrationData.saveCalibration(calFilename, tempCal, true);
    }
    break;
  case 3:
    // SD->RAM->EEPROM
    calibrationData.loadCalibration(calFilename, CalData);
    loadMyCalibration(myCalFilename, CalData);
    eeprom.CalDataWrite();
    initUserDefinedStuff(); // Various things must be initialized.  This is normally done in setup().  KF5N February 21, 2024
    tft.writeTo(L2);        // This is specifically to clear the bandwidth indicator bar.  KF5N August 7, 2023
    tft.clearMemory();
    tft.writeTo(L1);
    RedrawDisplayScreen(); // Assume there are lots of changes and do a heavy-duty refresh.  KF5N August 7, 2023
    break;
  case 4:
    // EEPROM->Serial
    {
      Serial.println(F("Begin EEPROM->Serial"));
      calibration_t CalData_temp;
      EEPROM.get(CAL_BASE_ADDRESS + 4, CalData_temp);
      calibrationData.saveCalibration(calFilename, CalData_temp, false); // Write the temporary struct to the serial monitor.
      Serial.println(F("End EEPROM->Serial"));
    }
    break;
  case 5:
    // Defaults->Serial
    {
      Serial.println(F("Begin Defaults->Serial"));
      calibration_t defaultCal;
      calibrationData.saveCalibration(calFilename, defaultCal, false); // Write default CalData struct to the Serial monitor.
      Serial.println(F("End Defaults->Serial"));
    }
    break;
  case 6:
    // RAM->Serial
    Serial.println(F("Begin RAM->Serial"));
    calibrationData.saveCalibration(calFilename, CalData, false);
    Serial.println(F("End RAM->Serial"));
    break;
  case 7:
    // SD->Serial
    Serial.println(F("Begin SD->Serial print file"));
    printFile(calFilename);
    printFile(myCalFilename);
    Serial.println(F("End SD->Serial print file"));
    break;
  default:
    break;
  }
}

/*****
  Purpose: To select an option from a submenu

  Parameter list:
    char *options[]           submenus
    int numberOfChoices       choices available
    int defaultState          the starting option

  Return value
    int           an index into the band array
*****/
// int SubmenuSelect(const char *options[], int numberOfChoices, int
// defaultStart) {
int SubmenuSelect(const std::string options[], int numberOfChoices, int defaultStart) {
  int refreshFlag = 0;
  MenuSelect menu;
  int encoderReturnValue;

  tft.setTextColor(RA8875_BLACK);
  encoderReturnValue = defaultStart; // Start the options using this option

  tft.setFontScale((enum RA8875tsize)1);
  if (refreshFlag == 0) {
    // Show the option in the second field
    tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH, CHAR_HEIGHT, RA8875_GREEN);
    tft.setCursor(SECONDARY_MENU_X + 1, MENUS_Y + 1);
    tft.print(options[encoderReturnValue].c_str()); // Secondary Menu
    refreshFlag = 1;
  }

  while (true) {
    menu = readButton();                      // Read the ladder value
    if (menu != MenuSelect::BOGUS_PIN_READ) { // Valid choice?
      switch (menu) {
      case MenuSelect::MENU_OPTION_SELECT: // They made a choice
        tft.setTextColor(RA8875_WHITE);
        EraseMenus();
        return encoderReturnValue;
        break;

      case MenuSelect::MAIN_MENU_UP:
        encoderReturnValue++;
        if (encoderReturnValue >= numberOfChoices) {
          encoderReturnValue = 0;
        }
        break;

      case MenuSelect::MAIN_MENU_DN:
        encoderReturnValue--;
        if (encoderReturnValue < 0) {
          encoderReturnValue = numberOfChoices - 1;
        }
        break;

      default:
        encoderReturnValue = -1; // An error selection
        break;
      }
      if (encoderReturnValue != -1) {
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH, CHAR_HEIGHT, RA8875_GREEN); // Show the option in the second field
        tft.setTextColor(RA8875_BLACK);
        tft.setCursor(SECONDARY_MENU_X + 1, MENUS_Y + 1);
        tft.print(options[encoderReturnValue].c_str());
        refreshFlag = 0;
      }
    }
  }
}

/*****
  Purpose: To select an option from a submenu

  Parameter list:
    string options[]           submenus
    int numberOfChoices       choices available
    int defaultState          the starting option

  Return value
    int           an index into the band array
*****/
int SubmenuSelectString(std::string options[], int numberOfChoices, int defaultStart) {
  int refreshFlag = 0;
  MenuSelect menu;
  int encoderReturnValue;

  tft.setTextColor(RA8875_BLACK);
  encoderReturnValue = defaultStart; // Start the options using this option

  tft.setFontScale((enum RA8875tsize)1);
  if (refreshFlag == 0) {
    tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH, CHAR_HEIGHT,
                 RA8875_GREEN); // Show the option in the second field
    tft.setCursor(SECONDARY_MENU_X + 1, MENUS_Y + 1);
    tft.print(options[encoderReturnValue].c_str()); // Secondary Menu
    refreshFlag = 1;
  }
  //  delay(150L);

  while (true) {
    menu = readButton();                      // Read the ladder value
    if (menu != MenuSelect::BOGUS_PIN_READ) { // Valid choice?
      switch (menu) {
      case MenuSelect::MENU_OPTION_SELECT: // They made a choice
        tft.setTextColor(RA8875_WHITE);
        EraseMenus();
        return encoderReturnValue;
        break;

      case MenuSelect::MAIN_MENU_UP:
        encoderReturnValue++;
        if (encoderReturnValue >= numberOfChoices) {
          encoderReturnValue = 0;
        }
        break;

      case MenuSelect::MAIN_MENU_DN:
        encoderReturnValue--;
        if (encoderReturnValue < 0) {
          encoderReturnValue = numberOfChoices - 1;
        }
        break;

      default:
        encoderReturnValue = -1; // An error selection
        break;
      }
      if (encoderReturnValue != -1) {
        // Show the option in the second field
        tft.fillRect(SECONDARY_MENU_X, MENUS_Y, EACH_MENU_WIDTH, CHAR_HEIGHT, RA8875_GREEN);
        tft.setTextColor(RA8875_BLACK);
        tft.setCursor(SECONDARY_MENU_X + 1, MENUS_Y + 1);
        tft.print(options[encoderReturnValue].c_str());
        refreshFlag = 0;
      }
    }
  }
}
