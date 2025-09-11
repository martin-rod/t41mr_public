#include "T41EEE.h"

#include "AudioSignal.h"
#include "Bearing.h"
#include "Button.h"
#include "CWExciter.h"
#include "CWProcessing.h"
#include "CalibrationData.h"
#include "ConfigurationData.h"
#include "Display.h"
#include "Eeprom.h"
#include "Encoders.h"
#include "FFT.h"
#include "FIR.h"
#include "Filter.h"
#include "MemoryInfo.h"
#include "Noise.h"
#include "Process.h"
#include "SSBCalibrate.h"
#include "SSB_Exciter.h"
#include "Tune.h"
#include "Utility.h"

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME T41EEE

#include "git_version.h"
#include "trace.h"

// Receiver process object.
Process process;
// Instantiate the calibration objects.
CWCalibrate cwcalibrater;
SSBCalibrate ssbcalibrater;

// Rotary encoders
Rotary *volumeEncoder = nullptr;
Rotary *tuneEncoder = nullptr;
Rotary *filterEncoder = nullptr;
Rotary *fineTuneEncoder = nullptr;

// Rotary encoders pin
// Encoder pins
int VOLUME_ENCODER_A = 0;
int VOLUME_ENCODER_B = 0;
int FILTER_ENCODER_A = 0;
int FILTER_ENCODER_B = 0;
int FINETUNE_ENCODER_A = 0;
int FINETUNE_ENCODER_B = 0;
int TUNE_ENCODER_A = 0;
int TUNE_ENCODER_B = 0;

constexpr int T41_1_4SQRP_VOLUME_ENCODER_A = 2;
constexpr int T41_1_4SQRP_VOLUME_ENCODER_B = 3;
constexpr int T41_1_4SQRP_FILTER_ENCODER_A = 16;
constexpr int T41_1_4SQRP_FILTER_ENCODER_B = 15;
constexpr int T41_1_4SQRP_FINETUNE_ENCODER_A = 4;
constexpr int T41_1_4SQRP_FINETUNE_ENCODER_B = 5;
constexpr int T41_1_4SQRP_TUNE_ENCODER_A = 14;
constexpr int T41_1_4SQRP_TUNE_ENCODER_B = 17;

constexpr int T41_1_VOLUME_ENCODER_A = 2;
constexpr int T41_1_VOLUME_ENCODER_B = 3;
constexpr int T41_1_FILTER_ENCODER_A = 15;
constexpr int T41_1_FILTER_ENCODER_B = 14;
constexpr int T41_1_FINETUNE_ENCODER_A = 4;
constexpr int T41_1_FINETUNE_ENCODER_B = 5;
constexpr int T41_1_TUNE_ENCODER_A = 16;
constexpr int T41_1_TUNE_ENCODER_B = 17;

// Instantiate the PLL device.
Si5351 si5351;

RadioState radioState;
RadioState lastState;

// Instantiate the display
#define RA8875_DISPLAY 1
#ifndef RA8875_DISPLAY
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);
#else
RA8875 tft = RA8875(TFT_CS, TFT_DC);
#endif // RA8875_DISPLAY

SPISettings settingsA(70000000UL, MSBFIRST, SPI_MODE1);

// CW decode Filters
arm_fir_instance_f32 FIR_CW_DecodeL;
arm_fir_instance_f32 FIR_CW_DecodeR;
static float32_t DMAMEM FIR_CW_DecodeL_state[64 + 256 - 1];
static float32_t DMAMEM FIR_CW_DecodeR_state[64 + 256 - 1];

// Decimation and Interpolation Filters
arm_fir_interpolate_instance_f32 FIR_int1_EX_I;
arm_fir_interpolate_instance_f32 FIR_int1_EX_Q;
arm_fir_interpolate_instance_f32 FIR_int2_EX_I;
arm_fir_interpolate_instance_f32 FIR_int2_EX_Q;

float32_t audioMaxSquaredAve;

static float32_t DMAMEM FIR_int2_EX_I_state[519];
static float32_t DMAMEM FIR_int2_EX_Q_state[519];
static float32_t DMAMEM FIR_int1_EX_I_state[279];
static float32_t DMAMEM FIR_int1_EX_Q_state[279];

float32_t DMAMEM float_buffer_L_EX[2048];
float32_t DMAMEM float_buffer_R_EX[2048];
float32_t DMAMEM float_buffer_LTemp[2048];
float32_t DMAMEM float_buffer_RTemp[2048];
// === End Excite Variables ===

// === Global structure declarations ===

// SR_n, rate, text
const struct SR_Descriptor SR[18] = {
    // not OK
    {SampleRateEnum::SAMPLE_RATE_8K, 8000, "  8k"},
    // not OK
    {SampleRateEnum::SAMPLE_RATE_11K, 11025, " 11k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_16K, 16000, " 16k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_22K, 22050, " 22k"},
    // OK, one more indicator?
    {SampleRateEnum::SAMPLE_RATE_32K, 32000, " 32k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_44K, 44100, " 44k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_48K, 48000, " 48k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_50K, 50223, " 50k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_88K, 88200, " 88k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_96K, 96000, " 96k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_100K, 100000, "100k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_101K, 100466, "101k"},
    // OK
    {SampleRateEnum::SAMPLE_RATE_176K, 176400, "176k"},
    // OK    THIS IS USED IN THE T41
    {SampleRateEnum::SAMPLE_RATE_192K, 192000, "192k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_234K, 234375, "234k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_256K, 256000, "256k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_281K, 281000, "281k"},
    // NOT OK
    {SampleRateEnum::SAMPLE_RATE_353K, 352800, "353k"},
};

const arm_cfft_instance_f32 *S;
const arm_cfft_instance_f32 *iS;
const arm_cfft_instance_f32 *maskS;
const arm_cfft_instance_f32 *NR_FFT;
const arm_cfft_instance_f32 *NR_iFFT;
const arm_cfft_instance_f32 *spec_FFT;

arm_fir_decimate_instance_f32 FIR_dec1_I;
arm_fir_decimate_instance_f32 FIR_dec1_Q;
arm_fir_decimate_instance_f32 FIR_dec2_I;
arm_fir_decimate_instance_f32 FIR_dec2_Q;
arm_fir_interpolate_instance_f32 FIR_int1_I;
arm_fir_interpolate_instance_f32 FIR_int1_Q;
arm_fir_interpolate_instance_f32 FIR_int2_I;
arm_fir_interpolate_instance_f32 FIR_int2_Q;
arm_lms_norm_instance_f32 LMS_Norm_instance;

// dbText, dBScale, baseOffset
// 1, 2, and 5 dB options removed.  Greg KF5N July 30, 2024.
dispSc displayScale[] = {
    {"20 dB/", 10.0, 24},
    {"10 dB/", 20.0, 10},
};

// === Global variables declarations for Quad Oscillator 2 ===
int32_t NCOFreq = 0;

// === Global variables declarations ===
// === Global CW Correlation and FFT Variables ===

// Was cosBuffer2; Greg KF5N February 7, 2024
float32_t *cosBuffer = new float32_t[256];
// This can't be DMAMEM.  It will cause problems with the CW decoder.
float32_t *sinBuffer = new float32_t[256];

float32_t DMAMEM cwRiseBuffer[256];
float32_t DMAMEM cwFallBuffer[256];

SampleRateEnum SampleRate = SampleRateEnum::SAMPLE_RATE_192K;

// === Histogram stuff ===

unsigned long cwTimer;
unsigned long ditTimerOn;

// === end histogram stuff ===

// Global variables used by audio filter encoder.

int attenuator = 0;
bool calOnFlag = false;

uint32_t currentFreq;
// = ConfigData.centerFreq + NCOFreq  NCOFreq from FreqShift2()
uint32_t TxRxFreq;

/*!< The value of s_roomCount minus s_hotCount.*/
uint32_t s_roomC_hotC;
/*!< The value of TEMPMON_TEMPSENSE0[TEMP_VALUE] at room temperature .*/
uint32_t s_hotTemp;
/*!< The value of TEMPMON_TEMPSENSE0[TEMP_VALUE] at the hot temperature.*/
uint32_t s_hotCount;
/*!< The value of s_hotTemp minus room temperature(25C).*/
float s_hotT_ROOM;

float32_t coefficient_set[5] = {0, 0, 0, 0, 0};

float32_t dbm = -145.0;

float32_t DMAMEM float_buffer_L[BUFFER_SIZE * N_B];
float32_t DMAMEM float_buffer_R[BUFFER_SIZE * N_B];

float32_t DMAMEM iFFT_buffer[FFT_length * 2 + 1];

bool volumeChangeFlag = false;

// Voltage in one-hundred 1 dB steps for volume control.
const float32_t volumeLog[101] = {
    0.000010, 0.000011, 0.000013, 0.000014, 0.000016, 0.000018, 0.000020, 0.000022, 0.000025, 0.000028, 0.000032, 0.000035,
    0.000040, 0.000045, 0.000050, 0.000056, 0.000063, 0.000071, 0.000079, 0.000089, 0.000100, 0.000112, 0.000126, 0.000141,
    0.000158, 0.000178, 0.000200, 0.000224, 0.000251, 0.000282, 0.000316, 0.000355, 0.000398, 0.000447, 0.000501, 0.000562,
    0.000631, 0.000708, 0.000794, 0.000891, 0.001000, 0.001122, 0.001259, 0.001413, 0.001585, 0.001778, 0.001995, 0.002239,
    0.002512, 0.002818, 0.003162, 0.003548, 0.003981, 0.004467, 0.005012, 0.005623, 0.006310, 0.007079, 0.007943, 0.008913,
    0.010000, 0.011220, 0.012589, 0.014125, 0.015849, 0.017783, 0.019953, 0.022387, 0.025119, 0.028184, 0.031623, 0.035481,
    0.039811, 0.044668, 0.050119, 0.056234, 0.063096, 0.070795, 0.079433, 0.089125, 0.100000, 0.112202, 0.125893, 0.141254,
    0.158489, 0.177828, 0.199526, 0.223872, 0.251189, 0.281838, 0.316228, 0.354813, 0.398107, 0.446684, 0.501187, 0.562341,
    0.630957, 0.707946, 0.794328, 0.891251, 1.000000,
};

uint32_t elapsed_micros_idx_t = 0;
uint32_t elapsed_micros_sum;

constexpr uint32_t Si_5351_crystal = 25000000L;

void InitializeTemperatureVars();

/*****
  Purpose: To read the local time

  Parameter list:
    void

  Return value:
    time_t                a time data point
*****/
time_t getTeensy3Time() { return Teensy3Clock.get(); }

// is added in Teensyduino 1.52 beta-4, so this can be deleted !?
/*****
  Purpose: To set the real time clock

  Parameter list:
    unsigned long t

  Return value:
    void
*****/
void FLASHMEM T4_rtc_set(unsigned long t) {
  // #if defined (T4)

#if 0
// stop the RTC
 
  SNVS_HPCR &= ~(SNVS_HPCR_RTC_EN | SNVS_HPCR_HP_TS);
// wait
  while (SNVS_HPCR & SNVS_HPCR_RTC_EN);
// stop the SRTC
 
  SNVS_LPCR &= ~SNVS_LPCR_SRTC_ENV;
// wait
  while (SNVS_LPCR & SNVS_LPCR_SRTC_ENV);
// set the SRTC
 
  SNVS_LPSRTCLR = t << 15;
  SNVS_LPSRTCMR = t >> 17;
// start the SRTC
 
  SNVS_LPCR |= SNVS_LPCR_SRTC_ENV;
// wait
  while (!(SNVS_LPCR & SNVS_LPCR_SRTC_ENV));
// start the RTC and sync it to the SRTC
 
  SNVS_HPCR |= SNVS_HPCR_RTC_EN | SNVS_HPCR_HP_TS;
#endif
}

/*****
  Purpose: void initTempMon

  Parameter list:
    void
  Return value;
    void
*****/
void FLASHMEM initTempMon(uint16_t freq, uint32_t lowAlarmTemp, uint32_t highAlarmTemp, uint32_t panicAlarmTemp) {

#define TMS0_POWER_DOWN_MASK (0x1U)
#define TMS1_MEASURE_FREQ(x) (((uint32_t)(((uint32_t)(x)) << 0U)) & 0xFFFFU)

  uint32_t calibrationData;
  uint32_t roomCount;
  constexpr int TEMPMON_ROOMTEMP = 25;

  // first power on the temperature sensor - no register change
  TEMPMON_TEMPSENSE0 &= ~TMS0_POWER_DOWN_MASK;
  TEMPMON_TEMPSENSE1 = TMS1_MEASURE_FREQ(freq);

  calibrationData = HW_OCOTP_ANA1;
  s_hotTemp = (uint32_t)(calibrationData & 0xFFU) >> 0x00U;
  s_hotCount = (uint32_t)(calibrationData & 0xFFF00U) >> 0X08U;
  roomCount = (uint32_t)(calibrationData & 0xFFF00000U) >> 0x14U;
  s_hotT_ROOM = s_hotTemp - TEMPMON_ROOMTEMP;
  s_roomC_hotC = roomCount - s_hotCount;
}

/*****
  Purpose: Read the Teensy's temperature. Get worried over 50C

  Parameter list:
    void

  Return value:
    float           temperature Centigrade
*****/
float TGetTemp() {
  uint32_t nmeas;
  float tmeas;

  while (!(TEMPMON_TEMPSENSE0 & 0x4U)) {
    ;
  }
  // ready to read temperature code value

  nmeas = (TEMPMON_TEMPSENSE0 & 0xFFF00U) >> 8U;
  // Calculate temperature
  tmeas = s_hotTemp - (float)((nmeas - s_hotCount) * s_hotT_ROOM / s_roomC_hotC);
  return tmeas;
}

/*****
  Purpose: To set the I2S frequency

  Parameter list:
    int freq        the frequency to set

  Return value:
    int             the frequency or 0 if too large
*****/
int SetI2SFreq(int freq) {
  int n1;
  int n2;
  int c0;
  int c2;
  int c1;
  double C;

  // PLL between 27*24 = 648MHz und 54*24=1296MHz
  // Fudge to handle 8kHz - El Supremo

  if (freq > 8000) {
    // SAI prescaler 4 => (n1*n2) = multiple of 4
    n1 = 4;
  } else {
    n1 = 8;
  }
  n2 = 1 + (24000000 * 27) / (freq * 256 * n1);
  if (n2 > 63) {
    // n2 must fit into a 6-bit field
#ifdef DEBUG
    Serial.printf("ERROR: n2 exceeds 63 - %d\n", n2);
#endif
    return 0;
  }
  C = ((double)freq * 256 * n1 * n2) / 24000000;
  c0 = C;
  c2 = 10000;
  c1 = C * c2 - (c0 * c2);
  set_audioClock(c0, c1, c2, true);
  CCM_CS1CDR = (CCM_CS1CDR & ~(CCM_CS1CDR_SAI1_CLK_PRED_MASK | CCM_CS1CDR_SAI1_CLK_PODF_MASK)) | CCM_CS1CDR_SAI1_CLK_PRED(n1 - 1) |
               CCM_CS1CDR_SAI1_CLK_PODF(n2 - 1);

  CCM_CS2CDR = (CCM_CS2CDR & ~(CCM_CS2CDR_SAI2_CLK_PRED_MASK | CCM_CS2CDR_SAI2_CLK_PODF_MASK)) | CCM_CS2CDR_SAI2_CLK_PRED(n1 - 1) |
               CCM_CS2CDR_SAI2_CLK_PODF(n2 - 1);
  return freq;
}

void InitializeTemperatureVars() {
  // updates the temp value at a RTC/3 clock rate 0xFFFF determines a 2 second sample rate period
  uint16_t temp_check_frequency = 0x03U;
  // 42 degrees C
  uint32_t highAlarmTemp = 85U;
  uint32_t lowAlarmTemp = 25U;
  uint32_t panicAlarmTemp = 90U;

  // this starts the measurements
  initTempMon(temp_check_frequency, lowAlarmTemp, highAlarmTemp, panicAlarmTemp);
  TEMPMON_TEMPSENSE0 |= 0x2U;
}

/*****
  Purpose: to collect array inits in one place

  Parameter list:
    void

  Return value:
    void
*****/
FLASHMEM void InitializeDataArrays() {

  InitilizeFFT();

  InitilizeNR();

  // init complex FFTs
  switch (FFT_length) {
  case 2048:
    S = &arm_cfft_sR_f32_len2048;
    iS = &arm_cfft_sR_f32_len2048;
    maskS = &arm_cfft_sR_f32_len2048;
    break;
  case 1024:
    S = &arm_cfft_sR_f32_len1024;
    iS = &arm_cfft_sR_f32_len1024;
    maskS = &arm_cfft_sR_f32_len1024;
    break;
  case 512:
    S = &arm_cfft_sR_f32_len512;
    iS = &arm_cfft_sR_f32_len512;
    maskS = &arm_cfft_sR_f32_len512;
    break;
  }

  // Changed specification to 512 instance
  spec_FFT = &arm_cfft_sR_f32_len512;
  NR_FFT = &arm_cfft_sR_f32_len256;
  NR_iFFT = &arm_cfft_sR_f32_len256;

  // Calculate the FFT of the FIR filter coefficients once to produce the FIR
  // filter mask InitFilterMask();

  // Set sample rate
  SetI2SFreq(SR[static_cast<size_t>(SampleRate)].rate);

  // set number of stages
  biquad_lowpass1.numStages = N_stages_biquad_lowpass1;
  // set pointer to coefficients file
  biquad_lowpass1.pCoeffs = biquad_lowpass1_coeffs;

  // set state variables to zero
  for (unsigned i = 0; i < 4 * N_stages_biquad_lowpass1; i++) {
    biquad_lowpass1_state[i] = 0.0;
  }
  // set pointer to the state variables
  biquad_lowpass1.pState = biquad_lowpass1_state;

  // set filter bandwidth of IIR filter, also adjust IIR AM filter, calculate IIR coeffs
  int LP_F_help = bands.bands[ConfigData.currentBand].FHiCut;
  if (LP_F_help < -bands.bands[ConfigData.currentBand].FLoCut) {
    LP_F_help = -bands.bands[ConfigData.currentBand].FLoCut;
  }

  // 1st stage
  SetIIRCoeffs((float32_t)LP_F_help, 1.3, (float32_t)SR[static_cast<size_t>(SampleRate)].rate / DF, 0);
  // fill coefficients into the right file
  for (int i = 0; i < 5; i++) {
    biquad_lowpass1_coeffs[i] = coefficient_set[i];
  }

  // Initiate decimation and interpolation FIR filters

  // Decimation filter 1, M1 = DF1
  CalcFIRCoeffs(FIR_dec1_coeffs, n_dec1_taps, (float32_t)(n_desired_BW * 1000.0), n_att, 0, 0.0,
                (float32_t)SR[static_cast<size_t>(SampleRate)].rate);

  if (arm_fir_decimate_init_f32(&FIR_dec1_I, n_dec1_taps, (uint32_t)DF1, FIR_dec1_coeffs, FIR_dec1_I_state, BUFFER_SIZE * N_B)) {
    while (1) {
    }
  }

  if (arm_fir_decimate_init_f32(&FIR_dec1_Q, n_dec1_taps, (uint32_t)DF1, FIR_dec1_coeffs, FIR_dec1_Q_state, BUFFER_SIZE * N_B)) {
    while (1) {
    }
  }

  // Decimation filter 2, M2 = DF2
  CalcFIRCoeffs(FIR_dec2_coeffs, n_dec2_taps, (float32_t)(n_desired_BW * 1000.0), n_att, 0, 0.0,
                (float32_t)(SR[static_cast<size_t>(SampleRate)].rate / DF1));
  if (arm_fir_decimate_init_f32(&FIR_dec2_I, n_dec2_taps, (uint32_t)DF2, FIR_dec2_coeffs, FIR_dec2_I_state,
                                BUFFER_SIZE * N_B / (uint32_t)DF1)) {
    while (1) {
    }
  }

  if (arm_fir_decimate_init_f32(&FIR_dec2_Q, n_dec2_taps, (uint32_t)DF2, FIR_dec2_coeffs, FIR_dec2_Q_state,
                                BUFFER_SIZE * N_B / (uint32_t)DF1)) {
    while (1) {
    }
  }

  // Interpolation filter 1, L1 = 2 not sure whether I should design with the final sample rate ??
  // yes, because the interpolation filter is AFTER the upsampling, so it has to be in the target sample rate!

  CalcFIRCoeffs(FIR_int1_coeffs, 48, (float32_t)(n_desired_BW * 1000.0), n_att, 0, 0.0,
                SR[static_cast<size_t>(SampleRate)].rate / 4.0);

  if (arm_fir_interpolate_init_f32(&FIR_int1_I, (uint8_t)DF2, 48, FIR_int1_coeffs, FIR_int1_I_state,
                                   BUFFER_SIZE * N_B / (uint32_t)DF)) {
    while (1) {
    }
  }

  if (arm_fir_interpolate_init_f32(&FIR_int1_Q, (uint8_t)DF2, 48, FIR_int1_coeffs, FIR_int1_Q_state,
                                   BUFFER_SIZE * N_B / (uint32_t)DF)) {
    while (1) {
    }
  }

  // Interpolation filter 2, L2 = 4 not sure whether I should design with the final sample rate ??
  // yes, because the interpolation filter is AFTER the upsampling, so it has to be in the target sample rate!
  CalcFIRCoeffs(FIR_int2_coeffs, 32, (float32_t)(n_desired_BW * 1000.0), n_att, 0, 0.0,
                (float32_t)SR[static_cast<size_t>(SampleRate)].rate);

  if (arm_fir_interpolate_init_f32(&FIR_int2_I, (uint8_t)DF1, 32, FIR_int2_coeffs, FIR_int2_I_state,
                                   BUFFER_SIZE * N_B / (uint32_t)DF1)) {
    while (1) {
    }
  }

  if (arm_fir_interpolate_init_f32(&FIR_int2_Q, (uint8_t)DF1, 32, FIR_int2_coeffs, FIR_int2_Q_state,
                                   BUFFER_SIZE * N_B / (uint32_t)DF1)) {
    while (1) {
    }
  }

  // The correct bandwidths are calculated and set accordingly.
  SetDecIntFilters();

  ZoomFFTPrep();
  SpectralNoiseReductionInit();
  InitLMSNoiseReduction();
}

int InitializeSDCard() {
  if (!SD.begin(chipSelect)) {
    return 0;
  }
  return 1;
}

bool powerUp = false;
uint32_t afterPowerUp = 0;

FLASHMEM void setup() {
  TRACE_LEVEL(TR_L_INFO);

  powerUp = true;
  Serial.begin(115200);

  if (CrashReport) {
    Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);
    Serial.println(CrashReport);
  }

  memInfo();

  TRACE_T41(TR_L_INFO, "stack available:%d", getStackAvailable());
  TRACE_T41(TR_L_INFO, "heap available:%d", getHeapAvailable());

  Serial.printf("T41 %s %s\n", GIT_TAG_VERSION, GIT_DATE);
  Serial.printf("branch:%s sha:%s\n", GIT_BRANCH, GIT_SHA);

  TRACE_T41(TR_L_INFO, "T41 %s %s", GIT_TAG_VERSION, GIT_DATE);
  TRACE_T41(TR_L_INFO, "branch:%s sha:%s", GIT_BRANCH, GIT_SHA);

  TRACE_T41(TR_L_FATAL, "Test message TR_L_FATAL");
  TRACE_T41(TR_L_ERROR, "Test message TR_L_ERROR");
  // globalTraceLevel=TR_L_FATAL;

  TRACE_T41(TR_L_WARN, "Test message TR_L_WARN");
  TRACE_T41(TR_L_INFO, "Test message TR_L_INFO");
  TRACE_T41(TR_L_DEBUG, "Test message TR_L_DEBUG");
  TRACE_T41(TR_L_TRACE, "Test message TR_L_TRACE");

  // Use this serial for FT8 keying.
  SerialUSB1.begin(115200);
  SerialUSB1.printf("T41 USB1\n");

  SerialUSB2.begin(115200);
  SerialUSB2.printf("T41 USB2\n");

  // get TIME from real time clock with 3V backup battery
  setSyncProvider(getTeensy3Time);
  setTime(now());
  // set the RTC
  Teensy3Clock.set(now());
  T4_rtc_set(Teensy3Clock.get());

  // Configure Audio Adapter
  sgtl5000_1.enable();
  // This sets to one of two possible I2C addresses, controlled by a jumper on the Audio Adapter.
  sgtl5000_1.setAddress(LOW);

  // Don't use the audio pre-processor.  This causes a spurious signal in the SSB transmit output. Need to use one of the
  // equalizers
#if 0
  sgtl5000_1.audioPreProcessorEnable();
#endif

  AudioMemory(250);
  AudioMemory_F32(100);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  // Set headphone volume.
  sgtl5000_1.volume(0.8);
  sgtl5000_1.micGain(0);
  // Line-in is not used.  Can't turn it off though.
  sgtl5000_1.lineInLevel(0);

#ifdef QSE2
  // Setting of 13 limits line-out level to 3.15 volts p-p (maximum)
  sgtl5000_1.lineOutLevel(13);
#else
  // Setting of 20 limits line-out level to 2.14 volts p-p
  sgtl5000_1.lineOutLevel(20);
#endif

  // This is required for QSE2DC, specifically for carrier calibration.reduces noise.
  // https://forum.pjrc.com/threads/27215-24-bit-audio-boards?p=78831&viewfull=1#post78831
#if 0
  // sgtl5000_1.adcHighPassFilterDisable();
#endif

  sgtl5000_1.adcHighPassFilterEnable();

  // This updates the transmit signal chain settings.  Located in SSB_Exciter.cpp.
  updateMic();

  // Updates the AGC (compressor) in the receiver.
  initializeAudioPaths();

  // Set up "Controlled Envelope Single Side Band" from the Open Audio Library.
  cessb1.setSampleRate_Hz(48000);
  // gainIn, gainCompensate, gainOut
  cessb1.setGains(3.5f, 1.4f, 0.5f);
  cessb1.setSideband(false);

  // Set to CESSB or SSB Data.  Greg KF5N August 17 2024
  cessb1.setProcessing(ConfigData.cessb);

  // Limits determined emperically.  These may need more adjustment.
  // Greg KF5N August 4, 2024.
  Q_out_L_Ex.setMaxBuffers(32);
  Q_out_R_Ex.setMaxBuffers(32);
  // Receiver audio buffer limit.
  Q_out_L.setMaxBuffers(64);

  // GPOs used to control hardware.
  pinMode(FILTERPIN15M, OUTPUT);
  pinMode(FILTERPIN20M, OUTPUT);
  pinMode(FILTERPIN40M, OUTPUT);
  pinMode(FILTERPIN80M, OUTPUT);
  pinMode(RXTX, OUTPUT);
  pinMode(MUTE, OUTPUT);

  // Keep audio junk out of the speakers/headphones until configuration is
  // complete.
  digitalWrite(MUTE, MUTEAUDIO);
  pinMode(PTT, INPUT_PULLUP);

  // Pin 39.  Switch matrix output connects to this pin.
  pinMode(BUSY_ANALOG_PIN, INPUT);

  pinMode(KEYER_DIT_INPUT_TIP, INPUT_PULLUP);
  pinMode(KEYER_DAH_INPUT_RING, INPUT_PULLUP);

  // SPI bus to display.
  pinMode(TFT_MOSI, OUTPUT);
  digitalWrite(TFT_MOSI, HIGH);
  pinMode(TFT_SCLK, OUTPUT);
  digitalWrite(TFT_SCLK, HIGH);
  // The Main board has a pull-up resistor on an output???
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);

  TRACE_T41(TR_L_INFO, "Pin setup");

  int sdCardFlag = InitializeSDCard();
  TRACE_T41(TR_L_INFO, "InitializeSDCard sdCardFlag:%d", sdCardFlag);

  bool eepromInitFlag = eeprom.EEPROMStartup();
  TRACE_T41(TR_L_INFO, "EEPROMStartup");

  LoadMapFiles(mapFilename);
  CheckMapList();

  // Encoders
  switch (ConfigData.hwVersion) {
  case HwVersion::T41_1_4SQRP:
    VOLUME_ENCODER_A = T41_1_4SQRP_VOLUME_ENCODER_A;
    VOLUME_ENCODER_B = T41_1_4SQRP_VOLUME_ENCODER_B;
    FILTER_ENCODER_A = T41_1_4SQRP_FILTER_ENCODER_A;
    FILTER_ENCODER_B = T41_1_4SQRP_FILTER_ENCODER_B;
    FINETUNE_ENCODER_A = T41_1_4SQRP_FINETUNE_ENCODER_A;
    FINETUNE_ENCODER_B = T41_1_4SQRP_FINETUNE_ENCODER_B;
    TUNE_ENCODER_A = T41_1_4SQRP_TUNE_ENCODER_A;
    TUNE_ENCODER_B = T41_1_4SQRP_TUNE_ENCODER_B;
    break;
  default:
    VOLUME_ENCODER_A = T41_1_VOLUME_ENCODER_A;
    VOLUME_ENCODER_B = T41_1_VOLUME_ENCODER_B;
    FILTER_ENCODER_A = T41_1_FILTER_ENCODER_A;
    FILTER_ENCODER_B = T41_1_FILTER_ENCODER_B;
    FINETUNE_ENCODER_A = T41_1_FINETUNE_ENCODER_A;
    FINETUNE_ENCODER_B = T41_1_FINETUNE_ENCODER_B;
    TUNE_ENCODER_A = T41_1_TUNE_ENCODER_A;
    TUNE_ENCODER_B = T41_1_TUNE_ENCODER_B;
    break;
  }

  volumeEncoder = new Rotary(VOLUME_ENCODER_A, VOLUME_ENCODER_B);
  tuneEncoder = new Rotary(TUNE_ENCODER_A, TUNE_ENCODER_B);
  filterEncoder = new Rotary(FILTER_ENCODER_A, FILTER_ENCODER_B);
  fineTuneEncoder = new Rotary(FINETUNE_ENCODER_A, FINETUNE_ENCODER_B);

  tuneEncoder->begin(true);
  volumeEncoder->begin(true);
  attachInterrupt(digitalPinToInterrupt(VOLUME_ENCODER_A), EncoderVolume, CHANGE);
  attachInterrupt(digitalPinToInterrupt(VOLUME_ENCODER_B), EncoderVolume, CHANGE);
  filterEncoder->begin(true);
  attachInterrupt(digitalPinToInterrupt(FILTER_ENCODER_A), EncoderFilter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FILTER_ENCODER_B), EncoderFilter, CHANGE);
  fineTuneEncoder->begin(true);
  attachInterrupt(digitalPinToInterrupt(FINETUNE_ENCODER_A), EncoderFineTune, CHANGE);
  attachInterrupt(digitalPinToInterrupt(FINETUNE_ENCODER_B), EncoderFineTune, CHANGE);

  attachInterrupt(digitalPinToInterrupt(KEYER_DIT_INPUT_TIP), KeyTipOn, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KEYER_DAH_INPUT_RING), KeyRingOn, CHANGE);

  // Move these to the initialization function?
  arm_fir_init_f32(&FIR_CW_DecodeL, 64, CW_Filter_Coeffs2, FIR_CW_DecodeL_state, 256);
  arm_fir_init_f32(&FIR_CW_DecodeR, 64, CW_Filter_Coeffs2, FIR_CW_DecodeR_state, 256);
  // Used in CW exciter.
  arm_fir_interpolate_init_f32(&FIR_int1_EX_I, 2, 48, coeffs48K_8K_LPF_FIR, FIR_int1_EX_I_state, 256);
  arm_fir_interpolate_init_f32(&FIR_int1_EX_Q, 2, 48, coeffs48K_8K_LPF_FIR, FIR_int1_EX_Q_state, 256);
  arm_fir_interpolate_init_f32(&FIR_int2_EX_I, 4, 32, coeffs192K_10K_LPF_FIR, FIR_int2_EX_I_state, 512);
  arm_fir_interpolate_init_f32(&FIR_int2_EX_Q, 4, 32, coeffs192K_10K_LPF_FIR, FIR_int2_EX_Q_state, 512);

  // Display interface settings
  uint32_t iospeed_display = IOMUXC_PAD_DSE(3) | IOMUXC_PAD_SPEED(1);
  // clk
  *(digital_pin_to_info_PGM + 13)->pad = iospeed_display;
  // MOSI
  *(digital_pin_to_info_PGM + 11)->pad = iospeed_display;
  *(digital_pin_to_info_PGM + TFT_CS)->pad = iospeed_display;

  // parameter list from library code
  tft.begin(RA8875_800x480, 8, 20000000UL, 4000000UL);
  tft.setRotation(0);

  // Setup for scrolling attributes. Part of initSpectrum_RA8875() call written by Mike Lewis
  // mainly used to turn on layers!
  tft.useLayers(true);
  tft.layerEffect(OR);
  tft.clearMemory();
  tft.writeTo(L2);
  tft.clearMemory();
  tft.writeTo(L1);

  ConfigData.sdCardPresent = sdCardFlag;
  if (sdCardFlag == 0) {
    tft.setFontScale((enum RA8875tsize)1);
    tft.setTextColor(RA8875_RED, RA8875_BLACK);
    tft.setCursor(100, 240);
    tft.print("SD card cannot be initialized.");
    delay(2000);
    tft.clearScreen();
  }

  // Push and hold a button at power up to activate switch matrix calibration.
#ifdef DEBUG_SWITCH_CAL
  if (analogRead(BUSY_ANALOG_PIN) < NOTHING_TO_SEE_HERE) {
    tft.fillWindow(RA8875_BLACK);
    tft.setFontScale(1);
    tft.setTextColor(RA8875_GREEN);
    tft.setCursor(10, 10);
    tft.print("Release button to start calibration.");
    delay(2000);
    button.EnableButtonInterrupts();
    SaveAnalogSwitchValues();
    eeprom.CalDataWrite();
  }
#else
  button.EnableButtonInterrupts();
  if (eepromInitFlag) {
    TRACE_T41(TR_L_TRACE, "SaveAnalogSwitchValues() begin");
    SaveAnalogSwitchValues();
    eeprom.CalDataWrite();
    TRACE_T41(TR_L_TRACE, "SaveAnalogSwitchValues() end");
  }
#endif

  //  Entry graphics
  Splash();

  //  Draw objects to the display.
  RedrawDisplayScreen();

  // start local oscillator Si5351
  si5351.reset();
  si5351.init(SI5351_CRYSTAL_LOAD_10PF, Si_5351_crystal, CalData.freqCorrectionFactor);
  // Allows CLK1 and CLK2 to exceed 100 MHz simultaneously.
  si5351.set_ms_source(SI5351_CLK2, SI5351_PLLB);
#ifdef PLLMODULE
  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
  si5351.output_enable(SI5351_CLK2, 1);
#else
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
  si5351.output_enable(SI5351_CLK1, 1);
#endif
  si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);

  InitializeDataArrays();
  InitializeTemperatureVars();

  // Initialize user defined stuff
  initUserDefinedStuff();
  // Set volume to zero at power-up.
  speakerVolume.setGain(0.0);
  headphoneVolume.setGain(0.0);
  // Adjust volume to saved value.
  volumeChangeFlag = true;
  filterEncoderMove = 0;
  fineTuneEncoderMove = 0;

  // To make sure the receiver will be configured on
  // the first pass through.  KF5N September 3, 2023
  lastState = RadioState::NOSTATE;

  // Set up the initial state/mode.
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE) {
    radioState = RadioState::CW_RECEIVE_STATE;
    bands.bands[ConfigData.currentBand].mode = RadioMode::CW_MODE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::SSB_MODE) {
    radioState = RadioState::SSB_RECEIVE_STATE;
    bands.bands[ConfigData.currentBand].mode = RadioMode::SSB_MODE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::FT8_MODE) {
    radioState = RadioState::FT8_RECEIVE_STATE;
    bands.bands[ConfigData.currentBand].mode = RadioMode::FT8_MODE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::AM_MODE) {
    radioState = RadioState::AM_RECEIVE_STATE;
    bands.bands[ConfigData.currentBand].mode = RadioMode::AM_MODE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::SAM_MODE) {
    radioState = RadioState::SAM_RECEIVE_STATE;
    bands.bands[ConfigData.currentBand].mode = RadioMode::SAM_MODE;
  }

  // Changed from middle to first. Do Menu Down to get to
  mainMenuIndex = 0;
  // Calibrate quickly

  // ButtonZoom() increments zoomIndex, so this cancels it so the read from EEPROM is accurately restored.  KF5N August 3, 2023
  zoomIndex = static_cast<int>(ConfigData.spectrum_zoom) - 1;

  // Restore zoom settings.  KF5N August 3, 2023
  button.ButtonZoom();

  // Start with lower gain so you don't get blasted.
  ConfigData.rfGainCurrent = 0;

  // Forces an update.
  lastState = RadioState::NOSTATE;

  if ((MASTER_CLK_MULT_RX == 2) or (MASTER_CLK_MULT_TX == 2)) {
    // Required only for QSD2/QSE2.
    ResetFlipFlops();
  }
}

// Automatically increases as time passes; no ++ necessary.
elapsedMicros usec = 0;

/*****
  Purpose: Code here executes forever, or until: 1) power is removed, 2) user
does a reset, 3) a component fails, or 4) the cows come home.

  Parameter list:
    void

  Return value:
    void
*****/
float audioBW{0.0};
uint32_t receiverMute = 10;
void loop() {
  MenuSelect menu;
  long ditTimerOff;
  long dahTimerOn;
  bool cwKeyDown;
  unsigned long cwBlockIndex;

  menu = readButton();
  if (menu != MenuSelect::BOGUS_PIN_READ and (radioState != RadioState::SSB_TRANSMIT_STATE) and
      (radioState != RadioState::FT8_TRANSMIT_STATE) and (radioState != RadioState::CW_TRANSMIT_STRAIGHT_STATE) and
      (radioState != RadioState::CW_TRANSMIT_KEYER_STATE)) {
    button.ExecuteButtonPress(menu);
  }

  //  State detection for modes which can transmit.  AM and SAM don't transmit, so there is not a state transition required.
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::SSB_MODE and digitalRead(PTT) == HIGH) {
    radioState = RadioState::SSB_RECEIVE_STATE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::SSB_MODE && digitalRead(PTT) == LOW) {
    radioState = RadioState::SSB_TRANSMIT_STATE;
  }

  if (bands.bands[ConfigData.currentBand].mode == RadioMode::FT8_MODE and SerialUSB1.rts() == LOW) {
    radioState = RadioState::FT8_RECEIVE_STATE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::FT8_MODE and SerialUSB1.rts() == HIGH) {
    radioState = RadioState::FT8_TRANSMIT_STATE;
  }

  if (bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE &&
      (digitalRead(ConfigData.paddleDit) == HIGH && digitalRead(ConfigData.paddleDah) == HIGH)) {
    // Was using symbolic constants.
    radioState = RadioState::CW_RECEIVE_STATE;
    // Also changed in code below. KF5N August 8, 2023
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE &&
      (digitalRead(ConfigData.paddleDit) == LOW && bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE &&
       ConfigData.keyType == KeyTypeEnum::STRAIGHT_KEY)) {
    radioState = RadioState::CW_TRANSMIT_STRAIGHT_STATE;
  }
  if (bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE &&
      (keyPressedOn == 1 && bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE &&
       ConfigData.keyType == KeyTypeEnum::PADDLES)) {
    radioState = RadioState::CW_TRANSMIT_KEYER_STATE;
  }

  if (lastState != radioState) {
    SetAudioOperatingState(radioState);
    button.ExecuteModeChange();
    // Serial.printf("Set audio state, begin loop. radioState = %d lastState = %d\n", radioState, lastState);
  }

  // Don't turn off audio in the case of CW for sidetone.
  if (powerUp and not(radioState == RadioState::CW_TRANSMIT_STRAIGHT_STATE or radioState == RadioState::CW_TRANSMIT_KEYER_STATE)) {
    afterPowerUp = afterPowerUp + 1;
    speakerScale.setGain(0);
    headphoneScale.setGain(0);
    if (afterPowerUp > receiverMute) {
      powerUp = false;
      afterPowerUp = 0;
      // Determines duration of mute.
      receiverMute = 3;
      speakerScale.setGain(ConfigData.speakerScale);
      headphoneScale.setGain(ConfigData.headphoneScale);
    }
  }
  if (radioState == RadioState::CW_TRANSMIT_STRAIGHT_STATE or radioState == RadioState::CW_TRANSMIT_KEYER_STATE) {
    speakerScale.setGain(ConfigData.speakerScale);
    headphoneScale.setGain(ConfigData.headphoneScale);
  }

  //  Begin radio state machines

  //  Begin SSB Mode state machine
  switch (radioState) {
  case RadioState::AM_RECEIVE_STATE:
  case RadioState::SAM_RECEIVE_STATE:
  case RadioState::FT8_RECEIVE_STATE:
  case RadioState::SSB_RECEIVE_STATE:
    if (lastState != radioState) {
      // xmit off
      digitalWrite(RXTX, LOW);
      ShowTransmitReceiveStatus();
    }
    ShowSpectrum();
    break;
  case RadioState::SSB_TRANSMIT_STATE:
    // xmit on
    digitalWrite(RXTX, HIGH);

    ShowTransmitReceiveStatus();
    while (digitalRead(PTT) == LOW) {
      ExciterIQData();

#ifdef DEBUG_CESSB
      struct levelsZ *cessbData;
      if (cessb1.levelDataCount() > 2000) {
        // Before getLevels(1), because it
        Serial.print("levelDataCount = ");
        // resets counts to 0.

        Serial.println(cessb1.levelDataCount());
        cessbData =
            // Update the CESSB information struct.  Write
            cessb1.getLevels(1);
        // the data to serial. Detailed Report

        Serial.print(10.0f * log10f(cessbData->pwr0));
        Serial.print(" In Ave Pwr Out ");
        Serial.println(10.0f * log10f(cessbData->pwr1));
        Serial.print(20.0f * log10f(cessbData->peak0));
        Serial.print(" In  Peak   Out ");
        Serial.println(20.0f * log10f(cessbData->peak1));
        Serial.print(cessbData->peak0, 6);
        Serial.print(" In  Peak Volts   Out ");
        Serial.println(cessbData->peak1, 6);
        Serial.print("Enhancement = ");
        float32_t enhance = (10.0f * log10f(cessbData->pwr1) - 20.0f * log10f(cessbData->peak1)) -
                            (10.0f * log10f(cessbData->pwr0) - 20.0f * log10f(cessbData->peak0));
        Serial.print(enhance);
        Serial.println(" dB");
      }
#endif
    }
    break;

  case RadioState::FT8_TRANSMIT_STATE:
    // xmit on
    digitalWrite(RXTX, HIGH);

    ShowTransmitReceiveStatus();
    while (SerialUSB1.rts() == HIGH) {
      ExciterIQData();
    }
    break;

  default:
    break;
  }
  //=== End SSB Mode ===

  // Begin CW Mode state machine
  switch (radioState) {
  case RadioState::CW_RECEIVE_STATE:
    if (lastState != radioState) {
      ShowTransmitReceiveStatus();
      keyPressedOn = 0;
    }
    // if removed CW signal on is 2 mS
    ShowSpectrum();
    break;
  case RadioState::CW_TRANSMIT_STRAIGHT_STATE:
    ShowTransmitReceiveStatus();
    // false initiates CW_SHAPING_RISE.
    cwKeyDown = false;
    cwTimer = millis();
    // Start CW transmit timer on
    while (millis() - cwTimer <= ConfigData.cwTransmitDelay) {
      digitalWrite(RXTX, HIGH);
      // Turn on CW signal
      if (digitalRead(ConfigData.paddleDit) == LOW && ConfigData.keyType == KeyTypeEnum::STRAIGHT_KEY) {
        // Reset timer
        cwTimer = millis();
        if (!cwKeyDown) {
          CW_ExciterIQData(CW_SHAPING_RISE);
          cwKeyDown = true;
        } else {
          CW_ExciterIQData(CW_SHAPING_NONE);
        }
      } else {
        // Turn off CW signal
        if (digitalRead(ConfigData.paddleDit) == HIGH && ConfigData.keyType == KeyTypeEnum::STRAIGHT_KEY) {
          keyPressedOn = 0;
          // Initiate falling CW signal.
          if (cwKeyDown) {
            CW_ExciterIQData(CW_SHAPING_FALL);
            cwKeyDown = false;
          } else {
            // No waveforms; but DC offset is still present.
            CW_ExciterIQData(CW_SHAPING_ZERO);
          }
        }
      }
    }

    // mutes audio
    // digitalWrite(MUTE, MUTEAUDIO);
    // End Straight Key Mode
    digitalWrite(RXTX, LOW);
    break;
  case RadioState::CW_TRANSMIT_KEYER_STATE:
    ShowTransmitReceiveStatus();
    cwTimer = millis();
    while (millis() - cwTimer <= ConfigData.cwTransmitDelay) {
      digitalWrite(RXTX, HIGH);

      // Keyer Dit
      if (digitalRead(ConfigData.paddleDit) == LOW) {
        ditTimerOn = millis();
        // Queue audio blocks--execution time of this loop will be between
        // 0-20ms shorter than the desired dit time, due to audio buffering.
        CW_ExciterIQData(CW_SHAPING_RISE);
        for (cwBlockIndex = 0; cwBlockIndex < transmitDitUnshapedBlocks; cwBlockIndex++) {
          CW_ExciterIQData(CW_SHAPING_NONE);
        }
        CW_ExciterIQData(CW_SHAPING_FALL);

        // Wait for calculated dit time, allowing audio blocks to be played
        while (millis() - ditTimerOn <= transmitDitLength) {
        }

        // Pause for one dit length of silence
        ditTimerOff = millis();
        while (millis() - ditTimerOff <= transmitDitLength) {
          CW_ExciterIQData(CW_SHAPING_ZERO);
        }
        cwTimer = millis();
        // Keyer DAH
      } else if (digitalRead(ConfigData.paddleDah) == LOW) {
        dahTimerOn = millis();
        // Queue audio blocks--execution time of this loop will be between
        // 0-20ms shorter than the desired dah time, due to audio buffering

        CW_ExciterIQData(CW_SHAPING_RISE);
        for (cwBlockIndex = 0; cwBlockIndex < transmitDahUnshapedBlocks; cwBlockIndex++) {
          CW_ExciterIQData(CW_SHAPING_NONE);
        }
        CW_ExciterIQData(CW_SHAPING_FALL);

        // Wait for calculated dah time, allowing audio blocks to be played

        while (millis() - dahTimerOn <= 3UL * transmitDitLength) {
        }

        // Pause for one dit length of silence

        ditTimerOff = millis();
        while (millis() - ditTimerOff <= transmitDitLength) {
          CW_ExciterIQData(CW_SHAPING_ZERO);
        }
        cwTimer = millis();
      } else {
        CW_ExciterIQData(CW_SHAPING_ZERO);
      }
      // Fix for keyer click-clack.  KF5N August 16, 2023
      keyPressedOn = 0;
      // End Relay timer
    }

    // End Straight Key Mode
    digitalWrite(RXTX, LOW);
    break;
  case RadioState::NOSTATE:
    break;
  default:
    break;
  }

  //  End radio state machine

  if (lastState != radioState) {
    lastState = radioState;
    ShowTransmitReceiveStatus();
  }

  // Used to monitor CPU temp and load factors
  if (elapsed_micros_idx_t > (SR[static_cast<size_t>(SampleRate)].rate / 960)) {
    ShowTempAndLoad();
  }

  if (volumeChangeFlag == true) {
    // Compensate for audio filter setting.
    // Nominal bandwidth is 2.8kHz.  This will be the 0 dB reference.
    // The upper and lower frequency limits are
    // bands[ConfigData.currentBand].FLoCut and
    // bands[ConfigData.currentBand].FHiCut.

    if (bands.bands[ConfigData.currentBand].mode == RadioMode::SSB_MODE or
        bands.bands[ConfigData.currentBand].mode == RadioMode::CW_MODE or
        bands.bands[ConfigData.currentBand].mode == RadioMode::FT8_MODE) {
      audioBW = bands.bands[ConfigData.currentBand].FHiCut - bands.bands[ConfigData.currentBand].FLoCut;
    } else if (bands.bands[ConfigData.currentBand].mode == RadioMode::AM_MODE or
               bands.bands[ConfigData.currentBand].mode == RadioMode::SAM_MODE) {
      audioBW = bands.bands[ConfigData.currentBand].FAMCut;
    }

    process.audioGainCompensate = 2800.0 / audioBW;

    speakerVolume.setGain(volumeLog[ConfigData.audioVolume]);
    headphoneVolume.setGain(volumeLog[ConfigData.audioVolume]);

    volumeChangeFlag = false;
    UpdateVolumeField();
  }
}
