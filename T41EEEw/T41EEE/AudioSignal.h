#pragma once

#include "SDT.h"

// Uncomment for the original T41 audio mute control.
#if 1
constexpr uint8_t UNMUTEAUDIO = LOW;
constexpr uint8_t MUTEAUDIO = HIGH;
#else
// Use this for external amp with mute LOW, unmute HIGH.
constexpr uint8_t UNMUTEAUDIO = HIGH;
constexpr uint8_t MUTEAUDIO = LOW;
#endif

enum class AudioState : int { SPEAKER, HEADPHONE, BOTH, MUTE_BOTH };

// Teensy and OpenAudio objects
// Revised by KF5N July 24, 2024
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
extern AudioEffectGain_F32 speakerVolume;
extern AudioEffectGain_F32 headphoneVolume;
extern AudioConvert_F32toI16 float2Int1; // Converts Float to Int16.  See class in AudioStream_F32.h
extern AudioSynthWaveformSine_F32 toneSSBCal1;
extern AudioSynthWaveformSine_F32 toneSSBCal2;
extern AudioMixer4_F32 mixer1;
extern AudioEffectCompressor2_F32 compressor1; // Open Audio Compressor 2
extern radioCESSB_Z_transmit_F32 cessb1;
// end Teensy and OpenAudio objects

extern AudioEffectGain_F32 speakerScale;
extern AudioEffectGain_F32 headphoneScale;

void initializeAudioPaths();
void SetAudioOperatingState(RadioState operatingState);
void controlAudioOut(AudioState audioState, bool mute);
