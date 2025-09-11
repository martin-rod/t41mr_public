#pragma once

#include "Arduino.h"

// Library include files
#include <Adafruit_GFX.h> // Install the Adafruit GFX library via the IDE Library Manager.
#include <ArduinoJson.h>  // Install the ArduinoJson library via the IDE Library Manager.
#include <Bounce.h>
#include <EEPROM.h>
#include <Metro.h>
#include <OpenAudio_ArduinoLibrary.h> // Manually installed library.  Please see compilation instructions in the ino or README.
#include <RA8875.h>                   // This library is included with TeensyDuino.
#include <Rotary.h>                   // Manually installed library.  Please see compilation instructions in the ino or README.
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h> // Part of Teensy Time library.
#include <arm_const_structs.h>
#include <arm_math.h>
#include <si5351.h> // Install the Etherkit Si5351 library via the IDE Library Manager.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utility/imxrt_hw.h> // for setting I2S freq, Thanks, FrankB!

#define T41_USB_AUDIO

#define USE_LOG10FAST

// Uncomment this line for QSE2
// #define QSE2

// Uncomment this line if using an external PLL module
// #define PLLMODULE

// Uncommented for debugging, comment out for normal use
// #define DEBUG

// Uncomment to see temperature and load information
// #define DEBUG1

// Uncomment to run switch cal by pushing and holding a button at power-up
// Debug switch cal must be disabled for normal radio operation!
// #define DEBUG_SWITCH_CAL

// Uncomment to get CESSB operating parameters printed to the serial monitor
// #define DEBUG_CESSB

// Uncomment ...
// #define DEBUG_SMETER

enum class HwVersion : int {
  T41_1_AL_JACK = 0,
  T41_1_4SQRP = 1,
  T41_2_GREG = 2,
};

constexpr int EQUALIZER_CELL_COUNT = 14;

enum class VfoState : int {
  VFO_A = 0,
  VFO_B = 1,
  VFO_SPLIT = 2,
};

enum class TimeFormatEnum : int {
  TIME_24H,
  TIME_12H,
};

enum class ItuRegionEnum : int {
  ITU_REGION_1 = 1, // Africa, Europe, Russia, Ukraine ...
  ITU_REGION_2 = 2, // Americas, Greenland, ...
  ITU_REGION_3 = 3, // Asia, Oceania, ...
};

enum class KeyTypeEnum : int {
  STRAIGHT_KEY = 0,
  PADDLES = 1,
};

enum class PaddleFlipEnum : int {
  RIGHT_PADDLE_DAH = 0,
  RIGHT_PADDLE_DIT = 1,
};

constexpr uint16_t MAROON = 0x7800; /* 128,   0,   0 */
constexpr uint16_t DARK_RED = 0x400;
constexpr uint16_t DARKGREY = 0x7BEF; /* 128, 128, 128 */
constexpr uint16_t RED = 0xF800;      /* 255,   0,   0 */
constexpr uint16_t YELLOW = 0xFFE0;   /* 255, 255,   0 */
constexpr uint16_t FILTER_WIN = 0x10; // Color of SSB filter width

// === Pin Assignments ===
// Pins 0 and 1 are usually reserved for the USB COM port communications  On the Teensy 4.1 board, pins GND, 0-12, and pins
// 13-23, 3.3V, GND, and Vin are "covered up" by the Audio board. However, not all of those pins are
// actually used by the board. See: https://www.pjrc.com/store/teensy3_audio.html

// Display pins
constexpr int TFT_DC = 9;
constexpr int TFT_CS = 10;
constexpr int TFT_MOSI = 11;
constexpr int TFT_SCLK = 13;
constexpr int TFT_RST = 255;

// Set multiplication factors for your QSD and QSE boards.
// Default values here and below are for V10/V11 boards.
constexpr int MASTER_CLK_MULT_RX = 4;
constexpr int MASTER_CLK_MULT_TX = 4;

// Encoder pins
extern int VOLUME_ENCODER_A;
extern int VOLUME_ENCODER_B;
extern int FILTER_ENCODER_A;
extern int FILTER_ENCODER_B;
extern int FINETUNE_ENCODER_A;
extern int FINETUNE_ENCODER_B;
extern int TUNE_ENCODER_A;
extern int TUNE_ENCODER_B;

// Filter Board pins
constexpr int FILTERPIN80M = 30; // 80M filter relay
constexpr int FILTERPIN40M = 31; // 40M filter relay
constexpr int FILTERPIN20M = 28; // 20M filter relay
constexpr int FILTERPIN15M = 29; // 15M filter relay
constexpr int RXTX = 22;         // Transmit/Receive
constexpr int PTT = 37;          // Transmit/Receive
constexpr int MUTE = 38; // Mute Audio, HIGH = "On" Audio PA, LOW = Mute Audio PA off.  This may be reversed depending on PA.

// Key/Keyer pins
constexpr int KEYER_DAH_INPUT_RING = 35; // Ring connection for keyer.  Default for righthanded user.
constexpr int KEYER_DIT_INPUT_TIP = 36;  // Tip connection for keyer.  Also straight key.

// Constants

constexpr float PIH = 1.5707963267948966192313216916398f;

enum class SpectrumZoomState : int32_t {
  SPECTRUM_ZOOM_1 = 0,
  SPECTRUM_ZOOM_2 = 1,
  SPECTRUM_ZOOM_4 = 2,
  SPECTRUM_ZOOM_8 = 3,
  SPECTRUM_ZOOM_16 = 4
};

enum class SampleRateEnum : size_t {
  SAMPLE_RATE_8K = 0,
  SAMPLE_RATE_11K = 1,
  SAMPLE_RATE_16K = 2,
  SAMPLE_RATE_22K = 3,
  SAMPLE_RATE_32K = 4,
  SAMPLE_RATE_44K = 5,
  SAMPLE_RATE_48K = 6,
  SAMPLE_RATE_50K = 7,
  SAMPLE_RATE_88K = 8,
  SAMPLE_RATE_96K = 9,
  SAMPLE_RATE_100K = 10,
  SAMPLE_RATE_101K = 11,
  SAMPLE_RATE_176K = 12,
  SAMPLE_RATE_192K = 13,
  SAMPLE_RATE_234K = 14,
  SAMPLE_RATE_256K = 15,
  SAMPLE_RATE_281K = 16, // ??
  SAMPLE_RATE_353K = 17,
};

#define HAM_BAND 1

#define BUFFER_SIZE 128

#define START_MENU 0
#define MENU_RF_GAIN 15
#define MENU_F_LO_CUT 40

#define CW_TEXT_START_X 5
#define CW_TEXT_START_Y 449                  // 480 * 0.97 = 465 - height = 465 - 16 = 449
#define CW_MESSAGE_WIDTH MAX_WATERFALL_WIDTH // 512
#define CW_MESSAGE_HEIGHT 16                 // tft.getFontHeight()

//  States used to control radio function.
enum class RadioState {
  SSB_RECEIVE_STATE,
  SSB_TRANSMIT_STATE,
  FT8_TRANSMIT_STATE,
  FT8_RECEIVE_STATE,
  CW_RECEIVE_STATE,
  CW_TRANSMIT_STRAIGHT_STATE,
  CW_TRANSMIT_KEYER_STATE,
  AM_RECEIVE_STATE,
  SAM_RECEIVE_STATE,
  SSB_CALIBRATE_STATE,
  SSB_IM3TEST_STATE,
  CW_CALIBRATE_STATE,
  SET_CW_SIDETONE,
  NOSTATE
};

enum class RadioMode : int { CW_MODE, SSB_MODE, FT8_MODE, AM_MODE, SAM_MODE };

// Plain enum, because it needs to be iterated in mode change function.
enum class Sideband : int { LOWER, UPPER, BOTH_AM, BOTH_SAM };

// Global CW Filter declarations
#define IIR_CW_NUMSTAGES 4
extern float32_t CW_Filter_Coeffs[];
extern float32_t HP_DC_Filter_Coeffs[];
extern float32_t HP_DC_Filter_Coeffs2[]; // AFP 11-02-22

#define DISPLAY_S_METER_DBM 0
#define DISPLAY_S_METER_DBMHZ 1
#define N2 100

#define MAX_LMS_TAPS 96
#define MAX_LMS_DELAY 256

extern float32_t audioMaxSquaredAve;

// Histogram stuff

// Global Excite Variables

extern float32_t coeffs192K_10K_LPF_FIR[];
extern float32_t coeffs48K_8K_LPF_FIR[];

extern float32_t EQ_Band1Coeffs[];
extern float32_t EQ_Band2Coeffs[];
extern float32_t EQ_Band3Coeffs[];
extern float32_t EQ_Band4Coeffs[];
extern float32_t EQ_Band5Coeffs[];
extern float32_t EQ_Band6Coeffs[];
extern float32_t EQ_Band7Coeffs[];
extern float32_t EQ_Band8Coeffs[];
extern float32_t EQ_Band9Coeffs[];
extern float32_t EQ_Band10Coeffs[];
extern float32_t EQ_Band11Coeffs[];
extern float32_t EQ_Band12Coeffs[];
extern float32_t EQ_Band13Coeffs[];
extern float32_t EQ_Band14Coeffs[];

// Global object declarations

// Teensy and OpenAudio objects.  Revised by KF5N July 24, 2024
extern AudioConnection_F32 patchCord15; // Patch cords 15 and 16 are used to connect/disconnect the I and Q datastreams.
extern AudioConnection patchCord16;

extern AudioAmplifier volumeAdjust;
extern AudioRecordQueue ADC_RX_I;
extern AudioRecordQueue ADC_RX_Q;
extern AudioRecordQueue Q_in_L_Ex;
extern AudioRecordQueue Q_in_R_Ex;
extern AudioPlayQueue Q_out_L;
extern AudioPlayQueue Q_out_L_Ex;
extern AudioPlayQueue Q_out_R_Ex;

extern AudioControlSGTL5000 sgtl5000_1;  // F32 controller for the Teensy Audio Board
extern AudioConvert_I16toF32 int2Float1; // Converts Int16 to Float.  See class in AudioStream_F32.h
extern AudioEffectGain_F32 micGain;      // Added with CESSB.  Greg KF5N July 24, 2024.
extern AudioEffectGain_F32 speakerVolume, headphoneVolume;
extern AudioConvert_F32toI16 float2Int1; // Converts Float to Int16.  See class in AudioStream_F32.h
extern AudioSynthWaveformSine_F32 toneSSBCal1, toneSSBCal2;
extern AudioMixer4_F32 mixer1;
extern AudioEffectCompressor2_F32 compressor1; // Open Audio Compressor 2
extern radioCESSB_Z_transmit_F32 cessb1;
// end Teensy and OpenAudio objects

extern void SetAudioOperatingState(RadioState operatingState); // Configures audio system for requested mode state.

typedef struct SR_Descriptor {
  const SampleRateEnum SR_n;
  const uint32_t rate;
  const char *const text;
} SR_Desc;

extern const struct SR_Descriptor SR[];

extern const arm_cfft_instance_f32 *S;
extern const arm_cfft_instance_f32 *iS;
extern const arm_cfft_instance_f32 *maskS;
extern const arm_cfft_instance_f32 *NR_FFT;
extern const arm_cfft_instance_f32 *NR_iFFT;
extern const arm_cfft_instance_f32 *spec_FFT;

extern arm_biquad_casd_df1_inst_f32 IIR_biquad_Zoom_FFT_I;
extern arm_biquad_casd_df1_inst_f32 IIR_biquad_Zoom_FFT_Q;

extern arm_fir_decimate_instance_f32 FIR_dec1_I;
extern arm_fir_decimate_instance_f32 FIR_dec1_Q;
extern arm_fir_decimate_instance_f32 FIR_dec2_I;
extern arm_fir_decimate_instance_f32 FIR_dec2_Q;
extern arm_fir_decimate_instance_f32 Fir_Zoom_FFT_Decimate_I;
extern arm_fir_decimate_instance_f32 Fir_Zoom_FFT_Decimate_Q;
extern arm_fir_interpolate_instance_f32 FIR_int1_I;
extern arm_fir_interpolate_instance_f32 FIR_int1_Q;
extern arm_fir_interpolate_instance_f32 FIR_int2_I;
extern arm_fir_interpolate_instance_f32 FIR_int2_Q;
extern arm_lms_norm_instance_f32 LMS_Norm_instance;
extern elapsedMicros usec;

typedef struct DEMOD_Descriptor {
  const uint8_t DEMOD_n;
  const char *const text;
} DEMOD_Desc;
extern const DEMOD_Descriptor DEMOD[];

struct dispSc {
  const char *dbText;
  float32_t dBScale;
  uint16_t baseOffset;
};

extern struct dispSc displayScale[];

typedef struct Menu_Descriptor {
  const uint8_t no;        // Menu ID
  const char *const text1; // upper text
  const char *text2;       // lower text
} Menu_D;
extern Menu_D Menus[];

// Global variables declarations

constexpr uint32_t FFT_length = 512;
constexpr size_t filenameSize = 16;
