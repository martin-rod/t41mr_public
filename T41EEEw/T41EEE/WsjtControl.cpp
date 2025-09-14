// based on KN6ZDE work - https://github.com/tmr4/T41_Vxx, https://github.com/tmr4/T41_SDR

// PJRC Using USB Serial Communication
// https://www.pjrc.com/teensy/td_serial.html

// Reference:
// https://www.kenwood.com/i/products/info/amateur/ts_480/pdf/ts_480_pc.pdf


#include "WsjtControl.h"

#include <charconv>

#include "Button.h"
#include "ConfigurationData.h"
#include "Display.h"
#include "T41EEE.h"
#include "Tune.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME WSJT

#include "trace.h"

void TraceCurrentBand(void) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_TRACE, "ConfigData.currentBand:%d", ConfigData.currentBand);
  TRACE_T41(TR_L_TRACE, "radioState:%d", radioState);
  TRACE_T41(TR_L_TRACE, "mode:%d", bands.bands[ConfigData.currentBand].mode);
  TRACE_T41(TR_L_TRACE, "sideband:%d", bands.bands[ConfigData.currentBand].sideband);
  TRACE_T41(TR_L_TRACE, "FHiCut:%d", bands.bands[ConfigData.currentBand].FHiCut);
  TRACE_T41(TR_L_TRACE, "FLoCut:%d", bands.bands[ConfigData.currentBand].FLoCut);
  TRACE_T41(TR_L_TRACE, "FAMCut:%d", bands.bands[ConfigData.currentBand].FAMCut);
}

bool GetTxRxFreq(uint32_t &value) {
  value = TxRxFreq;
  return true;
}

bool GetTransmitReceiveStatus(int &value) {
  if (radioState == RadioState::SSB_TRANSMIT_STATE or radioState == RadioState::FT8_TRANSMIT_STATE or
      radioState == RadioState::CW_TRANSMIT_STRAIGHT_STATE or radioState == RadioState::CW_TRANSMIT_KEYER_STATE) {
    // TX
    value = 1;
  } else {
    // RX
    value = 0;
  }
  return true;
}

bool GetVFOStatus(int &value) {
  switch (ConfigData.activeVFO) {
  case VfoState::VFO_A:
    value = 0;
    break;
  case VfoState::VFO_B:
    value = 1;
    break;
  case VfoState::VFO_SPLIT:
    value = 3;
    break;
  }
  return true;
}

bool ChangeBandByOne(int direction) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "ChangeBandByOne direction:%d", direction);
  return false;
}

bool RedrawBandFrequency() {
  DrawBandWidthIndicatorBar();
  BandInformation();
  ShowFrequency();
  return true;
}

bool ChangeBandFrequencyVFO(uint32_t frequency, char name) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "ChangeBandFrequencyVFO frequency:%u name:%d", frequency, name);

  int newBand = 0;
  if (not FindBandForFrequency(frequency, newBand)) {
    TRACE_T41(TR_L_ERROR, "not found frequency:%u", frequency);
    return false;
  }
  TRACE_T41(TR_L_INFO, "found frequency:%u band:%d", frequency, newBand);

  bool changedBand = false;
  if (name == 'A') {
    if (ConfigData.currentBandA != newBand) {
      changedBand = true;
      TRACE_T41(TR_L_INFO, "changed band:%d", newBand);
      ConfigData.currentBandA = newBand;
      ConfigData.currentBand = newBand;
      SetBandRelay();
    }
  } else if (name == 'B') {
    if (ConfigData.currentBandB != newBand) {
      changedBand = true;
      TRACE_T41(TR_L_INFO, "changed band:%d", newBand);
      ConfigData.currentBandB = newBand;
      ConfigData.currentBand = newBand;
      SetBandRelay();
    }
  } else {
    TRACE_T41(TR_L_ERROR, "bad VFO name:%d", name);
    return false;
  }

  bool changedFreq = false;
  if (name == 'A') {
    if (ConfigData.currentFreqA != frequency) {
      changedFreq = true;
      TRACE_T41(TR_L_INFO, "changed frequency:%u", frequency);
      ConfigData.currentFreqA = frequency;
      SetBandRelay();
    }
  } else if (name == 'B') {
    if (ConfigData.currentFreqB != frequency) {
      changedFreq = true;
      TRACE_T41(TR_L_INFO, "changed frequency:%u", frequency);
      ConfigData.currentFreqB = frequency;
      SetBandRelay();
    }
  } else {
    TRACE_T41(TR_L_ERROR, "bad VFO name:%d", name);
    return false;
  }

  bool changedTxRxFreq = false;
  if (TxRxFreq != frequency) {
    changedTxRxFreq = true;
    TRACE_T41(TR_L_INFO, "changedTxRxFreq frequency:%u", frequency);
    TxRxFreq = frequency;
    ConfigData.centerFreq = frequency;
    NCOFreq = 0L;
    SetFreq();
  }

  if (changedBand or changedFreq or changedTxRxFreq) {
    RedrawBandFrequency();
  }

  return true;
}

bool ChangeFreqIncrement(int32_t frequencyStep) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "ChangeFreqIncrement frequencyStep:%d", frequencyStep);
  return false;
}

bool ChangeFtIncrement(int32_t frequencyStep) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "ChangeFtIncrement frequencyStep:%d", frequencyStep);
  return false;
}

bool VFOSelect(int32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "VFOSelect value:%d", value);
  return false;
}

bool SetFtActive(int32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "SetFtActive value:%d", value);
  return false;
}

bool UpdateAGC(int32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "UpdateAGC value:%d", value);
  return false;
}

bool SetNoiseFloor(int32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "SetNoiseFloor value:%d", value);
  return false;
}

bool GetNoiseFloor(int32_t &value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "GetNoiseFloor value:%d", value);
  return false;
}

bool SetNoiseNeco(int32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "SetNoiseNeco value:%d", value);
  return false;
}

bool SetPowerLevel(int32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "SetNoiseFloor value:%d", value);
  return false;
}

bool SetFilterBandWidth(uint32_t value) {
  TRACE_LEVEL(TR_L_TRACE);
  TRACE_T41(TR_L_INFO, "SetFilterBandWidth value:%u", value);
  return false;
}

bool GetFilterBandWidth(uint32_t &value) {
  TRACE_LEVEL(TR_L_TRACE);
  TraceCurrentBand();
  bool found = false;
  int32_t width = 0;
  switch (bands.bands[ConfigData.currentBand].sideband) {
  case Sideband::LOWER:
  case Sideband::UPPER:
    width = bands.bands[ConfigData.currentBand].FHiCut;
    found = true;
    break;
  case Sideband::BOTH_AM:
  case Sideband::BOTH_SAM:
    width = bands.bands[ConfigData.currentBand].FAMCut;
    found = true;
    break;
  default:
    found = false;
  }

  if (not found) {
    TRACE_T41(TR_L_ERROR, "not found");
    return false;
  }

  if (width <= 0) {
    TRACE_T41(TR_L_ERROR, "width:%d", width);
    return false;
  }

  value = width;
  TRACE_T41(TR_L_INFO, "width:%u", value);
  return true;
}

// Kenwood Band
int GetKenwoodBand() {
  int band;
  switch (ConfigData.currentBand) {
  case BandEnum::BAND_80M:
    band = 1;
    break;
  case BandEnum::BAND_40M:
    band = 2;
    break;
  case BandEnum::BAND_20M:
    band = 4;
    break;
  case BandEnum::BAND_17M:
    band = 5;
    break;
  case BandEnum::BAND_15M:
    band = 6;
    break;
  case BandEnum::BAND_12M:
    band = 7;
    break;
  case BandEnum::BAND_10M:
    band = 8;
    break;
  default:
    band = 2; // 40m
    break;
  }
  return band;
}

// Kenwood TS-480 operating modes
bool GetKenwoodMode(int16_t &modeKenwwod) {
  bool found = false;
  // 1: LSB, 2: USB, 3: CW, 4: FM, 5: AM, 6: FSK, 7:CW reverse, 8:Tune, 9: FSK reverse
  switch (bands.bands[ConfigData.currentBand].mode) {
  case RadioMode::CW_MODE:
    modeKenwwod = 3;
    found = true;
    break;
  case RadioMode::SSB_MODE:
  case RadioMode::FT8_MODE:
  case RadioMode::AM_MODE:
  case RadioMode::SAM_MODE:
    switch (bands.bands[ConfigData.currentBand].sideband) {
    case Sideband::UPPER:
      modeKenwwod = 2; // USB
      found = true;
      break;
    case Sideband::LOWER:
      modeKenwwod = 1; // LSB
      found = true;
      break;
    case Sideband::BOTH_AM:
    case Sideband::BOTH_SAM:
      modeKenwwod = 5; // AM
      found = true;
      break;
    }
    break;
  }
  return found;
}

bool KenwoodModeToT41Mode(int modeKenwwod, RadioMode &radioMode, Sideband &sideBand, RadioState &radioState) {
  bool found = false;
  switch (modeKenwwod) {
  case 1: // LSB
    radioState = RadioState::SSB_RECEIVE_STATE;
    radioMode = RadioMode::SSB_MODE;
    sideBand = Sideband::LOWER;
    found = true;
    break;
  case 2: // USB
    radioState = RadioState::SSB_RECEIVE_STATE;
    radioMode = RadioMode::SSB_MODE;
    sideBand = Sideband::UPPER;
    found = true;
    break;
  case 3: // CW
    radioState = RadioState::CW_RECEIVE_STATE;
    radioMode = RadioMode::CW_MODE;
    // not correct
    sideBand = Sideband::LOWER;
    found = true;
    break;
  case 5: // AM
    radioState = RadioState::AM_RECEIVE_STATE;
    radioMode = RadioMode::AM_MODE;
    sideBand = Sideband::BOTH_AM;
    found = true;
    break;
  default:
    found = false;
    break;
  }
  return found;
}

bool SetMode(RadioMode radioMode, Sideband sideBand, RadioState radioStateLocal) {
  radioState = radioStateLocal;
  bands.bands[ConfigData.currentBand].mode = radioMode;
  bands.bands[ConfigData.currentBand].sideband = sideBand;

  SetAudioOperatingState(radioState);
  button.ExecuteModeChange();

  return true;
}

bool SetModeFromKenwood(int modeKenwwod) {
  RadioState localRadioState;
  RadioMode radioMode;
  Sideband sideBand;
  bool translatedOK = KenwoodModeToT41Mode(modeKenwwod, radioMode, sideBand, localRadioState);
  if (translatedOK) {
    SetMode(radioMode, sideBand, localRadioState);
    return true;
  }

  return false;
}

// ---

void WSJTControlSetup() {
  SerialUSB1.begin(115200);
  size_t i = 0;
  while (SerialUSB1.available() && i < 1024) {
    SerialUSB1.read();
    i++;
  }
}

int WSJTControlSendCmd(char *cmd, size_t cmdLength) {
  TRACE_LEVEL(TR_L_DEBUG);

  TRACE_T41(TR_L_INFO, "TX cmd:'%s' size:%u", cmd, cmdLength);

  if (cmdLength == 0) {
    return 0;
  }

  int available = SerialUSB1.availableForWrite();
  if (available < (int)cmdLength) {
    return -1;
  }

  size_t result = SerialUSB1.write(cmd, cmdLength);
  SerialUSB1.send_now();
  return result;
}

size_t WSJTControlGetCommand(char *cmdRequest, size_t cmdRequestSize) {
  TRACE_LEVEL(TR_L_DEBUG);
  size_t i = 0;

  while (SerialUSB1.available()) {
    int byte = SerialUSB1.read();
    if (byte <= 0) {
      TRACE_T41(TR_L_ERROR, "read: %d <=0");
      continue;
    }

    if (byte > 127) {
      TRACE_T41(TR_L_ERROR, "read: %d >127", byte);
      continue;
    }

    if (not(byte == ';' or std::isdigit(byte) or std::isupper(byte))) {
      TRACE_T41(TR_L_ERROR, "read: %d bad character");
      continue;
    }

    cmdRequest[i] = (char)byte;

    if (i == (cmdRequestSize - 1)) {
      TRACE_T41(TR_L_ERROR, "read: %u end of buffer", i);
      cmdRequest[i + 1] = 0;
      break;
    }

    if (cmdRequest[i] == ';') {
      TRACE_T41(TR_L_TRACE, "cmd end ;");
      i++;
      cmdRequest[i] = 0;
      break;
    }

    i++;
  }
  return i;
}

void WSJTLoop() {
  TRACE_LEVEL(TR_L_DEBUG);

  static bool firstReceivedID[BandEnum::NUMBER_OF_BANDS] = {false};

  if (not SerialUSB1.available()) {
    TRACE_T41(TR_L_TRACE, "RX not available");
    return;
  }

  char cmdRequest[64];
  constexpr size_t cmdRequestSize = sizeof(cmdRequest);

  size_t cmdRequestLength = WSJTControlGetCommand(cmdRequest, cmdRequestSize);
  if (cmdRequestLength == 0) {
    TRACE_T41(TR_L_TRACE, "cmdLength==0");
    return;
  }

  TRACE_T41(TR_L_INFO, "RX cmd:'%s' cmdLength:%u", cmdRequest, cmdRequestLength);

  if (cmdRequestLength == 1 && cmdRequest[0] == ';') {
    TRACE_T41(TR_L_WARN, "RX only ';'");
    return;
  }

  if (cmdRequestLength < 3) {
    TRACE_T41(TR_L_WARN, "RX short cmd");
    return;
  }

  if (cmdRequest[cmdRequestLength - 1] != ';') {
    TRACE_T41(TR_L_INFO, "RX not end ';'");
    return;
  }

  char cmdResponse[64] = {0};
  constexpr size_t cmdResponseSize = sizeof(cmdResponse);

  switch (cmdRequest[0]) {

  case 'A':
    // AI - Sets or reads the Auto Information (AI) function ON/ OFF
    if (cmdRequestLength == 3 && cmdRequest[1] == 'I' && cmdRequest[2] == ';') {
      // Auto info off
      snprintf(cmdResponse, cmdResponseSize, "AI0;");
    } else if (cmdRequest[1] == 'I' && cmdRequest[3] == ';') {
      return;
    }
    break;

  case 'B':
    if (cmdRequest[1] == 'U' && cmdRequest[2] == ';') {
      // BU
      ChangeBandByOne(1);
      return;
    } else if (cmdRequest[1] == 'D' && cmdRequest[2] == ';') {
      // BD
      ChangeBandByOne(-1);
      return;
    } else if (cmdRequest[1] == 'U' && cmdRequest[3] == ';') {
      if (atoi(&cmdRequest[2]) == 0) {
        snprintf(cmdResponse, cmdResponseSize, "BU0%d;", GetKenwoodBand());
      } else {
        snprintf(cmdResponse, cmdResponseSize, "BU1%d;", GetKenwoodBand());
      }
    } else if (cmdRequest[1] == 'D' && cmdRequest[3] == ';') {
      if (atoi(&cmdRequest[2]) == 0) {
        snprintf(cmdResponse, cmdResponseSize, "BD0%d;", GetKenwoodBand());
      } else {
        snprintf(cmdResponse, cmdResponseSize, "BD1%d;", GetKenwoodBand());
      }
    }
    break;

  case 'F':
    switch (cmdRequest[1]) {
    // FA, FB - Reads and sets the VFO A / VFO B frequency.
    case 'A':
    case 'B':
      if (cmdRequestLength == 14 and cmdRequest[13] == ';') {
        uint32_t frequency = 0;
        auto [ptr, ec] = std::from_chars(&cmdRequest[2], &cmdRequest[13], frequency);
        if (ec != std::errc()) {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad conversion", cmdRequest);
          snprintf(cmdResponse, cmdResponseSize, "O;");
          break;
        }
        if (ptr != &cmdRequest[13]) {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad conversion length", cmdRequest);
          snprintf(cmdResponse, cmdResponseSize, "O;");
          break;
        }
        bool success = false;
        if (cmdRequest[1] == 'A') {
          success = ChangeBandFrequencyVFO(frequency, 'A');
        } else if (cmdRequest[1] == 'B') {
          success = ChangeBandFrequencyVFO(frequency, 'B');
        } else {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad value", cmdRequest);
          snprintf(cmdResponse, cmdResponseSize, "O;");
          break;
        }
        if (success) {
          return;
        } else {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s cmd err", cmdRequest);
          // snprintf(cmdResponse, cmdResponseSize, "?;");
          // snprintf(cmdResponse, cmdResponseSize, "E;");

          // WSJTX ignore errors ?
          snprintf(cmdResponse, cmdResponseSize, "O;");
        }
      } else if (cmdRequestLength == 3 and cmdRequest[2] == ';') {
        // read VFO frequency
        if (cmdRequest[1] == 'A') {
          snprintf(cmdResponse, cmdResponseSize, "FA%011u;", ConfigData.currentFreqA);
        } else if (cmdRequest[1] == 'B') {
          snprintf(cmdResponse, cmdResponseSize, "FB%011u;", ConfigData.currentFreqB);
        } else {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad value", cmdRequest);
          snprintf(cmdResponse, cmdResponseSize, "O;");
        }
      }
      break;
    // FR
    case 'R':
      if (cmdRequestLength == 3 and cmdRequest[2] == ';') {
        snprintf(cmdResponse, cmdResponseSize, "FR0;"); // receive on VFO A
      } else if (cmdRequest[3] == ';') {
        // select VFO
        VFOSelect(atoi(&cmdRequest[2]));
        return;
      }
      break;
    // FS
    case 'S':
      if (cmdRequestLength == 4 and cmdRequest[3] == ';') {
        // fine tune on or off
        SetFtActive(atoi(&cmdRequest[2]));
        return;
      }
      break;
    // FT
    case 'T':
      if (cmdRequestLength == 3 and cmdRequest[2] == ';') {
        // transmit on VFO A
        snprintf(cmdResponse, cmdResponseSize, "FT0;");
      } else if (cmdRequest[3] == ';') {
        // select VFO
        VFOSelect(atoi(&cmdRequest[2]));
        return;
      }
      break;
    // FW - Selects or reads the DSP filtering
    // bandwidth 0000 ~ 9999 (in Hz)
    case 'W':
      if (cmdRequestLength == 7 && cmdRequest[6] == ';') {
        uint32_t value = 0;
        auto [ptr, ec] = std::from_chars(&cmdRequest[2], &cmdRequest[13], value);
        if (ec != std::errc()) {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad conversion", cmdRequest);
          break;
        }
        if (ptr != &cmdRequest[13]) {
          TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad conversion length", cmdRequest);
          break;
        }
        SetFilterBandWidth(value);
        return;
      } else if (cmdRequestLength == 3 and cmdRequest[2] == ';') {
        uint32_t value = 0;
        GetFilterBandWidth(value);
        snprintf(cmdResponse, cmdResponseSize, "FW%04lu;", value);
      }
      break;
    default:
      TRACE_T41(TR_L_ERROR, "cmdRequest:%s cmd not executed", cmdRequest);
      snprintf(cmdResponse, cmdResponseSize, "?;");
      return;
      break;
    }
    break;

  case 'G':
    if (cmdRequest[1] == 'T' && cmdRequest[3] == ';') {
      // update AGC
      int32_t AGCMode = atol(&cmdRequest[2]);
      UpdateAGC(AGCMode);
      // AGCMode = atol(&cmd[2]);
      // UpdateInfoBoxItem(IB_ITEM_AGC);
    }
    return;
    break;

  case 'I':
    // ID
    if (cmdRequestLength == 3 and cmdRequest[1] == 'D' and cmdRequest[2] == ';') {
      if (not firstReceivedID[ConfigData.currentBand]) {
        firstReceivedID[ConfigData.currentBand] = true;
        TRACE_T41(TR_L_DEBUG, "init for WSJTX - firstReceivedID[%d]", ConfigData.currentBand);
        SetMode(RadioMode::FT8_MODE, Sideband::UPPER, RadioState::FT8_RECEIVE_STATE);
      }
      // TS-480
      snprintf(cmdResponse, cmdResponseSize, "ID020;");
    } else if (cmdRequestLength == 3 and cmdRequest[1] == 'F' && cmdRequest[2] == ';') {
      // IF; - retrieves transceiver status
      // WSJT-X recieved w/ USB Serial+Audio:
      // IF00007048000125004+0000000001000361100007030000; which is 48, expects 37
      //                            1         2         3      |  4
      //                  0123456789012345678901234567890123456789012345678
      //                  IF00007048000125004+0000000001000361100007030000;
      // should be        IF000070480005000+00000000001xx000000;
      //
      // should have sent per below:
      //                            1         2         3      |  4
      //                  0123456789012345678901234567890123456789012345678
      //                  IF000070480005000+00000000001xx000000;
      //                    01234567890
      // TxRxFreq %011d     00007048000
      // 5000     %04d                 5000
      // 0        %+06d                    +00000
      // 0        %d                             0
      // 0        %d                              0
      // 0        %d                               0
      // 0        %02d                              00
      // XRState  %d                                  1
      // mode     %d                                   x
      // aVFO     %d                                    x
      // 0        %d                                     0
      // 0        %d                                      0
      // 0        %d                                       0
      // 0        %02d                                      00
      // 0        %d                                          0
      // ;                                                     ;
      //                  IF000070480005000+00000000001xx000000;
      TRACE_T41(TR_L_DEBUG, "cmd proc IF");
      uint32_t frequency = 0;
      GetTxRxFreq(frequency);
      int txRx = 0;
      GetTransmitReceiveStatus(txRx);
      int activeVFO = 0;
      GetVFOStatus(activeVFO);
      int16_t mode = 0;
      GetKenwoodMode(mode);
      snprintf(cmdResponse, cmdResponseSize, "IF%011lu%04d%+06d%d%d%d%02d%d%d%d%d%d%d%02d%d;",
               frequency, // freq in Hz
               5000,      // freq step size
               0,         // RIT/XIT freq in Hz, +-99999, this isn't preserved in the T41 but would be VFO A - VFO B if split
               0,         // RIT on/off
               0,         // XIT on/off
               0, 0,      // channel bank number
               txRx,      // RX/TX (1/0)
               mode,      // operating mode
               activeVFO, // RX VFO
               0,         // scan Status
               0,         // split status (Kenwood manual refers to SP command which doesn't exist)
               0,         // CTCSS enabled
               1,         // CTCSS tone frequency
               0          // shift status
      );
    }
    break;
  case 'K': //
    if (cmdRequestLength == 3 && cmdRequest[1] == 'S' && cmdRequest[2] == ';') {
      snprintf(cmdResponse, cmdResponseSize, "KS0%d;", DEFAULT_KEYER_WPM);
    }
    break;

  case 'M':
    // MD - Recalls or reads the operating mode status
    if (cmdRequestLength == 3 && cmdRequest[1] == 'D' && cmdRequest[2] == ';') {
      // read mode status
      int16_t modeKenwwod = 0;
      bool translateOK = GetKenwoodMode(modeKenwwod);
      if (translateOK) {
        snprintf(cmdResponse, cmdResponseSize, "MD%01d;", modeKenwwod);
      }
    } else if (cmdRequestLength == 4 && cmdRequest[1] == 'D' && cmdRequest[3] == ';') {
      // set demod mode status
      uint32_t modeKenwwod = 0;
      auto [ptr, ec] = std::from_chars(&cmdRequest[2], &cmdRequest[3], modeKenwwod);
      if (ec != std::errc()) {
        TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad conversion", cmdRequest);
        break;
      }
      if (ptr != &cmdRequest[3]) {
        TRACE_T41(TR_L_ERROR, "cmdRequest:%s bad conversion length", cmdRequest);
        break;
      }
      SetModeFromKenwood(modeKenwwod);
      return;
    }
    break;

  case 'N':
    if (cmdRequest[1] == 'F' && cmdRequest[2] == ';') {
      // send noise floor
      int32_t value = 0;
      GetNoiseFloor(value);
#if 0
        sprintf(cmd, "NF%04d;", currentNoiseFloor[currentBand]);
#endif
    } else if (cmdRequest[1] == 'F' && cmdRequest[6] == ';') {
      // set noise floor
#if 0
        currentNoiseFloor[currentBand] = atoi(&cmd[2]);
#endif
      int32_t value = atoi(&cmdRequest[2]);
      SetNoiseFloor(value);
      return;
    } else if (cmdRequest[1] == 'G' && cmdRequest[3] == ';') {
      int32_t value = atoi(&cmdRequest[2]);
      SetNoiseNeco(value);
#if 0
        // *** TODO: consider just toggling this through call to
        liveNoiseFloorFlag = atoi(&cmd[2]);

        // save final noise floor setting if toggling flag off
        if (liveNoiseFloorFlag == 0) {
          EEPROMData.currentNoiseFloor[currentBand] = currentNoiseFloor[currentBand];
          EEPROMWrite();
        }
        UpdateInfoBoxItem(IB_ITEM_FLOOR);
#endif
    }
    break;

  case 'O': // PCxxx;
    if (cmdRequest[1] == 'M' && cmdRequest[3] == ';') {
      // operating demod mode
      int16_t mode = 0;
      GetKenwoodMode(mode);
      int item = cmdRequest[2];
      snprintf(cmdResponse, cmdResponseSize, "OM%d%d;", item, mode);
    } else if (cmdRequest[1] == 'M' && cmdRequest[4] == ';') {
      // set demod mode status
      char val[2] = {cmdRequest[2], 0};
      // int item = atoi(val);
      val[0] = cmdRequest[3];
      int mode = atoi(val);
      RadioMode radioMode;
      Sideband sideBan;
      RadioState radioState;
      bool found = KenwoodModeToT41Mode(mode, radioMode, sideBan, radioState);
      if (found) {
        if (radioMode == RadioMode::CW_MODE) {

        } else {
          // ChangeDemodMode(demod);
        }
      }
      return;
    }
    break;

  case 'P': // PCxxx;
    if (cmdRequest[1] == 'C' && cmdRequest[5] == ';') {
      // set transmitter power level
      int32_t value = atoi(&cmdRequest[2]);
      SetPowerLevel(value);
#if 0
        transmitPowerLevel = atoi(&cmd[2]);
      ShowCurrentPowerSetting();
#endif
      return;
    } else if (cmdRequest[1] == 'S' && cmdRequest[2] == ';') {
      // sprintf(cmd,"PS0;"); // 0=Off, 1=On
      snprintf(cmdResponse, cmdResponseSize, "PS1;");
    }
    break;

  case 'R': //
    // RX
    if (cmdRequestLength == 3 && cmdRequest[1] == 'X' && cmdRequest[2] == ';') {
      SetMode(RadioMode::FT8_MODE, Sideband::UPPER, RadioState::FT8_RECEIVE_STATE);
      return;
    }
    break;

  case 'T':
    // TX
    if (cmdRequestLength == 3 && cmdRequest[1] == 'X' && cmdRequest[2] == ';') {
      SetMode(RadioMode::FT8_MODE, Sideband::UPPER, RadioState::FT8_TRANSMIT_STATE);
      return;
    }
    break;

  default:
    TRACE_T41(TR_L_ERROR, "cmdRequest:%s cmd not executed", cmdRequest);
    snprintf(cmdResponse, cmdResponseSize, "?;");
    return;
    break;
  }

  size_t cmdResponseLength = strnlen(cmdResponse, cmdResponseSize);
  int res = WSJTControlSendCmd(cmdResponse, cmdResponseLength);
  if (res == 0) {
    TRACE_T41(TR_L_WARN, "TX cmd empty");
  } else if (res < 0) {
    TRACE_T41(TR_L_ERROR, "TX cmd error");
  }
}

/*
List devices:

$ aplay -L
...
hw:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 Direct hardware device without any conversions
plughw:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 Hardware device with all software conversions
sysdefault:CARD=Audio
 Dual S. Audio, USB Audio
 Default Audio Device
front:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 Front output / input
surround21:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 2.1 Surround output to Front and Subwoofer speakers
surround40:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 4.0 Surround output to Front and Rear speakers
surround41:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 4.1 Surround output to Front, Rear and Subwoofer speakers
surround50:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 5.0 Surround output to Front, Center and Rear speakers
surround51:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 5.1 Surround output to Front, Center, Rear and Subwoofer speakers
surround71:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 7.1 Surround output to Front, Center, Side, Rear and Woofer speakers
iec958:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 IEC958 (S/PDIF) Digital Audio Output
dmix:CARD=Audio,DEV=0
 Dual S. Audio, USB Audio
 Direct sample mixing device
usbstream:CARD=Audio
 Dual S. Audio
 USB Stream Output

$ aplay -l
...
**** List of PLAYBACK Hardware Devices ****
card 0: HDMI [HDA Intel HDMI], device 3: HDMI 0 [B24W-6 LED]
Subdevices: 1/1
Subdevice #0: subdevice #0
card 0: HDMI [HDA Intel HDMI], device 7: HDMI 1 [HDMI 1]
Subdevices: 1/1
Subdevice #0: subdevice #0
card 0: HDMI [HDA Intel HDMI], device 8: HDMI 2 [HDMI 2]
Subdevices: 1/1
Subdevice #0: subdevice #0
card 1: PCH [HDA Intel PCH], device 0: ALC3232 Analog [ALC3232 Analog]
Subdevices: 1/1
Subdevice #0: subdevice #0
card 2: Audio [Dual S. Audio], device 0: USB Audio [USB Audio]
Subdevices: 1/1
Subdevice #0: subdevice #0

---
$ arecord -L
...
hw:CARD=Audio,DEV=0
    Dual S. Audio, USB Audio
    Direct hardware device without any conversions
plughw:CARD=Audio,DEV=0
    Dual S. Audio, USB Audio
    Hardware device with all software conversions
sysdefault:CARD=Audio
    Dual S. Audio, USB Audio
    Default Audio Device
front:CARD=Audio,DEV=0
    Dual S. Audio, USB Audio
    Front output / input
dsnoop:CARD=Audio,DEV=0
    Dual S. Audio, USB Audio
    Direct sample snooping device
usbstream:CARD=Audio
    Dual S. Audio
    USB Stream Output


arecord -l
**** List of CAPTURE Hardware Devices ****
card 1: PCH [HDA Intel PCH], device 0: ALC3232 Analog [ALC3232 Analog]
Subdevices: 1/1
Subdevice #0: subdevice #0
card 2: Audio [Dual S. Audio], device 0: USB Audio [USB Audio]
Subdevices: 1/1
Subdevice #0: subdevice #0

===

Record / play on PC
-------------------

recording output 'Dual S. Audio' from T41 on the input PC:

to stdout:
arecord -D hw:CARD=Audio,DEV=0 -v -f cd -
arecord -D hw:2,0 -v -f cd -

to file:
arecord -D hw:2,0 -v -f cd audio.wav
aplay audio.wav

arecord -D hw:CARD=Audio,DEV=0 -v -f cd audio.wav
aplay audio.wav


to audio:
arecord -D hw:CARD=Audio,DEV=0  -v -f cd - | aplay

---

recording input 'Dual S. Audio' to T41 on the output PC:

# modprobe snd_aloop
...

?
 */
